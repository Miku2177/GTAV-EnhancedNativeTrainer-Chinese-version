/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

/*
	F4					activate
	NUM2/8/4/6			navigate thru the menus and lists (numlock must be on)
	NUM5 				select
	NUM0/BACKSPACE/F4 	back
	NUM9/3 				use vehicle boost when active
	NUM+ 				use vehicle rockets when active
*/

#pragma comment(lib, "Shlwapi.lib")

#include "script.h"
#include "hotkeys.h"
#include "propplacement.h"
#include "area_effect.h"

#include "../version.h"
#include "../utils.h"
#include "../ui_support/file_dialog.h"

#include <set>
#include <iostream>
#include <vector>
#include <psapi.h>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

bool AIMBOT_INCLUDED = false;

int last_player_slot_seen = 0;

int game_frame_num = 0;

bool everInitialised = false;

ENTDatabase* database = NULL;

bool onlineWarningShown = false;

//std::mutex db_mutex;

// features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featurePlayerIgnoredByPolice = false;
bool featurePlayerIgnoredByPoliceUpdated = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerNoNoiseUpdated = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastSwimUpdated = false;
bool featurePlayerFastRun = false;
bool featurePlayerFastRunUpdated = false;
bool featurePlayerSuperJump = false;
bool featurePlayerInvisible = false;
bool featurePlayerInvisibleUpdated = false;
bool featurePlayerInvisibleInVehicle = false;
bool featurePlayerInvisibleInVehicleUpdated = false;
bool featurePlayerDrunk = false;
bool featurePlayerDrunkUpdated = false;
bool featureNightVision = false;
bool featureNightVisionUpdated = false;
bool featureThermalVision = false;
bool featureThermalVisionUpdated = false;
bool featureWantedLevelFrozen = false;
bool featureWantedLevelFrozenUpdated = false;
bool engine_running = true;
bool engine_switched = false;
bool engine_killed = false;

bool featurePlayerLife = false;
bool featurePlayerLifeUpdated = true;
bool featurePlayerLife_Died = false;
bool featurePlayerLife_Changed = false;
bool featurePrison_Hardcore = false;
bool featurePrison_Robe = true;
bool featurePrison_Yard = true;

bool featureNoRagdoll = false;
bool featureNoRagdollUpdated = false;

Ped oldplayerPed = -1;
int tick, playerDataMenuIndex, playerPrisonMenuIndex = 0;
int death_time2 = -1;

// prison break variables
bool detained, in_prison = false;
int alert_level = 0;
int time_before_get_to_prison = -1;
int time_in_prison_tick = -1;
bool out_of_prison = true;
bool ExPrisonerDrunk = false;
int ExPrisonerDrunk_tick = -1;
int secs, mins, secs_difference = -1;
int seconds_to_show = 60;
char* seconds_to_show_char = "60";
char* minutes_to_show_char = "60";
char seconds_to_show_char_modifiable[2], minutes_to_show_char_modifiable[2];
bool clear_wanted_level = false;
int populate_tick = 0;
Hash JailGuard_Weapon2 = -1;
Ped prisonPed, temp_ped, temp_ped2 = -1;
std::vector<Ped> ADDITIONAL_PRISONERS;
int civilian_torso_drawable, civilian_torso_texture, civilian_legs_drawable, civilian_legs_texture = -1;
//

int  frozenWantedLevel = 0;

std::vector<Vehicle> VEHICLE_ENGINE;
std::vector<Vehicle> VEHICLE_KILLED;

// player model control, switching on normal ped model when needed

char* player_models[] = {"player_zero", "player_one", "player_two"};

char* mplayer_models[] = {"mp_f_freemode_01", "mp_m_freemode_01"};

const char* CLIPSET_DRUNK = "move_m@drunk@verydrunk";

const std::vector<std::string> GRAVITY_CAPTIONS{"Minimum", "0.1x", "0.5x", "0.75x", "1x (Normal)"};
const std::vector<float> GRAVITY_VALUES{0.0f, 0.1f, 0.5f, 0.75f, 1.0f};

const std::vector<std::string> REGEN_CAPTIONS{"Minimum", "0.1x", "0.25x", "0.5x", "1x (Normal)", "2x", "5x", "10x", "20x", "50x", "100x", "200x", "500x", "1000x"};
const std::vector<float> REGEN_VALUES{0.0f, 0.1f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f, 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f};
const int REGEN_DEFAULT = 4;
int regenIndex = REGEN_DEFAULT;

//Player Health
const std::vector<std::string> PLAYER_HEALTH_CAPTIONS{ "1", "10", "20", "30", "40", "50", "100", "200", "300", "500", "1000", "5000", "10000", "20000", "30000" };
const std::vector<double> PLAYER_HEALTH_VALUES{ 101, 110, 120, 130, 140, 150, 200, 300, 400, 600, 1100, 5100, 10100, 20100, 30100 };
int current_player_health = 2;
bool current_player_health_Changed = true;

//Player Armor
const std::vector<std::string> PLAYER_ARMOR_CAPTIONS{ "0", "15", "20", "30", "40", "50", "100" };
const std::vector<double> PLAYER_ARMOR_VALUES{ 0, 15, 20, 30, 40, 50, 100 };
int current_player_armor = 6;
bool current_player_armor_Changed = true;

//Player Goes To Prison
const std::vector<std::string> PLAYER_PRISON_CAPTIONS{ "OFF", "Died At 2 Stars Or More", "Died At 3 Stars Or More", "Died At 4 Stars Or More", "Died At 5 Stars" };
const std::vector<double> PLAYER_PRISON_VALUES{ 0, 1, 2, 3, 4 };
int current_player_prison = 0;
bool current_player_prison_Changed = true;

//Money For escape attempt
const std::vector<std::string> PLAYER_ESCAPEMONEY_CAPTIONS{ "OFF", "10 000$", "30 000$", "50 000$", "100 000$", "500 000$", "1 000 000$", "5 000 000$", "10 000 000$", "50 000 000$", "100 000 000$" };
const std::vector<double> PLAYER_ESCAPEMONEY_VALUES{ 0, 10000, 30000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 };
int current_player_escapemoney = 4;
bool current_player_escapemoney_Changed = true;

//Time before discharge
const std::vector<std::string> PLAYER_DISCHARGE_CAPTIONS{ "UNLIMITED", "10 MIN", "20 MIN", "30 MIN", "60 MIN", "90 MIN", "120 MIN" };
const std::vector<double> PLAYER_DISCHARGE_VALUES{ 0, 600, 1200, 1800, 3600, 5400, 7200 };
int current_player_discharge = 3;
bool current_player_discharge_Changed = true;

/* Prop unblocker related code - will need to clean up later*/

/*THIS causes ENT not to load when Menyoo is present!*/

void UnlockAllObjects()
{
	static auto checkModelBeforeCreation = FindPattern("\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x48\x50", "xxxxx????xxx");
	memset(reinterpret_cast<void*>(checkModelBeforeCreation), 0x90, 24);
}

/* End of prop unblocker code*/

void onchange_player_health_mode(int value, SelectFromListMenuItem* source){
	current_player_health = value;
	current_player_health_Changed = true;
}

void onchange_player_armor_mode(int value, SelectFromListMenuItem* source){
	current_player_armor = value;
	current_player_armor_Changed = true;
}

void onchange_player_prison_mode(int value, SelectFromListMenuItem* source){
	current_player_prison = value;
	current_player_prison_Changed = true;
}

void onchange_player_escapemoney_mode(int value, SelectFromListMenuItem* source){
	current_player_escapemoney = value;
	current_player_escapemoney_Changed = true;
}

void onchange_player_discharge_mode(int value, SelectFromListMenuItem* source){
	current_player_discharge = value;
	current_player_discharge_Changed = true;
}

void check_player_model(){
	/*
	std::stringstream ss;
	ss << "PID: " << *gp;
	set_status_text_centre_screen(ss.str());
	*/

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(playerPed)){
		return;
	}

	//find out whether we're a default player model
	bool found = false;
	Hash playerModel = ENTITY::GET_ENTITY_MODEL(playerPed);
	int playerSlot = 0;

	for each (char* model  in player_models){
		if(GAMEPLAY::GET_HASH_KEY(model) == playerModel){
			last_player_slot_seen = playerSlot;
			found = true;
			break;
		}
		playerSlot++;
	}

	if(!found && NETWORK::NETWORK_IS_GAME_IN_PROGRESS()){
		for each (char* model  in mplayer_models){
			if(GAMEPLAY::GET_HASH_KEY(model) == playerModel){
				last_player_slot_seen = playerSlot;
				found = true;
				break;
			}
			playerSlot++;
		}
	}

	if(ENTITY::IS_ENTITY_DEAD(playerPed) && is_player_reset_on_death()){
		if(!found){
			set_status_text("Resetting death state because a custom skin was used");
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();

			int spPlayerCount = sizeof(player_models) / sizeof(player_models[0]);
			if(last_player_slot_seen < spPlayerCount){
				applyChosenSkin(player_models[last_player_slot_seen]);
			}
			else{
				applyChosenSkin(mplayer_models[last_player_slot_seen - spPlayerCount]);
			}
		}

		// wait until player is resurrected
		while(ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())){
			WAIT(0);
		}
	}
}

void invincibility_switching(){
	featurePlayerInvincible = !featurePlayerInvincible;
	featurePlayerInvincibleUpdated = true;
	if (featurePlayerInvincible) set_status_text("Invincibility ON");
	else set_status_text("Invincibility OFF");
	WAIT(100);
}

void engineonoff_switching() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (!VEHICLE_ENGINE.empty() && VEHICLE_ENGINE[0] != veh) VEHICLE_ENGINE[0] = veh;
		if (VEHICLE_ENGINE.empty()) VEHICLE_ENGINE.push_back(veh);
		if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh)) engine_running = true;
		engine_running = !engine_running;
		engine_switched = true;
	}
	WAIT(100);
}

void engine_kill(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
		Vehicle veh2 = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (VEHICLE_KILLED.empty()) VEHICLE_KILLED.push_back(veh2);
		if (!VEHICLE_KILLED.empty()) VEHICLE_KILLED[0] = veh2;
		engine_killed = true;//!engine_killed;
		set_status_text("You have destroyed your vehicle's engine for some reason");
	}
}

// Updates all features that can be turned off by the game, being called each game frame
void update_features(){
	if(NETWORK::NETWORK_IS_GAME_IN_PROGRESS()){
		if(!onlineWarningShown){
			set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT ~HUD_COLOUR_WHITE~is not for use online");
			onlineWarningShown = true;
		}
		WAIT(0);
		return;
	}
	else{
		onlineWarningShown = false;
	}
	/*
	GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("shop_controller");

	std::ostringstream ss;
	ss << "Scripts disabled";
	set_status_text_centre_screen(ss.str());*/

	/*
	std::ostringstream perfSS;
	perfSS << "Calls this frame: " << get_calls_per_frame() << " in " << get_ticks_since_last_frame() << "ms";
	write_text_to_log_file(perfSS.str());
	*/

	/*int* gp = reinterpret_cast<int *>(getGlobalPtr(0x1801D9));
	*gp = 1;

	gp = reinterpret_cast<int *>(getGlobalPtr(0x187385 + 0x10F18));
	*gp = 1;

	gp = reinterpret_cast<int *>(getGlobalPtr(0x250FDB + 0xf158D));
	*gp = 0;

	int* gp = reinterpret_cast<int *>(getGlobalPtr(0x42CA + 0x9));
	*gp = 0xFF;
	*/

	everInitialised = true;
	game_frame_num++;
	if(game_frame_num >= 216000){
		game_frame_num = 0;
	}

	if(game_frame_num % 3600 == 0){
		DWORD myThreadID;
		HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
		CloseHandle(myHandle);
	}

	UpdateXInputControlState();

	check_for_hotkey_presses();

	if(is_menu_showing() || is_in_airbrake_mode() || is_in_prop_placement_mode()){
		if(is_input_blocked_in_menu() || is_in_airbrake_mode() || is_in_prop_placement_mode()){
			setGameInputToEnabled(false);
		}
		else{
			setGameInputToEnabled(true);
		}
	}
	else{
		setGameInputToEnabled(true);
	}

	if(is_in_airbrake_mode() || is_in_prop_placement_mode()){
		setAirbrakeRelatedInputToBlocked(true);
	}
	else{
		setAirbrakeRelatedInputToBlocked(false);
	}

	update_centre_screen_status_text();

	update_vehicle_guns();

	update_world_features();

	update_teleport_features();

	check_player_model();

	maintain_bodyguards();

	update_props_pending_dialogs();

	debug_native_investigation();

	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	update_area_effects(playerPed);
	update_vehicles(playerPed);

	// player invincible
	if(featurePlayerInvincibleUpdated){
		if(bPlayerExists && !featurePlayerInvincible){
			PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
		}
		WAIT(100);
		featurePlayerInvincibleUpdated = false;
		WAIT(100);
	}

	if(featurePlayerInvincible && bPlayerExists){
		PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
	}
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && engine_switched) {
		Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh) && !VEHICLE_ENGINE.empty() && VEHICLE_ENGINE[0] != veh) engine_running = true;
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, engine_running, false);
		if (!VEHICLE_ENGINE.empty() && VEHICLE_ENGINE[0] != veh)
		{
			VEHICLE_ENGINE[0] = veh;
			engine_running = false;
			engine_switched = false;
		}
	}
	
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && engine_killed) {
		Vehicle veh2 = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (!VEHICLE_KILLED.empty() && VEHICLE_KILLED[0] == veh2) {
			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh2, -4000);
			//VEHICLE::_SET_VEHICLE_HALT(veh2, 1000.0, 1, 1);
		}
	}

	if(featureWantedLevelFrozen){
		if(featureWantedLevelFrozenUpdated){
			frozenWantedLevel = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			featureWantedLevelFrozenUpdated = false;
		}
		PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
	}

	if(featureWantedLevelFrozenUpdated && !featureWantedLevelFrozen){
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		featureWantedLevelFrozenUpdated = false;
	}

	////////////////////////////////////// PLAYER DATA ////////////////////////////////////////////////
	
	Ped playerPed_Data = PLAYER::PLAYER_PED_ID();
	
	int playerHealth = ENTITY::GET_ENTITY_HEALTH(playerPed_Data);
	playerHealth = (playerHealth - 100);
	
	if ((bPlayerExists && featurePlayerLife && featurePlayerLifeUpdated) || (bPlayerExists && featurePlayerLife_Died && featurePlayerLifeUpdated) ||
		(bPlayerExists && featurePlayerLife_Changed && featurePlayerLifeUpdated)) {

		tick = tick + 1;
		if (tick > 200) {
			PED::SET_PED_MAX_HEALTH(playerPed_Data, PLAYER_HEALTH_VALUES[current_player_health]);
			ENTITY::SET_ENTITY_HEALTH(playerPed_Data, PLAYER_HEALTH_VALUES[current_player_health]);
			PLAYER::SET_PLAYER_MAX_ARMOUR(playerPed_Data, PLAYER_ARMOR_VALUES[current_player_armor]);
			PED::SET_PED_ARMOUR(playerPed_Data, PLAYER_ARMOR_VALUES[current_player_armor]);
			oldplayerPed = playerPed_Data;
			tick = 0;
			featurePlayerLifeUpdated = false;
		}
	}
	
	if (featurePlayerLife_Died) death_time2 = PLAYER::GET_TIME_SINCE_LAST_DEATH();
	if (death_time2 > -1 && death_time2 < 2000 && featurePlayerLife_Died)
	{
		featurePlayerLifeUpdated = true;
	}
	
	if (playerPed_Data != oldplayerPed && featurePlayerLife_Changed) { // If You Switch Character Your Health & Armor Will Be Restored
		featurePlayerLifeUpdated = true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////// PRISON BREAK //////////////////////////////////////////
	
	if (PLAYER_PRISON_VALUES[current_player_prison] > 0) {
		Ped playerPed_Prison = PLAYER::PLAYER_PED_ID();
		Vector3 my_position_in_prison = ENTITY::GET_ENTITY_COORDS(playerPed_Prison, true);
		time_before_get_to_prison = PLAYER::GET_TIME_SINCE_LAST_DEATH();

		const int arrSize3 = 1024;
		Ped guards[arrSize3];
		int count_prison_guards = worldGetAllPeds(guards, arrSize3);

		float distance_from_center_x, distance_from_center_y = -1;
		float distance_from_guard_x, distance_from_guard_y = -1;

		// Prison center coords
		float prison_x = 1689.69;
		float prison_y = 2589.49;
		float prison_z = 46;
		// Main characters
		const Hash PLAYER_ZERO = 0xD7114C9;
		const Hash PLAYER_ONE = 0x9B22DBAF;
		const Hash PLAYER_TWO = 0x9B810FA2;
		// Main characters cash
		const Hash SP0_TOTAL_CASH = 0x324C31D;
		const Hash SP1_TOTAL_CASH = 0x44BD6982;
		const Hash SP2_TOTAL_CASH = 0x8D75047D;

		// Got wanted level? You'd better not die
		if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > PLAYER_PRISON_VALUES[current_player_prison] && !in_prison) detained = true;
		// You won't be detained if you escape the police
		if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > -1 && current_player_prison < 4 && PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < PLAYER_PRISON_VALUES[current_player_prison + 1] &&
			time_before_get_to_prison > 6000) detained = false;

		// TRANSFERING YOU TO PRISON
		if ((detained && time_before_get_to_prison > 0 && time_before_get_to_prison < 6000 && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID())) ||
			(alert_level > 0 && time_before_get_to_prison > 0 && time_before_get_to_prison < 6000 && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID())))
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed_Prison, 1691.334, 2545.26, 46, 0, 0, 1);
			PLAYER::SET_MAX_WANTED_LEVEL(5);
			PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
			// Disarm you including armor
			WEAPON::REMOVE_ALL_PED_WEAPONS(playerPed_Prison, false);
			PED::SET_PED_ARMOUR(PLAYER::PLAYER_ID(), 0);
			
			if (out_of_prison == true)
			{
				if (PLAYER_DISCHARGE_VALUES[current_player_discharge] != 0) time_in_prison_tick = PLAYER_DISCHARGE_VALUES[current_player_discharge];
				else time_in_prison_tick = 10000;
				out_of_prison = false;
			}

			// Money for trasnfering you to prison / escape attempt
			// How much money have you got?
			int outValue_your_current_amount = -1;
			int statHash_your_purse = -1;
			// Michael
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO)
			{
				STATS::STAT_GET_INT(SP0_TOTAL_CASH, &outValue_your_current_amount, -1);
				statHash_your_purse = SP0_TOTAL_CASH;
				if (PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney] > 0) STATS::STAT_SET_INT(statHash_your_purse, outValue_your_current_amount - PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney], true);
			}
			// Franklin
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE)
			{
				STATS::STAT_GET_INT(SP1_TOTAL_CASH, &outValue_your_current_amount, -1);
				statHash_your_purse = SP1_TOTAL_CASH;
				if (PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney] > 0) STATS::STAT_SET_INT(statHash_your_purse, outValue_your_current_amount - PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney], true);
			}
			// Trevor
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO)
			{
				STATS::STAT_GET_INT(SP2_TOTAL_CASH, &outValue_your_current_amount, -1);
				statHash_your_purse = SP2_TOTAL_CASH;
				if (PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney] > 0) STATS::STAT_SET_INT(statHash_your_purse, outValue_your_current_amount - PLAYER_ESCAPEMONEY_VALUES[current_player_escapemoney], true);
			}

			// Character Prison Clothes
			// Michael
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO)
			{
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 4, 11, 4, 1);
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 3, 12, 4, 1);
			}
			// Franklin
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE)
			{
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 4, 1, 5, 1);
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 3, 1, 5, 1);
			}
			// Trevor
			if (ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO)
			{
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 4, 5, 2, 1);
				PED::SET_PED_COMPONENT_VARIATION(playerPed_Prison, 3, 5, 2, 1);
			}

			if (!ADDITIONAL_PRISONERS.empty()) {
				for (int i = 0; i < ADDITIONAL_PRISONERS.size(); i++) {
					PED::DELETE_PED(&ADDITIONAL_PRISONERS[i]);
				}
				ADDITIONAL_PRISONERS.clear();
				ADDITIONAL_PRISONERS.shrink_to_fit();
			}

			detained = false;
			in_prison = true;
			alert_level = 0;
			populate_tick = 0;
		}

		// IMPRISONED
		if (in_prison)
		{
			Hash JailGuard_Weapon1 = GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL");
			if (!featurePrison_Hardcore) JailGuard_Weapon2 = GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE");
			if (featurePrison_Hardcore) JailGuard_Weapon2 = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE");
			Hash JailGuard_Weapon3 = GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN");
			Vector3 guard_position_in_prison = ENTITY::GET_ENTITY_COORDS(guards[0], true);
			int randomize_jail = -1;

			// Populate the prison
			if (featurePrison_Yard)
			{
				if (populate_tick < 15) populate_tick = populate_tick + 1;
				if (populate_tick > 5 && populate_tick < 15) {
					int randomize_peds_in_jail_rot = (1 + rand() % 90);
					int randomize_peds_in_jail_x1 = (1 + rand() % 40);
					int randomize_peds_in_jail_y1 = (1 + rand() % 60);
					if (randomize_peds_in_jail_rot < 30) prisonPed = GAMEPLAY::GET_HASH_KEY("S_M_Y_PrisMuscl_01");
					if (randomize_peds_in_jail_rot > 30 && randomize_peds_in_jail_rot < 60) prisonPed = GAMEPLAY::GET_HASH_KEY("S_M_Y_Prisoner_01");
					if (randomize_peds_in_jail_rot > 60) prisonPed = GAMEPLAY::GET_HASH_KEY("U_M_Y_Prisoner_01");
					STREAMING::REQUEST_MODEL(prisonPed);
					while (!STREAMING::HAS_MODEL_LOADED(prisonPed)){
						make_periodic_feature_call();
						WAIT(0);
					}
					temp_ped = PED::CREATE_PED(25, prisonPed, randomize_peds_in_jail_x1 + 1701, randomize_peds_in_jail_y1 + 2484, my_position_in_prison.z, randomize_peds_in_jail_rot, 0, 0);
					temp_ped2 = PED::CREATE_PED(25, prisonPed, randomize_peds_in_jail_x1 + 1654, randomize_peds_in_jail_y1 + 2626, my_position_in_prison.z, randomize_peds_in_jail_rot, 0, 0);
					if (ADDITIONAL_PRISONERS.empty()) ADDITIONAL_PRISONERS.push_back(temp_ped);
					ADDITIONAL_PRISONERS.push_back(temp_ped);
					ADDITIONAL_PRISONERS.push_back(temp_ped2);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(temp_ped, 1, 1);
					AI::TASK_WANDER_STANDARD(temp_ped, 10.0f, 10);
					ENTITY::SET_ENTITY_AS_MISSION_ENTITY(temp_ped2, 1, 1);
					AI::TASK_WANDER_STANDARD(temp_ped2, 10.0f, 10);
				}
			}

			// Either main gates are closed or you're a sissy!
			if (featurePrison_Hardcore) {
				Hash prisonDoor = GAMEPLAY::GET_HASH_KEY("prop_gate_prison_01");
				OBJECT::_DOOR_CONTROL(prisonDoor, 1845.0, 2605.0, 45.0, 1, 0.0, 50.0, 0);
			}
			else
			{
				OBJECT::_DOOR_CONTROL(GAMEPLAY::GET_HASH_KEY("prop_gate_prison_01"), 1845.0, 2605.0, 45.0, 0, 0.0, 50.0, 0);
			}

			// No way you will call or switch your character
			CONTROLS::DISABLE_CONTROL_ACTION(2, 19, true);
			MOBILE::DESTROY_MOBILE_PHONE();

			// You don't need a parachute in prison
			if (WEAPON::HAS_PED_GOT_WEAPON(playerPed_Prison, PARACHUTE_ID, FALSE))	WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, PARACHUTE_ID);

			// How much time before discharge
			if (PLAYER_DISCHARGE_VALUES[current_player_discharge] > 0)
			{

				secs_difference = clock() / CLOCKS_PER_SEC;
				if (time_in_prison_tick > 0 && (((clock() / CLOCKS_PER_SEC) - secs) != 0))
				{
					time_in_prison_tick = time_in_prison_tick - 1;
					mins = time_in_prison_tick / 60;
					secs = secs_difference;
					seconds_to_show = seconds_to_show - 1;
					if (seconds_to_show == 0) seconds_to_show = 60;
					sprintf(seconds_to_show_char_modifiable, "%d", seconds_to_show);
					sprintf(minutes_to_show_char_modifiable, "%d", mins);
					if (seconds_to_show == 60) seconds_to_show_char = "00";
					if (seconds_to_show == 0) seconds_to_show_char = "00";
					if (seconds_to_show == 1) seconds_to_show_char = "01";
					if (seconds_to_show == 2) seconds_to_show_char = "02";
					if (seconds_to_show == 3) seconds_to_show_char = "03";
					if (seconds_to_show == 4) seconds_to_show_char = "04";
					if (seconds_to_show == 5) seconds_to_show_char = "05";
					if (seconds_to_show == 6) seconds_to_show_char = "06";
					if (seconds_to_show == 7) seconds_to_show_char = "07";
					if (seconds_to_show == 8) seconds_to_show_char = "08";
					if (seconds_to_show == 9) seconds_to_show_char = "09";
					if (mins == 0) minutes_to_show_char = "00";
					if (mins == 1) minutes_to_show_char = "01";
					if (mins == 2) minutes_to_show_char = "02";
					if (mins == 3) minutes_to_show_char = "03";
					if (mins == 4) minutes_to_show_char = "04";
					if (mins == 5) minutes_to_show_char = "05";
					if (mins == 6) minutes_to_show_char = "06";
					if (mins == 7) minutes_to_show_char = "07";
					if (mins == 8) minutes_to_show_char = "08";
					if (mins == 9) minutes_to_show_char = "09";
				}
				std::string temp_text = std::to_string(mins);
				UI::SET_TEXT_FONT(4);
				UI::SET_TEXT_SCALE(0.0, 0.45);
				UI::SET_TEXT_PROPORTIONAL(1);
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
				UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
				UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
				UI::SET_TEXT_OUTLINE();
				UI::_SET_TEXT_ENTRY("STRING");
				if (mins > 9) UI::_ADD_TEXT_COMPONENT_SCALEFORM(minutes_to_show_char_modifiable);
				else UI::_ADD_TEXT_COMPONENT_SCALEFORM(minutes_to_show_char);
				UI::_DRAW_TEXT(0.008, 0.65);
				GRAPHICS::DRAW_RECT(0.0, 0.665, 0.1, 0.05, 10, 10, 10, 25);

				UI::SET_TEXT_FONT(4);
				UI::SET_TEXT_SCALE(0.0, 0.45);
				UI::SET_TEXT_PROPORTIONAL(1);
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
				UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
				UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
				UI::SET_TEXT_OUTLINE();
				UI::_SET_TEXT_ENTRY("STRING");
				UI::_ADD_TEXT_COMPONENT_SCALEFORM(":");
				UI::_DRAW_TEXT(0.020, 0.65);
				GRAPHICS::DRAW_RECT(0.0, 0.665, 0.1, 0.05, 10, 10, 10, 25);

				UI::SET_TEXT_FONT(4);
				UI::SET_TEXT_SCALE(0.0, 0.45);
				UI::SET_TEXT_PROPORTIONAL(1);
				UI::SET_TEXT_COLOUR(255, 255, 255, 255);
				UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
				UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
				UI::SET_TEXT_OUTLINE();
				UI::_SET_TEXT_ENTRY("STRING");
				if (seconds_to_show > 9 && seconds_to_show < 60) UI::_ADD_TEXT_COMPONENT_SCALEFORM(seconds_to_show_char_modifiable);
				else UI::_ADD_TEXT_COMPONENT_SCALEFORM(seconds_to_show_char);
				UI::_DRAW_TEXT(0.025, 0.65);
			}

			// Let's change a level of alertness
			distance_from_center_x = my_position_in_prison.x - prison_x;
			distance_from_center_y = my_position_in_prison.y - prison_y;
			if (distance_from_center_x < 0) distance_from_center_x = distance_from_center_x * -1;
			if (distance_from_center_y < 0) distance_from_center_y = distance_from_center_y * -1;

			for (int i = 0; i < count_prison_guards; i++) {
				if ((ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(guards[i], playerPed_Prison, 1) || WEAPON::IS_PED_ARMED(playerPed_Prison, 7) || (my_position_in_prison.z - prison_z) > 8) && alert_level < 2 && time_in_prison_tick > 0) alert_level = 1;
				if (playerPed_Prison == PED::GET_PED_SOURCE_OF_DEATH(guards[i]) ||
					(ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(guards[i], playerPed_Prison, 1) && (PED::GET_PED_TYPE(guards[i]) == 27 || PED::GET_PED_TYPE(guards[i]) == 6))) alert_level = 2;
				if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(guards[i]) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(guards[i], playerPed_Prison, 1) &&
					alert_level < 3 && alert_level == 1 && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2) alert_level = 2; // In case you spawned bodyguards 
			}

			if ((distance_from_center_x > 110 || distance_from_center_y > 130 || PED::IS_PED_SHOOTING(playerPed_Prison)) && alert_level < 3 && time_in_prison_tick > 0) alert_level = 2;

			if ((my_position_in_prison.z - prison_z) > 55 || distance_from_center_x > 190 || distance_from_center_y > 200 || PED::IS_PED_IN_ANY_VEHICLE(playerPed_Prison, true)) alert_level = 3;

			// Alertness levels
			if (alert_level == 0) { // Behave yourself
				for (int i = 0; i < count_prison_guards; i++)
				{
					randomize_jail = (1 + rand() % 10);
					if (randomize_jail < 6) JailGuard_Weapon3 = GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN");
					else JailGuard_Weapon3 = GAMEPLAY::GET_HASH_KEY("WEAPON_NIGHTSTICK");
					if (WEAPON::IS_PED_ARMED(guards[i], 7) && WEAPON::IS_PED_ARMED(guards[i], 6) && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2 &&
						PED::GET_PED_TYPE(guards[i]) != 6 && PED::GET_PED_TYPE(guards[i]) != 27)
					{
						if (!featurePrison_Hardcore) {
							if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE") ||
								(WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER"))) WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon3, 999, false, true);
						}
						if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) != JailGuard_Weapon3)
						{
							WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon3, 999, false, true);
							AI::TASK_WANDER_STANDARD(guards[i], 10.0f, 10);
						}
					}
					guard_position_in_prison = ENTITY::GET_ENTITY_COORDS(guards[i], true);
					distance_from_guard_x = my_position_in_prison.x - guard_position_in_prison.x;
					distance_from_guard_y = my_position_in_prison.y - guard_position_in_prison.y;
					if (distance_from_guard_x < 0) distance_from_guard_x = distance_from_guard_x * -1;
					if (distance_from_guard_y < 0) distance_from_guard_y = distance_from_guard_y * -1;
					if (distance_from_guard_x < 55 && distance_from_guard_y < 55 && !WEAPON::IS_PED_ARMED(guards[i], 7) && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2)
					{
						std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());
						for each (Ped xped in peds){
							if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID())) && !WEAPON::IS_PED_ARMED(guards[i], 7) && PED::GET_PED_TYPE(guards[i]) != 0 &&
								PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2){
								PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
								PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
							}
						}
					}
				}
				PLAYER::SET_MAX_WANTED_LEVEL(0);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
				AUDIO::STOP_ALARM("PRISON_ALARMS", true);
			}

			if (alert_level == 1) { // Hit someone in the face / Withdraw a weapon / Got lost?!
				for (int i = 0; i < count_prison_guards; i++)
				{
					randomize_jail = (1 + rand() % 10);
					if (randomize_jail < 6) JailGuard_Weapon3 = GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN");
					else JailGuard_Weapon3 = GAMEPLAY::GET_HASH_KEY("WEAPON_NIGHTSTICK");
					guard_position_in_prison = ENTITY::GET_ENTITY_COORDS(guards[i], true);
					distance_from_guard_x = my_position_in_prison.x - guard_position_in_prison.x;
					distance_from_guard_y = my_position_in_prison.y - guard_position_in_prison.y;
					if (distance_from_guard_x < 0) distance_from_guard_x = distance_from_guard_x * -1;
					if (distance_from_guard_y < 0) distance_from_guard_y = distance_from_guard_y * -1;
					if (distance_from_guard_x < 25 && distance_from_guard_y < 25 && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2)
					{
						PED::SET_PED_AS_ENEMY(playerPed_Prison, true);
						PED::REGISTER_TARGET(guards[i], playerPed_Prison);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 5, true);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 46, true);
						AI::TASK_COMBAT_PED(guards[i], playerPed_Prison, 0, 16);
						PED::SET_PED_KEEP_TASK(guards[i], true);
					}
					if (WEAPON::IS_PED_ARMED(guards[i], 7) && WEAPON::IS_PED_ARMED(guards[i], 6) && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2 &&
						PED::GET_PED_TYPE(guards[i]) != 6 && PED::GET_PED_TYPE(guards[i]) != 27)
					{
						if (!featurePrison_Hardcore) {
							if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE") ||
								(WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER"))) WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon3, 999, false, true);
						}
						if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) != JailGuard_Weapon3)
						{
							WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon3, 999, false, true);
						}
					}
					if (ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ENTITY(playerPed_Prison, guards[i], 1) && WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_STUNGUN"))
					{
						float playerHealth_prison = ENTITY::GET_ENTITY_HEALTH(playerPed_Prison);
						srand(time(0));
						int randomize_stungun_damage = (rand() % 50 + 1);
						if (randomize_stungun_damage < 50) playerHealth_prison = playerHealth_prison - 0.0009;
						else playerHealth_prison = playerHealth_prison - 0.001;
						ENTITY::SET_ENTITY_HEALTH(playerPed_Prison, playerHealth_prison);
					}
				}
				PLAYER::SET_MAX_WANTED_LEVEL(1);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 1, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				AUDIO::STOP_ALARM("PRISON_ALARMS", true);
			}

			if (alert_level == 2) { // Trying to leave the courtyard. Being uppish?
				for (int i = 0; i < count_prison_guards; i++)
				{
					guard_position_in_prison = ENTITY::GET_ENTITY_COORDS(guards[i], true);
					distance_from_guard_x = my_position_in_prison.x - guard_position_in_prison.x;
					distance_from_guard_y = my_position_in_prison.y - guard_position_in_prison.y;
					if (distance_from_guard_x < 0) distance_from_guard_x = distance_from_guard_x * -1;
					if (distance_from_guard_y < 0) distance_from_guard_y = distance_from_guard_y * -1;
					if (distance_from_guard_x < 95 && distance_from_guard_y < 95 && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2)
					{
						PED::SET_PED_AS_ENEMY(playerPed_Prison, true);
						PED::REGISTER_TARGET(guards[i], playerPed_Prison);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 5, true);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 46, true);
						AI::TASK_COMBAT_PED(guards[i], playerPed_Prison, 0, 16);
						PED::SET_PED_KEEP_TASK(guards[i], true);
					}
					if (WEAPON::IS_PED_ARMED(guards[i], 7) && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2 &&
						PED::GET_PED_TYPE(guards[i]) != 6 && PED::GET_PED_TYPE(guards[i]) != 27)
					{
						if (!featurePrison_Hardcore) {
							if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE") ||
								(WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER")))
							{
								WEAPON::REMOVE_ALL_PED_WEAPONS(guards[i], true);
								WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon1, 999, false, true);
							}
						}
						if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) != JailGuard_Weapon1)
						{
							WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon1, 999, false, true);
						}
					}
				}
				PLAYER::SET_MAX_WANTED_LEVEL(2);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 2, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				AUDIO::STOP_ALARM("PRISON_ALARMS", false);
			}

			if (alert_level == 3) { // Escaping the prison. A death wish?
				for (int i = 0; i < count_prison_guards; i++)
				{
					guard_position_in_prison = ENTITY::GET_ENTITY_COORDS(guards[i], true);
					distance_from_guard_x = my_position_in_prison.x - guard_position_in_prison.x;
					distance_from_guard_y = my_position_in_prison.y - guard_position_in_prison.y;
					if (distance_from_guard_x < 0) distance_from_guard_x = distance_from_guard_x * -1;
					if (distance_from_guard_y < 0) distance_from_guard_y = distance_from_guard_y * -1;
					if (distance_from_guard_x < 155 && distance_from_guard_y < 155 && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2)
					{
						PED::SET_PED_AS_ENEMY(playerPed_Prison, true);
						PED::REGISTER_TARGET(guards[i], playerPed_Prison);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 5, true);
						PED::SET_PED_COMBAT_ATTRIBUTES(guards[i], 46, true);
						AI::TASK_COMBAT_PED(guards[i], playerPed_Prison, 0, 16);
						PED::SET_PED_KEEP_TASK(guards[i], true);
					}
					if (WEAPON::IS_PED_ARMED(guards[i], 7) && PED::GET_PED_TYPE(guards[i]) != 0 && PED::GET_PED_TYPE(guards[i]) != 1 && PED::GET_PED_TYPE(guards[i]) != 2 &&
						PED::GET_PED_TYPE(guards[i]) != 6 && PED::GET_PED_TYPE(guards[i]) != 27)
					{
						if (!featurePrison_Hardcore) {
							if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE") ||
								(WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) == GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER")))
							{
								WEAPON::REMOVE_ALL_PED_WEAPONS(guards[i], true);
								WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon2, 999, false, true);
							}
						}
						if (WEAPON::GET_SELECTED_PED_WEAPON(guards[i]) != JailGuard_Weapon2)
						{
							WEAPON::GIVE_WEAPON_TO_PED(guards[i], JailGuard_Weapon2, 999, false, true);
						}
					}
				}
				PLAYER::SET_MAX_WANTED_LEVEL(5);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 5, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				AUDIO::STOP_ALARM("PRISON_ALARMS", false);
				in_prison = false;
				alert_level = 0;
				CONTROLS::ENABLE_CONTROL_ACTION(2, 19, true);
				if (!ADDITIONAL_PRISONERS.empty()) {
					for (int i = 0; i < ADDITIONAL_PRISONERS.size(); i++) {
						PED::DELETE_PED(&ADDITIONAL_PRISONERS[i]);
					}
					ADDITIONAL_PRISONERS.clear();
					ADDITIONAL_PRISONERS.shrink_to_fit();
				}
			}

			// You served your time. Get lost. Let's hope we won't see you again
			if (time_in_prison_tick < 1)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed_Prison, 1990.48, 3047.6, 47.2151, 0, 0, 1);
				PLAYER::SET_MAX_WANTED_LEVEL(5);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
				AUDIO::STOP_ALARM("PRISON_ALARMS", false);
				PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
				alert_level = 0;
				CONTROLS::ENABLE_CONTROL_ACTION(2, 19, true);
				out_of_prison = true;
				in_prison = false;
				clear_wanted_level = true;
				ExPrisonerDrunk = true;
				ExPrisonerDrunk_tick = 0;
				if (!ADDITIONAL_PRISONERS.empty()) {
					for (int i = 0; i < ADDITIONAL_PRISONERS.size(); i++) {
						PED::DELETE_PED(&ADDITIONAL_PRISONERS[i]);
					}
					ADDITIONAL_PRISONERS.clear();
					ADDITIONAL_PRISONERS.shrink_to_fit();
				}
				// Let's give back civilian clothes. We'll see if you're lucky
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(playerPed_Prison, true);
			}
		} // END OF IMPRISONED

		if (clear_wanted_level == true && (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0))
		{
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
		}

		// You're found drunk in a bar 
		if (ExPrisonerDrunk && ExPrisonerDrunk_tick == 0){
			STREAMING::REQUEST_ANIM_SET("move_m@drunk@verydrunk");
			while (!STREAMING::HAS_ANIM_SET_LOADED("move_m@drunk@verydrunk")){
				WAIT(1);
			}
			PED::SET_PED_MOVEMENT_CLIPSET(playerPed_Prison, "move_m@drunk@verydrunk", 1.0f);
			CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 1.0f);
			AUDIO::SET_PED_IS_DRUNK(playerPed_Prison, true);
			ExPrisonerDrunk_tick = ExPrisonerDrunk_tick + 1;
		}
		
		if (ExPrisonerDrunk_tick > 0 && ExPrisonerDrunk_tick < 3000) ExPrisonerDrunk_tick = ExPrisonerDrunk_tick + 1;

		if (ExPrisonerDrunk_tick == 3000){
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed_Prison, 1.0f);
			CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
			ExPrisonerDrunk = false;
			ExPrisonerDrunk_tick = 0;
			ExPrisonerDrunk_tick = ExPrisonerDrunk_tick + 5;
			clear_wanted_level = false;
		}

		// YOU'D BETTER CHANGE AS SOON AS POSSIBLE. POLICE WILL NOTICE YOUR INTERESTING CLOTHES
		if (featurePrison_Robe && in_prison == false && (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 5))
		{
			for (int i = 0; i < count_prison_guards; i++)
			{
				Vector3 police_position = ENTITY::GET_ENTITY_COORDS(guards[i], true);
				int distance_from_police_x = my_position_in_prison.x - police_position.x;
				int distance_from_police_y = my_position_in_prison.y - police_position.y;
				int distance_from_police_z = my_position_in_prison.z - police_position.z;
				if (distance_from_police_x < 0) distance_from_police_x = distance_from_police_x * -1;
				if (distance_from_police_y < 0) distance_from_police_y = distance_from_police_y * -1;
				if (distance_from_police_z < 0) distance_from_police_z = distance_from_police_z * -1;

				// Your escape is not over yet so you still can't use mobile or switch character unless you change your clothes. And you don't need a parachute either
				if (((ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 12) ||
					(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 1) ||
					(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 5)))
				{
					CONTROLS::DISABLE_CONTROL_ACTION(2, 19, true);
					MOBILE::DESTROY_MOBILE_PHONE();
					if (WEAPON::HAS_PED_GOT_WEAPON(playerPed_Prison, PARACHUTE_ID, FALSE)) WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, PARACHUTE_ID);
				}

				// Met a cop at a distance
				if (distance_from_police_x < 100 && distance_from_police_y < 100 && distance_from_police_z < 10 && PED::IS_PED_FACING_PED(guards[i], playerPed_Prison, 100) &&
					(PED::GET_PED_TYPE(guards[i]) == 6 || PED::GET_PED_TYPE(guards[i]) == 27))
				{
					if (((ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 12) || 
						(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 1) || 
						(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 5)) &&
						(!PED::IS_PED_IN_ANY_VEHICLE(playerPed_Prison, false) || PED::IS_PED_ON_ANY_BIKE(playerPed_Prison)))
					{
						if ((PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 2)) {
							PLAYER::SET_MAX_WANTED_LEVEL(5);
							PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 1, 0);
							PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
						}
						// Met a cop right in front of you
						if (distance_from_police_x < 10 && distance_from_police_y < 10 && distance_from_police_z < 1 && (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 4))
						{
							PLAYER::SET_MAX_WANTED_LEVEL(5);
							PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 3, 0);
							PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
						}
					}
				}
				
				// Prisoner driver met a cop
				if (PED::IS_PED_IN_ANY_VEHICLE(playerPed_Prison, false) && !PED::IS_PED_ON_ANY_BIKE(playerPed_Prison))
				{
					if (((ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ZERO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 12) ||
						(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_ONE && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 1) ||
						(ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == PLAYER_TWO && PED::GET_PED_DRAWABLE_VARIATION(playerPed_Prison, 3) == 5)))
					{
						if ((distance_from_police_x < 4 && distance_from_police_y < 4 && distance_from_police_z < 1) && (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 4) && PED::IS_PED_FACING_PED(guards[i], playerPed_Prison, 100) &&
							(PED::GET_PED_TYPE(guards[i]) == 6 || PED::GET_PED_TYPE(guards[i]) == 27))
						{
							PLAYER::SET_MAX_WANTED_LEVEL(5);
							PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 3, 0);
							PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
						}
					}
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// police ignore player
	if(featurePlayerIgnoredByPolice){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
		}
	}
	else if(featurePlayerIgnoredByPoliceUpdated){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, false);
		}
		featurePlayerIgnoredByPoliceUpdated = false;
	}

	// player special ability
	if(featurePlayerUnlimitedAbility){
		if(bPlayerExists){
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
		}
	}

	// player no noise
	if(featurePlayerNoNoiseUpdated){
		if(bPlayerExists && !featurePlayerNoNoise){
			PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
		}
		featurePlayerNoNoiseUpdated = false;
	}
	if(featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);
	}

	// player fast swim
	if(featurePlayerFastSwimUpdated){
		if(bPlayerExists && !featurePlayerFastSwim){
			PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
		}
		featurePlayerFastSwimUpdated = false;
	}
	if(featurePlayerFastSwim){
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);
	}

	// player fast run
	if(featurePlayerFastRunUpdated){
		if(bPlayerExists && !featurePlayerFastRun){
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
		}
		featurePlayerFastRunUpdated = false;
	}
	if(featurePlayerFastRun){
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);
	}

	// player super jump
	if(featurePlayerSuperJump){
		if(bPlayerExists){
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
		}
	}

	//No Radgoll
	if(featureNoRagdoll){
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 0);
		}
	}
	else if(featureNoRagdollUpdated){
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 1);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 1);
		}
		featureNoRagdollUpdated = false;
	}

	//Player Invisible
	if(featurePlayerInvisibleUpdated){
		featurePlayerInvisibleUpdated = false;

		if(bPlayerExists && featurePlayerInvisible){
			ENTITY::SET_ENTITY_VISIBLE(playerPed, false);
		}
		else if(bPlayerExists){
			ENTITY::SET_ENTITY_VISIBLE(playerPed, true);
		}
	}

	//Player Invisible In Vehicle
	if (featurePlayerInvisibleInVehicle) {
		if (bPlayerExists && featurePlayerInvisibleInVehicle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1))
		ENTITY::SET_ENTITY_VISIBLE(playerPed, false);
		else if (bPlayerExists)	ENTITY::SET_ENTITY_VISIBLE(playerPed, true);
		featurePlayerInvisibleInVehicleUpdated = true;
	}
	
	if (!featurePlayerInvisibleInVehicle && featurePlayerInvisibleInVehicleUpdated) {
		if (bPlayerExists)	ENTITY::SET_ENTITY_VISIBLE(playerPed, true);
		featurePlayerInvisibleInVehicleUpdated = false;
	}

	if(featurePlayerDrunkUpdated){
		featurePlayerDrunkUpdated = false;
		if(featurePlayerDrunk){
			STREAMING::REQUEST_ANIM_SET((char*) CLIPSET_DRUNK);
			while(!STREAMING::HAS_ANIM_SET_LOADED((char*) CLIPSET_DRUNK)){
				make_periodic_feature_call();
				WAIT(0);
			}
			PED::SET_PED_MOVEMENT_CLIPSET(playerPed, (char*) CLIPSET_DRUNK, 1.0f);
			CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 1.0f);
		}
		else{
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			CAM::STOP_GAMEPLAY_CAM_SHAKING(true);
		}
		AUDIO::SET_PED_IS_DRUNK(playerPed, featurePlayerDrunk);
	}

	if(featureNightVisionUpdated){
		GRAPHICS::SET_NIGHTVISION(featureNightVision);
		featureNightVisionUpdated = false;
	}

	if(featureThermalVisionUpdated){
		GRAPHICS::SET_SEETHROUGH(featureThermalVision);
		featureThermalVisionUpdated = false;
	}

	update_weapon_features(bPlayerExists, player);

	if(AIMBOT_INCLUDED){
		update_aimbot_esp_features();
	}

	update_vehicle_features(bPlayerExists, playerPed);

	update_misc_features(bPlayerExists, playerPed);

	update_time_features(player);

	update_bodyguard_features();

	//Disable airbrake on death
	if(ENTITY::IS_ENTITY_DEAD(playerPed)){
		exit_airbrake_menu_if_showing();
	}

	//----Hotkeys----

	//Move through door (use '-key)
	//Pushes player through solid door objects.
	if(bPlayerExists){
		bool throughDoorPressed = IsKeyJustUp(KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR) || IsControllerButtonJustUp(KeyConfig::KEY_HOT_AIRBRAKE_THROUGH_DOOR);
		//bool disablePolicePressed = IsKeyJustUp(VK_OEM_6);
		if(throughDoorPressed){
			moveThroughDoor();
		}
	}
}

int activeLineIndexWantedFreeze = 0;

bool getFrozenWantedFeature(){
	return featureWantedLevelFrozen;
}

void updateFrozenWantedFeature(int level){
	frozenWantedLevel = level;
	featureWantedLevelFrozenUpdated = true;
}

void onchange_regen_callback(int index, SelectFromListMenuItem *source){
	if(ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())){
		PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), REGEN_VALUES.at(regenIndex = index));
		std::ostringstream ss;
		ss << "Health regeneration rate: " << REGEN_VALUES.at(regenIndex);
		set_status_text(ss.str());
	}
}

bool onconfirm_playerData_menu(MenuItem<int> choice){

	if (choice.value == -1){
		featurePlayerLifeUpdated = true; 
		set_status_text("Settings Are Being Applied");
	}
	return false;
}

bool onconfirm_playerPrison_menu(MenuItem<int> choice){

	return false;
}

bool process_player_life_menu(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Player Data";
	
	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;
	LifeItem<int> *item2;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Apply On Game Load";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerLife;
	toggleItem->toggleValueUpdated = &featurePlayerLifeUpdated;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Re-apply On Respawn";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerLife_Died;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Apply On Character Change";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerLife_Changed;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Apply Now";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(PLAYER_HEALTH_CAPTIONS, onchange_player_health_mode);
	listItem->wrap = false;
	listItem->caption = "Set Player Health";
	listItem->value = current_player_health;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_ARMOR_CAPTIONS, onchange_player_armor_mode);
	listItem->wrap = false;
	listItem->caption = "Set Player Armor";
	listItem->value = current_player_armor;
	menuItems.push_back(listItem);
	
	item2 = new LifeItem<int>();
	item2->caption = "Current Health";
	item2->value = 0;
	item2->life = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
	menuItems.push_back(item2);

	//item = new LifeItem<int>();
	//item->caption = "Maximum Health";
	//item->value = 1;
	//item->lifeType = MAXHEALTH;
	//item->minimum = 100;
	//item->life = PED::GET_PED_MAX_HEALTH(playerPed);
	//menuItems.push_back(item);

	item2 = new LifeItem<int>();
	item2->caption = "Current Armor";
	item2->value = 1;
	item2->life = PED::GET_PED_ARMOUR(playerPed);
	menuItems.push_back(item2);

	//item = new LifeItem<int>();
	//item->caption = "Maximum Armor";
	//item->value = 3;
	//item->lifeType = MAXARMOR;
	//item->minimum = 0;
	//item->life = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID());
	//menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(REGEN_CAPTIONS, onchange_regen_callback);
	listItem->caption = "Health Regeneration Rate";
	listItem->value = regenIndex;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &playerDataMenuIndex, caption, onconfirm_playerData_menu, NULL, NULL);
	//draw_generic_menu<int>(menuItems, nullptr, "Player Data", nullptr, nullptr, nullptr, nullptr);
}

bool process_player_prison_menu(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Prison Break Options";

	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;
	listItem = new SelectFromListMenuItem(PLAYER_PRISON_CAPTIONS, onchange_player_prison_mode);
	listItem->wrap = false;
	listItem->caption = "Player Imprisoned If";
	listItem->value = current_player_prison;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_ESCAPEMONEY_CAPTIONS, onchange_player_escapemoney_mode);
	listItem->wrap = false;
	listItem->caption = "Escape Failure Payment";
	listItem->value = current_player_escapemoney;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_DISCHARGE_CAPTIONS, onchange_player_discharge_mode);
	listItem->wrap = false;
	listItem->caption = "Time To Discharge";
	listItem->value = current_player_discharge;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Hardcore Mode";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Hardcore;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Cops React To Prison Clothes";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Robe;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "More Prisoners At The Yard";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Yard;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &playerPrisonMenuIndex, caption, onconfirm_playerPrison_menu, NULL, NULL);
}

int activeLineIndexPlayer = 0;

bool onconfirm_player_menu(MenuItem<int> choice){
	switch(activeLineIndexPlayer){
		case 0:
			if(process_skinchanger_menu())	return true;
			break;
		case 1:
			heal_player();
			break;
		case 18:
			process_anims_menu_top();
			break;
		case 19:
			process_player_life_menu();
			break;
		case 20:
			process_player_prison_menu();
			break;
		default:
			break;
	}

	return false;
}

void process_player_menu(){
	const int lineCount = 21;

	std::string caption = "Player Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Player Appearance", NULL, NULL, false},
		{"Heal Player", NULL, NULL, true},
		{"Add or Remove Cash", NULL, NULL, true, CASH},
		{"Wanted Level", NULL, NULL, true, WANTED},
		{"Freeze Wanted Level", &featureWantedLevelFrozen, &featureWantedLevelFrozenUpdated, true},
		{"Invincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated, true},
		{"Police Ignore You", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated, true},
		{"Unlimited Ability", &featurePlayerUnlimitedAbility, NULL, true},
		{"Noiseless", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated, true},
		{"Fast Swim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated, true},
		{"Fast Run", &featurePlayerFastRun, &featurePlayerFastRunUpdated, true},
		{"Super Jump", &featurePlayerSuperJump, NULL, true},
		{"No Ragdoll", &featureNoRagdoll, &featureNoRagdollUpdated, true},
		{"Invisibility", &featurePlayerInvisible, &featurePlayerInvisibleUpdated, true},
		{"Invisibility In Vehicle", &featurePlayerInvisibleInVehicle, &featurePlayerInvisibleInVehicleUpdated, true },
		{"Drunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated, true},
		{"Night Vision", &featureNightVision, &featureNightVisionUpdated, true},
		{"Thermal Vision", &featureThermalVision, &featureThermalVisionUpdated, true},
		{"Animations", NULL, NULL, false},
		{"Player Data", NULL, NULL, false},
		{"Prison Break", NULL, NULL, false}
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_player_menu);
}

// Reset all settings
bool onconfirm_reset_menu(MenuItem<int> choice){
	switch (activeLineIndexPlayer){
	case 0:
		menu_beep();
		set_menu_showing(true);
		WAIT(200);
		process_main_menu();
		set_menu_showing(false);
		break;
	case 1:
		reset_globals();
		process_main_menu();
		set_menu_showing(false);
		break;
	default:
		break;
	}

	return false;
}

void process_reset_menu(){
	const int lineCount = 2;
	
	std::string caption = "Reset All Settings";
	
	StandardOrToggleMenuDef lines[lineCount] = {
		{ "NO", NULL, NULL, true},
		{ "YES", NULL, NULL, true},
	};
	
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_reset_menu);

}

//==================
// MAIN MENU
//==================

int activeLineIndexMain = 0;

bool onconfirm_main_menu(MenuItem<int> choice){
	switch(choice.value){
		case 0:
			process_player_menu();
			break;
		case 1:
			process_teleport_menu(-1);
			break;
		case 2:
			process_weapon_menu();
			break;
		case 3:
			process_bodyguard_menu();
			break;
		case 4:
			process_veh_menu();
			break;
		case 5:
			process_world_menu();
			break;
		case 6:
			process_time_menu();
			break;
		case 7:
			process_props_menu();
			break;
		case 8:
			process_misc_menu();
			break;
		case 9:
			process_reset_menu();
			break;
		case 10:
			process_aimbot_esp_menu();
			break;
	}
	return false;
}

void process_main_menu(){
	std::ostringstream captionSS;
	captionSS << "~HUD_COLOUR_MENU_YELLOW~Enhanced ~HUD_COLOUR_WHITE~Native Trainer ~HUD_COLOUR_GREY~Update ";
	captionSS << VERSION_STRING;

	std::vector<MenuItem<int>*> menuItems;

	int i = 0;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "Player";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Locations";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Weapons";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Bodyguards";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Vehicles";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "World";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Time";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Objects";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Miscellaneous";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Reset All Settings";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	if(AIMBOT_INCLUDED){
		item = new MenuItem<int>();
		item->caption = "Aimbot ESP";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	MenuParameters<int> params(menuItems, captionSS.str());
	params.menuSelectionPtr = &activeLineIndexMain;
	params.onConfirmation = onconfirm_main_menu;
	params.sanitiseHeaderText = false;

	draw_generic_menu<int>(params);
}

void reset_globals(){
	reset_skin_globals();

	reset_vehicle_globals();

	reset_bodyguards_globals();

	reset_teleporter_globals();

	reset_weapon_globals();

	if(AIMBOT_INCLUDED){
		reset_aimbot_globals();
	}

	reset_world_globals();

	reset_misc_globals();

	reset_prop_globals();

	reset_areaeffect_globals();

	activeLineIndexMain =
		activeLineIndexPlayer =
		activeLineIndexWantedFreeze =
		frozenWantedLevel = 0;
		
	current_player_health = 2;
	current_player_armor = 6;
	current_player_prison = 0;
	current_player_escapemoney = 4;
	current_player_discharge = 3;

	featurePlayerDrunk =
		featurePlayerInvincible =
		featurePlayerIgnoredByPolice =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerFastSwim =
		featurePlayerFastRun =
		featurePlayerSuperJump =
		featurePlayerInvisible =
		featurePlayerInvisibleInVehicle =
		featureNightVision =
		featureThermalVision =

		featurePlayerLife =
		featurePlayerLife_Died = 
		featurePlayerLife_Changed =
		featurePrison_Hardcore =
		featureNoRagdoll =

		featureWantedLevelFrozen = false;

	featurePlayerInvincibleUpdated =
		featurePlayerIgnoredByPoliceUpdated =
		featurePlayerNoNoiseUpdated =
		featurePlayerFastSwimUpdated =
		featurePlayerFastRunUpdated =
		featurePlayerDrunkUpdated =
		featureNightVisionUpdated =
		featureThermalVisionUpdated =
		featurePlayerInvisibleUpdated =
		featurePlayerInvisibleInVehicleUpdated =
		featurePlayerLifeUpdated =
		featurePrison_Robe =
		featurePrison_Yard = 

		featureNoRagdollUpdated =
		featureWantedLevelFrozenUpdated = true;

	set_status_text("All settings reset to defaults");

	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
	CloseHandle(myHandle);
}

void main(){
	//reset_globals();

	setGameInputToEnabled(true, true);
	setAirbrakeRelatedInputToBlocked(false, true);

	write_text_to_log_file("Setting up calls");

	set_periodic_feature_call(update_features);

	write_text_to_log_file("Loading settings");

	load_settings();

	init_vehicle_feature();

	write_text_to_log_file("Loaded settings OK");

	// this creates a new locale based on the current application default
	// (which is either the one given on startup, but can be overriden with
	// std::locale::global) - then extends it with an extra facet that 
	// controls numeric output.
	std::locale comma_locale(std::locale(), new comma_numpunct());

	// tell cout to use our new locale.
	std::cout.imbue(comma_locale);

	set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ is ready!");

	while(true){
		if(trainer_switch_pressed()){
			menu_beep();
			set_menu_showing(true);
			process_main_menu();
			set_menu_showing(false);
		}
		else if(airbrake_switch_pressed()){
			menu_beep();
			process_airbrake_menu();
		}

		update_features();

		WAIT(0);
	}
}

void make_minidump(EXCEPTION_POINTERS* e){
	write_text_to_log_file("Dump requested");

	auto hDbgHelp = LoadLibraryA("dbghelp");
	if(hDbgHelp == nullptr){
		return;
	}
	auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump)) GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
	if(pMiniDumpWriteDump == nullptr){
		return;
	}

	auto hFile = CreateFileW(get_storage_dir_path("ENT-minidump.dmp"), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE){
		return;
	}

	MINIDUMP_EXCEPTION_INFORMATION exceptionInfo;
	exceptionInfo.ThreadId = GetCurrentThreadId();
	exceptionInfo.ExceptionPointers = e;
	exceptionInfo.ClientPointers = FALSE;

	auto dumped = pMiniDumpWriteDump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		hFile,
		MINIDUMP_TYPE(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory),
		e ? &exceptionInfo : nullptr,
		nullptr,
		nullptr);

	CloseHandle(hFile);

	write_text_to_log_file("Dump complete");

	return;
}

LONG CALLBACK unhandled_handler(EXCEPTION_POINTERS* e){
	write_text_to_log_file("Exception occured");
	make_minidump(e);
	return EXCEPTION_EXECUTE_HANDLER;
}

int filterException(int code, PEXCEPTION_POINTERS ex){
	set_status_text("Whoops, ENT crashed!");

	write_text_to_log_file("ScriptMain exception");
	make_minidump(ex);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ScriptMain(){
	srand(time(NULL));

	#ifdef _DEBUG
	__try{
		#endif

		set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ is initializing...");

		clear_log_file();

		write_text_to_log_file("Trying to init storage");
		init_storage();
		write_text_to_log_file("Init storage complete");

		write_text_to_log_file("Trying to init XINPUT");
		init_xinput();
		write_text_to_log_file("Init XINPUT complete");

		database = new ENTDatabase();
		if(!database->open()){
			write_text_to_log_file("Failed to open database");
			set_status_text("ENT couldn't open the database - exiting");
			database = NULL;
			return;
		}

		build_anim_tree();

		write_text_to_log_file("ScriptMain called - handler set");

		write_text_to_log_file("Reading XML config...");
		read_config_file();
		write_text_to_log_file("XML config read complete");

		write_text_to_log_file("Reading INI config....");
		read_config_ini_file();
		write_text_to_log_file("INI config read complete");

		//UnlockAllObjects();

		main();

		write_text_to_log_file("ScriptMain ended");

		#ifdef _DEBUG
	}
	__except(filterException(GetExceptionCode(), GetExceptionInformation())){

	}
	#endif
}

void ScriptTidyUp(){
	#ifdef _DEBUG
	__try{
		#endif

		write_text_to_log_file("ScriptTidyUp called");

		save_settings();
		write_text_to_log_file("Saved settings");

		setGameInputToEnabled(true, true);
		setAirbrakeRelatedInputToBlocked(false, true);
		write_text_to_log_file("Reset input");

		cleanup_script();
		write_text_to_log_file("Cleaned up script");
		WAIT(0);
		cleanup_props();
		write_text_to_log_file("Cleaned up props");
		WAIT(0);
		cleanup_anims();
		write_text_to_log_file("Cleaned up anims");

		end_xinput();
		write_text_to_log_file("XInput terminated");

		if(database != NULL){
			database->close();
			delete database;
			write_text_to_log_file("Database killed");
		}

		write_text_to_log_file("ScriptTidyUp done");
		#ifdef _DEBUG
	}
	__except(filterException(GetExceptionCode(), GetExceptionInformation())){

	}
	#endif
}

void add_player_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelFrozen", &featureWantedLevelFrozen, &featureWantedLevelFrozenUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerIgnoredByPolice", &featurePlayerIgnoredByPolice, &featurePlayerIgnoredByPoliceUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerUnlimitedAbility", &featurePlayerUnlimitedAbility});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerNoNoise", &featurePlayerNoNoise, &featurePlayerNoNoiseUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastSwim", &featurePlayerFastSwim, &featurePlayerFastSwimUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastRun", &featurePlayerFastRun, &featurePlayerFastRunUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerSuperJump", &featurePlayerSuperJump});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoRagdoll", &featureNoRagdoll, &featureNoRagdollUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisible", &featurePlayerInvisible, &featurePlayerInvisibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisibleInVehicle", &featurePlayerInvisibleInVehicle, &featurePlayerInvisibleInVehicleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerDrunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureNightVision", &featureNightVision, &featureNightVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureThermalVision", &featureThermalVision, &featureThermalVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife", &featurePlayerLife, &featurePlayerLifeUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife_Died", &featurePlayerLife_Died});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife_Changed", &featurePlayerLife_Changed});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Hardcore", &featurePrison_Hardcore});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Robe", &featurePrison_Robe});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Yard", &featurePrison_Yard});
}

void add_world_feature_enablements3(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{"current_player_health", std::to_string(current_player_health)});
	results->push_back(StringPairSettingDBRow{"current_player_armor", std::to_string(current_player_armor)});
	results->push_back(StringPairSettingDBRow{"current_player_prison", std::to_string(current_player_prison)});
	results->push_back(StringPairSettingDBRow{"current_player_escapemoney", std::to_string(current_player_escapemoney)});
	results->push_back(StringPairSettingDBRow{"current_player_discharge", std::to_string(current_player_discharge)});
}

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements(){
	std::vector<FeatureEnabledLocalDefinition> results;

	add_player_feature_enablements(&results);

	add_world_feature_enablements(&results);

	add_time_feature_enablements(&results);

	add_misc_feature_enablements(&results);

	add_props_feature_enablements(&results);

	add_vehicle_feature_enablements(&results);

	add_weapon_feature_enablements(&results);

	add_bodyguards_feature_enablements(&results);

	add_areaeffect_feature_enablements(&results);

	return results;
}

std::vector<StringPairSettingDBRow> get_generic_settings(){
	std::vector<StringPairSettingDBRow> settings;
	add_time_generic_settings(&settings);
	add_world_generic_settings(&settings);
	add_vehicle_generic_settings(&settings);
	handle_generic_settings_teleportation(&settings);
	add_world_feature_enablements2(&settings);
	add_world_feature_enablements3(&settings);
	add_weapon_feature_enablements2(&settings);
	add_bodyguards_feature_enablements2(&settings);
	add_coords_generic_settings(&settings);
	add_misc_generic_settings(&settings);
	add_hotkey_generic_settings(&settings);
	add_props_generic_settings(&settings);
	add_weapons_generic_settings(&settings);
	add_areaeffect_generic_settings(&settings);

	if(AIMBOT_INCLUDED){
		add_aimbot_esp_generic_settings(&settings);
	}

	add_bodyguards_generic_settings(&settings);
	add_skin_generic_settings(&settings);

	settings.push_back(StringPairSettingDBRow{"frozenWantedLevel", std::to_string(frozenWantedLevel)});

	return settings;
}

void handle_generic_settings(std::vector<StringPairSettingDBRow> settings){
	for(int i = 0; i < settings.size(); i++){
		StringPairSettingDBRow setting = settings.at(i);
		if(setting.name.compare("frozenWantedLevel") == 0){
			frozenWantedLevel = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_health") == 0){
			current_player_health = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_armor") == 0){
			current_player_armor = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_prison") == 0){
			current_player_prison = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_escapemoney") == 0){
			current_player_escapemoney = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_discharge") == 0){
			current_player_discharge = stoi(setting.value);
		}
	}

	//pass to anyone else, vehicles, weapons etc

	handle_generic_settings_time(&settings);

	handle_generic_settings_teleportation(&settings);

	handle_generic_settings_misc(&settings);

	handle_generic_settings_vehicle(&settings);

	handle_generic_settings_world(&settings);

	handle_generic_settings_hotkey(&settings);

	handle_generic_settings_props(&settings);

	handle_generic_settings_weapons(&settings);

	handle_generic_settings_areaeffect(&settings);

	if(AIMBOT_INCLUDED){
		handle_generic_settings_aimbot_esp(&settings);
	}

	handle_generic_settings_bodyguards(&settings);

	handle_generic_settings_skin(&settings);
}

DWORD WINAPI save_settings_thread(LPVOID lpParameter){
	save_settings();
	return 0;
}

void save_settings(){
	if(!everInitialised){
		return;
	}

	write_text_to_log_file("Saving settings, start");

	/*
	if (!db_mutex.try_lock())
	{
		write_text_to_log_file("Couldn't get lock, aborting");
		return;
	}
	*/

	if(database != NULL){
		write_text_to_log_file("Actually saving");
		database->store_setting_pairs(get_generic_settings());
		database->store_feature_enabled_pairs(get_feature_enablements());
		write_text_to_log_file("Save flag released");
	}
}

void load_settings(){
	handle_generic_settings(database->load_setting_pairs());

	write_text_to_log_file("Got generic pairs");

	database->load_feature_enabled_pairs(get_feature_enablements());

	write_text_to_log_file("Got feature pairs");

	//db_mutex.unlock();
}

void init_storage(){
	WCHAR* folder = get_storage_dir_path();
	write_text_to_log_file("Trying to create storage folder");

	std::wstring ws1(folder);
	std::string folderSS1(ws1.begin(), ws1.end());

	write_text_to_log_file(folderSS1);
	if(CreateDirectoryW(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		write_text_to_log_file("Storage dir created or exists");
	}
	else{
		write_text_to_log_file("Couldn't create storage dir");
	}
	delete folder;

	WCHAR* folder2 = get_temp_dir_path();
	std::wstring ws2(folder2);
	std::string folderSS2(ws2.begin(), ws2.end());

	write_text_to_log_file("Trying to create temp folder");
	write_text_to_log_file(folderSS2);
	if(CreateDirectoryW(folder2, NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		write_text_to_log_file("Temp dir created or exists");
	}
	else{
		write_text_to_log_file("Couldn't create temp dir");
	}
	delete folder2;
}

WCHAR* get_temp_dir_path(){
	WCHAR s[MAX_PATH];
	GetTempPathW(MAX_PATH, s);

	WCHAR combined[MAX_PATH];
	PathCombineW(combined, s, L"GTAV Enhanced Native Trainer");

	WCHAR *result = new WCHAR[MAX_PATH];

	wcsncpy(result, combined, MAX_PATH);

	std::wstring ws(result);
	std::string folderSS(ws.begin(), ws.end());

	return result;
}

WCHAR* get_temp_dir_path(char* file){
	WCHAR *output = new WCHAR[MAX_PATH];

	WCHAR* folder = get_temp_dir_path();

	WCHAR* wfile = new WCHAR[MAX_PATH];
	mbstowcs(wfile, file, MAX_PATH);

	PathCombineW(output, folder, wfile);

	delete folder;
	delete wfile;

	return output;
}

WCHAR* get_storage_dir_path(){
	PWSTR localAppData;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &localAppData);

	WCHAR combined[MAX_PATH];
	PathCombineW(combined, localAppData, L"GTAV Enhanced Native Trainer");

	WCHAR *result = new WCHAR[MAX_PATH];

	wcsncpy(result, combined, MAX_PATH);

	CoTaskMemFree(localAppData);

	return result;
}

WCHAR* get_storage_dir_path(char* file){
	WCHAR *output = new WCHAR[MAX_PATH];
	WCHAR* folder = get_storage_dir_path();

	WCHAR* wfile = new WCHAR[MAX_PATH];
	mbstowcs(wfile, file, MAX_PATH);

	PathCombineW(output, folder, wfile);

	delete folder;
	delete wfile;

	return output;
}

ENTDatabase* get_database(){
	return database;
}

struct GraphicsTest{
	void(*function)(BOOL);
	bool state;
};

bool allGraphicsOn = false;

std::vector<GraphicsTest> graphicsTests = {
	{ GRAPHICS::_0x1BBC135A4D25EDDE, false },
	{ GRAPHICS::_0x23BA6B0C2AD7B0D3, false },
	{ GRAPHICS::_0x22A249A53034450A, false },
	{ GRAPHICS::_0xDC459CFA0CCE245B, false },
	{ GRAPHICS::_0xC6372ECD45D73BCD, false },
	//{ GRAPHICS::_0x61BB1D9B3A95D802, false },
	{ GRAPHICS::_0xEF398BEEE4EF45F9, false },
	//{ GRAPHICS::_0x80ECBC0C856D3B0B, false }, //off = far shadows

	{ GRAPHICS::_0x25FC3E33A31AD0C9, false },
	{ GRAPHICS::_0x6DDBF9DFFC4AC080, false },
	//{ GRAPHICS::_0xD39D13C9FEBF0511, false }, //off = detailed shadows

	{ GRAPHICS::_0x0AE73D8DF3A762B2, false },
	{ GRAPHICS::_0xA51C4B86B71652AE, false },
	{ GRAPHICS::_0xC0416B061F2B7E5E, false },

	{ GRAPHICS::_0x06F761EA47C1D3ED, false },
	{ GRAPHICS::_0xE63D7C6EECECB66B, false },
	//{ GRAPHICS::_0x7AC24EAB6D74118D, false },
	{ GRAPHICS::_0x8CDE909A0370BB3A, false },
	{ GRAPHICS::_0x8CDE909A0370BB3A, false },

	{ GRAPHICS::ENABLE_ALIEN_BLOOD_VFX, false },
	{ GRAPHICS::_0xCA4AE345A153D573, false },
	{ GRAPHICS::_0x9B079E5221D984D3, false },
	{ GRAPHICS::_0xA46B73FAA3460AE1, false },

	{ GRAPHICS::_0x0E4299C549F0D1F1, false },
	{ GRAPHICS::_0x02369D5C8A51FDCF, false },
	{ GRAPHICS::_0x03300B57FCAC6DDB, false },
	{ GRAPHICS::_SET_FORCE_PED_FOOTSTEPS_TRACKS, false },
	{ GRAPHICS::_SET_FORCE_VEHICLE_TRAILS, false },

	{ GRAPHICS::_0x74C180030FDE4B69, false },
	//{ GRAPHICS::_0xD1C55B110E4DF534, false },
	{ GRAPHICS::_0x108BE26959A9D9BB, false },
	{ GRAPHICS::_0xA356990E161C9E65, false },
	//{ GRAPHICS::_SET_BLACKOUT, false }
};

bool get_graphics_test(std::vector<int> extras){
	int choice = extras.at(0);
	GraphicsTest* gt = &graphicsTests.at(choice);
	return gt->state;
}

void set_graphics_test(bool applied, std::vector<int> extras){
	int choice = extras.at(0);

	GraphicsTest* gt = &graphicsTests.at(choice);
	gt->state = !gt->state;
	gt->function(gt->state);

	//std::ostringstream ss;
	//ss << "Item " << choice << " set to " << gt->state << " using " << gt->function;
	//set_status_text_centre_screen(ss.str());
}

void set_all_graphics_test(bool applied, std::vector<int> extras){
	allGraphicsOn = applied;
	for(int i = 0; i < graphicsTests.size(); i++){
		GraphicsTest* gt = &graphicsTests.at(i);
		gt->state = applied;
		gt->function(gt->state);
	}
}

bool get_all_graphics_test(std::vector<int> extras){
	return allGraphicsOn;
}

bool onconfirm_testmenu(MenuItem<int> choice){
	Ped ped = PLAYER::PLAYER_PED_ID();
	Hash hash = GAMEPLAY::GET_HASH_KEY("WEAPON_NIGHTSTICK");
	DWORD start = GetTickCount();
	for(int i = 0; i < 100000; i++){
		Hash hash;
		WEAPON::GET_CURRENT_PED_WEAPON(ped, &hash, true);
	}
	DWORD end = GetTickCount();
	std::ostringstream ss;
	ss << "1000 weapon calls took " << (end - start) << "msec";
	set_status_text_centre_screen(ss.str());
	return false;
}

void process_test_menu(){
	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *mitem = new MenuItem<int>();
	mitem->caption = "SCPW Performance Test";
	mitem->isLeaf = false;
	mitem->value = -2;
	menuItems.push_back(mitem);

	FunctionDrivenToggleMenuItem<int> *item = new FunctionDrivenToggleMenuItem<int>();
	item->getter_call = get_all_graphics_test;
	item->setter_call = set_all_graphics_test;
	item->caption = "All Options";
	item->isLeaf = false;
	item->value = -1;
	menuItems.push_back(item);

	for(int i = 0; i < graphicsTests.size(); i++){
		FunctionDrivenToggleMenuItem<int> *item = new FunctionDrivenToggleMenuItem<int>();
		item->getter_call = get_graphics_test;
		item->setter_call = set_graphics_test;
		item->extra_arguments.push_back(i);
		item->isLeaf = false;
		item->value = i;

		std::ostringstream ss;
		ss << "Test #" << i;

		item->caption = ss.str();
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, 0, "Test Funcs", onconfirm_testmenu, NULL, NULL, skin_save_menu_interrupt);

}

void debug_native_investigation(){
	/*BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
		{
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			bool bbay = VEHICLE::_0x1033371FC8E842A7(veh);
			std::ostringstream ss;
			ss << "B/Bay: " << (bbay ? "Yes" : "No");
			set_status_text_centre_screen(ss.str());
		}
	}*/

	/*
	bool online = NETWORK::NETWORK_IS_GAME_IN_PROGRESS() == 1;

	std::ostringstream ss;
	ss << "Online: " << (online ? "Yes" : "No");
	set_status_text_centre_screen(ss.str());*/
	/*
	for (int i = 0; i < graphicsTests.size(); i++)
	{
		GraphicsTest* gt = &graphicsTests.at(i);
		//gt->state = applied;
		gt->function(gt->state);
	}
	*/

	/*
	if (!PED::_0x784002A632822099(PLAYER::PLAYER_PED_ID())) //putting on helmet?
	{
		set_status_text_centre_screen("0x784... false");
	}

	if (!PED::_0x66680A92700F43DF(PLAYER::PLAYER_PED_ID())) //getting in plane?
	{
		set_status_text_centre_screen("0x666... false");
	}

	if (PED::_0x0525A2C2562F3CD4(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x0525... true");
	}
	if (PED::_0xE0D36E5D9E99CC21(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0xE0D... true");
	}
	if (PED::_0x604E810189EE3A59(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x604... true");
	}

	if (PED::_0x3E802F11FBE27674(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}
	if (PED::_0xF41B5D290C99A3D6(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x3e80... true");
	}

	if (PED::_0x604E810189EE3A59(PLAYER::PLAYER_PED_ID()))
	{
		set_status_text_centre_screen("0x604... true");
	}
	*/

	/*
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), true))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

		int primary, secondary;
		VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);

		std::ostringstream ss;
		ss << "Primary: " << primary << ", sec: " << secondary;
		set_status_text_centre_screen(ss.str());*/

		/*if (!VEHICLE::_0x8D474C8FAEFF6CDE(veh))
		{
			set_status_text_centre_screen("0x8D474... false");
		}
		else if (VEHICLE::_0xAE3FEE8709B39DCB(veh))
		{
			set_status_text_centre_screen("0xAE3F... true");
		}
		else if (VEHICLE::_0x26C10ECBDA5D043B(veh))
		{
			set_status_text_centre_screen("0x26C10... true");
		}
		else if (VEHICLE::_0xB5CC40FBCB586380(veh)) //possibly is_vehicle_strong
		{
			set_status_text_centre_screen("0xB5CCC... true");
		}
		else if (VEHICLE::_0x62CA17B74C435651(veh))
		{
			set_status_text_centre_screen("0x62CA1... true");
		}
		else if (VEHICLE::_0x634148744F385576(veh))
		{
			set_status_text_centre_screen("0x634... true");
		}
		else if (VEHICLE::_0x1033371FC8E842A7(veh))
		{
			set_status_text_centre_screen("0x10333... true");
		}
		else if (!VEHICLE::_0x9A83F5F9963775EF(veh))
		{
			set_status_text_centre_screen("0x9a8... false");
		}
		else if (VEHICLE::_0xAE3FEE8709B39DCB(veh))
		{
			set_status_text_centre_screen("0xAE3Fee... true");
		}
		else if (VEHICLE::_0x291E373D483E7EE7(veh))
		{
			set_status_text_centre_screen("0x291... true");
		}
		else if (VEHICLE::_0x4198AB0022B15F87(veh))
		{
			set_status_text_centre_screen("0x419... true");
		}
		else if (VEHICLE::_0x755D6D5267CBBD7E(veh))
		{
			set_status_text_centre_screen("0x755... true");
		}
		else if (VEHICLE::_0x5991A01434CE9677(veh))
		{
			set_status_text_centre_screen("0x599... true");
		}
		else if (VEHICLE::_0x1821D91AD4B56108(veh))
		{
			set_status_text_centre_screen("0x182... true");
		}
		else if (VEHICLE::_0x6E08BF5B3722BAC9(veh))
		{
			set_status_text_centre_screen("0x6E0... true");
		}
		else if (VEHICLE::_0xD4C4642CB7F50B5D(veh))
		{
			set_status_text_centre_screen("0xD4C... true");
		}
	}*/
}

void heal_player(){
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	ENTITY::SET_ENTITY_HEALTH(playerPed, PED::GET_PED_MAX_HEALTH(playerPed));
	PED::SET_PED_ARMOUR(playerPed, PLAYER::GET_PLAYER_MAX_ARMOUR(player));
	PLAYER::SPECIAL_ABILITY_FILL_METER(player, false);

	PED::SET_PED_WETNESS_HEIGHT(playerPed, -2.0);
	PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);

	if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if(ENTITY::DOES_ENTITY_EXIST(playerVeh) && !ENTITY::IS_ENTITY_DEAD(playerVeh)){
			VEHICLE::SET_VEHICLE_FIXED(playerVeh);
		}
	}

	set_status_text("Player healed");
}

void toggle_invisibility(){
	featurePlayerInvisible = !featurePlayerInvisible;
	if(featurePlayerInvisible){
		set_status_text("Player invisible");
	}
	else{
		set_status_text("Player no longer invisible");
	}
	featurePlayerInvisibleUpdated = true;
}

void reset_wanted_level(){
	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
	set_status_text("Wanted level cleared");
}

int get_frame_number(){
	return game_frame_num;
}

void toggle_thermal_vision(){
	featureThermalVision = !featureThermalVision;
	featureThermalVisionUpdated = true;
}

void toggle_night_vision(){
	featureNightVision = !featureNightVision;
	featureNightVisionUpdated = true;
}

void cleanup_script(){
	cleanup_area_effects();
}

bool is_player_ignored_by_police(){
	return featurePlayerIgnoredByPolice;
}
