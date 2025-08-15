/*
部分代码最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

现为增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "vehicles.h"
#include "speed_altitude.h"
#include "..\features\vehmodmenu.h"
#include "hotkeys.h"
#include "script.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "..\debug\debuglog.h"
#include "area_effect.h"
#include <fstream>
#include "vehicle_weapons.h"
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <cstdlib>

bool featureKMH = false;
bool featureAltitude = true;
bool featureSpeedOnFoot = false;
bool featureSpeedOnGround = false;
bool featureSpeedInAir = false;

int SpeedSizeIndex = 0;
bool SizeChanged = true;
int SpeedPositionIndexN = 0;
bool PositionChanged = true;

float textX, textY = -1;
float rectXScaled, rectYScaled = -1;

//////////////////////////////////////////// 显示速度 / 高度 ///////////////////////////////////////////
void update_speed_text(int speed, Vector3 player_coords)
{
	std::string speedometerStatusLines[1];
	std::stringstream ss;
	int col2_R = ENTColor::colsMenu[5].rgba[0];
	int col2_G = ENTColor::colsMenu[5].rgba[1];
	int col2_B = ENTColor::colsMenu[5].rgba[2];

	// 速度高度字体颜色：将字体颜色改为白色（原为深橙色）
	col2_R = 255;
	col2_G = 242;
	col2_B = 0;

	int numLines = sizeof(speedometerStatusLines) / sizeof(speedometerStatusLines[0]);

	if (featureKMH) {
		ss << "千米/时:    " << round((speed * 1.609344) * 2.3);

		if (featureAltitude) {
			ss << "\n海拔高度:  " << floor(player_coords.z * 1) / 1;
		}
	}
	else {
		ss << "英里/时:    " << round(speed * 2.3);

		if (featureAltitude) {
			ss << "\n海拔高度:  " << floor(player_coords.z * 1) / 1;
		}
	}

	int index = 0;
	speedometerStatusLines[index++] = ss.str();
	float size = SPEED_SIZE_VALUES[SpeedSizeIndex];
	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 0) { // 右下角
		textX = (97.4 - (size * 2.5)) / 100;
		textY = (85 - (size * 1.2)) / 100;
	}
	if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 1) { // 底部居中
		textX = (50 - (size * 1.1)) / 100;
		textY = (95 - (size * 1.2)) / 100;
	}
	if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 2) { // 右上角
		textX = (97.4 - (size * 2.5)) / 100;
		textY = (10.5 + (size * 0.0001)) / 100;
	}

	int numActualLines = 0;
	for (int i = 0; i < numLines; i++) {
		numActualLines++;
		UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)speedometerStatusLines[i].c_str());
		text_parameters(size / 10, size / 10, col2_R, col2_G, col2_B, 255);
		UI::END_TEXT_COMMAND_DISPLAY_TEXT(textX, textY);
		textY += 0.025f;
	}

	//if (size < 4) { // 绘制背景
	if (SpeedSizeIndex < 1) { // 绘制背景
		if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 0) { // 右下角
			rectXScaled = 1 - ((300 / (float)screen_w) / 4);
			rectYScaled = 0.95 - (((0 + (1 * 18)) / (float)screen_h) * 5);
		}
		if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 1) { // 底部居中
			rectXScaled = 0.55 - ((230 / (float)screen_w) / 4);
			rectYScaled = 1 - (((0 + (1 * 11)) / (float)screen_h) * 5);
		}
		if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 2) { // 右上角
			rectXScaled = 1 - ((300 / (float)screen_w) / 4);
			rectYScaled = 0.24 - (((0 + (1 * 18)) / (float)screen_h) * 5);
		}
		float rectWidthScaled = (230 / (float)screen_w) / 2;
		float rectHeightScaled = (0 + (1 * 18)) / (float)screen_h;
		int rect_col[4] = { 0, 0, 0, 180 }; // 128, 128, 128, 75   速度显示 背景透明度
		GRAPHICS::DRAW_RECT(rectXScaled, rectYScaled, rectWidthScaled, rectHeightScaled, rect_col[0], rect_col[1], rect_col[2], rect_col[3]);

		if (featureAltitude) {
			if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 0) { // 右下角
				rectXScaled = 1 - ((300 / (float)screen_w) / 4);
				rectYScaled = 0.95 - (((0 + (1 * 18)) / (float)screen_h) * 5) + ((0 + (1 * 18)) / (float)screen_h);
			}
			if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 1) { // 底部居中
				rectXScaled = 0.55 - ((230 / (float)screen_w) / 4);
				rectYScaled = 1 - (((0 + (1 * 11)) / (float)screen_h) * 5) + ((0 + (1 * 18)) / (float)screen_h);
			}
			if (NPC_RAGDOLL_VALUES[SpeedPositionIndexN] == 2) { // 右上角
				rectXScaled = 1 - ((300 / (float)screen_w) / 4);
				rectYScaled = 0.24 - (((0 + (1 * 18)) / (float)screen_h) * 5) + ((0 + (1 * 18)) / (float)screen_h);
			}
			float rectWidthScaled = (230 / (float)screen_w) / 2;
			float rectHeightScaled = (0 + (1 * 18)) / (float)screen_h;
			int rect_col[4] = { 0, 0, 0, 180 }; // 128, 128, 128, 75   高度显示 背景透明度
			GRAPHICS::DRAW_RECT(rectXScaled, rectYScaled, rectWidthScaled, rectHeightScaled, rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
		}
	}
}

void update_speedaltitude(Ped playerPed) {

	// 步行
	if (featureSpeedOnFoot) {
		if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			update_speed_text(ENTITY::GET_ENTITY_SPEED(playerPed), ENTITY::GET_ENTITY_COORDS(playerPed, true));
		}
	}

	// 在地面上
	if (featureSpeedOnGround) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			Entity veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (!is_this_a_heli_or_plane(veh)) {
				update_speed_text(ENTITY::GET_ENTITY_SPEED(veh), ENTITY::GET_ENTITY_COORDS(playerPed, true));
			}
		}
	}

	// 在空中
	if (featureSpeedInAir) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			Entity veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (is_this_a_heli_or_plane(veh)) {
				update_speed_text(ENTITY::GET_ENTITY_SPEED(veh), ENTITY::GET_ENTITY_COORDS(playerPed, true));
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////