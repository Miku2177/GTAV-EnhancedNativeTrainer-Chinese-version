/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include <string>
#include <vector>
#include "..\storage\database.h"

void process_time_menu();

void reset_time_globals();

void update_time_features(Player player);

void add_time_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_time_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_time(std::vector<StringPairSettingDBRow>* settings);

void all_time_flow_rate();

void movetime_day_forward();

void movetime_day_backward();

void set_date();

void set_time();

void toggle_game_speed();

void movetime_hour_forward();

void movetime_hour_backward();

void movetime_fivemin_forward();

void movetime_fivemin_backward();

void movetime_set(int hour, int minute);

std::string get_day_of_game_week();

bool flowtime_menu_interrupt();