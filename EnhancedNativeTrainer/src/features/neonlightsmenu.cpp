/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

int menuIndex = 0;

void apply_neon_colors(int colorIndex){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // 获取当前载具
	NeonLightsColor whichcolor = NEON_COLORS[colorIndex];
	VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
}

void onhighlight_neon_lights_selection(MenuItem<int> choice){
	onconfirm_neon_lights_selection(choice);
}

bool onconfirm_neon_lights_selection(MenuItem<int> choice){
	// 通用变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return true;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("~r~错误：~r~玩家不在车辆内！");
		return true;
	}

	apply_neon_colors(choice.value);

	return true;
}

bool is_neonLights(std::vector<int> extras){
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if(VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, loc)){
		return true; // 一旦检测到至少一个霓虹灯开启，立即返回
	}

	return false;
}

void set_neonLights(bool applied, std::vector<int> extras){
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int rCol = 0, gCol = 0, bCol = 0;
	bool lightFound = false;

	if(!is_this_a_car(veh) && !is_this_a_motorcycle(veh)){
		set_status_text("~r~错误：~r~无法为，此辆车添加霓虹灯！");
		return;
	}

	if(applied) // 开启霓虹灯
	{
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, true);
		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &rCol, &gCol, &bCol);
		if(!rCol && !gCol && !bCol){
			NeonLightsColor col = NEON_COLORS.at(0);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else{
		// 关闭灯光
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, false);
	}
}

std::string getNeonPositionLabel(int i){
	switch(i){
		case NEON_LEFT:
			return "左侧";
		case NEON_RIGHT:
			return "右侧";
		case NEON_FRONT:
			return "前面";
		case NEON_BACK:
			return "后面";
		default:
			return "未知？";
	}
}

bool process_neon_colour_menu(){
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int r = 0, g = 0, b = 0;
	VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &r, &g, &b);

	for(int i = 0; i < NEON_COLORS.size(); i++){
		NeonLightsColor thisCol = NEON_COLORS[i];

		//尝试将当前颜色匹配到一个值，以便设置菜单索引
		if(colIndex == -1 && r == thisCol.rVal && g == thisCol.gVal && b == thisCol.bVal){
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = NEON_COLORS[i].colorString;
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if(colIndex == -1){
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "霓虹灯颜色选项", onconfirm_neon_lights_selection, onhighlight_neon_lights_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_neon_menu(MenuItem<int> choice){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if(choice.value == -1) //切换所有
	{
		bool anyEnabled = false;
		int r = 0, g = 0, b = 0;

		for(int loc = 0; loc <= 3; loc++){
			if(VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, loc)){
				anyEnabled = true;
				break;
			}
		}
		for(int loc = 0; loc <= 3; loc++){
			if(r == 0 && g == 0 && b == 0){
				VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &r, &g, &b);
			}
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, loc, !anyEnabled);
		}

		// 如果没有颜色，则设置为默认值
		if(!anyEnabled && r == 0 && g == 0 && b == 0){
			NeonLightsColor col = NEON_COLORS.at(0);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else if(choice.value == -2){
		process_neon_colour_menu();
	}
	return false;
}


bool process_neon_lights_menu(){
	// 通用变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // 获取当前载具

	if(!is_this_a_car(veh) && !is_this_a_motorcycle(veh)){
		set_status_text("无法给此车辆添加霓虹灯！ ");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *allLightsToggle = new MenuItem<int>();
	allLightsToggle->caption = "开关所有霓虹灯";
	allLightsToggle->value = -1;
	allLightsToggle->isLeaf = true;
	menuItems.push_back(allLightsToggle);

	for(int loc = 0; loc <= 3; loc++){
		FunctionDrivenToggleMenuItem<int> *neonLightsToggle = new FunctionDrivenToggleMenuItem<int>();
		std::ostringstream ss;
		ss << "开启霓虹灯: " << getNeonPositionLabel(loc);
		neonLightsToggle->caption = ss.str();
		neonLightsToggle->getter_call = is_neonLights;
		neonLightsToggle->setter_call = set_neonLights;
		neonLightsToggle->extra_arguments.push_back(loc);
		menuItems.push_back(neonLightsToggle);
	}

	MenuItem<int> *chooseColourAll = new MenuItem<int>();
	chooseColourAll->caption = UI::_GET_LABEL_TEXT("CMOD_NEON_1");
	//chooseColourAll->caption = "Choose Colour";
	chooseColourAll->value = -2;
	chooseColourAll->isLeaf = false;
	menuItems.push_back(chooseColourAll);

	return draw_generic_menu<int>(menuItems, &menuIndex, "霓虹灯", onconfirm_neon_menu, NULL, NULL, vehicle_menu_interrupt);
}
