/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "script.h"

int whichpart = 0;
int whichtype = 0;
PaintIndexItem<int> *parentIndexItem = NULL;

// 保存/加载车辆颜色
int activeSavedVehColourIndex = -1;
std::string activeSavedVehColourSlotName;
int lastKnownSavedVehColourCount = 0;
bool vehcolourSaveMenuInterrupt = false;
bool vehcolourSaveSlotMenuInterrupt = false;
bool requireRefreshOfVehColourSaveSlots = false;
bool requireRefreshOfVehColourSlotMenu = false;

// 零件
const std::vector<std::string> MENU_PAINT_WHAT{ "主色调", "副色调", "主色调/副色调", "珠光色", "轮毂颜色", "内饰颜色", "仪表盘颜色" };

// 油漆名称
const std::vector<std::string> MENU_PAINT_TYPE{ "经典", "金属", "哑光", "金属质感", "工业", "磨损", "铬合金" };
const std::string MENU_PAINT_INDEX_TYPE( "颜色索引" );
const std::string MENU_CUSTOM_PAINT_TYPE( "自定义颜色" );

const std::vector<PaintColor> PAINTS_CLASSIC{
	{0, "黑色"},
	{1, "石墨色"},
	{2, "黑钢色"},
	{3, "暗钢色"},
	{4, "银色"},
	{5, "蓝银色"},
	{6, "轧钢色"},
	{7, "暗影银色"},
	{8, "石银色"},
	{9, "午夜银色"},
	{10, "铸铁银色"},
	{11, "炭黑灰色"},
	{27, "红色"},
	{28, "都灵红色"},
	{29, "方程式红色"},
	{30, "烈焰红色"},
	{31, "优雅红色"},
	{32, "石榴红色"},
	{33, "日落红色"},
	{34, "赤霞红色"},
	{35, "糖果红色"},
	{36, "日出橙色"},
	{37, "金色"},
	{38, "橙色"},
	{49, "深绿色"},
	{50, "赛车绿色"},
	{51, "海绿色"},
	{52, "橄榄绿色"},
	{53, "亮绿色"},
	{54, "汽油绿色"},
	{61, "银河蓝色"},
	{62, "深蓝色"},
	{63, "撒克逊蓝色"},
	{64, "蓝色"},
	{65, "水手蓝色"},
	{66, "港口蓝色"},
	{67, "钻石蓝色"},
	{68, "浪花蓝色"},
	{69, "航海蓝色"},
	{70, "超炫蓝色"},
	{71, "莎夫特紫色"},
	{72, "帆船紫色"},
	{73, "赛车蓝色"},
	{74, "浅蓝色"},
	{88, "黄色"},
	{89, "竞速黄色"},
	{90, "青铜色"},
	{91, "露珠黄色"},
	{92, "柠檬绿色"},
	{93, "香槟色"},
	{94, "费尔泽棕色"},
	{95, "溪棕色"},
	{96, "巧克力棕色"},
	{97, "枫棕色"},
	{98, "鞍棕色"},
	{99, "稻草棕色"},
	{100, "苔藓棕色"},
	{101, "野牛棕色"},
	{102, "山毛榉木棕色"},
	{103, "榉木棕色"},
	{104, "赭石棕色"},
	{105, "沙棕色"},
	{106, "漂白棕色"},
	{107, "奶油色"},
	{111, "冰白色"},
	{112, "霜白色"},
	{125, "保安绿色"},
	{127, "警车蓝色"},
	{134, "纯白色"},
	{135, "热粉色"},
	{136, "鲑鱼粉色"},
	{137, "菲斯特粉色"},
	{138, "亮橙色"},
	{139, "绿色"},
	{140, "荧光蓝色"},
	{141, "午夜蓝色"},
	{142, "黑紫色"},
	{143, "酒红色"},
	{144, "猎人绿色"},
	{145, "亮紫色"},
	{146, "午夜紫色"},
	{147, "碳黑色"},
	{150, "熔岩红色"},
	{156, "合金色"},
	{157, "伊普西隆蓝色"},
};

const std::vector<PaintColor> PAINTS_MATTE{
	{12, "哑光黑色"},
	{13, "哑光灰色"},
	{14, "哑光浅灰色"},
	{39, "哑光红色"},
	{40, "哑光深红色"},
	{41, "哑光橙色"},
	{42, "哑光黄色"},
	{55, "哑光柠檬绿色"},
	{82, "哑光深蓝色"},
	{83, "哑光蓝色"},
	{84, "哑光午夜蓝色"},
	{128, "哑光绿色"},
	{129, "哑光棕色"},
	{131, "哑光冰白色"},
	{148, "哑光莎夫特紫色"},
	{149, "哑光午夜紫色"},
	{151, "哑光森林绿色"},
	{152, "哑光橄榄绿色"},
	{153, "哑光暗土色"},
	{154, "哑光沙漠棕色"},
	{155, "哑光叶绿色"},
};

const std::vector<PaintColor> PAINTS_METALS{
	{117, "拉丝钢色"},
	{118, "拉丝黑钢色"},
	{119, "拉丝铝色"},
	{158, "纯金色"},
	{159, "拉丝金色"},
	{160, "抛光金色"},
};

const std::vector<PaintColor> PAINTS_UTIL{
	{15, "工业黑色"},
	{16, "工业黑聚酯"},
	{17, "工业深银色"},
	{18, "工业银色"},
	{19, "工业枪金属色"},
	{20, "工业暗影银色"},
	{43, "工业红色"},
	{44, "工业亮红色"},
	{45, "工业石榴红色"},
	{56, "工业深绿色"},
	{57, "工业绿色"},
	{75, "工业深蓝色"},
	{76, "工业午夜蓝色"},
	{77, "工业蓝色"},
	{78, "工业海沫蓝色"},
	{79, "工业闪电蓝色"},
	{80, "工业毛伊蓝色"},
	{81, "工业亮蓝色"},
	{108, "工业棕色"},
	{109, "工业中棕色"},
	{110, "工业浅棕色"},
	{122, "工业米白色"},
};

const std::vector<PaintColor> PAINTS_WORN{
	{21, "磨损黑色"},
	{22, "磨损石墨色"},
	{23, "磨损银灰色"},
	{24, "磨损银色"},
	{25, "磨损蓝银色"},
	{26, "磨损暗影银色"},
	{46, "磨损红色"},
	{47, "磨损金红色"},
	{48, "磨损深红色"},
	{58, "磨损深绿色"},
	{59, "磨损绿色"},
	{60, "磨损海浪色"},
	{85, "磨损深蓝色"},
	{86, "磨损蓝色"},
	{87, "磨损浅蓝色"},
	{113, "磨损蜜棕色"},
	{114, "磨损棕色"},
	{115, "磨损深棕色"},
	{116, "磨损稻草棕色"},
	{121, "磨损米白色"},
	{123, "磨损橙色"},
	{124, "磨损浅橙色"},
	{126, "磨损士黄色"},
	{130, "磨损淡橙色"},
	{132, "磨损白色"},
	{133, "磨损橄榄军绿色"},
};

const std::vector<PaintColor> PAINTS_CHROME{
	{120, "铬合金"}
};

const std::vector<PaintColor> PAINTS_BY_TYPE[7]{
	PAINTS_CLASSIC,
	PAINTS_CLASSIC,
	PAINTS_MATTE,
	PAINTS_METALS,
	PAINTS_UTIL,
	PAINTS_WORN,
	PAINTS_CHROME
};

bool onconfirm_paintdirt(MenuItem<float> choice){
	return true;
}

void onhighlight_paintdirt(MenuItem<float> choice){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, choice.value);
}

bool onconfirm_paintfade(MenuItem<float> choice){
	return true;
}

void onhighlight_paintfade(MenuItem<float> choice){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, choice.value);
}

bool process_paint_menu_fades(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	set_status_text("漆面褪色效果，仅适用于特定类型的车辆！");

	std::vector<MenuItem<float>*> menuItems;

	MenuItem<float> *item = new MenuItem<float>();
	item->caption = "全新漆面";
	item->value = 0.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	for(int i = 20; i < 100; i += 20){
		MenuItem<float> *item = new MenuItem<float>();
		std::ostringstream ss;
		ss << i << "% 褪色";
		item->caption = ss.str();
		item->value = (float) i / 100;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	item = new MenuItem<float>();
	item->caption = "完全褪色";
	item->value = 1.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<float>(menuItems, 0, "漆面褪色", onconfirm_paintfade, onhighlight_paintfade, NULL, vehicle_menu_interrupt);
}

bool process_paint_menu_dirt(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	std::vector<MenuItem<float>*> menuItems;

	MenuItem<float> *item = new MenuItem<float>();
	item->caption = "清洁车辆";
	item->value = 0.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	for(int i = 20; i < 100; i += 20){
		MenuItem<float> *item = new MenuItem<float>();
		std::ostringstream ss;
		ss << i << "% 肮脏";
		item->caption = ss.str();
		item->value = ((float) i / 100) * 15.0f;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	item = new MenuItem<float>();
	item->caption = "完全肮脏";
	item->value = 14.89999f;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<float>(menuItems, 0, "污垢等级", onconfirm_paintdirt, onhighlight_paintdirt, NULL, vehicle_menu_interrupt);
}

void onhighlight_livery(MenuItem<int> choice){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_LIVERY(veh, choice.value);

	//VEHICLE::SET_VEHICLE_MOD(veh, 48, choice.value, 0); //vehicle, modType (48 liv), mod index, bool customTires)
}

bool onconfirm_livery(MenuItem<int> choice){
	return true;
}

bool process_paint_menu_liveries(){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	int count = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);
	//int livCount = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 48); //48 is the livery index

	/*if (count <= -2) //for some reason, one of the liveries for the halloween car is -1
	{
		set_status_text("No liveries for this vehicle");
	}*/

	std::vector<MenuItem<int>*> menuItems;

	for(int i = 0; i < count; i++) // 存活计数
	{
		std::string modItemNameStr;

		char* modItemNameChr = VEHICLE::GET_LIVERY_NAME(veh, i);
		if(modItemNameChr == NULL){
			std::ostringstream ss;
			ss << "编号 #" << (i + 1);
			modItemNameStr = ss.str();
		}
		else{
			char* modItemNameTxt = UI::_GET_LABEL_TEXT(modItemNameChr);
			if(modItemNameTxt != NULL){
				modItemNameStr = std::string(modItemNameTxt);
			}
			else{
				std::ostringstream ss;
				ss << "编号 #" << (i + 1);
				modItemNameStr = ss.str();
			}
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = modItemNameStr;
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	int currentSelection = VEHICLE::GET_VEHICLE_LIVERY(veh);
	//int currentSelection = VEHICLE::GET_VEHICLE_MOD(veh, 48);
	return draw_generic_menu<int>(menuItems, &currentSelection, "涂装", onconfirm_livery, onhighlight_livery, NULL, vehicle_menu_interrupt);
}

// 保存/加载车辆颜色
bool veh_colour_menu_interrupt() {
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) return true;
	
	if (vehcolourSaveMenuInterrupt)
	{
		vehcolourSaveMenuInterrupt = false;
		return true;
	}

	return false;
}

bool veh_colour_save_slot_menu_interrupt()
{
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) return true;

	if (vehcolourSaveSlotMenuInterrupt)
	{
		vehcolourSaveSlotMenuInterrupt = false;
		return true;
	}
	return false;
}

bool spawn_saved_veh_colour(int slot, std::string caption)
{
	ENTDatabase* database = get_database();

	std::vector<SavedVehColourDBRow*> savedVehColours = database->get_saved_veh_colours(slot);

	SavedVehColourDBRow* savedVehColour = savedVehColours.at(0);
	
	VEHICLE::SET_VEHICLE_COLOURS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1), savedVehColour->pcolour, savedVehColour->scolour);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1), savedVehColour->pearl, savedVehColour->wheel);
	VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1), savedVehColour->pcustomr, savedVehColour->pcustomg, savedVehColour->pcustomb);
	VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1), savedVehColour->scustomr, savedVehColour->scustomg, savedVehColour->scustomb);

	for (std::vector<SavedVehColourDBRow*>::iterator it = savedVehColours.begin(); it != savedVehColours.end(); ++it)
	{
		delete (*it);
	}
	savedVehColours.clear();

	return false;
}

void save_current_veh_colour(int slot)
{
	std::ostringstream ss;
	if (slot != -1)
	{
		ss << activeSavedVehColourSlotName;
	}
	else
	{
		ss << "新建颜色存档 " << (lastKnownSavedVehColourCount + 1);
	}

	keyboard_on_screen_already = true;
	curr_message = "输入保存的颜色名称"; // 保存车辆颜色
	auto existingText = ss.str();
	std::string result = show_keyboard("手动输入名称", (char*)existingText.c_str());
	if (!result.empty())
	{
		ENTDatabase* database = get_database();

		if (database->save_veh_colour(PLAYER::PLAYER_PED_ID(), result, slot))
		{
			activeSavedVehColourSlotName = result;
			set_status_text("颜色保存成功了！");
		}
		else
		{
			set_status_text("颜色保存失败了！");
		}
	}
}

bool onconfirm_veh_savedcolour_slot_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 1: //生成
		spawn_saved_veh_colour(activeSavedVehColourIndex, activeSavedVehColourSlotName);
		break;
	case 2: //覆盖
	{
		save_current_veh_colour(activeSavedVehColourIndex);
		requireRefreshOfVehColourSaveSlots = true;
		requireRefreshOfVehColourSlotMenu = true;
		vehcolourSaveSlotMenuInterrupt = true;
		vehcolourSaveMenuInterrupt = true;
	}
	break;
	case 3: //重命名
	{
		keyboard_on_screen_already = true;
		curr_message = "输入新的名称："; // 重命名已保存的车辆颜色
		std::string result = show_keyboard("手动输入名称", (char*)activeSavedVehColourSlotName.c_str());
		if (!result.empty())
		{
			ENTDatabase* database = get_database();
			database->rename_saved_veh_colour(result, activeSavedVehColourIndex);
			activeSavedVehColourSlotName = result;
		}
		requireRefreshOfVehColourSaveSlots = true;
		requireRefreshOfVehColourSlotMenu = true;
		vehcolourSaveSlotMenuInterrupt = true;
		vehcolourSaveMenuInterrupt = true;
	}
	break;
	case 4: //删除
	{
		ENTDatabase* database = get_database();
		database->delete_saved_veh_colour(activeSavedVehColourIndex);
		requireRefreshOfVehColourSlotMenu = false;
		requireRefreshOfVehColourSaveSlots = true;
		vehcolourSaveSlotMenuInterrupt = true;
		vehcolourSaveMenuInterrupt = true;
	}
	break;
	}
	return false;
}

bool process_veh_savedcolour_slot_menu(int slot)
{
	do
	{
		vehcolourSaveSlotMenuInterrupt = false;
		requireRefreshOfVehColourSlotMenu = false;

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 1;
		item->caption = "应用";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 2;
		item->caption = "用当前内容覆盖";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 3;
		item->caption = "重命名";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 4;
		item->caption = "删除";
		menuItems.push_back(item);

		draw_generic_menu<int>(menuItems, 0, activeSavedVehColourSlotName, onconfirm_veh_savedcolour_slot_menu, NULL, NULL, veh_colour_save_slot_menu_interrupt);
	} while (requireRefreshOfVehColourSlotMenu);
	return false;
}

bool onconfirm_veh_savedcolor_menu(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		save_current_veh_colour(-1);
		requireRefreshOfVehColourSaveSlots = true;
		vehcolourSaveMenuInterrupt = true;
		return false;
	}

	activeSavedVehColourIndex = choice.value;
	activeSavedVehColourSlotName = choice.caption;
	return process_veh_savedcolour_slot_menu(choice.value);
}

bool process_veh_savedcolour_menu()
{
	do
	{
		vehcolourSaveMenuInterrupt = false;
		requireRefreshOfVehColourSlotMenu = false;
		requireRefreshOfVehColourSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedVehColourDBRow*> savedVehColours = database->get_saved_veh_colours();

		lastKnownSavedVehColourCount = savedVehColours.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = -1;
		item->caption = "创建新的颜色存档";
		menuItems.push_back(item);

		for each (SavedVehColourDBRow *sv in savedVehColours)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "保存的颜色", onconfirm_veh_savedcolor_menu, NULL, NULL, veh_colour_menu_interrupt);

		for (std::vector<SavedVehColourDBRow*>::iterator it = savedVehColours.begin(); it != savedVehColours.end(); ++it)
		{
			delete (*it);
		}
		savedVehColours.clear();
	} while (requireRefreshOfVehColourSaveSlots);

	return false;
}
// 保存/加载车辆颜色逻辑结束

bool onconfirm_randcolour_category_menu(MenuItem<int> choice)
{
	whichpart = choice.value;
	if (whichpart == 10) {
		Vehicle my_veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 1);
		for (int i = 0; i < 5; i++) {
			int rand_colour_1 = 0;
			int rand_colour_2 = 0;
			int colorIndex1 = 0;
			int colorIndex2 = 0;
			int rand_category_1 = (rand() % 5 + 0);
			int rand_category_2 = (rand() % 5 + 0);
			if (rand_category_1 == 0) rand_colour_1 = (rand() % (PAINTS_CLASSIC.size() - 1) + 0);
			if (rand_category_1 == 1) rand_colour_1 = (rand() % (PAINTS_MATTE.size() - 1) + 0);
			if (rand_category_1 == 2) rand_colour_1 = (rand() % (PAINTS_METALS.size() - 1) + 0);
			if (rand_category_1 == 3) rand_colour_1 = (rand() % (PAINTS_UTIL.size() - 1) + 0);
			if (rand_category_1 == 4) rand_colour_1 = (rand() % (PAINTS_WORN.size() - 1) + 0);
			if (rand_category_1 == 5) rand_colour_1 = (rand() % (PAINTS_CHROME.size() - 1) + 0);
			if (rand_category_2 == 0) rand_colour_2 = (rand() % (PAINTS_CLASSIC.size() - 1) + 0);
			if (rand_category_2 == 1) rand_colour_2 = (rand() % (PAINTS_MATTE.size() - 1) + 0);
			if (rand_category_2 == 2) rand_colour_2 = (rand() % (PAINTS_METALS.size() - 1) + 0);
			if (rand_category_2 == 3) rand_colour_2 = (rand() % (PAINTS_UTIL.size() - 1) + 0);
			if (rand_category_2 == 4) rand_colour_2 = (rand() % (PAINTS_WORN.size() - 1) + 0);
			if (rand_category_2 == 5) rand_colour_2 = (rand() % (PAINTS_CHROME.size() - 1) + 0);
			int rand_colour_r = (rand() % 255 + 0);
			int rand_colour_g = (rand() % 255 + 0);
			int rand_colour_b = (rand() % 255 + 0);
			int colorPrimary = -1;
			int colorSecondary = -1;
			VEHICLE::GET_VEHICLE_COLOURS(my_veh, &colorPrimary, &colorSecondary);
			if (rand_colour_1 < PAINTS_BY_TYPE[rand_category_1].size()) colorIndex1 = PAINTS_BY_TYPE[rand_category_1][rand_colour_1].colorIndex;
			if (rand_colour_2 < PAINTS_BY_TYPE[rand_category_2].size()) colorIndex2 = PAINTS_BY_TYPE[rand_category_2][rand_colour_2].colorIndex;
			if (i == 0 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 0) VEHICLE::SET_VEHICLE_COLOURS(my_veh, colorIndex1, colorIndex2);
			if (i == 1 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 0) VEHICLE::SET_VEHICLE_EXTRA_COLOURS(my_veh, colorIndex1, colorIndex2);
			if (i == 0 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 1) VEHICLE::SET_VEHICLE_COLOURS(my_veh, colorIndex1, colorSecondary);
			if (i == 1 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 1) VEHICLE::SET_VEHICLE_EXTRA_COLOURS(my_veh, colorIndex1, colorSecondary);
			if (i == 0 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 2) VEHICLE::SET_VEHICLE_COLOURS(my_veh, colorPrimary, colorIndex2);
			if (i == 1 && NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 2) VEHICLE::SET_VEHICLE_EXTRA_COLOURS(my_veh, colorPrimary, colorIndex2);
			if (i == 2 && (NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 0 || NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 1)) VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(my_veh, rand_colour_r, rand_colour_g, rand_colour_b);
			if (i == 3 && (NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 0 || NPC_RAGDOLL_VALUES[VehRandomColourIndex] == 2)) VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(my_veh, rand_colour_r, rand_colour_g, rand_colour_b);
			if (i == 4) {
				VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(my_veh, colorIndex1);
				VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(my_veh, colorIndex2);
			}
		}
	}
	return false;
}

bool process_veh_randomcolour_menu() {
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int>* item;
	SelectFromListMenuItem* listItem;

	item = new MenuItem<int>();
	item->caption = "启用随机颜色";
	item->value = 10;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_RAND_COLOUR_CAPTIONS, onchange_vehicles_random_colour_index);
	listItem->wrap = false;
	listItem->caption = "随机方式";
	listItem->value = VehRandomColourIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, NULL, " 车辆随机颜色", onconfirm_randcolour_category_menu, NULL, NULL, vehicle_menu_interrupt);

	return false;
}

bool onconfirm_paint_menu(MenuItem<int> choice){
	whichpart = choice.value;
	if(whichpart >= 0 && whichpart < 7){
		process_paint_menu_type();
	}
	else if(whichpart == 7){
		process_paint_menu_dirt();
	}
	else if(whichpart == 8){
		process_paint_menu_fades();
	}
	else if(whichpart == 9){
		process_paint_menu_liveries();
	}
	else if (whichpart == 163) {
		process_veh_savedcolour_menu();
	}
	else if (whichpart == 164) {
		process_veh_randomcolour_menu();
	}

	return false;
}

bool process_paint_menu(){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int livCount = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);

	std::vector<MenuItem<int> *> menuItems;
	//SelectFromListMenuItem* listItem;
	MenuItem<int> *item;
	int index;

	for(index = 0; index < MENU_PAINT_WHAT.size(); index++){
		item = new MenuItem<int>();
		item->caption = MENU_PAINT_WHAT[index];
		item->value = index;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "污垢等级";
	item->value = index++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "漆面褪色";
	item->value = index++;
	item->isLeaf = false;
	menuItems.push_back(item);

	if(livCount > 1){
		std::ostringstream ss;
		ss << "涂装 (" << livCount << ")";
		item = new MenuItem<int>();
		item->caption = ss.str();
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "保存的颜色";
	item->value = 163;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "车辆随机颜色";
	item->value = 164;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, 0, "选择喷涂部位", onconfirm_paint_menu, NULL, NULL, vehicle_menu_interrupt);//22
}

bool onconfirm_paint_menu_type(MenuItem<int> choice){
	std::string category = choice.caption;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	whichtype = choice.value;

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;

	int primary, secondary, pearl, wheel, extra;
	VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);

	std::vector<PaintColor> paints;
	if(whichtype == 7){
		apply_paint(PAINTS_BY_TYPE[whichtype].at(0));
		return false;
	}
	else if (whichtype == 8){
		int components[3] = { 0, 0, 0 };

		switch (whichpart){
		case 0:
		case 2:
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &components[0], &components[1], &components[2]);
			break;
		case 1:
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &components[0], &components[1], &components[2]);
			break;
		}

		for (int a = 0; a < 3; a++){
			PaintColorItem<int> *colorItem;

			colorItem = new PaintColorItem<int>();
			colorItem->caption = ENTColor::colsCompCaptions[a];
			colorItem->isLeaf = true;
			colorItem->colorval = components[a];
			colorItem->part = whichpart;
			colorItem->component = a;
			menuItems.push_back(colorItem);
		}

		draw_generic_menu<int>(menuItems, NULL, category, NULL, NULL, NULL, vehicle_menu_interrupt);
		return false;
	}
	else{
		paints = PAINTS_BY_TYPE[whichtype];
	}

	int matchIndex = 0, ps = paints.size();

    switch (whichpart){
    case 5:
        VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &extra);
        break;
    case 6:
        VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &extra);
        break;
    }

	for(int a = 0; a < ps; a++){
		item = new MenuItem<int>();
		item->caption = paints.at(a).name;
		item->value = a;
		menuItems.push_back(item);

		switch(whichpart){
			case 0:
			case 2:
				if(paints.at(a).colorIndex == primary){
					matchIndex = a;
				}
				break;
			case 1:
				if(paints.at(a).colorIndex == secondary){
					matchIndex = a;
				}
				break;
			case 3:
				if(paints.at(a).colorIndex == pearl){
					matchIndex = a;
				}
				break;
			case 4:
				if(paints.at(a).colorIndex == wheel){
					matchIndex = a;
				}
				break;
            case 5:
                if (paints.at(a).colorIndex == extra){
                    matchIndex = a;
                }
                break;
            case 6:
                if (paints.at(a).colorIndex == extra){
                    matchIndex = a;
                }
                break;
            default:
				break;
		}
	}

	draw_generic_menu<int>(menuItems, &matchIndex, category, onconfirm_color_menu_selection, onhighlight_color_menu_selection, NULL, vehicle_menu_interrupt);
	return false;
}

bool process_paint_menu_type(){
	bool rval = false;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	PaintIndexItem<int> *indexItem;
	int index;

	for(index = 0; index < MENU_PAINT_TYPE.size(); index++){
		item = new MenuItem<int>();
		item->caption = MENU_PAINT_TYPE[index];
		item->value = index;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	int primary, secondary, pearl, wheel, extra;
	VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);

	indexItem = new PaintIndexItem<int>();
	indexItem->caption = MENU_PAINT_INDEX_TYPE;
	indexItem->value = index++;
	indexItem->isLeaf = true;
	switch (whichpart) {
	case 0:
	case 2:
		indexItem->colorindex = primary;
		break;
	case 1:
		indexItem->colorindex = secondary;
		break;
	case 3:
		indexItem->colorindex = pearl;
		break;
	case 4:
		indexItem->colorindex = wheel;
		break;
    case 5:
        VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &extra);
        indexItem->colorindex = extra;
        break;
    case 6:
        VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &extra);
        indexItem->colorindex = extra;
        break;
    }
	indexItem->part = whichpart;
	menuItems.push_back(indexItem);
	parentIndexItem = indexItem;

	if (whichpart >= 0 && whichpart < 3){
		item = new MenuItem<int>();
		item->caption = MENU_CUSTOM_PAINT_TYPE;
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	rval = draw_generic_menu<int>(menuItems, NULL, "选择颜色类型", onconfirm_paint_menu_type, NULL, NULL, vehicle_menu_interrupt);//11
	parentIndexItem = NULL;

	return rval;
}

void onhighlight_color_menu_selection(MenuItem<int> choice){
	apply_paint(choice.value == -1 ? PAINTS_CHROME.at(0) : PAINTS_BY_TYPE[whichtype].at(choice.value));
}

bool onconfirm_color_menu_selection(MenuItem<int> choice){
	return true;
}

void apply_paint(PaintColor whichpaint){
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	if(bPlayerExists){
		if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			int primary, secondary, pearl, wheel, colorIndex = whichpaint.colorIndex;
			VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);

			BOOL isCustom;
			int red, green, blue;

			switch(whichpart){
				case 0:
					isCustom = VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh);
					if (isCustom){
						VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &red, &green, &blue);
					}
					VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh);
					VEHICLE::SET_VEHICLE_COLOURS(veh, colorIndex, secondary);
					if (isCustom){
						VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, red, green, blue);
					}
					break;
				case 1:
					isCustom = VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh);
					if (isCustom){
						VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &red, &green, &blue);
					}
					VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh);
					VEHICLE::SET_VEHICLE_COLOURS(veh, primary, colorIndex);
					if (isCustom){
						VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, red, green, blue);
					}
					break;
				case 2:
					VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh);
					VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh);
					VEHICLE::SET_VEHICLE_COLOURS(veh, colorIndex, colorIndex);
					break;
				case 3:
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colorIndex, wheel);
					break;
				case 4:
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pearl, colorIndex);
					break;
                case 5:
                    VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(veh, colorIndex);
                    break;
                case 6:
                    VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(veh, colorIndex);
                    break;
                default:
					break;
			}

			if(whichtype == 1 && whichpart >= 0 && whichpart <= 2){
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colorIndex, wheel);
			}

			if (parentIndexItem){
				parentIndexItem->SetIndex(colorIndex);
			}
		}
		else{
			set_status_text("玩家不在车辆中！");
		}
	}
}
