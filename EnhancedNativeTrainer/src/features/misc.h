/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
#include "..\..\inc\main.h"
#include "..\io\io.h"
#include "..\io\config_io.h"
#include "..\ui_support\menu_functions.h"
#include "..\debug\debuglog.h"
#include "..\storage\database.h"
/* 来源于 Zorg93 的 "EnableMPCars" 代码 */
#include <cstddef>

const std::vector<std::string> MISC_MUSICEVENT_VALUES{ "AH3A_FIB_DOCS_RT", "AH3A_RUBBLE_RT", "AH3B_BURNTOUT_TWO_RT", "AH3B_DOWNLOADING_RT", "AH3B_GET_TO_VAN_RT", "ARM3_RESTART_4", /*"BG_SIGHTSEER_START_ATTACK",*/ "EPS6_START", "EXTREME1_BIKE", "FAM3_CHASE_RESTART", 
"FBI1_OUTSIDE_CORONERS_RT", "FH2A_FIGHT_RESTART", "FH2B_HELI_CHASE_RESTART", "FH2B_LEAVE_BANK", "FINA_RESTART_CHASE", "FRA2_CUT_LAMAR_RT", "JH2A_EXIT_TUNNEL_RESTART", "KILL_LIST_START_MUSIC", "LM1_TERMINADOR_ENTER_WAREHOUSE_RESTART", "MGPS_START", 
"MIC1_DRIVE_TO_GRAVEYARD", "MIC2_HANGING_RT", "MP_DM_START_ALL", "PAP2_CAR_RESTART", "RH1_START", "SOL2_RESTART1", "SOL5_FIGHT_BAD_RT", "TRV1_DRIVE_TRAILER_RT", "TRV2_WING_RESTART" };

const std::vector<std::string> MISC_CUTSCENE_VALUES{ "abigail_mcs_1_concat", "abigail_mcs_2", "ac_ig_3_p3_b", "ah_1_ext_t6", "ah_1_int", "ah_1_mcs_1", "ah_2_ext_alt", "ah_2_ext_p4", "ah_3a_ext", "ah_3a_int", "ah_3a_mcs_3", "ah_3a_mcs_6", "ah_3b_ext", 
"ah_3b_int", "ah_3b_mcs_1", "ah_3b_mcs_2", "ah_3b_mcs_3", "ah_3b_mcs_4", "ah_3b_mcs_5", "ah_3b_mcs_7", "armenian_1_int", "armenian_1_mcs_1", "armenian_2_int", "armenian_2_mcs_6", "armenian_3_int", "armenian_3_mcs_3", "armenian_3_mcs_4", 
"armenian_3_mcs_5", "armenian_3_mcs_6", "armenian_3_mcs_7", "armenian_3_mcs_8", "armenian_3_mcs_9_concat", "arm_1_mcs_2_concat", "arm_2_mcs_4", "ass_int_2_alt1", "ass_mcs_1", "bar_1_rcm_p2", "bar_2_rcm", "bar_3_rcm", "bar_4_rcm", "bar_5_rcm_p2", 
"bss_1_mcs_2", "bss_1_mcs_3", "bs_1_int", "bs_2a_2b_int", "bs_2a_ext", "bs_2a_int", "bs_2a_mcs_1", "bs_2a_mcs_10", "bs_2a_mcs_11", "bs_2a_mcs_2", "bs_2a_mcs_3_alt", "bs_2a_mcs_4", "bs_2a_mcs_5", "bs_2a_mcs_6", "bs_2a_mcs_7_p1", "bs_2a_mcs_8", 
"bs_2a_mcs_8_p3", "bs_2b_ext_alt1a", "bs_2b_ext_alt2", "bs_2b_int", "bs_2b_mcs_1", "bs_2b_mcs_3", "car_1_ext_concat", "car_1_int_concat", "car_2_mcs_1", "car_4_ext", "car_4_mcs_1", "car_5_ext", "car_5_mcs_1", "car_steal_3_mcs_1", "car_steal_3_mcs_2", 
"car_steal_3_mcs_3", "chinese_1_int", "chinese_2_int", "chi_1_mcs_1", "chi_1_mcs_4_concat", "chi_2_mcs_5", "choice_int", "cletus_mcs_1_concat", "dhp1_mcs_1", "drf_mic_1_cs_1", "drf_mic_1_cs_2", "drf_mic_2_cs_1", "drf_mic_2_cs_2", "drf_mic_3_cs_1", 
"drf_mic_3_cs_2", "ef_1_rcm", "ef_2_rcm", "ef_3_rcm_concat", "eps_4_mcs_1", "eps_4_mcs_2", "eps_4_mcs_3", "ep_1_rcm_concat", "ep_2_rcm", "ep_3_rcm_alt1", "ep_4_rcm_concat", "ep_5_rcm", "ep_6_rcm", "ep_7_rcm", "ep_8_rcm", "es_1_rcm_concat", 
"es_1_rcm_p1", "es_2_rcm_concat", "es_3_mcs_1", "es_3_mcs_2", "es_3_rcm", "es_4_rcm_p1", "es_4_rcm_p2_concat", "exile_1_int", "exile_2_int", "exile_3_int", "exl_1_mcs_1_p3_b", "exl_2_mcs_3", "exl_3_ext", "family_1_int", "family_2_int", "family_2_mcs_2", 
"family_2_mcs_3", "family_2_mcs_4", "family_3_ext", "family_3_int", "family_4_mcs_2", "family_4_mcs_3_concat", "family_5_int", "family_5_mcs_1", "family_5_mcs_2", "family_5_mcs_3", "family_5_mcs_4", "family_5_mcs_5", "family_5_mcs_5_p4", 
"family_5_mcs_5_p5", "fam_1_ext_2", "fam_1_ext_alt2", "fam_1_ext_alt3", "fam_1_mcs_2", "fam_3_mcs_1", "fam_4_int_alt1", "fam_5_mcs_6", "fam_6_int", "fam_6_int_p3_t7", "fam_6_mcs_1", "fam_6_mcs_2_concat", "fam_6_mcs_3", "fam_6_mcs_4", "fam_6_mcs_5", 
"fam_6_mcs_6", "fam_6_mcs_6_p4_b", "fbi_1_ext", "fbi_1_int", "fbi_1_mcs_1_concat", "fbi_2_ext", "fbi_2_int", "fbi_2_mcs_1", "fbi_2_mcs_2", "fbi_2_mcs_3b", "fbi_3_int", "fbi_3_mcs_1", "fbi_3_mcs_2", "fbi_3_mcs_3", "fbi_3_mcs_4p2", "fbi_3_mcs_5", 
"fbi_3_mcs_5p2", "fbi_3_mcs_6p1_b", "fbi_3_mcs_6p2", "fbi_3_mcs_7", "fbi_3_mcs_8", "fbi_4_int", "fbi_4_mcs_2_concat", "fbi_4_mcs_3_concat", "fbi_5a_mcs_1", "fbi_5a_mcs_10", "fbi_5b_mcs_1", "fbi_5_ext", "fbi_5_int", "fin_a_ext", "fin_a_int", "fin_b_ext", 
"fin_b_mcs_1_aandb", "fin_b_mcs_2", "fin_c2_mcs_1", "fin_c2_mcs_5", "fin_c_ext", "fin_c_int", "fin_c_mcs_1", "fin_ext_p1", "fin_ext_p2", "franklin_1_int", "fra_0_int", "fra_0_mcs_1", "fra_0_mcs_4_p2_t3", "fra_0_mcs_5_p1", "fra_1_mcs_1", "fra_2_ext", 
"fra_2_ig_4_alt1_concat", "fra_2_int", "hao_mcs_1", "hun_2_mcs_1", "jh2_fina_mcs4_a1a2", "jh_1_ig_3", "jh_1_int", "jh_1_mcs_4p2", "jh_1_mcs_4_p1_alt1", "jh_2a_intp4", "jh_2a_mcs_1", "jh_2b_int", "jh_2b_mcs_1", "jh_2_arrest_fail_c", "jh_2_celeb", 
"josh_1_int_concat", "josh_2_intp1_t4", "josh_3_intp1", "josh_4_int_concat", "lamar_1_int", "lam_1_mcs_1_concat", "lam_1_mcs_2", "lam_1_mcs_3", "lester_1_int", "les_1a_mcs_0", "les_1a_mcs_1", "les_1a_mcs_2", "les_1a_mcs_3", "les_1a_mcs_4", "les_1b_mcs_1",
"lsdhs_mcs_2", "lsdhs_mcs_3_p1_concat", "lsdhs_mcs_3_p2", "lsdh_2a_ext", "lsdh_2a_int", "lsdh_2b_int", "lsdh_2b_mcs_1", "lsdh_int", "martin_1_ext", "martin_1_int", "martin_1_mcs_1", "maude_mcs_1", "maude_mcs_2", "maude_mcs_3", "maude_mcs_4", "maude_mcs_5", 
"mic_1_int", "mic_1_mcs_1", "mic_1_mcs_2", "mic_1_mcs_3", "mic_2_int", "mic_2_mcs_1", "mic_2_mcs_3_concat", "mic_3_ext", "mic_3_int", "mic_3_mcs_1_p1_a2", "mic_4_int", "mmb_1_rcm", "mmb_2_rcm", "mmb_3_rcm", "nmt_1_rcm", "nmt_2_mcs_2", "nmt_2_rcm", "nmt_3_rcm", 
"oscar_mcs_1", "paper_1_rcm_alt1", "pap_1_mcs_1", "pap_1_rcm", "pap_2_mcs_1", "pap_2_rcm_p2", "pap_3_rcm", "pap_4_rcm", "pro_mcs_1",  "pro_mcs_2", "pro_mcs_3_pt1", "pro_mcs_5", "pro_mcs_7_concat", "rbhs_int", "rbhs_mcs_1", "rbhs_mcs_3", "rbhs_msc_3_p3", 
"rbh_2ab_mcs_6", "rbh_2a_ext_1", "rbh_2a_int", "rbh_2a_mcs_2_p3", "rbh_2a_mcs_2_p7", "rbh_2a_mcs_4", "rbh_2a_mcs_5", "sas_1_rcm_concat", "sas_2_rcm_t7", "scrap_1_rcm", "scrap_2_rcm", "sdrm_mcs_2", "sol_1_ext", "sol_1_int_alt", "sol_1_mcs_1_concat",  
"sol_1_mcs_2", "sol_1_mcs_3",  "sol_2_ext_concat", "sol_2_int_alt1", "sol_3_int", "sol_5_mcs_1", "sol_5_mcs_2", "sol_5_mcs_2_p5", "tmom_1_rcm", "tmom_2_rcm", "tonya_mcs_1", "tonya_mcs_2", "tonya_mcs_3", "trevor_1_int", "trevor_2_int", "trevor_drive_int", 
"trv2_mcs_8", "trvram_1", "trvram_2_concat", "trvram_3", "trvram_4", "trvram_5_con", "trv_1_mcs_1_p1", "trv_1_mcs_3_concat", "trv_1_mcs_4", "trv_2_mcs_4_concat", "trv_2_mcs_6", "trv_5_ext", "trv_5_int", "trv_dri_ext", "trv_dri_mcs_concat" };

const std::vector<std::string> MISC_FILTERS_VALUES{ "默认", "AmbientPUSH", "BeastIntro01", "CAMERA_secuirity_FUZZ", "DeadlineNeon01", "glasses_black", "glasses_Darkblue", "glasses_green", "glasses_orange", "glasses_pink", "glasses_purple", "glasses_yellow", 
"graveyard_shootout", "hud_def_desatcrunch", "hud_def_flash", "lab_none_dark_OVR", "LostTimeDark", "LostTimeFlash", "METRO_Tunnels", "michealspliff", "mp_bkr_int01_garage", "MP_corona_heist_DOF", "mp_gr_int01_black", "MP_heli_cam", "mugShot_lineup", "New_sewers", 
"NEW_station_unfinished", "NG_filmic04", "NG_filmic11", "NG_filmic12", "NG_filmic16", "NG_filmic17", "NG_filmic18", "NG_filmic19", "plane_inside_mode", "prologue_ending_fog", "prologue_shootout", "rply_brightness_neg", "rply_contrast_neg", "rply_saturation", 
"SALTONSEA", "secret_camera", "shades_pink", "StuntFastDark", "switch_cam_2", "ufo", "ufo_deathray", "underwater_deep", "v_bahama", "V_FIB_stairs", "WATER_cove", "WATER_hills", "WATER_lab", "WATER_militaryPOOP", "WATER_muddy", "WATER_port", "WATER_resevoir", 
"WATER_shore", "WATER_silverlake", "WhiteOut" };//屏幕滤镜效果

// 修改器控制
const std::vector<std::string> MISC_TRAINERCONTROL_CAPTIONS{ "按下时", "松开时" };
extern int TrainerControlIndex;

const std::vector<std::string> MISC_PHONE_BILL_CAPTIONS{ "10 美元", "50 美元", "100 美元", "500 美元", "1000 美元", "5000 美元", "1万 美元", "5万 美元", "10万 美元", "50万 美元", "100万 美元", "500万 美元", "1000万 美元", "5000万 美元", "1亿 美元" };
const float MISC_PHONE_BILL_VALUES[] = { 10.0, 50.0, 100.0, 500.0, 1000.0, 5000.0, 10000.0, 50000.0, 100000.0, 500000.0, 1000000.0, 5000000.0, 10000000.0, 50000000.0, 100000000.0 };//越狱罚款额

extern const Hash PLAYER_ZERO;
extern const Hash PLAYER_ONE;
extern const Hash PLAYER_TWO;

extern const Hash SP0_TOTAL_CASH;
extern const Hash SP1_TOTAL_CASH;
extern const Hash SP2_TOTAL_CASH;

const std::vector<std::string> MISC_DEF_MENUTAB_CAPTIONS{ "关", "地图", "简讯", "好友", "相册", "游戏", "设置", "统计", "商店", "在线" };
const int MISC_DEF_MANUTAB_VALUES[] = { -2, -1, 1, 2, 3, 5, 6, 10, 18, 42 };
extern int DefMenuTabIndex;

const std::vector<std::string> MISC_PHONE_FREESECONDS_CAPTIONS{ "0", "3", "5", "10", "15" };
const int MISC_PHONE_FREESECONDS_VALUES[] = { 0, 3, 5, 10, 15 };

extern bool featureGamePause;

// 标题字体选项
const std::vector<std::string> MISC_FONT_HEADER_CAPTIONS{ "默认", "标准", "花体", "加粗", "特殊" };
const int MISC_FONT_HEADER_VALUES[] = { 4, 0, 1, 7, 8 };
extern int FontHeaderIndex;
extern bool FontHeaderChanged;

// 项目字体选项
const std::vector<std::string> MISC_FONT_ITEM_CAPTIONS{ "默认", "标准", "花体", "加粗", "特殊" };
const int MISC_FONT_ITEM_VALUES[] = { 0, 4, 1, 7, 8 };
extern int FontItemIndex;
extern bool FontItemChanged;

// 通缉字体选项
const std::vector<std::string> MISC_FONT_WANTED_CAPTIONS{ "默认", "标准", "花体", "加粗", "特殊" };
const int MISC_FONT_WANTED_VALUES[] = { 7, 0, 1, 4, 8 };
extern int FontWantedIndex;
extern bool FontWantedChanged;

// 状态显示字体选项 (用于FPS、时间、燃料等状态信息)
const std::vector<std::string> MISC_FONT_STATUS_CAPTIONS{ "默认", "标准", "花体", "加粗", "特殊" };
const int MISC_FONT_STATUS_VALUES[] = { 4, 0, 1, 7, 8 };
extern int FontStatusIndex;
extern bool FontStatusChanged;

// 菜单显示项目数选项
const std::vector<std::string> MISC_MENU_ITEMS_COUNT_CAPTIONS{ "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "50" };
const int MISC_MENU_ITEMS_COUNT_VALUES[] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };
extern int MenuItemsCountIndex;
extern bool MenuItemsCountChanged;

// 标题栏和菜单项宽度 - 默认 350.0f
const std::vector<std::string> MISC_MENU_WIDTH_CAPTIONS{ "默认", "200", "250", "260", "270", "280", "290", "300", "310", "320", "330", "340", "360", "370", "380", "390", "400", "500", "600", "700", "800", "900", "1000", "1100", "1200" };
const float MISC_MENU_WIDTH_VALUES[] = { 350.0f, 200.0f, 250.0f, 260.0f, 270.0f, 280.0f, 290.0f, 300.0f, 310.0f, 320.0f, 330.0f, 340.0f, 360.0f, 370.0f, 380.0f, 390.0f, 400.0f, 500.0f, 600.0f, 700.0f, 800.0f, 900.0f, 1000.0f, 1100.0f, 1200.0f };
const int MENU_WIDTH_DEFAULT_INDEX = 0;
extern int MenuWidthIndex;
extern bool MenuWidthChanged;

// 标题栏高度 - 默认50.0f
const std::vector<std::string> MISC_MENU_HEIGHT_CAPTIONS{ "默认", "10", "15", "20", "25", "30", "35", "40", "45", "55", "60", "65", "70", "75", "80", "85", "90", "95", "100", "110", "120", "130", "140", "150" };
const float MISC_MENU_HEIGHT_VALUES[] = { 50.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 55.0f, 60.0f, 65.0f, 70.0f, 75.0f, 80.0f, 85.0f, 90.0f, 95.0f, 100.0f, 110.0f, 120.0f, 130.0f, 140.0f, 150.0f };
const int MENU_HEIGHT_DEFAULT_INDEX = 0;
extern int MenuHeightIndex;
extern bool MenuHeightChanged;

// 标题栏顶部偏移量 - 默认 15.0f
const std::vector<std::string> MISC_MENU_TOP_OFFSET_CAPTIONS{ "默认", "0", "5", "10", "20", "25", "30", "35", "40", "45", "50", "55", "60", "65","70", "75", "80", "85", "90", "95", "100", "110", "120", "130", "140", "150" };
const float MISC_MENU_TOP_OFFSET_VALUES[] = { 15.0f, 0.0f, 5.0f, 10.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, 70.0f, 75.0f, 80.0f, 85.0f, 90.0f, 95.0f, 100.0f, 110.0f, 120.0f, 130.0f, 140.0f, 150.0f };
const int MENU_TOP_OFFSET_DEFAULT_INDEX = 0;
extern int MenuTopOffsetIndex;
extern bool MenuTopOffsetChanged;

// 标题栏和菜单项左侧偏移量 - 默认 35.0f
const std::vector<std::string> MISC_MENU_LEFT_OFFSET_CAPTIONS{ "默认", "0", "5", "10", "15", "20", "25", "30", "40", "45", "50", "100", "200", "300", "400", "450", "500", "550", "600", "650", "700", "750", "800", "820", "840", "860", "880",  "900", "910", "920", "930", "940", "950", "960", "970", "980", "990", "1000", "1100", "1200" };
const float MISC_MENU_LEFT_OFFSET_VALUES[] = { 35.0f, 0.0f, 5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 40.0f, 45.0f, 50.0f, 100.0f, 200.0f, 300.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f, 820.0f, 840.0f, 860.0f, 880.0f, 900.0f, 910.0f, 920.0f, 930.0f, 940.0f, 950.0f, 960.0f, 970.0f, 980.0f, 990.0f, 1000.0f, 1100.0f, 1200.0f };
const int MENU_LEFT_OFFSET_DEFAULT_INDEX = 0;
extern int MenuLeftOffsetIndex;
extern bool MenuLeftOffsetChanged;

// 标题文本左侧偏移量 - 默认 45.0f
const std::vector<std::string> MISC_MENU_TEXT_LEFT_OFFSET_CAPTIONS{ "默认", "0", "5",  "10", "15", "20", "25", "30", "35", "40", "50", "55", "60", "110", "210", "310", "410", "460", "510", "560", "610", "660", "710", "760", "810", "830", "850", "870", "890", "910", "920", "930", "940", "950", "960", "970", "980", "990", "1000", "1010", "1110" };
const float MISC_MENU_TEXT_LEFT_OFFSET_VALUES[] = { 45.0f, 0.0f, 5.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 50.0f, 55.0f, 60.0f, 110.0f, 210.0f, 310.0f, 410.0f, 460.0f, 510.0f, 560.0f, 610.0f, 660.0f, 710.0f, 760.0f, 810.0f, 830.0f, 850.0f, 870.0f, 890.0f, 910.0f, 920.0f, 930.0f, 940.0f, 950.0f, 960.0f, 970.0f, 980.0f, 990.0f, 1000.0f, 1010.0f, 1110.0f };
const int MENU_TEXT_LEFT_OFFSET_DEFAULT_INDEX = 0;
extern int MenuTextLeftOffsetIndex;
extern bool MenuTextLeftOffsetChanged;

// 菜单项高度 - 默认 30.0f
const std::vector<std::string> MISC_MENU_ITEM_HEIGHT_CAPTIONS{ "默认", "10", "15", "20", "25", "35", "40", "45", "50", "55", "60" };
const float MISC_MENU_ITEM_HEIGHT_VALUES[] = { 30.0f, 10.0f, 15.0f, 20.0f, 25.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f };
const int MENU_ITEM_HEIGHT_DEFAULT_INDEX = 0;
extern int MenuItemHeightIndex;
extern bool MenuItemHeightChanged;

// 菜单项间距 - 默认 8.0f
const std::vector<std::string> MISC_MENU_ITEM_SPACING_CAPTIONS{ "默认", "0", "1", "2", "3", "4", "5", "6", "7", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "50" };
const float MISC_MENU_ITEM_SPACING_VALUES[] = { 8.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 50.0f };
const int MENU_ITEM_SPACING_DEFAULT_INDEX = 0;
extern int MenuItemSpacingIndex;
extern bool MenuItemSpacingChanged;

// 菜单项文本偏移量 - 默认 10.0f
const std::vector<std::string> MISC_MENU_ITEM_TEXT_OFFSET_CAPTIONS{ "默认", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "25", "30", "35", "40", "45", "50", "55", "60", "65", "70", "75", "80", "85", "90", "95", "100" };
const float MISC_MENU_ITEM_TEXT_OFFSET_VALUES[] = { 10.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, 70.0f, 75.0f, 80.0f, 85.0f, 90.0f, 95.0f, 100.0f };
const int MENU_ITEM_TEXT_OFFSET_DEFAULT_INDEX = 0;
extern int MenuItemTextOffsetIndex;
extern bool MenuItemTextOffsetChanged;

// 菜单项与标题距离 - 默认 75.0f
const std::vector<std::string> MISC_MENU_ITEM_TOP_OFFSET_CAPTIONS{ "默认", "10", "20", "25", "30", "35", "40", "45", "50", "55", "60", "65", "70", "80", "85", "90", "95", "100", "105", "110", "115", "120", "125", "130", "135", "140", "145" };
const float MISC_MENU_ITEM_TOP_OFFSET_VALUES[] = { 75.0f, 20.0f, 25.0f, 30.0f, 35.0f, 40.0f, 45.0f, 50.0f, 55.0f, 60.0f, 65.0f, 70.0f, 80.0f, 85.0f, 90.0f, 95.0f, 100.0f, 105.0f, 110.0f, 115.0f, 120.0f, 125.0f, 130.0f, 135.0f, 140.0f, 145.0f };
const int MENU_ITEM_TOP_OFFSET_DEFAULT_INDEX = 0;
extern int MenuItemTopOffsetIndex;
extern bool MenuItemTopOffsetChanged;

// 预览图左右判断依据 - 默认 500.0f
const std::vector<std::string> MISC_PREVIEW_POSITION_THRESHOLD_CAPTIONS{ "默认", "200", "300", "400", "450", "550", "600", "650", "700", "750", "800", "850", "900" };
const float MISC_PREVIEW_POSITION_THRESHOLD_VALUES[] = { 500.0f, 200.0f, 300.0f, 400.0f, 450.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f, 850.0f, 900.0f };
const int PREVIEW_POSITION_THRESHOLD_DEFAULT_INDEX = 0;
extern int PreviewPositionThresholdIndex;
extern bool PreviewPositionThresholdChanged;

// 预览图分辨率适配值 - 默认 1920.0f
const std::vector<std::string> MISC_PREVIEW_RESOLUTION_SCALE_CAPTIONS{ "默认 (1080P)", "2560 (2k)", "3840 (4K)", "1760", "1680", "1600", "1440", "1366", "1280", "2816", "3072", "3200", "3328",  };
const float MISC_PREVIEW_RESOLUTION_SCALE_VALUES[] = { 1920.0f, 2560.0f, 3840.0f, 1760.0f, 1680.0f, 1600.0f, 1440.0f, 1366.0f, 1280.0f, 2816.0f, 3072.0f, 3200.0f, 3328.0f };
const int PREVIEW_RESOLUTION_SCALE_DEFAULT_INDEX = 0;
extern int PreviewResolutionScaleIndex;
extern bool PreviewResolutionScaleChanged;

// 预览图间距 - 默认 8.0f
const std::vector<std::string> MISC_PREVIEW_SPACING_CAPTIONS{ "默认", "0", "1", "2", "3", "4", "5", "6", "7", "9", "10", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41", "42", "43", "44", "45", "50" };
const float MISC_PREVIEW_SPACING_VALUES[] = { 8.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f, 10.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f, 17.0f, 18.0f, 19.0f, 20.0f, 21.0f, 22.0f, 23.0f, 24.0f, 25.0f, 26.0f, 27.0f, 28.0f, 29.0f, 30.0f, 31.0f, 32.0f, 33.0f, 34.0f, 35.0f, 36.0f, 37.0f, 38.0f, 39.0f, 40.0f, 41.0f, 42.0f, 43.0f, 44.0f, 45.0f, 50.0f };
const int PREVIEW_SPACING_DEFAULT_INDEX = 0;
extern int PreviewSpacingIndex;
extern bool PreviewSpacingChanged;

void process_misc_menu();

void reset_misc_globals();

void update_misc_features(BOOL playerExists, Ped playerPed);

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings);

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_default_index(int value, SelectFromListMenuItem* source);

void onchange_misc_radio_off_index(int value, SelectFromListMenuItem* source);

void onchange_misc_radio_switching_index(int value, SelectFromListMenuItem* source);

void onchange_misc_trainercontrol_index(int value, SelectFromListMenuItem* source);

void onchange_misc_trainercontrolscrolling_index(int value, SelectFromListMenuItem* source);

void onchange_misc_def_menutab_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_freeseconds_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_bike_index(int value, SelectFromListMenuItem* source);

void onchange_misc_font_header_index(int value, SelectFromListMenuItem* source);

void onchange_misc_font_item_index(int value, SelectFromListMenuItem* source);

void onchange_misc_menu_item_top_offset_index(int value, SelectFromListMenuItem* source);

void onchange_misc_font_wanted_index(int value, SelectFromListMenuItem* source);

void onchange_misc_font_status_index(int value, SelectFromListMenuItem* source);
void onchange_misc_menu_items_count_index(int value, SelectFromListMenuItem* source);

void onchange_misc_preview_position_threshold_index(int value, SelectFromListMenuItem* source);
void onchange_misc_preview_resolution_scale_index(int value, SelectFromListMenuItem* source);
void onchange_misc_preview_spacing_index(int value, SelectFromListMenuItem* source);

void process_misc_font_settings_menu();

void process_misc_main_key_settings_menu();

void process_misc_menu_layout_settings_menu();

void process_misc_vehicle_preview_settings_menu();

bool is_vehicle_preview_enabled();

bool is_hud_hidden();

void set_hud_hidden(bool hidden);

void HUD_switching();

void Traffic_switching();

bool is_jellman_scenery_enabled();

void reset_menu_layout_to_defaults();// 新增：菜单布局默认重置的统一函数声明（供重置/全局重置调用）

extern bool g_MenuLayoutNeedsRefresh;// 新增：菜单布局设置界面刷新标志（用于在重置后强制重建菜单项显示）

extern bool radio_v_checked;
extern bool featureRealisticRadioVolume;
extern std::string screenfltr;

extern bool dynamic_loading;

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
	unsigned char* GetCodePageAddress(int page) const { return codeBlocksOffset[page]; }
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
			return NULL; //表格初始化尚未发生
		}
		for (int i = 0; i<count; i++)
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
	__int64* AddressOf(int index) const { return &GlobalBasePtr[index >> 18 & 0x3F][index & 0x3FFFF]; }
	bool IsInitialised()const { return *GlobalBasePtr != NULL; }
};

//电台曲目切换
void SkipRadioFwd1(uint32_t a1);
static void SkipRadioFwd2Internal(uintptr_t a1, uint32_t a2);
void SkipRadioFwd2(uint32_t a1);
void SKIP_RADIO_FORWARD_CUSTOM();
bool CompareMemoryJACCO(const uint8_t* pData, const uint8_t* bMask, const char* sMask);
intptr_t FindPatternJACCO(const char* bMask, const char* sMask);
void SInit();

extern bool show_transparency;
