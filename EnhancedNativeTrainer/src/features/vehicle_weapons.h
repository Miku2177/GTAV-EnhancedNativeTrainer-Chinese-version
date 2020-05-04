/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Zenamez and fellow contributors 2017
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"
#include <sstream>
#include <vector>

void toggle_bomb_bay_camera();

void start_bombing_run();

void update_bombs();

void play_explosion(Entity ent);

bool process_veh_weapons_menu();

bool onconfirm_veh_weapons_menu(MenuItem<int> choice);