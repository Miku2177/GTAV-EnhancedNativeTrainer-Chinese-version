/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\io\io.h"
#include "..\io\config_io.h"
#include "..\io\xinput.h"
#include "..\ui_support\menu_functions.h"
#include "..\storage\database.h"
#include "..\debug\debuglog.h"

#include "skins.h"
#include "script.h"
#include "vehicles.h"
#include "misc.h"
#include "teleportation.h"
#include "airbrake.h"
#include "weapons.h"
#include "aimbot_esp.h"
#include "anims.h"
#include "world.h"
#include "time.h"
#include "props.h"
#include "bodyguards.h"
#include "vehmodmenu.h"

#include <DbgHelp.h>
#include <ShlObj.h>
#include <windows.h>
#include <Shlwapi.h>

#include <string>
#include <sstream> 
#include <fstream>
#include <mutex>
#include <thread>

#include <ctime>
#include <cctype>
#include <vector>

#include <locale>
#include <iostream>
#include <iomanip>

#include <fstream>

//Ensures numbers are formatted with commas, not the locale option
class comma_numpunct: public std::numpunct<char>{
	protected:
	virtual char do_thousands_sep() const{
		return ',';
	}

	virtual std::string do_grouping() const{
		return "\03";
	}
};

// trim from start
static inline std::string &ltrim(std::string &s){
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s){
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s){
	return ltrim(rtrim(s));
}

void ScriptMain();

void ScriptTidyUp();

bool process_skinchanger_menu();

bool process_skinchanger_detail_menu();

bool player_movement_speed();

bool mostwanted_menu();

bool maxwantedlevel_menu();

bool process_ragdoll_menu();

bool process_player_life_menu();

void update_features();

void reset_globals();

void onchange_player_health_mode(int value, SelectFromListMenuItem* source);

void onchange_player_armor_mode(int value, SelectFromListMenuItem* source);

void onchange_regen_callback(int value, SelectFromListMenuItem* source);

void onchange_player_stats_mode(int value, SelectFromListMenuItem* source);

void onchange_player_prison_mode(int value, SelectFromListMenuItem* source);

void onchange_player_mostwanted_mode(int value, SelectFromListMenuItem* source);

void onchange_player_mostwanted_level_mode(int value, SelectFromListMenuItem* source);

void onchange_player_wanted_maxpossible_level_mode(int value, SelectFromListMenuItem* source);

void onchange_player_movement_mode(int value, SelectFromListMenuItem* source);

void onchange_NPC_ragdoll_mode(int value, SelectFromListMenuItem* source);

void onchange_player_escapemoney_mode(int value, SelectFromListMenuItem* source);

void onchange_player_discharge_mode(int value, SelectFromListMenuItem* source);

void onchange_player_escapestars_mode(int value, SelectFromListMenuItem* source);

void process_reset_menu();

void process_main_menu();

void process_powerpunch_menu();

void set_all_nearby_peds_to_calm();

//DB persistent stuff

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements();

std::vector<StringPairSettingDBRow> get_generic_settings();

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings);

void add_world_feature_enablements3(std::vector<StringPairSettingDBRow>* results);

bool getFrozenWantedFeature();

void updateFrozenWantedFeature(int level);

void load_settings();

void save_settings();

DWORD WINAPI save_settings_thread(LPVOID lpParameter);

void init_storage();

int get_frame_number();

WCHAR* get_storage_dir_path();

WCHAR* get_storage_dir_path(char* file);

WCHAR* get_temp_dir_path();

WCHAR* get_temp_dir_path(char* file);

ENTDatabase* get_database();

void process_test_menu();

void debug_native_investigation();

void heal_player();

void toggle_invisibility();

void fill_weapon_ammo_hotkey();

void reset_wanted_level();

void toggle_thermal_vision();

void toggle_night_vision();

bool get_graphics_test(std::vector<int> extras);

void set_graphics_test(bool applied, std::vector<int> extras);

void cleanup_script();

bool is_player_ignored_by_police();

extern Vehicle temp_vehicle;
extern Ped temp_ped;
extern bool featurePlayerInvisible;
extern bool featurePlayerInvisibleInVehicle;
extern bool featureShowStatusMessage;
extern bool featureNoAutoRespawn;
extern bool player_died;
extern bool npc_player_died;
extern bool featureWantedLevelFrozen;
extern const std::vector<std::string> PLAYER_HEALTH_CAPTIONS;
extern const int PLAYER_HEALTH_VALUES[];
extern bool in_prison;
extern bool super_jump_no_parachute;
extern int myGroup;