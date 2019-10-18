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

#include <string>
#include <vector>

#include "..\storage\database.h"
#include <cstdint>




#include "..\..\inc\main.h"
#include "..\ui_support\menu_functions.h"
#include "..\ent-enums.h"
#include "..\utils.h"
#include "..\common\ENTUtil.h"
#include <random>

extern bool featureWorldMoonGravity;

void process_world_menu();

void process_weather_menu();

void process_clouds_menu();

void reset_world_globals();

void update_world_features();

void onchange_world_radar_map_index(int value, SelectFromListMenuItem* source);

void onchange_world_waves_index(int value, SelectFromListMenuItem* source);

void onchange_lightning_intensity_index(int value, SelectFromListMenuItem* source);

void onchange_world_reducedgrip_snowing_index(int value, SelectFromListMenuItem* source);

void onchange_world_reducedgrip_raining_index(int value, SelectFromListMenuItem* source);

void onchange_world_headlights_blackout_index(int value, SelectFromListMenuItem* source);

void onchange_world_train_speed_index(int value, SelectFromListMenuItem* source);

//void onchange_world_damaged_vehicles_index(int value, SelectFromListMenuItem* source);

void onchange_world_no_peds_gravity_index(int value, SelectFromListMenuItem* source);

void onchange_peds_health_index(int value, SelectFromListMenuItem* source);

void onchange_world_npc_vehicles_speed_index(int value, SelectFromListMenuItem* source);

void onchange_ped_accuracy_index(int value, SelectFromListMenuItem* source);

void onchange_world_wind_strength_index(int value, SelectFromListMenuItem* source);

void add_world_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_world_feature_enablements2(std::vector<StringPairSettingDBRow>* results);

void add_world_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_world(std::vector<StringPairSettingDBRow>* settings);

//Snow related 
//void writeJmp(BYTE* pFrom, BYTE* pTo);

bool CompareMemory(const uint8_t* pData, const uint8_t* bMask, const char* sMask);

extern bool featureFirstPersonDeathCamera;

//intptr_t FindPattern(const char* bMask, const char* sMask);

void EnableTracks(bool tracksVehicle, bool tracksPeds, bool deepTracksVehicle, bool deepTracksPed);

void EnableSnow(bool featureSnow);