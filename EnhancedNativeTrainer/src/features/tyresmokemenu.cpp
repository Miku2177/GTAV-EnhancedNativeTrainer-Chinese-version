#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

void apply_smoke_colors(int colorIndex){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); // 获取当前载具
	TireSmokeColor whichcolor = SMOKE_COLORS[colorIndex];
	VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, whichcolor.rVal, whichcolor.gVal, whichcolor.bVal);
	VEHICLE::TOGGLE_VEHICLE_MOD(veh, 20, 1);
}

void onhighlight_smoke_selection(MenuItem<int> choice){
	onconfirm_smoke_selection(choice);
}

bool onconfirm_smoke_selection(MenuItem<int> choice){
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return true;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return true;
	}

	apply_smoke_colors(choice.value);

	return true;
}

void set_smoke(bool applied, std::vector<int> extras){
	int loc = extras.at(0);
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int rCol, bCol, gCol = bCol = rCol = 0;
	bool lightFound = false;

	if(!is_this_a_car(veh) || !is_this_a_motorcycle(veh)){
		set_status_text("无法为该车辆添加烟雾效果！");
		return;
	}

	if(applied) // 开启烟雾效果
	{
		if(!rCol && !bCol && !gCol){
			TireSmokeColor col = SMOKE_COLORS.at(0);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, col.rVal, col.gVal, col.bVal);
		}
	}
	else{
		// 默认烟雾效果
		VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, 255, 255, 255);
	}
}

bool process_smoke_colour_menu(){
	std::vector<MenuItem<int>*> menuItems;

	int colIndex = -1;

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int r = 0, g = 0, b = 0;
	VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &r, &g, &b);

	for(int i = 0; i < SMOKE_COLORS.size(); i++){
		TireSmokeColor thisCol = SMOKE_COLORS[i];

		//尝试将当前颜色匹配到一个值，以便设置菜单索引
		if(colIndex == -1 && r == thisCol.rVal && g == thisCol.gVal && b == thisCol.bVal){
			colIndex = i;
		}

		MenuItem<int> *item = new MenuItem<int>();
		std::string smoke_label = SMOKE_COLORS[i].colorString;
		item->caption = UI::_GET_LABEL_TEXT(&smoke_label[0]);
		item->isLeaf = true;
		item->value = i;
		menuItems.push_back(item);
	}

	if(colIndex == -1){
		colIndex = 0;
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "轮胎烟雾颜色选项", onconfirm_smoke_selection, onhighlight_smoke_selection, NULL, vehicle_menu_interrupt);
}