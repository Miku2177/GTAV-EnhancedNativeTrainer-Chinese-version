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

void create_bomb(Vector3 position, Vector3 rotation);

void toggle_bomb_bay_camera();

void plane_autopilot(bool enabled);

bool process_veh_weapons_menu();

bool onconfirm_veh_weapons_menu(MenuItem<int> choice);

void reset_veh_weapons_globals();

void update_veh_weapons_features();