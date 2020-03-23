#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "../features/vehicles.h"
#include <vector>
#include "../features/vehicle_weapons.h"
#include "../../inc/natives.h"

//warning C4129 : 'D' : unrecognized character escape sequence - Attempting to properly "escape" the string breaks the sound library
#pragma warning( disable : 4129 )

Camera bombCam = NULL;
Vehicle veh_b;
Vector3 vehPosition;
Vector3 curRotation;

bool bombDoorOpen;

bool tempbombDoor = false;

Object oBomb;

std::vector<Object> vBomb;

bool featureBombDoorCamera = false;

void toggle_bomb_bay_camera()
{
	veh_b = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	vehPosition = ENTITY::GET_ENTITY_COORDS(veh_b, true);
	curRotation = ENTITY::GET_ENTITY_ROTATION(veh_b, 2);
		
	if (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01"))) STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01"));
	
	if (featureBombDoorCamera) 
	{
		if (bombDoorOpen == true && !CAM::DOES_CAM_EXIST(bombCam)) {
			CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);
			bombCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", vehPosition.x, vehPosition.y, vehPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
			CAM::ATTACH_CAM_TO_ENTITY(bombCam, veh_b, 0.0f, 0.0f, -0.5f, 1);
			CAM::POINT_CAM_AT_COORD(bombCam, vehPosition.x, vehPosition.y, vehPosition.z - 2);
			CAM::SHAKE_CAM(bombCam, "ROAD_VIBRATION_SHAKE", 0.4f);
			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(bombCam, 1.0);
			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(bombCam, 1.0);
			CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(bombCam, 1.0);
			CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
			CAM::SET_CAM_ACTIVE(bombCam, true);
			UI::DISPLAY_RADAR(false);
			UI::DISPLAY_HUD(false);
		}
		if (CAM::DOES_CAM_EXIST(bombCam)) {
			CAM::SET_CAM_ROT(bombCam, curRotation.x, curRotation.y, curRotation.z, 2);
			CAM::POINT_CAM_AT_COORD(bombCam, vehPosition.x, vehPosition.y, vehPosition.z - 2);
		}

		if ((bombDoorOpen == false || !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) && CAM::DOES_CAM_EXIST(bombCam))
		{
			CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
			CAM::DETACH_CAM(bombCam);
			CAM::SET_CAM_ACTIVE(bombCam, false);
			CAM::DESTROY_CAM(bombCam, true);
			UI::DISPLAY_RADAR(true);
			UI::DISPLAY_HUD(true);
			CONTROLS::ENABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);
		}
	}
}

void toggle_bomb_bay_doors()
{
	Vehicle veh_b = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (bombDoorOpen) {
		VEHICLE::CLOSE_BOMB_BAY_DOORS(veh_b);
		ENTITY::DELETE_ENTITY(&oBomb);
	}
	else {
		VEHICLE::OPEN_BOMB_BAY_DOORS(veh_b);
		oBomb = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01"), vehPosition.x, vehPosition.y, vehPosition.z - 0.5, false, false, false);
		ENTITY::SET_ENTITY_ROTATION(oBomb, curRotation.x, curRotation.y, curRotation.z, 0, false);
		if (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("bodyshell"))) STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("bodyshell"));
		int boneIndex1 = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh_b, "bodyshell");
		ENTITY::ATTACH_ENTITY_TO_ENTITY(oBomb, veh_b, boneIndex1, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, false, false, false, true, 0, true);
	}

	AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
	AUDIO::PLAY_SOUND_FRONTEND(-1, "DRUG_TRAFFIC_AIR_BAY_DOOR_OPEN_MASTER", 0, 1);

	bombDoorOpen = !bombDoorOpen;
}

void start_bombing_run()	
{
	Hash currVehModel = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) set_status_text("Player isn't in a vehicle");
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && GAMEPLAY::GET_HASH_KEY("CUBAN800") == currVehModel && bombDoorOpen == false) set_status_text("Bomb-Door is closed");
		
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && GAMEPLAY::GET_HASH_KEY("CUBAN800") == currVehModel && bombDoorOpen == true) {
		Vehicle veh_b = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

		ENTITY::DETACH_ENTITY(oBomb, true, true);
		ROPE::ACTIVATE_PHYSICS(oBomb);

		if (!vBomb.empty()) vBomb.push_back(oBomb);
		else vBomb.push_back(oBomb);

		oBomb = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01"), vehPosition.x, vehPosition.y, vehPosition.z - 0.5, false, false, false);
		ENTITY::SET_ENTITY_ROTATION(oBomb, curRotation.x, curRotation.y, curRotation.z, 0, false);
		if (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("bodyshell"))) STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("bodyshell"));
		int boneIndex1 = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh_b, "bodyshell");
		ENTITY::ATTACH_ENTITY_TO_ENTITY(oBomb, veh_b, boneIndex1, 0.0, 0.0, -0.5, 0.0, 0.0, 0.0, false, false, false, true, 0, true);
	}
}

void update_bombs()
{
	if (!vBomb.empty()) {
		for (int i = 0; i < vBomb.size(); i++) {
			Vector3 bomb_coords = ENTITY::GET_ENTITY_COORDS(vBomb[i], 1);

			Vector3 CamRot = ENTITY::GET_ENTITY_ROTATION(veh_b, 2);
			float cur_s = ENTITY::GET_ENTITY_SPEED(veh_b);
			int p_force = 1 * cur_s; // 50
			float rad = 2 * 3.14 * (CamRot.z / 360);
			float v_x = -(sin(rad) * p_force);
			float v_y = (cos(rad) * p_force);
			ENTITY::APPLY_FORCE_TO_ENTITY(vBomb[i], 1, v_x, v_y, -(cur_s * 100), 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1); // oBomb

			if (!ENTITY::HAS_COLLISION_LOADED_AROUND_ENTITY(vBomb[i]))
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(bomb_coords.x, bomb_coords.y, bomb_coords.z);
			}

			if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(vBomb[i]))
			{
				play_explosion(vBomb[i]);
				ENTITY::DELETE_ENTITY(&vBomb[i]);
				vBomb.erase(vBomb.begin() + i);
				continue;
			}
		}
	}
}


void play_explosion(Entity ent)
{
	//https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/CarpetBomb.cs#L167
	Vector3 ent_pos = ENTITY::GET_ENTITY_COORDS(ent, true);

	if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_oddjobtraffickingair"))
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_oddjobtraffickingair");

	if (ENTITY::IS_ENTITY_IN_WATER(ent)) {
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_ojdg4_water_exp", ent_pos.x, ent_pos.y, ent_pos.z, 0.0, 0.0, 0.0, 35.0, 0, 0, 0); // 5.0
	}
	else {
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_drug_grd_train_exp", ent_pos.x, ent_pos.y, ent_pos.z, 0.0, 0.0, 0.0, 35.0, 0, 0, 0); // 5.0
	}

	FIRE::ADD_EXPLOSION(ent_pos.x, ent_pos.y, ent_pos.z, ExplosionTypeTrain, 3500.0f, 1, 0, 0.5);
}

bool onconfirm_veh_weapons_menu(MenuItem<int> choice){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
	if (!bPlayerExists){
		return true;
	}
		
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		set_status_text("Player isn't in a vehicle");
		return true;
	}

	if (choice.value == -1) {
		toggle_bomb_bay_doors();
	}
	else if (choice.value == -2) {
		if (bombDoorOpen == true) start_bombing_run();
		else set_status_text("Bomb door closed");
	}
	return false;
}

bool process_veh_weapons_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;
	int i = 0;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable Bomb-Door Camera";
	toggleItem->toggleValue = &featureBombDoorCamera;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Open/Close Bomb Bay";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Drop Bomb";
	item->value = -2;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, 0, "Plane Bomb Options", onconfirm_veh_weapons_menu, NULL, NULL, vehicle_menu_interrupt);
}

void reset_veh_weapons_globals()
{
	featureBombDoorCamera = false;
}

void update_veh_weapons_features()
{
	toggle_bomb_bay_camera();
	update_bombs();
	
	if (!featureBombDoorCamera && CAM::DOES_CAM_EXIST(bombCam))	{
		CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
		CAM::DETACH_CAM(bombCam);
		CAM::SET_CAM_ACTIVE(bombCam, false);
		CAM::DESTROY_CAM(bombCam, true);
		UI::DISPLAY_RADAR(true);
		UI::DISPLAY_HUD(true);
		CONTROLS::ENABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);
	}

	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && tempbombDoor == false) {
		VEHICLE::CLOSE_BOMB_BAY_DOORS(veh_b);
		tempbombDoor = true;
		
	}
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
		tempbombDoor = false;
		bombDoorOpen = false;
	}
}

void add_vehicle_weapons_enablements(std::vector<FeatureEnabledLocalDefinition>* results) {
	results->push_back(FeatureEnabledLocalDefinition{ "featureBombDoorCamera", &featureBombDoorCamera });
}

//Main - https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/MainScript.cs


/*// THE ORIGINAL CODE BY ZEMANEZ

Camera bombCam = NULL;

bool bombDoorOpen;

int bomb_drop_timer = GetTickCount();
//int bomb_count = 0; <- uncomment if Bomb Vector errors out!
int bombs_dropped = 0;

Hash bomb_sound_id = -1;
Object oBomb;

std::vector<Object> vBomb;

bool featureBombDoorCamera = false;
bool featureAutoPilot = false;
bool featureAutoPilotUpdated = false;


Object create_bomb(Vector3 position, Vector3 rotation)
{
	Hash mBomb = GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01");

	if (STREAMING::IS_MODEL_IN_CDIMAGE(mBomb))
	{
		STREAMING::REQUEST_MODEL(mBomb);
		while (!STREAMING::HAS_MODEL_LOADED(mBomb))
		{
			WAIT(0);
		}
	}

	oBomb = OBJECT::CREATE_OBJECT(mBomb, position.x, position.y, position.z, false, false, false);
	ENTITY::SET_ENTITY_RECORDS_COLLISIONS(oBomb, true);
	ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(oBomb, true);
	ENTITY::SET_ENTITY_LOD_DIST(oBomb, 1000);
	ENTITY::SET_ENTITY_ROTATION(oBomb, rotation.x, rotation.y, rotation.z, 0, false);
	//bomb_count++;

	return oBomb;
}

void toggle_bomb_bay_camera()
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (bombCam == NULL)
	{
		// Disable the weapon controls to prevent the player from breaking the bomb spawner 
		CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);

		// Get the bone index for the bomb-bay door(s) for attaching the camera to 
		int boneIndex = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh, "chassis_dummy");

		// Get bone coord for attaching cam to 
		Vector3 boneCoords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh, boneIndex);

		// Get cam offset from world coords so the camera is a fixed distance (and facing the correct direction) from the bomb door 
		Vector3 camOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, boneCoords.x, boneCoords.y, boneCoords.z - 1.3f);

		// Create the camera to be attached to the bomb-bay doors - "0x19286a9" is from the scripts (launcher_basejumppack.c4) 
		bombCam = CAM::CREATE_CAMERA_WITH_PARAMS(0x19286a9, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 65.0f, 0, 2);
		CAM::ATTACH_CAM_TO_ENTITY(bombCam, veh, 0.0f, 1.3324f, -0.9f, 1);
		CAM::POINT_CAM_AT_COORD(veh, camOffset.x, camOffset.y, camOffset.z);

		// Not really necessary but gives it a gentle "turbulence" shake 
		CAM::SHAKE_CAM(bombCam, "ROAD_VIBRATION_SHAKE", 0.4f);

		// Needed else it'll draw more than the bomb-bay doors which is not what we want 
		CAM::SET_CAM_FOV(bombCam, 65.0f);

		CAM::SET_CAM_ACTIVE(bombCam, true);
		CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0);
		UI::DISPLAY_RADAR(false);
		UI::DISPLAY_HUD(false);
	}
	else if (CAM::DOES_CAM_EXIST(bombCam))
	{
		// Cleanup routine 
		CAM::SET_CAM_ACTIVE(bombCam, false);
		CAM::DESTROY_CAM(bombCam, true);
		bombCam = NULL;
		UI::DISPLAY_RADAR(true);
		UI::DISPLAY_HUD(true);
		CONTROLS::ENABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);
	}
}

void toggle_bomb_bay_doors()
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (bombDoorOpen)
		VEHICLE::CLOSE_BOMB_BAY_DOORS(veh);
	else
		VEHICLE::OPEN_BOMB_BAY_DOORS(veh);

	// Wait for doors to open/close before starting/stopping bomb drop 
	WAIT(20);

	// Not needed but nice to have  - came out of scripts
	AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
	AUDIO::PLAY_SOUND_FRONTEND(-1, "DRUG_TRAFFIC_AIR_BAY_DOOR_OPEN_MASTER", 0, 1);

	bombDoorOpen ^= bombDoorOpen;
}

// Needed to prevent the plane from crashing during bomb-cam since the control(s) are disabled 
void plane_autopilot(bool enabled)
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	if (enabled)
	{
		if (!UI::IS_WAYPOINT_ACTIVE())
		{
			set_status_text("Waypoint not found. Please set a waypoint to use Autopilot!");
			return;
		}

		Blip wayPointBlip = UI::GET_FIRST_BLIP_INFO_ID(8);
		Vector3 wayPointCoord = UI::GET_BLIP_COORDS(wayPointBlip);
		AI::TASK_VEHICLE_DRIVE_TO_COORD(PLAYER::PLAYER_PED_ID(), veh, wayPointCoord.x, wayPointCoord.y, wayPointCoord.z + 150, 40.0f, 1, veh, 262144, -1.0, -1.0);
	}
	else
	{
		AI::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
	}
}

void start_bombing_run()
{
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	// Have to make sure the vehicle is upright, actually moving and isn't in the water else this will fail 
	if (!VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(veh) || ENTITY::GET_ENTITY_SPEED(veh) > 0.0f || !ENTITY::IS_ENTITY_IN_WATER(veh) && bomb_drop_timer > 0)
	{
		// Make sure we've not got any leftovers from any previous bombing runs else it'll get very messy 
		cleanup_active_bomb();
		cleanup_active_sounds();

		// Not needed but nice to have  - came out of scripts
		AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
		bomb_sound_id = AUDIO::GET_SOUND_ID();
		AUDIO::PLAY_SOUND_FRONTEND(bomb_sound_id, "DRUG_TRAFFIC_AIR_BOMB_CAM_MASTER", 0, 1);

		// Reset timer
		bomb_drop_timer = GetTickCount();
	}
}

void add_bomb()
{
	Vector3 prev_rot;
	Vector3 rot;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	if (vBomb.size() < 1) //bomb_count < 1
	{
		// Set up bomb offset to vehicle 
		rot = { 80.0f, 0.0f, ENTITY::GET_ENTITY_HEADING(veh) + 180.0f };

		// Get vehicle offset
		Vector3 veh_coords = ENTITY::GET_ENTITY_COORDS(veh, true);

		// Create a bomb model 
		oBomb = create_bomb(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, veh_coords.x, veh_coords.y, veh_coords.z), rot);

		// Store bomb in our vector so we can keep track of how many are there. Could also use it to "find" the previous bomb's coords/rotation to tweak other bombs but no idea how to do that at the moment 
		vBomb.push_back(oBomb);
		//bomb_count++;

		prev_rot = rot;
	}
	else // Set the rotation of bombs following the first one to all be facing the same direction (down towards the ground) when they're spawned in/dropped 
	{
		if (prev_rot.x < 79.1)
		{
			prev_rot.x += 4.0f;

			if (prev_rot.x > 79.1)
				prev_rot.x = 79.1;

			rot = prev_rot;
		}

		// Set up bomb offset to vehicle 
		rot = { 80.0f, 0.0f, ENTITY::GET_ENTITY_HEADING(veh) + 180.0f };

		// Get vehicle offset
		Vector3 veh_coords = ENTITY::GET_ENTITY_COORDS(veh, true);

		// Create a bomb model 
		oBomb = create_bomb(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, veh_coords.x, veh_coords.y, veh_coords.z), rot);

		// Add new bomb into vector for later (may result in errors. If so, comment out and revert back to increment code below it) 
		vBomb.push_back(oBomb);
		//bomb_count++;
	}
}

// Cleanup unused models after we're finished 
void cleanup_active_bomb()
{
	if (vBomb.size() > 1)
	{
		for (int i = 0; i < vBomb.size(); i++)
		{
			ENTITY::DELETE_ENTITY(&vBomb[i]);
		}

		//ENTITY::DELETE_ENTITY(&oBomb);
		//bomb_count--;
	}
}

// Cleanup + release requested sound so it doesn't keep playing when we're done + releases some unneeded memory 
void cleanup_active_sounds()
{
	if (bomb_sound_id != -1)
	{
		AUDIO::STOP_SOUND(bomb_sound_id);
		AUDIO::RELEASE_SOUND_ID(bomb_sound_id);
		bomb_sound_id = -1;
	}
}

void update_bombs()
{
	//https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/CarpetBomb.cs#L112
	int timer = 0;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	Vector3 veh_coords = ENTITY::GET_ENTITY_COORDS(veh, true);
	Vector3 veh_world_offset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, veh_coords.x, veh_coords.y, veh_coords.z);


	if (bomb_drop_timer > 0 && timer < 1000) //No idea if this will work
	{
		if (bombs_dropped < 10)
		{
			add_bomb();
			bombs_dropped++;
		}

		if (bombs_dropped == 10)
		{
			//vBomb[vBomb.size() - 1];
			cleanup_active_bomb();
		}
		timer++;
	}
	else
	{
		timer = 0;
		bombs_dropped = 0;
	}

	for (int i = 0; i < vBomb.size(); i++)
	{
		Vector3 bomb_coords = ENTITY::GET_ENTITY_COORDS(vBomb[i], 1);

		if (!ENTITY::HAS_COLLISION_LOADED_AROUND_ENTITY(vBomb[i]))
		{
			STREAMING::REQUEST_COLLISION_AT_COORD(bomb_coords.x, bomb_coords.y, bomb_coords.z);
		}

		if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(vBomb[i]))
		{
			cleanup_active_bomb();

			if (bomb_coords.z > veh_world_offset.z)
			{
				play_explosion(vBomb[i]);
			}

			//Remove bomb from Vector
			vBomb.erase(vBomb.begin() + i);
			continue;
		}

	}
}


void play_explosion(Entity ent)
{
	//https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/CarpetBomb.cs#L167
	Vector3 ent_pos = ENTITY::GET_ENTITY_COORDS(ent, true);

	if (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_oddjobtraffickingair"))
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_oddjobtraffickingair");

	if (ENTITY::IS_ENTITY_IN_WATER(ent))
	{
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_ojdg4_water_exp", ent_pos.x, ent_pos.y, ent_pos.z, 0.0, 0.0, 0.0, 3.0, 0, 0, 0);
	}
	else
	{
		GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_drug_grd_train_exp", ent_pos.x, ent_pos.y, ent_pos.z, 0.0, 0.0, 0.0, 3.0, 0, 0, 0);
	}

	// So it makes a "cloud" explosion (also hides the bomb being deleted since the AoE is so big
	FIRE::ADD_EXPLOSION(ent_pos.x, ent_pos.y, ent_pos.z, ExplosionTypeTrain, 1.0, 1, 0, 0);
	WAIT(20);
	FIRE::ADD_EXPLOSION(ent_pos.x, ent_pos.y * 3, ent_pos.z, ExplosionTypeTruck, 30.0, 1, 0, 1.5);
	WAIT(20);
	FIRE::ADD_EXPLOSION(ent_pos.x, ent_pos.y, ent_pos.z * 3, ExplosionTypeTrain, 30.0, 1, 0, 1.5);
	WAIT(20);
	FIRE::ADD_EXPLOSION(ent_pos.x * 3, ent_pos.y, ent_pos.z, ExplosionTypeTruck, 30.0, 1, 0, 1.5);
}

bool onconfirm_veh_weapons_menu(MenuItem<int> choice) {
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();


	if (!bPlayerExists) {
		return true;
	}


	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("Player isn't in a vehicle");
		return true;
	}
	return false;
}

bool process_veh_weapons_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable Bomb-Door Camera";
	toggleItem->toggleValue = &featureBombDoorCamera;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable Auto-Pilot";
	toggleItem->toggleValue = &featureAutoPilot;
	toggleItem->toggleValueUpdated = &featureAutoPilotUpdated;
	menuItems.push_back(toggleItem);

	MenuItem<int> *chooseTrimColor = new MenuItem<int>();
	chooseTrimColor->caption = "Trim Color ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseTrimColor->value = -2;
	chooseTrimColor->isLeaf = false;
	menuItems.push_back(chooseTrimColor);

	return draw_generic_menu<int>(menuItems, 0, "Plane Bomb Options", onconfirm_veh_weapons_menu, NULL, NULL, vehicle_menu_interrupt);
}

void reset_veh_weapons_globals()
{
	featureBombDoorCamera = false;
	featureAutoPilot = false;

	featureAutoPilotUpdated = false;
}

void update_veh_weapons_features()
{
	if (!featureBombDoorCamera)
	{
		toggle_bomb_bay_camera();
	}

	if (featureAutoPilotUpdated)
	{
		if (featureAutoPilot)
		{
			plane_autopilot(featureAutoPilot);
		}
		else
		{
			plane_autopilot(featureAutoPilot);
		}
		featureAutoPilotUpdated = false;
	}

}*/
