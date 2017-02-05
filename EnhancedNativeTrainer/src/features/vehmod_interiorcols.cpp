#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

std::vector<PaintColor> PAINTS_ALL;

void apply_dash_colors(int colorIndex){
	VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), PAINTS_ALL.at(colorIndex).colorIndex);
}

void onhighlight_dash_colour_selection(MenuItem<int> choice){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(playerPed)){
		return;
	}

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return;
	}

	apply_dash_colors(choice.value);
}

bool onconfirm_dash_colour_selection(MenuItem<int> choice){
	return true;
}

bool process_dash_colour_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int colIndex = 0, currDashCol;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &currDashCol);

	for(int a = 0; a < PAINTS_ALL.size(); a++){
		PaintColor b = PAINTS_ALL.at(a);

		if(currDashCol == b.colorIndex){
			colIndex = a;
		}

		item = new MenuItem<int>();
		item->caption = b.name;
		item->value = a;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Dash Color", onconfirm_dash_colour_selection, onhighlight_dash_colour_selection, NULL, vehicle_menu_interrupt);
}

void apply_trim_colors(int colorIndex){
	VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), PAINTS_ALL.at(colorIndex).colorIndex);
}

void onhighlight_trim_colour_selection(MenuItem<int> choice){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(playerPed)){
		return;
	}

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return;
	}

	apply_trim_colors(choice.value);
}

bool onconfirm_trim_colour_selection(MenuItem<int> choice){
	return true;
}

bool process_trim_colour_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int colIndex = 0, currTrimCol;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &currTrimCol);

	for(int a = 0; a < PAINTS_ALL.size(); a++){
		PaintColor b = PAINTS_ALL.at(a);

		if(currTrimCol == b.colorIndex){
			colIndex = a;
		}

		item = new MenuItem<int>();
		item->caption = b.name;
		item->value = a;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "Choose Trim Color", onconfirm_trim_colour_selection, onhighlight_trim_colour_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_trim_colour_menu(MenuItem<int> choice){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if(choice.value == -1){
		process_dash_colour_menu();
	}
	else if(choice.value == -2){
		process_trim_colour_menu();
	}

	return false;
}

bool process_interior_colour_menu(){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	if(!is_this_a_car(PED::GET_VEHICLE_PED_IS_USING(playerPed))){
		set_status_text("This vehicle is not supported");
		return false;
	}

	if(PAINTS_ALL.empty()){
		size_t totalSize = 0, paintTypes = array_size(PAINTS_BY_TYPE);
		for(int a = 1; a < paintTypes; a++){
			totalSize += PAINTS_BY_TYPE[a].size();
		}
		PAINTS_ALL.reserve(totalSize);
		for(int a = 1; a < paintTypes; a++){
			PAINTS_ALL.insert(PAINTS_ALL.end(), PAINTS_BY_TYPE[a].begin(), PAINTS_BY_TYPE[a].end());
		}
	}

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *chooseDashColor = new MenuItem<int>();
	chooseDashColor->caption = "Dash Color ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseDashColor->value = -1;
	chooseDashColor->isLeaf = false;
	menuItems.push_back(chooseDashColor);

	MenuItem<int> *chooseTrimColor = new MenuItem<int>();
	chooseTrimColor->caption = "Trim Color ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseTrimColor->value = -2;
	chooseTrimColor->isLeaf = false;
	menuItems.push_back(chooseTrimColor);

	return draw_generic_menu<int>(menuItems, 0, "Interior Colors", onconfirm_trim_colour_menu, NULL, NULL, vehicle_menu_interrupt);
}
