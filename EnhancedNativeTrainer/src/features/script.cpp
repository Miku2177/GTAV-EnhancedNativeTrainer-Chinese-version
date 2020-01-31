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
#include "skins.h"
#include "hotkeys.h"
#include "propplacement.h"
#include "area_effect.h"
#include "prison_break.h"
#include "road_laws.h"
#include "vehicles.h"
#include "weapons.h"

#include "../version.h"
#include "../utils.h"
#include "../ui_support/file_dialog.h"
#include "..\ui_support\menu_functions.h"

#include <set>
#include <iostream>
#include <vector>
#include <psapi.h>
#include <ctime>

#pragma warning(disable : 4244 4305) // double <-> float conversions

bool AIMBOT_INCLUDED = false;

bool player_died = false;
bool npc_player_died = false;

int last_player_slot_seen = 0;

int game_frame_num = 0;

int jumpfly_secs_passed, jumpfly_secs_curr, jumpfly_tick = 0;
bool skydiving = false;

bool everInitialised = false;

ENTDatabase* database = NULL;
Camera DeathCam = NULL;

bool onlineWarningShown = false;

//std::mutex db_mutex;

// features
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featurePlayerIgnoredByPolice = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastRun = false;
bool featurePlayerRunApartments = false;
//bool featurePlayerSuperJump = false;
bool featurePlayerInvisible = false;
bool featurePlayerInvisibleInVehicle = false;
bool featurePlayerDrunk = false;
bool featurePlayerDrunkUpdated = false;
bool featureNightVision = false;
bool featureNightVisionUpdated = false;
bool featureThermalVision = false;
bool featureThermalVisionUpdated = false;
bool featureWantedLevelFrozen = false;
bool featureWantedLevelFrozenUpdated = false;
bool featureLevitation = false;
bool featureNoScubaGearMask = false;
bool featureNoScubaSound = false;

bool super_jump_no_parachute, super_jump_intheair = false;

bool manual_instant = false;
bool first_person_rotate = false;

bool featureWantedLevelNoPHeli = false;
bool featureWantedLevelNoPHeliUpdated = false;
bool featureWantedNoPRoadB = false;
bool featureWantedNoPRoadBUpdated = false;
bool featureWantedLevelNoPBoats = false;
bool featureWantedLevelNoPBoatsUpdated = false;
bool featureWantedLevelNoSWATVehicles = false;
bool featureWantedLevelNoSWATVehiclesUpdated = false;

bool NoTaxiWhistling = false;
bool featurePlayerCanBeHeadshot = false;
bool featurePlayerInjuredMovement = false;
bool featureRespawnsWhereDied = false;

bool lev_message = false;

bool engine_running = true;
bool engine_switched = false;
bool engine_killed = false;
bool we_have_troubles, iaminside = false;

bool been_injured = false;

bool p_invisible = false;
bool featurePlayerLife = false;
bool featurePlayerLifeUpdated = true;
bool featurePlayerStatsUpdated = true;
bool featurePlayerLife_Died = false;
bool featurePlayerStatsEnable = false;
bool featurePlayerLife_Changed = false;

bool featureNoRagdoll = false;
bool featureRagdollIfInjured = false;

// ragdoll if injured variables
bool been_damaged_by_weapon, ragdoll_task = false;
float been_damaged_health, been_damaged_armor = -1;
int ragdoll_seconds = 0; 
//
Ped oldplayerPed = -1;
int tick, playerDataMenuIndex, playerPrisonMenuIndex, playerForceshieldMenuIndex = 0;
int scr_tick_secs_passed, scr_tick_secs_curr = 0;
int NPCragdollMenuIndex = 0;
int PlayerMovementMenuIndex = 0;
int PlayerMostWantedMenuIndex = 0;
int PlayerWantedMaxPossibleLevelMenuIndex = 0;
int death_time2 = -1;

int wanted_maxpossible_level = 3;
bool wanted_maxpossible_level_Changed;

int frozenWantedLevel = 0;

std::vector<Vehicle> VEHICLE_ENGINE;
std::vector<Vehicle> VEHICLE_KILLED;

// player model control, switching on normal ped model when needed

char* player_models[] = {"player_zero", "player_one", "player_two"};

char* mplayer_models[] = {"mp_f_freemode_01", "mp_m_freemode_01"};

const char* CLIPSET_DRUNK = "move_m@drunk@verydrunk";

const std::vector<std::string> GRAVITY_CAPTIONS{"Minimum", "0.1x", "0.5x", "0.75x", "1x (Normal)"};
const float GRAVITY_VALUES[] = { 0.0f, 0.1f, 0.5f, 0.75f, 1.0f };

const std::vector<std::string> REGEN_CAPTIONS{"No Regeneration", "0.1x", "0.25x", "0.5x", "1x (Normal)", "2x", "5x", "10x", "20x", "50x", "100x", "200x", "500x", "1000x"};
const std::vector<float> REGEN_VALUES{0.0f, 0.1f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f, 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f};
int current_regen_speed = 4;
bool current_regen_speed_changed = true;

//Player Health
const std::vector<std::string> PLAYER_HEALTH_CAPTIONS{ "OFF", "1", "10", "20", "30", "50", "100", "200", "300", "500", "1000", "5000", "10000", "20000", "30000" };
const int PLAYER_HEALTH_VALUES[] = { 0, 101, 110, 120, 130, 150, 200, 300, 400, 600, 1100, 5100, 10100, 20100, 30100 };
int current_player_health = 6;
bool current_player_health_Changed = true;
int PedsHealthIndex = 0;
bool PedsHealthChanged = true;

//Player Armor
const std::vector<std::string> PLAYER_ARMOR_CAPTIONS{ "0", "15", "20", "30", "40", "50", "100" };
const int PLAYER_ARMOR_VALUES[] = { 0, 15, 20, 30, 40, 50, 100 };
int current_player_armor = 6;
bool current_player_armor_Changed = true;
int current_player_stats = 6;
bool current_player_stats_Changed = true;

//NPC Ragdoll If Shot
const std::vector<std::string> NPC_RAGDOLL_CAPTIONS{ "OFF", "Never", "Always" };
const int NPC_RAGDOLL_VALUES[] = { 0, 1, 2 };
int current_npc_ragdoll = 0;
bool current_npc_ragdoll_Changed = true;

//Player Movement Speed && Hancock Mode
const std::vector<std::string> PLAYER_MOVEMENT_CAPTIONS{ "Normal", "0.5x", "1x", "2x", "3x", "4x", "5x", "6x", "7x", "8x", "9x", "10x" };
const double PLAYER_MOVEMENT_VALUES[] = { 0.00, 0.60, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 10.00 };
int current_player_movement = 0;
bool current_player_movement_Changed = true; 
int current_player_jumpfly = 0;
bool current_player_jumpfly_Changed = true;
int current_player_superjump = 0;
bool current_player_superjump_Changed = true;

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

void onchange_regen_callback(int value, SelectFromListMenuItem* source) {
	current_regen_speed = value;
	current_regen_speed_changed = true;
}

void onchange_player_stats_mode(int value, SelectFromListMenuItem* source) {
	current_player_stats = value;
	current_player_stats_Changed = true;
}

void onchange_player_prison_mode(int value, SelectFromListMenuItem* source){
	current_player_prison = value;
	current_player_prison_Changed = true;
}

void onchange_player_mostwanted_mode(int value, SelectFromListMenuItem* source) {
	current_player_mostwanted = value;
	current_player_mostwanted_Changed = true;
}

void onchange_player_mostwanted_level_mode(int value, SelectFromListMenuItem* source) {
	mostwanted_level_enable = value;
	mostwanted_level_enable_Changed = true;
}

void onchange_player_wanted_maxpossible_level_mode(int value, SelectFromListMenuItem* source) {
	wanted_maxpossible_level = value;
	wanted_maxpossible_level_Changed = true;
}

void onchange_player_movement_mode(int value, SelectFromListMenuItem* source) {
	current_player_movement = value;
	current_player_movement_Changed = true;
}

void onchange_player_jumpfly_mode(int value, SelectFromListMenuItem* source) {
	current_player_jumpfly = value;
	current_player_jumpfly_Changed = true;
}

void onchange_player_superjump_mode(int value, SelectFromListMenuItem* source) {
	current_player_superjump = value;
	current_player_superjump_Changed = true;
}

void onchange_NPC_ragdoll_mode(int value, SelectFromListMenuItem* source) {
	current_npc_ragdoll = value;
	current_npc_ragdoll_Changed = true;
}

void onchange_player_escapemoney_mode(int value, SelectFromListMenuItem* source){
	current_player_escapemoney = value;
	current_player_escapemoney_Changed = true;
}

void onchange_player_discharge_mode(int value, SelectFromListMenuItem* source){
	current_player_discharge = value;
	current_player_discharge_Changed = true;
}

void onchange_player_escapestars_mode(int value, SelectFromListMenuItem* source) {
	current_escape_stars = value;
	current_escape_stars_Changed = true;
}

void check_player_model(){
	/*
	std::stringstream ss;
	ss << "PID: " << *gp;
	set_status_text_centre_screen(ss.str());
	*/

	// common variables
	//Player player = PLAYER::PLAYER_ID(); 
	//Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())){
		return;
	}

	//find out whether we're a default player model
	bool found = false;
	Hash playerModel = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());
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

	if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && !featureRespawnsWhereDied) { //  && is_player_reset_on_death()
		if (!found) {
			if (is_player_reset_on_death()) set_status_text("Resetting death state because a custom skin was used");
			if (!is_player_reset_on_death()) {
				Hash model_d = ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID());
				if (model_d != GAMEPLAY::GET_HASH_KEY("a_c_dolphin") && model_d != GAMEPLAY::GET_HASH_KEY("a_c_sharkhammer") && model_d != GAMEPLAY::GET_HASH_KEY("a_c_humpback") &&
					model_d != GAMEPLAY::GET_HASH_KEY("a_c_killerwhale") && model_d != GAMEPLAY::GET_HASH_KEY("a_c_stingray") && model_d != GAMEPLAY::GET_HASH_KEY("a_c_sharktiger") &&
					model_d != GAMEPLAY::GET_HASH_KEY("a_c_fish") && model_d != GAMEPLAY::GET_HASH_KEY("a_c_whalegrey")) model_to_restore = model_d; // !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && 
			}
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			//if (!is_player_reset_on_death()) WAIT(5500); // 8500
			int spPlayerCount = sizeof(player_models) / sizeof(player_models[0]);
			if (last_player_slot_seen < spPlayerCount) {
				applyChosenSkin(player_models[last_player_slot_seen]);
				if (!is_player_reset_on_death()) {
					npc_player_died = true;
					ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0);
				}
			}
			else {
				applyChosenSkin(mplayer_models[last_player_slot_seen - spPlayerCount]);
				if (!is_player_reset_on_death()) {
					npc_player_died = true;
					ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0);
				}
			}
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) WAIT(0); // wait until player is resurrected
		}
	}

	if (!featureRespawnsWhereDied && !is_player_reset_on_death() && npc_player_died == true && PLAYER::GET_TIME_SINCE_LAST_DEATH() > 100 && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID())) { //  && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) //  && PLAYER::GET_TIME_SINCE_LAST_DEATH() < 10000 
		npc_player_died = false;
		if (npc_player_died == false && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID())) applyChosenSkin(model_to_restore);
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
	Vehicle veh = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh = temp_vehicle;
	}

	if (!VEHICLE_ENGINE.empty() && VEHICLE_ENGINE[0] != veh) VEHICLE_ENGINE[0] = veh;
	if (VEHICLE_ENGINE.empty()) VEHICLE_ENGINE.push_back(veh);
	if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh)) engine_running = true;
	engine_running = !engine_running;
	engine_switched = true;
	WAIT(100);
}

void engine_damage() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh3 = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh3 = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh3 = temp_vehicle;
	}
	
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh3, -1);
}

void engine_kill(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh2 = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh2 = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh2 = temp_vehicle;
	}

	if (VEHICLE_KILLED.empty()) VEHICLE_KILLED.push_back(veh2);
	if (!VEHICLE_KILLED.empty()) VEHICLE_KILLED[0] = veh2;
	engine_killed = true;
	set_status_text("You have destroyed this vehicle's engine for some reason");
}

// Updates all features that can be turned off by the game, being called each game frame
void update_features(){
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

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

	// First Person Death Camera
	if (featureFirstPersonDeathCamera) {
		if ((PED::GET_PED_TYPE(PLAYER::PLAYER_PED_ID()) == 0 && ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == GAMEPLAY::GET_HASH_KEY("player_zero")) ||
			(PED::GET_PED_TYPE(PLAYER::PLAYER_PED_ID()) == 1 && ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == GAMEPLAY::GET_HASH_KEY("player_one")) ||
			((PED::GET_PED_TYPE(PLAYER::PLAYER_PED_ID()) == 2 || PED::GET_PED_TYPE(PLAYER::PLAYER_PED_ID()) == 3) && ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == GAMEPLAY::GET_HASH_KEY("player_two"))) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2);
			if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
				if (!CAM::DOES_CAM_EXIST(DeathCam)) {
					DeathCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
					CAM::ATTACH_CAM_TO_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, -0.15, 0, 1);
					CAM::POINT_CAM_AT_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, 0.0, 0, 1);
					CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(DeathCam, 1.0);
					CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCam, 1.0);
					CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCam, 1.0);
					CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
					CAM::SET_CAM_ACTIVE(DeathCam, true);
					ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 0, 0);
				}
			}

			if (CAM::DOES_CAM_EXIST(DeathCam)) { // camera rotation
				Vector3 rot_cam = CAM::GET_CAM_ROT(DeathCam, 2);
				if ((CONTROLS::IS_CONTROL_PRESSED(2, 34) || CONTROLS::IS_CONTROL_PRESSED(2, 35)) && first_person_rotate == false) {
					CAM::DESTROY_CAM(DeathCam, true);
					DeathCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
					CAM::ATTACH_CAM_TO_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, -0.15, 0, 1);
					first_person_rotate = true;
				}
				if (CONTROLS::IS_CONTROL_PRESSED(2, 34)) rot_cam.z = rot_cam.z - 1; // left only
				if (CONTROLS::IS_CONTROL_PRESSED(2, 35)) rot_cam.z = rot_cam.z + 1; // right only
				if (CONTROLS::IS_CONTROL_PRESSED(2, 32)) rot_cam.x = rot_cam.x - 1; // up only
				if (CONTROLS::IS_CONTROL_PRESSED(2, 33)) rot_cam.x = rot_cam.x + 1; // down only
				CAM::SET_CAM_ROT(DeathCam, rot_cam.x, rot_cam.y, rot_cam.z, 2);
			}
		}
	}
	if ((featureFirstPersonDeathCamera && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && !PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) || 
		(!featureFirstPersonDeathCamera && (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)))) {
		if (CAM::DOES_CAM_EXIST(DeathCam)) {
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), 1, 1);
			CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
			CAM::DETACH_CAM(DeathCam);
			CAM::SET_CAM_ACTIVE(DeathCam, false);
			CAM::DESTROY_CAM(DeathCam, true);
			ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255, 0);
			first_person_rotate = false;
		}
	}

	// Manual Respawn
	if (featureNoAutoRespawn && GAMEPLAY::GET_MISSION_FLAG() == 0) {
		if ((ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) && player_died == false) {
			SCRIPT::SET_NO_LOADING_SCREEN(true);
			GAMEPLAY::SET_FADE_OUT_AFTER_DEATH(false);
			GAMEPLAY::SET_FADE_OUT_AFTER_ARREST(false);
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			GAMEPLAY::IGNORE_NEXT_RESTART(true);
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
			manual_instant = true;
		}
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) { // 23 // CONTROLS::IS_CONTROL_JUST_PRESSED(2, 176) || 
			player_died = true;
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(false);
			SCRIPT::SET_NO_LOADING_SCREEN(false);
			CAM::DO_SCREEN_FADE_OUT(4000);
			manual_instant = false;
		}
		if ((PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) { // CONTROLS::IS_CONTROL_JUST_PRESSED(2, 176) || 
			CAM::DO_SCREEN_FADE_OUT(500);
			WAIT(1000);
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
			GAMEPLAY::IGNORE_NEXT_RESTART(true);
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			Vector3 ped_me = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			BOOL onGround = false;
			Vector3 CoordsWhereDied = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			PATHFIND::GET_SAFE_COORD_FOR_PED(ped_me.x, ped_me.y, ped_me.z, onGround, &CoordsWhereDied, 16);
			NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 0, false, false);
			PLAYER::RESET_PLAYER_ARREST_STATE(PLAYER::PLAYER_PED_ID());
			WAIT(1000);
			CAM::DO_SCREEN_FADE_IN(500);
			manual_instant = false;
		}
		if (!ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) player_died = false;
	}
		
	// Instant Respawn On Death/Arrest
	if (featureRespawnsWhereDied && GAMEPLAY::GET_MISSION_FLAG() == 0 && manual_instant == false) {
		if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) {
			player_died = true;
			CAM::DO_SCREEN_FADE_OUT(500);
			WAIT(1000);
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
			GAMEPLAY::IGNORE_NEXT_RESTART(true);
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			Vector3 ped_me = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			BOOL onGround = false;
			Vector3 CoordsWhereDied = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			PATHFIND::GET_SAFE_COORD_FOR_PED(ped_me.x, ped_me.y, ped_me.z, onGround, &CoordsWhereDied, 16);
			NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 0, false, false);
			PLAYER::RESET_PLAYER_ARREST_STATE(playerPed);
			WAIT(1000);
			CAM::DO_SCREEN_FADE_IN(500);
		}
	}

	update_centre_screen_status_text();

	update_vehicle_guns();

	update_world_features();

	update_skin_features();

	update_teleport_features();

	check_player_model();

	maintain_bodyguards();

	update_props_pending_dialogs();

	debug_native_investigation();

	update_area_effects(playerPed);
	update_vehicles(playerPed);

	// player invincible
	if(featurePlayerInvincibleUpdated){
		if(bPlayerExists && !featurePlayerInvincible){
			//PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
			PLAYER::_0x733A643B5B0C53C1(player, FALSE);
		}
		WAIT(100);
		featurePlayerInvincibleUpdated = false;
		WAIT(100);
	}

	if(featurePlayerInvincible && bPlayerExists){
		//PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
		PLAYER::_0x733A643B5B0C53C1(player, TRUE);
	}
	
	if (engine_switched) { 
		if (!VEHICLE_ENGINE.empty()) VEHICLE::SET_VEHICLE_ENGINE_ON(VEHICLE_ENGINE[0], engine_running, true);
	}
	
	if (engine_killed) {
		if (!VEHICLE_KILLED.empty()) {
			VEHICLE::SET_VEHICLE_ENGINE_ON(VEHICLE_KILLED[0], false, true);
			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(VEHICLE_KILLED[0], -4000);
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
	
	// No Police Helicopters
	if (featureWantedLevelNoPHeli) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(2, false);
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(12, false);
		featureWantedLevelNoPHeliUpdated = true;
	}
	else if (featureWantedLevelNoPHeliUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(2, true);
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(12, true);
		featureWantedLevelNoPHeliUpdated = false;
	}

	// No Road Blocks
	if (featureWantedNoPRoadB) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(8, false);
		featureWantedNoPRoadBUpdated = true;
	}
	else if (featureWantedNoPRoadBUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(8, true);
		featureWantedNoPRoadBUpdated = false;
	}

	// No Police Boats
	if (featureWantedLevelNoPBoats) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(13, false);
		featureWantedLevelNoPBoatsUpdated = true;
	}
	else if (featureWantedLevelNoPBoatsUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(13, true);
		featureWantedLevelNoPBoatsUpdated = false;
	}

	// No SWAT Vehicles
	if (featureWantedLevelNoSWATVehicles) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(4, false);
		featureWantedLevelNoSWATVehiclesUpdated = true;

	}
	else if (featureWantedLevelNoSWATVehiclesUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(4, true);
		featureWantedLevelNoSWATVehiclesUpdated = false;
	}

	// No Whistling For Taxi
	if (NoTaxiWhistling && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) && !UI::IS_HELP_MESSAGE_BEING_DISPLAYED()) {
		CONTROLS::DISABLE_CONTROL_ACTION(2, 51, 1);
	}
	
	// Levitation
	if (!featureLevitation) lev_message = false;
	if (featureLevitation) {
		if (lev_message == false) {
			set_status_text("Hold 'Jump' to use your force.");
			lev_message = true;
		}
		Vector3 my_coords = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		const int arrSize_punch = 1024;
		Ped surr_p_peds[arrSize_punch];
		int count_surr_p_peds = worldGetAllPeds(surr_p_peds, arrSize_punch);
		for (int i = 0; i < count_surr_p_peds; i++) {
			Vector3 obj_coords = ENTITY::GET_ENTITY_COORDS(surr_p_peds[i], true);
			int tempgot_x = (my_coords.x - obj_coords.x);
			int tempgot_y = (my_coords.y - obj_coords.y);
			int tempgot_z = (my_coords.z - obj_coords.z);
			if (tempgot_x < 0) tempgot_x = (tempgot_x * -1);
			if (tempgot_y < 0) tempgot_y = (tempgot_y * -1);
			if (tempgot_z < 0) tempgot_z = (tempgot_z * -1);
			ROPE::ACTIVATE_PHYSICS(surr_p_peds[i]);
			if (surr_p_peds[i] != playerPed && !PED::IS_PED_IN_ANY_VEHICLE(surr_p_peds[i], 0)) { // PED::GET_PED_TYPE(surr_p_peds[i]) != 0 && PED::GET_PED_TYPE(surr_p_peds[i]) != 1 && PED::GET_PED_TYPE(surr_p_peds[i]) != 2 && PED::GET_PED_TYPE(surr_p_peds[i]) != 3
				if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && tempgot_x < 20 && tempgot_y < 20) {
					PED::SET_PED_CAN_RAGDOLL(surr_p_peds[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(surr_p_peds[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(surr_p_peds[i]);
					STREAMING::REQUEST_ANIM_DICT("dead@fall");
					while (!STREAMING::HAS_ANIM_DICT_LOADED("dead@fall")) WAIT(0);
					if (!ENTITY::IS_ENTITY_PLAYING_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 3)) {
						AI::TASK_PLAY_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 8.0, 0.0, -1, 9, 0, 0, 0, 0);
						PED::SET_PED_TO_RAGDOLL(surr_p_peds[i], 1, 1, 1, 1, 1, 1);
					}
					if (tempgot_z < 20) ENTITY::APPLY_FORCE_TO_ENTITY(surr_p_peds[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true); // 20
				}
				if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && (tempgot_x > 19 || tempgot_y > 19) && tempgot_z > 5) {
					Vector3 curPLocation = ENTITY::GET_ENTITY_COORDS(surr_p_peds[i], 0);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(surr_p_peds[i], curPLocation.x, curPLocation.y, curPLocation.z - 1, 1, 1, 1);
				}
				if (CONTROLS::IS_CONTROL_RELEASED(2, 22) || tempgot_x > 19 || tempgot_y > 19) {
					if (ENTITY::IS_ENTITY_PLAYING_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 3)) {
						Vector3 curPLocation = ENTITY::GET_ENTITY_COORDS(surr_p_peds[i], 0);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(surr_p_peds[i], curPLocation.x, curPLocation.y, curPLocation.z, 1, 1, 1);
						AI::STOP_ANIM_TASK(surr_p_peds[i], "dead@fall", "dead_fall_down", 1.0);
					}
				}
			}
		} // end of int (peds)
		Object surr_objects[arrSize_punch];
		int count_surr_o = worldGetAllObjects(surr_objects, arrSize_punch);
		for (int i = 0; i < count_surr_o; i++) {
			Vector3 obj_coords = ENTITY::GET_ENTITY_COORDS(surr_objects[i], true);
			int tempgot_x = (my_coords.x - obj_coords.x);
			int tempgot_y = (my_coords.y - obj_coords.y);
			int tempgot_z = (my_coords.z - obj_coords.z);
			if (tempgot_x < 0) tempgot_x = (tempgot_x * -1);
			if (tempgot_y < 0) tempgot_y = (tempgot_y * -1);
			if (tempgot_z < 0) tempgot_z = (tempgot_z * -1);
			if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && tempgot_x < 20 && tempgot_y < 20 && tempgot_z < 20) ENTITY::APPLY_FORCE_TO_ENTITY(surr_objects[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true);
		} // end of int (objects)
		Vehicle surr_vehicles[arrSize_punch];
		int count_surr_v = worldGetAllVehicles(surr_vehicles, arrSize_punch);
		for (int i = 0; i < count_surr_v; i++) {
			Vector3 obj_coords = ENTITY::GET_ENTITY_COORDS(surr_vehicles[i], true);
			int tempgot_x = (my_coords.x - obj_coords.x);
			int tempgot_y = (my_coords.y - obj_coords.y);
			int tempgot_z = (my_coords.z - obj_coords.z);
			if (tempgot_x < 0) tempgot_x = (tempgot_x * -1);
			if (tempgot_y < 0) tempgot_y = (tempgot_y * -1);
			if (tempgot_z < 0) tempgot_z = (tempgot_z * -1);
			if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && tempgot_x < 20 && tempgot_y < 20 && tempgot_z < 20) ENTITY::APPLY_FORCE_TO_ENTITY(surr_vehicles[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true);
		} // end of int (vehicles)
	}

	// Hancock Mode
	if (PLAYER_MOVEMENT_VALUES[current_player_jumpfly] > 0.00 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
		Vector3 CamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		float p_force = PLAYER_MOVEMENT_VALUES[current_player_jumpfly]; //5;
		//if (PLAYER_MOVEMENT_VALUES[current_player_jumpfly] == 0.50) p_force = 0.60;

		float rad = 2 * 3.14 * (CamRot.z / 360);
		float v_x = -(sin(rad) * p_force * 10);
		float v_y = (cos(rad) * p_force * 10);
		float v_z = p_force * (CamRot.x * 0.2);
		if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), CamRot.x, CamRot.y, CamRot.z, 1, true);
		if (CONTROLS::IS_CONTROL_PRESSED(2, 22)) {
			jumpfly_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - jumpfly_secs_curr) != 0) {
				jumpfly_tick = jumpfly_tick + 1;
				jumpfly_secs_curr = jumpfly_secs_passed;
			}
			if (jumpfly_tick > 5) {
				if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) {
					AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
					AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle
				}
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0, 0, p_force, 0, 0, 0, true, false, true, true, true, true);
			}
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) skydiving = true;
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 32) && skydiving == true) { 
			if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) {
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle
			}
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), CamRot.x, CamRot.y, CamRot.z, 1, true);
			ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, v_x / 8, v_y / 8, v_z / 8, 0, 0, 0, true, false, true, true, true, true);
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 33) && skydiving == true) { 
			jumpfly_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - jumpfly_secs_curr) != 0) {
				jumpfly_tick = jumpfly_tick + 1;
				jumpfly_secs_curr = jumpfly_secs_passed;
			}
			if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) {
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle
			}
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), CamRot.x, CamRot.y, CamRot.z, 1, true);
			if (jumpfly_tick < 30) ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false); 
			else {
				ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, -(v_x / 8), -(v_y / 8), -(v_z / 8), 0, 0, 0, true, false, true, true, true, true);
			}
		}
		if (CONTROLS::IS_CONTROL_RELEASED(2, 33) && CONTROLS::IS_CONTROL_RELEASED(2, 22)) {
			ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
			jumpfly_tick = 0; 
		}
		if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(PLAYER::PLAYER_PED_ID())) { 
			AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3);
			if (skydiving == true) {
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "move_strafe@roll_fps", "combatroll_fwd_p1_00", 8.0, 0.0, -1, 9, 0, 0, 0, 0);
				WAIT(400);
			}
			skydiving = false;
		}
		if (skydiving == false) {
			AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3);
			AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID(), "move_strafe@roll_fps", "combatroll_fwd_p1_00", 3); 
		}
		if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) skydiving = true;
		else skydiving = false;
		if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "move_strafe@roll_fps", "combatroll_fwd_p1_00", 3)) skydiving = false;
	}

	// Player can be headshot
	if (featurePlayerCanBeHeadshot && !featurePlayerInvincible) {
		Vector3 coords_bullet_p = PED::GET_PED_BONE_COORDS(playerPed, 31086, 0, 0, 0); // head bone
		if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2) && GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_bullet_p.x, coords_bullet_p.y, coords_bullet_p.z, 0.1, 0, 0)) {
			PED::CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
			ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(playerPed);
			ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0);
		}
	}

	// Injured Player Movement
	if (featurePlayerInjuredMovement && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		if (!STREAMING::HAS_ANIM_DICT_LOADED("move_injured_generic")) STREAMING::REQUEST_ANIM_DICT("move_injured_generic"); // move_m@injured
		float curr_health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
		Vector3 coords_calf_l = PED::GET_PED_BONE_COORDS(playerPed, 63931, 0, 0, 0); // left calf
		Vector3 coords_calf_r = PED::GET_PED_BONE_COORDS(playerPed, 36864, 0, 0, 0); // right calf
		Vector3 coords_pelvis = PED::GET_PED_BONE_COORDS(playerPed, 11816, 0, 0, 0); // pelvis
		if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2) && (GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_calf_l.x, coords_calf_l.y, coords_calf_l.z, 0.4, 0, 0) || 
			GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_calf_r.x, coords_calf_r.y, coords_calf_r.z, 0.4, 0, 0) || GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_pelvis.x, coords_pelvis.y, coords_pelvis.z, 0.2, 0, 0))) {
			been_injured = true;
		}
		if (curr_health < 90 || been_injured == true) PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "move_injured_generic", 1.0f); // @walk
		if (curr_health < 50 || been_injured == true) CONTROLS::DISABLE_CONTROL_ACTION(2, 21, 1); // sprint
		if (curr_health < 30/* || been_injured == true*/) CONTROLS::DISABLE_CONTROL_ACTION(2, 22, 1); // jump
		if ((curr_health > (PLAYER_HEALTH_VALUES[current_player_health] - 111)) || (PLAYER::GET_TIME_SINCE_LAST_DEATH() > 100 && PLAYER::GET_TIME_SINCE_LAST_DEATH() < 5000) || 
			(PLAYER::GET_TIME_SINCE_LAST_ARREST() > 100 && PLAYER::GET_TIME_SINCE_LAST_ARREST() < 5000)/* || player_died == true*/) { // 99
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			PED::CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
			ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(playerPed);
			been_injured = false;
			//player_died = false;
		}
	}
	
	// Can run in apartments
	if (featurePlayerRunApartments && GAMEPLAY::GET_MISSION_FLAG() == 0 && !UI::IS_HELP_MESSAGE_BEING_DISPLAYED()) {
		Vector3 coords_apprun_ped = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		if (!INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) && 
			(INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 206849 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 166657 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 166401 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 115458 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 114946 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 171777 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 197121 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 197889 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 4866 ||
			INTERIOR::GET_INTERIOR_AT_COORDS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z) == 36866)) {
			iaminside = true;
			if (!ENTITY::IS_ENTITY_IN_AREA(playerPed, -802.346,	171.234, 70.8347, -799.234, 174.817, 74.8347, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -810.195, 169.765, 74.7408, -808.39, 171.398, 78.7408, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -10.91648, -1442.35, 29.1015, -8.14864, -1440.2, 33.1015, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 2.63788, 528.786, 172.628, 5.68781, 530.781, 176.628, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 1977.13, 3819.58, 31.4501, 1978.99, 3820.1, 35.4501, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -1161.46,	-1521.46, 8.6327, -1160.53, -1518.42, 12.6327, 1, 1, 1)) {
				if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 1) {
					we_have_troubles = false;
					PLAYER::SET_MAX_WANTED_LEVEL(5);
					PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				}

				if (PED::IS_PED_SHOOTING(playerPed)) we_have_troubles = true;

				if (we_have_troubles == false) {
					GAMEPLAY::CLEAR_AREA_OF_COPS(coords_apprun_ped.x, coords_apprun_ped.y, coords_apprun_ped.z, 20, 0);
					PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), true);
					UI::HIDE_HUD_COMPONENT_THIS_FRAME(1);
				}
				else {
					PLAYER::SET_POLICE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), false);
					UI::SHOW_HUD_COMPONENT_THIS_FRAME(1);
				}
			}
			else if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 2) {
				PLAYER::SET_MAX_WANTED_LEVEL(5);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
			}
		}
		else {
			if (iaminside && we_have_troubles == false) {
				PLAYER::SET_MAX_WANTED_LEVEL(5);
				PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 0, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
			}
			iaminside = false;
		}
	}
	
	// max wanted level
	if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > VEH_STARSPUNISH_VALUES[wanted_maxpossible_level]) {
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), VEH_STARSPUNISH_VALUES[wanted_maxpossible_level], 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
	}

	////////////////////////////////////// PLAYER DATA ////////////////////////////////////////////////
	
	if ((bPlayerExists && featurePlayerLife && featurePlayerLifeUpdated) || (bPlayerExists && featurePlayerLife_Died && featurePlayerLifeUpdated) ||
		(bPlayerExists && featurePlayerLife_Changed && featurePlayerLifeUpdated) || (featurePlayerStatsEnable && featurePlayerStatsUpdated)) {
		
		scr_tick_secs_passed = clock() / CLOCKS_PER_SEC;
		if (((clock() / (CLOCKS_PER_SEC / 1000)) - scr_tick_secs_curr) != 0) {
			tick = tick + 1;
			scr_tick_secs_curr = scr_tick_secs_passed;
		}
		if (tick > 200) {
			if (PLAYER_HEALTH_VALUES[current_player_health] > 0) {
				PED::SET_PED_MAX_HEALTH(playerPed, PLAYER_HEALTH_VALUES[current_player_health]);
				ENTITY::SET_ENTITY_HEALTH(playerPed, PLAYER_HEALTH_VALUES[current_player_health]);
			}
			else {
				PED::SET_PED_MAX_HEALTH(playerPed, 200);
				ENTITY::SET_ENTITY_HEALTH(playerPed, 200);
			}
			if (detained == false && in_prison == false) {
				PLAYER::SET_PLAYER_MAX_ARMOUR(playerPed, PLAYER_ARMOR_VALUES[current_player_armor]);
				PED::SET_PED_ARMOUR(playerPed, PLAYER_ARMOR_VALUES[current_player_armor]);
			}
			oldplayerPed = playerPed;
			tick = 0;
			featurePlayerLifeUpdated = false;

			if (featurePlayerStatsEnable && featurePlayerStatsUpdated && GAMEPLAY::GET_MISSION_FLAG() == 0) {

				if (PED::GET_PED_TYPE(playerPed) == 0) {
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SPECIAL_ABILITY_UNLOCKED"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STAMINA"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STRENGTH"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_LUNG_CAPACITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_WHEELIE_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_FLYING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_SHOOTING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP0_STEALTH_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
				}
				if (PED::GET_PED_TYPE(playerPed) == 1) {
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_SPECIAL_ABILITY_UNLOCKED"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STAMINA"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STRENGTH"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_LUNG_CAPACITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_WHEELIE_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_FLYING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_SHOOTING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP1_STEALTH_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
				}
				if (PED::GET_PED_TYPE(playerPed) == 2 || PED::GET_PED_TYPE(playerPed) == 3) {
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_SPECIAL_ABILITY_UNLOCKED"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STAMINA"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STRENGTH"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_LUNG_CAPACITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_WHEELIE_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_FLYING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_SHOOTING_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
					STATS::STAT_SET_INT(GAMEPLAY::GET_HASH_KEY("SP2_STEALTH_ABILITY"), PLAYER_ARMOR_VALUES[current_player_stats], true);
				}

				GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("stats_controller");

				featurePlayerStatsUpdated = false;
			}
		} // end of tick
	} 
	
	if (featurePlayerLife_Died) death_time2 = PLAYER::GET_TIME_SINCE_LAST_DEATH();
	if (((death_time2 > -1 && death_time2 < 2000) || (player_died == true && !featureNoAutoRespawn)) && featurePlayerLife_Died) {
		featurePlayerLifeUpdated = true;
		featurePlayerStatsUpdated = true;
		player_died = false;
	}
	
	if (playerPed != oldplayerPed && featurePlayerLife_Changed) { // If You Switch Character Your Health & Armor Will Be Restored
		featurePlayerLifeUpdated = true;
		featurePlayerStatsUpdated = true;
	}

	if (featurePlayerStatsEnable && GAMEPLAY::GET_MISSION_FLAG() == 1 && !SCRIPT::HAS_SCRIPT_LOADED("stats_controller")) {
		while (!SCRIPT::HAS_SCRIPT_LOADED("stats_controller")) {
			SCRIPT::REQUEST_SCRIPT("stats_controller");
			SYSTEM::WAIT(0);
		}
		SYSTEM::START_NEW_SCRIPT("stats_controller", 1424);
		
		featurePlayerStatsUpdated = true;
	}

	if (featurePlayerStatsUpdated == true && !SCRIPT::HAS_SCRIPT_LOADED("stats_controller")) {
		while (!SCRIPT::HAS_SCRIPT_LOADED("stats_controller")) {
			SCRIPT::REQUEST_SCRIPT("stats_controller");
			SYSTEM::WAIT(0);
		}
		SYSTEM::START_NEW_SCRIPT("stats_controller", 1424);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	prison_break(); ///// <--- PRISON BREAK /////
	
	most_wanted(); ///// <--- WANTED FUGITIVE /////

	// police ignore player
	if(featurePlayerIgnoredByPolice){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
		}
	}
	if(!featurePlayerIgnoredByPolice){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, false);
		}
	}

	// player special ability
	if(featurePlayerUnlimitedAbility){
		if(bPlayerExists){
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
		}
	}

	// player no noise
	if(bPlayerExists && !featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
	}
	if(featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);
	}

	// player fast swim
	if(bPlayerExists && !featurePlayerFastSwim){
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
	}
	if(featurePlayerFastSwim){
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);
	}

	// player fast run 
	if(bPlayerExists && !featurePlayerFastRun){
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
	}
	if(featurePlayerFastRun){
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);
	}

	// player super jump
	if(PLAYER_MOVEMENT_VALUES[current_player_superjump] > 0.00) { // featurePlayerSuperJump
		if(bPlayerExists){
			float CamRot = ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed);
			int p_force = PLAYER_MOVEMENT_VALUES[current_player_superjump] / 3; // 2
			float rad = 2 * 3.14 * (CamRot / 360);
			float v_x = -(sin(rad) * p_force * 10);
			float v_y = (cos(rad) * p_force * 10);
			float v_z = p_force * (CamRot * 0.2);
			if (PLAYER_MOVEMENT_VALUES[current_player_superjump] != 0.60) GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(player);
			if (PLAYER_MOVEMENT_VALUES[current_player_superjump] != 0.60 && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22) && ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed) < 1.5 && PED::IS_PED_ON_FOOT(playerPed) && !AI::IS_PED_STILL(playerPed)) super_jump_no_parachute = true;
			if ((PLAYER_MOVEMENT_VALUES[current_player_superjump] == 0.60 || PLAYER_MOVEMENT_VALUES[current_player_superjump] > 1.00) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22) && ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed) < 1.5 && 
				PED::IS_PED_ON_FOOT(playerPed) && !AI::IS_PED_STILL(playerPed)) {
				super_jump_no_parachute = true;
				if (PLAYER_MOVEMENT_VALUES[current_player_superjump] != 0.60) ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 3, v_x, v_y, PLAYER_MOVEMENT_VALUES[current_player_superjump] * 10, 0, 0, 0, true, false, true, true, true, true); // * 20
				if (PLAYER_MOVEMENT_VALUES[current_player_superjump] == 0.60) ENTITY::APPLY_FORCE_TO_ENTITY(playerPed, 3, v_x, v_y, PLAYER_MOVEMENT_VALUES[current_player_superjump] * 30, 0, 0, 0, true, false, true, true, true, true); // * 25
			}
			if (super_jump_no_parachute == true && ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed) > 0.3) {
				WEAPON::REMOVE_WEAPON_FROM_PED(playerPed, PARACHUTE_ID);
				super_jump_intheair = true;
			}
			if (super_jump_no_parachute == true && ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(playerPed) < 0.3 && super_jump_intheair == true) {
				super_jump_intheair = false;
				super_jump_no_parachute = false;
			}
		}
	}

	// No Radgoll
	if(featureNoRagdoll){
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 0);
		}
	}
	if(!featureNoRagdoll){
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 1);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 1);
		}
	}

	// Ragdoll If Shot
	if (featureRagdollIfInjured) {
		//auto addr = getScriptHandleBaseAddress(playerPed);
		//float curr_health = (*(float *)(addr + 0x280)) - 100;
		float curr_health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
		float curr_playerArmour = PED::GET_PED_ARMOUR(playerPed);

		if (!ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_OBJECT(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_PED(playerPed) && !ENTITY::HAS_ENTITY_BEEN_DAMAGED_BY_ANY_VEHICLE(playerPed) && !WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2)) {
			been_damaged_health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100; // (*(float *)(addr + 0x280)) - 100;
			been_damaged_armor = PED::GET_PED_ARMOUR(playerPed);
		}

		if ((been_damaged_health != curr_health || been_damaged_armor != curr_playerArmour)) {
			if (WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2) && !WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 1) && !PED::IS_PED_RAGDOLL(playerPed) && 
				PED::IS_PED_ON_FOOT(playerPed) && ragdoll_seconds == 0) been_damaged_by_weapon = true;
			been_damaged_health = curr_health;
			been_damaged_armor = curr_playerArmour;
		}

		if (featurePlayerInvincible) {
			Vector3 coords_bullet = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			if (GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_bullet.x, coords_bullet.y, coords_bullet.z, 1.0, 0, 0) && !PED::IS_PED_RAGDOLL(playerPed) && PED::IS_PED_ON_FOOT(playerPed) && ragdoll_seconds == 0 &&
				!PED::IS_PED_SHOOTING(playerPed)) {
				been_damaged_by_weapon = true;
				ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(playerPed); 
			}
		}
		
		if (been_damaged_by_weapon == true) {
			int time1 = (rand() % 3000 + 0); // UP MARGIN + DOWN MARGIN
			int time2 = (rand() % 3000 + 0); 
			int ragdollType = (rand() % 3 + 0); 
			int ScreamType = (rand() % 8 + 5);
			AUDIO::PLAY_PAIN(ScreamType, 0, 0);
			AUDIO::_PLAY_AMBIENT_SPEECH1(PLAYER::PLAYER_ID(), "GENERIC_SHOCKED_HIGH", "SPEECH_PARAMS_FORCE");
			if (PED::GET_PED_ARMOUR(playerPed) > 4 && (ragdollType == 2 || ragdollType == 3)) PED::SET_PED_TO_RAGDOLL(playerPed, time1, time2, ragdollType, true, true, false);
			if (PED::GET_PED_ARMOUR(playerPed) < 5 || featurePlayerInvincible) PED::SET_PED_TO_RAGDOLL(playerPed, time1, time2, ragdollType, true, true, false);
			been_damaged_by_weapon = false;
			ragdoll_task = true;
		}
		
		if (ragdoll_task == true) {
			scr_tick_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - scr_tick_secs_curr) != 0) {
				ragdoll_seconds = ragdoll_seconds + 1;
				scr_tick_secs_curr = scr_tick_secs_passed;
			}
			
			if (ragdoll_seconds == 4) {
				ragdoll_task = false;
				ragdoll_seconds = 0; 
			}
		}
	}
	 
	// NPC Ragdoll If Shot
	if ((NPC_RAGDOLL_VALUES[current_npc_ragdoll] == 1 || NPC_RAGDOLL_VALUES[current_npc_ragdoll] == 2) && GAMEPLAY::GET_MISSION_FLAG() == 0) {
		const int arrSize5 = 1024;
		Ped NPCragdoll[arrSize5];
		int count_NPC_ragdoll = worldGetAllPeds(NPCragdoll, arrSize5);

		for (int i = 0; i < count_NPC_ragdoll; i++) {
			if (NPC_RAGDOLL_VALUES[current_npc_ragdoll] == 1 && NPCragdoll[i] != playerPed) { // PED::GET_PED_TYPE(NPCragdoll[i]) != 0 && PED::GET_PED_TYPE(NPCragdoll[i]) != 1 && PED::GET_PED_TYPE(NPCragdoll[i]) != 2 && PED::GET_PED_TYPE(NPCragdoll[i]) != 3
				if (WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(NPCragdoll[i], 0, 1)) PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(NPCragdoll[i], 1);
				if (!WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(NPCragdoll[i], 0, 1)) PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(NPCragdoll[i], 1);
			}

			if (NPC_RAGDOLL_VALUES[current_npc_ragdoll] == 2 && WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(NPCragdoll[i], 0, 2) && !WEAPON::HAS_PED_BEEN_DAMAGED_BY_WEAPON(NPCragdoll[i], 0, 1) && !PED::IS_PED_RAGDOLL(NPCragdoll[i]) &&
				PED::IS_PED_ON_FOOT(NPCragdoll[i]) && NPCragdoll[i] != playerPed) { // PED::GET_PED_TYPE(NPCragdoll[i]) != 0 && PED::GET_PED_TYPE(NPCragdoll[i]) != 1 &&	PED::GET_PED_TYPE(NPCragdoll[i]) != 2 && PED::GET_PED_TYPE(NPCragdoll[i]) != 3
				int time1 = (rand() % 3000 + 0); // UP MARGIN + DOWN MARGIN
				int time2 = (rand() % 3000 + 0);
				int ragdollType = (rand() % 3 + 0);
				int ScreamType = (rand() % 8 + 5);
				AUDIO::PLAY_PAIN(ScreamType, 0, 0);
				AUDIO::_PLAY_AMBIENT_SPEECH1(NPCragdoll[i], "GENERIC_SHOCKED_HIGH", "SPEECH_PARAMS_FORCE");
				//PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(NPCragdoll[i], 1);
				PED::SET_PED_TO_RAGDOLL(NPCragdoll[i], time1, time2, ragdollType, true, true, false);
				PED::CLEAR_PED_LAST_DAMAGE_BONE(NPCragdoll[i]);
				ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(NPCragdoll[i]);
			}
		}
	}
		
	// Player Movement Speed
	if (PLAYER_MOVEMENT_VALUES[current_player_movement] > 0.00) {
		if (AI::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID())) PED::SET_PED_MOVE_RATE_OVERRIDE(playerPed, PLAYER_MOVEMENT_VALUES[current_player_movement]);
		else PED::SET_PED_MOVE_RATE_OVERRIDE(playerPed, 1.00);
	}
		
	// Health Regeneration Rate
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && REGEN_VALUES[current_regen_speed] != 1.0f) PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), REGEN_VALUES[current_regen_speed]);

	// No Scuba Gear Mask
	if (featureNoScubaGearMask && ENTITY::IS_ENTITY_IN_WATER(playerPed) == 1) {
		PED::CLEAR_ALL_PED_PROPS(playerPed);
	}

	// No Scuba Breathing Sound
	if (featureNoScubaSound) {
		AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", true);
	}
	else {
		AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", false);
	}

	// Player Invisible && Player Invisible In Vehicle
	if ((!featurePlayerInvisible && !featurePlayerInvisibleInVehicle && p_invisible == true) || (featurePlayerInvisibleInVehicle && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1) && p_invisible == true)) {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
		p_invisible = false;
	}

	if(featurePlayerInvisible || (featurePlayerInvisibleInVehicle && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1))) {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
		p_invisible = true;
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

	update_anims_features(bPlayerExists, playerPed);

	update_vehmodmenu_features(bPlayerExists, playerPed);

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

bool onconfirm_playerData_menu(MenuItem<int> choice){
	if (choice.value == -1) {
		featurePlayerLifeUpdated = true;
		featurePlayerStatsUpdated = true;
		set_status_text("Settings are being applied");
	}
	//else if (choice.value == -2) {
	//	featurePlayerStatsUpdated = true;
	//	set_status_text("Stats were changed");
	//}
	return false;
}

bool onconfirm_PlayerWantedMaxPossibleLevel_menu(MenuItem<int> choice) {

	return false;
}

bool onconfirm_PlayerMostWanted_menu(MenuItem<int> choice) {

	return false;
}

bool onconfirm_PlayerMovement_menu(MenuItem<int> choice) {

	return false;
}

bool onconfirm_NPCragdoll_menu(MenuItem<int> choice) {

	return false;
}

bool onconfirm_playerPrison_menu(MenuItem<int> choice){

	return false;
}

bool onconfirm_playerForceshield_menu(MenuItem<int> choice) {
	switch (playerForceshieldMenuIndex) {
		case 0:
			process_powerpunch_menu();
			break;
		default:
			break;
	}
	return false;
}

bool onconfirm_powerpunch_menu(MenuItem<int> choice)
{
	switch (activeLineIndexPowerPunchWeapons) {
	case 5:
	{
		if (WEAPONS_POWERPUNCH_VALUES[PowerPunchIndex] != 55) {
			std::ostringstream ss;
			ss << "~r~Warning! Enable Manual Mode To Use It";
			set_status_text(ss.str());
		}
		std::string::size_type sz;
		std::string result_p = show_keyboard(nullptr, (char *)lastPowerWeapon.c_str());
		if (!result_p.empty()) {
			if (strlen(result_p.c_str()) > 18) result_p = "9223372036854775807"; // result_p.resize(18);
			lastPowerWeapon = result_p;
		}
		break;
	}
	default:
		break;
	}
	return false;
}

void process_powerpunch_menu() {
	std::string caption = "Power Punch Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;
	MenuItem<int> *item = new MenuItem<int>();

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePowerPunch;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Fists Only";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchFists;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Melee Weapons";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchMeleeWeapons;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Firearms";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchFireWeapons;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(WEAPONS_POWERPUNCH_CAPTIONS, onchange_power_punch_index);
	listItem->wrap = false;
	listItem->caption = "Power Punch Strength";
	listItem->value = PowerPunchIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "Enter Punch Strength";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	//listItem = new SelectFromListMenuItem(PEDS_POWERPUNCH_CAPTIONS, onchange_peds_power_punch_index);
	//listItem->wrap = false;
	//listItem->caption = "Including Peds";
	//listItem->value = PedsPowerPunchIndex;
	//menuItems.push_back(listItem);

	//toggleItem = new ToggleMenuItem<int>();
	//toggleItem->caption = "Peds Can Power Punch Peds";
	//toggleItem->value = i++;
	//toggleItem->toggleValue = &featurepowerpunchpeds;
	//menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexPowerPunchWeapons, caption, onconfirm_powerpunch_menu, NULL, NULL);
}

bool process_player_life_menu(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Player Data";
	
	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

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
	
	listItem = new SelectFromListMenuItem(REGEN_CAPTIONS, onchange_regen_callback);
	listItem->wrap = false;
	listItem->caption = "Health Regeneration Rate";
	listItem->value = current_regen_speed;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Toggle Modified Stats";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerStatsEnable;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(PLAYER_ARMOR_CAPTIONS, onchange_player_stats_mode);
	listItem->wrap = false;
	listItem->caption = "Character Ability Stats";
	listItem->value = current_player_stats;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &playerDataMenuIndex, caption, onconfirm_playerData_menu, NULL, NULL);
}

bool maxwantedlevel_menu() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Wanted Level Options";

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Freeze Wanted Level";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelFrozen;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_wanted_maxpossible_level_mode);
	listItem->wrap = false;
	listItem->caption = "Max Wanted Level";
	listItem->value = wanted_maxpossible_level;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Police Ignore You";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerIgnoredByPolice;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Police Helicopters";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoPHeli;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Police Boats";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoPBoats;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Road Blocks";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedNoPRoadB;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No SWAT Vehicles";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoSWATVehicles;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &PlayerWantedMaxPossibleLevelMenuIndex, caption, onconfirm_PlayerWantedMaxPossibleLevel_menu, NULL, NULL);
}

bool mostwanted_menu() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Wanted Fugitive Options";

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerMostWanted;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_mostwanted_level_mode);
	listItem->wrap = false;
	listItem->caption = "Wanted Status At";
	listItem->value = mostwanted_level_enable;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_mostwanted_mode);
	listItem->wrap = false;
	listItem->caption = "Wanted Stars If Seen";
	listItem->value = current_player_mostwanted;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Disable Player Switching While Fugitive";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerNoSwitch;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &PlayerMostWantedMenuIndex, caption, onconfirm_PlayerMostWanted_menu, NULL, NULL);
}

bool player_movement_speed() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Player Movement Speed Options";

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Fast Swim";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerFastSwim;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Fast Run";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerFastRun;
	menuItems.push_back(toggleItem);
	
	//toggleItem = new ToggleMenuItem<int>();
	//toggleItem->caption = "Super Jump";
	//toggleItem->value = i++;
	//toggleItem->toggleValue = &featurePlayerSuperJump;
	//menuItems.push_back(toggleItem);
	
	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_superjump_mode);
	listItem->wrap = false;
	listItem->caption = "Super Jump";
	listItem->value = current_player_superjump;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_jumpfly_mode);
	listItem->wrap = false;
	listItem->caption = "Hancock Mode";
	listItem->value = current_player_jumpfly;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_movement_mode);
	listItem->wrap = false;
	listItem->caption = "Movement Speed:";
	listItem->value = current_player_movement; 
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &PlayerMovementMenuIndex, caption, onconfirm_PlayerMovement_menu, NULL, NULL);
}

bool process_ragdoll_menu() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Ragdoll Options";

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Ragdoll";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoRagdoll;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Ragdoll If Shot";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRagdollIfInjured;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(NPC_RAGDOLL_CAPTIONS, onchange_NPC_ragdoll_mode);
	listItem->wrap = false;
	listItem->caption = "NPC Ragdoll If Shot";
	listItem->value = current_npc_ragdoll;
	menuItems.push_back(listItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Limp If Injured";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerInjuredMovement;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &NPCragdollMenuIndex, caption, onconfirm_NPCragdoll_menu, NULL, NULL);
}

bool process_player_prison_menu(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Prison Break Options";

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

	listItem = new SelectFromListMenuItem(PLAYER_ESCAPESTARS_CAPTIONS, onchange_player_escapestars_mode);
	listItem->wrap = false;
	listItem->caption = "Number Of Stars After Escaping";
	listItem->value = current_escape_stars;
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
	toggleItem->caption = "Peds React To Prison Clothes";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePedPrison_Robe;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "More Prisoners At The Yard";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Yard;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &playerPrisonMenuIndex, caption, onconfirm_playerPrison_menu, NULL, NULL);
}

bool process_player_forceshield_menu() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::vector<MenuItem<int> *> menuItems;
	std::string caption = "Jedi Powers Options";

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;
	MenuItem<int> *item;

	int i = 0;
	item = new MenuItem<int>();
	item->caption = "Power Punch";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_MASS_CAPTIONS, onchange_player_forceshield_mode);
	listItem->wrap = false;
	listItem->caption = "Player Force Shield";
	listItem->value = current_player_forceshield;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Levitation";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureLevitation;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &playerForceshieldMenuIndex, caption, onconfirm_playerForceshield_menu, NULL, NULL);
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
		case 5:
			maxwantedlevel_menu();
			break;
		case 6:
			mostwanted_menu();
			break;
		case 10:
			player_movement_speed();
			break;
		case 11:
			process_ragdoll_menu();
			break;
		case 17:
			process_anims_menu_top();
			break;
		case 18:
			process_player_life_menu();
			break;
		case 19:
			process_player_prison_menu();
			break;
		case 20:
			process_player_forceshield_menu();
			break;
		default:
			break;
	}

	return false;
}

void process_player_menu(){
	const int lineCount = 27;

	std::string caption = "Player Options";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"Player Appearance", NULL, NULL, false},
		{"Heal Player", NULL, NULL, true},
		{"Invincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated, true},
		{"Add or Remove Cash", NULL, NULL, true, CASH},
		{"Wanted Level", NULL, NULL, true, WANTED},
		{"Wanted Level Settings", NULL, NULL, false},
		{"Wanted Fugitive", NULL, NULL, false},
		{"Unlimited Ability", &featurePlayerUnlimitedAbility, NULL, true},
		{"Noiseless", &featurePlayerNoNoise, NULL, true}, 
		{"Can Run In Apartments", &featurePlayerRunApartments, NULL, true},
		{"Player Movement Speed", NULL, NULL, false},
		{"Ragdoll", NULL, NULL, false},
		{"Invisibility", &featurePlayerInvisible, NULL, true}, 
		{"Invisibility In Vehicle", &featurePlayerInvisibleInVehicle, NULL, true }, 
		{"Drunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated, true},
		{"Night Vision", &featureNightVision, &featureNightVisionUpdated, true},
		{"Thermal Vision", &featureThermalVision, &featureThermalVisionUpdated, true},
		{"Animations", NULL, NULL, false},
		{"Player Data", NULL, NULL, false},
		{"Prison Break", NULL, NULL, false},
		{"Jedi Powers", NULL, NULL, false},
		{"No Whistling For Taxi", &NoTaxiWhistling, NULL, false},
		{"Player Can Be Headshot", &featurePlayerCanBeHeadshot, NULL, false},
		{"Manual Respawn", &featureNoAutoRespawn, NULL },
		{"Instant Respawn On Death/Arrest", &featureRespawnsWhereDied, NULL, false},
		{"No Scuba Gear Mask", &featureNoScubaGearMask, NULL, true },
		{"No Scuba Breathing Sound", &featureNoScubaSound, NULL, true },
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

	reset_time_globals();

	reset_vehmodmenu_globals();

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
	
	current_player_health = 6;
	current_regen_speed = 4;
	current_player_armor = 6;
	current_player_stats = 6;
	current_npc_ragdoll = 0;
	current_player_movement = 0;
	current_player_jumpfly = 0;
	current_player_superjump = 0;
	current_player_mostwanted = 0;
	mostwanted_level_enable = 0;
	wanted_maxpossible_level = 3;
	current_player_prison = 0;
	current_player_escapemoney = 4;
	current_player_discharge = 3;
	current_escape_stars = 2;

	featurePlayerDrunk =
		featurePlayerInvincible =
		featurePlayerIgnoredByPolice =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerMostWanted =
		featurePlayerFastSwim =
		featurePlayerFastRun =
		featurePlayerRunApartments =
		//featurePlayerSuperJump =
		featurePlayerInvisible =
		featurePlayerInvisibleInVehicle =
		featureNightVision =
		featureThermalVision =

		featurePlayerLife =
		featurePlayerLife_Died = 
		featurePlayerStatsEnable =
		featurePlayerLife_Changed =
		featurePrison_Hardcore =
		featureNoRagdoll =
		featureRagdollIfInjured =
		featureLevitation =

		featureWantedLevelNoPHeli =
		featureWantedNoPRoadB =
		featureWantedLevelNoPBoats =
		featureWantedLevelNoSWATVehicles =
		NoTaxiWhistling =
		featurePlayerCanBeHeadshot =
		featureRespawnsWhereDied =
		featurePlayerInjuredMovement =
		featureNoScubaSound =
		featureNoScubaGearMask =

		featureWantedLevelFrozen = false;
		featurePlayerInvincibleUpdated =
		featurePlayerDrunkUpdated =
		featureNightVisionUpdated =
		featureThermalVisionUpdated =
		featurePlayerLifeUpdated =
		featurePrison_Robe =
		featurePedPrison_Robe =
		featurePrison_Yard = 
		featurePlayerStatsUpdated =

		featurePlayerNoSwitch = true;
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

	if (featureShowStatusMessage) set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ ver. ~HUD_COLOUR_MENU_YELLOW~" + VERSION_STRING + "~HUD_COLOUR_WHITE~."); // ~HUD_COLOUR_WHITE~ is ready //  bugfix 5 //  is ready
	
	while(true){
		if(trainer_switch_pressed()){
			menu_beep();
			set_menu_showing(true);
			process_main_menu();
			set_menu_showing(false);
		}
		else if(airbrake_switch_pressed()){
			menu_beep();
			if (airbrake_enable) process_airbrake_menu();
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

		if (PLAYER::IS_PLAYER_PLAYING(PLAYER::PLAYER_ID()))
		{
			init_xinput(); //Init it after loading to prevent loading screen hangs
			write_text_to_log_file("Init XINPUT complete");
		}
		
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

		//Find the radio skip patterns
		SInit();

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
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerIgnoredByPolice", &featurePlayerIgnoredByPolice}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoPHeli", &featureWantedLevelNoPHeli});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedNoPRoadB", &featureWantedNoPRoadB});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoPBoats", &featureWantedLevelNoPBoats});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoSWATVehicles", &featureWantedLevelNoSWATVehicles});
	results->push_back(FeatureEnabledLocalDefinition{"NoTaxiWhistling", &NoTaxiWhistling});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoScubaGearMask", &featureNoScubaGearMask});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoScubaSound", &featureNoScubaSound});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerCanBeHeadshot", &featurePlayerCanBeHeadshot});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInjuredMovement", &featurePlayerInjuredMovement});
	results->push_back(FeatureEnabledLocalDefinition{"featureRespawnsWhereDied", &featureRespawnsWhereDied});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerUnlimitedAbility", &featurePlayerUnlimitedAbility});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerNoNoise", &featurePlayerNoNoise}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastSwim", &featurePlayerFastSwim}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerMostWanted", &featurePlayerMostWanted});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerNoSwitch", &featurePlayerNoSwitch});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastRun", &featurePlayerFastRun}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerRunApartments", &featurePlayerRunApartments});
	//results->push_back(FeatureEnabledLocalDefinition{"featurePlayerSuperJump", &featurePlayerSuperJump});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoRagdoll", &featureNoRagdoll}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureRagdollIfInjured", &featureRagdollIfInjured}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisible", &featurePlayerInvisible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisibleInVehicle", &featurePlayerInvisibleInVehicle}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerDrunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureNightVision", &featureNightVision, &featureNightVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureThermalVision", &featureThermalVision, &featureThermalVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife", &featurePlayerLife, &featurePlayerLifeUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife_Died", &featurePlayerLife_Died});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerStatsEnable", &featurePlayerStatsEnable});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife_Changed", &featurePlayerLife_Changed});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Hardcore", &featurePrison_Hardcore});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Robe", &featurePrison_Robe});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedPrison_Robe", &featurePedPrison_Robe});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Yard", &featurePrison_Yard});
	results->push_back(FeatureEnabledLocalDefinition{"featureLevitation", &featureLevitation});
}

void add_world_feature_enablements3(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{"current_player_health", std::to_string(current_player_health)});
	results->push_back(StringPairSettingDBRow{"current_regen_speed", std::to_string(current_regen_speed)});
	results->push_back(StringPairSettingDBRow{"current_player_armor", std::to_string(current_player_armor)});
	results->push_back(StringPairSettingDBRow{"current_player_stats", std::to_string(current_player_stats)});
	results->push_back(StringPairSettingDBRow{"current_npc_ragdoll", std::to_string(current_npc_ragdoll)});
	results->push_back(StringPairSettingDBRow{"current_player_movement", std::to_string(current_player_movement)});
	results->push_back(StringPairSettingDBRow{"current_player_jumpfly", std::to_string(current_player_jumpfly)});
	results->push_back(StringPairSettingDBRow{"current_player_superjump", std::to_string(current_player_superjump)});
	results->push_back(StringPairSettingDBRow{"current_player_mostwanted", std::to_string(current_player_mostwanted)});
	results->push_back(StringPairSettingDBRow{"mostwanted_level_enable", std::to_string(mostwanted_level_enable)});
	results->push_back(StringPairSettingDBRow{"wanted_maxpossible_level", std::to_string(wanted_maxpossible_level)});
	results->push_back(StringPairSettingDBRow{"current_player_prison", std::to_string(current_player_prison)});
	results->push_back(StringPairSettingDBRow{"current_player_escapemoney", std::to_string(current_player_escapemoney)});
	results->push_back(StringPairSettingDBRow{"current_player_discharge", std::to_string(current_player_discharge)});
	results->push_back(StringPairSettingDBRow{"current_escape_stars", std::to_string(current_escape_stars)});
}

std::vector<FeatureEnabledLocalDefinition> get_feature_enablements(){
	std::vector<FeatureEnabledLocalDefinition> results;

	add_player_feature_enablements(&results);

	add_world_feature_enablements(&results);

	add_time_feature_enablements(&results);

	add_misc_feature_enablements(&results);

	add_player_skin_feature_enablements(&results);

	add_props_feature_enablements(&results);

	add_vehicle_feature_enablements(&results);

	add_vehmodmenu_feature_enablements(&results);

	add_teleporter_feature_enablements(&results);

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
		else if (setting.name.compare("current_regen_speed") == 0) {
			current_regen_speed = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_armor") == 0){
			current_player_armor = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_stats") == 0) {
			current_player_stats = stoi(setting.value);
		}
		else if (setting.name.compare("current_npc_ragdoll") == 0) {
			current_npc_ragdoll = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_movement") == 0) {
			current_player_movement = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_jumpfly") == 0) {
			current_player_jumpfly = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_superjump") == 0) {
			current_player_superjump = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_mostwanted") == 0) {
			current_player_mostwanted = stoi(setting.value);
		}
		else if (setting.name.compare("mostwanted_level_enable") == 0) {
			mostwanted_level_enable = stoi(setting.value);
		}
		else if (setting.name.compare("wanted_maxpossible_level") == 0) {
			wanted_maxpossible_level = stoi(setting.value);
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
		else if (setting.name.compare("current_escape_stars") == 0) {
			current_escape_stars = stoi(setting.value);
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
