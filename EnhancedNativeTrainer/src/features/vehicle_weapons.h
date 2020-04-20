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

//Object create_bomb(Vector3 position, Vector3 rotation);

void toggle_bomb_bay_camera();

//void plane_autopilot(bool enabled);

void start_bombing_run();

//void add_bomb();

//void cleanup_active_bomb();

//void cleanup_active_sounds();

void update_bombs();

void play_explosion(Entity ent);

bool process_veh_weapons_menu();

bool onconfirm_veh_weapons_menu(MenuItem<int> choice);