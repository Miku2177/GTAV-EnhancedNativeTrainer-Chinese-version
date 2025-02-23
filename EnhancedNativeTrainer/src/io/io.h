/*
这段代码最初是 GTA V SCRIPT HOOK SDK 的一部分。
http://dev-c.com
(C) Alexander Blade 2015

它现在是 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 和其他贡献者 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "keyboard.h"

bool get_key_pressed(int nVirtKey);

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r);

bool trainer_switch_pressed();

void reset_trainer_switch();

bool airbrake_switch_pressed();

void setGameInputToEnabled(bool enabled, bool force = false);

void setAirbrakeRelatedInputToBlocked(bool blocked, bool force = false);
