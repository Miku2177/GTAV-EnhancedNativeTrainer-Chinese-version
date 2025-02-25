/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith 及其他贡献者 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"
#include "props.h"
#include <string>

void begin_prop_placement(SpawnedPropInstance prop);

void prop_placement();

void create_prop_placement_help_text();

void update_prop_placement_text();

bool is_in_prop_placement_mode();

bool is_prop_placement_frozen_time();
