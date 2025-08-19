/*
这段代码最初是 GTA V SCRIPT HOOK SDK 的一部分。
http://dev-c.com
(C) Alexander Blade 2015

它现在是 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 和其他贡献者 2015
*/

#include "io.h"
#include "config_io.h"
#include "..\features\script.h"
#include <string>
#include <sstream>
#include "../io/controller.h"

DWORD trainerResetTime = 0;

bool gameInputDisabledByUs = false;
bool gameInputBlockedByUs = false;

bool trainer_switch_pressed()
{
	/*std::stringstream ss;
	ss << "Map first: " << controller_binds["KEY_TOGGLE_MAIN_MENU"].first << " Map second: " << controller_binds["KEY_TOGGLE_MAIN_MENU"].second;
	set_status_text_centre_screen(ss.str());*/

	bool result = IsKeyJustUp(KeyConfig::KEY_TOGGLE_MAIN_MENU) || (CONTROLS::IS_CONTROL_PRESSED(2, controller_binds["KEY_TOGGLE_MAIN_MENU"].first) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, controller_binds["KEY_TOGGLE_MAIN_MENU"].second)); //INPUT_FRONTEND_RB + INPUT_ENTER (Y)
	if (result)
	{
		//避免重复按键
		DWORD maxTickCount = GetTickCount() + 200;
		do
		{
			make_periodic_feature_call();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
	}
	return result;
}

void get_button_state(bool *a, bool *b, bool *up, bool *down, bool *l, bool *r)
{
	KeyInputConfig *keyConf = config->get_key_config();
	if (a) *a = IsKeyJustUp(KeyConfig::KEY_MENU_SELECT) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_ACCEPT); //A //IS_DISABLED_CONTROL_PRESSED
	if (b) *b = (is_menu_showing() && !IsKeyJustUp(VK_ESCAPE) && IsKeyJustUp(KeyConfig::KEY_MENU_BACK)) || (is_menu_showing() && (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_CANCEL) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_PAUSE))); //B

	if (MISC_TRAINERCONTROL_VALUES[TrainerControlIndex] == 0) { // 按压
		if (up) *up = IsKeyDown(KeyConfig::KEY_MENU_UP) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_UP); //Dpad up
		if (down) *down = IsKeyDown(KeyConfig::KEY_MENU_DOWN) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_DOWN); //Dpad down
		if (r) *r = IsKeyDown(KeyConfig::KEY_MENU_RIGHT) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_RIGHT); //Dpad right
		if (l) *l = IsKeyDown(KeyConfig::KEY_MENU_LEFT) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_FRONTEND_LEFT); //Dpad left
	}
	if (MISC_TRAINERCONTROL_VALUES[TrainerControlIndex] == 1) { // 松开
		if (up) *up = IsKeyJustUp(KeyConfig::KEY_MENU_UP) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_UP);
		if (down) *down = IsKeyJustUp(KeyConfig::KEY_MENU_DOWN) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_DOWN);
		if (r) *r = IsKeyJustUp(KeyConfig::KEY_MENU_RIGHT) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_RIGHT);
		if (l) *l = IsKeyJustUp(KeyConfig::KEY_MENU_LEFT) || CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, INPUT_FRONTEND_LEFT);
	}
}

bool get_key_pressed(int nVirtKey)
{
	// 返回 (GetKeyState(nVirtKey) & 0x8000) != 0;
	return (GetAsyncKeyState(nVirtKey) & 0x8000) != 0;
}

bool airbrake_switch_pressed()
{
	bool result = IsKeyJustUp(KeyConfig::KEY_TOGGLE_AIRBRAKE) || (CONTROLS::IS_CONTROL_PRESSED(2, controller_binds["KEY_TOGGLE_AIRBRAKE"].first) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, controller_binds["KEY_TOGGLE_AIRBRAKE"].second)); //INPUT_FRONTEND_ACCEPT (A) + INPUT_FRONTEND_RB (RB)
	if (result)
	{
		// 避免重复按键
		DWORD maxTickCount = GetTickCount() + 200;
		do
		{
			make_periodic_feature_call();
			WAIT(0);
		} while (GetTickCount() < maxTickCount);
	}
	return result;
}

void reset_trainer_switch()
{
	trainerResetTime = GetTickCount();
}

void setGameInputToEnabled(bool enabled, bool force)
{
	if (enabled && (gameInputDisabledByUs || force))
	{
		PLAYER::SET_PLAYER_CONTROL(0, 1, 0);
		gameInputDisabledByUs = false;
	}
	else if (!enabled)
	{
		PLAYER::SET_PLAYER_CONTROL(0, 0, 256);
		gameInputDisabledByUs = true;
	}
}

void setAirbrakeRelatedInputToBlocked(bool blocked, bool force)
{
	if (blocked || force || (!blocked && gameInputBlockedByUs))
	{
		void(*function)(int index, int control, BOOL enable);
		if (blocked)
		{
			function = CONTROLS::DISABLE_CONTROL_ACTION;
		}
		else
		{
			function = CONTROLS::ENABLE_CONTROL_ACTION;
		}
		
		function(2, 332, 1); // 收音机滚轮向上
		function(2, 333, 1); // 收音机滚轮向下

		for (int i = 81; i <= 85; i++)
		{
			function(2, i, 1); // 收音机相关的东西
		}

		gameInputBlockedByUs = blocked;
	}
}