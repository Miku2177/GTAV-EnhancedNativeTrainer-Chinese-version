/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\ui_support\menu_functions.h"
#include "..\storage\database.h"

#include "..\ent-enums.h"
#include "..\utils.h"
#include "..\common\ENTUtil.h"

#include <string>
#include <random>

struct PaintColor{
	int colorIndex;
	std::string name;
};

extern const std::vector<PaintColor> PAINTS_BY_TYPE[7];

struct NeonLightsColor{
	std::string colorString;
	int rVal, gVal, bVal;
};

struct TireSmokeColor{
	std::string colorString;
	int rVal, gVal, bVal;
};

struct WheelSelection{
	int wheelIndex;
	std::string name;
};

extern const std::vector<std::string> VALUES_SUPERCARS;

/***
* METHODS
*/

void process_veh_menu();

void process_speed_menu();

bool process_savedveh_menu();

bool process_savedveh_sort_menu();

bool process_savedveh_slot_menu(int slot);

bool process_vehmod_menu();

bool process_carspawn_menu();

void update_vehicle_features(BOOL playerExists, Ped playerPed);

void reset_vehicle_globals();

bool process_spawn_menu_cars();

bool process_spawn_menu_indus();

void vehicle_flip();

void speedlimiter_switching();

void invincibility_switching();

void engineonoff_switching();

void engine_kill();

void process_window_roll();

void interior_light();

void search_light();

bool process_spawn_menu_generic(int topMenuSelection);

bool spawn_saved_car(int slot, std::string caption);

void save_current_vehicle(int slot);

bool onconfirm_spawn_menu_vehicle_selection(MenuItem<std::string> choice);

bool do_spawn_vehicle(std::string modelName, std::string modelTitle);

Vehicle do_spawn_vehicle(DWORD modelHash, std::string modelTitle, bool cleanup);

bool process_vehmod_wheel_selection_menu();

bool process_vehmod_wheel_selection();

//Paint menus

bool process_paint_menu();

bool process_paint_menu_type();

bool process_paint_menu_liveries();

bool onconfirm_color_menu_selection(MenuItem<int> choice);

void onhighlight_color_menu_selection(MenuItem<int> choice);

void apply_paint(PaintColor whichpaint);

//Vehicle mod getters and setters

bool is_custom_tyres(std::vector<int> extras);

void set_custom_tyres(bool applied, std::vector<int> extras);

bool is_turbocharged(std::vector<int> extras);

void set_turbocharged(bool applied, std::vector<int> extras);

bool is_bulletproof_tyres(std::vector<int> extras);

void set_bulletproof_tyres(bool applied, std::vector<int> extras);

bool is_xenon_headlights(std::vector<int> extras);

void set_xenon_headlights(bool applied, std::vector<int> extras);

bool is_extra_enabled(std::vector<int> extras);

void set_extra_enabled(bool applied, std::vector<int> extras);

void set_plate_text(MenuItem<int> choice);

bool is_convertible_roofdown(std::vector<int> extras);

void set_convertible_roofdown(bool applied, std::vector<int> extras);

bool is_bombbay_open(std::vector<int> extras);

void set_bombbay_open(bool applied, std::vector<int> extras);

int find_menu_index_to_restore(int category, int actualCategory, Vehicle veh);

bool vehicle_menu_interrupt();

bool vehicle_save_menu_interrupt();

bool vehicle_save_sort_menu_interrupt();

bool vehicle_save_slot_menu_interrupt();

void add_vehicle_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_vehicle_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_vehicle(std::vector<StringPairSettingDBRow>* settings);

bool onconfirm_paintfade(MenuItem<float> choice);

void onhighlight_paintfade(MenuItem<float> choice);

int get_current_veh_invincibility_mode();

void onchange_veh_invincibility_mode(int value, SelectFromListMenuItem* source);

void onchange_veh_speed_boost_index(int value, SelectFromListMenuItem *source);

int get_current_veh_eng_pow_index();

void onchange_veh_eng_pow_index(int value, SelectFromListMenuItem* source);

void onchange_veh_mass_index(int value, SelectFromListMenuItem* source);

void onchange_speed_size_index(int value, SelectFromListMenuItem* source);

void onchange_speed_position_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_blips_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_background_opacity_index(int value, SelectFromListMenuItem* source);

void onchange_veh_remember_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipsize_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipcolour_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipsymbol_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipflash_index(int value, SelectFromListMenuItem* source);

void onchange_car_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_bike_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_boat_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_plane_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_heli_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_refuelspeed_index(int value, SelectFromListMenuItem* source);

void onchange_fuelprice_index(int value, SelectFromListMenuItem* source);

void onchange_canprice_index(int value, SelectFromListMenuItem* source);

void onchange_random1_index(int value, SelectFromListMenuItem* source);

void onchange_random2_index(int value, SelectFromListMenuItem* source);

void onchange_barposition_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours_r_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours_g_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours_b_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours2_r_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours2_g_index(int value, SelectFromListMenuItem* source);

void onchange_speed_colours2_b_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_r_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_g_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_b_index(int value, SelectFromListMenuItem* source);

void onchange_veh_turn_signals_index(int value, SelectFromListMenuItem* source);

void onchange_veh_speedlimiter_index(int value, SelectFromListMenuItem* source);

void onchange_veh_lightsOff_index(int value, SelectFromListMenuItem* source);

void set_old_vehicle_state(bool updatedState);

MenuItemImage* vehicle_image_preview_finder(MenuItem<std::string> choice);

void init_vehicle_feature();

void fix_vehicle();

void clean_vehicle();

void apply_neon_colors(int colorIndex);

void onhighlight_neon_lights_selection(MenuItem<int> colorIndex);

bool onconfirm_neon_lights_selection(MenuItem<int> choice);

bool is_neonLights(std::vector<int> extras);

void set_neonLights(bool applied, std::vector<int> extras);

bool process_neon_lights_menu();

//Smoke related code

void apply_smoke_colors(int colorIndex);

void onhighlight_smoke_selection(MenuItem<int> choice);

bool onconfirm_smoke_selection(MenuItem<int> choice);

void set_smoke(bool applied, std::vector<int> extras);

bool process_smoke_colour_menu();

//End of smoke related code

// Vehicle Interior Colours

void apply_trim_colors(int colorIndex);

void onhighlight_trim_colour_selection(MenuItem<int> choice);

bool onconfirm_trim_colour_selection(MenuItem<int> choice);

bool process_trim_colour_menu();

void apply_dash_colors(int colorIndex);

void onhighlight_dash_colour_selection(MenuItem<int> choice);

bool onconfirm_dash_colour_selection(MenuItem<int> choice);

bool process_dash_colour_menu();

bool process_interior_colour_menu();

void drive_passenger();

bool inline is_this_a_car(Vehicle veh){
	// Return true if the current vehicle is a car, e.g. as certain vehicles don't support neon lights
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_CAR(et) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(et);
}

bool inline is_this_a_motorcycle(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return (BOOL) VEHICLE::IS_THIS_MODEL_A_BIKE(et);
}

bool inline is_this_a_heli_or_plane(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et);
}

bool inline is_this_a_bicycle(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_BICYCLE(et);
}

bool inline is_this_a_boat_or_sub(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_BOAT(et) || VEHICLE::_IS_THIS_MODEL_A_SUBMERSIBLE(et);
}

bool inline is_this_a_train(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_TRAIN(et);
}

bool did_player_just_enter_vehicle(Ped playerPed);

void fully_tune_vehicle(Vehicle veh, bool optics = true);

void reset_vehicle(Vehicle veh);

bool onconfirm_seat_menu(MenuItem<int> choice);

bool process_veh_seat_menu();

/* From Zorg93's "EnableMPCars" code */
#pragma once
#include <cstddef>

struct ScriptHeader
{
	char padding1[16];					//0x0
	unsigned char** codeBlocksOffset;	//0x10
	char padding2[4];					//0x18
	int codeLength;						//0x1C
	char padding3[4];					//0x20
	int localCount;						//0x24
	char padding4[4];					//0x28
	int nativeCount;					//0x2C
	__int64* localOffset;				//0x30
	char padding5[8];					//0x38
	__int64* nativeOffset;				//0x40
	char padding6[16];					//0x48
	int nameHash;						//0x58
	char padding7[4];					//0x5C
	char* name;							//0x60
	char** stringsOffset;				//0x68
	int stringSize;						//0x70
	char padding8[12];					//0x74
	//END_OF_HEADER

	bool IsValid() const { return codeLength > 0; }
	int CodePageCount() const { return (codeLength + 0x3FFF) >> 14; } //Exception = codeLength is nullPtr
	int GetCodePageSize(int page) const
	{
		return (page < 0 || page >= CodePageCount() ? 0 : (page == CodePageCount() - 1) ? codeLength & 0x3FFF : 0x4000);
	}
	unsigned char* GetCodePageAddress(int page) const{ return codeBlocksOffset[page]; }
	unsigned char* GetCodePositionAddress(int codePosition) const
	{
		return codePosition < 0 || codePosition >= codeLength ? NULL : &codeBlocksOffset[codePosition >> 14][codePosition & 0x3FFF];
	}
	char* GetString(int stringPosition)const
	{
		return stringPosition < 0 || stringPosition >= stringSize ? NULL : &stringsOffset[stringPosition >> 14][stringPosition & 0x3FFF];
	}

};
struct ScriptTableItem
{
	ScriptHeader* Header;
	char padding[4];
	int hash;

	inline bool IsLoaded() const
	{
		return Header != NULL;
	}
};

struct ScriptTable
{
	ScriptTableItem* TablePtr;
	char padding[16];
	int count;
	ScriptTableItem* FindScript(int hash)
	{
		if (TablePtr == NULL)
		{
			return NULL;//table initialisation hasnt happened yet
		}
		for (int i = 0; i<count;i++)
		{
			if (TablePtr[i].hash == hash)
			{
				return &TablePtr[i];
			}
		}
		return NULL;
	}
};

struct GlobalTable
{
	__int64** GlobalBasePtr;
	__int64* AddressOf(int index) const{ return &GlobalBasePtr[index >> 18 & 0x3F][index & 0x3FFFF];}
	bool IsInitialised()const{ return *GlobalBasePtr != NULL; }
};