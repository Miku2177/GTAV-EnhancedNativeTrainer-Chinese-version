#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "../features/vehicles.h"
#include <vector>
#include "../features/vehicle_weapons.h"
#include "../../inc/natives.h"

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
		/* Disable the weapon controls to prevent the player from breaking the bomb spawner */
		CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);

		/* Get the bone index for the bomb-bay door(s) for attaching the camera to */
		int boneIndex = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh, "chassis_dummy");

		/* Get bone coord for attaching cam to */
		Vector3 boneCoords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh, boneIndex);

		/* Get cam offset from world coords so the camera is a fixed distance (and facing the correct direction) from the bomb door */
		Vector3 camOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, boneCoords.x, boneCoords.y, boneCoords.z - 1.3f);

		/* Create the camera to be attached to the bomb-bay doors - "0x19286a9" is from the scripts (launcher_basejumppack.c4) */
		bombCam = CAM::CREATE_CAMERA_WITH_PARAMS(0x19286a9, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 65.0f, 0, 2);
		CAM::ATTACH_CAM_TO_ENTITY(bombCam, veh, 0.0f, 1.3324f, -0.9f, 1);
		CAM::POINT_CAM_AT_COORD(veh, camOffset.x, camOffset.y, camOffset.z);

		/* Not really necessary but gives it a gentle "turbulence" shake */
		CAM::SHAKE_CAM(bombCam, "ROAD_VIBRATION_SHAKE", 0.4f);

		/* Needed else it'll draw more than the bomb-bay doors which is not what we want */
		CAM::SET_CAM_FOV(bombCam, 65.0f);

		CAM::SET_CAM_ACTIVE(bombCam, true);
		CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0);
		UI::DISPLAY_RADAR(false);
		UI::DISPLAY_HUD(false);
	}
	else if (CAM::DOES_CAM_EXIST(bombCam))
	{
		/* Cleanup routine */
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

	/* Wait for doors to open/close before starting/stopping bomb drop */
	WAIT(20);

	/* Not needed but nice to have  - came out of scripts*/
	AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
	AUDIO::PLAY_SOUND_FRONTEND(-1, "DRUG_TRAFFIC_AIR_BAY_DOOR_OPEN_MASTER", 0, 1);

	bombDoorOpen ^= bombDoorOpen;
}

/* Needed to prevent the plane from crashing during bomb-cam since the control(s) are disabled */
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
	/* Have to make sure the vehicle is upright, actually moving and isn't in the water else this will fail */
	if (!VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(veh) || ENTITY::GET_ENTITY_SPEED(veh) > 0.0f || !ENTITY::IS_ENTITY_IN_WATER(veh) && bomb_drop_timer > 0)
	{
		/* Make sure we've not got any leftovers from any previous bombing runs else it'll get very messy */
		cleanup_active_bomb();
		cleanup_active_sounds();

		/* Not needed but nice to have  - came out of scripts*/
		AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
		bomb_sound_id = AUDIO::GET_SOUND_ID();
		AUDIO::PLAY_SOUND_FRONTEND(bomb_sound_id, "DRUG_TRAFFIC_AIR_BOMB_CAM_MASTER", 0, 1);

		/* Reset timer*/
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
		/* Set up bomb offset to vehicle */
		rot = { 80.0f, 0.0f, ENTITY::GET_ENTITY_HEADING(veh) + 180.0f };

		/* Get vehicle offset*/
		Vector3 veh_coords = ENTITY::GET_ENTITY_COORDS(veh, true);

		/* Create a bomb model */
		create_bomb(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, veh_coords.x, veh_coords.y, veh_coords.z), rot);

		/* Store bomb in our vector so we can keep track of how many are there. Could also use it to "find" the previous bomb's coords/rotation to tweak other bombs but no idea how to do that at the moment */
		vBomb.push_back(oBomb);
		//bomb_count++;

		prev_rot = rot;
	}
	else /* Set the rotation of bombs following the first one to all be facing the same direction (down towards the ground) when they're spawned in/dropped */
	{
		if (prev_rot.x < 79.1)
		{
			prev_rot.x += 4.0f;

			if (prev_rot.x > 79.1)
				prev_rot.x = 79.1;

			rot = prev_rot;
		}

		/* Set up bomb offset to vehicle */
		rot = { 80.0f, 0.0f, ENTITY::GET_ENTITY_HEADING(veh) + 180.0f };

		/* Get vehicle offset*/
		Vector3 veh_coords = ENTITY::GET_ENTITY_COORDS(veh, true);

		/* Create a bomb model */
		create_bomb(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, veh_coords.x, veh_coords.y, veh_coords.z), rot);

		/* Add new bomb into vector for later (may result in errors. If so, comment out and revert back to increment code below it) */
		vBomb.push_back(oBomb);
		//bomb_count++;
	}
}

/* Cleanup unused models after we're finished */
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

/* Cleanup + release requested sound so it doesn't keep playing when we're done + releases some unneeded memory */
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
	//https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/CarpetBomb.cs#L71
}


void play_explosion()
{
	//https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/CarpetBomb.cs#L167
}

bool onconfirm_veh_weapons_menu(MenuItem<int> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();


	if (!bPlayerExists){
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
	// common variables
	//BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	//Player player = PLAYER::PLAYER_ID();
	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	
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
	
}

//Main - https://github.com/CamxxCore/CarpetBomber/blob/master/GTAV_CarpetBomber/MainScript.cs