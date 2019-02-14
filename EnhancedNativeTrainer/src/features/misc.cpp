/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "misc.h"
#include "hotkeys.h"
#include "world.h"
#include <Psapi.h>
#include "..\ui_support\menu_functions.h"

//==================
// MISC MENU
//==================

int activeLineIndexTrainerConfig = 0;
int activeLineIndexPhoneBill = 0;
int activeLineIndexDefMenuTab = 0;
int activeLineIndexBillSettings = 0;
int activeLineIndexPhoneOnBike = 0;
int activeLineIndexAirbrake = 0;
int activeLineHotkeyConfig = 0;

// airbrake mode variables
bool mouse_view_control = false;
bool help_showing = true;
bool frozen_time = false;

// Phone Bill variables
bool featureBlockInputInMenu = false;
bool featurePhoneBillEnabled = false;
bool featureGamePause = false;
bool featureZeroBalance = false;
bool featurePhone3DOnBike = false;
bool featureNoPhoneOnHUD = false;
int secs_passed, secs_curr = -1;
float temp_seconds, bill_seconds = 0;
float bill_to_pay, mins = -1;
//
// Dynamic Health Bar variables
bool featureDynamicHealthBar = false;
//bool featureDynamicHealthBarUpdated = false;
bool been_damaged = false;
float curr_damaged_health, curr_damaged_armor = -1;
int healthbar_secs_passed, healthbar_secs_curr, healthbar_seconds = -1;
float health_bar_x = 0.015;
float health_bar_y = 0.966;
//
// Show FPS
static int frames = 0;
static double FPStime, FPStime_passed, FPStime_curr, starttime = 0;
int fps = 0; 
char fps_to_show_char_modifiable[15];
//
// Use Phone While On Bike
Object temp_obj = -1;
char* anim_dict = "anim@cellphone@in_car@ps";
char* animation_of_d = "cellphone_text_read_base";
bool accel = false;
//
//int Comp_secs_passed, Comp_secs_curr, Comp_seconds = -1;

bool featurePlayerRadio = false;
bool featurePlayerRadioUpdated = false;
bool featureRadioFreeze = false, featureRadioFreezeUpdated = false;
bool featureRadioAlwaysOff = false;
bool featureRadioAlwaysOffUpdated = false;
bool featureBoostRadio = true;
//bool featureBoostRadioUpdated = true;
bool featureWantedMusic = false;
//bool featureWantedMusicUpdated = false;
bool featureFlyingMusic = false;
//bool featureFlyingMusicUpdated = false;
bool featurePoliceScanner = false;
//bool featurePoliceScannerUpdated = false;
bool featureNoScubaSound = false;
//bool featureNoScubaSoundUpdated = false;
bool featureNoComleteMessage = false;
//bool featureNoComleteMessageUpdated = false;
bool featurePoliceRadio = false;
//bool featurePoliceRadioUpdated = false;
bool police_radio_check = false;

bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;
bool featureMiscHideHudUpdated = false;
bool featurePhoneShowHud = false;
bool featureMiscHideENTHud = false;
//bool featurePhoneShowHudUpdated = false;
bool featureInVehicleNoHud = false;
//bool featureInVehicleNoHudUpdated = false;
bool phone_toggle = false;
bool phone_toggle_vehicle = false;
bool phone_toggle_defaultphone = false;

bool despawnPointerDisabledMessage = true;
bool featureFirstPersonDeathCamera = false;
bool featureFirstPersonStuntJumpCamera = false;
bool featureNoStuntJumps = false;
bool featureHidePlayerInfo = false;

bool featureShowFPS = false;
//bool featureShowFPSUpdated = false;

bool featureShowVehiclePreviews = true;
bool featureControllerIgnoreInTrainer = false;

bool featureMiscJellmanScenery = false;

bool featureResetPlayerModelOnDeath = true;

bool featureEnableMissingRadioStation = false;

bool slipperywhenwet = false;

const int TRAINERCONFIG_HOTKEY_MENU = 99;
int radioStationIndex = -1;

ScriptTable* scriptTable;
GlobalTable globalTable;
ScriptHeader* shopController;
HINSTANCE _hinstDLL;
bool enabledDespawnPointer = false;

Camera StuntCam = NULL;

// First Person Cutscene Camera Variables
Cam CutCam = NULL;
Object xaxis, zaxis = -1;
//

// Main characters
const Hash PLAYER_ZERO = 0xD7114C9;
const Hash PLAYER_ONE = 0x9B22DBAF;
const Hash PLAYER_TWO = 0x9B810FA2;
// Main characters cash
const Hash SP0_TOTAL_CASH = 0x324C31D;
const Hash SP1_TOTAL_CASH = 0x44BD6982;
const Hash SP2_TOTAL_CASH = 0x8D75047D;

// Phone Bill Amount
const std::vector<std::string> MISC_PHONE_BILL_CAPTIONS{ "10$", "50$", "100$", "500$", "1000$", "5000$", "10000$", "50000$", "100000$", "500000$", "1000000$" };
const float MISC_PHONE_BILL_VALUES[] = { 10.0, 50.0, 100.0, 500.0, 1000.0, 5000.0, 10000.0, 50000.0, 100000.0, 500000.0, 1000000.0 };
//const std::vector<float> MISC_PHONE_BILL_VALUES{ 10.0, 50.0, 100.0, 500.0, 1000.0, 5000.0, 10000.0, 50000.0, 100000.0, 500000.0, 1000000.0 };
int PhoneBillIndex = 2;
bool PhoneBillChanged = true;

// Phone Bill Free Seconds
const std::vector<std::string> MISC_PHONE_FREESECONDS_CAPTIONS{ "0", "3", "5", "10", "15" };
const int MISC_PHONE_FREESECONDS_VALUES[] = { 0, 3, 5, 10, 15 };
//const std::vector<int> MISC_PHONE_FREESECONDS_VALUES{ 0, 3, 5, 10, 15 };
int PhoneFreeSecondsIndex = 0;
bool PhoneFreeSecondsChanged = true;
int PhoneBikeAnimationIndex = 0;
bool PhoneBikeAnimationChanged = true;

// Default Menu Tab
const std::vector<std::string> MISC_DEF_MENUTAB_CAPTIONS{ "OFF", "Map", "Brief", "Friends", "Gallery", "Game", "Settings", "Stats", "Store", "Online" };
const int MISC_DEF_MANUTAB_VALUES[] = { -2, -1, 1, 2, 3, 5, 6, 10, 18, 42 };
//const std::vector<int> MISC_DEF_MANUTAB_VALUES{ -2, -1, 1, 2, 3, 5, 6, 10, 18, 42 };
int DefMenuTabIndex = 0;
bool DefMenuTabChanged = true;

// Default Phone
const std::vector<std::string> MISC_PHONE_DEFAULT_CAPTIONS{ "OFF", "Michael's", "Trevor's", "Franklin's", "Military", "Prologue" };
const int MISC_PHONE_DEFAULT_VALUES[] = { -1, 0, 1, 2, 3, 4 };
//const std::vector<int> MISC_PHONE_DEFAULT_VALUES{ -1, 0, 1, 2, 3, 4 };
int PhoneDefaultIndex = 0;
bool PhoneDefaultChanged = true;

void onchange_hotkey_function(int value, SelectFromListMenuItem* source){
	change_hotkey_function(source->extras.at(0), value);
}

bool process_misc_hotkey_menu(){
	std::vector<MenuItem<int>*> menuItems;

	for(int i = 1; i < 10; i++){
		std::ostringstream itemCaption;
		std::vector<std::string> captions;
		void(*callback)(int, SelectFromListMenuItem*);

		itemCaption << "Hotkey " << i;

		bool keyAssigned = get_config()->get_key_config()->is_hotkey_assigned(i);
		if(!keyAssigned){
			captions.push_back("Key Not Bound");
			callback = NULL;
		}
		else{
			for each (HOTKEY_DEF var in HOTKEY_AVAILABLE_FUNCS){
				captions.push_back(var.caption);
			}
			callback = onchange_hotkey_function;
		}

		SelectFromListMenuItem* item = new SelectFromListMenuItem(captions, callback);
		item->caption = itemCaption.str();
		item->wrap = keyAssigned;
		item->extras.push_back(i);
		item->value = get_hotkey_function_index(i);
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, &activeLineHotkeyConfig, "Hotkey Config", NULL, NULL, NULL);

	return false;
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
		set_status_text("Saved to INI file");
		write_text_to_log_file("INI config file written or updated");
	}
	else if(choice.value == ENTColor::colsVarsNum + 1){
		ENTColor::reset_colors();
		set_status_text("ENT menu colors reset");
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
	item->caption = "Save Menu Color Config";
	item->value = index++;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	item = new MenuItem<int>();
	item->caption = "Reset Menu Colors";
	item->value = index++;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	draw_generic_menu<int>(menuItems, nullptr, "Trainer Menu Colors", onconfirm_trainermenucolors_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_trainerconfig_menu(MenuItem<int> choice){
	if(choice.value == TRAINERCONFIG_HOTKEY_MENU){
		process_misc_hotkey_menu();
	}
	else if(choice.value == 63){
		process_misc_trainermenucolors_menu();
	}
	return false;
}

void process_misc_trainerconfig_menu(){
	std::string caption = "Trainer Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int>* stdItem = new MenuItem<int>();
	stdItem->caption = "Hotkey Setup";
	stdItem->value = TRAINERCONFIG_HOTKEY_MENU;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Lock Controls While In Menu";
	toggleItem->toggleValue = &featureBlockInputInMenu;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Turn Off All Controller Input In Trainer";
	toggleItem->toggleValue = &featureControllerIgnoreInTrainer;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Vehicle Previews";
	toggleItem->toggleValue = &featureShowVehiclePreviews;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Include Nkjellman's Extra Scenery";
	toggleItem->toggleValue = &featureMiscJellmanScenery;
	menuItems.push_back(toggleItem);

	stdItem = new MenuItem<int>();
	stdItem->caption = "Menu Colors";
	stdItem->value = 63;
	stdItem->isLeaf = false;
	menuItems.push_back(stdItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexTrainerConfig, caption, onconfirm_trainerconfig_menu, NULL, NULL);
}

bool onconfirm_misc_freezeradio_menu(MenuItem<int> choice){
	if(choice.value == -1){
		featureRadioFreeze = false;
		set_status_text("Radio is no longer frozen to a station");
	}
	else{
		featureRadioFreeze = true;
		set_status_text(std::string("Radio is frozen to station ") + std::string(AUDIO::GET_RADIO_STATION_NAME(choice.value)));
	}
	featureRadioFreezeUpdated = true;
	radioStationIndex = choice.value;

	return false;
}

void process_misc_freezeradio_menu(){
	std::vector<MenuItem<int> *> menuItems;
	int const stations = AUDIO::_MAX_RADIO_STATION_INDEX();

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "None";
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

	draw_generic_menu<int>(menuItems, nullptr, "Freeze Radio to Station", onconfirm_misc_freezeradio_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_airbrake_menu(MenuItem<int> choice) {

	if (choice.value == -1) {
		process_airbrake_menu();
	}
	return false;
}

void process_airbrake_global_menu() {
	std::string caption = "Airbrake Menu Options";

	std::vector<MenuItem<int>*> menuItems;
	//SelectFromListMenuItem *listItem;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "Enable Airbrake Mode [F6 to open/close]";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Mouse Mode";
	toggleItem->toggleValue = &mouse_view_control;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Help & Controls";
	toggleItem->toggleValue = &help_showing;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Frozen Time";
	toggleItem->toggleValue = &frozen_time;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexAirbrake, caption, onconfirm_airbrake_menu, NULL, NULL);
}

bool onconfirm_defmenutab_menu(MenuItem<int> choice) {

	return false;
}

void process_def_menutab_menu() {
	std::string caption = "Pause Menu Settings Options";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	
	listItem = new SelectFromListMenuItem(MISC_DEF_MENUTAB_CAPTIONS, onchange_misc_def_menutab_index);
	listItem->wrap = false;
	listItem->caption = "Default Pause Menu Tab";
	listItem->value = DefMenuTabIndex;
	menuItems.push_back(listItem);

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Pause Game When Menu Open";
	toggleItem->toggleValue = &featureGamePause;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Hide Player Info In Pause Menu";
	toggleItem->toggleValue = &featureHidePlayerInfo;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexDefMenuTab, caption, onconfirm_defmenutab_menu, NULL, NULL);
}

bool onconfirm_billsettings_menu(MenuItem<int> choice) {

	return false;
}

void process_billsettings_menu() {
	std::string caption = "Phone Bill Options";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable Phone Bill";
	toggleItem->toggleValue = &featurePhoneBillEnabled;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_BILL_CAPTIONS, onchange_misc_phone_bill_index);
	listItem->wrap = false;
	listItem->caption = "Amount Per Minute";
	listItem->value = PhoneBillIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_FREESECONDS_CAPTIONS, onchange_misc_phone_freeseconds_index);
	listItem->wrap = false;
	listItem->caption = "First Free Seconds";
	listItem->value = PhoneFreeSecondsIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Can't Use Phone If Zero Balance";
	toggleItem->toggleValue = &featureZeroBalance;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexBillSettings, caption, onconfirm_billsettings_menu, NULL, NULL);
}

bool onconfirm_phoneonbike_menu(MenuItem<int> choice) {

	return false;
}

void process_phoneonbike_menu() {
	std::string caption = "Use Phone While On Bike Options";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable";
	toggleItem->toggleValue = &featurePhone3DOnBike;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Phone On HUD While In First Person";
	toggleItem->toggleValue = &featureNoPhoneOnHUD;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_FREESECONDS_CAPTIONS, onchange_misc_phone_bike_index);
	listItem->wrap = false;
	listItem->caption = "Animation Type";
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
	std::string caption = "Phone Settings Options";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	MenuItem<int> *item;

	int i = 0;

	listItem = new SelectFromListMenuItem(MISC_PHONE_DEFAULT_CAPTIONS, onchange_misc_phone_default_index);
	listItem->wrap = false;
	listItem->caption = "Default Phone Model";
	listItem->value = PhoneDefaultIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "Phone Bill";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Use Phone While On Bike";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &activeLineIndexPhoneBill, caption, onconfirm_phonebill_menu, NULL, NULL);
}

int activeLineIndexMisc = 0;

bool onconfirm_misc_menu(MenuItem<int> choice){
	switch(activeLineIndexMisc){
		case 0:
			process_misc_trainerconfig_menu();
			break;
		case 2:
			// next radio track
			AUDIO::SKIP_RADIO_FORWARD();
			break;
		case 3:
			process_misc_freezeradio_menu();
			break;
		case 19:
			process_phone_bill_menu();
			break;
		case 24:
			process_def_menutab_menu();
			break;
		case 25:
			process_airbrake_global_menu();
			break;
		default:
			// switchable features
			break;
	}
	return false;
}

void process_misc_menu(){
	const int lineCount = 26;

	std::string caption = "Miscellaneous Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Trainer Options", NULL, NULL, false},
		{"Portable Radio", &featurePlayerRadio, &featurePlayerRadioUpdated, true},
		{"Next Radio Track", NULL, NULL, true},
		{"Freeze Radio To Station", nullptr, nullptr, false},
		{"Radio Always Off", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated, true},
		{"Boost Radio Volume", &featureBoostRadio, NULL, true}, // &featureBoostRadioUpdated
		{"Restore Missing Radio Station", &featureEnableMissingRadioStation, NULL, false },
		{"Radio In Police Vehicle", &featurePoliceRadio, NULL, true}, // &featurePoliceRadioUpdated
		{"No Wanted Music", &featureWantedMusic, NULL, true}, // &featureWantedMusicUpdated
		{"No Flight Music", &featureFlyingMusic, NULL, true}, // &featureFlyingMusicUpdated
		{"No Police Scanner", &featurePoliceScanner, NULL, true }, // &featurePoliceScannerUpdated
		{"No Scuba Breathing Sound", &featureNoScubaSound, NULL, true }, // &featureNoScubaSoundUpdated
		{"No 'Mission Passed' Message", &featureNoComleteMessage, NULL, true },
		{"Hide HUD", &featureMiscHideHud, &featureMiscHideHudUpdated},
		{"Hide HUD If Menu Open", &featureMiscHideENTHud},
		{"Show HUD If Phone In Hand Only", &featurePhoneShowHud, NULL }, // &featurePhoneShowHudUpdated
		{"Show HUD In Vehicle Only", &featureInVehicleNoHud, NULL }, // &featureInVehicleNoHudUpdated
		{"Dynamic Health Bar", &featureDynamicHealthBar }, // , &featureDynamicHealthBarUpdated
		{"Reset Player Model On Death", &featureResetPlayerModelOnDeath, nullptr, true},
		{"Phone Settings", NULL, NULL, false},
		{"First Person Death Camera", &featureFirstPersonDeathCamera, NULL },
		{"First Person Stunt Jump Camera", &featureFirstPersonStuntJumpCamera, NULL },
		{"No Stunt Jumps", &featureNoStuntJumps, NULL },
		{"FPS Counter", &featureShowFPS, NULL }, // &featureShowFPSUpdated
		{"Pause Menu Settings", NULL, NULL, false},
		{"Airbrake Menu", NULL, NULL, false},
	};
	
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexMisc, caption, onconfirm_misc_menu);
}

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source){
	PhoneBillIndex = value;
	PhoneBillChanged = true;
}

void onchange_misc_phone_default_index(int value, SelectFromListMenuItem* source) {
	PhoneDefaultIndex = value;
	PhoneDefaultChanged = true;
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

void reset_misc_globals(){
	featureMiscHideHud =
		featurePhoneShowHud = 
		featureMiscHideENTHud =
		featureInVehicleNoHud =
		featureDynamicHealthBar =
		featurePlayerRadio =
		featureMiscLockRadio =
		featureMiscJellmanScenery =
		featureRadioFreeze =
		featureWantedMusic = 
		featureFlyingMusic = 
		featurePoliceScanner = 
		featureNoScubaSound = 
		featureNoComleteMessage =
		featurePoliceRadio =
		featureEnableMissingRadioStation =
		featureRadioAlwaysOff = false;

	PhoneBillIndex = 2;
	PhoneDefaultIndex = 0;
	PhoneFreeSecondsIndex = 0;
	PhoneBikeAnimationIndex = 0;

	DefMenuTabIndex = 0;

	featureShowVehiclePreviews = true;
	featureControllerIgnoreInTrainer = false;
	featureBlockInputInMenu = false;
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

	featureRadioFreezeUpdated =
		featureRadioAlwaysOffUpdated =
		featureMiscHideHudUpdated =
		//featurePhoneShowHudUpdated =
		//featureInVehicleNoHudUpdated =
		//featureDynamicHealthBarUpdated =
		//featureWantedMusicUpdated =
		//featureFlyingMusicUpdated =
		//featurePoliceScannerUpdated =
		//featureNoScubaSoundUpdated = 
		//featureNoComleteMessageUpdated =
		featureBoostRadio = true;
		//featureBoostRadioUpdated = true;
		//featurePoliceRadioUpdated =
		//featurePlayerRadioUpdated =
		//featureShowFPSUpdated = true;

	ENTColor::reset_colors();
}

void update_misc_features(BOOL playerExists, Ped playerPed){
	if (featureRadioAlwaysOff || featurePlayerRadioUpdated){
		if (featureRadioAlwaysOff){
			if (featurePlayerRadio){
				featurePlayerRadio = false;
				featurePlayerRadioUpdated = true;
			}
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, !featureRadioAlwaysOff);
		}

		AUDIO::SET_USER_RADIO_CONTROL_ENABLED(!featureRadioAlwaysOff);
	}

	// Portable radio
	if (featurePlayerRadio || featurePlayerRadioUpdated) {
		if (featurePlayerRadio) {
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else {
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	// No Wanted Music
	//if (featureWantedMusic || featureWantedMusicUpdated) {
		if (featureWantedMusic) {
			AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", true);
		}
		else {
			AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", false);
		}
	//}

	// No Flying Music
	//if (featureFlyingMusic || featureFlyingMusicUpdated) {
		if (featureFlyingMusic) {
			AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", true);
		}
		else {
			AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", false);
		}
	//}

	// No Police Scanner
	//if (featurePoliceScanner || featurePoliceScannerUpdated) {
		if (featurePoliceScanner) {
			AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", true);
		}
		else {
			AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", false);
		}
	//}

	// No Scuba Breathing Sound
	//if (featureNoScubaSound || featureNoScubaSoundUpdated) {
		if (featureNoScubaSound) {
			AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", true);
		}
		else {
			AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", false);
		}
	//}

	// No 'Mission Passed' Message
	if (featureNoComleteMessage) {
		if (!SCRIPT::HAS_SCRIPT_LOADED("family3") && !SCRIPT::HAS_SCRIPT_LOADED("jewelry_heist") && !SCRIPT::HAS_SCRIPT_LOADED("family5") && !SCRIPT::HAS_SCRIPT_LOADED("wardrobe_sp") &&
			!SCRIPT::HAS_SCRIPT_LOADED("family6"))
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("mission_stat_watcher");
	}

	// Radio Boost
	//if (featureBoostRadio || featureBoostRadioUpdated) {
		if (featureBoostRadio) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				AUDIO::SET_VEHICLE_RADIO_LOUD(playerVeh, 1);
				AUDIO::RELEASE_AMBIENT_AUDIO_BANK();
			}
		}
	//}
	if (!featureBoostRadio) AUDIO::SET_VEHICLE_RADIO_LOUD(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);

	// Radio In Police Vehicles
	//if (featurePoliceRadio || featurePoliceRadioUpdated) {
		if (featurePoliceRadio) {
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
			Vector3 coords_radio = ENTITY::GET_ENTITY_COORDS(playerVeh, 1);
			Vector3 coords_radio_2 = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			if (PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed) && VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(playerVeh)) {
				police_radio_check = true;
				AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, true);
				AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
				AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
				AUDIO::SET_RADIO_AUTO_UNFREEZE(true);
				AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
			}
			
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) if (!PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed)) police_radio_check = false;

			if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(coords_radio.x, coords_radio.y, coords_radio.z, coords_radio_2.x, coords_radio_2.y, coords_radio_2.z, false) < 3 && 
				VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(playerVeh) && police_radio_check) {
				AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, true);
				AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
				AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
				AUDIO::SET_RADIO_AUTO_UNFREEZE(true);
				AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
			}
		}
	//}
	
	// Freeze radio to station
	if (featureRadioFreeze) {
		if (AUDIO::GET_PLAYER_RADIO_STATION_INDEX() != radioStationIndex) {
			AUDIO::SET_RADIO_TO_STATION_INDEX(radioStationIndex);
		}
	}
	else if (featureRadioFreezeUpdated) {
		// Leave it empty for now.
	}

	// hide hud
	if (featureMiscHideHud || (featureMiscHideENTHud && menu_showing == true)) {
		for (int i = 0; i < 21; i++) {
			//at least in theory...
			switch (i){
			case 5: //mp message
			case 10: //help text
			case 11: //floating help 1
			case 12: //floating help 2
			case 14: //reticle
			case 16: //radio wheel
			case 19: //weapon wheel
				continue;
			}
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(i);
		}

		UI::DISPLAY_RADAR(false);
		featureMiscHideHudUpdated = false;
	}
	else if ((featureMiscHideHudUpdated && !featureMiscHideENTHud) || (featureMiscHideENTHud && menu_showing == false)){
		UI::DISPLAY_RADAR(true);
		featureMiscHideHudUpdated = false;
	}
	
	// show hud if phone in hand
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
	else if (!featureMiscHideHud && !featureInVehicleNoHud && !featureMiscHideENTHud) {
		UI::DISPLAY_RADAR(true);
		phone_toggle = false;
	}
	
	// show hud in vehicle only
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
	else if (!featureMiscHideHud && !featurePhoneShowHud && !featureMiscHideENTHud) {
		UI::DISPLAY_RADAR(true);
		phone_toggle_vehicle = false;
	}
	
	// Default Phone
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
	
	// Use Phone While On Bike
	if (featurePhone3DOnBike) {
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

		if (PED::IS_PED_ON_ANY_BIKE(playerPed) && PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed)) {
			if (featureNoPhoneOnHUD && CAM::GET_FOLLOW_PED_CAM_VIEW_MODE() == 4) MOBILE::SET_MOBILE_PHONE_POSITION(100, 200, 300);
			
			Hash temp_Hash = -1;
			Vector3 temp_pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			
			if (!STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) {
				STREAMING::REQUEST_ANIM_DICT(anim_dict);
				while (!STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) WAIT(0);
				if (STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) {
					AI::TASK_PLAY_ANIM(playerPed, anim_dict, animation_of_d, 8.0, 0.0, -1, 9, 0, 0, 0, 0);
					if (PED::GET_PED_TYPE(playerPed) == 0 && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing"); // michael
					if (PED::GET_PED_TYPE(playerPed) == 1 && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_03"); // franklin
					if ((PED::GET_PED_TYPE(playerPed) == 2 || PED::GET_PED_TYPE(playerPed) == 3) && (MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == -1 || MISC_PHONE_DEFAULT_VALUES[PhoneDefaultIndex] == 3)) 
						temp_Hash = GAMEPLAY::GET_HASH_KEY("prop_phone_ing_02"); // trevor
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
			}

			if (CONTROLS::IS_CONTROL_RELEASED(2, 71) && accel == true) {
				AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
				OBJECT::DELETE_OBJECT(&temp_obj);
				STREAMING::REMOVE_ANIM_DICT(anim_dict);
				CONTROLS::DISABLE_CONTROL_ACTION(0, 71, 1);
				accel = false;
			}
			if (STREAMING::HAS_ANIM_DICT_LOADED(anim_dict) && !ENTITY::DOES_ENTITY_EXIST(temp_obj)) {
				AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
				OBJECT::DELETE_OBJECT(&temp_obj);
				STREAMING::REMOVE_ANIM_DICT(anim_dict);
			}
		}
		
		if (CONTROLS::IS_CONTROL_PRESSED(2, 75) || CONTROLS::IS_CONTROL_PRESSED(2, 72) || CONTROLS::IS_CONTROL_PRESSED(2, 63) || CONTROLS::IS_CONTROL_PRESSED(2, 64) || 
			(CONTROLS::IS_CONTROL_PRESSED(2, 71) && veh_s.x == 0 && veh_s.y == 0)) {
			AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
			OBJECT::DELETE_OBJECT(&temp_obj);
			STREAMING::REMOVE_ANIM_DICT(anim_dict);
			accel = true;
		}

		if ((PED::IS_PED_ON_ANY_BIKE(playerPed) && !PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && STREAMING::HAS_ANIM_DICT_LOADED(anim_dict)) ||
			(!PED::IS_PED_ON_ANY_BIKE(playerPed) && STREAMING::HAS_ANIM_DICT_LOADED(anim_dict))) {
			AI::STOP_ANIM_TASK(playerPed, anim_dict, animation_of_d, 1.0);
			OBJECT::DELETE_OBJECT(&temp_obj);
			STREAMING::REMOVE_ANIM_DICT(anim_dict);
			accel = false;
		} 
	}

	// DYNAMIC HEALTH BAR
	if (featureDynamicHealthBar && !CUTSCENE::IS_CUTSCENE_PLAYING()) {
		if (!featureMiscHideHud && !featurePhoneShowHud && !featureInVehicleNoHud && !featureMiscHideENTHud) UI::DISPLAY_RADAR(false); // There is no need to hide HUD if it's already hidden
		
		auto addr = getScriptHandleBaseAddress(playerPed);
		float health = (*(float *)(addr + 0x280)) - 100;
		float playerArmour = PED::GET_PED_ARMOUR(playerPed);

		if (!ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(playerPed)) {
			curr_damaged_health = (*(float *)(addr + 0x280)) - 100;
			curr_damaged_armor = PED::GET_PED_ARMOUR(playerPed);
		}
		
		if (curr_damaged_health != health || curr_damaged_armor != playerArmour) {
			healthbar_seconds = -1;
			been_damaged = true;
			curr_damaged_health = health;
			curr_damaged_armor = playerArmour;
		}
		
		if (been_damaged == true) {
			healthbar_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - healthbar_secs_curr) != 0) {
				healthbar_seconds = healthbar_seconds + 1;
				healthbar_secs_curr = healthbar_secs_passed;
			}

			if (healthbar_seconds == 15) {
				been_damaged = false;
				healthbar_seconds = -1;
			}

			// Health
			if (health < 20) {
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 220, 20, 20, 55);
				if ((health_bar_x + (health / 1399)) > 0.015) GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + (health / 2799), health_bar_y + 0.01, (health / 1399), 0.009, 220, 20, 20, 255);
			}
			else {
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 41, 56, 40, 75);
				if ((health / 1399) < 0.070) GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + (health / 2799), health_bar_y + 0.01, (health / 1399), 0.009, 78, 150, 77, 255);
				else GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 78, 150, 77, 255);
			}
			// Armor
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0880, health_bar_y + 0.01, 0.036, 0.017, 38, 85, 87, 110);
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 39, 55, 56, 90);
			if ((playerArmour / 2935) < 0.035) GRAPHICS::DRAW_RECT(health_bar_x + 0.0715 + (playerArmour / 5871), health_bar_y + 0.01, (playerArmour / 2935), 0.009, 62, 129, 164, 255);
			else GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 62, 129, 164, 255);
		}
	}
	
	// Default Menu Tab
	if (MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex] > -2) {
		int GetHash = GAMEPLAY::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE");
		if (CONTROLS::IS_CONTROL_PRESSED(2, 199) || CONTROLS::IS_CONTROL_PRESSED(2, 200)) {
			UI::ACTIVATE_FRONTEND_MENU(GetHash, featureGamePause, MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex]);
			WAIT(100);
		} 
	} 
	
	// Phone Bill
	if (featurePhoneBillEnabled) {
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
		
		if (featureZeroBalance) {
			int outValue_your_phone_bill = -1;
			int statHash_all_your_money = -1;
			
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO) {
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP0_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE) {
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP1_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO) {
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP2_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
		}
	}
	
	// First Person Stunt Jump Camera
	if (featureFirstPersonStuntJumpCamera) {
		Ped stunt_player = PLAYER::PLAYER_PED_ID();

		if (GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS() && !CAM::DOES_CAM_EXIST(StuntCam)) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(stunt_player, true);
			Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(PED::GET_VEHICLE_PED_IS_USING(stunt_player), 2);

			StuntCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
			
			if (!PED::IS_PED_ON_ANY_BIKE(stunt_player)) {
				CAM::ATTACH_CAM_TO_PED_BONE(StuntCam, stunt_player, 31086, 0, -0.15, 0.05, 1);
				CAM::POINT_CAM_AT_PED_BONE(StuntCam, stunt_player, 31086, 0, 0.0, 0.05, 1);
			}
			if (PED::IS_PED_ON_ANY_BIKE(stunt_player)) {
				CAM::ATTACH_CAM_TO_PED_BONE(StuntCam, stunt_player, 31086, 0, -0.15, -0.10, 1);
				CAM::POINT_CAM_AT_PED_BONE(StuntCam, stunt_player, 31086, 0, 0.0, -0.10, 1);
			}
			CAM::SET_CAM_ROT(StuntCam, curRotation.x, curRotation.y, curRotation.z, 2);

			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(StuntCam, 1.0);
			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(StuntCam, 1.0);
			CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(StuntCam, 1.0);

			CAM::RENDER_SCRIPT_CAMS(true, false, 1, true, true);
			CAM::SET_CAM_ACTIVE(StuntCam, true);
			CAM::SET_CAM_NEAR_CLIP(StuntCam, .329);
		}

		if (!GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS() && CAM::DOES_CAM_EXIST(StuntCam)) {
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), 1, 1);
			CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
			CAM::DETACH_CAM(StuntCam);
			CAM::SET_CAM_ACTIVE(StuntCam, false);
			CAM::DESTROY_CAM(StuntCam, true);
			//StuntCam = NULL;
		}
	}
	
	// First Person Cutscene Camera 
	/*if (CUTSCENE::IS_CUTSCENE_PLAYING() && ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) {
		Vector3 Pedrotation = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2);
		if (!CAM::DOES_CAM_EXIST(CutCam)) {
			int PlayerIndex = PED::GET_PED_BONE_INDEX(PLAYER::PLAYER_PED_ID(), 8433);
			int PedHash = GAMEPLAY::GET_HASH_KEY("bot_01b_bit_03");
			Vector3 Ped1Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 1.0f, 0.0f);
			Vector3 Ped2Coords = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 2.0f, 0.0f);
			float PedlookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

			xaxis = OBJECT::CREATE_OBJECT(PedHash, Ped1Coords.x, Ped1Coords.y, Ped1Coords.z, 1, true, 1);
			zaxis = OBJECT::CREATE_OBJECT(PedHash, Ped2Coords.x, Ped2Coords.y, Ped2Coords.z, 1, true, 1);
			ENTITY::SET_ENTITY_VISIBLE(xaxis, false);
			ENTITY::SET_ENTITY_VISIBLE(zaxis, false);
			ENTITY::SET_ENTITY_COLLISION(xaxis, false, true);
			ENTITY::SET_ENTITY_COLLISION(zaxis, false, true);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(xaxis, PLAYER::PLAYER_PED_ID(), PlayerIndex, 0.0f, 0.0f, -0.1f, 105.0f, 0.0f, 0.0f, false, false, false, true, 0, true);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(zaxis, PLAYER::PLAYER_PED_ID(), PlayerIndex, 0.0f, 0.08f, -0.1f, 50.0f, 0.0f, 0.0f, false, false, false, true, 0, true);

			Vector3 coordsPed = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			CutCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", coordsPed.x, coordsPed.y, coordsPed.z, Pedrotation.x, Pedrotation.y, Pedrotation.z, 50.0, true, 2);
			CAM::ATTACH_CAM_TO_ENTITY(CutCam, zaxis, 0, 0, 0, true);
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
	else {
		OBJECT::DELETE_OBJECT(&xaxis);
		OBJECT::DELETE_OBJECT(&zaxis);
		if (CAM::DOES_CAM_EXIST(CutCam)) {
			CAM::RENDER_SCRIPT_CAMS(false, false, 1, false, false);
			CAM::DESTROY_CAM(CutCam, true);
		}
	}*/

	// No Stunt Jumps
	if (featureNoStuntJumps && GAMEPLAY::IS_STUNT_JUMP_IN_PROGRESS()) GAMEPLAY::CANCEL_STUNT_JUMP();

	// FPS Counter
	if (featureShowFPS)	{
		FPStime_passed = clock() / CLOCKS_PER_SEC;
		if (((clock() / CLOCKS_PER_SEC) - FPStime_curr) != 0) {
			FPStime = FPStime + 1;
			FPStime_curr = FPStime_passed;
		}

		frames++;
		
		if (FPStime - starttime > 0.25 && frames > 10) {
			fps = (double)frames / (FPStime - starttime);
			if ((FPStime - starttime) >= 0) starttime = FPStime;
			frames = 0;
		}
			
		sprintf(fps_to_show_char_modifiable, "%d", fps);
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(fps_to_show_char_modifiable);
		UI::_DRAW_TEXT(0.003, 0.135);
		GRAPHICS::DRAW_RECT(0.0, 0.15, 0.05, 0.03, 10, 10, 10, 100);
	}
	
	// Hide player info in pause menu
	if (featureHidePlayerInfo) UI::_SET_DIRECTOR_MODE(true);
	else UI::_SET_DIRECTOR_MODE(false);

	
	//Enable's 1.44's new radio station. Credit goes to Sjaak for finding this!
	if (featureEnableMissingRadioStation)
	{
		int version = getGameVersion();
		if (version > 41)
		{
			UNK3::_0x477D9DB48F889591("RADIO_22_DLC_BATTLE_MIX1_RADIO", 0);
		}
		else 
		{
			set_status_text("Game version outdated. This requires 1.44 onwards to function!");
			featureEnableMissingRadioStation = false;
		}
	}
}

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated });
	results->push_back(FeatureEnabledLocalDefinition{"featureRadioFreeze", &featureRadioFreeze, &featureRadioFreezeUpdated });
	results->push_back(FeatureEnabledLocalDefinition{"featureRadioAlwaysOff", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated });
	results->push_back(FeatureEnabledLocalDefinition{"featureBoostRadio", &featureBoostRadio }); // , &featureBoostRadioUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedMUsic", &featureWantedMusic}); // , &featureWantedMusicUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featureFlyingMusic", &featureFlyingMusic}); // , &featureFlyingMusicUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceScanner", &featurePoliceScanner}); // , &featurePoliceScannerUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featureNoScubaSound", &featureNoScubaSound}); // , &featureNoScubaSoundUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featureNoComleteMessage", &featureNoComleteMessage}); // , &featureNoComleteMessageUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceRadio", &featurePoliceRadio}); // , &featurePoliceRadioUpdated
	
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscLockRadio", &featureMiscLockRadio});
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscHideHud", &featureMiscHideHud, &featureMiscHideHudUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneShowHud", &featurePhoneShowHud}); // , &featurePhoneShowHudUpdated
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscHideENTHud", &featureMiscHideENTHud});
	results->push_back(FeatureEnabledLocalDefinition{"featureInVehicleNoHud", &featureInVehicleNoHud}); // , &featureInVehicleNoHudUpdated

	results->push_back(FeatureEnabledLocalDefinition{"featureDynamicHealthBar", &featureDynamicHealthBar}); // , &featureDynamicHealthBarUpdated
		
	results->push_back(FeatureEnabledLocalDefinition{"featureControllerIgnoreInTrainer", &featureControllerIgnoreInTrainer});
	results->push_back(FeatureEnabledLocalDefinition{"featureBlockInputInMenu", &featureBlockInputInMenu});
	results->push_back(FeatureEnabledLocalDefinition{"mouse_view_control", &mouse_view_control});
	results->push_back(FeatureEnabledLocalDefinition{"help_showing", &help_showing});
	results->push_back(FeatureEnabledLocalDefinition{"frozen_time", &frozen_time});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneBillEnabled", &featurePhoneBillEnabled});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoGamePause", &featureGamePause});
	results->push_back(FeatureEnabledLocalDefinition{"featureZeroBalance", &featureZeroBalance});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhone3DOnBike", &featurePhone3DOnBike});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoPhoneOnHUD", &featureNoPhoneOnHUD});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowVehiclePreviews", &featureShowVehiclePreviews});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowFPS", &featureShowFPS});
	results->push_back(FeatureEnabledLocalDefinition{"featureHiddenRadioStation", &featureEnableMissingRadioStation});

	results->push_back(FeatureEnabledLocalDefinition{"featureFirstPersonDeathCamera", &featureFirstPersonDeathCamera});
	results->push_back(FeatureEnabledLocalDefinition{"featureFirstPersonStuntJumpCamera", &featureFirstPersonStuntJumpCamera});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoStuntJumps", &featureNoStuntJumps});
	results->push_back(FeatureEnabledLocalDefinition{"featureHidePlayerInfo", &featureHidePlayerInfo});
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscJellmanScenery", &featureMiscJellmanScenery});

	results->push_back(FeatureEnabledLocalDefinition{"featureResetPlayerModelOnDeath", &featureResetPlayerModelOnDeath});
}

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"radioStationIndex", std::to_string(radioStationIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneBillIndex", std::to_string(PhoneBillIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneDefaultIndex", std::to_string(PhoneDefaultIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneFreeSecondsIndex", std::to_string(PhoneFreeSecondsIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneBikeAnimationIndex", std::to_string(PhoneBikeAnimationIndex)});
	results->push_back(StringPairSettingDBRow{"DefMenuTabIndex", std::to_string(DefMenuTabIndex)});
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
		else if (setting.name.compare("PhoneFreeSecondsIndex") == 0){
			PhoneFreeSecondsIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PhoneBikeAnimationIndex") == 0) {
			PhoneBikeAnimationIndex = stoi(setting.value);
		}
		else if (setting.name.compare("DefMenuTabIndex") == 0) {
			DefMenuTabIndex = stoi(setting.value);
		}
	}
}

bool is_player_reset_on_death(){
	Hash dmHash = GAMEPLAY::GET_HASH_KEY("director_mode");
	if(!featureResetPlayerModelOnDeath || SCRIPT::_GET_NUM_OF_INSTANCES_OF_SCRIPT_WITH_NAME_HASH(dmHash) > 0){
		return false;
	}

	return true;
}

bool is_vehicle_preview_enabled(){
	return featureShowVehiclePreviews;
}

bool is_input_blocked_in_menu(){
	return featureBlockInputInMenu;
}

bool is_controller_ignored_in_trainer(){
	return featureControllerIgnoreInTrainer;
}

bool is_hud_hidden(){
	return featureMiscHideHud;
}

void set_hud_hidden(bool hidden){
	featureMiscHideHud = hidden;
	featureMiscHideHudUpdated = true;
}

void set_hud_shown(bool hidden){
	featurePhoneShowHud = hidden;
	//featurePhoneShowHudUpdated = true;
}

bool is_jellman_scenery_enabled(){
	return featureMiscJellmanScenery;
}
