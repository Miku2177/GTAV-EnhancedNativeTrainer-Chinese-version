/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "misc.h"
#include "script.h"
#include "hotkeys.h"
#include "world.h"
#include "vehicles.h"
#include <Psapi.h>
#include "../utils.h"
#include <iterator>
#include "..\ui_support\menu_functions.h"

//==================
// 其他菜单选项
//==================

bool g_MenuLayoutNeedsRefresh = false;//添加刷新标志定义

bool iterated_radio_stations = false; //为了防止不必要的循环而反复执行。

int activeLineIndexTrainerConfig = 0;
int activeLineIndexPhoneBill = 0;
int activeLineIndexRadioSettings = 0;
int activeLineIndexHudSettings = 0;
int activeLineIndexDefMenuTab = 0;
int activeLineIndexBillSettings = 0;
int activeLineIndexPhoneOnBike = 0;
int activeLineIndexAirbrake = 0;
int activeLineHotkeyConfig = 0;
int activeLineMainKeyConfig = 0;
int activeLineNavKeyConfig = 0;

// 自由移动模式变量
bool airbrake_enable = true; // 启用自由移动
bool mouse_view_control = false; // 禁用鼠标视角控制
bool help_showing = true; // 显示帮助信息
bool frozen_time = false; // 时间冻结功能关闭
bool show_transparency = true; // 显示透明度

// 电话账单变量
bool featurePhoneBillEnabled = false;
bool featureGamePause = false;
bool featureZeroBalance = false;
bool featurePhone3DOnBike = false;
bool featureNoPhoneOnHUD = false;
int secs_passed, secs_curr = -1;
float temp_seconds, bill_seconds = 0;
float bill_to_pay = -1;

bool featureDisableRecording = false;
bool featureNoNotifications = false;

// 动态健康条变量
bool featureDynamicHealthBar = false;
int temp_h, temp_h_d = -1;
bool dynamic_loading = true;
bool been_damaged = false;
float curr_damaged_health, curr_damaged_armor = -1;
int healthbar_secs_curr, healthbar_seconds = -1; 
float health_bar_x = 0.015;
float health_bar_y = 0.966;

// 显示帧率
static int frames = 0;
static double FPStime, FPStime_passed, FPStime_curr, starttime = 0;
int fps = 0; 
char fps_to_show_char_modifiable[15];

// 骑车时使用手机
Object temp_obj = -1;
char* anim_dict = "anim@cellphone@in_car@ps";
char* animation_of_d = "cellphone_text_read_base";
bool accel = false;
bool p_exist = false;

bool radio_pressed = false;

int r_secs_passed, r_secs_curr, r_seconds = -1;

bool radio_v_checked = false;
bool no_blur_initialized = false;

bool no_phone, bill_no_phone = false;

Vehicle playerVeh = -1;

//跳过曲目
static uintptr_t* g_radioStationList;
static int* g_radioStationCount;
static void(*CRadioStation__Advance)(uintptr_t This, uint32_t a2);
static uintptr_t* g_unkRadioStationData;
bool skip_track_pressed = false;

// 过场动画查看器，第一人称过场动画摄像机
bool cutscene_is_playing, cutscene_being_watched, found_ped_in_cutscene = false;
bool con_disabled = false;
bool manual_cutscene = false;
Ped curr_cut_ped_me, my_first_coords, curr_cut_ped, switched_c = -1;
bool featureFirstPersonCutscene = false;

bool featurePlayerRadio = false;
bool featureDisablePhone = false;
bool featureDisablePhoneMenu = false;
bool featurePlayerRadioUpdated = false;
bool featureRadioFreeze = false, featureRadioFreezeUpdated = false;
bool featureBoostRadio = true;
bool featureRealisticRadioVolume = false;
bool featureWantedMusic = false;
bool featureFlyingMusic = false;
bool featurePoliceScanner = false;
bool featureNoComleteMessage = false;
bool featurePoliceRadio = false;
//bool police_radio_check = false;
bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;
bool featureMiscHideHudUpdated = false;
bool featurePhoneShowHud = false;
bool featureInVehicleNoHud = false;
bool featureMarkerHud = false;
bool phone_toggle = false;
bool phone_toggle_vehicle = false;
bool phone_toggle_defaultphone = false;
bool featureFirstPersonDeathCamera = false;
bool featureFirstPersonStuntJumpCamera = false;
bool featureNoStuntJumps = false;
bool featureHidePlayerInfo = false;
bool featureShowFPS = false;
bool featurenowheelblurslow = false;
bool featureShowVehiclePreviews = true;
bool featureShowStatusMessage = true;
bool featureNoAutoRespawn = false;
bool featureMiscJellmanScenery = false;
bool featureEnableMissingRadioStation = false;

std::string screenfltr;
bool sfilter_enabled = false;

//bool featureBlockInputInMenu = false;
//bool featureControllerIgnoreInTrainer = false;

const int TRAINERCONFIG_HOTKEY_MENU = 99;
const int TRAINERCONFIG_MAIN_KEY_MENU = 100;
const int MAINKEY_HOTKEY_FUNCTIONS = 101;
const int MAINKEY_NAVIGATION_KEYS = 102;
int radioStationIndex = -1;

Camera StuntCam = NULL;

// 第一人称过场动画摄像机变量
Cam CutCam = NULL;
Object xaxis, zaxis = -1;

// 主要角色
const Hash PLAYER_ZERO = 0xD7114C9;
const Hash PLAYER_ONE = 0x9B22DBAF;
const Hash PLAYER_TWO = 0x9B810FA2;

// 主要角色现金
const Hash SP0_TOTAL_CASH = 0x324C31D;
const Hash SP1_TOTAL_CASH = 0x44BD6982;
const Hash SP2_TOTAL_CASH = 0x8D75047D;

// 电话账单金额
int PhoneBillIndex = 2;
bool PhoneBillChanged = true;

// 电话账单免费时长
int PhoneFreeSecondsIndex = 0;
bool PhoneFreeSecondsChanged = true;
int PhoneBikeAnimationIndex = 0;
bool PhoneBikeAnimationChanged = true;

int missing_station = 0;

// 默认菜单标签
int DefMenuTabIndex = 0;
bool DefMenuTabChanged = true;

// 默认手机
const std::vector<std::string> MISC_PHONE_DEFAULT_CAPTIONS{ "关", "迈克尔", "崔佛", "富兰克", "军用", "序章" };
const int MISC_PHONE_DEFAULT_VALUES[] = { -1, 0, 1, 2, 3, 4 };
int PhoneDefaultIndex = 0;
bool PhoneDefaultChanged = true;

// 收音机关闭
const std::vector<std::string> MISC_RADIO_OFF_CAPTIONS{ "默认", "始终", "仅限摩托车" };
int RadioOffIndex = 0;
bool RadioOffChanged = true;

// 收音机关台切换
const std::vector<std::string> MISC_RADIO_SWITCHING_CAPTIONS{ "关", "下一首电台歌曲", "每 3 分钟", "每 5 分钟", "每 7 分钟", "每 10 分钟", "每 15 分钟", "每 30 分钟" };
const int MISC_RADIO_SWITCHING_VALUES[] = { 0, 1, 180, 300, 420, 600, 900, 1800 };
int RadioSwitchingIndex = 0;
bool RadioSwitchingChanged = true;

// 训练器控制
int TrainerControlIndex = 0;
bool TrainerControlChanged = true;

// 菜单 (活动条) 滚动方式
int TrainerControlScrollingIndex = 0;
bool TrainerControlScrollingChanged = true;

// 字体设置变量
int FontHeaderIndex = 0;
bool FontHeaderChanged = true;

int FontItemIndex = 0;
bool FontItemChanged = true;

int FontWantedIndex = 0;
bool FontWantedChanged = true;

int FontStatusIndex = 0;
bool FontStatusChanged = true;

int MenuItemsCountIndex = 0;
bool MenuItemsCountChanged = true;

int activeLineIndexFontSettings = 0;

void onchange_hotkey_function(int value, SelectFromListMenuItem* source){
	change_hotkey_function(source->extras.at(0), value);
}

std::string get_key_display_name(std::string keyFunction){
	KeyConfig* keyConfig = get_config()->get_key_config()->get_key(keyFunction);
	if(keyConfig == NULL || keyConfig->keyCode == 0){
		return "未绑定";
	}
	return std::string(keyValToName(keyConfig->keyCode));
}

void process_misc_main_key_menu(){
	const std::string caption = "主要按键设置";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "快捷键功能";
	stdItem->value = MAINKEY_HOTKEY_FUNCTIONS;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	stdItem = new MenuItem<int>();
	stdItem->caption = "菜单导航按键";
	stdItem->value = MAINKEY_NAVIGATION_KEYS;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	draw_generic_menu<int>(menuItems, &activeLineMainKeyConfig, caption, onconfirm_main_key_menu, NULL, NULL);
}

bool onconfirm_main_key_menu(MenuItem<int> choice){
	if(choice.value == MAINKEY_HOTKEY_FUNCTIONS){
		process_misc_hotkey_menu();
	}
	else if(choice.value == MAINKEY_NAVIGATION_KEYS){
		process_misc_navigation_key_menu();
	}
	return false;
}

void process_misc_navigation_key_menu(){
	const std::string caption = "菜单导航按键";

	std::vector<MenuItem<int>*> menuItems;

	// 主菜单开关键
	MenuItem<int>* keyItem = new MenuItem<int>();
	keyItem->caption = "开/关 主菜单: " + get_key_display_name(KeyConfig::KEY_TOGGLE_MAIN_MENU);
	keyItem->value = 1;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	// 菜单导航键
	keyItem = new MenuItem<int>();
	keyItem->caption = "向上移动菜单: " + get_key_display_name(KeyConfig::KEY_MENU_UP);
	keyItem->value = 2;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	keyItem = new MenuItem<int>();
	keyItem->caption = "向下移动菜单: " + get_key_display_name(KeyConfig::KEY_MENU_DOWN);
	keyItem->value = 3;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	keyItem = new MenuItem<int>();
	keyItem->caption = "向左移动菜单: " + get_key_display_name(KeyConfig::KEY_MENU_LEFT);
	keyItem->value = 4;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	keyItem = new MenuItem<int>();
	keyItem->caption = "向右移动菜单: " + get_key_display_name(KeyConfig::KEY_MENU_RIGHT);
	keyItem->value = 5;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	keyItem = new MenuItem<int>();
	keyItem->caption = "选择菜单选项: " + get_key_display_name(KeyConfig::KEY_MENU_SELECT);
	keyItem->value = 6;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	keyItem = new MenuItem<int>();
	keyItem->caption = "返回上级菜单: " + get_key_display_name(KeyConfig::KEY_MENU_BACK);
	keyItem->value = 7;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	// 自由移动功能键
	keyItem = new MenuItem<int>();
	keyItem->caption = "开启/关闭自由移动功能: " + get_key_display_name(KeyConfig::KEY_TOGGLE_AIRBRAKE);
	keyItem->value = 8;
	keyItem->isLeaf = true;
	menuItems.push_back(keyItem);

	// 添加重置和保存选项
	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "重置所有按键为默认值";
	stdItem->value = 1001;
	stdItem->isLeaf = true;
	menuItems.push_back(stdItem);

	stdItem = new MenuItem<int>();
	stdItem->caption = "保存按键配置到XML";
	stdItem->value = 1002;
	stdItem->isLeaf = true;
	menuItems.push_back(stdItem);

	draw_generic_menu<int>(menuItems, &activeLineNavKeyConfig, caption, onconfirm_navigation_key_menu, NULL, NULL);
}

bool onconfirm_navigation_key_menu(MenuItem<int> choice){
	std::string keyFunction;
	switch(choice.value){
		case 1: keyFunction = KeyConfig::KEY_TOGGLE_MAIN_MENU; break;
		case 2: keyFunction = KeyConfig::KEY_MENU_UP; break;
		case 3: keyFunction = KeyConfig::KEY_MENU_DOWN; break;
		case 4: keyFunction = KeyConfig::KEY_MENU_LEFT; break;
		case 5: keyFunction = KeyConfig::KEY_MENU_RIGHT; break;
		case 6: keyFunction = KeyConfig::KEY_MENU_SELECT; break;
		case 7: keyFunction = KeyConfig::KEY_MENU_BACK; break;
		case 8: keyFunction = KeyConfig::KEY_TOGGLE_AIRBRAKE; break;
		case 1001: // 重置所有按键为默认值
			reset_navigation_keys_to_default();
			set_status_text("导航按键已重置为默认值!");
			return false;
		case 1002: // 保存按键配置到XML
			write_config_file();
			set_status_text("按键配置已保存到XML!");
			return false;
		default:
			return false;
	}
	
	// 启动按键输入模式
	start_key_input_mode(keyFunction);
	return false;
}

void reset_navigation_keys_to_default(){
	// 重置导航键为默认值
	KeyInputConfig* keyConfig = get_config()->get_key_config();
	
	// 删除旧的配置并设置默认值
	delete keyConfig->keyConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU];
	keyConfig->keyConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU] = new KeyConfig(VK_F4);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_UP];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_UP] = new KeyConfig(VK_NUMPAD8);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_DOWN];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_DOWN] = new KeyConfig(VK_NUMPAD2);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_LEFT];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_LEFT] = new KeyConfig(VK_NUMPAD4);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_RIGHT];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_RIGHT] = new KeyConfig(VK_NUMPAD6);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_SELECT];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_SELECT] = new KeyConfig(VK_NUMPAD5);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_MENU_BACK];
	keyConfig->keyConfigs[KeyConfig::KEY_MENU_BACK] = new KeyConfig(VK_NUMPAD0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE];
	keyConfig->keyConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE] = new KeyConfig(VK_F6);
}

// 按键输入模式变量
std::string current_key_function = "";
bool key_input_mode = false;

void start_key_input_mode(std::string keyFunction){
	current_key_function = keyFunction;
	key_input_mode = true;
	set_status_text("请按下要绑定的按键 (ESC 取消)");
}

void handle_key_input_mode(){
	if(!key_input_mode) return;
	
	// 检查ESC键取消
	if(IsKeyJustUp(VK_ESCAPE, true)){
		key_input_mode = false;
		current_key_function = "";
		set_status_text("按键绑定已取消");
		return;
	}
	
	// 检查其他按键
	for(int i = 0; i < (sizeof ALL_KEYS / sizeof ALL_KEYS[0]); i++){
		if(ALL_KEYS[i].keyCode != VK_ESCAPE && IsKeyJustUp(ALL_KEYS[i].keyCode, true)){
			// 找到按键，进行绑定
			KeyInputConfig* keyConfig = get_config()->get_key_config();
			delete keyConfig->keyConfigs[current_key_function];
			keyConfig->keyConfigs[current_key_function] = new KeyConfig(ALL_KEYS[i].keyCode);
			
			set_status_text("按键 " + std::string(ALL_KEYS[i].name) + " 已绑定到 " + current_key_function);
			key_input_mode = false;
			current_key_function = "";
			return;
		}
	}
}

bool process_misc_hotkey_menu(){
	std::vector<MenuItem<int>*> menuItems;

	// 添加9个快捷键的键位绑定菜单
	for(int i = 1; i < 10; i++){
		std::ostringstream itemCaption;
		std::string keyFunction;
		
		switch(i){
			case 1: keyFunction = KeyConfig::KEY_HOT_1; break;
			case 2: keyFunction = KeyConfig::KEY_HOT_2; break;
			case 3: keyFunction = KeyConfig::KEY_HOT_3; break;
			case 4: keyFunction = KeyConfig::KEY_HOT_4; break;
			case 5: keyFunction = KeyConfig::KEY_HOT_5; break;
			case 6: keyFunction = KeyConfig::KEY_HOT_6; break;
			case 7: keyFunction = KeyConfig::KEY_HOT_7; break;
			case 8: keyFunction = KeyConfig::KEY_HOT_8; break;
			case 9: keyFunction = KeyConfig::KEY_HOT_9; break;
		}
		
		itemCaption << "快捷键 " << i << " 按键: " << get_key_display_name(keyFunction);
		
		MenuItem<int>* keyItem = new MenuItem<int>();
		keyItem->caption = itemCaption.str();
		keyItem->value = i + 2000; // 偏移值以区分按键绑定和功能绑定
		keyItem->isLeaf = true;
		menuItems.push_back(keyItem);
	}

	// 分隔线
	MenuItem<int>* separatorItem = new MenuItem<int>();
	separatorItem->caption = "==================";
	separatorItem->value = -1;
	separatorItem->isLeaf = true;
	menuItems.push_back(separatorItem);

	// 原有的功能绑定菜单
	for(int i = 1; i < 10; i++){
		std::ostringstream itemCaption;
		std::vector<std::string> captions;
		void(*callback)(int, SelectFromListMenuItem*);

		itemCaption << "快捷键 " << i << " 功能";

		bool keyAssigned = get_config()->get_key_config()->is_hotkey_assigned(i);
		if(!keyAssigned){
			captions.push_back("未绑定按键");
			callback = NULL;

			SelectFromListMenuItem* item = new SelectFromListMenuItem(captions, callback);
			item->caption = itemCaption.str();
			item->value = NULL;
			menuItems.push_back(item);
		}
		else{
			for each (HOTKEY_DEF var in HOTKEY_AVAILABLE_FUNCS){
				captions.push_back(var.caption);
			}
			callback = onchange_hotkey_function;

			SelectFromListMenuItem* item = new SelectFromListMenuItem(captions, callback);
			item->caption = itemCaption.str();
			item->wrap = keyAssigned;
			item->extras.push_back(i);
			item->value = get_hotkey_function_index(i);
			menuItems.push_back(item);
		}
	}

	// 添加重置和保存选项
	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "重置所有快捷键为默认值";
	stdItem->value = 3001;
	stdItem->isLeaf = true;
	menuItems.push_back(stdItem);

	stdItem = new MenuItem<int>();
	stdItem->caption = "保存快捷键配置到XML";
	stdItem->value = 3002;
	stdItem->isLeaf = true;
	menuItems.push_back(stdItem);

	draw_generic_menu<int>(menuItems, &activeLineHotkeyConfig, "快捷键功能", onconfirm_hotkey_menu, NULL, NULL);

	return false;
}

bool onconfirm_hotkey_menu(MenuItem<int> choice){
	if(choice.value >= 2001 && choice.value <= 2009){
		// 快捷键按键绑定
		int hotkeyIndex = choice.value - 2000;
		std::string keyFunction;
		
		switch(hotkeyIndex){
			case 1: keyFunction = KeyConfig::KEY_HOT_1; break;
			case 2: keyFunction = KeyConfig::KEY_HOT_2; break;
			case 3: keyFunction = KeyConfig::KEY_HOT_3; break;
			case 4: keyFunction = KeyConfig::KEY_HOT_4; break;
			case 5: keyFunction = KeyConfig::KEY_HOT_5; break;
			case 6: keyFunction = KeyConfig::KEY_HOT_6; break;
			case 7: keyFunction = KeyConfig::KEY_HOT_7; break;
			case 8: keyFunction = KeyConfig::KEY_HOT_8; break;
			case 9: keyFunction = KeyConfig::KEY_HOT_9; break;
		}
		
		start_key_input_mode(keyFunction);
	}
	else if(choice.value == 3001){
		// 重置所有快捷键为默认值
		reset_hotkeys_to_default();
		set_status_text("快捷键已重置为默认值!");
	}
	else if(choice.value == 3002){
		// 保存快捷键配置到XML
		write_config_file();
		set_status_text("快捷键配置已保存到XML!");
	}
	return false;
}

void reset_hotkeys_to_default(){
	// 重置9个快捷键为默认值（空绑定）
	KeyInputConfig* keyConfig = get_config()->get_key_config();
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_1];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_1] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_2];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_2] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_3];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_3] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_4];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_4] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_5];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_5] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_6];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_6] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_7];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_7] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_8];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_8] = new KeyConfig(0);
	
	delete keyConfig->keyConfigs[KeyConfig::KEY_HOT_9];
	keyConfig->keyConfigs[KeyConfig::KEY_HOT_9] = new KeyConfig(0);
}

void process_misc_trainermenucoloring_menu(int part){
	std::vector<MenuItem<int> *> menuItems;
	int index = 0;
	ColorItem<int> *colorItem;

	for(auto a : ENTColor::colsCompCaptions){
		colorItem = new ColorItem<int>();
		colorItem->caption = a;
		colorItem->isLeaf = true;
		colorItem->colorval = ENTColor::colsMenu[part].rgba[index];
		colorItem->part = part;
		colorItem->component = index++;
		menuItems.push_back(colorItem);
	}

	draw_generic_menu<int>(menuItems, nullptr, ENTColor::colsCaptions[part], nullptr, nullptr, nullptr, nullptr);
}

bool onconfirm_trainermenucolors_menu(MenuItem<int> choice){
	if(choice.value >= 0 && choice.value < ENTColor::colsVarsNum){
		process_misc_trainermenucoloring_menu(choice.value);
	}
	else if(choice.value == ENTColor::colsVarsNum){
		write_config_ini_file();
		set_status_text_centre_screen("菜单颜色 ~g~已保存至 ~s~配置文件！"); // 屏幕中间提示，带闪烁
		set_status_text("菜单颜色，已保存到 INI 文件！");
		write_text_to_log_file("INI 配置文件，已写入或更新！");
		return true; // 返回 true 退出当前菜单，自动返回上一级菜单
	}
	else if(choice.value == ENTColor::colsVarsNum + 1){
		ENTColor::reset_colors();
		set_status_text("ENT 菜单颜色已重置为默认！");
		set_status_text_centre_screen("菜单颜色 ~g~已重置 ~s~为默认！"); // 屏幕中间提示，带闪烁
		return true; // 返回 true 退出当前菜单，自动返回上一级菜单
	}
	return false;
}

void process_misc_trainermenucolors_menu(){
	std::vector<MenuItem<int> *> menuItems;
	int index = 0;
	MenuItem<int> *item;

	for(auto a : ENTColor::colsCaptions){
		item = new MenuItem<int>();
		item->caption = a;
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "保存菜单颜色";
	item->value = index++;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	item = new MenuItem<int>();
	item->caption = "重置菜单颜色";
	item->value = index++;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	draw_generic_menu<int>(menuItems, nullptr, "修改器菜单颜色", onconfirm_trainermenucolors_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_trainerconfig_menu(MenuItem<int> choice){
	if(choice.value == TRAINERCONFIG_HOTKEY_MENU){
		//write_text_to_log_file("onconfirm_trainerconfig");
		process_misc_hotkey_menu();
	}
	else if(choice.value == TRAINERCONFIG_MAIN_KEY_MENU){
		process_misc_main_key_menu();
	}
	else if(choice.value == 63){
		process_misc_trainermenucolors_menu();
	}
	else if(choice.value == 64){
		process_misc_font_settings_menu();
	}
	else if(choice.value == 65){
		process_misc_menu_layout_settings_menu();
	}
	else if(choice.value == 66){
		process_misc_vehicle_preview_settings_menu();
	}
	return false;
}

// 车辆预览图设置菜单实现
int activeLineIndexVehiclePreview = 0;

void process_misc_vehicle_preview_settings_menu() {
	const std::string caption = "车辆预览图设置";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	// 添加预览图左右判断依据设置
	listItem = new SelectFromListMenuItem(MISC_PREVIEW_POSITION_THRESHOLD_CAPTIONS, onchange_misc_preview_position_threshold_index);
	listItem->wrap = false;
	listItem->caption = "预览图右侧显示切换";
	listItem->value = PreviewPositionThresholdIndex;
	menuItems.push_back(listItem);

	// 添加预览图分辨率适配值设置
	listItem = new SelectFromListMenuItem(MISC_PREVIEW_RESOLUTION_SCALE_CAPTIONS, onchange_misc_preview_resolution_scale_index);
	listItem->wrap = false;
	listItem->caption = "预览图分辨率适配";
	listItem->value = PreviewResolutionScaleIndex;
	menuItems.push_back(listItem);

	// 添加预览图间距设置
	listItem = new SelectFromListMenuItem(MISC_PREVIEW_SPACING_CAPTIONS, onchange_misc_preview_spacing_index);
	listItem->wrap = false;
	listItem->caption = "车辆预览图间距";
	listItem->value = PreviewSpacingIndex;
	menuItems.push_back(listItem);

	// 添加显示车辆预览图选项
	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "显示车辆预览图";
	toggleItem->toggleValue = &featureShowVehiclePreviews;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexVehiclePreview, caption, NULL, NULL, NULL);
}

void process_misc_trainerconfig_menu(){
	const std::string caption = "修改器设置选项";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "主要按键设置";
	stdItem->value = TRAINERCONFIG_MAIN_KEY_MENU;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	listItem = new SelectFromListMenuItem(MISC_TRAINERCONTROL_CAPTIONS, onchange_misc_trainercontrol_index);
	listItem->wrap = false;
	listItem->caption = "菜单控制方式";
	listItem->value = TrainerControlIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(MISC_TRAINERCONTROLSCROLLING_CAPTIONS, onchange_misc_trainercontrolscrolling_index);
	listItem->wrap = false;
	listItem->caption = "菜单滚动方式";
	listItem->value = TrainerControlScrollingIndex;
	menuItems.push_back(listItem);

	// 添加字体设置菜单项
	MenuItem<int>* fontSettingsItem = new MenuItem<int>();
	fontSettingsItem->caption = "字体类型设置";
	fontSettingsItem->value = 64;
	fontSettingsItem->isLeaf = false;
	menuItems.push_back(fontSettingsItem);

	// 添加菜单布局设置菜单项
	MenuItem<int>* menuLayoutSettingsItem = new MenuItem<int>();
	menuLayoutSettingsItem->caption = "菜单布局设置";
	menuLayoutSettingsItem->value = 65;
	menuLayoutSettingsItem->isLeaf = false;
	menuItems.push_back(menuLayoutSettingsItem);
	
	// 添加车辆预览图设置菜单项
	MenuItem<int>* vehiclePreviewSettingsItem = new MenuItem<int>();
	vehiclePreviewSettingsItem->caption = "车辆预览图设置";
	vehiclePreviewSettingsItem->value = 66;
	vehiclePreviewSettingsItem->isLeaf = false;
	menuItems.push_back(vehiclePreviewSettingsItem);

	// 添加菜单显示项目数设置
	SelectFromListMenuItem *menuItemsCountItem = new SelectFromListMenuItem(MISC_MENU_ITEMS_COUNT_CAPTIONS, onchange_misc_menu_items_count_index);
	menuItemsCountItem->wrap = false;
	menuItemsCountItem->caption = "菜单显示项目数";
	menuItemsCountItem->value = MenuItemsCountIndex;
	menuItems.push_back(menuItemsCountItem);

	//ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	//toggleItem->caption = "Lock Controls While In Menu";
	//toggleItem->toggleValue = &featureBlockInputInMenu;
	//menuItems.push_back(toggleItem);

	//toggleItem = new ToggleMenuItem<int>();
	//toggleItem->caption = "Turn Off All Controller Input In Trainer";
	//toggleItem->toggleValue = &featureControllerIgnoreInTrainer;
	//menuItems.push_back(toggleItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启动时显示状态消息";
	toggleItem->toggleValue = &featureShowStatusMessage;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "包含 Nkjellman 的额外场景";
	toggleItem->toggleValue = &featureMiscJellmanScenery;
	menuItems.push_back(toggleItem);

	stdItem = new MenuItem<int>();
	stdItem->caption = "修改器菜单颜色设置";
	stdItem->value = 63;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexTrainerConfig, caption, onconfirm_trainerconfig_menu, NULL, NULL);
}

bool onconfirm_misc_freezeradio_menu(MenuItem<int> choice){
	if(choice.value == -1){
		featureRadioFreeze = false;
		set_status_text("电台固定已成功解除！\n您可以自由切换电台了！");
	}
	else{
		featureRadioFreeze = true;
		set_status_text(std::string("已冻结电台, 并固定为: ") + std::string(AUDIO::GET_RADIO_STATION_NAME(choice.value)));
	}
	featureRadioFreezeUpdated = true;
	radioStationIndex = choice.value;

	return false;
}

void play_cutscene(std::string curr_c) {
	char* cstr = new char[curr_c.length() + 1];
	strcpy(cstr, curr_c.c_str());

	CUTSCENE::REQUEST_CUTSCENE(cstr, 8);
	while (!CUTSCENE::HAS_CUTSCENE_LOADED() && !CONTROLS::IS_CONTROL_PRESSED(2, 22)) {
		make_periodic_feature_call();
		WAIT(0);
	}
	if (CUTSCENE::HAS_CUTSCENE_LOADED()) {
		cutscene_is_playing = true;
		manual_cutscene = true;
		CUTSCENE::SET_CUTSCENE_FADE_VALUES(0, 0, 0, 0);
		CUTSCENE::START_CUTSCENE(0);
		CAM::SET_WIDESCREEN_BORDERS(0, 0);
		delete[] cstr;
	}
}

void stop_cutscene() {
	OBJECT::DELETE_OBJECT(&xaxis);
	OBJECT::DELETE_OBJECT(&zaxis);
	if (CAM::DOES_CAM_EXIST(CutCam)) {
		CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
		CAM::DESTROY_CAM(CutCam, true);
	}
	CAM::DO_SCREEN_FADE_IN(0);
	CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
	CUTSCENE::REMOVE_CUTSCENE();
	CAM::DO_SCREEN_FADE_IN(0);
	curr_cut_ped_me = -1;
	my_first_coords = -1;
	curr_cut_ped = -1;
	cutscene_is_playing = false;
	cutscene_being_watched = false;
	switched_c = -1;
	found_ped_in_cutscene = false;
	con_disabled = false;
	manual_cutscene = false;
}

bool onconfirm_misc_cutscene_menu(MenuItem<int> choice) {
	if (choice.value == -1) {
		stop_cutscene();
	} 
	else if(choice.value == -2) {
		found_ped_in_cutscene = false;
	}
	else if (choice.value == -3) {
		keyboard_on_screen_already = true;
		curr_message = "输入过场动画名称 ( 例如: mph_nar_fin_ext 或 bmad_intro )";
		std::string result = show_keyboard("手动输入名称", NULL);
		if (!result.empty()) {
			result = trim(result);
			play_cutscene(result);
		}
	}
	else {
		std::string value_m = MISC_CUTSCENE_VALUES[choice.value];
		play_cutscene(value_m);
	}
	return false;
}

void process_misc_cutplayer_menu() {
	std::vector<MenuItem<int>*> menuItems;
	std::vector<std::string> captions;
	
	captions = MISC_CUTSCENE_VALUES;
	ToggleMenuItem<int>* toggleItem;
	
	int i = -1;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "按 [空格键] 停止播放";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "第一人称视角过场动画";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureFirstPersonCutscene;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "切换观看视角";
	item->value = -2;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "手动输入名称";
	item->value = -3;
	item->isLeaf = true;
	menuItems.push_back(item);

	for each (std::string scenario in captions)
	{
		item = new MenuItem<int>();
		item->caption = scenario;
		item->value = i++;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, nullptr, "播放过场动画", onconfirm_misc_cutscene_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_misc_filters_menu(MenuItem<int> choice) {
	std::string value_m = MISC_FILTERS_VALUES[choice.value];
	char *cstr = new char[value_m.length() + 1];
	strcpy(cstr, value_m.c_str());

	GRAPHICS::SET_TIMECYCLE_MODIFIER(cstr);
	GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
	screenfltr = cstr;
	return false;
}

void process_misc_filters_menu() {
	std::vector<MenuItem<int>*> menuItems;
	std::vector<std::string> captions;
	
	captions = MISC_FILTERS_VALUES;
	MenuItem<int> *item = new MenuItem<int>();
	
	int i = 0;
	for each (std::string scenario in captions)
	{
		item = new MenuItem<int>();
		item->caption = scenario;
		item->value = i++;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, nullptr, "屏幕滤镜", onconfirm_misc_filters_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_misc_musicevent_menu(MenuItem<int> choice) {
	if (choice.value == -1) {
		AUDIO::TRIGGER_MUSIC_EVENT("AC_STOP");
	}
	else {
		std::string value_m = MISC_MUSICEVENT_VALUES[choice.value];
		char *cstr = new char[value_m.length() + 1];
		strcpy(cstr, value_m.c_str());
		AUDIO::TRIGGER_MUSIC_EVENT(cstr);
		delete[] cstr;
	}
	return false;
}

void process_misc_musicevent_menu() {
	std::vector<MenuItem<int>*> menuItems;
	std::vector<std::string> captions;
	
	captions = MISC_MUSICEVENT_VALUES;
	
	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "无";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	int i = 0;
	for each (std::string scenario in captions)
	{
		item = new MenuItem<int>();
		item->caption = scenario;
		item->value = i++;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, nullptr, "播放场景音乐", onconfirm_misc_musicevent_menu, nullptr, nullptr, nullptr);
}

void process_misc_freezeradio_menu(){ 
	std::vector<MenuItem<int> *> menuItems;
	int const stations = AUDIO::_MAX_RADIO_STATION_INDEX();

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "无";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	for(int a = 0; a < stations; a++){
		item = new MenuItem<int>();
		item->caption = AUDIO::GET_RADIO_STATION_NAME(a);
		item->value = a;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, nullptr, "冻结并固定电台", onconfirm_misc_freezeradio_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_airbrake_menu(MenuItem<int> choice) {

	if (choice.value == -1) {
		if (airbrake_enable) process_airbrake_menu();
	}
	return false;
}
 
void process_airbrake_global_menu() {
	const std::string caption = "自由移动模式";

	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用";
	toggleItem->toggleValue = &airbrake_enable;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "自由移动模式 [默认 F6 开/关 ]";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "鼠标模式";
	toggleItem->toggleValue = &mouse_view_control;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "显示帮助控制说明";
	toggleItem->toggleValue = &help_showing;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "冻结时间";
	toggleItem->toggleValue = &frozen_time;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "透 明 度";
	toggleItem->toggleValue = &show_transparency;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexAirbrake, caption, onconfirm_airbrake_menu, NULL, NULL);
}

bool onconfirm_defmenutab_menu(MenuItem<int> choice) {

	return false;
}

void process_def_menutab_menu() {
	const std::string caption = "暂停菜单设置";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	
	listItem = new SelectFromListMenuItem(MISC_DEF_MENUTAB_CAPTIONS, onchange_misc_def_menutab_index);
	listItem->wrap = false;
	listItem->caption = "默认的暂停菜单页";
	listItem->value = DefMenuTabIndex;
	menuItems.push_back(listItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "暂停菜单打开时, 暂停游戏";
	toggleItem->toggleValue = &featureGamePause;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "暂停菜单页, 隐藏角色信息";
	toggleItem->toggleValue = &featureHidePlayerInfo;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexDefMenuTab, caption, onconfirm_defmenutab_menu, NULL, NULL);
}

bool onconfirm_billsettings_menu(MenuItem<int> choice) {

	return false;
}

void process_billsettings_menu() {
	const std::string caption = "手机电话费";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用电话费";
	toggleItem->toggleValue = &featurePhoneBillEnabled;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_BILL_CAPTIONS, onchange_misc_phone_bill_index);
	listItem->wrap = false;
	listItem->caption = "每分钟话费金额";
	listItem->value = PhoneBillIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_FREESECONDS_CAPTIONS, onchange_misc_phone_freeseconds_index);
	listItem->wrap = false;
	listItem->caption = "前几秒免费";
	listItem->value = PhoneFreeSecondsIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "余额为零时禁止使用手机";
	toggleItem->toggleValue = &featureZeroBalance;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexBillSettings, caption, onconfirm_billsettings_menu, NULL, NULL);
}

bool onconfirm_phoneonbike_menu(MenuItem<int> choice) {

	return false;
}

void process_phoneonbike_menu() {
	const std::string caption = "骑车时使用手机";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用";
	toggleItem->toggleValue = &featurePhone3DOnBike;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "第一人称视角时, 屏幕上不显示手机";
	toggleItem->toggleValue = &featureNoPhoneOnHUD;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_FREESECONDS_CAPTIONS, onchange_misc_phone_bike_index);
	listItem->wrap = false;
	listItem->caption = "动作动画类型";
	listItem->value = PhoneBikeAnimationIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexPhoneOnBike, caption, onconfirm_phoneonbike_menu, NULL, NULL);
}

bool onconfirm_phonebill_menu(MenuItem<int> choice){
	switch (activeLineIndexPhoneBill) {
		case 1:
			process_billsettings_menu();
			break;
		case 2:
			process_phoneonbike_menu();
			break;
		default:
			break;
		}
	return false;
}

void process_phone_bill_menu(){
	const std::string caption = "手机设置选项";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	MenuItem<int> *item;

	int i = 0;

	listItem = new SelectFromListMenuItem(MISC_PHONE_DEFAULT_CAPTIONS, onchange_misc_phone_default_index);
	listItem->wrap = false;
	listItem->caption = "默认的手机型号";
	listItem->value = PhoneDefaultIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "手机电话费";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "骑车时使用手机";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "禁用手机";
	toggleItem->toggleValue = &featureDisablePhone;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "菜单打开时禁用手机";
	toggleItem->toggleValue = &featureDisablePhoneMenu;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexPhoneBill, caption, onconfirm_phonebill_menu, NULL, NULL);
}

bool onconfirm_radiosettings_menu(MenuItem<int> choice) {
	switch (activeLineIndexRadioSettings) {
	case 2:
		// 下一曲收音机曲目
		if (getGameVersion() > 41) SKIP_RADIO_FORWARD_CUSTOM();
		else AUDIO::SKIP_RADIO_FORWARD();
		skip_track_pressed = true;
		break;
	case 4:
		process_misc_freezeradio_menu();
		break;
	default:
		break;
	}
	return false;
}

void process_radio_settings_menu() {
	const std::string caption = "电台设置选项";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	MenuItem<int> *item;

	int i = 0;

	listItem = new SelectFromListMenuItem(MISC_RADIO_OFF_CAPTIONS, onchange_misc_radio_off_index);
	listItem->wrap = false;
	listItem->caption = "关闭电台";
	listItem->value = RadioOffIndex;
	menuItems.push_back(listItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "随身电台";
	toggleItem->toggleValue = &featurePlayerRadio;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "下一首电台曲目";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(MISC_RADIO_SWITCHING_CAPTIONS, onchange_misc_radio_switching_index);
	listItem->wrap = false;
	listItem->caption = "电台随机切换";
	listItem->value = RadioSwitchingIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "冻结并固定电台";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "提高电台的音量";
	toggleItem->toggleValue = &featureBoostRadio;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "统一的电台音量";
	toggleItem->toggleValue = &featureRealisticRadioVolume;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "恢复 缺失/隐藏的 电台";
	toggleItem->toggleValue = &featureEnableMissingRadioStation;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用警车的电台";
	toggleItem->toggleValue = &featurePoliceRadio;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexRadioSettings, caption, onconfirm_radiosettings_menu, NULL, NULL);
}

bool onconfirm_hudsettings_menu(MenuItem<int> choice) {
	return false;
}

void process_hud_settings_menu() {
	const std::string caption = "小地图设置";

	std::vector<MenuItem<int>*> menuItems;

	int i = 0;
	
	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "隐藏/显示 小地图";
	toggleItem->toggleValue = &featureMiscHideHud;
	toggleItem->toggleValueUpdated = &featureMiscHideHudUpdated;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅在打开电话时显示 小地图";
	toggleItem->toggleValue = &featurePhoneShowHud;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅在车辆中显示 小地图";
	toggleItem->toggleValue = &featureInVehicleNoHud;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅在设置导航点后显示 小地图";
	toggleItem->toggleValue = &featureMarkerHud;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "显示实时动态生命值条";
	toggleItem->toggleValue = &featureDynamicHealthBar;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "禁用动态模糊和减速效果";
	toggleItem->toggleValue = &featurenowheelblurslow;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "禁用屏幕录像";
	toggleItem->toggleValue = &featureDisableRecording;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "禁用任何游戏通知";
	toggleItem->toggleValue = &featureNoNotifications;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexHudSettings, caption, onconfirm_hudsettings_menu, NULL, NULL);
}

int activeLineIndexMisc = 0;

bool onconfirm_misc_menu(MenuItem<int> choice){
	switch(activeLineIndexMisc){
		case 0:
			process_misc_trainerconfig_menu();
			break;
		case 1:
			process_radio_settings_menu();
			break;
		case 2:
			process_hud_settings_menu();
			break;
		case 3:
			process_phone_bill_menu();
			break;
		case 4:
			process_def_menutab_menu();
			break;
		case 5:
			process_misc_musicevent_menu();
			break;
		case 6:
			process_misc_cutplayer_menu();
			break;
		case 7:
			process_misc_filters_menu();
			break;
		case 15:
			process_airbrake_global_menu();
			break;
		default:
			// 可切换功能
			break;
	}
	return false;
}

void process_misc_menu(){
	const int lineCount = 16; 

	const std::string caption = "其他选项";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"修改器设置", NULL, NULL, false},
		{"电台设置", NULL, NULL, false},
		{"小地图设置", NULL, NULL, false},
		{"手机设置", NULL, NULL, false},
		{"暂停菜单设置", NULL, NULL, false},
		{"播放场景音乐", nullptr, nullptr, false},
		{"播放过场动画", nullptr, nullptr, false},
		{"屏幕滤镜", nullptr, nullptr, false},
		{"无通缉音乐", &featureWantedMusic, NULL, true}, 
		{"无飞行音乐", &featureFlyingMusic, NULL, true}, 
		{"无警察电台广播", &featurePoliceScanner, NULL, true }, 
		{"无任务完成消息", &featureNoComleteMessage, NULL, true },
		{"第一人称特技跳跃视角", &featureFirstPersonStuntJumpCamera, NULL },
		{"无特技跳跃", &featureNoStuntJumps, NULL },
		{"FPS 帧率显示 ", &featureShowFPS, NULL }, 
		{"自由移动模式", NULL, NULL, false},
	};
	
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexMisc, caption, onconfirm_misc_menu);
}

// THE ORIGINAL CODE IS BY CAMXXCORE
#define XOR_32_64 0x31 // 逻辑异或
#define RET 0xC3 // 返回

bool setupPatches() {
	auto result = FindPatternJACCO("\x38\x51\x64\x74\x19", "xxxxx");
	if (!result) {
		return false;
	}

	auto address = result + 26;
	address = address + *(int32_t*)address + 4u;
	auto vigNetCallPtr = result + 8;
	auto timescalePtr = result + 34;
	
	unsigned char vigNetPatch[] = { RET, 0x90, 0x90, 0x90, 0x90 }; // 移除晕影
	unsigned char vigNetCall[] = { 0x90, 0x90, 0x90, 0x90, 0x90 }; // 晕影调用补丁（NOP）
	unsigned char timeScaleOverride[] = { XOR_32_64, 0xD2 }; // 时间缩放覆盖补丁

	memcpy((void*)address, vigNetPatch, sizeof(vigNetPatch) / sizeof(vigNetPatch[0]));
	memcpy((void*)vigNetCallPtr, vigNetCall, sizeof(vigNetCall) / sizeof(vigNetCall[0]));
	memcpy((void*)timescalePtr, timeScaleOverride, sizeof(timeScaleOverride) / sizeof(timeScaleOverride[0]));
	return true;
}

void initialize() {
	if (!setupPatches()) {
		return;
	}
	setupPatches();
}

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source){
	PhoneBillIndex = value;
	PhoneBillChanged = true;
}

void onchange_misc_phone_default_index(int value, SelectFromListMenuItem* source) {
	PhoneDefaultIndex = value;
	PhoneDefaultChanged = true;
}

void onchange_misc_radio_off_index(int value, SelectFromListMenuItem* source) {
	RadioOffIndex = value;
	RadioOffChanged = true;
}

void onchange_misc_radio_switching_index(int value, SelectFromListMenuItem* source) {
	RadioSwitchingIndex = value;
	RadioSwitchingChanged = true;
}

void onchange_misc_trainercontrol_index(int value, SelectFromListMenuItem* source) {
	TrainerControlIndex = value;
	TrainerControlChanged = true;
}

void onchange_misc_trainercontrolscrolling_index(int value, SelectFromListMenuItem* source) {
	TrainerControlScrollingIndex = value;
	TrainerControlScrollingChanged = true;
}

void onchange_misc_def_menutab_index(int value, SelectFromListMenuItem* source) {
	DefMenuTabIndex = value;
	DefMenuTabChanged = true;
}

void onchange_misc_phone_freeseconds_index(int value, SelectFromListMenuItem* source){
	PhoneFreeSecondsIndex = value;
	PhoneFreeSecondsChanged = true;
}

void onchange_misc_phone_bike_index(int value, SelectFromListMenuItem* source) {
	PhoneBikeAnimationIndex = value;
	PhoneBikeAnimationChanged = true;
}

void onchange_misc_font_header_index(int value, SelectFromListMenuItem* source) {
    FontHeaderIndex = value;
    fontHeader = MISC_FONT_HEADER_VALUES[value]; // 更新全局字体变量
    FontHeaderChanged = true;
}

void onchange_misc_font_item_index(int value, SelectFromListMenuItem* source) {
    FontItemIndex = value;
    fontItem = MISC_FONT_ITEM_VALUES[value]; // 更新全局字体变量
    FontItemChanged = true;
}

void onchange_misc_font_wanted_index(int value, SelectFromListMenuItem* source) {
    FontWantedIndex = value;
    fontWanted = MISC_FONT_WANTED_VALUES[value]; // 更新全局字体变量
    FontWantedChanged = true;
}

void onchange_misc_font_status_index(int value, SelectFromListMenuItem* source) {
    FontStatusIndex = value;
    fontStatus = MISC_FONT_STATUS_VALUES[value]; // 更新全局字体变量
    FontStatusChanged = true;
}

void onchange_misc_menu_items_count_index(int value, SelectFromListMenuItem* source) {
    MenuItemsCountIndex = value;
    itemsPerLine = MISC_MENU_ITEMS_COUNT_VALUES[value]; // 更新全局菜单显示项目数变量
    MenuItemsCountChanged = true;
}

void process_misc_font_settings_menu() {
    const std::string caption = "字体类型设置";

    std::vector<MenuItem<int>*> menuItems;
    SelectFromListMenuItem *listItem;

    listItem = new SelectFromListMenuItem(MISC_FONT_HEADER_CAPTIONS, onchange_misc_font_header_index);
    listItem->wrap = false;
    listItem->caption = "标题选项字体类型";
    listItem->value = FontHeaderIndex;
    menuItems.push_back(listItem);

    listItem = new SelectFromListMenuItem(MISC_FONT_ITEM_CAPTIONS, onchange_misc_font_item_index);
    listItem->wrap = false;
    listItem->caption = "项目选项字体类型";
    listItem->value = FontItemIndex;
    menuItems.push_back(listItem);

    listItem = new SelectFromListMenuItem(MISC_FONT_WANTED_CAPTIONS, onchange_misc_font_wanted_index);
    listItem->wrap = false;
    listItem->caption = "通缉等级星星类型";
    listItem->value = FontWantedIndex;
    menuItems.push_back(listItem);

    listItem = new SelectFromListMenuItem(MISC_FONT_STATUS_CAPTIONS, onchange_misc_font_status_index);
    listItem->wrap = false;
    listItem->caption = "状态显示字体类型";
    listItem->value = FontStatusIndex;
    menuItems.push_back(listItem);

    draw_generic_menu<int>(menuItems, &activeLineIndexFontSettings, caption, NULL, NULL, NULL);
}

void HUD_switching() {
	featureMiscHideHud = !featureMiscHideHud;
	//if (featureMiscHideHud) set_status_text("HUD OFF");
	//else set_status_text("HUD ON");
	WAIT(100);
}

void Traffic_switching() {
	featureWorldNoTraffic = !featureWorldNoTraffic;
	featureWorldNoTrafficUpdated = !featureWorldNoTrafficUpdated;
	WAIT(100);
}

void reset_misc_globals(){
	featureMiscHideHud =
		featurePhoneShowHud = 
		featureInVehicleNoHud =
		featureMarkerHud =
		featureDynamicHealthBar =
		featureDisableRecording =
		featureNoNotifications =
		featurePlayerRadio =
		featureDisablePhone =
		featureDisablePhoneMenu =
		featureMiscLockRadio =
		featureMiscJellmanScenery =
		featureRadioFreeze =
		featureWantedMusic = 
		featureFlyingMusic = 
		featurePoliceScanner = 
		featureNoComleteMessage =
		featurePoliceRadio =
		featureEnableMissingRadioStation = false;

	PhoneBillIndex = 2;
	PhoneDefaultIndex = 0;
	RadioOffIndex = 0;
	RadioSwitchingIndex = 0;
	TrainerControlIndex = 0;
	TrainerControlScrollingIndex = 0;
	PhoneFreeSecondsIndex = 0;
	PhoneBikeAnimationIndex = 0;
	DefMenuTabIndex = 0;
	FontHeaderIndex = 0;
	fontHeader = MISC_FONT_HEADER_VALUES[0]; // 重置为默认标题字体
	FontItemIndex = 0;
	fontItem = MISC_FONT_ITEM_VALUES[0]; // 重置为默认项目字体
	FontWantedIndex = 0;
	fontWanted = MISC_FONT_WANTED_VALUES[0]; // 重置为默认通缉字体
	MenuItemsCountIndex = 0;
	itemsPerLine = MISC_MENU_ITEMS_COUNT_VALUES[0]; // 重置为默认菜单显示项目数
	FontStatusIndex = 0;
	fontStatus = MISC_FONT_STATUS_VALUES[0]; // 重置为默认状态字体

	// 重置菜单布局设置
	reset_menu_layout_to_defaults();

	//featureControllerIgnoreInTrainer = false;
	//featureBlockInputInMenu = false;
	featureShowVehiclePreviews = true;
	featureShowStatusMessage = true;
	airbrake_enable = true;
	show_transparency = true;
	featureFirstPersonCutscene = false;
	mouse_view_control = false;
	help_showing = true;
	frozen_time = false;
	featurePhoneBillEnabled = false;
	featureGamePause = false;
	featureZeroBalance = false;
	featurePhone3DOnBike = false;
	featureNoPhoneOnHUD = false;
	featureFirstPersonDeathCamera = false;
	featureFirstPersonStuntJumpCamera = false;
	featureNoStuntJumps = false;
	featureHidePlayerInfo = false;
	featureShowFPS = false;
	featurenowheelblurslow = false;
	featureNoAutoRespawn = false;
	featureRealisticRadioVolume = false;

	featureRadioFreezeUpdated =
	featureMiscHideHudUpdated =
	featureBoostRadio = true;

	ENTColor::reset_colors();
}

void update_misc_features(BOOL playerExists, Ped playerPed){
	// 收音机关闭
	if (NPC_RAGDOLL_VALUES[RadioOffIndex] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) radio_pressed = false;
	if (NPC_RAGDOLL_VALUES[RadioOffIndex] > 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && CONTROLS::IS_CONTROL_PRESSED(2, 85)) {
		radio_pressed = true;
		AUDIO::SET_VEHICLE_RADIO_ENABLED(PED::GET_VEHICLE_PED_IS_USING(playerPed), true);
		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
	}
	if (NPC_RAGDOLL_VALUES[RadioOffIndex] > 0 && radio_pressed == false) {
		if (NPC_RAGDOLL_VALUES[RadioOffIndex] == 1) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, false);
			}
			AUDIO::SET_USER_RADIO_CONTROL_ENABLED(false);
		}
		if (NPC_RAGDOLL_VALUES[RadioOffIndex] == 2) {
			Vehicle cur_veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				if (VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(cur_veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(cur_veh))) {
					AUDIO::SET_VEHICLE_RADIO_ENABLED(cur_veh, false);
					AUDIO::SET_USER_RADIO_CONTROL_ENABLED(false);
				}
			}
			if ((PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && !VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(cur_veh)) && !VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(cur_veh))) || !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				AUDIO::SET_VEHICLE_RADIO_ENABLED(cur_veh, true);
				AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
			}
		}
	}
	if (NPC_RAGDOLL_VALUES[RadioOffIndex] == 0) {
		AUDIO::SET_VEHICLE_RADIO_ENABLED(PED::GET_VEHICLE_PED_IS_USING(playerPed), true);
		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
		radio_pressed = false;
	}

	// 便携式收音机
	if (featurePlayerRadio || featurePlayerRadioUpdated) {
		if (featurePlayerRadio) AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		else AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
	}

	// 无通缉音乐
	if (featureWantedMusic) AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", true);
	else AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", false);
	
	// 无飞行音乐
	if (featureFlyingMusic) AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", true);
	else AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", false);
	
	// 无警察扫描器
	if (featurePoliceScanner) AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", true);
	else AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", false);
	
	// 无“任务完成”消息
	if (featureNoComleteMessage) {
		if (!SCRIPT::HAS_SCRIPT_LOADED("family3") && !SCRIPT::HAS_SCRIPT_LOADED("jewelry_heist") && !SCRIPT::HAS_SCRIPT_LOADED("family5") && !SCRIPT::HAS_SCRIPT_LOADED("wardrobe_sp") && !SCRIPT::HAS_SCRIPT_LOADED("family6"))
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("mission_stat_watcher");
	}

	// 收音机增强
	if (featureBoostRadio) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		AUDIO::SET_VEHICLE_RADIO_LOUD(playerVeh, 1);
	}
	if (!featureBoostRadio) AUDIO::SET_VEHICLE_RADIO_LOUD(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);
	
	// 一致的收音机音量
	if (featureRealisticRadioVolume && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && radio_v_checked == false) {
		Vehicle cur_v = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(1) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(1) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		//if ((VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(cur_v)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(cur_v))) && CAM::_0xEE778F8C7E1142E2(2) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		//if ((VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(cur_v)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(cur_v))) && CAM::_0xEE778F8C7E1142E2(2) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(3) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(3) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(4) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(4) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		if ((ENTITY::GET_ENTITY_MODEL(cur_v) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE") || ENTITY::GET_ENTITY_MODEL(cur_v) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE2")) && CAM::_0xEE778F8C7E1142E2(5) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		if ((ENTITY::GET_ENTITY_MODEL(cur_v) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE") || ENTITY::GET_ENTITY_MODEL(cur_v) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE2")) && CAM::_0xEE778F8C7E1142E2(5) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(6) != 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(false);
		if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(cur_v)) && CAM::_0xEE778F8C7E1142E2(6) == 4) AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		radio_v_checked = true;
	}
	if (featureRealisticRadioVolume && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && CONTROLS::IS_CONTROL_JUST_RELEASED(2, 0)) {
		WAIT(100);
		radio_v_checked = false;
	}
	if (((!featureRealisticRadioVolume && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) || (featureRealisticRadioVolume && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))) && radio_v_checked == true) {
		AUDIO::SET_FRONTEND_RADIO_ACTIVE(true);
		radio_v_checked = false;
	}
	
	// 收音机关台切换
	if (MISC_RADIO_SWITCHING_VALUES[RadioSwitchingIndex] > 0 && (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) || featurePlayerRadio || featurePlayerRadioUpdated)) {
		if (MISC_RADIO_SWITCHING_VALUES[RadioSwitchingIndex] > 1) {
			r_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - r_secs_curr) != 0) {
				r_seconds = r_seconds + 1;
				r_secs_curr = r_secs_passed;
			}
		}
		if ((MISC_RADIO_SWITCHING_VALUES[RadioSwitchingIndex] == 1 && (is_hotkey_held_veh_radio_skip() || skip_track_pressed == true)) || (MISC_RADIO_SWITCHING_VALUES[RadioSwitchingIndex] > 1 && r_seconds > MISC_RADIO_SWITCHING_VALUES[RadioSwitchingIndex])) {
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			int const stations = AUDIO::_MAX_RADIO_STATION_INDEX();
			int random_station = (rand() % stations + 0);
			AUDIO::SET_RADIO_TO_STATION_INDEX(random_station); //AUDIO::SET_VEH_RADIO_STATION(veh, AUDIO::GET_RADIO_STATION_NAME(random_station));
			r_seconds = 0;
			skip_track_pressed = false;
		}
	}

	// 警车中的收音机
	if (featurePoliceRadio) {
		//Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		//Vector3 coords_radio = ENTITY::GET_ENTITY_COORDS(playerVeh, 1);
		//Vector3 coords_radio_2 = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		if (/*(*/PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed)/* || (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(coords_radio.x, coords_radio.y, coords_radio.z, coords_radio_2.x, coords_radio_2.y, coords_radio_2.z, false) < 15 && police_radio_check))*/ 
			&& VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(playerVeh)) {
			//police_radio_check = true;
			AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, true);
			AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
			AUDIO::SET_RADIO_AUTO_UNFREEZE(true);
			AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
		}
		//if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) if (!PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed)) police_radio_check = false;
	}
	
	// 冻结收音机频道
	if (featureRadioFreeze) {
		if (AUDIO::GET_PLAYER_RADIO_STATION_INDEX() != radioStationIndex && AUDIO::GET_PLAYER_RADIO_STATION_INDEX() != 255) {
			AUDIO::SET_RADIO_TO_STATION_INDEX(radioStationIndex);
		}
	}
	else if (featureRadioFreezeUpdated) {
		// 暂时留空。
	}

	// 隐藏 HUD
	if (featureMiscHideHud/* || (featureMiscHideENTHud && menu_showing == true)*/) {
		for (int i = 0; i < 21; i++) {
			//至少在理论上...
			switch (i) {
			case 5:  //多人游戏消息
			case 10: //帮助文本
			case 11: //浮动帮助 1
			case 12: //浮动帮助 2
			case 14: //准星
			case 16: //无线电轮
			case 19: //武器轮
				continue;
			}
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(i);
		}

		UI::DISPLAY_RADAR(false);
		featureMiscHideHudUpdated = false;
	}
	else if (/*(*/featureMiscHideHudUpdated/* && !featureMiscHideENTHud) || (featureMiscHideENTHud && menu_showing == false)*/){
		UI::DISPLAY_RADAR(true);
		featureMiscHideHudUpdated = false;
	}
	
	// 如果手持电话，则显示 HUD
	if (featurePhoneShowHud) {
		if (!phone_toggle) {
			UI::DISPLAY_RADAR(false);
			featureMiscHideHudUpdated = false;
		}
		
		if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed)) {
			UI::DISPLAY_RADAR(true);
			phone_toggle = true;
		}
	
		if (!PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed)) {
			UI::DISPLAY_RADAR(false);
			phone_toggle = false;
		}
	}
	else if (!featureMiscHideHud && !featureInVehicleNoHud && !featureMarkerHud/* && !featureMiscHideENTHud*/) {
		UI::DISPLAY_RADAR(true);
		phone_toggle = false;
	}
	
	// 仅在车辆内显示 HUD
	if (featureInVehicleNoHud) {
		if (!phone_toggle_vehicle && !featurePhoneShowHud) {
			UI::DISPLAY_RADAR(false);
			featureMiscHideHudUpdated = false;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
			UI::DISPLAY_RADAR(true);
			phone_toggle_vehicle = true;
		}

		if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && !featurePhoneShowHud) {
			UI::DISPLAY_RADAR(false);
			phone_toggle_vehicle = false;
		}
	}
	else if (!featureMiscHideHud && !featurePhoneShowHud && !featureMarkerHud/* && !featureMiscHideENTHud*/) {
		UI::DISPLAY_RADAR(true);
		phone_toggle_vehicle = false;
	}
	
	// 仅在设置了标记时显示 HUD
	if (featureMarkerHud) {
		if (!phone_toggle_vehicle && !featurePhoneShowHud && !featureInVehicleNoHud) {
			UI::DISPLAY_RADAR(false);
			featureMiscHideHudUpdated = false;
		}
		bool blipFound = false;
		int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR(); // 查找标记图标
		for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator)) {
			if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4) {
				blipFound = true;
				break;
			}
		}
		if (blipFound) {
			UI::DISPLAY_RADAR(true);
			phone_toggle_vehicle = true;
		}
		if (!blipFound && !featurePhoneShowHud && !featureInVehicleNoHud) {
			UI::DISPLAY_RADAR(false);
			phone_toggle_vehicle = false;
		}
	}
	else if (!featureMiscHideHud && !featurePhoneShowHud && !featureInVehicleNoHud/* && !featureMiscHideENTHud*/) {
		UI::DISPLAY_RADAR(true);
		phone_toggle_vehicle = false;
	}

	// 禁用录制
	if (featureDisableRecording) {
		CONTROLS::DISABLE_CONTROL_ACTION(2, 170, 1); // 保存回放片段
		CONTROLS::DISABLE_CONTROL_ACTION(2, 288, 1); // 开始/停止回放录制
		CONTROLS::DISABLE_CONTROL_ACTION(2, 289, 1); // 开始/停止回放录制（次要）
		CONTROLS::DISABLE_CONTROL_ACTION(2, 302, 1); // 回放录制
	}

	// 无通知
	if (featureNoNotifications) UI::THEFEED_HIDE_THIS_FRAME();

	// 默认手机
	if (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] > -1) {
		if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && phone_toggle_defaultphone == false) {
			MOBILE::CREATE_MOBILE_PHONE(MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex]);
			phone_toggle_defaultphone = true;
		}
		if (!PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && phone_toggle_defaultphone == true) {
			MOBILE::DESTROY_MOBILE_PHONE();
			phone_toggle_defaultphone = false;
		}
	}
	
	// 骑车时使用手机
	if (featurePhone3DOnBike) {
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		if (!STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) {
			STREAMING::REQUEST_ANIM_DICT(anim_dict);
			while (!STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) WAIT(0);
		}
		Vector3 veh_s = ENTITY::GET_ENTITY_VELOCITY(PED::GET_VEHICLE_PED_IS_USING(playerPed));
		if (MISC_PHONE_FREESECONDS_VALUES[PhoneBikeAnimationIndex] == 0) {
			anim_dict = "anim@cellphone@in_car@ps";
			animation_of_d = "cellphone_text_read_base";
		}
		if (MISC_PHONE_FREESECONDS_VALUES[PhoneBikeAnimationIndex] == 3) {
			anim_dict = "cellphone@";
			animation_of_d = "cellphone_text_read_base_cover_low";
		}
		if (MISC_PHONE_FREESECONDS_VALUES[PhoneBikeAnimationIndex] == 5) {
			anim_dict = "cellphone@str";
			animation_of_d = "cellphone_text_read_a";
		}
		if (MISC_PHONE_FREESECONDS_VALUES[PhoneBikeAnimationIndex] == 10) {
			anim_dict = "cellphone@female";
			animation_of_d = "cellphone_email_read_base";
		}
		if (MISC_PHONE_FREESECONDS_VALUES[PhoneBikeAnimationIndex] == 15) {
			anim_dict = "cellphone@first_person";
			animation_of_d = "cellphone_text_read_base";
		}

		if ((PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1) && (VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh)))) && PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed)) { // PED::IS_PED_ON_ANY_BIKE(playerPed)
			
			if (featureNoPhoneOnHUD && CAM::_0xEE778F8C7E1142E2(2) == 4/* && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)*/) {
				if (PED::GET_VEHICLE_PED_IS_IN(playerPed, 1) != GAMEPLAY::GET_HASH_KEY("VERUS") && PED::GET_VEHICLE_PED_IS_IN(playerPed, 1) != GAMEPLAY::GET_HASH_KEY("SEASHARK") &&
					PED::GET_VEHICLE_PED_IS_IN(playerPed, 1) != GAMEPLAY::GET_HASH_KEY("SEASHARK2") && PED::GET_VEHICLE_PED_IS_IN(playerPed, 1) != GAMEPLAY::GET_HASH_KEY("SEASHARK3")) MOBILE::SET_MOBILE_PHONE_POSITION(10000, 10000, 10000);
			}
			
			Hash temp_Hash = -1;
			Vector3 temp_pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			
			if (STREAMING::HAS_ANIM_DICT_LOADED(anim_dict) && p_exist == false && CONTROLS::GET_CONTROL_VALUE(0, 9) == 127) { // 127 表示轮子没有转动
				WAIT(0);
				AI::TASK_PLAY_ANIM(playerPed, anim_dict, animation_of_d, 8.0, 0.0, -1, 9, 0, 0, 0, 0);
				if (!ENTITY::DOES_ENTITY_EXIST(temp_obj)) {
					if (PED::GET_PED_TYPE(playerPed) == 0 && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing"); // 迈克尔
						if (PED::GET_PED_TYPE(playerPed) == 1 && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_03"); // 富兰克林
						if ((PED::GET_PED_TYPE(playerPed) == 2 || PED::GET_PED_TYPE(playerPed) == 3) && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3))
							temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_02"); // 崔佛
						if (PED::GET_PED_TYPE(playerPed) != 0 && PED::GET_PED_TYPE(playerPed) != 1 && PED::GET_PED_TYPE(playerPed) != 2 && PED::GET_PED_TYPE(playerPed) != 3 &&
							(MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_prologue_phone");
						if (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 0) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing");
						if (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 1) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_02");
						if (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 2) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_03");
						if (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 4) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_prologue_phone");
						temp_obj = OBJECT::CREATE_OBJECT(temp_Hash, temp_pos.x, temp_pos.y, temp_pos.z, 1, true, 1);
						int PlayerIndex1 = PED::GET_PED_BONE_INDEX(playerPed, 0x6f06);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(temp_obj, playerPed, PlayerIndex1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
				}
				p_exist = true;
			}

			if (CONTROLS::IS_CONTROL_RELEASED(2, 71) && CONTROLS::IS_CONTROL_RELEASED(2, 72) && accel == true) { // 加速/刹车
				AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
				CONTROLS::DISABLE_CONTROL_ACTION(2, 71, 1);
				CONTROLS::DISABLE_CONTROL_ACTION(2, 72, 1);
				accel = false;
				p_exist = false;
			}
			if (CONTROLS::IS_CONTROL_RELEASED(2, 63) && CONTROLS::IS_CONTROL_RELEASED(2, 64)) VEHICLE::_SET_BIKE_LEAN_ANGLE(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0, 0); //  && CONTROLS::IS_CONTROL_PRESSED(2, 71)
			if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 75) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 72) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 63) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 64) ||
				(CONTROLS::IS_CONTROL_JUST_PRESSED(2, 71) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 62) && veh_s.x < 2 && veh_s.y < 2)) { // 离开/刹车/左/右/加速/下
				AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
				accel = true;
				p_exist = false;
			}
		}
		
		if ((PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1) && ((VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh))) && !PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && STREAMING::HAS_ANIM_DICT_LOADED(anim_dict))) ||
			(!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1) && STREAMING::HAS_ANIM_DICT_LOADED(anim_dict))) {
			OBJECT::DELETE_OBJECT(&temp_obj);
			AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
			//STREAMING::REMOVE_ANIM_DICT(anim_dict);
			//STREAMING::REMOVE_ANIM_DICT(animation_of_d);
			accel = false;
			p_exist = false;
		} 
	}
	
	// 锁定玩家车辆车门
	if (featureLockVehicleDoors) {
		if (featureLockVehicleDoorsUpdated == false) {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_l = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				find_nearest_vehicle();
				veh_l = temp_vehicle;
			}
			VEHICLE::SET_VEHICLE_DOORS_LOCKED(veh_l, 4);
		}
		featureLockVehicleDoorsUpdated = true;
		PED::SET_PED_CAN_BE_DRAGGED_OUT(VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh_l, -1), false);
	}
	if (!featureLockVehicleDoors && featureLockVehicleDoorsUpdated == true) {
		VEHICLE::SET_VEHICLE_DOORS_LOCKED(veh_l, 0);
		PED::SET_PED_CAN_BE_DRAGGED_OUT(VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh_l, -1), true);

		featureLockVehicleDoorsUpdated = false;
	}

	// 动态健康条
	if (featureDynamicHealthBar && ENTITY::DOES_ENTITY_EXIST(playerPed) && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && !DLC2::GET_IS_LOADING_SCREEN_ACTIVE() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS() && dynamic_loading == true && apply_pressed == false) {
		temp_h = ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID()) - 100;
		temp_h_d = floor(ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID()) / 100);
		oldplayerPed = playerPed;
		dynamic_loading = false;
	}
	if (featureDynamicHealthBar && !CUTSCENE::IS_CUTSCENE_PLAYING() && ENTITY::DOES_ENTITY_EXIST(playerPed) && !DLC2::GET_IS_LOADING_SCREEN_ACTIVE() && !STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
		if (!featureMiscHideHud && !featurePhoneShowHud && !featureInVehicleNoHud && !featureMarkerHud/* && !featureMiscHideENTHud*/) UI::DISPLAY_RADAR(false); // 如果 HUD 已经隐藏，就没有必要再次隐藏
		//auto addr = getScriptHandleBaseAddress(playerPed);
		//float health = (*(float *)(addr + 0x280)) - 100;
		float health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
		float playerArmour = PED::GET_PED_ARMOUR(playerPed);

		if (!ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(playerPed)) {
			//curr_damaged_health = (*(float *)(addr + 0x280)) - 100;
			float health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
			curr_damaged_armor = PED::GET_PED_ARMOUR(playerPed);
		}
		if (curr_damaged_health != health || curr_damaged_armor != playerArmour) {
			healthbar_seconds = -1;
			been_damaged = true;
			curr_damaged_health = health;
			curr_damaged_armor = playerArmour;
		}
		if (been_damaged == true) {
			secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - healthbar_secs_curr) != 0) {
				healthbar_seconds = healthbar_seconds + 1;
				healthbar_secs_curr = secs_passed;
			}
			if (healthbar_seconds == 15) {
				been_damaged = false;
				healthbar_seconds = -1;
			}
			// 生命值
			if (health < (temp_h / 5)) {
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 41, 56, 40, 245); // 220, 20, 20, 245 // 55
				if ((health_bar_x + ((health / temp_h_d) / (temp_h / temp_h_d / 0.070))) > 0.015)
					GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + ((health / temp_h_d) / (temp_h / temp_h_d / 0.035)), health_bar_y + 0.01, ((health / temp_h_d) / (temp_h / temp_h_d / 0.070)), 0.009, 220, 20, 20, 255);
			}
			else {
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 41, 56, 40, 245); // 75
				if (((health / temp_h_d) / (temp_h / temp_h_d / 0.070)) < 0.070)
					GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + ((health / temp_h_d) / (temp_h / temp_h_d / 0.035)), health_bar_y + 0.01, ((health / temp_h_d) / (temp_h / temp_h_d / 0.070)), 0.009, 78, 150, 77, 255);
				else GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 78, 150, 77, 255);
			}

			GRAPHICS::DRAW_RECT(health_bar_x + 0.071, health_bar_y + 0.01, 0.001, 0.009, 255, 170, 110, 255); // 垂直条 // 0.017

			// 装甲
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.017, 38, 85, 87, 110); // health_bar_x + 0.0880 // 0.036
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 39, 55, 56, 245); // 90
			if ((playerArmour / 2935) < 0.035) GRAPHICS::DRAW_RECT(health_bar_x + 0.0715 + (playerArmour / 5871), health_bar_y + 0.01, (playerArmour / 2935), 0.009, 62, 129, 164, 255);
			else GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 62, 129, 164, 255);
		}
	}

	// 默认菜单标签
	if (MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex] > -2 && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) == 1 && !CUTSCENE::IS_CUTSCENE_PLAYING() && keyboard_on_screen_already == false) {
		int GetHash = GAMEPLAY::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE");
		if (IsKeyDown(VK_ESCAPE) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_PAUSE)/* || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 199) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, 200)*/) {
			UI::ACTIVATE_FRONTEND_MENU(GetHash, featureGamePause, MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex]);
			AUDIO::SET_AUDIO_FLAG("PlayMenuMusic", true);
		} else AUDIO::SET_AUDIO_FLAG("PlayMenuMusic", false);
	} 
	
	// 没有脚本化模糊和慢动作
	if (!featurenowheelblurslow) no_blur_initialized = false;
	if (featurenowheelblurslow && (CONTROLS::IS_CONTROL_PRESSED(2, 37) || CONTROLS::IS_CONTROL_PRESSED(2, 85) || CONTROLS::IS_CONTROL_PRESSED(2, 19))) { // 武器/收音机/角色轮盘
		if (no_blur_initialized == false) {
			initialize();
			no_blur_initialized = true;
		}
		GAMEPLAY::SET_TIME_SCALE(1.0f);
		GRAPHICS::_STOP_ALL_SCREEN_EFFECTS();
	}

	// 无电话 && 如果菜单打开则禁用电话
	if (featureDisablePhone || (featureDisablePhoneMenu && menu_showing == true)) {
		if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed)) CONTROLS::_SET_CONTROL_NORMAL(0, 177, 1);
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");
		no_phone = true;
	}
	if ((!featureDisablePhone && no_phone == true && !featureDisablePhoneMenu) || (featureDisablePhoneMenu && menu_showing == false && !featureDisablePhone && no_phone == true) || (!featureDisablePhoneMenu && no_phone == true && !featureDisablePhone)) {
		SCRIPT::REQUEST_SCRIPT("cellphone_controller");
		SYSTEM::START_NEW_SCRIPT("cellphone_controller", 1424);
		no_phone = false;
	}

	// 电话账单
	if (featurePhoneBillEnabled) {
		float mins = -1;
		if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && AUDIO::IS_MOBILE_PHONE_CALL_ONGOING()) {
			secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - secs_curr) != 0) {
				temp_seconds = temp_seconds + 1;
				if (temp_seconds > MISC_PHONE_FREESECONDS_VALUES[PhoneFreeSecondsIndex]) bill_seconds = bill_seconds + 1;
				secs_curr = secs_passed;
			}
		}
		if (!AUDIO::IS_MOBILE_PHONE_CALL_ONGOING() && temp_seconds != 0) temp_seconds = 0;
		if (!AUDIO::IS_MOBILE_PHONE_CALL_ONGOING() && bill_seconds > 0) { 
			int outValue_your_phone_bill = -1;
			int statHash_all_your_money = -1;
			mins = bill_seconds / 60.0;
			bill_to_pay = MISC_PHONE_BILL_VALUES[PhoneBillIndex] * mins;
			
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO) {
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP0_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE) {
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP1_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO) {
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP2_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			temp_seconds = 0;
			bill_seconds = 0;
		}
		if (featureZeroBalance && !SCRIPT::HAS_SCRIPT_LOADED("prologue1")) {
			int outValue_your_phone_bill = -1;
			int statHash_all_your_money = -1;
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO) {
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP0_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
					GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");
					bill_no_phone = true;
				}
				else if (bill_no_phone == true) {
					SCRIPT::REQUEST_SCRIPT("cellphone_controller");
					SYSTEM::START_NEW_SCRIPT("cellphone_controller", 1424);
					bill_no_phone = false;
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE) {
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP1_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
					GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");
					bill_no_phone = true;
				}
				else if (bill_no_phone == true) {
					SCRIPT::REQUEST_SCRIPT("cellphone_controller");
					SYSTEM::START_NEW_SCRIPT("cellphone_controller", 1424);
					bill_no_phone = false;
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO) {
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP2_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
					GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("cellphone_controller");
					bill_no_phone = true;
				}
				else if (bill_no_phone == true) {
					SCRIPT::REQUEST_SCRIPT("cellphone_controller");
					SYSTEM::START_NEW_SCRIPT("cellphone_controller", 1424);
					bill_no_phone = false;
				}
			}
		}
		if ((!featureZeroBalance && bill_no_phone == true) || (featureZeroBalance && SCRIPT::HAS_SCRIPT_LOADED("prologue1") && bill_no_phone == true)) {
			SCRIPT::REQUEST_SCRIPT("cellphone_controller");
			SYSTEM::START_NEW_SCRIPT("cellphone_controller", 1424);
			bill_no_phone = false;
		}
	}
	
	// 第一人称特技跳跃镜头
	if (featureFirstPersonStuntJumpCamera) {
		if (GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS()) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(PED::GET_VEHICLE_PED_IS_USING(playerPed), 2);
			if (!CAM::DOES_CAM_EXIST(StuntCam)) {
				StuntCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);

				if (!PED::IS_PED_ON_ANY_BIKE(playerPed)) CAM::ATTACH_CAM_TO_PED_BONE(StuntCam, playerPed, 31086, 0, -0.15, 0.05, 1); 
				if (PED::IS_PED_ON_ANY_BIKE(playerPed)) CAM::ATTACH_CAM_TO_PED_BONE(StuntCam, playerPed, 31086, 0, -0.15, -0.10, 1); 
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(StuntCam, 1.0);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(StuntCam, 1.0);
				CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(StuntCam, 1.0);
				CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
				CAM::SET_CAM_ACTIVE(StuntCam, true);
				CAM::SET_CAM_NEAR_CLIP(StuntCam, .329);
			}
			CAM::SET_CAM_ROT(StuntCam, curRotation.x, curRotation.y, curRotation.z, 2);
		}

		if (!GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS() && CAM::DOES_CAM_EXIST(StuntCam)) {
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), 1, 1);
			CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
			CAM::DETACH_CAM(StuntCam);
			CAM::SET_CAM_ACTIVE(StuntCam, false);
			CAM::DESTROY_CAM(StuntCam, true);
		}
	}
		
	// 当前是否正在播放过场动画？
	if (cutscene_is_playing == true) {
		con_disabled = true;
	}
	else { 
		if (con_disabled == true && manual_cutscene == true && GAMEPLAY::GET_MISSION_FLAG() == 0) {
			con_disabled = false;
			manual_cutscene = false;
		}
		OBJECT::DELETE_OBJECT(&xaxis);
		OBJECT::DELETE_OBJECT(&zaxis);
		if (CAM::DOES_CAM_EXIST(CutCam)) {
			CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
			CAM::DESTROY_CAM(CutCam, true);
		}
		curr_cut_ped_me = -1;
		my_first_coords = -1;
		curr_cut_ped = -1;
		cutscene_being_watched = false;
		found_ped_in_cutscene = false;
		switched_c = -1;
	}
	if (cutscene_is_playing == true && CUTSCENE::IS_CUTSCENE_PLAYING()) cutscene_being_watched = true;
	if (cutscene_being_watched == true && (!CUTSCENE::IS_CUTSCENE_PLAYING() || ((CUTSCENE::GET_CUTSCENE_TOTAL_DURATION() - CUTSCENE::GET_CUTSCENE_TIME() < 3000) && CAM::IS_SCREEN_FADING_OUT() && manual_cutscene == true))) { // && CUTSCENE::HAS_CUTSCENE_FINISHED()
		if (manual_cutscene == true) {
			CAM::DO_SCREEN_FADE_IN(0);
			CUTSCENE::STOP_CUTSCENE_IMMEDIATELY();
			CUTSCENE::REMOVE_CUTSCENE();
			CAM::DO_SCREEN_FADE_IN(0);
		}
		cutscene_is_playing = false;
		cutscene_being_watched = false;
	}
	if (CUTSCENE::IS_CUTSCENE_PLAYING()) cutscene_is_playing = true;
	if (CUTSCENE::IS_CUTSCENE_PLAYING() && manual_cutscene == true && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) stop_cutscene();

	// 第一人称过场动画镜头
	if (featureFirstPersonCutscene) {
		if (CUTSCENE::IS_CUTSCENE_PLAYING()) {
			Vector3 Pedrotation = ENTITY::GET_ENTITY_ROTATION(curr_cut_ped, 2);
			int PlayerIndex = PED::GET_PED_BONE_INDEX(curr_cut_ped, 8433);
			int PedHash = GAMEPLAY::GET_HASH_KEY("bot_01b_bit_03"); // prop_wardrobe_door_01
			Vector3 Ped1Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped, 0.0f, 1.0f, 0.0f);
			Vector3 Ped2Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped, 0.0f, 2.0f, 0.0f);
			
			if (!CAM::DOES_CAM_EXIST(CutCam)) { 
				const int US_ARR_PED_SIZE = 1024;
				Ped us_ped[US_ARR_PED_SIZE];
				int found_ped = worldGetAllPeds(us_ped, US_ARR_PED_SIZE);
				for (int i = 0; i < found_ped; i++) {
					if (ENTITY::IS_ENTITY_ON_SCREEN(us_ped[i]) && (ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_zero") ||
						ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_one") || ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_two") ||
						ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"mp_f_freemode_01") || ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"mp_m_freemode_01")) && found_ped_in_cutscene == false &&
						ENTITY::IS_ENTITY_VISIBLE(us_ped[i]) && switched_c != us_ped[i] && PED::GET_PED_TYPE(us_ped[i]) != 28) {
						curr_cut_ped_me = us_ped[i];
						my_first_coords = us_ped[i];
						PlayerIndex = PED::GET_PED_BONE_INDEX(curr_cut_ped, 8433);
						Ped1Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped_me, 0.0f, 1.0f, 0.0f);
						Ped2Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped_me, 0.0f, 2.0f, 0.0f);
						xaxis = OBJECT::CREATE_OBJECT(PedHash, Ped1Coords.x, Ped1Coords.y, Ped1Coords.z, 1, true, 1);
						zaxis = OBJECT::CREATE_OBJECT(PedHash, Ped2Coords.x, Ped2Coords.y, Ped2Coords.z, 1, true, 1);
						ENTITY::SET_ENTITY_VISIBLE(xaxis, false);
						ENTITY::SET_ENTITY_VISIBLE(zaxis, false);
						ENTITY::SET_ENTITY_COLLISION(xaxis, false, true);
						ENTITY::SET_ENTITY_COLLISION(zaxis, false, true);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(xaxis, curr_cut_ped_me, PlayerIndex, 0.0f, 0.0f, -0.1f, 105.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
						ENTITY::ATTACH_ENTITY_TO_ENTITY(zaxis, curr_cut_ped_me, PlayerIndex, 0.0f, 0.08f, -0.1f, 50.0f, 0.0f, 0.0f, false, false, false, true, 0, true);

						Vector3 coordsPed = ENTITY::GET_ENTITY_COORDS(curr_cut_ped_me, true);
						CutCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", coordsPed.x, coordsPed.y, coordsPed.z, Pedrotation.x, Pedrotation.y, Pedrotation.z, 50.0, true, 2);
						CAM::ATTACH_CAM_TO_ENTITY(CutCam, zaxis, 0, 0, 0, true);
						CAM::SET_CAM_NEAR_CLIP(CutCam, .229); // 329
					}
				}
			}
			if (CAM::DOES_CAM_EXIST(CutCam)) {
				if (cutscene_being_watched == true && found_ped_in_cutscene == false) {
					const int US_ARR_PED_SIZE = 1024;
					Ped us_ped[US_ARR_PED_SIZE];
					int found_ped = worldGetAllPeds(us_ped, US_ARR_PED_SIZE);
					for (int i = 0; i < found_ped; i++) {
						Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(my_first_coords, true);
						Vector3 coordsPed_temp = ENTITY::GET_ENTITY_COORDS(us_ped[i], true);
						float dist_t = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsPed_temp.x, coordsPed_temp.y, coordsPed_temp.z);
						if (dist_t < 10) { // 20 
							if (ENTITY::IS_ENTITY_ON_SCREEN(us_ped[i]) && found_ped_in_cutscene == false &&
								ENTITY::IS_ENTITY_VISIBLE(us_ped[i]) && switched_c != us_ped[i] && PED::GET_PED_TYPE(us_ped[i]) != 28) { 
								if (curr_cut_ped_me != -1) curr_cut_ped = curr_cut_ped_me;
								else curr_cut_ped = us_ped[i];
								OBJECT::DELETE_OBJECT(&xaxis);
								OBJECT::DELETE_OBJECT(&zaxis);

								CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
								CAM::DESTROY_CAM(CutCam, true);
								
								if (ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_zero") || ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_one") ||
									ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_two")) PlayerIndex = PED::GET_PED_BONE_INDEX(curr_cut_ped, 8433);
								else PlayerIndex = PED::GET_PED_BONE_INDEX(curr_cut_ped, 31086); // 8433
								Ped1Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped, 0.0f, 1.0f, 0.0f);
								Ped2Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(curr_cut_ped, 0.0f, 2.0f, 0.0f);
								xaxis = OBJECT::CREATE_OBJECT(PedHash, Ped1Coords.x, Ped1Coords.y, Ped1Coords.z, 1, true, 1);
								zaxis = OBJECT::CREATE_OBJECT(PedHash, Ped2Coords.x, Ped2Coords.y, Ped2Coords.z, 1, true, 1);
								ENTITY::SET_ENTITY_VISIBLE(xaxis, false);
								ENTITY::SET_ENTITY_VISIBLE(zaxis, false);
								ENTITY::SET_ENTITY_COLLISION(xaxis, false, true);
								ENTITY::SET_ENTITY_COLLISION(zaxis, false, true);
								if (ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_zero") || ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_one") ||
									ENTITY::GET_ENTITY_MODEL(us_ped[i]) == GAMEPLAY::GET_HASH_KEY((char *)"player_two")) {
									ENTITY::ATTACH_ENTITY_TO_ENTITY(xaxis, curr_cut_ped, PlayerIndex, 0.0f, 0.0f, -0.1f, 105.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
									ENTITY::ATTACH_ENTITY_TO_ENTITY(zaxis, curr_cut_ped, PlayerIndex, 0.0f, 0.08f, -0.1f, 50.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
								}
								else {
									ENTITY::ATTACH_ENTITY_TO_ENTITY(xaxis, curr_cut_ped, PlayerIndex, 0.0f, 0.0f, -0.1f, 105.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
									ENTITY::ATTACH_ENTITY_TO_ENTITY(zaxis, curr_cut_ped, PlayerIndex, 0.0f, 0.08f, -0.1f, 0.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
								}

								Vector3 coordsPed = ENTITY::GET_ENTITY_COORDS(curr_cut_ped, true);
								CutCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", coordsPed.x, coordsPed.y, coordsPed.z, Pedrotation.x, Pedrotation.y, Pedrotation.z, 50.0, true, 2);
								CAM::ATTACH_CAM_TO_ENTITY(CutCam, zaxis, 0, 0, 0, true);
								CAM::SET_CAM_NEAR_CLIP(CutCam, .229); // 329
								curr_cut_ped_me = -1;
								switched_c = curr_cut_ped;
								found_ped_in_cutscene = true;
							}
						}
					}
				} 
				if (!ENTITY::DOES_ENTITY_EXIST(curr_cut_ped)) {
					OBJECT::DELETE_OBJECT(&xaxis);
					OBJECT::DELETE_OBJECT(&zaxis);
					if (CAM::DOES_CAM_EXIST(CutCam)) {
						CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
						CAM::DESTROY_CAM(CutCam, true);
					}
					found_ped_in_cutscene = false;
					switched_c = -1;
				}
				if (CAM::DOES_CAM_EXIST(CutCam)) {
					CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(CutCam, 1.0);
					CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(CutCam, 1.0);
					CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(CutCam, 1.0);
					CAM::RENDER_SCRIPT_CAMS(true, false, 1, false, false);

					CAM::STOP_CUTSCENE_CAM_SHAKING();
					CUTSCENE::CAN_SET_EXIT_STATE_FOR_CAMERA(1);
					Vector3 Ped1rotation = ENTITY::GET_ENTITY_ROTATION(xaxis, 2);
					Vector3 Ped2rotation = ENTITY::GET_ENTITY_ROTATION(zaxis, 2);
					CAM::SET_CAM_ROT(CutCam, Ped1rotation.x, Pedrotation.y, Ped2rotation.z, 2);
				}
			}
		} // if 语句的结束 (CUTSCENE::IS_CUTSCENE_PLAYING())
		else if (cutscene_is_playing == false) {
			if (con_disabled == true && manual_cutscene == true) {
				con_disabled = false;
				manual_cutscene = false;
			}
			OBJECT::DELETE_OBJECT(&xaxis);
			OBJECT::DELETE_OBJECT(&zaxis);
			if (CAM::DOES_CAM_EXIST(CutCam)) {
				CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
				CAM::DESTROY_CAM(CutCam, true);
			}
			found_ped_in_cutscene = false;
			switched_c = -1;
		}
	} // if 语句的结束 (featureFirstPersonCutscene)
	else {
		OBJECT::DELETE_OBJECT(&xaxis);
		OBJECT::DELETE_OBJECT(&zaxis);
		if (CAM::DOES_CAM_EXIST(CutCam)) {
			CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
			CAM::DESTROY_CAM(CutCam, true);
		}
		found_ped_in_cutscene = false;
		switched_c = -1;
	}

	// 无特技跳跃
	if (featureNoStuntJumps && GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS()) GAMEPLAY::CANCEL_STUNT_JUMP();

	// FPS 计数器
	if (featureShowFPS)	{
		FPStime_passed = clock() / CLOCKS_PER_SEC;
		if (((clock() / CLOCKS_PER_SEC) - FPStime_curr) != 0) {
			FPStime = FPStime + 1;
			FPStime_curr = FPStime_passed;
		}

		frames++;
		
		if (FPStime - starttime > 0.05 && frames > 10) { // 0.25
			fps = (double)frames / (FPStime - starttime);
			if ((FPStime - starttime) >= 0) starttime = FPStime;
			frames = 0;
		}
			
		sprintf(fps_to_show_char_modifiable, "%d", fps); // 将 fps 值格式化为字符串并存储到 fps_to_show_char_modifiable 中  
		UI::SET_TEXT_FONT(fontStatus); // 设置 FPS 字体类型 4
		UI::SET_TEXT_SCALE(0.0, 0.45); // 设置文本的缩放比例，宽度为 0.0，高度为 0.45  
		UI::SET_TEXT_PROPORTIONAL(1); // 启用文本的比例缩放，保持文本比例不变  
		UI::SET_TEXT_COLOUR(255, 242, 0, 255); // 设置文本颜色为黄色（RGB: 255, 242, 0），透明度为255（不透明）  
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255); // 设置文本边缘效果，类型为 3，颜色为黑色（RGB: 0, 0, 0），透明度为255  
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255); // 设置文本阴影效果，偏移量为 10，颜色为黑色（RGB: 10, 10, 10），透明度为255  
		UI::SET_TEXT_OUTLINE(); // 启用文本描边效果  
		UI::_SET_TEXT_ENTRY("STRING"); // 设置文本条目类型为字符串  
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(fps_to_show_char_modifiable); // 将 fps_to_show_char_modifiable 中的文本添加到文本组件中  
		UI::_DRAW_TEXT(0.005, 0.135); // 在屏幕坐标 ( x=0.005, y=0.135 ) 处绘制文本  
		GRAPHICS::DRAW_RECT(0.0, 0.15, 0.05, 0.03, 10, 10, 10, 180); 
		// 在屏幕坐标 ( x=0.0, y=0.15 ) 处绘制一个矩形，宽度为 0.05，高度为 0.03，颜色为深灰色（RGB: 10, 10, 10），透明度为 150  
	}
	
	// 在暂停菜单中隐藏玩家信息
	if (featureHidePlayerInfo) UI::_SET_DIRECTOR_MODE(true);
	else UI::_SET_DIRECTOR_MODE(false);
		
	// 启用1.44版本的新电台。感谢Sjaak发现了这一点！
	if (featureEnableMissingRadioStation)
	{
		if (!iterated_radio_stations) missing_station = missing_station + 1;
		if (missing_station > 300) { // 1000
			int version = getGameVersion();
			if ((version > 41 || version == -1))
			{
				//if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && !iterated_radio_stations)
				if (!iterated_radio_stations)
				{
					for (int i = 0; i < 100; i++)
					{
						char* radio_station = AUDIO::GET_RADIO_STATION_NAME(i);
						UNK3::_LOCK_RADIO_STATION(radio_station, 0);
					}
					WAIT(1000);
					iterated_radio_stations = true;
				}
			}
			else
			{
				set_status_text("您的游戏版本过旧, 此功能\n需1.44或更高版本才能运行!");
				featureEnableMissingRadioStation = false;
			}
		}
	}
	if (!featureEnableMissingRadioStation)
	{
		iterated_radio_stations = false;
		missing_station = 0;
	}

	if (sfilter_enabled == false && screenfltr != "DEFAULT" && screenfltr != "") {
		GRAPHICS::SET_TIMECYCLE_MODIFIER((char*)screenfltr.c_str());
		GRAPHICS::SET_TIMECYCLE_MODIFIER_STRENGTH(1.0f);
		sfilter_enabled = true;
	}
	if (DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) sfilter_enabled = false;

	// 处理按键输入模式
	handle_key_input_mode();
}

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated });
	results->push_back(FeatureEnabledLocalDefinition{"featureRadioFreeze", &featureRadioFreeze, &featureRadioFreezeUpdated });
	results->push_back(FeatureEnabledLocalDefinition{"featureBoostRadio", &featureBoostRadio }); 
	results->push_back(FeatureEnabledLocalDefinition{"featureRealisticRadioVolume", &featureRealisticRadioVolume});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedMUsic", &featureWantedMusic}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureDisablePhone", &featureDisablePhone});
	results->push_back(FeatureEnabledLocalDefinition{"featureDisablePhoneMenu", &featureDisablePhoneMenu});
	results->push_back(FeatureEnabledLocalDefinition{"featureFlyingMusic", &featureFlyingMusic}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceScanner", &featurePoliceScanner}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNoComleteMessage", &featureNoComleteMessage}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceRadio", &featurePoliceRadio}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscLockRadio", &featureMiscLockRadio});
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscHideHud", &featureMiscHideHud, &featureMiscHideHudUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneShowHud", &featurePhoneShowHud}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureInVehicleNoHud", &featureInVehicleNoHud});
	results->push_back(FeatureEnabledLocalDefinition{"featureMarkerHud", &featureMarkerHud});
	results->push_back(FeatureEnabledLocalDefinition{"featureDynamicHealthBar", &featureDynamicHealthBar});
	results->push_back(FeatureEnabledLocalDefinition{"featureDisableRecording", &featureDisableRecording});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoNotifications", &featureNoNotifications});
	results->push_back(FeatureEnabledLocalDefinition{"mouse_view_control", &mouse_view_control});
	results->push_back(FeatureEnabledLocalDefinition{"airbrake_enable", &airbrake_enable});
	results->push_back(FeatureEnabledLocalDefinition{"featureFirstPersonCutscene", &featureFirstPersonCutscene});
	results->push_back(FeatureEnabledLocalDefinition{"help_showing", &help_showing});
	results->push_back(FeatureEnabledLocalDefinition{"frozen_time", &frozen_time});
	results->push_back(FeatureEnabledLocalDefinition{"show_transparency", &show_transparency});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneBillEnabled", &featurePhoneBillEnabled});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoGamePause", &featureGamePause});
	results->push_back(FeatureEnabledLocalDefinition{"featureZeroBalance", &featureZeroBalance});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhone3DOnBike", &featurePhone3DOnBike});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoPhoneOnHUD", &featureNoPhoneOnHUD});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowVehiclePreviews", &featureShowVehiclePreviews});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowStatusMessage", &featureShowStatusMessage});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoAutoRespawn", &featureNoAutoRespawn});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowFPS", &featureShowFPS});
	results->push_back(FeatureEnabledLocalDefinition{"featurenowheelblurslow", &featurenowheelblurslow});
	results->push_back(FeatureEnabledLocalDefinition{"featureHiddenRadioStation", &featureEnableMissingRadioStation});
	results->push_back(FeatureEnabledLocalDefinition{"featureFirstPersonDeathCamera", &featureFirstPersonDeathCamera});
	results->push_back(FeatureEnabledLocalDefinition{"featureFirstPersonStuntJumpCamera", &featureFirstPersonStuntJumpCamera});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoStuntJumps", &featureNoStuntJumps});
	results->push_back(FeatureEnabledLocalDefinition{"featureHidePlayerInfo", &featureHidePlayerInfo});
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscJellmanScenery", &featureMiscJellmanScenery});
	//results->push_back(FeatureEnabledLocalDefinition{"featureControllerIgnoreInTrainer", &featureControllerIgnoreInTrainer});
	//results->push_back(FeatureEnabledLocalDefinition{"featureBlockInputInMenu", &featureBlockInputInMenu});
}

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"radioStationIndex", std::to_string(radioStationIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneBillIndex", std::to_string(PhoneBillIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneDefaultIndex", std::to_string(PhoneDefaultIndex)});
	results->push_back(StringPairSettingDBRow{"RadioOffIndex", std::to_string(RadioOffIndex)});
	results->push_back(StringPairSettingDBRow{"RadioSwitchingIndex", std::to_string(RadioSwitchingIndex)});
	results->push_back(StringPairSettingDBRow{"TrainerControlIndex", std::to_string(TrainerControlIndex)});
	results->push_back(StringPairSettingDBRow{"TrainerControlScrollingIndex", std::to_string(TrainerControlScrollingIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneFreeSecondsIndex", std::to_string(PhoneFreeSecondsIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneBikeAnimationIndex", std::to_string(PhoneBikeAnimationIndex)});
	results->push_back(StringPairSettingDBRow{"DefMenuTabIndex", std::to_string(DefMenuTabIndex)});
	results->push_back(StringPairSettingDBRow{"FontHeaderIndex", std::to_string(FontHeaderIndex)});
	results->push_back(StringPairSettingDBRow{"FontItemIndex", std::to_string(FontItemIndex)});
	results->push_back(StringPairSettingDBRow{"FontWantedIndex", std::to_string(FontWantedIndex)});
	results->push_back(StringPairSettingDBRow{"FontStatusIndex", std::to_string(FontStatusIndex)});
	results->push_back(StringPairSettingDBRow{"MenuItemsCountIndex", std::to_string(MenuItemsCountIndex)});
	// 添加菜单布局设置
	results->push_back(StringPairSettingDBRow{"MenuWidthIndex", std::to_string(MenuWidthIndex)});
	results->push_back(StringPairSettingDBRow{"MenuHeightIndex", std::to_string(MenuHeightIndex)});
	results->push_back(StringPairSettingDBRow{"MenuTopOffsetIndex", std::to_string(MenuTopOffsetIndex)});
	results->push_back(StringPairSettingDBRow{"MenuLeftOffsetIndex", std::to_string(MenuLeftOffsetIndex)});
	results->push_back(StringPairSettingDBRow{"MenuTextLeftOffsetIndex", std::to_string(MenuTextLeftOffsetIndex)});
	results->push_back(StringPairSettingDBRow{"MenuItemHeightIndex", std::to_string(MenuItemHeightIndex)});
	results->push_back(StringPairSettingDBRow{"MenuItemSpacingIndex", std::to_string(MenuItemSpacingIndex)});
	results->push_back(StringPairSettingDBRow{"MenuItemTextOffsetIndex", std::to_string(MenuItemTextOffsetIndex)});
	results->push_back(StringPairSettingDBRow{"MenuItemTopOffsetIndex", std::to_string(MenuItemTopOffsetIndex)});
	// 添加预览图设置
	results->push_back(StringPairSettingDBRow{"PreviewPositionThresholdIndex", std::to_string(PreviewPositionThresholdIndex)});
	results->push_back(StringPairSettingDBRow{"PreviewResolutionScaleIndex", std::to_string(PreviewResolutionScaleIndex)});
	results->push_back(StringPairSettingDBRow{"PreviewSpacingIndex", std::to_string(PreviewSpacingIndex)});
	results->push_back(StringPairSettingDBRow{"screenfltr", screenfltr});
}

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings){
	for(int a = 0; a < settings->size(); a++){
		StringPairSettingDBRow setting = settings->at(a);
		if(setting.name.compare("radioStationIndex") == 0){
			radioStationIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PhoneBillIndex") == 0){
			PhoneBillIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PhoneDefaultIndex") == 0) {
			PhoneDefaultIndex = stoi(setting.value);
		}
		else if (setting.name.compare("RadioOffIndex") == 0) {
			RadioOffIndex = stoi(setting.value);
		}
		else if (setting.name.compare("RadioSwitchingIndex") == 0) {
			RadioSwitchingIndex = stoi(setting.value);
		}
		else if (setting.name.compare("TrainerControlIndex") == 0) {
			TrainerControlIndex = stoi(setting.value);
		}
		else if (setting.name.compare("TrainerControlScrollingIndex") == 0) {
			TrainerControlScrollingIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PhoneFreeSecondsIndex") == 0){
			PhoneFreeSecondsIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PhoneBikeAnimationIndex") == 0) {
			PhoneBikeAnimationIndex = stoi(setting.value);
		}
		else if (setting.name.compare("DefMenuTabIndex") == 0) {
			DefMenuTabIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FontHeaderIndex") == 0) {
            FontHeaderIndex = stoi(setting.value);
            fontHeader = MISC_FONT_HEADER_VALUES[FontHeaderIndex];
        }
		else if (setting.name.compare("FontItemIndex") == 0) {
            FontItemIndex = stoi(setting.value);
            fontItem = MISC_FONT_ITEM_VALUES[FontItemIndex];
        }
		else if (setting.name.compare("FontWantedIndex") == 0) {
            FontWantedIndex = stoi(setting.value);
            fontWanted = MISC_FONT_WANTED_VALUES[FontWantedIndex];
        }
		else if (setting.name.compare("FontStatusIndex") == 0) {
            FontStatusIndex = stoi(setting.value);
            fontStatus = MISC_FONT_STATUS_VALUES[FontStatusIndex];
        }
		else if (setting.name.compare("MenuItemsCountIndex") == 0) {
            MenuItemsCountIndex = stoi(setting.value);
            itemsPerLine = MISC_MENU_ITEMS_COUNT_VALUES[MenuItemsCountIndex];
            MenuItemsCountChanged = true;
        }
		// 添加菜单布局设置的加载
		else if (setting.name.compare("MenuWidthIndex") == 0) {
			MenuWidthIndex = stoi(setting.value);
			if (MenuWidthIndex < 0) MenuWidthIndex = 0;
			if (MenuWidthIndex >= (int)MISC_MENU_WIDTH_CAPTIONS.size()) MenuWidthIndex = (int)MISC_MENU_WIDTH_CAPTIONS.size() - 1;
			menuWidth = MISC_MENU_WIDTH_VALUES[MenuWidthIndex];
			MenuWidthChanged = true;
		}
		else if (setting.name.compare("MenuHeightIndex") == 0) {
			MenuHeightIndex = stoi(setting.value);
			if (MenuHeightIndex < 0) MenuHeightIndex = 0;
			if (MenuHeightIndex >= (int)MISC_MENU_HEIGHT_CAPTIONS.size()) MenuHeightIndex = (int)MISC_MENU_HEIGHT_CAPTIONS.size() - 1;
			menuHeight = MISC_MENU_HEIGHT_VALUES[MenuHeightIndex];
			MenuHeightChanged = true;
		}
		else if (setting.name.compare("MenuTopOffsetIndex") == 0) {
			MenuTopOffsetIndex = stoi(setting.value);
			if (MenuTopOffsetIndex < 0) MenuTopOffsetIndex = 0;
			if (MenuTopOffsetIndex >= (int)MISC_MENU_TOP_OFFSET_CAPTIONS.size()) MenuTopOffsetIndex = (int)MISC_MENU_TOP_OFFSET_CAPTIONS.size() - 1;
			menuTopOffset = MISC_MENU_TOP_OFFSET_VALUES[MenuTopOffsetIndex];
			MenuTopOffsetChanged = true;
		}
		else if (setting.name.compare("MenuLeftOffsetIndex") == 0) {
			MenuLeftOffsetIndex = stoi(setting.value);
			if (MenuLeftOffsetIndex < 0) MenuLeftOffsetIndex = 0;
			if (MenuLeftOffsetIndex >= (int)MISC_MENU_LEFT_OFFSET_CAPTIONS.size()) MenuLeftOffsetIndex = (int)MISC_MENU_LEFT_OFFSET_CAPTIONS.size() - 1;
			menuLeftOffset = MISC_MENU_LEFT_OFFSET_VALUES[MenuLeftOffsetIndex];
			MenuLeftOffsetChanged = true;
		}
		else if (setting.name.compare("MenuTextLeftOffsetIndex") == 0) {
			MenuTextLeftOffsetIndex = stoi(setting.value);
			if (MenuTextLeftOffsetIndex < 0) MenuTextLeftOffsetIndex = 0;
			if (MenuTextLeftOffsetIndex >= (int)MISC_MENU_TEXT_LEFT_OFFSET_CAPTIONS.size()) MenuTextLeftOffsetIndex = (int)MISC_MENU_TEXT_LEFT_OFFSET_CAPTIONS.size() - 1;
			menuTextLeftOffset = MISC_MENU_TEXT_LEFT_OFFSET_VALUES[MenuTextLeftOffsetIndex];
			MenuTextLeftOffsetChanged = true;
		}
		else if (setting.name.compare("MenuItemHeightIndex") == 0) {
			MenuItemHeightIndex = stoi(setting.value);
			if (MenuItemHeightIndex < 0) MenuItemHeightIndex = 0;
			if (MenuItemHeightIndex >= (int)MISC_MENU_ITEM_HEIGHT_CAPTIONS.size()) MenuItemHeightIndex = (int)MISC_MENU_ITEM_HEIGHT_CAPTIONS.size() - 1;
			menuItemHeight = MISC_MENU_ITEM_HEIGHT_VALUES[MenuItemHeightIndex];
			MenuItemHeightChanged = true;
		}
		else if (setting.name.compare("MenuItemSpacingIndex") == 0) {
			MenuItemSpacingIndex = stoi(setting.value);
			if (MenuItemSpacingIndex < 0) MenuItemSpacingIndex = 0;
			if (MenuItemSpacingIndex >= (int)MISC_MENU_ITEM_SPACING_CAPTIONS.size()) MenuItemSpacingIndex = (int)MISC_MENU_ITEM_SPACING_CAPTIONS.size() - 1;
			menuItemSpacing = MISC_MENU_ITEM_SPACING_VALUES[MenuItemSpacingIndex];
			MenuItemSpacingChanged = true;
		}
		else if (setting.name.compare("MenuItemTextOffsetIndex") == 0) {
			MenuItemTextOffsetIndex = stoi(setting.value);
			if (MenuItemTextOffsetIndex < 0) MenuItemTextOffsetIndex = 0;
			if (MenuItemTextOffsetIndex >= (int)MISC_MENU_ITEM_TEXT_OFFSET_CAPTIONS.size()) MenuItemTextOffsetIndex = (int)MISC_MENU_ITEM_TEXT_OFFSET_CAPTIONS.size() - 1;
			menuItemTextOffset = MISC_MENU_ITEM_TEXT_OFFSET_VALUES[MenuItemTextOffsetIndex];
			MenuItemTextOffsetChanged = true;
		}
		else if (setting.name.compare("MenuItemTopOffsetIndex") == 0) {
			MenuItemTopOffsetIndex = stoi(setting.value);
			if (MenuItemTopOffsetIndex < 0) MenuItemTopOffsetIndex = 0;
			if (MenuItemTopOffsetIndex >= (int)MISC_MENU_ITEM_TOP_OFFSET_CAPTIONS.size()) MenuItemTopOffsetIndex = (int)MISC_MENU_ITEM_TOP_OFFSET_CAPTIONS.size() - 1;
			menuItemTopOffset = MISC_MENU_ITEM_TOP_OFFSET_VALUES[MenuItemTopOffsetIndex];
			MenuItemTopOffsetChanged = true;
		}
		// 添加预览图设置的加载
		else if (setting.name.compare("PreviewPositionThresholdIndex") == 0) {
			PreviewPositionThresholdIndex = stoi(setting.value);
			if (PreviewPositionThresholdIndex < 0) PreviewPositionThresholdIndex = 0;
			if (PreviewPositionThresholdIndex >= (int)MISC_PREVIEW_POSITION_THRESHOLD_CAPTIONS.size()) PreviewPositionThresholdIndex = (int)MISC_PREVIEW_POSITION_THRESHOLD_CAPTIONS.size() - 1;
			previewPositionThreshold = MISC_PREVIEW_POSITION_THRESHOLD_VALUES[PreviewPositionThresholdIndex];
			PreviewPositionThresholdChanged = true;
		}
		else if (setting.name.compare("PreviewResolutionScaleIndex") == 0) {
			PreviewResolutionScaleIndex = stoi(setting.value);
			if (PreviewResolutionScaleIndex < 0) PreviewResolutionScaleIndex = 0;
			if (PreviewResolutionScaleIndex >= (int)MISC_PREVIEW_RESOLUTION_SCALE_CAPTIONS.size()) PreviewResolutionScaleIndex = (int)MISC_PREVIEW_RESOLUTION_SCALE_CAPTIONS.size() - 1;
			previewResolutionScale = MISC_PREVIEW_RESOLUTION_SCALE_VALUES[PreviewResolutionScaleIndex];
			PreviewResolutionScaleChanged = true;
		}
		else if (setting.name.compare("PreviewSpacingIndex") == 0) {
			PreviewSpacingIndex = stoi(setting.value);
			if (PreviewSpacingIndex < 0) PreviewSpacingIndex = 0;
			if (PreviewSpacingIndex >= (int)MISC_PREVIEW_SPACING_CAPTIONS.size()) PreviewSpacingIndex = (int)MISC_PREVIEW_SPACING_CAPTIONS.size() - 1;
			previewSpacing = MISC_PREVIEW_SPACING_VALUES[PreviewSpacingIndex];
			PreviewSpacingChanged = true;
		}
		else if (setting.name.compare("screenfltr") == 0) {
			screenfltr = setting.value;
		}
	}
}

bool is_vehicle_preview_enabled(){
	return featureShowVehiclePreviews;
}

//bool 菜单中是否阻止输入(){
//	return 菜单中阻止输入功能;
//}

//bool 修改器中是否忽略控制器(){
//	return 修改器中忽略控制器功能;
//}

bool is_hud_hidden(){
	return featureMiscHideHud;
}

void set_hud_hidden(bool hidden){
	featureMiscHideHud = hidden;
	featureMiscHideHudUpdated = true;
}

void set_hud_shown(bool hidden){
	featurePhoneShowHud = hidden;
}

bool is_jellman_scenery_enabled(){
	return featureMiscJellmanScenery;
}

void SkipRadioFwd1(uint32_t a1)
{
	for (int i = 0; i < *g_radioStationCount; i++)
	{
		uintptr_t radioStation = g_radioStationList[i];

		if (radioStation)
		{
			CRadioStation__Advance(radioStation, a1);
		}
	}
}

static void SkipRadioFwd2Internal(uintptr_t a1, uint32_t a2)
{
	uint64_t v2; // rax 寄存器

	if (*(uint32_t*)(a1 + 0x2BC) == 2)
	{
		v2 = *(uint64_t*)(a1 + 0x2A0);
		if (v2)
		{
			*(uint32_t*)(a1 + 0x2C0) = 0;
			*(uint8_t*)(a1 + 0x2CC) = 1;
			*(uint32_t*)(a1 + 0x2B4) = a2;
			*(uint64_t*)(a1 + 0x2A8) = v2;
			*(uint32_t*)(a1 + 0x2BC) = 5;
		}
	}
}

void SkipRadioFwd2(uint32_t a1)
{
	uint32_t v1; // esi 寄存器
	uintptr_t* v2; // rbx 寄存器
	size_t v3; // rdi 寄存器

	v1 = a1;
	v2 = g_unkRadioStationData;
	v3 = 3;
	do
	{
		SkipRadioFwd2Internal(*v2, v1);
		++v2;
		--v3;
	} while (v3);
}

void SKIP_RADIO_FORWARD_CUSTOM()
	{
		SkipRadioFwd1(300000);
		SkipRadioFwd2(300000);
	}

// 需要对此进行压缩，因为已经有一个用于可调雪地的扫描器
bool CompareMemoryJACCO(const uint8_t* pData, const uint8_t* bMask, const char* sMask)
{
	for (; *sMask; ++sMask, ++pData, ++bMask)
		if (*sMask == 'x' && *pData != *bMask)
			return false;

	return *sMask == NULL;
}

intptr_t FindPatternJACCO(const char* bMask, const char* sMask)
{
	// 游戏基址与大小
	static intptr_t pGameBase = (intptr_t)GetModuleHandle(nullptr);
	static uint32_t pGameSize = 0;
	if (!pGameSize)
	{
		MODULEINFO info;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)pGameBase, &info, sizeof(MODULEINFO));
		pGameSize = info.SizeOfImage;
	}

	// 扫描
	for (uint32_t i = 0; i < pGameSize; i++)
		if (CompareMemoryJACCO((uint8_t*)(pGameBase + i), (uint8_t*)bMask, sMask))
			return pGameBase + i;

	return 0;
}

void SInit()
{
	uintptr_t address = FindPatternJACCO("\x3B\x0D\x00\x00\x00\x00\x73\x0E\x48\x8B\x05\x00\x00\x00\x00\x8B\xC9", "xx????xxxxx????xx");

	g_radioStationList = *(uintptr_t**)(address + *(int*)(address + 11) + 15);
	g_radioStationCount = (int*)(address + *(int*)(address + 2) + 6);

	address = FindPatternJACCO("\x80\xB9\x00\x00\x00\x00\x00\x8B\xF2\x48\x8B\xD9\x0F\x85", "xx?????xxxxxxx");
	CRadioStation__Advance = (decltype(CRadioStation__Advance))(address - 15);

	address = FindPatternJACCO("\x48\x8D\x1D\x00\x00\x00\x00\xBF\x00\x00\x00\x00\x48\x83\x3B\x00", "xxx????x????xxxx");
	g_unkRadioStationData = (uintptr_t*)(address + *(int*)(address + 3) + 7);
}

// 菜单布局设置变量定义
int MenuWidthIndex = MENU_WIDTH_DEFAULT_INDEX; // 使用默认索引常量
bool MenuWidthChanged = false;

int MenuHeightIndex = MENU_HEIGHT_DEFAULT_INDEX; // 使用默认索引常量
bool MenuHeightChanged = false;

int MenuTopOffsetIndex = MENU_TOP_OFFSET_DEFAULT_INDEX; // 使用默认索引常量
bool MenuTopOffsetChanged = false;

int MenuLeftOffsetIndex = MENU_LEFT_OFFSET_DEFAULT_INDEX; // 使用默认索引常量
bool MenuLeftOffsetChanged = false;

int MenuTextLeftOffsetIndex = MENU_TEXT_LEFT_OFFSET_DEFAULT_INDEX; // 使用默认索引常量
bool MenuTextLeftOffsetChanged = false;

// 菜单项设置变量定义
int MenuItemHeightIndex = MENU_ITEM_HEIGHT_DEFAULT_INDEX; // 使用默认索引常量
bool MenuItemHeightChanged = false;

int MenuItemSpacingIndex = MENU_ITEM_SPACING_DEFAULT_INDEX; // 使用默认索引常量
bool MenuItemSpacingChanged = false;

int MenuItemTextOffsetIndex = MENU_ITEM_TEXT_OFFSET_DEFAULT_INDEX; // 使用默认索引常量
bool MenuItemTextOffsetChanged = false;

int MenuItemTopOffsetIndex = MENU_ITEM_TOP_OFFSET_DEFAULT_INDEX; // 使用默认索引常量
bool MenuItemTopOffsetChanged = false;

// 预览图设置变量定义
int PreviewPositionThresholdIndex = PREVIEW_POSITION_THRESHOLD_DEFAULT_INDEX; // 预览图左右判断依据
bool PreviewPositionThresholdChanged = false;

int PreviewResolutionScaleIndex = PREVIEW_RESOLUTION_SCALE_DEFAULT_INDEX; // 预览图分辨率适配值
bool PreviewResolutionScaleChanged = false;

int PreviewSpacingIndex = PREVIEW_SPACING_DEFAULT_INDEX; // 预览图间距
bool PreviewSpacingChanged = false;

// 菜单布局设置相关函数实现
void onchange_misc_menu_width_index(int value, SelectFromListMenuItem* source) {
    MenuWidthIndex = value;
    menuWidth = MISC_MENU_WIDTH_VALUES[value]; // 更新全局菜单宽度变量
    MenuWidthChanged = true;
}

void onchange_misc_menu_height_index(int value, SelectFromListMenuItem* source) {
    MenuHeightIndex = value;
    menuHeight = MISC_MENU_HEIGHT_VALUES[value]; // 更新全局菜单高度变量
    MenuHeightChanged = true;
}

void onchange_misc_menu_top_offset_index(int value, SelectFromListMenuItem* source) {
    MenuTopOffsetIndex = value;
    menuTopOffset = MISC_MENU_TOP_OFFSET_VALUES[value]; // 更新全局菜单顶部偏移量变量
    MenuTopOffsetChanged = true;
}

void onchange_misc_menu_left_offset_index(int value, SelectFromListMenuItem* source) {
    MenuLeftOffsetIndex = value;
    menuLeftOffset = MISC_MENU_LEFT_OFFSET_VALUES[value]; // 更新全局菜单左侧偏移量变量
    MenuLeftOffsetChanged = true;
}

void onchange_misc_menu_text_left_offset_index(int value, SelectFromListMenuItem* source) {
    MenuTextLeftOffsetIndex = value;
    menuTextLeftOffset = MISC_MENU_TEXT_LEFT_OFFSET_VALUES[value]; // 更新全局菜单文本左侧偏移量变量
    MenuTextLeftOffsetChanged = true;
}

// 菜单项设置相关函数实现
void onchange_misc_menu_item_height_index(int value, SelectFromListMenuItem* source) {
    MenuItemHeightIndex = value;
    menuItemHeight = MISC_MENU_ITEM_HEIGHT_VALUES[value]; // 更新全局菜单项高度变量
    MenuItemHeightChanged = true;
}

void onchange_misc_menu_item_spacing_index(int value, SelectFromListMenuItem* source) {
    MenuItemSpacingIndex = value;
    menuItemSpacing = MISC_MENU_ITEM_SPACING_VALUES[value]; // 更新全局菜单项间距变量
    MenuItemSpacingChanged = true;
}

void onchange_misc_menu_item_text_offset_index(int value, SelectFromListMenuItem* source) {
    MenuItemTextOffsetIndex = value;
    menuItemTextOffset = MISC_MENU_ITEM_TEXT_OFFSET_VALUES[value]; // 更新全局菜单项文本偏移量变量
    MenuItemTextOffsetChanged = true;
}

// 预览图设置相关函数实现
void onchange_misc_preview_position_threshold_index(int value, SelectFromListMenuItem* source) {
	PreviewPositionThresholdIndex = value;
	previewPositionThreshold = MISC_PREVIEW_POSITION_THRESHOLD_VALUES[value];
	PreviewPositionThresholdChanged = true;
}

void onchange_misc_preview_resolution_scale_index(int value, SelectFromListMenuItem* source) {
	PreviewResolutionScaleIndex = value;
	previewResolutionScale = MISC_PREVIEW_RESOLUTION_SCALE_VALUES[value];
	PreviewResolutionScaleChanged = true;
}

void onchange_misc_preview_spacing_index(int value, SelectFromListMenuItem* source) {
	PreviewSpacingIndex = value;
	previewSpacing = MISC_PREVIEW_SPACING_VALUES[value];
	PreviewSpacingChanged = true;
}

// 菜单项宽度和左侧偏移已合并到标题设置中，不再需要单独的onchange函数

void onchange_misc_menu_item_top_offset_index(int value, SelectFromListMenuItem* source) {
    MenuItemTopOffsetIndex = value;
    menuItemTopOffset = MISC_MENU_ITEM_TOP_OFFSET_VALUES[value]; // 更新全局菜单项顶部偏移量变量
    MenuItemTopOffsetChanged = true;
}

// 菜单布局设置菜单实现
int activeLineIndexMenuLayout = 0;

void reset_menu_layout_to_defaults() {
    // 使用默认索引常量，避免与用户选择索引0混淆
    MenuWidthIndex = MENU_WIDTH_DEFAULT_INDEX;
    menuWidth = MISC_MENU_WIDTH_VALUES[MenuWidthIndex];
    MenuWidthChanged = true;

    MenuHeightIndex = MENU_HEIGHT_DEFAULT_INDEX;
    menuHeight = MISC_MENU_HEIGHT_VALUES[MenuHeightIndex];
    MenuHeightChanged = true;

    MenuTopOffsetIndex = MENU_TOP_OFFSET_DEFAULT_INDEX;
    menuTopOffset = MISC_MENU_TOP_OFFSET_VALUES[MenuTopOffsetIndex];
    MenuTopOffsetChanged = true;

    MenuLeftOffsetIndex = MENU_LEFT_OFFSET_DEFAULT_INDEX;
    menuLeftOffset = MISC_MENU_LEFT_OFFSET_VALUES[MenuLeftOffsetIndex];
    MenuLeftOffsetChanged = true;

    MenuTextLeftOffsetIndex = MENU_TEXT_LEFT_OFFSET_DEFAULT_INDEX;
    menuTextLeftOffset = MISC_MENU_TEXT_LEFT_OFFSET_VALUES[MenuTextLeftOffsetIndex];
    MenuTextLeftOffsetChanged = true;

    MenuItemHeightIndex = MENU_ITEM_HEIGHT_DEFAULT_INDEX;
    menuItemHeight = MISC_MENU_ITEM_HEIGHT_VALUES[MenuItemHeightIndex];
    MenuItemHeightChanged = true;

    MenuItemSpacingIndex = MENU_ITEM_SPACING_DEFAULT_INDEX;
    menuItemSpacing = MISC_MENU_ITEM_SPACING_VALUES[MenuItemSpacingIndex];
    MenuItemSpacingChanged = true;

    MenuItemTextOffsetIndex = MENU_ITEM_TEXT_OFFSET_DEFAULT_INDEX;
    menuItemTextOffset = MISC_MENU_ITEM_TEXT_OFFSET_VALUES[MenuItemTextOffsetIndex];
    MenuItemTextOffsetChanged = true;

    MenuItemTopOffsetIndex = MENU_ITEM_TOP_OFFSET_DEFAULT_INDEX;
    menuItemTopOffset = MISC_MENU_ITEM_TOP_OFFSET_VALUES[MenuItemTopOffsetIndex];
    MenuItemTopOffsetChanged = true;

    // 重置预览图设置
    PreviewPositionThresholdIndex = PREVIEW_POSITION_THRESHOLD_DEFAULT_INDEX;
    previewPositionThreshold = MISC_PREVIEW_POSITION_THRESHOLD_VALUES[PreviewPositionThresholdIndex];
    PreviewPositionThresholdChanged = true;

    PreviewResolutionScaleIndex = PREVIEW_RESOLUTION_SCALE_DEFAULT_INDEX;
    previewResolutionScale = MISC_PREVIEW_RESOLUTION_SCALE_VALUES[PreviewResolutionScaleIndex];
    PreviewResolutionScaleChanged = true;

    PreviewSpacingIndex = PREVIEW_SPACING_DEFAULT_INDEX;
    previewSpacing = MISC_PREVIEW_SPACING_VALUES[PreviewSpacingIndex];
    PreviewSpacingChanged = true;
}

bool onconfirm_menu_layout_reset(MenuItem<int> choice) {
	if (choice.value == -0xA1B2C3) {// 检查特殊值-0xA1B2C3，避免与下拉菜单的索引冲突
		reset_menu_layout_to_defaults();
		set_status_text_centre_screen("菜单布局 ~g~已重置 ~s~为默认！"); // 屏幕中间提示，带闪烁
		set_status_text("菜单布局已重置为默认！");

		// 异步线程只设置刷新标志
		DWORD myThreadID;
		HANDLE myHandle = CreateThread(0, 0, [](LPVOID) -> DWORD {
			g_MenuLayoutNeedsRefresh = true; // 标志位交给主线程去刷新
			return 0;
			}, 0, 0, &myThreadID);
		CloseHandle(myHandle);

		return true; // 返回 true 退出当前菜单
	}
	return false;
}

void process_misc_menu_layout_settings_menu() {
    const std::string caption = "菜单布局设置";

    // 如果上次操作触发了重置，强制刷新当前菜单项显示的 value
    if (g_MenuLayoutNeedsRefresh) {
        g_MenuLayoutNeedsRefresh = false;
        // 直接继续构建菜单即可，下面使用的 Index 都是最新的 0
    }

    std::vector<MenuItem<int>*> menuItems;
    SelectFromListMenuItem *listItem;

    // 添加重置按钮
    MenuItem<int>* resetItem = new MenuItem<int>();
    resetItem->caption = "重置默认布局";
    resetItem->value = -0xA1B2C3; // 使用一个特殊值，避免与下拉菜单的索引冲突
    resetItem->isLeaf = true;
    menuItems.push_back(resetItem);

    // 添加菜单宽度设置
    listItem = new SelectFromListMenuItem(MISC_MENU_WIDTH_CAPTIONS, onchange_misc_menu_width_index);
    listItem->wrap = false;
    listItem->caption = "菜单 宽度";
    listItem->value = MenuWidthIndex;
    menuItems.push_back(listItem);

    // 添加菜单左侧偏移量设置
    listItem = new SelectFromListMenuItem(MISC_MENU_LEFT_OFFSET_CAPTIONS, onchange_misc_menu_left_offset_index);
    listItem->wrap = false;
    listItem->caption = "菜单 左右位置";
    listItem->value = MenuLeftOffsetIndex;
    menuItems.push_back(listItem);

    // 添加菜单项顶部偏移量设置
    listItem = new SelectFromListMenuItem(MISC_MENU_ITEM_TOP_OFFSET_CAPTIONS, onchange_misc_menu_item_top_offset_index);
    listItem->wrap = false;
    listItem->caption = "菜单项 上下位置";
    listItem->value = MenuItemTopOffsetIndex;
    menuItems.push_back(listItem);

    // 添加菜单顶部偏移量设置
    listItem = new SelectFromListMenuItem(MISC_MENU_TOP_OFFSET_CAPTIONS, onchange_misc_menu_top_offset_index);
    listItem->wrap = false;
    listItem->caption = "标题栏 上下位置";
    listItem->value = MenuTopOffsetIndex;
    menuItems.push_back(listItem);

    // 添加菜单高度设置
    listItem = new SelectFromListMenuItem(MISC_MENU_HEIGHT_CAPTIONS, onchange_misc_menu_height_index);
    listItem->wrap = false;
    listItem->caption = "标题栏 厚度";
    listItem->value = MenuHeightIndex;
    menuItems.push_back(listItem);

    // 添加菜单项高度设置
    listItem = new SelectFromListMenuItem(MISC_MENU_ITEM_HEIGHT_CAPTIONS, onchange_misc_menu_item_height_index);
    listItem->wrap = false;
    listItem->caption = "菜单项 厚度";
    listItem->value = MenuItemHeightIndex;
    menuItems.push_back(listItem);

    // 添加菜单文本左侧偏移量设置
    listItem = new SelectFromListMenuItem(MISC_MENU_TEXT_LEFT_OFFSET_CAPTIONS, onchange_misc_menu_text_left_offset_index);
    listItem->wrap = false;
    listItem->caption = "标题文本 左右位置";
    listItem->value = MenuTextLeftOffsetIndex;
    menuItems.push_back(listItem);

    // 添加菜单项文本偏移量设置
    listItem = new SelectFromListMenuItem(MISC_MENU_ITEM_TEXT_OFFSET_CAPTIONS, onchange_misc_menu_item_text_offset_index);
    listItem->wrap = false;
    listItem->caption = "菜单文本 左右位置";
    listItem->value = MenuItemTextOffsetIndex;
    menuItems.push_back(listItem);

    // 添加菜单项间距设置
    listItem = new SelectFromListMenuItem(MISC_MENU_ITEM_SPACING_CAPTIONS, onchange_misc_menu_item_spacing_index);
    listItem->wrap = false;
    listItem->caption = "菜单项 间距";
    listItem->value = MenuItemSpacingIndex;
    menuItems.push_back(listItem);

    draw_generic_menu<int>(menuItems, &activeLineIndexMenuLayout, caption, onconfirm_menu_layout_reset, NULL, NULL);
}
