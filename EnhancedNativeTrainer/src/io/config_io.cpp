/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "config_io.h"
#include "keyboard.h"
#include "..\debug\debuglog.h"
#include "..\ui_support\entcolor.h"
#include <sstream>
#include "..\features\script.h"
#include "..\features\fuel.h"

// 一个全局的 Windows "基本字符串"。实际内存由 MSXML 使用的 COM 方法分配，
// 这些方法会使用 &keyconf_bstr。我们必须使用 SysFreeString() 
// 在后续使用前释放此内存，以防止内存泄漏。
BSTR keyconf_bstr;

TrainerConfig *config = NULL;

/**读取 XML 配置文件。当前文件包含键盘按键选择。*/
void read_config_file(){
	TrainerConfig *result = new TrainerConfig();

	CoInitialize(NULL);

	//读取 XML
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if(!spXMLDoc->load("Enhanced Native Trainer/ent-config.xml")){
		write_text_to_log_file("未能找到配置文件，将使用默认配置。");
		config = result; // 默认配置
	}

	// 键盘绑定
	IXMLDOMNodeListPtr nodes = spXMLDoc->selectNodes(L"//ent-config/keys/key");
	long length;
	nodes->get_length(&length);
	for(int i = 0; i < length; i++){
		IXMLDOMNode *node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap *attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char *attrib_key_func = NULL;
		char *attrib_key_value = NULL;
		bool modCtrl = false;
		bool modAlt = false;
		bool modShift = false;

		for(long j = 0; j < length_attribs; j++){
			IXMLDOMNode *attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if(wcscmp(keyconf_bstr, L"function") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_func = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if(wcscmp(keyconf_bstr, L"value") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_value = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if(wcscmp(keyconf_bstr, L"modCtrl") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modCtrl = true;
				}
			}
			else if(wcscmp(keyconf_bstr, L"modAlt") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modAlt = true;
				}
			}
			else if(wcscmp(keyconf_bstr, L"modShift") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modShift = true;
				}
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		if(attrib_key_func != NULL && attrib_key_value != NULL){
			result->get_key_config()->set_key(attrib_key_func, attrib_key_value, modCtrl, modAlt, modShift);
		}

		delete attrib_key_func;
		delete attrib_key_value;

		attribs->Release();
		node->Release();
	}

	// 控制器绑定
	nodes = spXMLDoc->selectNodes(L"//ent-config/controller_keys/controller");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_controller_func = NULL;
		int attrib_button1_v = NULL;
		int attrib_button2_v = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"function") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_controller_func = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if (wcscmp(keyconf_bstr, L"button1") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_button1_v = std::stoi(_com_util::ConvertBSTRToString(V_BSTR(&var)));
			}
			else if (wcscmp(keyconf_bstr, L"button2") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_button2_v = std::stoi(_com_util::ConvertBSTRToString(V_BSTR(&var)));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		// 必须有一段代码用于将按键绑定存储到某个地方。
		if (attrib_controller_func != NULL) {
 
			if (attrib_button1_v == NULL)
			{
				std::stringstream ss;
				ss << "[错误] 读取时出现问题 " << attrib_controller_func << "的函数，Button1 的值为空 (NULL) 已跳过，将使用默认值替代。";
				write_text_to_log_file(ss.str());
				continue;
			}

			if (controller_binds.find(attrib_controller_func) != controller_binds.end())
			{
				std::stringstream ss;
				ss << "控制器函数 " << attrib_controller_func << " 指定值: " << attrib_button1_v << " 以及 " << attrib_button2_v;
				write_text_to_log_file(ss.str());
				controller_binds.at(attrib_controller_func) = std::pair(attrib_button1_v, attrib_button2_v);
			}
			else 
			{
				std::stringstream ss;
				ss << "[错误] 未找到控制器函数 " << attrib_controller_func << " 在控制器绑定映射中，跳过。";
				write_text_to_log_file(ss.str());
				continue;
			}
		}
		else
		{
			std::stringstream ss;
			ss << "[错误] 控制器函数与按钮 ID " << attrib_button1_v << " 以及 " << attrib_button2_v << " 值为  NULL  跳过。";
			write_text_to_log_file(ss.str());
			continue;
		}

		delete attrib_controller_func;
	
		attribs->Release();
		node->Release();
	}

	// 额外的映射相关功能列表
	nodes = spXMLDoc->selectNodes(L"//ent-config/map_stuff_list/stuff");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* map_stuff = NULL;
		
		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"sname") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				map_stuff = _com_util::ConvertBSTRToString(V_BSTR(&var));
				MAP_STUFF = MAP_STUFF + map_stuff + " ";
			}
			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete map_stuff;
		
		attribs->Release();
		node->Release();
	}

	// 天气列表
	nodes = spXMLDoc->selectNodes(L"//ent-config/weather_list/weather");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_weather = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"name") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_weather = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if (i == 0) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 1) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 2) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 3) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 4) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 5) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 6) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 7) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 8) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 9) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 10) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 11) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 12) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 13) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 14) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete attrib_weather;

		attribs->Release();
		node->Release();
	}

	// 加油站
	nodes = spXMLDoc->selectNodes(L"//ent-config/gas_stations/station");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_station_x_coord = NULL;
		char* attrib_station_y_coord = NULL;
		char* attrib_station_z_coord = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"x_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_x_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_X.push_back(std::stoi(attrib_station_x_coord));
			}
			else if (wcscmp(keyconf_bstr, L"y_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_y_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_Y.push_back(std::stoi(attrib_station_y_coord));
			}
			else if (wcscmp(keyconf_bstr, L"z_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_z_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_Z.push_back(std::stoi(attrib_station_z_coord));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete attrib_station_x_coord;
		delete attrib_station_y_coord;
		delete attrib_station_z_coord;

		attribs->Release();
		node->Release();
	}

	// nodes->Release(); // 不要调用此方法，它会在退出时导致崩溃
	spXMLDoc.Release();
	CoUninitialize();

	config = result;
}

void read_config_ini_file(){
	int tmpv[12][4];
	const char *sectionMenuColor = "菜单颜色", *file = "Enhanced Native Trainer/ent_customization.ini";
	const std::string tmpk[] = {"r", "g", "b", "a"};
	std::ifstream tmp(file);
	bool notexist = !((bool) tmp);
	tmp.close();

	if(notexist){
		write_text_to_log_file("颜色配置 INI 文件不存在！\n正在使用默认配置 !\n在游戏内保存时将创建 INI 配置文件 !  ");
		return;
	}

	for(int a = 0; a < ENTColor::colsVarsNum; a++){
		for(int b = 0; b < 4; b++){
			tmpv[a][b] = GetPrivateProfileInt(sectionMenuColor, (ENTColor::colsVarsReverse.at(a) + tmpk[b]).c_str(), -1, file);
		}
		if(!(tmpv[a][0] < 0 || tmpv[a][1] < 0 || tmpv[a][2] < 0 || tmpv[a][3] < 0) || !(tmpv[a][0] > 255 || tmpv[a][1] > 255 || tmpv[a][2] > 255 || tmpv[a][3] > 255)){
			for(int b = 0; b < 4; b++){
				ENTColor::colsMenu[a].rgba[b] = tmpv[a][b];
			}
		}
	}
}

void write_config_ini_file(){
	const char *sectionMenuColor = "菜单颜色", *file = "Enhanced Native Trainer/ent_customization.ini";
	const std::string tmpk[] = {"r", "g", "b", "a"};
	std::ofstream ini;
	std::ifstream tmp(file);
	bool notexist = !((bool) tmp);
	tmp.close();

	for(int a = 0; a < ENTColor::colsVarsNum; a++){
		for(int b = 0; b < 4; b++){
			WritePrivateProfileString(sectionMenuColor, (ENTColor::colsVarsReverse.at(a) + tmpk[b]).c_str(), std::to_string(ENTColor::colsMenu[a].rgba[b]).c_str(), file);
		}
	}

	if(notexist){
		tmp.open(file);
		if(tmp.is_open()){
			std::vector<std::string> lines, result;
			for(std::string line; std::getline(tmp, line); ){
				lines.push_back(line);
			}
			tmp.close();

			result.push_back(std::string(";;;; 增强型 原生修改器：配置 INI 文件（开始） ;;;;\n\n;;; （删除此文件以恢复默认颜色配置） ;;;\n"));

			result.push_back(std::string(";; 菜单颜色（开始） ;;\n;\t请遵循 RGBA 颜色系统，每个颜色组件的值范围为 0 ~ 255 ;"));
			for(auto a : lines){
				for(int b = 0; b < ENTColor::colsVarsNum; b++){
					if(a.compare(0, ENTColor::colsVarsReverse.at(b).length() + 1, (ENTColor::colsVarsReverse.at(b) + tmpk[0])) == 0){
						result.push_back(std::string("; ") + ENTColor::colsCaptions[b] + std::string(" ;"));
						break;
					}
				}
				result.push_back(a);
			}
			result.push_back(std::string(";; 菜单颜色（结束） ;;"));

			result.push_back(std::string("\n;;;; 增强型 原生修改器：配置 INI 文件（结束） ;;;;"));

			ini.open(file, std::ofstream::out | std::ofstream::trunc);
			if(ini.is_open()){
				for(auto a : result){
					ini << a << std::endl;
				}

				ini.close();
			}
		}
	}
}

void KeyInputConfig::set_key(char* function, char* keyName, bool modCtrl, bool modAlt, bool modShift){
	std::ostringstream ss;
	ss << "按键功能 " << function << " 被给予 " << keyName;
	write_text_to_log_file(ss.str());

	int vkID = keyNameToVal(keyName);
	if(vkID == -1){
		ss.str(""); ss.clear();
		ss << "按键功能 " << keyName << " 没有对应到一个值！";
		write_text_to_log_file(ss.str());
		return;
	}

	auto match = keyConfigs.find(function);
	if(match != keyConfigs.end()){
		KeyConfig* oldConfig = match->second;
		match->second = new KeyConfig(vkID);
		match->second->modCtrl = modCtrl;
		match->second->modAlt = modAlt;
		match->second->modShift = modShift;
		delete oldConfig;
	}
	else{
		ss.str(""); ss.clear();
		ss << "按键功能 " << function << " 没有对应到已知函数！";
		write_text_to_log_file(ss.str());
	}
};

bool KeyInputConfig::is_hotkey_assigned(int i){

	std::string target;
	switch(i){
		case 1:
			target = KeyConfig::KEY_HOT_1;
			break;
		case 2:
			target = KeyConfig::KEY_HOT_2;
			break;
		case 3:
			target = KeyConfig::KEY_HOT_3;
			break;
		case 4:
			target = KeyConfig::KEY_HOT_4;
			break;
		case 5:
			target = KeyConfig::KEY_HOT_5;
			break;
		case 6:
			target = KeyConfig::KEY_HOT_6;
			break;
		case 7:
			target = KeyConfig::KEY_HOT_7;
			break;
		case 8:
			target = KeyConfig::KEY_HOT_8;
			break;
		case 9:
			target = KeyConfig::KEY_HOT_9;
			break;
		default:
			return false;
	}

	KeyConfig* conf = this->keyConfigs[target];
	if(conf == NULL || conf->keyCode == 0){
		return false;
	}
	return true;
}

TrainerConfig::TrainerConfig(){
	this->keyConfig = new KeyInputConfig();
}

KeyInputConfig::KeyInputConfig(){
	this->keyConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU] = new KeyConfig(VK_F4);

	this->keyConfigs[KeyConfig::KEY_MENU_UP] = new KeyConfig(VK_NUMPAD8);
	this->keyConfigs[KeyConfig::KEY_MENU_DOWN] = new KeyConfig(VK_NUMPAD2);
	this->keyConfigs[KeyConfig::KEY_MENU_LEFT] = new KeyConfig(VK_NUMPAD4);
	this->keyConfigs[KeyConfig::KEY_MENU_RIGHT] = new KeyConfig(VK_NUMPAD6);
	this->keyConfigs[KeyConfig::KEY_MENU_SELECT] = new KeyConfig(VK_NUMPAD5);
	this->keyConfigs[KeyConfig::KEY_MENU_BACK] = new KeyConfig(VK_NUMPAD0);

	this->keyConfigs[KeyConfig::KEY_VEH_BOOST] = new KeyConfig(VK_NUMPAD9);
	this->keyConfigs[KeyConfig::KEY_VEH_STOP] = new KeyConfig(VK_NUMPAD3);
	this->keyConfigs[KeyConfig::KEY_VEH_ROCKETS] = new KeyConfig(VK_ADD);
	this->keyConfigs[KeyConfig::KEY_VEH_LEFTBLINK] = new KeyConfig(VK_LEFT);
	this->keyConfigs[KeyConfig::KEY_VEH_RIGHTBLINK] = new KeyConfig(VK_RIGHT);
	this->keyConfigs[KeyConfig::KEY_VEH_EMERGENCYBLINK] = new KeyConfig(VK_DECIMAL);
	this->keyConfigs[KeyConfig::KEY_VEH_STARTREFUELING] = new KeyConfig(VK_KEY_E);
	this->keyConfigs[KeyConfig::KEY_VEH_STOPREFUELING] = new KeyConfig(VK_KEY_S);
	
	this->keyConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE] = new KeyConfig(VK_F6);

	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_UP] = new KeyConfig(VK_KEY_Q);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_DOWN] = new KeyConfig(VK_KEY_Z);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FORWARD] = new KeyConfig(VK_KEY_W);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_BACK] = new KeyConfig(VK_KEY_S);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_SPEED] = new KeyConfig(VK_SHIFT);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_HELP] = new KeyConfig(VK_KEY_H);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_SPACE] = new KeyConfig(VK_SPACE);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL] = new KeyConfig(VK_KEY_M);

	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_UP] = new KeyConfig(VK_KEY_Q);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_DOWN] = new KeyConfig(VK_KEY_Z);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FORWARD] = new KeyConfig(VK_KEY_W);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_BACK] = new KeyConfig(VK_KEY_S);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_UP] = new KeyConfig(VK_OEM_6);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN] = new KeyConfig(VK_OEM_4);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_HELP] = new KeyConfig(VK_KEY_H);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION] = new KeyConfig(VK_KEY_G);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ALT_MOVE] = new KeyConfig(VK_MENU);

	this->keyConfigs[KeyConfig::KEY_HOT_1] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_2] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_3] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_4] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_5] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_6] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_7] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_8] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_9] = new KeyConfig(0);

}

KeyInputConfig::~KeyInputConfig(){
	for each (std::pair<std::string, KeyConfig*> conf in this->keyConfigs){
		delete conf.second;
	}
	keyConfigs.clear();
}

KeyConfig* KeyInputConfig::get_key(std::string function){
	auto match = keyConfigs.find(function);
	if(match != keyConfigs.end()){
		return match->second;
	}
	return NULL;
}

const std::string KeyConfig::KEY_TOGGLE_MAIN_MENU = std::string("toggle_main_menu");
const std::string KeyConfig::KEY_TOGGLE_AIRBRAKE = std::string("toggle_airbrake");

const std::string KeyConfig::KEY_MENU_UP = std::string("menu_up");
const std::string KeyConfig::KEY_MENU_DOWN = std::string("menu_down");
const std::string KeyConfig::KEY_MENU_LEFT = std::string("menu_left");
const std::string KeyConfig::KEY_MENU_RIGHT = std::string("menu_right");
const std::string KeyConfig::KEY_MENU_SELECT = std::string("menu_select");
const std::string KeyConfig::KEY_MENU_BACK = std::string("menu_back");

const std::string KeyConfig::KEY_VEH_BOOST = std::string("veh_boost");
const std::string KeyConfig::KEY_VEH_STOP = std::string("veh_stop");
const std::string KeyConfig::KEY_VEH_ROCKETS = std::string("veh_rockets");
const std::string KeyConfig::KEY_VEH_LEFTBLINK = std::string("veh_leftblink");
const std::string KeyConfig::KEY_VEH_RIGHTBLINK = std::string("veh_rightblink");
const std::string KeyConfig::KEY_VEH_EMERGENCYBLINK = std::string("veh_emergencyblink");
const std::string KeyConfig::KEY_VEH_STARTREFUELING = std::string("veh_startrefueling");
const std::string KeyConfig::KEY_VEH_STOPREFUELING = std::string("veh_stoprefueling");

const std::string KeyConfig::KEY_AIRBRAKE_UP = std::string("airbrake_up");
const std::string KeyConfig::KEY_AIRBRAKE_DOWN = std::string("airbrake_down");
const std::string KeyConfig::KEY_AIRBRAKE_FORWARD = std::string("airbrake_forward");
const std::string KeyConfig::KEY_AIRBRAKE_BACK = std::string("airbrake_back");
const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT = std::string("airbrake_rotate_left");
const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT = std::string("airbrake_rotate_right");
const std::string KeyConfig::KEY_AIRBRAKE_SPEED = std::string("airbrake_speed");
const std::string KeyConfig::KEY_AIRBRAKE_FREEZE_TIME = std::string("airbrake_freeze_time");
const std::string KeyConfig::KEY_AIRBRAKE_HELP = std::string("airbrake_help");
const std::string KeyConfig::KEY_AIRBRAKE_SPACE = std::string("airbrake_move_aside");
const std::string KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL = std::string("airbrake_mouse_control");

const std::string KeyConfig::KEY_OBJECTPLACER_UP = std::string("objectplacer_up");
const std::string KeyConfig::KEY_OBJECTPLACER_DOWN = std::string("objectplacer_down");
const std::string KeyConfig::KEY_OBJECTPLACER_FORWARD = std::string("objectplacer_forward");
const std::string KeyConfig::KEY_OBJECTPLACER_BACK = std::string("objectplacer_back");
const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT = std::string("objectplacer_rotate_left");
const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT = std::string("objectplacer_rotate_right");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE = std::string("objectplacer_speed_cycle");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_UP = std::string("objectplacer_speed_up");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN = std::string("objectplacer_speed_down");
const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME = std::string("objectplacer_freeze_time");
const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION = std::string("objectplacer_freeze_position");
const std::string KeyConfig::KEY_OBJECTPLACER_HELP = std::string("objectplacer_help");
const std::string KeyConfig::KEY_OBJECTPLACER_ALT_MOVE = std::string("objectplacer_alt_move");

const std::string KeyConfig::KEY_HOT_1 = std::string("hotkey_1");
const std::string KeyConfig::KEY_HOT_2 = std::string("hotkey_2");
const std::string KeyConfig::KEY_HOT_3 = std::string("hotkey_3");
const std::string KeyConfig::KEY_HOT_4 = std::string("hotkey_4");
const std::string KeyConfig::KEY_HOT_5 = std::string("hotkey_5");
const std::string KeyConfig::KEY_HOT_6 = std::string("hotkey_6");
const std::string KeyConfig::KEY_HOT_7 = std::string("hotkey_7");
const std::string KeyConfig::KEY_HOT_8 = std::string("hotkey_8");
const std::string KeyConfig::KEY_HOT_9 = std::string("hotkey_9");

// 绑定名称 -> 按钮 ID 1 和按钮 ID 2。对于只有一个按钮 ID 的键 - 使用 -1 作为“无绑定”值。
std::map<std::string, std::pair<int, int>> controller_binds =
{
	{ "KEY_TOGGLE_MAIN_MENU", {206, 192} },
	{ "KEY_TOGGLE_AIRBRAKE", {201, 206} },
	{ "KEY_MENU_UP", {188, -1} },
	{ "KEY_MENU_DOWN", {187, -1} },
	{ "KEY_MENU_LEFT", {189, -1} },
	{ "KEY_MENU_RIGHT", {190, -1} },
	{ "KEY_MENU_SELECT", {201, 1} },
	{ "KEY_MENU_BACK", {202, -1} },
	{ "KEY_VEH_BOOST", {206, 24} },
	{ "KEY_VEH_STOP", {205, 25} },
	{ "KEY_VEH_ROCKETS", {205, 206} },
	{ "KEY_VEH_LEFTBLINK", {25, -1} },
	{ "KEY_VEH_RIGHTBLINK", {24, -1} },
	{ "KEY_VEH_EMERGENCYBLINK", {202, -1} },
	{ "KEY_VEH_STARTREFUELING", {201, -1} },
	{ "KEY_VEH_STOPREFUELING", {202, -1} },
	{ "KEY_AIRBRAKE_UP", {25, -1} },
	{ "KEY_AIRBRAKE_DOWN", {24, -1} },
	{ "KEY_AIRBRAKE_FORWARD", {32, -1} },
	{ "KEY_AIRBRAKE_BACK", {33, -1} },
	{ "KEY_AIRBRAKE_ROTATE_LEFT", {205, -1} },
	{ "KEY_AIRBRAKE_ROTATE_RIGHT", {206, -1} },
	{ "KEY_AIRBRAKE_SPEED", {201, -1} },
	{ "KEY_AIRBRAKE_FREEZE_TIME", {202, -1} },
	{ "KEY_AIRBRAKE_HELP", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_AIRBRAKE_SPACE", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_AIRBRAKE_MOUSE_CONTROL", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_UP", {25, -1} },
	{ "KEY_OBJECTPLACER_DOWN", {24, -1} },
	{ "KEY_OBJECTPLACER_FORWARD", {32, -1} },
	{ "KEY_OBJECTPLACER_BACK", {33, -1} },
	{ "KEY_OBJECTPLACER_ROTATE_LEFT", {205, -1} },
	{ "KEY_OBJECTPLACER_ROTATE_RIGHT", {206, -1} },
	{ "KEY_OBJECTPLACER_SPEED_CYCLE", {201, -1} },
	{ "KEY_OBJECTPLACER_SPEED_UP", {-1, -1} },  // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_SPEED_DOWN", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_FREEZE_TIME", {202, -1} },
	{ "KEY_OBJECTPLACER_FREEZE_POSITION", {192, -1} },
	{ "KEY_OBJECTPLACER_HELP", {-1, -1} },  // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_ALT_MOVE", {-1, -1} }, // XML 中没有绑定吗？
};