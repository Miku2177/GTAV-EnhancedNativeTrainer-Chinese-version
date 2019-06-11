/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
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
/* From Zorg93's "EnableMPCars" code */
#include <cstddef>

const std::vector<std::string> MISC_MUSICEVENT_VALUES{ "AH1_HOLE_RESTART", "AH2A_FIRST_FLOOR_RESTART", "AH3A_ABSEIL_RT", "AH3A_FIB_DOCS_RT", "AH3A_RUBBLE_RT", "AH3B_BURNTOUT_RT", "AH3B_BURNTOUT_TWO_RT", "AH3B_DOWNLOADING_RT",
 "AH3B_EVADE_COPS_RT", "AH3B_GET_TO_VAN_RT", "AH3B_HACK_RT", "AH3B_HELI_LIFT_OFF", "AH3B_STAIRWELL", "APT_COUNTDOWN_30S", "APT_SUDDEN_DEATH_START_MUSIC", "ARM3_CS", "ARM3_RESTART_4", "BG_SIGHTSEER_START_ATTACK", "BST_START",
 "CAR1_COPS_RESTART", "CAR4_TRUCK_RESTART", "CHN1_AFTER_GRENADE_RT", "DH1_START", "DH2A_1ST_BOMB_RT", "DH2A_WAY_OUT_RT", "DH2B_DROP_SUB_RT", "DH2B_FLY_AWAY_RT", "EPS6_START", "EXL2_SNIPE_RT", "EXL3_SWITCH_1", "EXTREME1_BIKE",
 "EXTREME1_RESTART1", "EXTREME2_RESTART2", "FAM1_DO_CHASE_RT", "FAM2_NECK_GRAB", "FAM3_CHASE_RESTART", "FAM4_CHASE_RESTART", "FAM5_YOGA_MUSIC_RESTART", "FBI1_OUTSIDE_CORONERS_RT", "FBI1_SHOOTOUT_HALFWAY_RT", "FBI3_TORTURE_START",
 "FBI4_COVER_RESTART", "FBI5A_FORKLIFT_RESTART", "FBI5A_HELI_RESTART", "FH2A_BANK_MID_RESTART", "FH2A_CARS", "FH2A_FIGHT_RESTART", "FH2A_VAN_RESTART", "FH2B_BOMBS_RESTART", "FH2B_DROP_GOLD_RESTART", "FH2B_FIGHT_1_RESTART",
 "FH2B_HELI_CHASE_RESTART", "FH2B_LEAVE_BANK", "FH2B_NOOSE_FIGHT_RESTART", "FH2B_SWITCH_3", "FIN1_SO_2_RT", "FINA_RESTART_CHASE", "FINB_RESTART_ARRIVE", "FM_COUNTDOWN_30S", "FM_INTRO_START", "FRA1_FIGHT_RESTART", "FRA1_SPEED",
 "FRA2_CUT_LAMAR_RT", "GA_KILL_ALERTED_RS", "GA_KILL_HALF_RS", "HALLOWEEN_START_MUSIC", "JH2A_EXIT_TUNNEL_RESTART", "JH2A_GAS_SHOP_RESTART", "KILL_LIST_START_MUSIC", "LM1_TERMINADOR_ENTER_WAREHOUSE_RESTART", "LM1_TERMINADOR_GAMEPLAY_BEGINS",
 "MGPS_START", "MIC1_ARGUE_CS_SKIP", "MIC1_DRIVE_TO_GRAVEYARD", "MIC2_HANGING_RT", "MIC3_DAVE_ESCAPES_RESTART", "MIC3_ESCAPE_RESTART", "MM2_RESTART1", "MM3_RESTART1", "MP_DM_START_ALL", "OJDA5_START", "OJDG2_START", "PAP2_CAR_RESTART",
 "PENNED_IN_START_MUSIC", "PEYOTE_TRIPS_START", "PROLOGUE_TEST_GETAWAY_RT", "PTP_START", "RC18B_START", "RH1_START", "RH2A_BANK_RESTART", "RH2A_RESCUE_RESTART", "SOL1_CHASE_PLANE_RT", "SOL1_FIGHT_RT", "SOL1_GET_SOL_RT",
 "SOL1_SHOOT_PLANE_RT", "SOL2_RESTART1", "SOL5_FIGHT_BAD_RT", "TRV1_CHASE_BIKERS_RT", "TRV1_DRIVE_TRAILER_RT", "TRV2_RACE", "TRV2_WING_RESTART", "TRV4_EVADE_RT", "TRV4_FOOT_CHASE_RT", "TRV4_START_CS_SKIP" };

const std::vector<std::string> MISC_CUTSCENE_VALUES{ "abigail_mcs_1_concat", "abigail_mcs_2", "ac_ig_3_p3_b", "ah_1_ext_t6", "ah_1_int", "ah_1_mcs_1", "ah_2_ext_alt", "ah_2_ext_p4", /*"ah_3a_explosion",*/ "ah_3a_ext", "ah_3a_int", "ah_3a_mcs_3",
"ah_3a_mcs_6", "ah_3b_ext", "ah_3b_int", "ah_3b_mcs_1", "ah_3b_mcs_2", "ah_3b_mcs_3", "ah_3b_mcs_4", "ah_3b_mcs_5", /*"ah_3b_mcs_6_p1",*/ "ah_3b_mcs_7", "apa_fin_cel_apt2", /*"apa_fin_cel_apt3", "apa_fin_cel_apt4",*/ "apa_hum_fin_int", "apa_nar_int",
"apa_nar_mid", "apa_pri_int", "arena_int", "arena_int2", "armenian_1_int", "armenian_1_mcs_1", "armenian_2_int", "armenian_2_mcs_6", "armenian_3_int",
"armenian_3_mcs_3", "armenian_3_mcs_4", "armenian_3_mcs_5", "armenian_3_mcs_6", "armenian_3_mcs_7", "armenian_3_mcs_8", "armenian_3_mcs_9_concat", "arm_1_mcs_2_concat", "arm_2_mcs_4", "ass_int_2_alt1", "ass_mcs_1", "bar_1_rcm_p2", "bar_2_rcm",
"bar_3_rcm", "bar_4_rcm", "bar_5_rcm_p2", "bmad_intro", "bss_1_mcs_2", "bss_1_mcs_3", "bs_1_int", "bs_2a_2b_int", "bs_2a_ext", "bs_2a_int", "bs_2a_mcs_1", "bs_2a_mcs_10", "bs_2a_mcs_11", "bs_2a_mcs_2", "bs_2a_mcs_3_alt", "bs_2a_mcs_4", "bs_2a_mcs_5",
"bs_2a_mcs_6", "bs_2a_mcs_7_p1", "bs_2a_mcs_8", "bs_2a_mcs_8_p3", "bs_2b_ext_alt1a", "bs_2b_ext_alt2", "bs_2b_int", "bs_2b_mcs_1", "bs_2b_mcs_3", "bunk_int","car_1_ext_concat", "car_1_int_concat", "car_2_mcs_1", "car_4_ext", "car_4_mcs_1", "car_5_ext",
"car_5_mcs_1", "car_steal_3_mcs_1", "car_steal_3_mcs_2", "car_steal_3_mcs_3", "chinese_1_int", "chinese_2_int", "chi_1_mcs_1", "chi_1_mcs_4_concat", "chi_2_mcs_5", "choice_int", "cletus_mcs_1_concat", "club_intro", "club_intro2", "club_open",
"dixn_intro", "dhp1_mcs_1", "drf_mic_1_cs_1", "drf_mic_1_cs_2",
"drf_mic_2_cs_1", "drf_mic_2_cs_2", "drf_mic_3_cs_1", "drf_mic_3_cs_2", "ef_1_rcm", "ef_2_rcm", "ef_3_rcm_concat", "eps_4_mcs_1", "eps_4_mcs_2", "eps_4_mcs_3", "ep_1_rcm_concat", "ep_2_rcm", "ep_3_rcm_alt1", "ep_4_rcm_concat", "ep_5_rcm",
"ep_6_rcm", "ep_7_rcm", "ep_8_rcm", "es_1_rcm_concat", "es_1_rcm_p1", "es_2_rcm_concat", "es_3_mcs_1", "es_3_mcs_2", "es_3_rcm", "es_4_rcm_p1", "es_4_rcm_p2_concat", "exile_1_int", "exile_2_int", "exile_3_int", "exl_1_mcs_1_p3_b", "exl_2_mcs_3",
"exl_3_ext", "family_1_int", "family_2_int", "family_2_mcs_2",  "family_2_mcs_3", "family_2_mcs_4", "family_3_ext", "family_3_int", "family_4_mcs_2", "family_4_mcs_3_concat", "family_5_int", "family_5_mcs_1", "family_5_mcs_2", "family_5_mcs_3",
"family_5_mcs_4", "family_5_mcs_5", "family_5_mcs_5_p4", "family_5_mcs_5_p5", "fam_1_ext_2", "fam_1_ext_alt2", "fam_1_ext_alt3", "fam_1_mcs_2", "fam_3_mcs_1", "fam_4_int_alt1", "fam_5_mcs_6", "fam_6_int", "fam_6_int_p3_t7", "fam_6_mcs_1",
"fam_6_mcs_2_concat", "fam_6_mcs_3", "fam_6_mcs_4", "fam_6_mcs_5", "fam_6_mcs_6", "fam_6_mcs_6_p4_b", "fbi_1_ext", "fbi_1_int", "fbi_1_mcs_1_concat", "fbi_2_ext", "fbi_2_int", "fbi_2_mcs_1", "fbi_2_mcs_2", "fbi_2_mcs_3b",
"fbi_3_int", "fbi_3_mcs_1", "fbi_3_mcs_2", "fbi_3_mcs_3", "fbi_3_mcs_4p2", "fbi_3_mcs_5", "fbi_3_mcs_5p2", "fbi_3_mcs_6p1_b", "fbi_3_mcs_6p2", "fbi_3_mcs_7", "fbi_3_mcs_8", "fbi_4_int", "fbi_4_mcs_2_concat", "fbi_4_mcs_3_concat", "fbi_5a_mcs_1",
"fbi_5a_mcs_10", "fbi_5b_mcs_1", "fbi_5_ext", "fbi_5_int", "fin_a_ext", "fin_a_int", "fin_b_ext", "fin_b_mcs_1_aandb", "fin_b_mcs_2", "fin_c2_mcs_1", "fin_c2_mcs_5", "fin_c_ext", "fin_c_int", "fin_c_mcs_1", /*"fin_c_mcs_1_p1_a",*/
"fin_ext_p1", "fin_ext_p2", "franklin_1_int", "fra_0_int", "fra_0_mcs_1", "fra_0_mcs_4_p2_t3", "fra_0_mcs_5_p1", "fra_1_mcs_1", "fra_2_ext", "fra_2_ig_4_alt1_concat", "fra_2_int", 
"iaa_int", "iaaj_ext", "iaaj_int", "impexp_int", "impexp_int_l1", "hao_mcs_1", "hang_int", "hang_int_plane", "heist_int", "hun_2_mcs_1",
"jh2_fina_mcs4_a1a2", "jh_1_ig_3", "jh_1_int", "jh_1_mcs_4p2", "jh_1_mcs_4_p1_alt1", "jh_2a_intp4", "jh_2a_mcs_1", "jh_2b_int", "jh_2b_mcs_1", "jh_2_arrest_fail_c", "jh_2_celeb", "jh_2_fin_a_mcs4_a1", "jh_end_pt2_a1_p2_a", "josh_1_int_concat",
"josh_2_intp1_t4", "josh_3_intp1", "josh_4_int_concat", "lamar_1_int", "lam_1_mcs_1_concat", "lam_1_mcs_2", "lam_1_mcs_3", "lester_1_int", "les_1a_mcs_0", "les_1a_mcs_1", "les_1a_mcs_2", "les_1a_mcs_3", "les_1a_mcs_4", "les_1b_mcs_1",
"low_drv_ext", "low_drv_int", "low_fin_ext", "low_fin_int", "low_fin_mcs1", "low_fun_ext", "low_fun_int", "low_fun_mcs1", "low_int", "low_pho_ext", "low_pho_int", "low_tra_ext", "low_tra_int",
"lsdhs_mcs_2", "lsdhs_mcs_3_p1_concat", "lsdhs_mcs_3_p2", "lsdh_2a_ext", "lsdh_2a_int", "lsdh_2b_int", "lsdh_2b_mcs_1", "lsdh_int", "martin_1_ext", "martin_1_int", "martin_1_mcs_1", "maude_mcs_1", "maude_mcs_2", "maude_mcs_3",
"maude_mcs_4", "maude_mcs_5", "mic_1_int", "mic_1_mcs_1", "mic_1_mcs_2", "mic_1_mcs_3", "mic_2_int", "mic_2_mcs_1", "mic_2_mcs_3_concat", "mic_3_ext", "mic_3_int", "mic_3_mcs_1_p1_a2", "mic_4_int", "mmb_1_rcm", "mmb_2_rcm",
"mmb_3_rcm", /*"mph_fin_cel_apt",*/ "mph_fin_cel_apt1", "mph_fin_cel_str", "mph_fin_cel_tre", "mph_hum_arm_ext", "mph_hum_del_ext", "mph_hum_emp_ext", "mph_hum_fin_ext", "mph_hum_fin_int", "mph_hum_fin_mcs1",
"mph_hum_int", "mph_hum_key_ext", "mph_hum_key_mcs1", "mph_hum_mid", "mph_hum_val_ext", "mph_nar_bik_ext", "mph_nar_cok_ext", "mph_nar_fin_ext", "mph_nar_fin_int", "mph_nar_int", "mph_nar_met_ext", "mph_nar_mid", "mph_nar_tra_ext", "mph_nar_wee_ext",
"mph_pac_bik_ext", "mph_pac_con_ext", "mph_pac_fin_ext", "mph_pac_fin_int", "mph_pac_fin_mcs0", "mph_pac_fin_mcs1", "mph_pac_fin_mcs2", "mph_pac_hac_ext", "mph_pac_hac_mcs1", "mph_pac_int", "mph_pac_mid", "mph_pac_pho_ext", "mph_pac_wit_mcs1",
"mph_pac_wit_mcs2", "mph_pri_bus_ext", "mph_pri_fin_ext", "mph_pri_fin_int", "mph_pri_fin_mcs1", "mph_pri_fin_mcs2", "mph_pri_int", "mph_pri_mid", "mph_pri_pla_ext", "mph_pri_sta_ext", "mph_pri_sta_mcs1", "mph_pri_sta_mcs2", "mph_pri_unf_ext",
"mph_pri_unf_mcs1", "mph_tut_car_ext", "mph_tut_ext", "mph_tut_fin_int", "mph_tut_int", "mph_tut_mcs1",  "mph_tut_mid", "mp_intro_concat", "mp_intro_mcs_10_a1", "mp_intro_mcs_10_a2", "mp_intro_mcs_10_a3",  "mp_intro_mcs_10_a4","mp_intro_mcs_10_a5",
"mp_intro_mcs_11", /*"mp_intro_mcs_11_a1",*/ "mp_intro_mcs_12_a1", "mp_intro_mcs_12_a2", "mp_intro_mcs_12_a3", "mp_intro_mcs_13", "mp_intro_mcs_14_b", "mp_intro_mcs_16_a1", "mp_intro_mcs_16_a2", "mp_intro_mcs_17_a5", "mp_intro_mcs_17_a8",
"mp_intro_mcs_17_a9", "mp_intro_mcs_8_a1", "mp_intro_mcs_8_a1_cc", "mp_int_mcs_12_a3_3", "mp_int_mcs_12_a3_4", "mp_int_mcs_15_a1_b", "mp_int_mcs_15_a2b", /*"mp_int_mcs_15_a3",*/ /*"mp_int_mcs_15_a4",*/ "mp_int_mcs_17_a1", "mp_int_mcs_17_a2", 
/*"mp_int_mcs_17_a3",*/ /*"mp_int_mcs_17_a4",*/ /*"mp_int_mcs_17_a6",*/ /*"mp_int_mcs_17_a7",*/ "mp_int_mcs_18_a1", /*"mp_int_mcs_18_a2",*/ "mp_int_mcs_5_alt1", "mp_int_mcs_5_alt2", "nmt_1_rcm", "nmt_2_mcs_2", "nmt_2_rcm", "nmt_3_rcm", 
"oscar_mcs_1", "paper_1_rcm_alt1", "pap_1_mcs_1", "pap_1_rcm", "pap_2_mcs_1", "pap_2_rcm_p2", "pap_3_rcm", "pap_4_rcm", "pro_mcs_1",  "pro_mcs_2", "pro_mcs_3_pt1", "pro_mcs_5", "pro_mcs_7_concat", "rbhs_int", "rbhs_mcs_1", 
"rbhs_mcs_3", "rbhs_msc_3_p3", "rbh_2ab_mcs_6", "rbh_2a_ext_1", "rbh_2a_int", "rbh_2a_mcs_2_p3", "rbh_2a_mcs_2_p7",  "rbh_2a_mcs_4", "rbh_2a_mcs_5", "sas_1_rcm_concat", "sas_2_rcm_t7", "scrap_1_rcm", "scrap_2_rcm", 
"sdrm_mcs_2", "sil_int", "sil_pred_mcs1", "silj_ext", "silj_int", "silj_mcs1", "silj_mcs2", "smun_intro", "smun_intro2", "sol_1_ext", "sol_1_int_alt",
"sol_1_mcs_1_concat",  "sol_1_mcs_2", "sol_1_mcs_3",  "sol_2_ext_concat", "sol_2_int_alt1", "sol_3_int", "sol_5_mcs_1", "sol_5_mcs_2", "sol_5_mcs_2_p5", "sub_int", "subj_ext", "subj_mcs0", "subj_mcs1", "tale_intro","tmom_1_rcm", "tmom_2_rcm", 
"tonya_mcs_1", "tonya_mcs_2", "tonya_mcs_3", "trevor_1_int",
"trevor_2_int", "trevor_drive_int", "trv2_mcs_8", "trvram_1", "trvram_2_concat", "trvram_3", "trvram_4", "trvram_5_con", "trv_1_mcs_1_p1", "trv_1_mcs_3_concat", "trv_1_mcs_4", "trv_2_mcs_4_concat", "trv_2_mcs_6", "trv_5_ext", "trv_5_int",
"trv_dri_ext", "trv_dri_mcs_concat" };

const std::vector<std::string> MISC_FILTERS_VALUES{	"DEFAULT", "AmbientPUSH", "BeastIntro01", "CAMERA_secuirity_FUZZ", "DeadlineNeon01", "FIB_B", "glasses_black", "glasses_blue", "glasses_brown", "glasses_Darkblue", "glasses_green", "glasses_orange",
	"glasses_pink", "glasses_purple", "glasses_red", "glasses_yellow", "graveyard_shootout", "hud_def_desatcrunch", "hud_def_flash", "lab_none_dark_OVR", "LostTimeDark", "LostTimeFlash", "METRO_Tunnels", "michealspliff", "mp_bkr_int01_garage",
	"MP_corona_heist_DOF", "MP_corona_switch", "mp_gr_int01_black", "MP_heli_cam", "mugShot", "mugShot_lineup", "New_sewers", "NEW_station_unfinished", "NG_filmic04", "NG_filmic11", "NG_filmic12", "NG_filmic16",
	"NG_filmic17", "NG_filmic18", "NG_filmic19", "plane_inside_mode", "prologue_ending_fog", "prologue_shootout", "rply_brightness_neg", "rply_contrast_neg", "rply_saturation", "rply_saturation_neg", "SALTONSEA", "secret_camera", "shades_pink",
	"StuntFastDark", "switch_cam_2", "torpedo", "trailer_explosion_optimise", "ufo", "ufo_deathray", "underwater_deep", "v_bahama", "V_FIB_stairs", "WATER_cove", "WATER_hills", "WATER_lab", "WATER_militaryPOOP", "WATER_muddy", "WATER_port",
	"WATER_resevoir", "WATER_river", "WATER_salton", "WATER_shore", "WATER_silverlake", "WhiteOut" };

void process_misc_menu();

void reset_misc_globals();

void update_misc_features(BOOL playerExists, Ped playerPed);

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings);

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_default_index(int value, SelectFromListMenuItem* source);

void onchange_misc_def_menutab_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_freeseconds_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_bike_index(int value, SelectFromListMenuItem* source);

bool is_player_reset_on_death();

bool is_input_blocked_in_menu();

bool is_vehicle_preview_enabled();

bool is_controller_ignored_in_trainer();

bool is_hud_hidden();

void set_hud_hidden(bool hidden);

bool is_jellman_scenery_enabled();

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
			return NULL;//table initialisation hasnt happened yet
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

//void FindScriptAddresses();

//void FindPatterns();

//uintptr_t FindPattern2(const char *pattern, const char *mask, const char* startAddress, size_t size);

//uintptr_t FindPattern2(const char *pattern, const char *mask);

//void EnableCarsGlobal();