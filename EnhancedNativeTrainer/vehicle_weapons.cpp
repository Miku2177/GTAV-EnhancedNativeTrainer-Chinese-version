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
		/* 禁用武器控制，以防止玩家破坏炸弹生成器 */
		CONTROLS::DISABLE_CONTROL_ACTION(0, INPUT_VEH_FLY_ATTACK2, 1);

		/* 获取炸弹舱门的骨骼索引，用于将摄像机附加到其上 */
		int boneIndex = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh, "chassis_dummy");

		/* 获取骨骼坐标，用于将摄像机附加到其上 */
		Vector3 boneCoords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh, boneIndex);

		/* 获取摄像机相对于世界坐标的偏移量，以使摄像机与炸弹舱门保持固定距离（并朝向正确方向） */
		Vector3 camOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, boneCoords.x, boneCoords.y, boneCoords.z - 1.3f);

		/* 创建摄像机并将其附加到炸弹舱门上 - "0x19286a9" 来自脚本（launcher_basejumppack.c4） */
		bombCam = CAM::CREATE_CAMERA_WITH_PARAMS(0x19286a9, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 65.0f, 0, 2);
		CAM::ATTACH_CAM_TO_ENTITY(bombCam, veh, 0.0f, 1.3324f, -0.9f, 1);
		CAM::POINT_CAM_AT_COORD(veh, camOffset.x, camOffset.y, camOffset.z);

		/* 并非真正必要，但会为其添加轻微的“湍流”抖动效果 */
		CAM::SHAKE_CAM(bombCam, "ROAD_VIBRATION_SHAKE", 0.4f);

		/* 需要此操作，否则它会绘制比炸弹舱门更多的内容，这不是我们想要的 */
		CAM::SET_CAM_FOV(bombCam, 65.0f);

		CAM::SET_CAM_ACTIVE(bombCam, true);
		CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0);
		UI::DISPLAY_RADAR(false);
		UI::DISPLAY_HUD(false);
	}
	else if (CAM::DOES_CAM_EXIST(bombCam))
	{
		/* 清理程序 */
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

	/* 在开始/停止投弹之前，等待舱门打开/关闭 */
	WAIT(20);

	/* 并非必需，但有则更好 - 来自脚本 */
	AUDIO::REQUEST_SCRIPT_AUDIO_BANK("SCRIPT\DRUG_TRAFFIC_AIR", 0);
	AUDIO::PLAY_SOUND_FRONTEND(-1, "DRUG_TRAFFIC_AIR_BAY_DOOR_OPEN_MASTER", 0, 1);

	bombDoorOpen ^= bombDoorOpen;
}

/* 需要此操作以防止在炸弹摄像机模式下飞机坠毁，因为控制功能已被禁用 */
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
	// 公共变量
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if (!bPlayerExists){
		return true;
	}

	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))
	{
		set_status_text("玩家不在车辆中！");
		return true;
	}
	return false;
}

bool process_veh_weapons_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	
	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用炸弹舱门视角";
	toggleItem->toggleValue = &featureBombDoorCamera;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用自动驾驶功能";
	toggleItem->toggleValue = &featureAutoPilot;
	menuItems.push_back(toggleItem);

	MenuItem<int> *chooseTrimColor = new MenuItem<int>();
	chooseTrimColor->caption = "装饰颜色 ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseTrimColor->value = -2;
	chooseTrimColor->isLeaf = false;
	menuItems.push_back(chooseTrimColor);

	return draw_generic_menu<int>(menuItems, 0, "飞机炸弹选项", onconfirm_veh_weapons_menu, NULL, NULL, vehicle_menu_interrupt);
}

void reset_veh_weapons_globals()
{
	featureBombDoorCamera = false;
	featureAutoPilot = false;

	featureAutoPilotUpdated = false;
}

void update_veh_weapons_features()
{
	// 公共变量
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
