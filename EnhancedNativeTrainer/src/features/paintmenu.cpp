/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

int whichpart = 0;
int whichtype = 0;

//Parts
const std::vector<std::string> MENU_PAINT_WHAT{"Primary", "Secondary", "Primary & Secondary", "Pearlescent", "Wheels"};

//Paint Names
const std::vector<std::string> MENU_PAINT_TYPE{"Classic", "Metallic", "Matte", "Metals", "Util", "Worn", "Chrome"};

const std::vector<PaintColor> PAINTS_CLASSIC{
	{"Black", 0},
	{"Graphite", 1},
	{"Black Steel", 2},
	{"Dark Steel", 3},
	{"Silver", 4},
	{"Bluish Silver", 5},
	{"Rolled Steel", 6},
	{"Shadow Silver", 7},
	{"Stone Silver", 8},
	{"Midnight Silver", 9},
	{"Cast Iron Silver", 10},
	{"Anthracite Black", 11},
	{"Red", 27},
	{"Torino Red", 28},
	{"Formula Red", 29},
	{"Blaze Red", 30},
	{"Grace Red", 31},
	{"Garnet Red", 32},
	{"Sunset Red", 33},
	{"Cabernet Red", 34},
	{"Candy Red", 35},
	{"Sunrise Orange", 36},
	{"Gold", 37},
	{"Orange", 38},
	{"Dark Green", 49},
	{"Racing Green", 50},
	{"Sea Green", 51},
	{"Olive Green", 52},
	{"Bright Green", 53},
	{"Gasoline Green", 54},
	{"Galaxy Blue", 61},
	{"Dark Blue", 62},
	{"Saxon Blue", 63},
	{"Blue", 64},
	{"Mariner Blue", 65},
	{"Harbor Blue", 66},
	{"Diamond Blue", 67},
	{"Surf Blue", 68},
	{"Nautical Blue", 69},
	{"Ultra Blue", 70},
	{"Schafter Purple", 71},
	{"Spinnaker Purple", 72},
	{"Racing Blue", 73},
	{"Light Blue", 74},
	{"Yellow", 88},
	{"Race Yellow", 89},
	{"Bronze", 90},
	{"Dew Yellow", 91},
	{"Lime Green", 92},
	{"Champagne", 93},
	{"Feltzer Brown", 94},
	{"Creek Brown", 95},
	{"Chocolate Brown", 96},
	{"Maple Brown", 97},
	{"Saddle Brown", 98},
	{"Straw Brown", 99},
	{"Moss Brown", 100},
	{"Bison Brown", 101},
	{"Woodbeech Brown", 102},
	{"Beechwood Brown", 103},
	{"Sienna Brown", 104},
	{"Sandy Brown", 105},
	{"Bleached Brown", 106},
	{"Cream", 107},
	{"Ice White", 111},
	{"Frost White", 112},
	{"Securicor Green", 125},
	{"Police Car Blue", 127},
	{"Pure White", 134},
	{"Hot Pink", 135},
	{"Salmon Pink", 136},
	{"Pfister Pink", 137},
	{"Bright Orange", 138},
	{"Green", 139},
	{"Fluorescent Blue", 140},
	{"Midnight Blue", 141},
	{"Black Purple", 142},
	{"Wine Red", 143},
	{"Hunter Green", 144},
	{"Bright Purple", 145},
	{"Midnight Purple", 146},
	{"Carbon Black", 147},
	{"Lava Red", 150},
	{"Alloy", 156},
	{"Epsilon Blue", 157}
};

const std::vector<PaintColor> PAINTS_MATTE{
	{"Matte Black", 12},
	{"Matte Gray", 13},
	{"Matte Light Gray", 14},
	{"Matte Red", 39},
	{"Matte Dark Red", 40},
	{"Matte Orange", 41},
	{"Matte Yellow", 42},
	{"Matte Lime Green", 55},
	{"Matte Dark Blue", 82},
	{"Matte Blue", 83},
	{"Matte Midnight Blue", 84},
	{"Matte Green", 128},
	{"Matte Brown", 129},
	{"Matte Ice White", 131},
	{"Matte Schafter Purple", 148},
	{"Matte Midnight Purple", 149},
	{"Matte Forest Green", 151},
	{"Matte Olive Drab", 152},
	{"Matte Dark Earth", 153},
	{"Matte Desert Tan", 154},
	{"Matte Foliage Green", 155}
};

const std::vector<PaintColor> PAINTS_METALS{
	{"Brushed Steel", 117},
	{"Brushed Black Steel", 118},
	{"Brushed Aluminium", 119},
	{"Pure Gold", 158},
	{"Brushed Gold", 159},
	{"Polished Gold", 160}
};

const std::vector<PaintColor> PAINTS_UTIL{
	{"Util Black", 15},
	{"Util Black Poly", 16},
	{"Util Dark Silver", 17},
	{"Util Silver", 18},
	{"Util Gun Metal", 19},
	{"Util Shadow Silver", 20},
	{"Util Red", 43},
	{"Util Bright Red", 44},
	{"Util Garnet Red", 45},
	{"Util Dark Green", 56},
	{"Util Green", 57},
	{"Util Dark Blue", 75},
	{"Util Midnight Blue", 76},
	{"Util Blue", 77},
	{"Util Sea Foam Blue", 78},
	{"Util Lightning Blue", 79},
	{"Util Maui Blue Poly", 80},
	{"Util Bright Blue", 81},
	{"Util Brown", 108},
	{"Util Medium Brown", 109},
	{"Util Light Brown", 110},
	{"Util Off-White", 122}
};

const std::vector<PaintColor> PAINTS_WORN{
	{"Worn Black", 21},
	{"Worn Graphite", 22},
	{"Worn Silver Gray", 23},
	{"Worn Silver", 24},
	{"Worn Blue Silver", 25},
	{"Worn Shadow Silver", 26},
	{"Worn Red", 46},
	{"Worn Golden Red", 47},
	{"Worn Dark Red", 48},
	{"Worn Dark Green", 58},
	{"Worn Green", 59},
	{"Worn Sea Wash", 60},
	{"Worn Dark Blue", 85},
	{"Worn Blue", 86},
	{"Worn Light Blue", 87},
	{"Worn Honey Beige", 113},
	{"Worn Brown", 114},
	{"Worn Dark Brown", 115},
	{"Worn Straw Beige", 116},
	{"Worn Off-White", 121},
	{"Worn Orange", 123},
	{"Worn Light Orange", 124},
	{"Worn Taxi Yellow", 126},
	{"Worn Pale Orange", 130},
	{"Worn White", 132},
	{"Worn Olive Army Green", 133}
};

const std::vector<PaintColor> PAINTS_CHROME{
	{"Chrome", 120}
};

const std::vector<PaintColor> PAINTS_BY_TYPE[7]{
	PAINTS_CLASSIC,
	PAINTS_CLASSIC,
	PAINTS_MATTE,
	PAINTS_METALS,
	PAINTS_UTIL,
	PAINTS_WORN,
	PAINTS_CHROME
};

bool onconfirm_paintdirt(MenuItem<float> choice){
	return true;
}

void onhighlight_paintdirt(MenuItem<float> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, choice.value);
}

bool onconfirm_paintfade(MenuItem<float> choice){
	return true;
}

void onhighlight_paintfade(MenuItem<float> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, choice.value);
}

bool process_paint_menu_fades(){
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	set_status_text("Paint fade is only applicable to certain vehicles");

	std::vector<MenuItem<float>*> menuItems;

	MenuItem<float> *item = new MenuItem<float>();
	item->caption = "Fresh Paint";
	item->value = 0.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	for(int i = 20; i < 100; i += 20){
		MenuItem<float> *item = new MenuItem<float>();
		std::ostringstream ss;
		ss << i << "% Faded";
		item->caption = ss.str();
		item->value = (float) i / 100;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	item = new MenuItem<float>();
	item->caption = "Fully Faded";
	item->value = 1.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<float>(menuItems, 0, "Paint Fade", onconfirm_paintfade, onhighlight_paintfade, NULL, vehicle_menu_interrupt);
}

bool process_paint_menu_dirt(){
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	std::vector<MenuItem<float>*> menuItems;

	MenuItem<float> *item = new MenuItem<float>();
	item->caption = "Clean";
	item->value = 0.0f;
	item->isLeaf = true;
	menuItems.push_back(item);

	for(int i = 20; i < 100; i += 20){
		MenuItem<float> *item = new MenuItem<float>();
		std::ostringstream ss;
		ss << i << "% Dirty";
		item->caption = ss.str();
		item->value = ((float) i / 100) * 15.0f;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	item = new MenuItem<float>();
	item->caption = "Fully Dirty";
	item->value = 14.89999f;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<float>(menuItems, 0, "Dirt Level", onconfirm_paintdirt, onhighlight_paintdirt, NULL, vehicle_menu_interrupt);
}

void onhighlight_livery(MenuItem<int> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	VEHICLE::SET_VEHICLE_LIVERY(veh, choice.value);

	//VEHICLE::SET_VEHICLE_MOD(veh, 48, choice.value, 0); //vehicle, modType (48 liv), mod index, bool customTires)
}

bool onconfirm_livery(MenuItem<int> choice){
	return true;
}

bool process_paint_menu_liveries(){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	int count = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);
	//int livCount = VEHICLE::GET_NUM_VEHICLE_MODS(veh, 48); //48 is the livery index

	/*if (count <= -2) //for some reason, one of the liveries for the halloween car is -1
	{
		set_status_text("No liveries for this vehicle");
	}*/

	std::vector<MenuItem<int>*> menuItems;

	for(int i = 0; i < count; i++) //livcount
	{
		std::string modItemNameStr;

		char* modItemNameChr = VEHICLE::GET_LIVERY_NAME(veh, i);
		if(modItemNameChr == NULL){
			std::ostringstream ss;
			ss << "Livery #" << (i + 1);
			modItemNameStr = ss.str();
		}
		else{
			char* modItemNameTxt = UI::_GET_LABEL_TEXT(modItemNameChr);
			if(modItemNameTxt != NULL){
				modItemNameStr = std::string(modItemNameTxt);
			}
			else{
				std::ostringstream ss;
				ss << "Livery #" << (i + 1);
				modItemNameStr = ss.str();
			}
		}

		MenuItem<int> *item = new MenuItem<int>();
		item->caption = modItemNameStr;
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	int currentSelection = VEHICLE::GET_VEHICLE_LIVERY(veh);
	//int currentSelection = VEHICLE::GET_VEHICLE_MOD(veh, 48);
	return draw_generic_menu<int>(menuItems, &currentSelection, "Liveries", onconfirm_livery, onhighlight_livery, NULL, vehicle_menu_interrupt);
}

bool onconfirm_paint_menu(MenuItem<int> choice){
	whichpart = choice.value;
	if(whichpart >= 0 && whichpart < 5){
		process_paint_menu_type();
	}
	else if(whichpart == 5){
		process_paint_menu_dirt();
	}
	else if(whichpart == 6){
		process_paint_menu_fades();
	}
	else if(whichpart == 7){
		process_paint_menu_liveries();
	}

	return false;
}

bool process_paint_menu(){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("Player isn't in a vehicle");
		return false;
	}

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int livCount = VEHICLE::GET_VEHICLE_LIVERY_COUNT(veh);

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int index;

	for(index = 0; index < MENU_PAINT_WHAT.size(); index++){
		item = new MenuItem<int>();
		item->caption = MENU_PAINT_WHAT[index];
		item->value = index;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "Dirt Level";
	item->value = index++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Paint Fade";
	item->value = index++;
	item->isLeaf = false;
	menuItems.push_back(item);

	if(livCount > 1){
		std::ostringstream ss;
		ss << "Liveries (" << livCount << ")";
		item = new MenuItem<int>();
		item->caption = ss.str();
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Choose which part to paint", onconfirm_paint_menu, NULL, NULL, vehicle_menu_interrupt);
}

bool onconfirm_paint_menu_type(MenuItem<int> choice){
	std::string category = choice.caption;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	whichtype = choice.value;

	if(whichpart == 4 && VEHICLE::GET_VEHICLE_MOD(veh, 23) == -1){
		set_status_text("You can't repaint the default wheels");
		return false;
	}

	std::vector<PaintColor> paints;
	if(whichtype == 6){
		apply_paint(PAINTS_BY_TYPE[whichtype].at(0));
		return false;
	}
	else{
		paints = PAINTS_BY_TYPE[whichtype];
	}

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;

	int primary, secondary, pearl, wheel;
	VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);

	int matchIndex = 0, ps = paints.size();

	for(int a = 0; a < ps; a++){
		item = new MenuItem<int>();
		item->caption = paints.at(a).name;
		item->value = a;
		menuItems.push_back(item);

		switch(whichpart){
			case 0:
			case 2:
				if(paints.at(a).colorIndex == primary){
					matchIndex = a;
				}
				break;
			case 1:
				if(paints.at(a).colorIndex == secondary){
					matchIndex = a;
				}
				break;
			case 3:
				if(paints.at(a).colorIndex == pearl){
					matchIndex = a;
				}
				break;
			case 4:
				if(paints.at(a).colorIndex == wheel){
					matchIndex = a;
				}
				break;
			default:
				break;
		}
	}

	draw_generic_menu<int>(menuItems, &matchIndex, category, onconfirm_color_menu_selection, onhighlight_color_menu_selection, NULL, vehicle_menu_interrupt);
	return false;
}

bool process_paint_menu_type(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int index;

	for(index = 0; index < MENU_PAINT_TYPE.size(); index++){
		item = new MenuItem<int>();
		item->caption = MENU_PAINT_TYPE[index];
		item->value = index;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, NULL, "Choose Paint Type", onconfirm_paint_menu_type, NULL, NULL, vehicle_menu_interrupt);
}

void onhighlight_color_menu_selection(MenuItem<int> choice){
	apply_paint(choice.value == -1 ? PAINTS_CHROME.at(0) : PAINTS_BY_TYPE[whichtype].at(choice.value));
}

bool onconfirm_color_menu_selection(MenuItem<int> choice){
	return true;
}

void apply_paint(PaintColor whichpaint){
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	if(bPlayerExists){
		if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			int primary, secondary, pearl, wheel, colorIndex = whichpaint.colorIndex;
			VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);

			switch(whichpart){
				case 0:
					VEHICLE::SET_VEHICLE_COLOURS(veh, colorIndex, secondary);
					break;
				case 1:
					VEHICLE::SET_VEHICLE_COLOURS(veh, primary, colorIndex);
					break;
				case 2:
					VEHICLE::SET_VEHICLE_COLOURS(veh, colorIndex, colorIndex);
					break;
				case 3:
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colorIndex, wheel);
					break;
				case 4:
					VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pearl, colorIndex);
					break;
				default:
					break;
			}

			if(whichtype == 1 && whichpart >= 0 && whichpart <= 2){
				VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colorIndex, wheel);
			}
		}
		else{
			set_status_text("Player isn't in a vehicle");
		}
	}
}
