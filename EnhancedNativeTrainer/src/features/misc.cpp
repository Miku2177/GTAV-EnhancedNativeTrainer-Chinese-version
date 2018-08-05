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
#include <Psapi.h>

//==================
// MISC MENU
//==================

int activeLineIndexTrainerConfig = 0;
int activeLineIndexPhoneBill = 0;
int activeLineHotkeyConfig = 0;

// Phone Bill variables
bool featureBlockInputInMenu = false;
bool featurePhoneBillEnabled = false;
bool featureZeroBalance = false;
int secs_passed, secs_curr = -1;
float temp_seconds, bill_seconds = 0;
float bill_to_pay, mins = -1;
//
// Dynamic Health Bar variables
bool featureDynamicHealthBar = false;
bool featureDynamicHealthBarUpdated = false;
bool been_damaged = false;
float curr_damaged_health, curr_damaged_armor = -1;
int healthbar_secs_passed, healthbar_secs_curr, healthbar_seconds = -1;
float health_bar_x = 0.015;
float health_bar_y = 0.966;
//
// Show FPS
int fpsgrab, fps, FrameCountPre = 0;
bool featureFPS = false;
//
bool featurePlayerRadio = false;
bool featurePlayerRadioUpdated = false;
bool featureRadioFreeze = false, featureRadioFreezeUpdated = false;
bool featureRadioAlwaysOff = false;
bool featureRadioAlwaysOffUpdated = false;
bool featureBoostRadio = true;
bool featureBoostRadioUpdated = true;
bool featureWantedMusic = false;
bool featureWantedMusicUpdated = false;
bool featureFlyingMusic = false;
bool featureFlyingMusicUpdated = false;
bool featurePoliceScanner = false;
bool featurePoliceScannerUpdated = false;
bool featurePoliceRadio = false;
bool featurePoliceRadioUpdated = false;
bool police_radio_check = false;

bool featureMiscLockRadio = false;
bool featureMiscHideHud = false;
bool featureMiscHideHudUpdated = false;
bool featurePhoneShowHud = false;
bool featurePhoneShowHudUpdated = false;
bool featureInVehicleNoHud = false;
bool featureInVehicleNoHudUpdated = false;
bool phone_toggle = false;
bool phone_toggle_vehicle = false;

bool featureFindDespawnPointer = false;
bool featureFindDespawnPointerUpdated = false;
bool despawnPointerDisabledMessage = true;

bool featureShowFPS = false;
bool featureShowFPSUpdated = false;

bool featureShowVehiclePreviews = true;
bool featureControllerIgnoreInTrainer = false;

bool featureMiscJellmanScenery = false;

bool featureResetPlayerModelOnDeath = true;

const int TRAINERCONFIG_HOTKEY_MENU = 99;
int radioStationIndex = -1;

ScriptTable* scriptTable;
GlobalTable globalTable;
ScriptHeader* shopController;
HINSTANCE _hinstDLL;
bool enabledDespawnPointer = false;

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
const std::vector<float> MISC_PHONE_BILL_VALUES{ 10.0, 50.0, 100.0, 500.0, 1000.0, 5000.0, 10000.0, 50000.0, 100000.0, 500000.0, 1000000.0 };
int PhoneBillIndex = 2;
bool PhoneBillChanged = true;

// Phone Bill Free Seconds
const std::vector<std::string> MISC_PHONE_FREESECONDS_CAPTIONS{ "0", "3", "5", "10", "15" };
const std::vector<int> MISC_PHONE_FREESECONDS_VALUES{ 0, 3, 5, 10, 15 };
int PhoneFreeSecondsIndex = 0;
bool PhoneFreeSecondsChanged = true;

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

bool onconfirm_phonebill_menu(MenuItem<int> choice){
	
	return false;
}

void process_phone_bill_menu(){
	std::string caption = "Phone Bill Options";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable";
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
		case 15:
			process_phone_bill_menu();
			break;
		default:
			// switchable features
			break;
	}
	return false;
}

void process_misc_menu(){
	const int lineCount = 17;

	std::string caption = "Miscellaneous Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Trainer Options", NULL, NULL, false},
		{"Portable Radio", &featurePlayerRadio, &featurePlayerRadioUpdated, true},
		{"Next Radio Track", NULL, NULL, true},
		{"Freeze Radio To Station", nullptr, nullptr, false},
		{"Radio Always Off", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated, true},
		{"Boost Radio Volume", &featureBoostRadio, &featureBoostRadioUpdated, true},
		{"Radio In Police Vehicle", &featurePoliceRadio, &featurePoliceRadioUpdated, true},
		{"No Wanted Music", &featureWantedMusic, &featureWantedMusicUpdated, true},
		{"No Flight Music", &featureFlyingMusic, &featureFlyingMusicUpdated, true},
		{"No Police Scanner", &featurePoliceScanner, &featurePoliceScannerUpdated, true },
		{"Hide HUD", &featureMiscHideHud, &featureMiscHideHudUpdated},
		{"Show HUD If Phone In Hand", &featurePhoneShowHud, &featurePhoneShowHudUpdated},
		{"Show HUD In Vehicle", &featureInVehicleNoHud, &featureInVehicleNoHudUpdated },
		{"Dynamic Health Bar", &featureDynamicHealthBar, &featureDynamicHealthBarUpdated },
		{"Reset Player Model On Death", &featureResetPlayerModelOnDeath, nullptr, true},
		{"Phone Bill", NULL, NULL, false},
		{"Auto-Find Vehicle Despawn Pointer", &featureFindDespawnPointer, &featureFindDespawnPointerUpdated, true },
		//{"Show FPS", &featureShowFPS, &featureShowFPSUpdated },
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexMisc, caption, onconfirm_misc_menu);
}

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source){
	PhoneBillIndex = value;
	PhoneBillChanged = true;
}

void onchange_misc_phone_freeseconds_index(int value, SelectFromListMenuItem* source){
	PhoneFreeSecondsIndex = value;
	PhoneFreeSecondsChanged = true;
}

void reset_misc_globals(){
	featureMiscHideHud =
		featurePhoneShowHud = 
		featureInVehicleNoHud =
		featureDynamicHealthBar =
		featurePlayerRadio =
		featureMiscLockRadio =
		featureMiscJellmanScenery =
		featureRadioFreeze =
		featureWantedMusic = 
		featureFlyingMusic = 
		featurePoliceScanner = 
		featurePoliceRadio =
		featureRadioAlwaysOff = false;

	PhoneBillIndex = 2;
	PhoneFreeSecondsIndex = 0;

	featureShowVehiclePreviews = true;
	featureControllerIgnoreInTrainer = false;
	featureBlockInputInMenu = false;
	featurePhoneBillEnabled = false;
	featureZeroBalance = false;
	featureFindDespawnPointer = false;
	featureShowFPS = false;

	featureRadioFreezeUpdated =
		featureRadioAlwaysOffUpdated =
		featureMiscHideHudUpdated =
		featurePhoneShowHudUpdated =
		featureInVehicleNoHudUpdated =
		featureDynamicHealthBarUpdated =
		featureWantedMusicUpdated = 
		featureFlyingMusicUpdated =
		featurePoliceScannerUpdated = 
		featureBoostRadio =
		featureBoostRadioUpdated = 
		featurePoliceRadioUpdated = 
		featurePlayerRadioUpdated = 
		featureShowFPSUpdated =
		featureFindDespawnPointerUpdated = true;

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
	if (featurePlayerRadio || featurePlayerRadioUpdated){
		if (featurePlayerRadio){
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
		}
		else{
			AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(false);
		}
	}

	// No Wanted Music
	if (featureWantedMusic || featureWantedMusicUpdated){
		if (featureWantedMusic){
			AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", true);
		}
		else{
			AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", false);
		}
	}

	// No Flying Music
	if (featureFlyingMusic || featureFlyingMusicUpdated){
		if (featureFlyingMusic){
			AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", true);
		}
		else{
			AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", false);
		}
	}

	// No Police Scanner
	if (featurePoliceScanner || featurePoliceScannerUpdated){
		if (featurePoliceScanner){
			AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", true);
		}
		else{
			AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", false);
		}
	}

	// Radio Boost
	if (featureBoostRadio || featureBoostRadioUpdated){
		if (featureBoostRadio) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
				Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				AUDIO::SET_VEHICLE_RADIO_LOUD(playerVeh, 1);
				AUDIO::RELEASE_AMBIENT_AUDIO_BANK();
				//AUDIO::SET_VEHICLE_BOOST_ACTIVE(playerVeh, true); // hissing
			}
		}
	}
	if (!featureBoostRadio) AUDIO::SET_VEHICLE_RADIO_LOUD(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);

	// Radio In Police Vehicles
	if (featurePoliceRadio || featurePoliceRadioUpdated){
		if (featurePoliceRadio){
			Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
			Vector3 coords_radio = ENTITY::GET_ENTITY_COORDS(playerVeh, 1);
			Vector3 coords_radio_2 = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
			if (PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed) && VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(playerVeh)){
				police_radio_check = true;
				AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, true);
				AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
				AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
				AUDIO::SET_RADIO_AUTO_UNFREEZE(true);
				AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
			}
			
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) if (!PED::IS_PED_IN_ANY_POLICE_VEHICLE(playerPed)) police_radio_check = false;

			if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(coords_radio.x, coords_radio.y, coords_radio.z, coords_radio_2.x, coords_radio_2.y, coords_radio_2.z, false) < 3 && 
				VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(playerVeh) && police_radio_check)
			{
				AUDIO::SET_VEHICLE_RADIO_ENABLED(playerVeh, true);
				AUDIO::SET_MOBILE_PHONE_RADIO_STATE(true);
				AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(true);
				AUDIO::SET_RADIO_AUTO_UNFREEZE(true);
				AUDIO::SET_USER_RADIO_CONTROL_ENABLED(true);
			}
		}
	}
	
	// Freeze radio to station
	if (featureRadioFreeze){
		if (AUDIO::GET_PLAYER_RADIO_STATION_INDEX() != radioStationIndex){
			AUDIO::SET_RADIO_TO_STATION_INDEX(radioStationIndex);
		}
	}
	else if (featureRadioFreezeUpdated){
		// Leave it empty for now.
	}

	// hide hud
	if (featureMiscHideHud){
		for (int i = 0; i < 21; i++){
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
	else if (featureMiscHideHudUpdated){
		UI::DISPLAY_RADAR(true);
		featureMiscHideHudUpdated = false;
	}
	
	// show hud if phone in hand
	if (featurePhoneShowHud){
		if (!phone_toggle) 
		{
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
	else if (!featureMiscHideHud && !featureInVehicleNoHud){ // featurePhoneShowHudUpdated &&
		UI::DISPLAY_RADAR(true);
		phone_toggle = false;
	}
	
	// show hud in vehicle only
	if (featureInVehicleNoHud) {
		if (!phone_toggle_vehicle && !featurePhoneShowHud)
		{
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
	else if (!featureMiscHideHud && !featurePhoneShowHud) { // featureInVehicleNoHudUpdated &&
		UI::DISPLAY_RADAR(true);
		phone_toggle_vehicle = false;
	}
	
	// DYNAMIC HEALTH BAR
	if (featureDynamicHealthBar)
	{
		if (!featureMiscHideHud && !featurePhoneShowHud && !featureInVehicleNoHud) UI::DISPLAY_RADAR(false); // There is no need to hide HUD if it's already hidden
		
		auto addr = getScriptHandleBaseAddress(playerPed);
		float health = (*(float *)(addr + 0x280)) - 100;
		float playerArmour = PED::GET_PED_ARMOUR(playerPed);

		if (!ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(playerPed))
		{
			curr_damaged_health = (*(float *)(addr + 0x280)) - 100;
			curr_damaged_armor = PED::GET_PED_ARMOUR(playerPed);
		}
		
		if (curr_damaged_health != health || curr_damaged_armor != playerArmour)
		{
			healthbar_seconds = -1;
			been_damaged = true;
			curr_damaged_health = health;
			curr_damaged_armor = playerArmour;
		}
		
		if (been_damaged == true)
		{
			healthbar_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - healthbar_secs_curr) != 0)
			{
				healthbar_seconds = healthbar_seconds + 1;
				healthbar_secs_curr = healthbar_secs_passed;
			}

			if (healthbar_seconds == 15)
			{
				been_damaged = false;
				healthbar_seconds = -1;
			}

			// Health
			if (health < 20)
			{
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 220, 20, 20, 55);
				if ((health_bar_x + (health / 1399)) > 0.015) GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + (health / 2799), health_bar_y + 0.01, (health / 1399), 0.009, 220, 20, 20, 255);
			}
			else
			{
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.017, 41, 86, 40, 110);
				GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 41, 56, 40, 75);
				if ((health / 1399) < 0.070) GRAPHICS::DRAW_RECT(health_bar_x + 0.00 + (health / 2799), health_bar_y + 0.01, (health / 1399), 0.009, 78, 150, 77, 255);
				else GRAPHICS::DRAW_RECT(health_bar_x + 0.035, health_bar_y + 0.01, 0.070, 0.009, 78, 150, 77, 255);
			}
			// Armor
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0880, health_bar_y + 0.01, 0.036, 0.017, 41, 86, 40, 110);
			GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 41, 56, 40, 90);
			if ((playerArmour / 2935) < 0.035) GRAPHICS::DRAW_RECT(health_bar_x + 0.0715 + (playerArmour / 5871), health_bar_y + 0.01, (playerArmour / 2935), 0.009, 62, 129, 164, 255);
			else GRAPHICS::DRAW_RECT(health_bar_x + 0.0885, health_bar_y + 0.01, 0.034, 0.009, 62, 129, 164, 255);
		}
	}

	// Phone Bill
	if (featurePhoneBillEnabled)
	{
		if (PED::IS_PED_RUNNING_MOBILE_PHONE_TASK(playerPed) && AUDIO::IS_MOBILE_PHONE_CALL_ONGOING())
		{
			secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - secs_curr) != 0)
			{
				temp_seconds = temp_seconds + 1;
				if (temp_seconds > MISC_PHONE_FREESECONDS_VALUES[PhoneFreeSecondsIndex]) bill_seconds = bill_seconds + 1;
				secs_curr = secs_passed;
			}
		}
		
		if (!AUDIO::IS_MOBILE_PHONE_CALL_ONGOING() && temp_seconds != 0) temp_seconds = 0;
		
		if (!AUDIO::IS_MOBILE_PHONE_CALL_ONGOING() && bill_seconds > 0)
		{ 
			int outValue_your_phone_bill = -1;
			int statHash_all_your_money = -1;
			mins = bill_seconds / 60.0;
			bill_to_pay = MISC_PHONE_BILL_VALUES[PhoneBillIndex] * mins;
			
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO)
			{
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP0_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE)
			{
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP1_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO)
			{
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP2_TOTAL_CASH;
				STATS::STAT_SET_INT(statHash_all_your_money, outValue_your_phone_bill - bill_to_pay, true);
			}
			temp_seconds = 0;
			bill_seconds = 0;
		}
		
		if (featureZeroBalance)
		{
			int outValue_your_phone_bill = -1;
			int statHash_all_your_money = -1;
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO)
			{
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP0_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE)
			{
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP1_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO)
			{
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_phone_bill, -1);
				statHash_all_your_money = SP2_TOTAL_CASH;
				if (outValue_your_phone_bill < 1) {
					MOBILE::DESTROY_MOBILE_PHONE();
					CONTROLS::DISABLE_CONTROL_ACTION(2, 27, 1);
				}
			}
		}
	}

	//Show FPS
	//if (featureShowFPS)
	//{
	//	SYSTEM::SETTIMERA(0);
	//	WAIT(0);

	//	std::stringstream ss55;
		//ss55 << "\n featureShowFPS: " << featureShowFPS;
	//	ss55 << "\n FPS: " << fps;
	//	ss55 << "\n TRA: " << SYSTEM::TIMERA();
	//	ss55 << "\n fpsgrab: " << fpsgrab;
	//	callsPerFrame = 0;
	//	set_status_text_centre_screen(ss55.str());

	//	if (SYSTEM::TIMERA() <= 20)
	//	{
	//		fpsgrab = SYSTEM::TIMERA();
	//		FrameCountPre = GAMEPLAY::GET_FRAME_COUNT();
	//	}

	//	if (SYSTEM::TIMERA() >= 25 + fpsgrab)
	//	{
	//		fps = (GAMEPLAY::GET_FRAME_COUNT() - FrameCountPre);
			//PRINT FPS TO SCREEN
			//char *currFPS = new char[fps];
			//strcpy(currSound, ENGINE_SOUND[choice.value].c_str()); 
			//UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			//UI::_ADD_TEXT_COMPONENT_SCALEFORM(currFPS);
			//UI::SET_TEXT_FONT(0);
			//UI::SET_TEXT_SCALE(0.35, 035);
			//UI::SET_TEXT_WRAP(0.0, 1.0);
			//UI::SET_TEXT_COLOUR(240, 160, 0, 200);
			//UI::SET_TEXT_CENTRE(0);
			//UI::SET_TEXT_DROPSHADOW(20, 20, 20, 20, 20);
			//UI::SET_TEXT_EDGE(100, 100, 100, 100, 205);
			//UI::SET_TEXT_LEADING(1); 
			//UI::END_TEXT_COMMAND_DISPLAY_TEXT(0.005f, 0.005f);

	//		SYSTEM::SETTIMERA(0); //Reset timer back to 0 for next frame.
	//	}
	//}

	//Auto find anti-despawn pointer
	if (featureFindDespawnPointer)
	{
		if (!enabledDespawnPointer) 
		{
			set_status_text("Finding and applying despawn pointer");
			FindPatterns();
			FindScriptAddresses();
			EnableCarsGlobal();
			set_status_text("~g~Anti-despawn pointer applied");
		}
	}
	else if (featureFindDespawnPointerUpdated)
	{
		if (despawnPointerDisabledMessage)
		{
			set_status_text("~r~Restart your game to disable Despawn Pointer");
			despawnPointerDisabledMessage = false;
			return;
		}
		enabledDespawnPointer = false;
		return;
	}
}

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerRadio", &featurePlayerRadio, &featurePlayerRadioUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureRadioFreeze", &featureRadioFreeze, &featureRadioFreezeUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureRadioAlwaysOff", &featureRadioAlwaysOff, &featureRadioAlwaysOffUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureBoostRadio", &featureBoostRadio, &featureBoostRadioUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedMUsic", &featureWantedMusic, &featureWantedMusicUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureFlyingMusic", &featureFlyingMusic, &featureFlyingMusicUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceScanner", &featurePoliceScanner, &featurePoliceScannerUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceRadio", &featurePoliceRadio, &featurePoliceRadioUpdated});
	
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscLockRadio", &featureMiscLockRadio});
	results->push_back(FeatureEnabledLocalDefinition{"featureMiscHideHud", &featureMiscHideHud, &featureMiscHideHudUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneShowHud", &featurePhoneShowHud, &featurePhoneShowHudUpdated});
	results->push_back(FeatureEnabledLocalDefinition{ "featureInVehicleNoHud", &featureInVehicleNoHud, &featureInVehicleNoHudUpdated });

	results->push_back(FeatureEnabledLocalDefinition{"featureDynamicHealthBar", &featureDynamicHealthBar, &featureDynamicHealthBarUpdated});
		
	results->push_back(FeatureEnabledLocalDefinition{"featureControllerIgnoreInTrainer", &featureControllerIgnoreInTrainer});
	results->push_back(FeatureEnabledLocalDefinition{"featureBlockInputInMenu", &featureBlockInputInMenu});
	results->push_back(FeatureEnabledLocalDefinition{"featurePhoneBillEnabled", &featurePhoneBillEnabled});
	results->push_back(FeatureEnabledLocalDefinition{"featureZeroBalance", &featureZeroBalance});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowVehiclePreviews", &featureShowVehiclePreviews});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowFPS", &featureShowFPS });

	//results->push_back(FeatureEnabledLocalDefinition{ "featureFindDespawnPointer", &featureFindDespawnPointer, &featureFindDespawnPointerUpdated });

	results->push_back(FeatureEnabledLocalDefinition{"featureMiscJellmanScenery", &featureMiscJellmanScenery});

	results->push_back(FeatureEnabledLocalDefinition{"featureResetPlayerModelOnDeath", &featureResetPlayerModelOnDeath});
}

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"radioStationIndex", std::to_string(radioStationIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneBillIndex", std::to_string(PhoneBillIndex)});
	results->push_back(StringPairSettingDBRow{"PhoneFreeSecondsIndex", std::to_string(PhoneFreeSecondsIndex)});
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
		else if (setting.name.compare("PhoneFreeSecondsIndex") == 0){
			PhoneFreeSecondsIndex = stoi(setting.value);
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
	featurePhoneShowHudUpdated = true;
}

bool is_jellman_scenery_enabled(){
	return featureMiscJellmanScenery;
}

/* Code thanks to Zorg93's MPCar Enabler. I just butchered it to fit... */
void FindScriptAddresses()
{

	while (*(__int64*)scriptTable == 0)
	{
		Sleep(100);
	}
	//write_text_to_log_file("Found script base pointer %llX", (__int64)scriptTable);
	ScriptTableItem* Item = scriptTable->FindScript(0x39DA738B);
	if (Item == NULL)
	{
		write_text_to_log_file("Exception: finding address 2 (0x39DA738B)");
		write_text_to_log_file("Aborting...");
		FreeLibraryAndExitThread(_hinstDLL, 0);
	}
	while (!Item->IsLoaded())
	{
		Sleep(100);
	}
	shopController = Item->Header;

	Item = scriptTable->FindScript(0xAFD9916D);
	if (Item == NULL)
	{
		write_text_to_log_file("Exception: finding address 3 (0xAFD9916D)");
		write_text_to_log_file("Aborting...");
		FreeLibraryAndExitThread(_hinstDLL, 0);
	}
	while (!Item->IsLoaded())
	{
		Sleep(100);
	}
	//cheatController = Item->Header;
}

void FindPatterns()
{
	__int64 patternAddr = FindPattern2("\x4C\x8D\x05\x00\x00\x00\x00\x4D\x8B\x08\x4D\x85\xC9\x74\x11", "xxx????xxxxxxxx");
	if (!patternAddr)
	{
		write_text_to_log_file("ERROR: finding address 0");
		write_text_to_log_file("Aborting...");
		FreeLibraryAndExitThread(_hinstDLL, 0);
	}
	globalTable.GlobalBasePtr = (__int64**)(patternAddr + *(int*)(patternAddr + 3) + 7);


	patternAddr = FindPattern2("\x48\x03\x15\x00\x00\x00\x00\x4C\x23\xC2\x49\x8B\x08", "xxx????xxxxxx");
	if (!patternAddr)
	{
		write_text_to_log_file("ERROR: finding address 1");
		write_text_to_log_file("Aborting...");
		FreeLibraryAndExitThread(_hinstDLL, 0);
	}
	scriptTable = (ScriptTable*)(patternAddr + *(int*)(patternAddr + 3) + 7);


	//patternAddr = FindPattern("\x80\xF9\x05\x75\x08\x48\x05\x00\x00\x00\x00", "xxxxxxx????");
	//GetModelInfo = (__int64(*)(int, __int64))(*(int*)(patternAddr - 0x12) + patternAddr - 0x12 + 0x4);
	//displayNameOffset = *(int*)(patternAddr + 0x7);
	//__int64 getHashKeyPattern = FindPattern("\x48\x8B\x0B\x33\xD2\xE8\x00\x00\x00\x00\x89\x03", "xxxxxx????xx");
	//GetHashKey = reinterpret_cast<int(*)(char*, unsigned int)>(*reinterpret_cast<int*>(getHashKeyPattern + 6) + getHashKeyPattern + 10);

	while (!globalTable.IsInitialised())Sleep(100);//Wait for GlobalInitialisation before continuing
	//write_text_to_log_file("Found global base pointer %llX", (__int64)globalTable.GlobalBasePtr);
}

uintptr_t FindPattern2(const char *pattern, const char *mask, const char* startAddress, size_t size)
{
	const char* address_end = startAddress + size;
	const auto mask_length = static_cast<size_t>(strlen(mask) - 1);

	for (size_t i = 0; startAddress < address_end; startAddress++)
	{
		if (*startAddress == pattern[i] || mask[i] == '?')
		{
			if (mask[i + 1] == '\0')
			{
				return reinterpret_cast<uintptr_t>(startAddress) - mask_length;
			}

			i++;
		}
		else
		{
			i = 0;
		}
	}

	return 0;
}

uintptr_t FindPattern2(const char *pattern, const char *mask)
{
	MODULEINFO module = {};
	GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &module, sizeof(MODULEINFO));

	return FindPattern2(pattern, mask, reinterpret_cast<const char *>(module.lpBaseOfDll), module.SizeOfImage);
}

void EnableCarsGlobal()
{
	for (int i = 0; i < shopController->CodePageCount(); i++)
	{
		__int64 sigAddress = FindPattern2("\x28\x26\xCE\x6B\x86\x39\x03", "xxxxxxx", (const char*)shopController->GetCodePageAddress(i), shopController->GetCodePageSize(i));
		if (!sigAddress)
		{
			continue;
		}
		//DEBUGMSG("Pattern Found in codepage %d at memory address %llX", i, sigAddress);
		int RealCodeOff = (int)(sigAddress - (__int64)shopController->GetCodePageAddress(i) + (i << 14));
		for (int j = 0; j < 2000; j++)
		{
			if (*(int*)shopController->GetCodePositionAddress(RealCodeOff - j) == 0x0008012D)
			{
				int funcOff = *(int*)shopController->GetCodePositionAddress(RealCodeOff - j + 6) & 0xFFFFFF;
				//DEBUGMSG("found Function codepage address at %x", funcOff);
				for (int k = 0x5; k < 0x40; k++)
				{
					if ((*(int*)shopController->GetCodePositionAddress(funcOff + k) & 0xFFFFFF) == 0x01002E)
					{
						for (k = k + 1; k < 30; k++)
						{
							if (*(unsigned char*)shopController->GetCodePositionAddress(funcOff + k) == 0x5F)
							{
								int globalindex = *(int*)shopController->GetCodePositionAddress(funcOff + k + 1) & 0xFFFFFF;
								//DEBUGMSG("Found Global Variable %d, address = %llX", globalindex, (__int64)globalTable.AddressOf(globalindex));
								write_text_to_log_file("Found global despawn pointer and setting anti-despawn pointer to true"); //("Setting Global Variable %d to true", globalindex)
								*globalTable.AddressOf(globalindex) = 1;
								enabledDespawnPointer = true;
								//Log::Msg("MP Cars enabled");
								//FindSwitch(RealCodeOff - j);
								//atchCheatController();
								return;
							}
						}
						break;
					}
				}
				break;
			}
		}
		break;
	}
	write_text_to_log_file("Global Variable not found, check game version >= 1.0.678.1");
}