/*
Part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include <vector>
#include "..\storage\\database.h"

struct HOTKEY_DEF
{
	std::string caption;
	int id;
};

bool is_hotkey_held_slow_mo();

bool is_hotkey_held_half_normal_speed();

bool is_hotkey_held_normal_speed();

bool is_hotkey_toggled_speed();

bool is_hotkey_held_veh_burnout();

bool is_hotkey_held_veh_extrapower();

static const int HKEY_FIX_CAR = 101;
static const int HKEY_CLEAN_CAR = 102;
static const int HKEY_HEAL_PLAYER = 103;
static const int HKEY_RESET_WANTED = 104;
static const int HKEY_REPLAY_ANIM = 105;
static const int HKEY_TOGGLE_INVISIBILITY = 106;
static const int HKEY_FILL_AMMO = 107;
static const int HKEY_TELEPORT_TO_MARKER = 108;
static const int HKEY_TELEPORT_TO_MISSION_MARKER = 109;
static const int HKEY_TELEPORT_TO_VEHICLE_AS_PASSENGER = 111;
static const int HKEY_MENU_VEHICLE_SPAWN = 112;
static const int HKEY_MENU_VEHICLE_PAINT = 113;
static const int HKEY_MENU_SKINS = 114;
static const int HKEY_TELEPORT_TO_VEHICLE = 115;
static const int HKEY_MENU_NEXT_RADIO_TRACK = 116;
static const int HKEY_TOGGLE_THERMAL_VIS = 117;
static const int HKEY_TOGGLE_NIGHT_VIS = 118;
static const int HKEY_PASSENGER = 119;
static const int HKEY_SLOW_MOTION = 120;
static const int HKEY_HALF_NORMAL_SPEED = 121;
static const int HKEY_NORMAL_SPEED = 122;
static const int HKEY_VEHICLE_POWER = 123;
static const int HKEY_VEHICLE_BURNOUT = 124;
static const int HKEY_KILL_NEARBY_PEDS = 125;
static const int HKEY_EXPLODE_NEARBY_VEHS = 126;
static const int HKEY_VEHICLE_FLIP = 127;
static const int HKEY_SPEEDLIMITER_SWITCHING = 128;
static const int HKEY_INVINCIBILITY_SWITCHING = 129;
static const int HKEY_ENGINEONOFF_SWITCHING = 130;
static const int HKEY_ENGINE_KILL = 131;
static const int HKEY_WINDOW_ROLL = 132;
static const int HKEY_INTERIOR_LIGHT = 133;
static const int HKEY_SEARCH_LIGHT = 134;
static const int HKEY_GIVE_ALL_WEAPONS = 135;
static const int HKEY_SNIPER_VISION_TOGGLE = 136;
static const int HKEY_DOORS_LOCKED_TOGGLE = 137;
static const int HKEY_SEAT_CHANGE_TOGGLE = 138;
static const int HKEY_TOGGLED_SPEED = 139;

const std::vector<HOTKEY_DEF> HOTKEY_AVAILABLE_FUNCS{
	{ "No Function", 0 },
	{ "Fix Car", HKEY_FIX_CAR },
	{ "Clean Car", HKEY_CLEAN_CAR },
	{ "Heal Player", HKEY_HEAL_PLAYER },
	{ "Reset Wanted", HKEY_RESET_WANTED },
	{ "Replay Last Anim", HKEY_REPLAY_ANIM },
	{ "Teleport To Marker", HKEY_TELEPORT_TO_MARKER },
	{ "Teleport To Mission Marker", HKEY_TELEPORT_TO_MISSION_MARKER },
	{ "Teleport To Vehicle As Passenger", HKEY_TELEPORT_TO_VEHICLE_AS_PASSENGER },
	{ "Teleport To Last Veh", HKEY_TELEPORT_TO_VEHICLE },
	{ "Toggle Invisibility", HKEY_TOGGLE_INVISIBILITY },
	{ "Fill Ammo", HKEY_FILL_AMMO },
	{ "Veh. Spawn Menu", HKEY_MENU_VEHICLE_SPAWN },
	{ "Veh. Paint Menu", HKEY_MENU_VEHICLE_PAINT },
	{ "Skin Select Menu", HKEY_MENU_SKINS },
	{ "Next Radio Track", HKEY_MENU_NEXT_RADIO_TRACK },
	{ "Toggle Night Vision", HKEY_TOGGLE_NIGHT_VIS },
	{ "Toggle Thermal Vision", HKEY_TOGGLE_THERMAL_VIS },
	{ "Slow Motion (Hold Key)", HKEY_SLOW_MOTION},
	{ "Half Normal Motion (Hold Key)", HKEY_HALF_NORMAL_SPEED },
	{ "Normal Motion (Hold Key)", HKEY_NORMAL_SPEED },
	{ "Vehicle Extra Power (Hold Key)", HKEY_VEHICLE_POWER },
	{ "Vehicle Burnout (Hold Key)", HKEY_VEHICLE_BURNOUT },
	{ "Kill All Nearby Peds", HKEY_KILL_NEARBY_PEDS },
	{ "Explode All Vehicles", HKEY_EXPLODE_NEARBY_VEHS },
	{ "Flip A Vehicle", HKEY_VEHICLE_FLIP },
	{ "Speed Limiter", HKEY_SPEEDLIMITER_SWITCHING },
	{ "Toggle Invincibility", HKEY_INVINCIBILITY_SWITCHING },
	{ "Toggle Engine ON/OFF", HKEY_ENGINEONOFF_SWITCHING },
	{ "Kill Engine", HKEY_ENGINE_KILL },
	{ "Driver Window Roll", HKEY_WINDOW_ROLL },
	{ "Interior Light", HKEY_INTERIOR_LIGHT },
	{ "Search Light", HKEY_SEARCH_LIGHT },
	{ "Give All Weapons", HKEY_GIVE_ALL_WEAPONS },
	{ "Toggle Vision For Sniper Rifles", HKEY_SNIPER_VISION_TOGGLE },
	{ "Doors Locked", HKEY_DOORS_LOCKED_TOGGLE },
	{ "Change Seat", HKEY_SEAT_CHANGE_TOGGLE },
	{ "Toggle Global Game Speed", HKEY_TOGGLED_SPEED },
};

void change_hotkey_function(int hotkey, int funcIndex);

int get_hotkey_function_index(int hotkey);

void check_for_hotkey_presses();

void add_hotkey_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_hotkey(std::vector<StringPairSettingDBRow>* settings);

void trigger_function_for_hotkey_onkeyup(int i);

void trigger_function_for_hotkey_onkeydown(int i);