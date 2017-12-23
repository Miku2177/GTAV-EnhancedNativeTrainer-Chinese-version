#include "src\ui_support\menu_functions.h"
#include "src\io\config_io.h"
#include "src/features/vehicles.h"
#include <vector>

Camera bombCam = NULL;
Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

bool bombDoorOpen;

bool featureBombDoorCamera = false;
bool featureAutoPilot = false;
bool featureAutoPilotUpdated = false;


void create_bomb(Vector3 position, Vector3 rotation)
{
	Hash bomb_model = GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_01");

	if (STREAMING::IS_MODEL_IN_CDIMAGE(bomb_model))
	{
		STREAMING::REQUEST_MODEL(bomb_model);
		while (!STREAMING::HAS_MODEL_LOADED(bomb_model))
		{
			WAIT(0);
		}
	}
	
	Object oBomb = OBJECT::CREATE_OBJECT(bomb_model, position.x, position.y, position.z, false, false, false);
	ENTITY::SET_ENTITY_RECORDS_COLLISIONS(oBomb, true);
	ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(oBomb, true);
	ENTITY::SET_ENTITY_LOD_DIST(oBomb, 1000);
	ENTITY::SET_ENTITY_ROTATION(oBomb, rotation.x, rotation.y, rotation.z, 0, false);
}

void toggle_bomb_bay_camera()
{
	if (bombCam = NULL)
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

	if (enabled)
	{
		if (!UI::IS_WAYPOINT_ACTIVE())
			return;

		Blip wayPointBlip = UI::GET_FIRST_BLIP_INFO_ID(8);
		Vector3 wayPointCoord = UI::GET_BLIP_COORDS(wayPointBlip);
		AI::TASK_VEHICLE_DRIVE_TO_COORD(PLAYER::PLAYER_PED_ID(), veh, wayPointCoord.x, wayPointCoord.y, wayPointCoord.z + 150, 40.0f, 1, veh, 262144, -1.0, -1.0);
	}
	else
	{
		AI::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID());
	}
}

bool onconfirm_veh_weapons_menu(MenuItem<int> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists){
		return true;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

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
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
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
