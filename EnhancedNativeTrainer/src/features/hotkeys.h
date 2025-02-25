/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#pragma once

#include <vector>
#include "..\storage\\database.h"

struct HOTKEY_DEF
{
	std::string caption;
	int id;
};

bool is_hotkey_held_saved_veh_spawn();

bool is_hotkey_held_slow_mo();

bool is_hotkey_held_half_normal_speed();

bool is_hotkey_held_normal_speed();

bool is_hotkey_held_drop_mine();

bool is_hotkey_held_openclose_door();

bool is_hotkey_held_wanted_level();

bool is_hotkey_toggled_speed();

bool is_hotkey_held_veh_burnout();

bool is_hotkey_held_veh_extrapower();

bool is_hotkey_held_veh_nitrous();

bool is_hotkey_held_veh_radio_skip();

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
static const int HKEY_TELEPORT_TO_VEHICLE = 115;
static const int HKEY_MENU_NEXT_RADIO_TRACK = 116;
static const int HKEY_TOGGLE_THERMAL_VIS = 117;
static const int HKEY_TOGGLE_NIGHT_VIS = 118;
static const int HKEY_SLOW_MOTION = 120;
static const int HKEY_HALF_NORMAL_SPEED = 121;
static const int HKEY_NORMAL_SPEED = 122;
static const int HKEY_VEHICLE_POWER = 123;
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
static const int HKEY_HANDBRAKE_SWITCHING = 140;
static const int HKEY_BURNOUT_SWITCHING = 141;
static const int HKEY_ALARM_SWITCHING = 142;
static const int HKEY_TRACTIONCONTROL_SWITCHING = 143;
static const int HKEY_DELETE_CHECKPOINT = 144;
static const int HKEY_CLEAR_PROPS = 145;
static const int HKEY_ENTER_DAMAGED_V = 146;
static const int HKEY_FIRE_MODE = 147;
static const int HKEY_MAP_SIZE = 148;
static const int HKEY_SPAWN_BODYGUARD = 149;
static const int HKEY_ADDNEAR_BODYGUARD = 150;
static const int HKEY_DISMISS_BODYGUARD = 151;
static const int HKEY_MUFFLED_RADIO = 152;
static const int HKEY_VEHICLE_NITROUS = 153;
static const int HKEY_VEHICLE_BOMB = 154;
static const int HKEY_VEHICLE_POLICE = 155;
static const int HKEY_BODYGUARD_DIS_ARMED = 156;
static const int HKEY_SPAWN_SAVED_CAR = 157;
static const int HKEY_SPAWN_VEHICLE_MANUALLY = 158;
static const int HKEY_DROP_MINE = 159;
static const int HKEY_DOORS_OPENCLOSE_TOGGLE = 160;
static const int HKEY_TIME_FREEZEUNFREEZE_TOGGLE = 161;
static const int HKEY_DOORS_WANTEDLEVEL_TOGGLE = 162;
static const int HKEY_WANTEDLEVEL_FROZEN_TOGGLE = 163;
static const int HKEY_HUD_TOGGLE = 164;
static const int HKEY_TRAFFIC_TOGGLE = 165;

const std::vector<HOTKEY_DEF> HOTKEY_AVAILABLE_FUNCS{
	{ "无功能", 0 },
	{ "修复车辆", HKEY_FIX_CAR },
	{ "清洁车辆", HKEY_CLEAN_CAR },
	{ "治疗玩家", HKEY_HEAL_PLAYER },
	{ "清除通缉等级", HKEY_RESET_WANTED },
	{ "重复之前的动作", HKEY_REPLAY_ANIM },
	{ "传送到导航点", HKEY_TELEPORT_TO_MARKER },
	{ "传送到任务点", HKEY_TELEPORT_TO_MISSION_MARKER },
	{ "以乘客传送到附近车辆", HKEY_TELEPORT_TO_VEHICLE_AS_PASSENGER },
	{ "传送到最后驾驶的车辆", HKEY_TELEPORT_TO_VEHICLE },
	{ "隐身模式 (开关)", HKEY_TOGGLE_INVISIBILITY },
	{ "补充弹药", HKEY_FILL_AMMO },
	{ "下一首电台曲目", HKEY_MENU_NEXT_RADIO_TRACK },
	{ "夜视仪 (开关)", HKEY_TOGGLE_NIGHT_VIS },
	{ "热成像 (开关)", HKEY_TOGGLE_THERMAL_VIS },
	{ "慢动作（按住 按键）", HKEY_SLOW_MOTION},
	{ "半倍速度（按住 按键）", HKEY_HALF_NORMAL_SPEED },
	{ "正常速度（按住 按键）", HKEY_NORMAL_SPEED },
	{ "车辆提高动力（按住 按键）", HKEY_VEHICLE_POWER },
	{ "杀死附近所有的行人", HKEY_KILL_NEARBY_PEDS },
	{ "引爆附近所有的载具", HKEY_EXPLODE_NEARBY_VEHS },
	{ "翻转载具", HKEY_VEHICLE_FLIP },
	{ "速度限制", HKEY_SPEEDLIMITER_SWITCHING },
	{ "无敌模式 (开关)", HKEY_INVINCIBILITY_SWITCHING },
	{ "车辆引擎 (启停)", HKEY_ENGINEONOFF_SWITCHING },
	{ "毁坏引擎", HKEY_ENGINE_KILL },
	{ "主驾驶车窗 (升降)", HKEY_WINDOW_ROLL },
	{ "车内灯光 (开关)", HKEY_INTERIOR_LIGHT },
	{ "探照灯光 (开关)", HKEY_SEARCH_LIGHT },
	{ "获得所有的武器", HKEY_GIVE_ALL_WEAPONS },
	{ "切换狙击枪开镜视角", HKEY_SNIPER_VISION_TOGGLE },
	{ "锁定车门 (锁车)", HKEY_DOORS_LOCKED_TOGGLE },
	{ "切换座位", HKEY_SEAT_CHANGE_TOGGLE },
	{ "切换全局游戏速度", HKEY_TOGGLED_SPEED },
	{ "车辆手刹 (开关)", HKEY_HANDBRAKE_SWITCHING },
	{ "烧胎模式 (开关)", HKEY_BURNOUT_SWITCHING },
	{ "车辆警报 (开关)", HKEY_ALARM_SWITCHING },
	{ "切换牵引力控制", HKEY_TRACTIONCONTROL_SWITCHING },
	{ "移除导航路径线", HKEY_DELETE_CHECKPOINT },
	{ "删除所有的饰品", HKEY_CLEAR_PROPS },
	{ "进入损毁的车辆", HKEY_ENTER_DAMAGED_V },
	{ "射击模式", HKEY_FIRE_MODE },
	{ "小地图尺寸", HKEY_MAP_SIZE },
	{ "生成保镖", HKEY_SPAWN_BODYGUARD },
	{ "添加附近行人为保镖", HKEY_ADDNEAR_BODYGUARD },
	{ "解散所有的保镖", HKEY_DISMISS_BODYGUARD },
	{ "电台静音", HKEY_MUFFLED_RADIO },
	{ "氮气加速 (按住 按键)", HKEY_VEHICLE_NITROUS },
	{ "投掷炸弹", HKEY_VEHICLE_BOMB },
	{ "警灯 (开关)", HKEY_VEHICLE_POLICE },
	{ "添加/移除 保镖武器  ", HKEY_BODYGUARD_DIS_ARMED },
	{ "生成已保存的车辆", HKEY_SPAWN_SAVED_CAR },
	{ "手动生成车辆", HKEY_SPAWN_VEHICLE_MANUALLY },
	{ "放置地雷 [HK + 1-6]", HKEY_DROP_MINE },
	{ "载具车门 (开关) [HK + 1-6]", HKEY_DOORS_OPENCLOSE_TOGGLE },
	{ "时间 冻结/解冻", HKEY_TIME_FREEZEUNFREEZE_TOGGLE },
	{ "通缉等级设置 [HK + 0-5]", HKEY_DOORS_WANTEDLEVEL_TOGGLE },
	{ "冻结/解冻 通缉等级", HKEY_WANTEDLEVEL_FROZEN_TOGGLE },
	{ "隐藏/显示 小地图", HKEY_HUD_TOGGLE },
	{ "城市交通 (开关)", HKEY_TRAFFIC_TOGGLE },
	{ "HK 是指字母上方的数字键", 0 },
	{ "按住绑定的快捷键键 + HK 1-6", 0 },
	{ "就能使用对应的功能了", 0 },
};

void change_hotkey_function(int hotkey, int funcIndex);

int get_hotkey_function_index(int hotkey);

void check_for_hotkey_presses();

void add_hotkey_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_hotkey(std::vector<StringPairSettingDBRow>* settings);

void trigger_function_for_hotkey_onkeyup(int i);

void trigger_function_for_hotkey_onkeydown(int i);

extern bool hotkey_boddyguard;