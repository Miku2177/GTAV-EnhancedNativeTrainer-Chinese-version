/*
部分代码最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

现在是增强原生修改器器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 和 其他贡献者 2015
*/

/*
  F4					激活
  小键盘2/8/4/6			在菜单和列表中导航（必须打开数字锁）
  小键盘5 				选择
  小键盘0/退格键/F4 		返回
  小键盘9/3 				使用车辆加速（激活时）
  小键盘+ 				使用车辆火箭（激活时）
*/

#pragma comment(lib, "Shlwapi.lib")

#include "script.h"
#include "fuel.h"
#include "misc.h"
#include "world.h"
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
#include "../ui_support/menu_functions.h"
#include <set>
#include <iostream>
#include <vector>
#include <psapi.h>
#include <ctime>
#include "../io/controller.h"
#include "../rage_thread/rage_thread.h"

#pragma warning(disable : 4244 4305) // double <-> float conversions

//bool AIMBOT_INCLUDED = true;

std::string C_WEATHER_C;

// 自动加载地图内容
std::string MAP_STUFF;

bool player_died = false;
bool npc_player_died = false;

int last_player_slot_seen = 0;

int game_frame_num = 0;

int jumpfly_secs_passed, jumpfly_secs_curr, jumpfly_tick = 0;

int fuelLevelOffset = -1;
int fuelTankOffset = -1;

bool skydiving = false;

Vehicle temp_c_object = -1;

bool dive_glasses = false;
int ped_prop_idx = -1;

int injured_m = -2;
int noragdoll_m = -2;

bool player_d_armour = false;

int time_since_d = -1;
int time_since_a = -1;

char* i_anim_dict = "oddjobs@towing";
char* animation_of_i = "start_engine";
bool entered_first_time = false;

bool everInitialised = false;

bool falling_down = false;
bool looking_behind = false;
bool enabled_map = false;

bool veh_engine_t = false;

bool manual_pressed = true;
Vector3 CoordsWhereDied;

ENTDatabase* database = NULL;
Camera DeathCam = NULL;
Camera DeathCamM = NULL;

bool def_w = false;

bool onlineWarningShown = false;

bool injured_drunk = false;

// 功能
bool featurePlayerInvincible = false;
bool featurePlayerInvincibleUpdated = false;
bool featureNoFallDamage = false;
bool featureFireProof = false;
bool featurePlayerIgnoredByPolice = false;
bool featurePlayerUnlimitedAbility = false;
bool featurePlayerNoNoise = false;
bool featurePlayerFastSwim = false;
bool featurePlayerFastRun = false;
bool featurePlayerRunApartments = false;
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
//bool featureLevitation = false;
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
bool featureWantedLevelNoPRam = false;
bool featureWantedLevelNoSWATVehicles = false;
bool featureWantedLevelNoSWATVehiclesUpdated = false;
bool NoTaxiWhistling = false;
bool featurePlayerCanBeHeadshot = false;
bool featureRespawnsWhereDied = false;
bool lev_message = false;
bool engine_running = true;
bool we_have_troubles, iaminside = false;
bool been_injured = true;
bool p_invisible = false;
bool featurePlayerLife = false;
bool featurePlayerLifeUpdated = true;
bool featurePlayerStatsUpdated = true;
bool apply_pressed = false;
bool featureRagdollIfInjured = false;

// 受伤时布娃娃效果的变量
bool been_damaged_by_weapon, ragdoll_task = false;
float been_damaged_health, been_damaged_armor = -1;
int ragdoll_seconds = 0; 
//
Ped oldplayerPed = -1;
int playerDataMenuIndex, playerPrisonMenuIndex, playerForceshieldMenuIndex = 0; // tick, 
int scr_tick_secs_passed, scr_tick_secs_curr = 0;
int NPCragdollMenuIndex = 0;
int PlayerMovementMenuIndex = 0;
int PlayerMostWantedMenuIndex = 0;
int PlayerWantedMaxPossibleLevelMenuIndex = 0;

int frozenWantedLevel = 0;

Vehicle veh_engine;
Vehicle veh_killed;

// 玩家模型控制，在需要时切换回正常的 NPC 模型

char* player_models[] = { "player_zero", "player_one", "player_two" };

char* mplayer_models[] = { "mp_f_freemode_01", "mp_m_freemode_01" };

const char* CLIPSET_DRUNK = "move_m@drunk@verydrunk";

const std::vector<std::string> GRAVITY_CAPTIONS{ "最低", "0.1x", "0.5x", "0.75x", "1x (正常)" };
const float GRAVITY_VALUES[] = { 0.0f, 0.1f, 0.5f, 0.75f, 1.0f };

const std::vector<std::string> REGEN_CAPTIONS{ "禁止恢复血量", "0.1x", "0.25x", "0.5x", "1x (正常)", "2x", "5x", "10x", "20x", "50x", "100x", "200x", "500x", "1000x" };
const float REGEN_VALUES[] = { 0.0f, 0.1f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f, 20.0f, 50.0f, 100.0f, 200.0f, 500.0f, 1000.0f };
int current_regen_speed = 4;
bool current_regen_speed_changed = true;

// 玩家生命值
int current_player_health = 6;
bool current_player_health_Changed = true;
int PedsHealthIndex = 0;
bool PedsHealthChanged = true;

// 最高通缉等级
int wanted_maxpossible_level = 4;
bool wanted_maxpossible_level_Changed = true;

// 玩家护甲
const std::vector<std::string> PLAYER_ARMOR_CAPTIONS{ "关", "0", "15", "20", "30", "40", "50", "100" };
const int PLAYER_ARMOR_VALUES[] = { -1, 0, 15, 20, 30, 40, 50, 100 };
int current_player_armor = 7;
bool current_player_armor_Changed = true;
int current_player_stats = 0;
bool current_player_stats_Changed = true;

// NPC 被射中效果
int current_npc_ragdoll = 0;
bool current_npc_ragdoll_Changed = true;

// 被射中时摄像机震动
int feature_shake_ragdoll = 0;
bool feature_shake_ragdoll_Changed = true;

// 禁止布娃娃
int current_no_ragdoll = 0;
bool current_no_ragdoll_Changed = true;

// 受伤时跛行
int current_limp_if_injured = 0;
bool current_limp_if_injured_Changed = true;

// 受伤时摇晃相机
int feature_shake_injured = 0;
bool feature_shake_injured_Changed = true;
bool enable_camera_injured = false;
int curr_cam = -1;
int curr_hlth = -1;

// 玩家跑步速度 && 汉考克模式(美国一部电影中的主角)
const std::vector<std::string> PLAYER_MOVEMENT_CAPTIONS{ "正常", "0.5x", "1x", "2x", "3x", "4x", "5x", "6x", "7x", "8x", "9x", "10x" };
const double PLAYER_MOVEMENT_VALUES[] = { 0.00, 0.60, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 10.00 };
int current_player_movement = 0;
bool current_player_movement_Changed = true; 
int current_player_jumpfly = 0;
bool current_player_jumpfly_Changed = true;
int current_player_superjump = 0;
bool current_player_superjump_Changed = true;

/* Prop unblocker related code - will need to clean up later*/
//与道具解锁器相关的代码 - 以后需要清理

/*THIS causes ENT not to load when Menyoo is present!*/
//这会导致 Menyoo 存在时 ENT 无法加载！

void UnlockAllObjects()
{
	static auto checkModelBeforeCreation = FindPattern("\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x8B\x48\x50", "xxxxx????xxx");
	memset(reinterpret_cast<void*>(checkModelBeforeCreation), 0x90, 24);
}

/* End of prop unblocker code*/
//道具解锁器代码的结束

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

void onchange_no_ragdoll_mode(int value, SelectFromListMenuItem* source) {
	current_no_ragdoll = value;
	current_no_ragdoll_Changed = true;
}

void onchange_shake_ragdoll_mode(int value, SelectFromListMenuItem* source) {
	feature_shake_ragdoll = value;
	feature_shake_ragdoll_Changed = true;
}

void onchange_shake_injured_mode(int value, SelectFromListMenuItem* source) {
	feature_shake_injured = value;
	feature_shake_injured_Changed = true;
}

void onchange_limp_if_injured_mode(int value, SelectFromListMenuItem* source) {
	current_limp_if_injured = value;
	current_limp_if_injured_Changed = true;
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

// 获取屏幕的横向和纵向尺寸（以像素为单位）
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow(); // 获取桌面窗口的句柄
	GetWindowRect(hDesktop, &desktop); // 将屏幕尺寸获取到变量 desktop 中
	// 左上角的坐标为 (0,0)
	// 右下角的坐标为
	horizontal = desktop.right; //横向
	vertical = desktop.bottom; //纵向
}

void check_player_model(){
	/*
	std::stringstream ss;
	ss << "PID: " << *gp;
	set_status_text_centre_screen(ss.str());
	*/

	if(!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())){
		return;
	}

	//查明我们是否是默认玩家模型
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

	if (!ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) CoordsWhereDied = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);

	if (MISC_TRAINERCONTROL_VALUES[ResetSkinOnDeathIdx] == 0 && manual_pressed == true) {
		if ((ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && (!featureRespawnsWhereDied || detained == true || alert_level > 0)) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
			if (!found) {
				float temp_dist = 20000.0;
				float d_temp1 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 338.409, -1396.54, 32.5092);
				float d_temp2 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 363.229, -583.42, 28.6848);
				float d_temp3 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, -451.687, -345.756, 34.5016);
				float d_temp4 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 1105.7, -1533.99, 34.8937);
				float d_temp5 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 1841.54, 3669.09, 33.68);
				float d_temp6 = SYSTEM::VDIST(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, -240.879, 6324.1, 32.4261);

				if (temp_dist > d_temp1) {
					temp_dist = d_temp1;
					CoordsWhereDied.x = 338.409;
					CoordsWhereDied.y = -1396.54;
					CoordsWhereDied.z = 32.5092;
				}
				if (temp_dist > d_temp2) {
					temp_dist = d_temp2;
					CoordsWhereDied.x = 363.229;
					CoordsWhereDied.y = -583.42;
					CoordsWhereDied.z = 28.6848;
				}
				if (temp_dist > d_temp3) {
					temp_dist = d_temp3;
					CoordsWhereDied.x = -451.687;
					CoordsWhereDied.y = -345.756;
					CoordsWhereDied.z = 34.5016;
				}
				if (temp_dist > d_temp4) {
					temp_dist = d_temp4;
					CoordsWhereDied.x = 1105.7;
					CoordsWhereDied.y = -1533.99;
					CoordsWhereDied.z = 34.8937;
				}
				if (temp_dist > d_temp5) {
					temp_dist = d_temp5;
					CoordsWhereDied.x = 1841.54;
					CoordsWhereDied.y = 3669.09;
					CoordsWhereDied.z = 33.68;
				}
				if (temp_dist > d_temp6) {
					temp_dist = d_temp6;
					CoordsWhereDied.x = -240.879;
					CoordsWhereDied.y = 6324.1;
					CoordsWhereDied.z = 32.4261;
				}
				
				player_died = true;
				player_d_armour = true;
				CAM::DO_SCREEN_FADE_OUT(500);
				WAIT(1000);
				GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
				GAMEPLAY::IGNORE_NEXT_RESTART(true);
				GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
				GAMEPLAY::_RESET_LOCALPLAYER_STATE();
				NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 0, false, false);
				PLAYER::RESET_PLAYER_ARREST_STATE(PLAYER::PLAYER_PED_ID());
				PED::RESET_PED_MOVEMENT_CLIPSET(PLAYER::PLAYER_PED_ID(), 1.0f);
				WAIT(1000);
				CAM::DO_SCREEN_FADE_IN(500);
				GAMEPLAY::SET_TIME_SCALE(1.0f);
			}
		}
	}
}

void invincibility_switching(){
	featurePlayerInvincible = !featurePlayerInvincible;
	featurePlayerInvincibleUpdated = true;
	if (featurePlayerInvincible) set_status_text("无敌-已开启");
	else set_status_text("无敌-已关闭");
	WAIT(100);
}

void wantedlevel_switching() {
	featureWantedLevelFrozen = !featureWantedLevelFrozen;
	if (featureWantedLevelFrozen) set_status_text("通缉等级-已冻结");
	else set_status_text("通缉等级-已解冻");
	WAIT(100);
}

void ingnition_anim() {
	i_anim_dict = "oddjobs@towing";
	animation_of_i = "start_engine";
	Vehicle veh_ignition = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	if ((VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh_ignition)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh_ignition)))) {
		i_anim_dict = "veh@bike@sport@front@base";
		animation_of_i = "start_engine";
	}
	if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh_ignition)) || VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh_ignition))) {
		i_anim_dict = "anim@veh@savage@front@ds@base";
		animation_of_i = "pov_start_engine";
	}
	if (!STREAMING::HAS_ANIM_DICT_LOADED(i_anim_dict)) {
		STREAMING::REQUEST_ANIM_DICT(i_anim_dict);
		while (!STREAMING::HAS_ANIM_DICT_LOADED(i_anim_dict)) WAIT(0);
	}
	if (STREAMING::HAS_ANIM_DICT_LOADED(i_anim_dict)) {
		float anim_tmp = ENTITY::_GET_ENTITY_ANIM_DURATION(i_anim_dict, animation_of_i);
		AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), i_anim_dict, animation_of_i, 16.0, 3.0, -1, 0, 0, 0, 0, 0); // 8.0 1.5
		WAIT(anim_tmp * 499); // 499
		AI::STOP_ANIM_TASK(PLAYER::PLAYER_PED_ID(), i_anim_dict, animation_of_i, 1.0);
	}
}

void engineonoff_switching() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_engine = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_engine = temp_vehicle;
	}
	WAIT(70);
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && entered_first_time == false && CONTROLS::IS_CONTROL_RELEASED(2, 71)) ingnition_anim();
	entered_first_time = false;

	if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh_engine)) engine_running = true;
	engine_running = !engine_running;
	WAIT(100);
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) VEHICLE::SET_VEHICLE_ENGINE_ON(veh_engine, engine_running, false, !engine_running);
		else VEHICLE::SET_VEHICLE_ENGINE_ON(veh_engine, engine_running, true, !engine_running);
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
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_killed = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_killed = temp_vehicle;
	}
		
	VEHICLE::SET_VEHICLE_ENGINE_ON(veh_killed, false, true, true);
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh_killed, -4000);

	set_status_text("你因为某种原因，损坏了这辆车的发动机 !");
}

void text_parameters(float s_x, float s_y, int c_r, int c_g, int c_b, int alpha) {
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(s_x, s_y);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_COLOUR(c_r, c_g, c_b, alpha);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(20, 20, 20, 20, 20);
	UI::SET_TEXT_EDGE(100, 100, 100, 100, 205);
	UI::SET_TEXT_LEADING(1);
	UI::SET_TEXT_OUTLINE();
}

// 更新所有可以被游戏关闭的功能， 每帧游戏时调用一次
void update_features() {
	// 常用变量
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	if (NETWORK::NETWORK_IS_GAME_IN_PROGRESS()) {
		if (!onlineWarningShown) {
			set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT ~HUD_COLOUR_WHITE~禁止在线使用！");
			onlineWarningShown = true;
		}
		WAIT(0);
		return;
	}
	else {
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

	check_for_hotkey_presses();

	if (is_menu_showing() && (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, 0) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_NEXT_CAMERA))) {
		if (featureRealisticRadioVolume && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) radio_v_checked = false;
		set_menu_showing(false);
		WAIT(200);
		set_menu_showing(true);
	}

	if (is_menu_showing() && (IsKeyDown(VK_ESCAPE) || CONTROLS::IS_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_PAUSE))) {
		set_menu_showing(false);
		if (MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex] > -2 && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) == 1 && !CUTSCENE::IS_CUTSCENE_PLAYING() && keyboard_on_screen_already == false) {
			UI::ACTIVATE_FRONTEND_MENU(GAMEPLAY::GET_HASH_KEY("FE_MENU_VERSION_SP_PAUSE"), featureGamePause, MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex]);
			AUDIO::SET_AUDIO_FLAG("PlayMenuMusic", true);
		}
		else AUDIO::SET_AUDIO_FLAG("PlayMenuMusic", false);
		enabled_map = true;
	}
	if (!is_menu_showing() && enabled_map == true && !featurePenitentiaryMap && !featureCayoPericoMap) {
		WAIT(200);
		set_menu_showing(true);
		enabled_map = false;
	}
	if (!is_menu_showing() && enabled_map == true && MISC_DEF_MANUTAB_VALUES[DefMenuTabIndex] > -2) {
		set_menu_showing(true);
		enabled_map = false;
	}

	if (CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, 0) || CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, INPUT_NEXT_CAMERA)) {
		if (is_in_airbrake_mode() && !airbrake_switch_pressed()) {
			KeyConfig* key = get_config()->get_key_config()->get_key(KeyConfig::KEY_TOGGLE_AIRBRAKE);
			CONTROLS::_SET_CONTROL_NORMAL(0, key->keyCode, 1);
			WAIT(200);
			CONTROLS::_SET_CONTROL_NORMAL(0, key->keyCode, 1);
		}
	}
	
	if (is_menu_showing() && (CONTROLS::IS_DISABLED_CONTROL_PRESSED(26, 0) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_LOOK_BEHIND))) {
		set_menu_showing(false);
		looking_behind = true;
	}
	if (!CONTROLS::IS_DISABLED_CONTROL_PRESSED(26, 0) && !CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, INPUT_LOOK_BEHIND) && looking_behind == true) {
		set_menu_showing(true);
		looking_behind = false;
	}

	if(is_menu_showing() || is_in_airbrake_mode() || is_in_prop_placement_mode()){
		disableControls();
		if(/*is_input_blocked_in_menu() || */is_in_airbrake_mode() || is_in_prop_placement_mode()){
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

	// 第一人称死亡视角
	if (featureFirstPersonDeathCamera) {
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2);
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
			if (!CAM::DOES_CAM_EXIST(DeathCam)) {
				DeathCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z + 0.07, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
				if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) {
					CAM::ATTACH_CAM_TO_ENTITY(DeathCam, PLAYER::PLAYER_PED_ID(), 0, 0.0, 0.07, 1);
					CAM::POINT_CAM_AT_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 11816, 0, 0.0, 0.05, 1);
				}
				if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
					CAM::ATTACH_CAM_TO_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, -0.15, 0, 1);
					CAM::POINT_CAM_AT_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, 0.0, 0, 1);
				}
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(DeathCam, 1.0);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCam, 1.0);
				CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCam, 1.0);
				CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
				CAM::SET_CAM_ACTIVE(DeathCam, true);
				ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 0, 0);
			}
		}
		if (CAM::DOES_CAM_EXIST(DeathCam)) { // 摄像机旋转
			Vector3 rot_cam = CAM::GET_CAM_ROT(DeathCam, 2);
			if ((CONTROLS::IS_CONTROL_PRESSED(2, 34) || CONTROLS::IS_CONTROL_PRESSED(2, 35) || CONTROLS::IS_CONTROL_PRESSED(2, 32) || CONTROLS::IS_CONTROL_PRESSED(2, 33)) && first_person_rotate == false) {
				CAM::DESTROY_CAM(DeathCam, true);
				DeathCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z + 0.07, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
				if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) CAM::ATTACH_CAM_TO_ENTITY(DeathCam, PLAYER::PLAYER_PED_ID(), 0, 0.0, 0.07, 1);
				if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) CAM::ATTACH_CAM_TO_PED_BONE(DeathCam, PLAYER::PLAYER_PED_ID(), 31086, 0, -0.15, 0, 1);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(DeathCam, 1.0);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCam, 1.0);
				CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCam, 1.0);
				first_person_rotate = true;
			}
			if (CONTROLS::IS_CONTROL_PRESSED(2, 34)) rot_cam.z = rot_cam.z + 2; // 仅左侧
			if (CONTROLS::IS_CONTROL_PRESSED(2, 35)) rot_cam.z = rot_cam.z - 2; // 仅右侧
			if (CONTROLS::IS_CONTROL_PRESSED(2, 32)) rot_cam.x = rot_cam.x + 2; // 仅上侧
			if (CONTROLS::IS_CONTROL_PRESSED(2, 33)) rot_cam.x = rot_cam.x - 2; // 仅下侧
			CAM::SET_CAM_ROT(DeathCam, rot_cam.x, rot_cam.y, rot_cam.z, 2);
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
			ENTITY::RESET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID());
			first_person_rotate = false;
		}
	}

	// 手动复活
	if (featureNoAutoRespawn && GAMEPLAY::GET_MISSION_FLAG() == 0) {
		if ((ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) && player_died == false) {
			manual_pressed = false;
			if (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("bot_01b_bit_03"))) STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("bot_01b_bit_03"));
			SCRIPT::SET_NO_LOADING_SCREEN(true);
			GAMEPLAY::SET_FADE_OUT_AFTER_DEATH(false);
			GAMEPLAY::SET_FADE_OUT_AFTER_ARREST(false);
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			GAMEPLAY::IGNORE_NEXT_RESTART(true);
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
			manual_instant = true;
		}
		// 摄像机旋转
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			if (!CAM::DOES_CAM_EXIST(DeathCamM) && (CONTROLS::IS_CONTROL_PRESSED(2, 34) || CONTROLS::IS_CONTROL_PRESSED(2, 35) || CONTROLS::IS_CONTROL_PRESSED(2, 32) || CONTROLS::IS_CONTROL_PRESSED(2, 33)) && first_person_rotate == false) {
				temp_c_object = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("bot_01b_bit_03"), playerPosition.x + 1, playerPosition.y + 1, playerPosition.z + 1.5, 1, true, 1);
				ENTITY::SET_ENTITY_ALPHA(temp_c_object, 0, 0);
				ENTITY::FREEZE_ENTITY_POSITION(temp_c_object, true);
				DeathCamM = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x + 1, playerPosition.y + 1, playerPosition.z + 1.5, 0, 0, 0, 60.0, true, 2);
				CAM::ATTACH_CAM_TO_ENTITY(DeathCamM, temp_c_object, 0, 0.0, 0.0, 1);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(DeathCamM, 1.0);
				CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCamM, 1.0);
				CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCamM, 1.0);
				CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
				CAM::SET_CAM_ACTIVE(DeathCamM, true);
				GAMEPLAY::SET_TIME_SCALE(1.0f);
			}
			if (CAM::DOES_CAM_EXIST(DeathCamM)) { // 摄像机旋转
				Vector3 rot_cam = CAM::GET_CAM_ROT(DeathCamM, 2);
				ENTITY::FREEZE_ENTITY_POSITION(temp_c_object, true);
				if ((CONTROLS::IS_CONTROL_PRESSED(2, 34) || CONTROLS::IS_CONTROL_PRESSED(2, 35) || CONTROLS::IS_CONTROL_PRESSED(2, 32) || CONTROLS::IS_CONTROL_PRESSED(2, 33)) && first_person_rotate == false) {
					CAM::DESTROY_CAM(DeathCamM, true);
					DeathCamM = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x + 1, playerPosition.y + 1, playerPosition.z + 1.5, rot_cam.x, rot_cam.y, rot_cam.z, 60.0, true, 2);
					CAM::ATTACH_CAM_TO_ENTITY(DeathCamM, temp_c_object, 0, 0.0, 0.0, 1);
					CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCamM, 1.0);
					CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCamM, 1.0);
					first_person_rotate = true;
				}
				if (CONTROLS::IS_CONTROL_PRESSED(2, 34)) rot_cam.z = rot_cam.z + 2; // 仅左侧
				if (CONTROLS::IS_CONTROL_PRESSED(2, 35)) rot_cam.z = rot_cam.z - 2; // 仅右侧
				if (CONTROLS::IS_CONTROL_PRESSED(2, 32)) rot_cam.x = rot_cam.x + 2; // 仅上侧
				if (CONTROLS::IS_CONTROL_PRESSED(2, 33)) rot_cam.x = rot_cam.x - 2; // 仅下侧
				CAM::SET_CAM_ROT(DeathCamM, rot_cam.x, rot_cam.y, rot_cam.z, 2);
			}
		}
		//
		if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) { // 23 // CONTROLS::IS_CONTROL_JUST_PRESSED(2, 176) ||
			OBJECT::DELETE_OBJECT(&temp_c_object);
			manual_pressed = true;
			tick_allw = 0;
			player_died = true;
			player_d_armour = true;
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(false);
			SCRIPT::SET_NO_LOADING_SCREEN(false);
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			CAM::DO_SCREEN_FADE_OUT(500); // 4000
			WAIT(1000);
			GRAPHICS::_STOP_ALL_SCREEN_EFFECTS();
			CONTROLS::_SET_CONTROL_NORMAL(0, 22, 1);
			GAMEPLAY::SET_TIME_SCALE(1.0f);
			manual_instant = false;
			dynamic_loading = true;
		}
		if ((PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) { // CONTROLS::IS_CONTROL_JUST_PRESSED(2, 176) || 
			OBJECT::DELETE_OBJECT(&temp_c_object);
			manual_pressed = true;
			tick_allw = 0;
			CAM::DO_SCREEN_FADE_OUT(500);
			WAIT(1000);
			GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
			GAMEPLAY::IGNORE_NEXT_RESTART(true);
			GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("respawn_controller");
			GRAPHICS::_STOP_ALL_SCREEN_EFFECTS();
			GAMEPLAY::_RESET_LOCALPLAYER_STATE();
			Vector3 ped_me = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			BOOL onGround = false;
			Vector3 CoordsWhereDied = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			PATHFIND::GET_SAFE_COORD_FOR_PED(ped_me.x, ped_me.y, ped_me.z, onGround, &CoordsWhereDied, 16);
			NETWORK::NETWORK_RESURRECT_LOCAL_PLAYER(CoordsWhereDied.x, CoordsWhereDied.y, CoordsWhereDied.z, 0, false, false);
			PLAYER::RESET_PLAYER_ARREST_STATE(PLAYER::PLAYER_PED_ID());
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			WAIT(1000);
			CAM::DO_SCREEN_FADE_IN(500);
			GAMEPLAY::SET_TIME_SCALE(1.0f);
			manual_instant = false;
		}
		if (!ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) && !PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 1)) {
			if (detained == false && alert_level == 0) player_died = false;
			if (CAM::DOES_CAM_EXIST(DeathCamM)) {
				ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), 1, 1);
				CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
				CAM::DETACH_CAM(DeathCamM);
				CAM::SET_CAM_ACTIVE(DeathCamM, false);
				CAM::DESTROY_CAM(DeathCamM, true);
				ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), 255, 0);
				first_person_rotate = false;
				GAMEPLAY::SET_TIME_SCALE(1.0f);
			}
		}
	}
		
	// 死亡/被捕后立即复活
	if (featureRespawnsWhereDied && GAMEPLAY::GET_MISSION_FLAG() == 0 && manual_instant == false && detained == false && alert_level == 0) {
		if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) {
			player_died = true;
			player_d_armour = true;
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
			PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			WAIT(1000);
			CAM::DO_SCREEN_FADE_IN(500);
			GAMEPLAY::SET_TIME_SCALE(1.0f);
		}
	}

	update_centre_screen_status_text();
	
	update_world_features();

	update_skin_features();

	update_teleport_features();

	check_player_model();

	maintain_bodyguards();

	update_props_pending_dialogs();

	update_area_effects(playerPed);
	
	update_speedaltitude(playerPed);

	update_weapon_features(bPlayerExists, player);

	//if(AIMBOT_INCLUDED){
	//	update_aimbot_esp_features();
	//}

	update_vehicle_features(bPlayerExists, playerPed);

	update_anims_features(bPlayerExists, playerPed);

	update_vehmodmenu_features(bPlayerExists, playerPed);

	update_veh_weapons_features();

	update_misc_features(bPlayerExists, playerPed);

	update_time_features(player);

	// 无敌
	if(featurePlayerInvincibleUpdated){
		if(bPlayerExists && !featurePlayerInvincible){
			if (getGameVersion() < VER_1_0_678_1_STEAM || getGameVersion() < VER_1_0_678_1_NOSTEAM) PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
			if (getGameVersion() >= VER_1_0_678_1_STEAM || getGameVersion() >= VER_1_0_678_1_NOSTEAM) PLAYER::_0x733A643B5B0C53C1(player, FALSE);
		}
		WAIT(100);
		featurePlayerInvincibleUpdated = false;
		WAIT(100);
	}
	if(featurePlayerInvincible && bPlayerExists){
		if (getGameVersion() < VER_1_0_678_1_STEAM || getGameVersion() < VER_1_0_678_1_NOSTEAM) PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
		if (getGameVersion() >= VER_1_0_678_1_STEAM || getGameVersion() >= VER_1_0_678_1_NOSTEAM) PLAYER::_0x733A643B5B0C53C1(player, TRUE);
	}
	
	// 防火
	if (featureFireProof/* && !featurePlayerInvincible*/) {
		Vector3 my_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
		FIRE::STOP_FIRE_IN_RANGE(my_coords.x, my_coords.y, my_coords.z, 2);
		if (FIRE::IS_ENTITY_ON_FIRE(PLAYER::PLAYER_PED_ID())) FIRE::STOP_ENTITY_FIRE(PLAYER::PLAYER_PED_ID());
	}

	// 无摔落伤害
	if (featureNoFallDamage && !featurePlayerInvincible) {
		if (PED::IS_PED_FALLING(playerPed) || PED::IS_PED_IN_PARACHUTE_FREE_FALL(playerPed)) falling_down = true;
		if (falling_down) PLAYER::SET_PLAYER_INVINCIBLE(player, TRUE);
		if (!PED::IS_PED_FALLING(playerPed) && !PED::IS_PED_IN_PARACHUTE_FREE_FALL(playerPed) && falling_down) {
			if (!PED::IS_PED_RAGDOLL(playerPed) && (CONTROLS::IS_CONTROL_PRESSED(2, 32) || CONTROLS::IS_CONTROL_PRESSED(2, 33) || CONTROLS::IS_CONTROL_PRESSED(2, 34) || CONTROLS::IS_CONTROL_PRESSED(2, 35))) {
				falling_down = false;
				PLAYER::SET_PLAYER_INVINCIBLE(player, FALSE);
			}
		}
	}

	// 启用默认武器
	if (def_w == false) {
		CONTROLS::_SET_CONTROL_NORMAL(0, 159, 1); // 160
		WAIT(10);
		CONTROLS::_SET_CONTROL_NORMAL(0, 157, 1);
		def_w = true;
	}

	// 获取自上次死亡/被捕以来的时间
	if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) time_since_d = -1;
	if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) time_since_a = -1;
	if (time_since_d < 7000 && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) time_since_d = PLAYER::GET_TIME_SINCE_LAST_DEATH();
	if (time_since_a < 7000 && !PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), 0)) time_since_a = PLAYER::GET_TIME_SINCE_LAST_ARREST();

	// 禁用点火
	if ((!featureDisableIgnition || (featureDisableIgnition && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))) && veh_engine_t == true) {
		if (NPC_RAGDOLL_VALUES[EngineRunningIndex] == 0) VEHICLE::SET_VEHICLE_ENGINE_ON(veh_engine, false, true, false);
		veh_engine_t = false;
	}
	if (featureDisableIgnition && breaking_secs_tick == 0) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && veh_engine_t == false) {
			veh_engine = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (!VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh_engine)) {
				engine_running = true;
				entered_first_time = true;
				engineonoff_switching();
			}
			veh_engine_t = true;
		}
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && !VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(PED::GET_VEHICLE_PED_IS_USING(playerPed)) && CONTROLS::IS_CONTROL_PRESSED(2, 71)) {
			engine_running = false;
			engineonoff_switching();
		}
	}

	// 禁用引擎
	if (engine_running == false && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) VEHICLE::SET_VEHICLE_ENGINE_ON(veh_engine, engine_running, false, true);

	if (featureWantedLevelFrozen){
		if (featureWantedLevelFrozenUpdated){
			frozenWantedLevel = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			featureWantedLevelFrozenUpdated = false;
		}
		PLAYER::SET_MAX_WANTED_LEVEL(frozenWantedLevel);
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, frozenWantedLevel, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
	}
	if (featureWantedLevelFrozenUpdated == false && !featureWantedLevelFrozen){
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		//featureWantedLevelFrozenUpdated = false;
		featureWantedLevelFrozenUpdated = true;
	}
	
	// 禁止警察直升机
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

	// 无路障
	if (featureWantedNoPRoadB) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(8, false);
		featureWantedNoPRoadBUpdated = true;
	}
	else if (featureWantedNoPRoadBUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(8, true);
		featureWantedNoPRoadBUpdated = false;
	}

	// 禁止警察船只
	if (featureWantedLevelNoPBoats) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(13, false);
		featureWantedLevelNoPBoatsUpdated = true;
	}
	else if (featureWantedLevelNoPBoatsUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(13, true);
		featureWantedLevelNoPBoatsUpdated = false;
	}

	// 禁止特警车辆
	if (featureWantedLevelNoSWATVehicles) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(4, false);
		featureWantedLevelNoSWATVehiclesUpdated = true;

	}
	else if (featureWantedLevelNoSWATVehiclesUpdated == true) {
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(4, true);
		featureWantedLevelNoSWATVehiclesUpdated = false;
	}

	// 警方追捕行动减弱
	if (featureWantedLevelNoPRam && PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0) {
		Vector3 my_cor = ENTITY::GET_ENTITY_COORDS(playerPed, true);
		float my_speed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
		const int arrSize4 = 1024;
		Ped copram[arrSize4];
		int count_cop_ram = worldGetAllPeds(copram, arrSize4);
		for (int i = 0; i < count_cop_ram; i++) {
			if ((PED::GET_PED_TYPE(copram[i]) == 6 || PED::GET_PED_TYPE(copram[i]) == 27 || PED::GET_PED_TYPE(copram[i]) == 29) && copram[i] != playerPed) {
				Vector3 cop_cor = ENTITY::GET_ENTITY_COORDS(copram[i], true);
				//float cop_speed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_USING(copram[i]));
				float dist_diff_c_m = SYSTEM::VDIST(my_cor.x, my_cor.y, my_cor.z, cop_cor.x, cop_cor.y, cop_cor.z);
				if (dist_diff_c_m < 30) { // 50 //  && cop_speed > my_speed
					//AI::TASK_VEHICLE_TEMP_ACTION(copram[i], PED::GET_VEHICLE_PED_IS_USING(copram[i]), 27, 1); // 100
					AI::SET_DRIVE_TASK_CRUISE_SPEED(copram[i], my_speed);
					AI::SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(copram[i], 10.0f); // 30
					PED::SET_DRIVER_AGGRESSIVENESS(copram[i], 0.0f);
				}
			}
		}
	}

	// 禁止鸣笛叫出租车
	if (NoTaxiWhistling && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) && !UI::IS_HELP_MESSAGE_BEING_DISPLAYED() && GAMEPLAY::GET_MISSION_FLAG() == 0) CONTROLS::DISABLE_CONTROL_ACTION(2, 51, 1);
		
	// 悬浮
	if (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] < 1) lev_message = false;
	if (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] > 0/*featureLevitation*/) {
		if (lev_message == false) {
			set_status_text("按住空格使用您的力量！");
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
			if (surr_p_peds[i] != playerPed && !PED::IS_PED_IN_ANY_VEHICLE(surr_p_peds[i], 0)) { 
				if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && tempgot_x < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] && tempgot_y < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex]) {
					PED::SET_PED_CAN_RAGDOLL(surr_p_peds[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(surr_p_peds[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(surr_p_peds[i]);
					STREAMING::REQUEST_ANIM_DICT("dead@fall");
					while (!STREAMING::HAS_ANIM_DICT_LOADED("dead@fall")) WAIT(0);
					if (!ENTITY::IS_ENTITY_PLAYING_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 3)) {
						AI::TASK_PLAY_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 8.0, 0.0, -1, 9, 0, 0, 0, 0);
						PED::SET_PED_TO_RAGDOLL(surr_p_peds[i], 1, 1, 1, 1, 1, 1);
					}
					if (tempgot_z < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex]) ENTITY::APPLY_FORCE_TO_ENTITY(surr_p_peds[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true); // 20
				}
				if (CONTROLS::IS_CONTROL_PRESSED(2, 22) && (tempgot_x > (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] - 1) || tempgot_y > (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] - 1)) && tempgot_z > 5) {
					Vector3 curPLocation = ENTITY::GET_ENTITY_COORDS(surr_p_peds[i], 0);
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(surr_p_peds[i], curPLocation.x, curPLocation.y, curPLocation.z - 1, 1, 1, 1);
				}
				if (CONTROLS::IS_CONTROL_RELEASED(2, 22) || tempgot_x > (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] - 1) || tempgot_y > (VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] - 1)) {
					if (ENTITY::IS_ENTITY_PLAYING_ANIM(surr_p_peds[i], "dead@fall", "dead_fall_down", 3)) {
						Vector3 curPLocation = ENTITY::GET_ENTITY_COORDS(surr_p_peds[i], 0);
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(surr_p_peds[i], curPLocation.x, curPLocation.y, curPLocation.z, 1, 1, 1);
						AI::STOP_ANIM_TASK(surr_p_peds[i], "dead@fall", "dead_fall_down", 1.0);
					}
				}
			}
		} // 结束整数（行人）
		if (CONTROLS::IS_CONTROL_PRESSED(2, 22)) {
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
				if (tempgot_x < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] && tempgot_y < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] && tempgot_z < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex]) 
					ENTITY::APPLY_FORCE_TO_ENTITY(surr_objects[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true);
			} // 结束整数（物体）
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
				if (tempgot_x < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] && tempgot_y < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex] && tempgot_z < VEH_TURN_SIGNALS_ANGLE_VALUES[LevitationIndex]) 
					ENTITY::APPLY_FORCE_TO_ENTITY(surr_vehicles[i], 1, 0, 0, 0.6, 0, 0, 0, true, false, true, true, true, true);
			} // 结束整数（车辆）
		}
	}

	// 汉考克模式
	if (PLAYER_MOVEMENT_VALUES[current_player_jumpfly] > 0.00 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
		Vector3 CamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		float p_force = PLAYER_MOVEMENT_VALUES[current_player_jumpfly];
		float rad = 2 * 3.14 * (CamRot.z / 360);
		float v_x = -(sin(rad) * p_force * 10);
		float v_y = (cos(rad) * p_force * 10);
		float v_z = p_force * (CamRot.x * 0.2);
		Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
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
					AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle 自由待机
				}
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, 0, 0, p_force, 0, 0, 0, true, false, true, true, true, true);
			}
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) skydiving = true;
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 32) && skydiving == true) { // 仅向上移动
			if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) {
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle 自由待机
			}
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), CamRot.x, CamRot.y, CamRot.z, 1, true);
			ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, v_x / 8, v_y / 8, v_z / 8, 0, 0, 0, true, false, true, true, true, true);
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 33) && skydiving == true) { // 仅向下移动
			jumpfly_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - jumpfly_secs_curr) != 0) {
				jumpfly_tick = jumpfly_tick + 1;
				jumpfly_secs_curr = jumpfly_secs_passed;
			}
			if (!ENTITY::IS_ENTITY_PLAYING_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 3)) {
				AI::CLEAR_PED_TASKS_IMMEDIATELY(PLAYER::PLAYER_PED_ID());
				AI::TASK_PLAY_ANIM(PLAYER::PLAYER_PED_ID(), "skydive@base", "free_idle", 8.0, 0.0, -1, 9, 0, 0, 0, 0); // free_idle 自由待机
			}
			ENTITY::SET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), CamRot.x, CamRot.y, CamRot.z, 1, true);
			if (jumpfly_tick < 30) ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false); 
			else {
				ENTITY::FREEZE_ENTITY_POSITION(PLAYER::PLAYER_PED_ID(), false);
				ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, -(v_x / 8), -(v_y / 8), -(v_z / 8), 0, 0, 0, true, false, true, true, true, true);
			}
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 34) && skydiving == true) { // 仅向左移动
			curLocation.x += ((PLAYER_MOVEMENT_VALUES[current_player_jumpfly] / 2) * sin(degToRad(CamRot.z + 90)) * -1.0f);
			curLocation.y += ((PLAYER_MOVEMENT_VALUES[current_player_jumpfly] / 2) * cos(degToRad(CamRot.z + 90)));
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z, 1, 1, 1);
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 35) && skydiving == true) { // 仅向右移动
			curLocation.x += ((PLAYER_MOVEMENT_VALUES[current_player_jumpfly] / 2) * sin(degToRad(CamRot.z - 90)) * -1.0f);
			curLocation.y += ((PLAYER_MOVEMENT_VALUES[current_player_jumpfly] / 2) * cos(degToRad(CamRot.z - 90)));
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, curLocation.x, curLocation.y, curLocation.z, 1, 1, 1);
		}
		if (CONTROLS::IS_CONTROL_RELEASED(2, 33) && CONTROLS::IS_CONTROL_RELEASED(2, 22)) { // 仅向下移动 && 跳跃
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

	// 玩家可以被爆头
	if (featurePlayerCanBeHeadshot && !featurePlayerInvincible) {
		Vector3 coords_bullet_p = PED::GET_PED_BONE_COORDS(playerPed, 31086, 0, 0, 0); // 头骨
		if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2) && GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_bullet_p.x, coords_bullet_p.y, coords_bullet_p.z, 0.1, 0, 0)) {
			PED::CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
			ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(playerPed);
			ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0);
		}
	}
  
	// 受伤玩家移动方式
	if (injured_m == -2) injured_m = current_limp_if_injured;
	if (current_limp_if_injured == 0 && injured_m != 0) injured_m = current_limp_if_injured;

	if ((NPC_RAGDOLL_VALUES[current_limp_if_injured] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) || VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_injured] > 0) {
		float curr_health = ENTITY::GET_ENTITY_HEALTH(playerPed) - 100;
		float curr_set_h = PLAYER_HEALTH_VALUES[current_player_health] - 100;
		if (NPC_RAGDOLL_VALUES[current_limp_if_injured] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
			if (injured_m != current_limp_if_injured) {
				if (current_limp_if_injured == 1) set_status_text("轻度瘸腿走路");
					if (current_limp_if_injured == 2) set_status_text("重度瘸腿走路");
					injured_m = current_limp_if_injured;
					enable_camera_injured = false;
			}

			if (NPC_RAGDOLL_VALUES[current_limp_if_injured] == 1 && !STREAMING::HAS_ANIM_DICT_LOADED("move_injured_generic")) STREAMING::REQUEST_ANIM_DICT("move_injured_generic"); // move_m@injured
				Vector3 coords_calf_l = PED::GET_PED_BONE_COORDS(playerPed, 63931, 0, 0, 0); // 左小腿
				Vector3 coords_calf_r = PED::GET_PED_BONE_COORDS(playerPed, 36864, 0, 0, 0); // 右小腿
			Vector3 coords_pelvis = PED::GET_PED_BONE_COORDS(playerPed, 11816, 0, 0, 0); // 骨盆
			if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(playerPed, 0, 2) && (GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_calf_l.x, coords_calf_l.y, coords_calf_l.z, 0.4, 0, 0) ||
				GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_calf_r.x, coords_calf_r.y, coords_calf_r.z, 0.4, 0, 0) || GAMEPLAY::HAS_BULLET_IMPACTED_IN_AREA(coords_pelvis.x, coords_pelvis.y, coords_pelvis.z, 0.2, 0, 0))) {
				been_injured = true;
			}
			if (NPC_RAGDOLL_VALUES[current_limp_if_injured] == 1 && ((!featurePlayerLife && curr_health < 80) || (featurePlayerLife && curr_health < ((80.0 / 100.0) * curr_set_h)) ||
				been_injured == true)) PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "move_injured_generic", 1.0f); // @walk // 90
			if (NPC_RAGDOLL_VALUES[current_limp_if_injured] == 2 && ((!featurePlayerLife && curr_health < 80) || (featurePlayerLife && curr_health < ((80.0 / 100.0) * curr_set_h)) || been_injured == true) && injured_drunk == false) {
				PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
				STREAMING::REQUEST_ANIM_SET("move_m@drunk@verydrunk");
				while (!STREAMING::HAS_ANIM_SET_LOADED("move_m@drunk@verydrunk")) WAIT(1);
				PED::SET_PED_MOVEMENT_CLIPSET(playerPed, "move_m@drunk@verydrunk", 1.0f);
				injured_drunk = true;
			}
			if (((!featurePlayerLife && curr_health < 30) ||
				(featurePlayerLife && curr_health < ((30.0 / 100.0) * curr_set_h))) && !ENTITY::IS_ENTITY_DEAD(playerPed)) CONTROLS::DISABLE_CONTROL_ACTION(2, 22, 1); // 跳
			if ((!featurePlayerLife && curr_health < 50) || (featurePlayerLife && curr_health < ((50.0 / 100.0) * curr_set_h)) || been_injured == true) CONTROLS::DISABLE_CONTROL_ACTION(2, 21, 1); // 短跑
			if ((!featurePlayerLife && curr_health > 79) || (featurePlayerLife && curr_health > ((80.0 / 100.0) * curr_set_h) - 1) || (time_since_d > 100 && time_since_d < 5000) ||
				(time_since_a > 100 && time_since_a < 5000) || (injured_drunk == true && NPC_RAGDOLL_VALUES[current_limp_if_injured] != 2) || player_died == true) {
				PED::CLEAR_PED_LAST_DAMAGE_BONE(playerPed);
				ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(playerPed);
				been_injured = false;
				injured_drunk = false;
				PED::RESET_PED_MOVEMENT_CLIPSET(playerPed, 1.0f);
			}
		}
		if (curr_cam != VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_injured] || curr_hlth != curr_health) enable_camera_injured = false;
		if (VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_injured] > 0 && enable_camera_injured == false) {
			if ((!featurePlayerLife && curr_health < 80) || (featurePlayerLife && curr_health < ((80.0 / 100.0) * curr_set_h))) {
				float tmp_v = VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_injured];
				CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", tmp_v / 3);
			}
			if ((!featurePlayerLife && curr_health > 79) || (featurePlayerLife && curr_health > ((80.0 / 100.0) * curr_set_h) - 1)) CAM::SHAKE_GAMEPLAY_CAM("DRUNK_SHAKE", 0.0f);
			enable_camera_injured = true;
			curr_cam = VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_injured];
			curr_hlth = curr_health;
		}
	}

	// 可以在公寓里跑步
	if (featurePlayerRunApartments && GAMEPLAY::GET_MISSION_FLAG() == 0 && !UI::IS_HELP_MESSAGE_BEING_DISPLAYED()) {
		int curr_int = INTERIOR::GET_INTERIOR_AT_COORDS(ENTITY::GET_ENTITY_COORDS(playerPed, true).x, ENTITY::GET_ENTITY_COORDS(playerPed, true).y, ENTITY::GET_ENTITY_COORDS(playerPed, true).z);
		if (!INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(ENTITY::GET_ENTITY_COORDS(playerPed, true).x, ENTITY::GET_ENTITY_COORDS(playerPed, true).y, ENTITY::GET_ENTITY_COORDS(playerPed, true).z) &&
			(curr_int == 206849 || curr_int == 166657 || curr_int == 166401 || curr_int == 115458 || curr_int == 114946 || curr_int == 171777 || curr_int == 197121 || curr_int == 197889 || curr_int == 4866 || curr_int == 36866)) {
			iaminside = true;
			if (!ENTITY::IS_ENTITY_IN_AREA(playerPed, -802.346,	171.234, 70.8347, -799.234, 174.817, 74.8347, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -810.195, 169.765, 74.7408, -808.39, 171.398, 78.7408, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -10.91648, -1442.35, 29.1015, -8.14864, -1440.2, 33.1015, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 2.63788, 528.786, 172.628, 5.68781, 530.781, 176.628, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, 1977.13, 3819.58, 31.4501, 1978.99, 3820.1, 35.4501, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -1161.46,	-1521.46, 8.6327, -1160.53, -1518.42, 12.6327, 1, 1, 1) &&
				!ENTITY::IS_ENTITY_IN_AREA(playerPed, -666.436, -932.875, 20.8292, -659.849, -943.25, 22.8292, 1, 1, 1)) {
				if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) < 1) {
					we_have_troubles = false;
					PLAYER::SET_MAX_WANTED_LEVEL(5);
					PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), 1, 0);
					PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
				}

				//if (PED::IS_PED_SHOOTING(playerPed)) we_have_troubles = true;

				if (we_have_troubles == false) {
					GAMEPLAY::CLEAR_AREA_OF_COPS(ENTITY::GET_ENTITY_COORDS(playerPed, true).x, ENTITY::GET_ENTITY_COORDS(playerPed, true).y, ENTITY::GET_ENTITY_COORDS(playerPed, true).z, 20, 0);
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
	
	// 最大通缉等级
	if (PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > VEH_STARSPUNISH_VALUES[wanted_maxpossible_level]) {
		PLAYER::SET_MAX_WANTED_LEVEL(5);
		PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), VEH_STARSPUNISH_VALUES[wanted_maxpossible_level], 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), 0);
	}

  ////////////////////////////////////////// 玩家数据 ///////////////////////////////////////////////////
	if ((bPlayerExists && featurePlayerLife && featurePlayerLifeUpdated) || (bPlayerExists && featurePlayerLife && PLAYER_ARMOR_VALUES[current_player_stats] > -1 && featurePlayerStatsUpdated) || apply_pressed == true || player_d_armour == true) {
		if (!STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) { 
			if ((featurePlayerLifeUpdated && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) || (player_d_armour == true && !ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()))) {
				if (PLAYER_HEALTH_VALUES[current_player_health] > 0) {
					PED::SET_PED_MAX_HEALTH(playerPed, PLAYER_HEALTH_VALUES[current_player_health]);
					ENTITY::SET_ENTITY_HEALTH(playerPed, PLAYER_HEALTH_VALUES[current_player_health]);
				}
				if (detained == false && in_prison == false && PLAYER_ARMOR_VALUES[current_player_armor] > -1) {
					PLAYER::SET_PLAYER_MAX_ARMOUR(playerPed, PLAYER_ARMOR_VALUES[current_player_armor]);
					PED::SET_PED_ARMOUR(playerPed, PLAYER_ARMOR_VALUES[current_player_armor]);
				}
				player_d_armour = false;
			}
			oldplayerPed = playerPed;
			featurePlayerLifeUpdated = false;

			if (PLAYER_ARMOR_VALUES[current_player_stats] > -1 && featurePlayerStatsUpdated && GAMEPLAY::GET_MISSION_FLAG() == 0) {
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
			dynamic_loading = true;
			apply_pressed = false;
		} // end of IS_PLAYER_SWITCH_IN_PROGRESS
	} 
	
	if ((time_since_d > -1 && time_since_d < 2000) || (player_died == true && !featureNoAutoRespawn)) {
		featurePlayerLifeUpdated = true;
		featurePlayerStatsUpdated = true;
		if (!featurePlayerLife) dynamic_loading = true;
		if (detained == false && alert_level == 0) player_died = false;
	}
	
	if ((playerPed != oldplayerPed) || DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) { // 如果你切换角色，你的生命值和护甲将会恢复
		featurePlayerLifeUpdated = true;
		featurePlayerStatsUpdated = true;
		if (!featurePlayerLife) dynamic_loading = true;
	}

	if (PLAYER_ARMOR_VALUES[current_player_stats] > -1 && GAMEPLAY::GET_MISSION_FLAG() == 1 && !SCRIPT::HAS_SCRIPT_LOADED("stats_controller")) {
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

	///// <--- 监狱 逃脱 /////
	prison_break(); 
	
	///// <--- 通缉 逃犯 /////
	most_wanted(); 

	// 警方忽视玩家
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

	// 玩家特殊能力
	if(featurePlayerUnlimitedAbility){
		if(bPlayerExists){
			PLAYER::SPECIAL_ABILITY_FILL_METER(player, 1);
		}
	}

	// 播放器无噪音
	if(bPlayerExists && !featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 1.0);
	}
	if(featurePlayerNoNoise){
		PLAYER::SET_PLAYER_NOISE_MULTIPLIER(player, 0.0);
	}

	// 玩家快速游泳
	if(bPlayerExists && !featurePlayerFastSwim){
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.0);
	}
	if(featurePlayerFastSwim){
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);
	}

	// 玩家快速奔跑
	if(featurePlayerFastRun){
		if (AI::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID())) PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);
		else PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.0);
	}

	// 玩家奔跑速度
	if (PLAYER_MOVEMENT_VALUES[current_player_movement] > 0.00) {
		if (AI::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID())) PED::SET_PED_MOVE_RATE_OVERRIDE(playerPed, PLAYER_MOVEMENT_VALUES[current_player_movement]);
		else PED::SET_PED_MOVE_RATE_OVERRIDE(playerPed, 1.00);
	}

	// 玩家超级跳跃
	if(PLAYER_MOVEMENT_VALUES[current_player_superjump] > 0.00) {
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

	// 禁止布娃娃
	if (noragdoll_m == -2) noragdoll_m = current_no_ragdoll;
	if (current_no_ragdoll == 0 && noragdoll_m != 0) noragdoll_m = current_no_ragdoll;

	if (NPC_RAGDOLL_VALUES[current_no_ragdoll] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		if (noragdoll_m != current_no_ragdoll) {
			if (current_no_ragdoll == 1) set_status_text("坠落动作已启用！");
			if (current_no_ragdoll == 2) set_status_text("坠落动作已禁用！");
			noragdoll_m = current_no_ragdoll;
		}
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 0);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 0);
			FIRE::STOP_ENTITY_FIRE(playerPed);
			PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(playerPed, 1);
			PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(playerPed, 2);
			PED::_SET_PED_RAGDOLL_BLOCKING_FLAGS(playerPed, 4);
			PED::SET_PED_CONFIG_FLAG(playerPed, 104, true);
			PED::SET_PED_CONFIG_FLAG(playerPed, 33, false);
			PED::SET_PED_RAGDOLL_ON_COLLISION(playerPed, false);
			if (NPC_RAGDOLL_VALUES[current_no_ragdoll] == 2 && (PED::IS_PED_FALLING(playerPed) || PED::IS_PED_IN_PARACHUTE_FREE_FALL(playerPed))) AI::CLEAR_PED_TASKS_IMMEDIATELY(playerPed);
		}
	}
	if (NPC_RAGDOLL_VALUES[current_no_ragdoll] == 0) {
		if(bPlayerExists){
			PED::SET_PED_CAN_RAGDOLL(playerPed, 1);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(playerPed, 1);
		}
	}

	// 被射中时布娃娃效果
	if (featureRagdollIfInjured || VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_ragdoll] > 0) {
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
			if (featureRagdollIfInjured) {
				int time1 = (rand() % 3000 + 0); // 上边距 + 下边距
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
			if (VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_ragdoll] > 0) {
				Vector3 coords_me = ENTITY::GET_ENTITY_COORDS(playerPed, true);
				float tmp_v = VEH_TURN_SIGNALS_ACCELERATION_VALUES[feature_shake_ragdoll];
				CAM::SHAKE_GAMEPLAY_CAM("SMALL_EXPLOSION_SHAKE", tmp_v / 5); // JOLT_SHAKE
				been_damaged_by_weapon = false;
				ragdoll_task = false;
				ragdoll_seconds = 0;
			}
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
	 
	//  NPC 被射击像布娃娃一样
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
				PED::SET_PED_TO_RAGDOLL(NPCragdoll[i], time1, time2, ragdollType, true, true, false);
				PED::CLEAR_PED_LAST_DAMAGE_BONE(NPCragdoll[i]);
				ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(NPCragdoll[i]);
			}
		}
	}
		
	// 生命恢复速率
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && REGEN_VALUES[current_regen_speed] != 1.0f) PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(PLAYER::PLAYER_ID(), REGEN_VALUES[current_regen_speed]);

	// 没有潜水装备的面罩
	if (featureNoScubaGearMask && ENTITY::IS_ENTITY_IN_WATER(playerPed) == 1) {
		dive_glasses = true;
		PED::CLEAR_PED_PROP(playerPed, 1);
	}
	if (featureNoScubaGearMask && ENTITY::IS_ENTITY_IN_WATER(playerPed) == 0 && PED::GET_PED_PROP_INDEX(playerPed, 1) > 0 && dive_glasses == false) ped_prop_idx = PED::GET_PED_PROP_INDEX(playerPed, 1); // PED::GET_PED_PROP_INDEX(playerPed, 1) > -1
	if (featureNoScubaGearMask && ENTITY::IS_ENTITY_IN_WATER(playerPed) == 0 && dive_glasses == true) {
		if (ped_prop_idx > 0) PED::SET_PED_PROP_INDEX(playerPed, 1, ped_prop_idx, 0, 0); // ped_prop_idx > -1
		dive_glasses = false;
	}

	// 没有潜水呼吸声
	if (featureNoScubaSound) {
		AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", true);
	}
	else {
		AUDIO::SET_AUDIO_FLAG("SuppressPlayerScubaBreathing", false);
	}

	// 玩家隐形 && 车辆内玩家隐形
	if ((!featurePlayerInvisible && !featurePlayerInvisibleInVehicle && p_invisible == true) || (featurePlayerInvisibleInVehicle && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1) && p_invisible == true)) {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
		p_invisible = false;
	}
	if(featurePlayerInvisible || (featurePlayerInvisibleInVehicle && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1))) {
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
		p_invisible = true;
	}
	
	// 玩家醉酒
	if(featurePlayerDrunkUpdated) {
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

	// 夜视
	if(featureNightVisionUpdated){
		GRAPHICS::SET_NIGHTVISION(featureNightVision);
		featureNightVisionUpdated = false;
	}

	// 热成像
	if(featureThermalVisionUpdated){
		GRAPHICS::SET_SEETHROUGH(featureThermalVision);
		featureThermalVisionUpdated = false;
	}

	// 在死亡时禁用空中刹车
	if(ENTITY::IS_ENTITY_DEAD(playerPed)){
		exit_airbrake_menu_if_showing();
	}

	if (is_hotkey_held_wanted_level() && PLAYER::IS_PLAYER_CONTROL_ON(player)) {
		PED::SET_PED_CAN_SWITCH_WEAPON(PLAYER::PLAYER_PED_ID(), false);
		UI::HIDE_HUD_COMPONENT_THIS_FRAME(19);
		UI::HIDE_HUD_COMPONENT_THIS_FRAME(20);

		PLAYER::SET_MAX_WANTED_LEVEL(5);
		if ((GetKeyState('0') & 0x8000) || (GetKeyState(VK_NUMPAD0) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('1') & 0x8000) || (GetKeyState(VK_NUMPAD1) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 1, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('2') & 0x8000) || (GetKeyState(VK_NUMPAD2) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 2, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('3') & 0x8000) || (GetKeyState(VK_NUMPAD3) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 3, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('4') & 0x8000) || (GetKeyState(VK_NUMPAD4) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 4, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('5') & 0x8000) || (GetKeyState(VK_NUMPAD5) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 5, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
		}
		if ((GetKeyState('6') & 0x8000) || (GetKeyState(VK_NUMPAD6) & 0x8000)) {
			PLAYER::SET_PLAYER_WANTED_LEVEL(player, 0, 0);
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 0);
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
		apply_pressed = true;
		set_status_text("玩家数据已应用！");
	}
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
			ss << "~r~ 警告! 启用手动才能使用!";
			set_status_text(ss.str());
		}
		keyboard_on_screen_already = true;
		curr_message = "输入冲击强度："; // 强力拳击力量
		std::string result_p = show_keyboard("手动输入名称", (char *)lastPowerWeapon.c_str());
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
	const std::string caption = "冲击波选项";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;
	MenuItem<int> *item = new MenuItem<int>();

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePowerPunch;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅限拳脚";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchFists;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅限近战武器";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchMeleeWeapons;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "仅限枪械武器";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePunchFireWeapons;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(WEAPONS_POWERPUNCH_CAPTIONS, onchange_power_punch_index);
	listItem->wrap = false;
	listItem->caption = "超能冲击强度";
	listItem->value = PowerPunchIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "自定义冲击强度";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &activeLineIndexPowerPunchWeapons, caption, onconfirm_powerpunch_menu, NULL, NULL);
}

bool process_player_life_menu(){
	const std::string caption = "玩家数据选项";

	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "加载/重生/切换时应用";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerLife;
	toggleItem->toggleValueUpdated = &featurePlayerLifeUpdated;
	menuItems.push_back(toggleItem);
	
	item = new MenuItem<int>();
	item->caption = "立即应用";
	item->value = -1;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(PLAYER_HEALTH_CAPTIONS, onchange_player_health_mode);
	listItem->wrap = false;
	listItem->caption = "设置玩家血量值";
	listItem->value = current_player_health;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_ARMOR_CAPTIONS, onchange_player_armor_mode);
	listItem->wrap = false;
	listItem->caption = "设置玩家护甲值";
	listItem->value = current_player_armor;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(REGEN_CAPTIONS, onchange_regen_callback);
	listItem->wrap = false;
	listItem->caption = "血量自动恢复速度";
	listItem->value = current_regen_speed;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_ARMOR_CAPTIONS, onchange_player_stats_mode);
	listItem->wrap = false;
	listItem->caption = "角色特殊能力值";
	listItem->value = current_player_stats;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &playerDataMenuIndex, caption, onconfirm_playerData_menu, NULL, NULL);
}

bool maxwantedlevel_menu() {
	const std::string caption = "通缉等级设置";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "冻结通缉等级";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelFrozen;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_wanted_maxpossible_level_mode);
	listItem->wrap = false;
	listItem->caption = "最高通缉等级";
	listItem->value = wanted_maxpossible_level;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "警察无视您";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerIgnoredByPolice;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "无警用直升机";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoPHeli;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "无警用船只";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoPBoats;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "无路障";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedNoPRoadB;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "无特警车辆";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoSWATVehicles;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "警察追捕力度减弱减少";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWantedLevelNoPRam;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &PlayerWantedMaxPossibleLevelMenuIndex, caption, onconfirm_PlayerWantedMaxPossibleLevel_menu, NULL, NULL);
}

bool mostwanted_menu() {
	const std::string caption = "通缉逃犯选项";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerMostWanted;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_mostwanted_level_mode);
	listItem->wrap = false;
	listItem->caption = "被通缉时的等级";
	listItem->value = mostwanted_level_enable;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_mostwanted_mode);
	listItem->wrap = false;
	listItem->caption = "被发现时通缉等级";
	listItem->value = current_player_mostwanted;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "当在逃犯状态时禁用角色切换";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerNoSwitch;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &PlayerMostWantedMenuIndex, caption, onconfirm_PlayerMostWanted_menu, NULL, NULL);
}

bool player_movement_speed() {
	const std::string caption = "玩家移动速度";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "快速游泳";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerFastSwim;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "快速奔跑";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerFastRun;
	menuItems.push_back(toggleItem);
	
	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_superjump_mode);
	listItem->wrap = false;
	listItem->caption = "超级跳跃";
	listItem->value = current_player_superjump;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_jumpfly_mode);
	listItem->wrap = false;
	listItem->caption = "空中飞行";
	listItem->value = current_player_jumpfly;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_MOVEMENT_CAPTIONS, onchange_player_movement_mode);
	listItem->wrap = false;
	listItem->caption = "奔跑速度";
	listItem->value = current_player_movement; 
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "醉酒模式";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePlayerDrunk;
	toggleItem->toggleValueUpdated = &featurePlayerDrunkUpdated;
	menuItems.push_back(toggleItem);
	
	return draw_generic_menu<int>(menuItems, &PlayerMovementMenuIndex, caption, onconfirm_PlayerMovement_menu, NULL, NULL);
}

bool process_ragdoll_menu() {
	const std::string caption = "布娃娃选项";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_no_ragdoll_mode);
	listItem->wrap = false;
	listItem->caption = "布娃娃效果";
	listItem->value = current_no_ragdoll;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "被射击时布娃娃效果";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRagdollIfInjured;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS, onchange_shake_ragdoll_mode);
	listItem->wrap = false;
	listItem->caption = "被射击时晃动镜头";
	listItem->value = feature_shake_ragdoll;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(NPC_RAGDOLL_CAPTIONS, onchange_NPC_ragdoll_mode);
	listItem->wrap = false;
	listItem->caption = "NPC 被射击时布娃娃效果";
	listItem->value = current_npc_ragdoll;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_limp_if_injured_mode);
	listItem->wrap = false;
	listItem->caption = "受伤时瘸腿走路";
	listItem->value = current_limp_if_injured;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS, onchange_shake_injured_mode);
	listItem->wrap = false;
	listItem->caption = "玩家受伤时晃动镜头";
	listItem->value = feature_shake_injured;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, &NPCragdollMenuIndex, caption, onconfirm_NPCragdoll_menu, NULL, NULL);
}

bool process_player_prison_menu(){
	const std::string caption = "玩家越狱选项";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	listItem = new SelectFromListMenuItem(PLAYER_PRISON_CAPTIONS, onchange_player_prison_mode);
	listItem->wrap = false;
	listItem->caption = "玩家入狱条件";
	listItem->value = current_player_prison;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(MISC_PHONE_BILL_CAPTIONS, onchange_player_escapemoney_mode);
	listItem->wrap = false;
	listItem->caption = "越狱失败惩罚款";
	listItem->value = current_player_escapemoney;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_player_escapestars_mode);
	listItem->wrap = false;
	listItem->caption = "越狱后获得的通缉星数";
	listItem->value = current_escape_stars;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PLAYER_DISCHARGE_CAPTIONS, onchange_player_discharge_mode);
	listItem->wrap = false;
	listItem->caption = "坐牢的时间";
	listItem->value = current_player_discharge;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "极限困难模式";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Hardcore;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "警察对囚犯衣服的反应";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Robe;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "行人对囚犯衣服的反应";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePedPrison_Robe;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "监狱里更多的囚犯";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePrison_Yard;
	menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &playerPrisonMenuIndex, caption, onconfirm_playerPrison_menu, NULL, NULL);
}

bool process_player_forceshield_menu() {
	const std::string caption = "超能力选项";

	std::vector<MenuItem<int> *> menuItems;
	SelectFromListMenuItem *listItem;
	MenuItem<int> *item;

	int i = 0;

	item = new MenuItem<int>();
	item->caption = "冲击波";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_MASS_CAPTIONS, onchange_player_forceshield_mode);
	listItem->wrap = false;
	listItem->caption = "玩家能量护盾";
	listItem->value = current_player_forceshieldN;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ANGLE_CAPTIONS, onchange_levitation_index);
	listItem->wrap = false;
	listItem->caption = "磁悬浮";
	listItem->value = LevitationIndex;
	menuItems.push_back(listItem);

	//toggleItem = new ToggleMenuItem<int>();
	//toggleItem->caption = "Levitation";
	//toggleItem->value = i++;
	//toggleItem->toggleValue = &featureLevitation;
	//menuItems.push_back(toggleItem);

	return draw_generic_menu<int>(menuItems, &playerForceshieldMenuIndex, caption, onconfirm_playerForceshield_menu, NULL, NULL);
}

int activeLineIndexPlayer = 0;
int activeLineIndexReset = 0;

bool onconfirm_player_menu(MenuItem<int> choice){
	switch(activeLineIndexPlayer){
		case 0:
			if(process_skinchanger_menu())	return true;
			break;
		case 1:
			heal_player();
			break;
		case 7:
			maxwantedlevel_menu();
			break;
		case 8:
			mostwanted_menu();
			break;
		case 12:
			player_movement_speed();
			break;
		case 13:
			process_ragdoll_menu();
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
		case 21:
			process_player_forceshield_menu();
			break;
		default:
			break;
	}

	return false;
}

void process_player_menu(){
	const int lineCount = 29;

	const std::string caption = "玩家选项";

	StandardOrToggleMenuDef lines[lineCount] = {
		{"更改玩家外观", NULL, NULL, false},
		{"治疗玩家", NULL, NULL, true},
		{"玩家无敌", &featurePlayerInvincible, &featurePlayerInvincibleUpdated, true},
		{"无坠落伤害", &featureNoFallDamage, NULL, true},
		{"无燃烧伤害", &featureFireProof, NULL, true},
		{"增加或减少现金", NULL, NULL, true, CASH},
		{"当前通缉等级", NULL, NULL, true, WANTED},
		{"通缉等级设置", NULL, NULL, false},
		{"通缉逃犯", NULL, NULL, false},
		{"无限能力", &featurePlayerUnlimitedAbility, NULL, true},
		{"无噪音", &featurePlayerNoNoise, NULL, true}, 
		{"可以在公寓奔跑", &featurePlayerRunApartments, NULL, true},
		{"玩家移动速度", NULL, NULL, false},
		{"布娃娃", NULL, NULL, false},
		{"隐身", &featurePlayerInvisible, NULL, true}, 
		{"在车辆内隐身", &featurePlayerInvisibleInVehicle, NULL, true }, 
		{"开启夜视", &featureNightVision, &featureNightVisionUpdated, true},
		{"开启热成像", &featureThermalVision, &featureThermalVisionUpdated, true},
		{"场景动作", NULL, NULL, false},
		{"玩家数据", NULL, NULL, false},
		{"监狱逃脱", NULL, NULL, false},
		{"超能力", NULL, NULL, false},
		{"禁止吹口哨叫出租车", &NoTaxiWhistling, NULL, false},
		{"玩家可以被爆头", &featurePlayerCanBeHeadshot, NULL, false},
		{"手动重生", &featureNoAutoRespawn, NULL },
		{"死亡/被捕后, 立即重生", &featureRespawnsWhereDied, NULL, false},
		{"第一人称, 死亡/被捕视角", &featureFirstPersonDeathCamera, NULL },
		{"无潜水氧气面罩", &featureNoScubaGearMask, NULL, true },
		{"无潜水吸氧呼吸声", &featureNoScubaSound, NULL, true },
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexPlayer, caption, onconfirm_player_menu);
}

// 重置所有选项设置
bool onconfirm_reset_menu(MenuItem<int> choice){
	switch (activeLineIndexReset){
	case 0:
		menu_beep();
		set_menu_showing(true);
		WAIT(200);
		process_main_menu();
		activeLineIndexReset = 0;
		set_menu_showing(false);
		break;
	case 1:
		reset_globals();
		process_main_menu();
		activeLineIndexReset = 0;
		set_menu_showing(false);
		break;
	default:
		break;
	}

	return false;
}

void process_reset_menu(){
	const int lineCount = 7;
	
	const std::string caption = "重置所有选项";
	
	StandardOrToggleMenuDef lines[lineCount] = {
		{ "取消", NULL, NULL, true},
		{ "确认", NULL, NULL, true},
		{ "汉化：随梦&而飞", NULL, NULL, false},
		{ "感谢帮助我的朋友们！", NULL, NULL, false},
		{ "感谢：烈火神君", NULL, NULL, false},
		{ "感谢：羽一大魔王", NULL, NULL, false},
		{ "感谢：开发和维护者们", NULL, NULL, false},
	};
	
	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexReset, caption, onconfirm_reset_menu);
}

//==================
//   主菜单
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
	}
	return false;
}

void process_main_menu(){
	std::ostringstream captionSS;
	captionSS << "~HUD_COLOUR_MENU_YELLOW~     Enhanced ~HUD_COLOUR_WHITE~Native Trainer ~HUD_COLOUR_GREY~Update ";
	captionSS << VERSION_STRING;

	std::vector<MenuItem<int>*> menuItems;

	int i = 0;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "玩家";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "传送";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "武器";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "保镖";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "车辆";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "世界";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "时间";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "物体";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "其他选项";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "重置选项";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	MenuParameters<int> params(menuItems, captionSS.str());
	params.menuSelectionPtr = &activeLineIndexMain;
	params.onConfirmation = onconfirm_main_menu;
	params.sanitiseHeaderText = false;

	draw_generic_menu<int>(params);
}

void reset_globals(){
	reset_skin_globals();

	reset_vehicle_globals();

	reset_veh_weapons_globals();

	reset_time_globals();

	reset_vehmodmenu_globals();

	reset_bodyguards_globals();

	reset_teleporter_globals();

	reset_weapon_globals();

	//if(AIMBOT_INCLUDED){
	//	reset_aimbot_globals();
	//}

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
	current_player_armor = 7;
	current_player_stats = 0;
	current_npc_ragdoll = 0;
	current_limp_if_injured = 0;
	current_no_ragdoll = 0;
	feature_shake_ragdoll = 0;
	feature_shake_injured = 0;
	current_player_movement = 0;
	current_player_jumpfly = 0;
	current_player_superjump = 0;
	current_player_mostwanted = 1;
	mostwanted_level_enable = 1;
	wanted_maxpossible_level = 4;
	current_player_prison = 0;
	current_player_escapemoney = 4;
	current_player_discharge = 3;
	current_escape_stars = 4;

	featurePlayerDrunk =
		featurePlayerInvincible =
		featureNoFallDamage =
		featureFireProof =
		featurePlayerIgnoredByPolice =
		featurePlayerUnlimitedAbility =
		featurePlayerNoNoise =
		featurePlayerMostWanted =
		featurePlayerFastSwim =
		featurePlayerFastRun =
		featurePlayerRunApartments =
		featurePlayerInvisible =
		featurePlayerInvisibleInVehicle =
		featureNightVision =
		featureThermalVision =
		featurePlayerLife =
		featurePrison_Hardcore =
		featureRagdollIfInjured =
		//featureLevitation =
		featureWantedLevelNoPHeli =
		featureWantedNoPRoadB =
		featureWantedLevelNoPBoats =
		featureWantedLevelNoPRam =
		featureWantedLevelNoSWATVehicles =
		NoTaxiWhistling =
		featurePlayerCanBeHeadshot =
		featureRespawnsWhereDied =
		featureNoScubaSound =
		featureNoScubaGearMask =
		featurePrison_Robe =
		featurePedPrison_Robe =
		featureWantedLevelFrozen = false;

		featurePlayerInvincibleUpdated =
		featurePlayerDrunkUpdated =
		featureNightVisionUpdated =
		featureThermalVisionUpdated =
		featurePlayerLifeUpdated =
		featurePrison_Yard = 
		featurePlayerStatsUpdated =
		featurePlayerNoSwitch =
		featureWantedLevelFrozenUpdated = true;

	set_status_text("所有设置已重置为默认！");

	DWORD myThreadID;
	HANDLE myHandle = CreateThread(0, 0, save_settings_thread, 0, 0, &myThreadID);
	CloseHandle(myHandle);
}

void main(){
	//reset_globals();
	
	setGameInputToEnabled(true, true);

	setAirbrakeRelatedInputToBlocked(false, true);

	write_text_to_log_file("获取设置中...");

	set_periodic_feature_call(update_features);

	write_text_to_log_file("设置加载中...");

	load_settings();

	init_vehicle_feature();

	// 遍历车辆池并创建车辆数组
	PopulateVehicleModelsArray();

	write_text_to_log_file("设置加载完成！");

	//这将基于当前应用程序的默认区域设置创建一个新的区域设置
	//(该默认值是在启动时提供的，但可以通过 std::locale::global 覆盖)
	//然后扩展它，添加一个额外的 facet 来控制数值输出。
	std::locale comma_locale(std::locale(), new comma_numpunct());

	// 告诉 cout 使用我们的新区域设置。
	std::cout.imbue(comma_locale);

	if (featureShowStatusMessage) set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT~HUD_COLOUR_WHITE~ 当前版本 ~HUD_COLOUR_MENU_YELLOW~" + VERSION_STRING + "~HUD_COLOUR_WHITE~");
	
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
	write_text_to_log_file("请求转储");

	auto hDbgHelp = LoadLibraryA("调试帮助库");
	if(hDbgHelp == nullptr){
		return;
	}
	auto pMiniDumpWriteDump = (decltype(&MiniDumpWriteDump)) GetProcAddress(hDbgHelp, "写入迷你转储");
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

	write_text_to_log_file("转储已完成");

	return;
}

LONG CALLBACK unhandled_handler(EXCEPTION_POINTERS* e){
	write_text_to_log_file("发生异常！");
	make_minidump(e);
	return EXCEPTION_EXECUTE_HANDLER;
}

int filterException(int code, PEXCEPTION_POINTERS ex){
	set_status_text("糟糕，ENT 程序崩溃了！");

	write_text_to_log_file("脚本主程序 异常");
	make_minidump(ex);
	return EXCEPTION_EXECUTE_HANDLER;
}

void ScriptMain(){
	srand(time(NULL));

	#ifdef _DEBUG
	__try{
		#endif
		
		set_status_text("~HUD_COLOUR_WHITE~欢迎使用 ~HUD_COLOUR_MENU_YELLOW~ENT ~HUD_COLOUR_WHITE~修改器！");
		set_status_text("~HUD_COLOUR_MENU_YELLOW~ENT ~HUD_COLOUR_WHITE~修改器, 正在加载！");

		//If the folder does not exist - this will attempt to make it. This should not fail hence no else.
		std::string folder_path = GetCurrentModulePath() + "Enhanced Native Trainer";
		if (CreateDirectory(folder_path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
			write_text_to_log_file("ENT 文件夹已存在, 请继续！");
	
		clear_log_file();

		write_text_to_log_file("尝试初始化存储");
		init_storage();
		write_text_to_log_file("存储初始化完成");

		database = new ENTDatabase();
		if(!database->open()){
			write_text_to_log_file("无法打开数据库");
			set_status_text("ENT 无法打开数据库 - 正在退出");
			database = NULL;
			return;
		}

		build_anim_tree();

		write_text_to_log_file("脚本主程序调用 - 处理程序设置");

		write_text_to_log_file("正在读取 XML 配置...");
		read_config_file();
		write_text_to_log_file("XML 配置读取完成");

		write_text_to_log_file("正在读取 INI 配置...");
		read_config_ini_file();
		write_text_to_log_file("INI 配置读取完成");

		//UnlockAllObjects();

		//查找无线电跳跃和文件寄存器模式
		SInit();

		const std::string name = "ENT_vehicle_previews.ytd"; 
		std::string fullPath = GetCurrentModulePath() + "Enhanced Native Trainer\\" + name;
		int textureID = 0;

		if (does_file_exist(fullPath.c_str()))
		{
			if (textureID = RegisterFile(fullPath, name))
				write_text_to_log_file("注册的纹理文件： " + fullPath + " 纹理标识符 ID " + std::to_string(textureID));
			else
				write_text_to_log_file("无法注册纹理文件: " + fullPath);
		}
		else
			write_text_to_log_file("无法注册纹理文件: " + fullPath + " 注册文件不存在!");
		
		write_text_to_log_file("查找 shop_controller 脚本");

		if (findShopController())
		{
			write_text_to_log_file("已找到 shop_controller 脚本：正在尝试启用多人游戏车辆");
			enableCarsGlobal();
			write_text_to_log_file("多人游戏车辆已启用");
		}

		main();

		write_text_to_log_file("脚本主程序-已结束");

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

		write_text_to_log_file("脚本整理-已调用");

		save_settings();
		write_text_to_log_file("已保存设置");

		setGameInputToEnabled(true, true);
		setAirbrakeRelatedInputToBlocked(false, true);
		write_text_to_log_file("已重置输入");

		cleanup_script();
		write_text_to_log_file("已清理脚本");
		WAIT(0);
		cleanup_props();
		write_text_to_log_file("已清理道具");
		WAIT(0);
		cleanup_anims();
		write_text_to_log_file("已清理动画");

		if(database != NULL){
			database->close();
			delete database;
			write_text_to_log_file("数据库已终止");
		}

		write_text_to_log_file("脚本整理-完成");
		#ifdef _DEBUG
	}
	__except(filterException(GetExceptionCode(), GetExceptionInformation())){

	}
	#endif
}

void add_player_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvincible", &featurePlayerInvincible, &featurePlayerInvincibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoFallDamage", &featureNoFallDamage});
	results->push_back(FeatureEnabledLocalDefinition{"featureFireProof", &featureFireProof});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelFrozen", &featureWantedLevelFrozen/*, &featureWantedLevelFrozenUpdated*/});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerIgnoredByPolice", &featurePlayerIgnoredByPolice}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoPHeli", &featureWantedLevelNoPHeli});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedNoPRoadB", &featureWantedNoPRoadB});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoPBoats", &featureWantedLevelNoPBoats});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoPRam", &featureWantedLevelNoPRam});
	results->push_back(FeatureEnabledLocalDefinition{"featureWantedLevelNoSWATVehicles", &featureWantedLevelNoSWATVehicles});
	results->push_back(FeatureEnabledLocalDefinition{"NoTaxiWhistling", &NoTaxiWhistling});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoScubaGearMask", &featureNoScubaGearMask});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoScubaSound", &featureNoScubaSound});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerCanBeHeadshot", &featurePlayerCanBeHeadshot});
	results->push_back(FeatureEnabledLocalDefinition{"featureRespawnsWhereDied", &featureRespawnsWhereDied});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerUnlimitedAbility", &featurePlayerUnlimitedAbility});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerNoNoise", &featurePlayerNoNoise}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastSwim", &featurePlayerFastSwim}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerMostWanted", &featurePlayerMostWanted});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerNoSwitch", &featurePlayerNoSwitch});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerFastRun", &featurePlayerFastRun}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerRunApartments", &featurePlayerRunApartments});
	results->push_back(FeatureEnabledLocalDefinition{"featureRagdollIfInjured", &featureRagdollIfInjured}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisible", &featurePlayerInvisible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerInvisibleInVehicle", &featurePlayerInvisibleInVehicle}); 
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerDrunk", &featurePlayerDrunk, &featurePlayerDrunkUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureNightVision", &featureNightVision, &featureNightVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureThermalVision", &featureThermalVision, &featureThermalVisionUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerLife", &featurePlayerLife, &featurePlayerLifeUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Hardcore", &featurePrison_Hardcore});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Robe", &featurePrison_Robe});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedPrison_Robe", &featurePedPrison_Robe});
	results->push_back(FeatureEnabledLocalDefinition{"featurePrison_Yard", &featurePrison_Yard});
	//results->push_back(FeatureEnabledLocalDefinition{"featureLevitation", &featureLevitation});
}

void add_world_feature_enablements3(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{"current_player_health", std::to_string(current_player_health)});
	results->push_back(StringPairSettingDBRow{"current_regen_speed", std::to_string(current_regen_speed)});
	results->push_back(StringPairSettingDBRow{"current_player_armor", std::to_string(current_player_armor)});
	results->push_back(StringPairSettingDBRow{"current_player_stats", std::to_string(current_player_stats)});
	results->push_back(StringPairSettingDBRow{"current_npc_ragdoll", std::to_string(current_npc_ragdoll)});
	results->push_back(StringPairSettingDBRow{"current_no_ragdoll", std::to_string(current_no_ragdoll)});
	results->push_back(StringPairSettingDBRow{"feature_shake_ragdoll", std::to_string(feature_shake_ragdoll)});
	results->push_back(StringPairSettingDBRow{"feature_shake_injured", std::to_string(feature_shake_injured)});
	results->push_back(StringPairSettingDBRow{"current_limp_if_injured", std::to_string(current_limp_if_injured)});
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

	add_vehicle_weapons_enablements(&results);

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
	add_vehmodmenu_generic_settings(&settings);
	handle_generic_settings_teleportation(&settings);
	add_world_feature_enablements2(&settings);
	add_world_feature_enablements3(&settings);
	add_anims_feature_enablements(&settings);
	add_weapon_feature_enablements2(&settings);
	add_bodyguards_feature_enablements2(&settings);
	add_coords_generic_settings(&settings);
	add_misc_generic_settings(&settings);
	add_hotkey_generic_settings(&settings);
	add_props_generic_settings(&settings);
	add_weapons_generic_settings(&settings);
	add_areaeffect_generic_settings(&settings);

	//if(AIMBOT_INCLUDED){
	//	add_aimbot_esp_generic_settings(&settings);
	//}

	add_bodyguards_generic_settings(&settings);
	add_skin_generic_settings(&settings);
	add_anims_generic_settings(&settings);

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
		else if (setting.name.compare("current_limp_if_injured") == 0) {
			current_limp_if_injured = stoi(setting.value);
		}
		else if (setting.name.compare("current_no_ragdoll") == 0) {
			current_no_ragdoll = stoi(setting.value);
		}
		else if (setting.name.compare("feature_shake_ragdoll") == 0) {
			feature_shake_ragdoll = stoi(setting.value);
		}
		else if (setting.name.compare("feature_shake_injured") == 0) {
			feature_shake_injured = stoi(setting.value);
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

	handle_generic_settings_vehmodmenu(&settings);

	handle_generic_settings_world(&settings);

	handle_generic_settings_anims(&settings);

	handle_generic_settings_hotkey(&settings);

	handle_generic_settings_props(&settings);

	handle_generic_settings_weapons(&settings);

	handle_generic_settings_areaeffect(&settings);

	//if(AIMBOT_INCLUDED){
	//	handle_generic_settings_aimbot_esp(&settings);
	//}

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

	write_text_to_log_file("正在保存设置, 开始");

	/*
	if (!db_mutex.try_lock())
	{
		write_text_to_log_file("Couldn't get lock, aborting");
		return;
	}
	*/

	if(database != NULL){
		write_text_to_log_file("实时保存");
		database->store_setting_pairs(get_generic_settings());
		database->store_feature_enabled_pairs(get_feature_enablements());
		write_text_to_log_file("保存标志已释放");
	}
}

void load_settings(){
	handle_generic_settings(database->load_setting_pairs());

	write_text_to_log_file("获得了通用对");

	database->load_feature_enabled_pairs(get_feature_enablements());

	write_text_to_log_file("获得了特征对");

	//db_mutex.unlock();
}

void init_storage(){
	WCHAR* folder = get_storage_dir_path();
	write_text_to_log_file("正在尝试创建存储文件夹");

	std::wstring ws1(folder);
	std::string folderSS1(ws1.begin(), ws1.end());

	write_text_to_log_file(folderSS1);
	if(CreateDirectoryW(folder, NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		write_text_to_log_file("存储目录已创建或已存在");
	}
	else{
		write_text_to_log_file("无法创建存储目录");
	}
	delete folder;

	WCHAR* folder2 = get_temp_dir_path();
	std::wstring ws2(folder2);
	std::string folderSS2(ws2.begin(), ws2.end());

	write_text_to_log_file("正在尝试创建临时文件夹");
	write_text_to_log_file(folderSS2);
	if(CreateDirectoryW(folder2, NULL) || ERROR_ALREADY_EXISTS == GetLastError()){
		write_text_to_log_file("临时目录已创建或已存在");
	}
	else{
		write_text_to_log_file("无法创建临时目录");
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

	set_status_text("生命值已恢复");
}

void toggle_invisibility(){
	featurePlayerInvisible = !featurePlayerInvisible;
	if(featurePlayerInvisible){
		set_status_text("玩家隐身了");
	}
	else{
		set_status_text("玩家不再隐身了");
	}
}

void reset_wanted_level(){
	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
	set_status_text("通缉等级已清除");
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
