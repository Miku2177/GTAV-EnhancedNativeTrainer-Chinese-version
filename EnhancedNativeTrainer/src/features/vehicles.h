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
#include "..\ui_support\menu_functions.h"
#include "..\storage\database.h"
#include "..\ent-enums.h"
#include "..\utils.h"
#include "..\common\ENTUtil.h"
#include <string>
#include <random>

struct PaintColor{
	int colorIndex;
	std::string name;
};

extern const std::vector<PaintColor> PAINTS_BY_TYPE[7];

struct NeonLightsColor{
	std::string colorString;
	int rVal, gVal, bVal;
};

struct TireSmokeColor{
	std::string colorString;
	int rVal, gVal, bVal;
};

struct WheelSelection{
	int wheelIndex;
	std::string name;
};

struct XenonColour {
	std::string colorString;
	int colour;
};
extern const std::vector<std::string> VALUES_SUPERCARS;

const std::vector<std::string> VEH_MASS_CAPTIONS{ "1x", "3x", "5x", "10x", "30x", "50x", "Mayhem" };
const std::vector<int> VEH_MASS_VALUES{ 0, 5, 10, 30, 50, 100, 50000 };
extern int VehMassMultIndex;
extern bool massChanged;
extern int current_player_forceshield;
extern bool player_massChanged;

const std::vector<std::string> VEH_SPEEDLIMITER_CAPTIONS{ "OFF", "5 (MPH)", "10 (MPH)", "15 (MPH)", "20 (MPH)", "25 (MPH)", "30 (MPH)", "35 (MPH)", "40 (MPH)", "45 (MPH)", "50 (MPH)", "55 (MPH)", "60 (MPH)", "65 (MPH)", "70 (MPH)", "75 (MPH)", "80 (MPH)", "85 (MPH)", "90 (MPH)", "95 (MPH)", "100 (MPH)", "105 (MPH)", "110 (MPH)", "115 (MPH)", "120 (MPH)", "125 (MPH)", "130 (MPH)", "135 (MPH)", "140 (MPH)", "145 (MPH)", "150 (MPH)", "160 (MPH)", "170 (MPH)", "180 (MPH)", "190 (MPH)", "200 (MPH)" };
const std::vector<int> VEH_SPEEDLIMITER_VALUES{ 0, 2, 4, 7, 9, 11, 13, 16, 18, 20, 22, 24, 27, 29, 31, 33, 36, 38, 40, 42, 44, 46, 48, 51, 53, 55, 57, 59, 61, 63, 66, 70, 75, 80, 85, 89 };
extern int speedLimiterIndex;
extern bool speedLimiterChanged;
extern int DoorAutolockIndex;
extern bool DoorAutolockChanged;
extern int speedCityLimiterIndex;
extern bool speedCityLimiterChanged;
extern int speedCountryLimiterIndex;
extern bool speedCountryLimiterChanged;
extern Vehicle veh_l;
extern bool featureLockVehicleDoors;
extern bool featureLockVehicleDoorsUpdated;

// engine power stuff
const std::vector<std::string> VEH_ENG_POW_CAPTIONS{ "OFF", "1x", "5x", "10x", "25x", "50x", "75x", "100x", "125x", "150x", "175x", "200x", "225x", "250x", "275x", "300x", "325x", "350x", "375x", "400x" };
const std::vector<int> VEH_ENG_POW_VALUES{ -1, 0, 5, 10, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400 };
extern int engPowMultIndex;
extern bool powChanged;

//Blip Colour
const std::vector<std::string> VEH_BLIPCOLOUR_CAPTIONS{ "White", "Red", "Green", "Blue", "Orange", "Purple", "Grey", "Brown", "Pink", "Dark Green", "Dark Purple", "Dark Blue" };
const std::vector<int> VEH_BLIPCOLOUR_VALUES{ 0, 1, 2, 3, 17, 19, 20, 21, 23, 25, 27, 29 };
extern int VehBlipColourIndex;
extern bool VehBlipColour_Changed;

// Vehicle Colour
const std::vector<std::string> VEH_COLOUR_CAPTIONS{ "OFF", "White", "Red", "Green", "Blue", "Orange", "Purple", "Pink", "Yellow" };
const std::vector<int> VEH_COLOUR_VALUES{ -1, 134, 27, 139, 64, 38, 145, 135, 88 };
extern int VehColourIndex;
extern bool VehColour_Changed;

struct VehicleImage {
	char* modelName;
	char* dict;
	char* imgName;
	int localID;
};

static std::vector<VehicleImage> ALL_VEH_IMAGES;

const std::vector<XenonColour> XENON_COLOURS = {
	{ "White", 0 },
	{ "Blue", 1 },
	{ "Electric Blue", 2 },
	{ "Mint Green", 3 },
	{ "Lime Green", 4 },
	{ "Yellow", 5 },
	{ "Golden Shower", 6 },
	{ "Orange", 7 },
	{ "Red", 8 },
	{ "Pony Pink", 9 },
	{ "Hot Pink", 10 },
	{ "Purple", 11 },
	{ "Blacklight", 12 },
	{ "Stock", 255 }
};

const std::vector<NeonLightsColor> NEON_COLORS = { 
	{ "Bright White", NEON_COLOR_WHITE }, 
	{ "Dim White", NEON_COLOR_BLACK }, 
	{ "Electric Blue", NEON_COLOR_ELECTRICBLUE }, 
	{ "Mint Green", NEON_COLOR_MINTGREEN }, 
	{ "Lime Green", NEON_COLOR_LIMEGREEN },
	{ "Yellow", NEON_COLOR_YELLOW }, 
	{ "Gold", NEON_COLOR_GOLDENSHOWER }, 
	{ "Orange", NEON_COLOR_ORANGE }, 
	{ "Red", NEON_COLOR_RED }, 
	{ "Pink", NEON_COLOR_PONYPINK }, 
	{ "Hot Pink", NEON_COLOR_HOTPINK },
	{ "Purple", NEON_COLOR_PURPLE }, 
	{ "Black Light", NEON_COLOR_BLACKLIGHT } };

const std::vector<TireSmokeColor> SMOKE_COLORS = {
	{ "White", TIRESMOKE_COLOR_WHITE },
	{ "Black", TIRESMOKE_COLOR_BLACK },
	{ "Blue", TIRESMOKE_COLOR_BLUE },
	{ "Yellow", TIRESMOKE_COLOR_YELLOW },
	{ "Purple", TIRESMOKE_COLOR_PURPLE },
	{ "Orange", TIRESMOKE_COLOR_ORANGE },
	{ "Green", TIRESMOKE_COLOR_GREEN },
	{ "Red", TIRESMOKE_COLOR_RED },
	{ "Pink", TIRESMOKE_COLOR_PINK },
	{ "Brown", TIRESMOKE_COLOR_BROWN },
	{ "Patriot", TIRESMOKE_COLOR_PATRIOT }
};

//car name, DLC name for texture (scaleform_web.rpf > the name of the ytd file), name of car in file - no need for extracting files!
const std::vector<VehicleImage> INGAME_VEH_IMAGES =
{
	{ "AIRBUS", "candc_default", "airbus" },
	{ "BARRACKS", "candc_default", "barracks" },
	{ "BOXVILLE4", "candc_default", "boxville4" },
	{ "BUS", "candc_default", "bus" },
	{ "BUZZARD", "candc_default", "buzzard" },
	{ "CARGOBOB", "candc_default", "cargobob" },
	{ "COACH", "candc_default", "coach" },
	{ "CRUSADER", "candc_default", "crusader" },
	{ "DUBSTA3", "candc_default", "dubsta3" },
	{ "DUKES2", "candc_default", "dukes" },
	{ "DUMP", "candc_default", "dump" },
	{ "HYDRA", "candc_default", "hydra" },
	{ "INSURGENT", "candc_default", "insurgent" },
	{ "INSURGENT2", "candc_default", "insurgent2" },
	{ "JOURNEY", "candc_default", "journey" },
	{ "MARSHALL", "candc_default", "marshall" },
	{ "MESA", "candc_default", "mesa" },
	{ "MESA3", "candc_default", "mesa3" },
	{ "MONSTER", "candc_default", "monster" },
	{ "MULE", "candc_default", "mule" },
	{ "MULE3", "candc_default", "mule3" },
	{ "PBUS", "candc_default", "pbus" },
	{ "RENTALBUS", "candc_default", "rentbus" },
	{ "RHINO", "candc_default", "rhino" },
	{ "SAVAGE", "candc_default", "savage" },
	{ "TECHNICAL", "candc_default", "technical" },
	{ "VALKYRIE", "candc_default", "valkyrie" },
	{ "DINGHY3", "dock_default", "dinghy3" },
	{ "JETMAX", "dock_default", "jetmax" },
	{ "MARQUIS", "dock_default", "marquis" },
	{ "SEASHARK", "dock_default", "seashark" },
	{ "SPEEDER", "dock_default", "speeder" },
	{ "SQUALO", "dock_default", "squalo" },
	{ "SUBMERSIBLE2", "dock_default", "sub2" },
	{ "SUNTRAP", "dock_default", "suntrap" },
	{ "TROPIC", "dock_default", "tropic" },
	{ "ANNIHILATOR", "elt_default", "annihl" },
	{ "CUBAN800", "elt_default", "cuban800" },
	{ "DODO", "elt_default", "dodo" },
	{ "DUSTER", "elt_default", "duster" },
	{ "FROGGER", "elt_default", "frogger" },
	{ "LUXOR", "elt_default", "luxor" },
	{ "MAMMATUS", "elt_default", "mammatus" },
	{ "MAVERICK", "elt_default", "maverick" },
	{ "SHAMAL", "elt_default", "shamal" },
	{ "STUNT", "elt_default", "stunt" },
	{ "TITAN", "elt_default", "titan" },
	{ "VELUM", "elt_default", "velum" },
	{ "VELUM2", "elt_default", "velum2" },
	{ "VESTRA", "elt_dlc_business", "vestra" },
	{ "LUXOR2", "elt_dlc_luxe", "luxor2" },
	{ "SWIFT2", "elt_dlc_luxe", "swift2" },
	{ "BESRA", "elt_dlc_pilot", "besra" },
	{ "MILJET", "elt_dlc_pilot", "miljet" },
	{ "SWIFT", "elt_dlc_pilot", "swift" },
	{ "ADDER", "lgm_default", "adder" },
	{ "BANSHEE", "lgm_default", "banshee" },
	{ "BULLET", "lgm_default", "bullet" },
	{ "CARBONIZZARE", "lgm_default", "carboniz" },
	{ "CARBONRS", "lgm_default", "carbon" },
	{ "CHEETAH", "lgm_default", "cheetah" },
	{ "COGCABRIO", "lgm_default", "cogcabri" },
	{ "COMET2", "lgm_default", "comet2" },
	{ "COQUETTE", "lgm_default", "coquette" },
	{ "ELEGY2", "lgm_default", "elegy2" },
	{ "ENTITYXF", "lgm_default", "entityxf" },
	{ "EXEMPLAR", "lgm_default", "exemplar" },
	{ "FELTZER2", "lgm_default", "feltzer" },
	{ "HOTKNIFE", "lgm_default", "hotknife" },
	{ "JB700", "lgm_default", "jb700" },
	{ "KHAMELION", "lgm_default", "khamel" },
	{ "MONROE", "lgm_default", "monroe" },
	{ "NEMESIS", "sssa_dlc_heist", "nemesis" },
	{ "NINEF", "lgm_default", "ninef" },
	{ "NINEF2", "lgm_default", "ninef2" },
	{ "RAPIDGT", "lgm_default", "rapidgt" },
	{ "RAPIDGT2", "lgm_default", "rapidgt2" },
	{ "STINGER", "lgm_default", "stinger" },
	{ "STINGERGT", "lgm_default", "stingerg" },
	{ "VOLTIC", "lgm_default", "voltic_tless" },
	{ "ZTYPE", "lgm_default", "ztype" },
	{ "ALPHA", "lgm_dlc_business", "alpha" },
	{ "JESTER", "lgm_dlc_business", "jester" },
	{ "TURISMOR", "lgm_dlc_business", "turismor" },
	{ "COQUETTE2", "lgm_dlc_pilot", "coquette2" },
	{ "HUNTLEY", "lgm_dlc_business2", "huntley" },
	{ "MASSACRO", "lgm_dlc_business2", "massacro" },
	{ "THRUST", "lgm_dlc_business2", "thrust" },
	{ "ZENTORNO", "lgm_dlc_business2", "zentorno" },
	{ "FUROREGT", "lgm_dlc_lts_creator", "furore" },
	{ "FELTZER3", "lgm_dlc_luxe", "feltzer3" },
	{ "OSIRIS", "lgm_dlc_luxe", "osiris" },
	{ "VIRGO", "lgm_dlc_luxe", "virgo" },
	{ "WINDSOR", "lgm_dlc_luxe", "windsor" },
	{ "BMX", "pandm_default", "bmx" },
	{ "CRUISER", "pandm_default", "cruiser" },
	{ "SCORCHER", "pandm_default", "scorcher" },
	{ "TRIBIKE", "pandm_default", "tribike" },
	{ "TRIBIKE2", "pandm_default", "tribike2" },
	{ "TRIBIKE3", "pandm_default", "tribike3" },
	{ "AKUMA", "sssa_default", "akuma" },
	{ "BALLER", "sssa_default", "baller2" },
	{ "BATI", "sssa_default", "bati" },
	{ "BATI2", "sssa_default", "bati2" },
	{ "BFINJECTION", "sssa_default", "bfinject" },
	{ "BIFTA", "sssa_default", "bifta" },
	{ "BISON", "sssa_default", "bison" },
	{ "BLAZER", "sssa_default", "blazer" },
	{ "BODHI2", "sssa_default", "bodhi2" },
	{ "CAVALCADE", "sssa_default", "cavcade" },
	{ "DILETTANTE", "sssa_default", "dilettan" },
	{ "DOUBLE", "sssa_default", "double" },
	{ "DUNE", "sssa_default", "dune" },
	{ "FAGGIO2", "sssa_default", "faggio" },
	{ "FELON", "sssa_default", "felon" },
	{ "FELON2", "sssa_default", "felon2" },
	{ "FUGITIVE", "sssa_default", "fugitive" },
	{ "GAUNTLET", "sssa_default", "gauntlet" },
	{ "HEXER", "sssa_default", "hexer" },
	{ "INFERNUS", "sssa_default", "infernus" },
	{ "ISSI2", "sssa_default", "issi2" },
	{ "KALAHARI", "sssa_default", "kalahari" },
	{ "ORACLE", "sssa_dlc_heist", "oracle1" },
	{ "ORACLE2", "sssa_default", "oracle" },
	{ "PARADISE", "sssa_default", "paradise" },
	{ "PCJ", "sssa_default", "pcj" },
	{ "REBEL", "sssa_default", "rebel" },
	{ "ROCOTO", "sssa_default", "rocoto" },
	{ "RUFFIAN", "sssa_default", "ruffian" },
	{ "SADLER", "sssa_default", "sadler" },
	{ "SANCHEZ", "sssa_default", "sanchez" },
	{ "SANCHEZ2", "sssa_default", "sanchez2" },
	{ "SANDKING", "sssa_default", "sandking" },
	{ "SANDKING2", "sssa_default", "sandkin2" },
	{ "SCHWARZER", "sssa_default", "schwarze" },
	{ "STRETCH", "sssa_default", "stretch" },
	{ "SUPERD", "lgm_default", "superd" },
	{ "SURANO", "lgm_default", "surano_convertable" },
	{ "VACCA", "lgm_default", "vacca" },
	{ "VADER", "sssa_default", "vader" },
	{ "VIGERO", "sssa_default", "vigero" },
	{ "ZION", "sssa_default", "zion" },
	{ "ZION2", "sssa_default", "zion2" },
	{ "ASEA", "sssa_dlc_business", "asea" },
	{ "ASTEROPE", "sssa_dlc_business", "astrope" },
	{ "BOBCATXL", "sssa_dlc_business", "bobcatxl" },
	{ "CAVALCADE2", "sssa_dlc_business", "cavcade2" },
	{ "INGOT", "sssa_dlc_business", "ingot" },
	{ "INTRUDER", "sssa_dlc_business", "intruder" },
	{ "MINIVAN", "sssa_dlc_business", "minivan" },
	{ "PREMIER", "sssa_dlc_business", "premier" },
	{ "RADI", "sssa_dlc_business", "radi" },
	{ "RANCHERXL", "sssa_dlc_business", "rancherx" },
	{ "STANIER", "sssa_dlc_business", "stanier" },
	{ "STRATUM", "sssa_dlc_business", "stratum" },
	{ "WASHINGTON", "sssa_dlc_business", "washingt" },
	{ "DOMINATOR", "sssa_dlc_business2", "dominato" },
	{ "F620", "sssa_dlc_business2", "f620" },
	{ "FUSILADE", "sssa_dlc_business2", "fusilade" },
	{ "PENUMBRA", "sssa_dlc_business2", "penumbra" },
	{ "SENTINEL", "sssa_dlc_business2", "sentinel" },
	{ "JESTER2", "sssa_dlc_christmas_2", "jester2" },
	{ "MASSACRO2", "sssa_dlc_christmas_2", "massacro2" },
	{ "RATLOADER2", "sssa_dlc_christmas_2", "rloader2" },
	{ "SLAMVAN", "sssa_dlc_christmas_2", "slamvan" },
	{ "ENDURO", "sssa_dlc_heist", "enduro" },
	{ "GBURRITO2", "sssa_dlc_heist", "gburrito2" },
	{ "GRESLEY", "sssa_dlc_heist", "gresley" },
	{ "JACKAL", "sssa_dlc_heist", "jackal" },
	{ "KURUMA", "sssa_dlc_heist", "kuruma" },
	{ "KURUMA2", "sssa_dlc_heist", "kuruma2" },
	{ "LANDSTALKER", "sssa_dlc_heist", "landstalker" },
	{ "RUMPO", "sssa_dlc_heist", "rumpo" },
	{ "SCHAFTER2", "sssa_dlc_heist", "schafter2" },
	{ "SEMINOLE", "sssa_dlc_heist", "seminole" },
	{ "SURGE", "sssa_dlc_heist", "surge" },
	{ "BLADE", "sssa_dlc_hipster", "blade" },
	{ "BLAZER3", "sssa_dlc_hipster", "blazer3" },
	{ "BUFFALO", "sssa_dlc_hipster", "buffalo" },
	{ "BUFFALO2", "sssa_dlc_hipster", "buffalo2" },
	{ "GLENDALE", "sssa_dlc_hipster", "glendale" },
	{ "PANTO", "sssa_dlc_hipster", "panto" },
	{ "PICADOR", "sssa_dlc_hipster", "picador" },
	{ "PIGALLE", "sssa_dlc_hipster", "pigalle" },
	{ "PRIMO", "sssa_dlc_hipster", "primo" },
	{ "REBEL2", "sssa_dlc_hipster", "rebel2" },
	{ "REGINA", "sssa_dlc_hipster", "regina" },
	{ "RHAPSODY", "sssa_dlc_hipster", "rhapsody" },
	{ "SURFER", "sssa_dlc_hipster", "surfer" },
	{ "TAILGATER", "sssa_dlc_hipster", "tailgater" },
	{ "WARRENER", "sssa_dlc_hipster", "warrener" },
	{ "YOUGA", "sssa_dlc_hipster", "youga" },
	{ "SOVEREIGN", "sssa_dlc_independence", "sovereign" },
	{ "HAKUCHOU", "sssa_dlc_lts_creator", "hakuchou" },
	{ "INNOVATION", "sssa_dlc_lts_creator", "innovation" },
	{ "BLISTA2", "sssa_dlc_mp_to_sp", "blista2" },
	{ "BUFFALO3", "sssa_dlc_mp_to_sp", "buffalo3" },
	{ "DOMINATOR2", "sssa_dlc_mp_to_sp", "dominator2" },
	{ "GAUNTLET2", "sssa_dlc_mp_to_sp", "gauntlet2" },
	{ "STALION", "sssa_dlc_mp_to_sp", "stallion" },
	{ "STALION2", "sssa_dlc_mp_to_sp", "stalion2" },
	{ "RATLOADER", "sssa_dlc_valentines", "rloader" },
	{ "CASCO", "lgm_dlc_heist", "casco" },
	{ "CHINO", "lgm_dlc_luxe", "chino" },
	{ "COQUETTE3", "lgm_dlc_luxe", "coquette3" },
	{ "T20", "lgm_dlc_luxe", "t20" },
	{ "VINDICATOR", "lgm_dlc_luxe", "vindicator" },
	{ "BRAWLER", "lgm_dlc_luxe", "brawler" },
	{ "TORO", "dock_default", "toro" },
	//Missing preview images
	{ "AIRTUG", "ENT_vehicle_previews", "AIRTUG" },
	{ "AMBULANCE", "ENT_vehicle_previews", "VP_AMBULANCE" },
	{ "ARMYTANKER", "ENT_vehicle_previews", "VP_ARMYTANKER" },
	{ "ARMYTRAILER", "ENT_vehicle_previews", "VP_ARMYTANKER" },
	{ "ARMYTRAILER2", "ENT_vehicle_previews", "ARMYTRAILER2" },
	{ "ASEA2", "ENT_vehicle_previews", "VP_ASEA_SNOW" },
	{ "BAGGER", "ENT_vehicle_previews", "BAGGER" },
	{ "BALETRAILER", "ENT_vehicle_previews", "VP_BALETRAILER" },
	{ "BALLER2", "ENT_vehicle_previews", "VP_BALLER_SMALL" },
	{ "BARRACKS2", "ENT_vehicle_previews", "VP_BARRACKS2" },
	{ "BENSON", "ENT_vehicle_previews", "VP_BENSON" },
	{ "BIFF", "ENT_vehicle_previews", "VP_BIFF" },
	{ "BISON2", "ENT_vehicle_previews", "VP_BISON2" },
	{ "BISON3", "ENT_vehicle_previews", "VP_BISON3" },
	{ "BJXL", "ENT_vehicle_previews", "VP_BJXL" },
	{ "BLIMP", "ENT_vehicle_previews", "VP_BLIMP" },
	{ "BLIMP2", "ENT_vehicle_previews", "VP_BLIMP2" },
	{ "BLISTA", "ENT_vehicle_previews", "VP_BLISTA" },
	{ "BOATTRAILER", "ENT_vehicle_previews", "VP_BOATTRAILER" },
	{ "BOXVILLE2", "ENT_vehicle_previews", "VP_BOXVILLE2" },
	{ "BOXVILLE3", "ENT_vehicle_previews", "VP_BOXVILLE3" },
	{ "BLISTA3", "ENT_vehicle_previews", "VP_MONKEY_BLISTA" },
	{ "BTYPE", "ENT_vehicle_previews", "VP_ROOSEVELT" },
	{ "BUCCANEER", "ENT_vehicle_previews", "VP_BUCCANEER" },
	{ "BURRITO", "ENT_vehicle_previews", "VP_BURRITO" },
	{ "BURRITO3", "ENT_vehicle_previews", "VP_BURRITO3" },
	{ "BURRITO4", "ENT_vehicle_previews", "VP_BURRITO4" },
	{ "BURRITO5", "ENT_vehicle_previews", "VP_BURRITO5" },
	{ "BUZZARD2", "ENT_vehicle_previews", "VP_BUZZARD2" },
	{ "CABLECAR", "ENT_vehicle_previews", "VP_CABLECAR" },
	{ "CADDY", "ENT_vehicle_previews", "VP_CADDY" },
	{ "CADDY2", "ENT_vehicle_previews", "VP_CADDY2" },
	{ "CAMPER", "ENT_vehicle_previews", "VP_CAMPER" },
	{ "CARGOBOB2", "ENT_vehicle_previews", "VP_CARGOBOB2" },
	{ "CARGOBOB3", "ENT_vehicle_previews", "VP_CARGOBOB3" },
	{ "CUTTER", "ENT_vehicle_previews", "VP_CUTTER" },
	{ "CARGOPLANE", "ENT_vehicle_previews", "VP_CARGO" },
	{ "DAEMON", "ENT_vehicle_previews", "VP_DAEMON" },
	{ "DILETTANTE2", "ENT_vehicle_previews", "VP_DILET_LIVERY" },
	{ "DINGHY", "ENT_vehicle_previews", "VP_DINGHY" },
	{ "DINGHY2", "ENT_vehicle_previews", "VP_DINGHY2" },
	{ "HANDLER", "ENT_vehicle_previews", "VP_DOCKHANDLER" },
	{ "DOCKTRAILER", "ENT_vehicle_previews", "VP_DOCKTRAILER" },
	{ "DOCKTUG", "ENT_vehicle_previews", "VP_DOCKTUG" },
	{ "BULLDOZER", "ENT_vehicle_previews", "VP_BULLDOZER" },
	{ "DLOADER", "ENT_vehicle_previews", "VP_DUNELOADER" },
	{ "DUBSTA", "ENT_vehicle_previews", "VP_DUBSTA" },
	{ "DUBSTA2", "ENT_vehicle_previews", "VP_DUBSTA_BLACK" },
	{ "DUKES", "ENT_vehicle_previews", "VP_DUKES" },
	{ "DUNE2", "ENT_vehicle_previews", "VP_SPACEDOCKER" },
	{ "EMPEROR", "ENT_vehicle_previews", "VP_EMPEROR" },
	{ "EMPEROR2", "ENT_vehicle_previews", "VP_EMPEROR_RUSTY" },
	{ "EMPEROR3", "ENT_vehicle_previews", "VP_EMPEROR_SNOW" },
	{ "FBI", "ENT_vehicle_previews", "VP_FBI" },
	{ "FBI2", "ENT_vehicle_previews", "VP_FBI2" },
	{ "FIXTER", "ENT_vehicle_previews", "VP_FIXTER" },
	{ "FLATBED", "ENT_vehicle_previews", "VP_FLATBED" },
	{ "FORKLIFT", "ENT_vehicle_previews", "VP_FORKLIFT" },
	{ "FQ2", "ENT_vehicle_previews", "VP_FQ2" },
	{ "FREIGHT", "ENT_vehicle_previews", "VP_FREIGHT" },
	{ "FREIGHTCAR", "ENT_vehicle_previews", "VP_FREIGHTCAR" },
	{ "FREIGHTCONT1", "ENT_vehicle_previews", "VP_FREIGHTCONT1" },
	{ "FREIGHTCONT2", "ENT_vehicle_previews", "VP_FREIGHTCONT2" },
	{ "FREIGHTGRAIN", "ENT_vehicle_previews", "VP_FREIGHTGRAIN" },
	{ "FREIGHTTRAILER", "ENT_vehicle_previews", "VP_FREIGHTTRAILER" },
	{ "FROGGER2", "ENT_vehicle_previews", "VP_FROGGER2" },
	{ "FUTO", "ENT_vehicle_previews", "VP_FUTO" },
	{ "GBURRITO", "ENT_vehicle_previews", "VP_GBURRITO" },
	{ "GRAINTRAILER", "ENT_vehicle_previews", "VP_GRAINTRAILER" },
	{ "GRANGER", "ENT_vehicle_previews", "VP_GRANGER" },
	{ "GUARDIAN", "ENT_vehicle_previews", "VP_GUARDIAN" },
	{ "HABANERO", "ENT_vehicle_previews", "VP_HABANERO" },
	{ "HAULER", "ENT_vehicle_previews", "VP_HAULER" },
	{ "JET", "ENT_vehicle_previews", "VP_JET" },
	{ "LAZER", "ENT_vehicle_previews", "VP_LAZER" },
	{ "LECTRO", "ENT_vehicle_previews", "VP_LECTRO" },
	{ "MANANA", "ENT_vehicle_previews", "VP_MANANA" },
	{ "MESA2", "ENT_vehicle_previews", "VP_MESA_SNOW" },
	{ "METROTRAIN", "ENT_vehicle_previews", "VP_METROTRAIN" },
	{ "MIXER", "ENT_vehicle_previews", "VP_MIXER" },
	{ "MIXER2", "ENT_vehicle_previews", "VP_MIXER2" },
	{ "MOWER", "ENT_vehicle_previews", "VP_MOWER" },
	{ "MULE2", "ENT_vehicle_previews", "VP_MULE2" },
	{ "ORACLE2", "ENT_vehicle_previews", "VP_ORACLE2" },
	{ "PACKER", "ENT_vehicle_previews", "VP_PACKER" },
	{ "PATRIOT", "ENT_vehicle_previews", "VP_PATRIOT" },
	{ "PEYOTE", "ENT_vehicle_previews", "VP_PEYOTE" },
	{ "PHANTOM", "ENT_vehicle_previews", "VP_PHANTOM" },
	{ "PHOENIX", "ENT_vehicle_previews", "VP_PHOENIX" },
	{ "POLICE", "ENT_vehicle_previews", "VP_POLICE" },
	{ "POLICE2", "ENT_vehicle_previews", "VP_POLICE2" },
	{ "POLICE3", "ENT_vehicle_previews", "VP_POLICE3" },
	{ "POLICE4", "ENT_vehicle_previews", "VP_POLICE4" },
	{ "POLICEB", "ENT_vehicle_previews", "VP_POLICEB" },
	{ "POLICEOLD1", "ENT_vehicle_previews", "VP_POLICEOLD1" },
	{ "POLICEOLD2", "ENT_vehicle_previews", "VP_POLICEOLD2" },
	{ "POLICET", "ENT_vehicle_previews", "VP_POLICET" },
	{ "POLMAV", "ENT_vehicle_previews", "VP_POLMAV" },
	{ "PONY", "ENT_vehicle_previews", "VP_PONY" },
	{ "PONY2", "ENT_vehicle_previews", "VP_PONY2" },
	{ "POUNDER", "ENT_vehicle_previews", "VP_POUNDER" },
	{ "PRAIRIE", "ENT_vehicle_previews", "VP_PRAIRIE" },
	{ "PRANGER", "ENT_vehicle_previews", "VP_PRANGER" },
	{ "PREDATOR", "ENT_vehicle_previews", "VP_PREDATOR" },
	{ "PROPTRAILER", "ENT_vehicle_previews", "VP_PROPTRAILER" },
	{ "RAKETRAILER", "ENT_vehicle_previews", "VP_RAKETRAILER" },
	{ "RANCHERXL2", "ENT_vehicle_previews", "VP_RANCHERXL_SNOW" },
	{ "RIOT", "ENT_vehicle_previews", "VP_RIOT" },
	{ "RIPLEY", "ENT_vehicle_previews", "VP_RIPLEY" },
	{ "ROMERO", "ENT_vehicle_previews", "VP_HEARSE" },
	{ "RUINER", "ENT_vehicle_previews", "VP_RUINER" },
	{ "RUBBLE", "ENT_vehicle_previews", "VP_RUBBLE" },
	{ "RUMPO2", "ENT_vehicle_previews", "VP_RUMPO2" },
	{ "SABREGT", "ENT_vehicle_previews", "VP_SABRE" },
	{ "SADLER2", "ENT_vehicle_previews", "VP_SADLER_SNOW" },
	{ "SCRAP", "ENT_vehicle_previews", "VP_SCRAP" },
	{ "SEASHARK2", "ENT_vehicle_previews", "VP_SEASHARK2" },
	{ "SENTINEL2", "ENT_vehicle_previews", "VP_SENTINEL" },
	{ "SERRANO", "ENT_vehicle_previews", "VP_SERRANO" },
	{ "SHERIFF", "ENT_vehicle_previews", "VP_SHERIFF" },
	{ "SHERIFF2", "ENT_vehicle_previews", "VP_SHERIFF2" },
	{ "SKYLIFT", "ENT_vehicle_previews", "VP_SKYLIFT" },
	{ "SLAMVAN2", "ENT_vehicle_previews", "VP_SLAMVAN_LOSTMC" },
	{ "SPEEDO", "ENT_vehicle_previews", "VP_SPEEDO" },
	{ "SPEEDO2", "ENT_vehicle_previews", "VP_SPEEDO2" },
	{ "SPEEDO4", "ENT_vehicle_previews", "VP_SPEEDO" },
	{ "STOCKADE3", "ENT_vehicle_previews", "VP_STOCKADE3" },
	{ "SUBMERSIBLE", "ENT_vehicle_previews", "VP_SUB" },
	{ "SULTAN", "ENT_vehicle_previews", "VP_SULTAN" },
	{ "SURFER2", "ENT_vehicle_previews", "VP_SURFER2" },
	{ "TACO", "ENT_vehicle_previews", "VP_TACO" },
	{ "TANKER", "ENT_vehicle_previews", "VP_TANKER" },
	{ "TANKER2", "ENT_vehicle_previews", "VP_TANKER2" },
	{ "TANKERCAR", "ENT_vehicle_previews", "VP_TANKERCAR" },
	{ "TAXI", "ENT_vehicle_previews", "VP_TAXI" },
	{ "TIPTRUCK", "ENT_vehicle_previews", "VP_TIPTRUCK" },
	{ "TIPTRUCK2", "ENT_vehicle_previews", "VP_TIPTRUCK2" },
	{ "TORNADO", "ENT_vehicle_previews", "VP_TORNADO" },
	{ "TORNADO2", "ENT_vehicle_previews", "VP_TORNADO_CAB" },
	{ "TORNADO3", "ENT_vehicle_previews", "VP_TORNADO_RUSTY" },
	{ "TORNADO4", "ENT_vehicle_previews", "VP_TORNADO_CAB_RUSTY" },
	{ "TOURBUS", "ENT_vehicle_previews", "VP_TOURBUS" },
	{ "TOWTRUCK", "ENT_vehicle_previews", "VP_TOWTRUCK" },
	{ "TOWTRUCK2", "ENT_vehicle_previews", "VP_TOWTRUCK2" },
	{ "TR2", "ENT_vehicle_previews", "VP_TR2" },
	{ "TR3", "ENT_vehicle_previews", "VP_TR3" },
	{ "TR4", "ENT_vehicle_previews", "VP_TR4" },
	{ "TRACTOR", "ENT_vehicle_previews", "VP_TRACTOR" },
	{ "TRACTOR2", "ENT_vehicle_previews", "VP_TRACTOR2" },
	{ "TRACTOR3", "ENT_vehicle_previews", "VP_TRACTOR3" },
	{ "TRAILERLOGS", "ENT_vehicle_previews", "VP_TRAILERLOGS" },
	{ "TRAILERS", "ENT_vehicle_previews", "VP_TRAILERS" },
	{ "TRAILERS2", "ENT_vehicle_previews", "TRAILERS2" },
	{ "TRAILERS3", "ENT_vehicle_previews", "VP_TRAILERS3" },
	{ "TRAILERSMALL", "ENT_vehicle_previews", "VP_TRAILERSMALL" },
	{ "TRASH", "ENT_vehicle_previews", "VP_TRASH" },
	{ "TRASH2", "ENT_vehicle_previews", "VP_TRASH2" },
	{ "TRFLAT", "ENT_vehicle_previews", "VP_TRFLAT" },
	{ "TVTRAILER", "ENT_vehicle_previews", "VP_TVTRAILER" },
	{ "UTILLITRUCK", "ENT_vehicle_previews", "VP_UTILLITRUCK" },
	{ "UTILLITRUCK2", "ENT_vehicle_previews", "VP_UTILLITRUCK2" },
	{ "UTILLITRUCK3", "ENT_vehicle_previews", "VP_UTILLITRUCK3" },
	{ "VOODOO2", "ENT_vehicle_previews", "VP_VOODOO" },
	{ "BALLER5", "ENT_vehicle_previews", "VP_BALLER5" },
	{ "BALLER6", "ENT_vehicle_previews", "VP_BALLER6" },
	{ "SCHAFTER2", "ENT_vehicle_previews", "VP_SCHAFTER2" },
	{ "SCHAFTER3", "ENT_vehicle_previews", "VP_SCHAFTER3" },
	{ "SCHAFTER4", "ENT_vehicle_previews", "VP_SCHAFTER4" },
	{ "SCHAFTER5", "ENT_vehicle_previews", "VP_SCHAFTER5" },
	{ "SCHAFTER6", "ENT_vehicle_previews", "VP_SCHAFTER6" },
	{ "MOONBEAM", "ENT_vehicle_previews", "VP_MOONBEAM" },
	{ "COGNOSCENTI2", "ENT_vehicle_previews", "VP_COGNOSCENTI2" },
	{ "COG552", "ENT_vehicle_previews", "VP_COG552" },
	{ "VIRGO3", "ENT_vehicle_previews", "VP_VIRGO3" },
	{ "FACTION", "ENT_vehicle_previews", "VP_FACTION" },
	{ "HAULER2", "ENT_vehicle_previews", "VP_HAULER2" },
	{ "INSURGENT3", "ENT_vehicle_previews", "VP_INSURGENT3" },
	{ "PHANTOM3", "ENT_vehicle_previews", "VP_PHANTOM3" },
	{ "TECHNICAL3", "ENT_vehicle_previews", "VP_TECHNICAL3" },
	{ "CLIFFHANGER", "ENT_vehicle_previews", "VP_CLIFFHANGER" },
	{ "ISSI4", "ENT_vehicle_previews", "VP_ISSI4" },
	{ "ISSI5", "ENT_vehicle_previews", "VP_ISSI5" },
	{ "ISSI6", "ENT_vehicle_previews", "VP_ISSI6" },
	{ "BRUISER", "ENT_vehicle_previews", "VP_BRUISER" },
	{ "BRUISER2", "ENT_vehicle_previews", "VP_BRUISER2" },
	{ "BRUISER3", "ENT_vehicle_previews", "VP_BRUISER3" },
	{ "DEATHBIKE", "ENT_vehicle_previews", "VP_DEATHBIKE" },
	{ "DEATHBIKE2", "ENT_vehicle_previews", "VP_DEATHBIIKE2" },
	{ "DEATHBIKE3", "ENT_vehicle_previews", "VP_DEATHBIKE3" },
	{ "DOMINATOR4", "ENT_vehicle_previews", "VP_DOMINATOR4" },
	{ "DOMINATOR5", "ENT_vehicle_previews", "VP_DOMINATOR5" },
	{ "DOMINATOR6", "ENT_vehicle_previews", "VP_DOMINATOR6" },
	{ "IMPALER2", "ENT_vehicle_previews", "VP_IMPALER1" },
	{ "IMPALER3", "ENT_vehicle_previews", "VP_IMPALER2" },
	{ "IMPALER4", "ENT_vehicle_previews", "VP_IMPALER3" },
	{ "MONSTER3", "ENT_vehicle_previews", "VP_MONSTER1" },
	{ "MONSTER4", "ENT_vehicle_previews", "VP_MONSTER2" },
	{ "SLAMVAN4", "ENT_vehicle_previews", "VP_SLAMVAN4" },
	{ "SLAMVAN5", "ENT_vehicle_previews", "VP_SLAMVAN5" },
	{ "SLAMVAN6", "ENT_vehicle_previews", "VP_SLAMVAN6" },
	//Update 29 new entries
	{ "BUCCANEER2", "lsc_default", "buccaneer2_a" },
	{ "CHINO2", "lsc_default", "chino2_a" },
	{ "FACTION2", "lsc_default", "faction2_a" },
	{ "MOONBEAM2", "lsc_default", "moonbeam2_a" },
	{ "PRIMO2", "lsc_default", "primo2_a" },
	{ "VOODOO", "lsc_default", "voodoo_0" }, //voodoo = shiny
	{ "BALLER3", "lgm_dlc_apartments", "baller3" },
	{ "BALLER4", "sssa_default", "baller2" },
	{ "MAMBA", "lgm_dlc_apartments", "mamba" },
	{ "NIGHTSHADE", "lgm_dlc_apartments", "niteshad" },
	{ "VERLIERER2", "lgm_dlc_apartments", "verlier" },
	{ "BTYPE2", "sssa_dlc_halloween", "btype2" },
	{ "LURCHER", "sssa_dlc_halloween", "lurcher" },
	{ "BTYPE3", "lgm_dlc_valentines2", "roosevelt2" },
	{ "BANSHEE2", "lsc_jan2016", "banshee2_a" },
	{ "SULTANRS", "lsc_jan2016", "sultan2_a" },
	{ "TAMPA", "sssa_dlc_christmas_3", "tampa" },
	{ "SUPERVOLITO", "elt_dlc_apartments", "svolito" },
	{ "SUPERVOLITO2", "elt_dlc_apartments", "svolito2" },
	{ "COG55", "lgm_dlc_apartments", "cog55" },
	{ "COGNOSCENTI", "lgm_dlc_apartments", "cognosc" },
	{ "LIMO2", "candc_apartments", "limo2" },
	// Lowrider 2 update
	{ "FACTION3", "lsc_lowrider2", "faction3_a" },
	{ "MINIVAN2", "lsc_lowrider2", "minivan2_a" },
	{ "SABREGT2", "lsc_lowrider2", "sabregt2_a" },
	{ "SLAMVAN3", "lsc_lowrider2", "slamvan3_a" },
	{ "TORNADO5", "lsc_lowrider2", "tornado5_a" },
	{ "VIRGO2", "lsc_lowrider2", "virgo2_a" },
	// Further Adventures in Finance and Felony
	{ "XLS", "lgm_dlc_executive1", "xls" },
	{ "XLS2", "lgm_dlc_executive1", "xls" },
	{ "RUMPO3", "sssa_dlc_executive_1", "rumpo3" },
	{ "NIMBUS", "elt_dlc_executive1", "nimbus" },
	{ "VOLATUS", "elt_dlc_executive1", "volatus" },
	{ "SEVEN70", "lgm_dlc_executive1", "seven70" },
	{ "WINDSOR2", "lgm_dlc_executive1", "windsor2" },
	{ "BESTIAGTS", "lgm_dlc_executive1", "bestiagts" },
	{ "PROTOTIPO", "lgm_dlc_executive1", "prototipo" },
	{ "BRICKADE", "candc_executive1", "brickade" },
	{ "REAPER", "lgm_dlc_executive1", "reaper" },
	{ "PFISTER811", "lgm_dlc_executive1", "pfister811" },
	{ "TUG", "dock_dlc_executive1", "tug" },
	{ "FMJ", "lgm_dlc_executive1", "fmj" },
	//Cunning Stunts
	{ "BF400", "sssa_dlc_stunt", "bf400" },
	{ "BRIOSO",	"sssa_dlc_stunt", "brioso" },
	{ "CLIFFHANGER", "sssa_dlc_stunt",	"cliffhanger" },
	{ "CONTENDER", "sssa_dlc_stunt", "contender" },
	{ "GARGOYLE", "sssa_dlc_stunt", "gargoyle" },
	{ "LE7B", "lgm_dlc_stunt", "le7b" },
	{ "LYNX", "lgm_dlc_stunt", "lynx" },
	{ "OMNIS", "sssa_dlc_stunt", "omnis" },
	{ "RALLYTRUCK", "sssa_dlc_stunt", "rallytruck" },
	{ "SHEAVA", "lgm_dlc_stunt", "sheava" },
	{ "TAMPA2", "sssa_dlc_stunt", "tampa2" },
	{ "TROPHYTRUCK", "sssa_dlc_stunt", "trophy" },
	{ "TROPHYTRUCK2", "sssa_dlc_stunt", "trophy2" },
	{ "TROPOS", "sssa_dlc_stunt", "tropos" },
	{ "TYRUS", "lgm_dlc_stunt", "tyrus" },
	//Bikers 
	{ "AVARUS", "sssa_dlc_biker", "avarus" },
	{ "BAGGER", "sssa_dlc_biker", "bagger" },
	{ "BLAZER4", "sssa_dlc_biker", "blazer4" },
	{ "CHIMERA", "sssa_dlc_biker", "chimera" },
	{ "DAEMON2", "sssa_dlc_biker", "daemon2"},
	{ "DEFILER", "sssa_dlc_biker", "defiler" },
	{ "ESSKEY", "sssa_dlc_biker", "esskey" },
	{ "FAGGIO3", "sssa_dlc_biker", "faggio3" },
	{ "FAGGIO", "sssa_default", "faggio" },
	{ "MANCHEZ", "sssa_dlc_biker", "manchez" },
	{ "NIGHTBLADE", "sssa_dlc_biker", "nightblade" },
	{ "RATBIKE", "sssa_dlc_biker", "ratbike" },
	{ "SANCTUS", "sssa_dlc_biker", "sanctus" },
	{ "TORNADO6", "sssa_dlc_biker", "tornado6" },
	{ "VORTEX", "sssa_dlc_biker", "vortex" },
	{ "WOLFSBANE", "sssa_dlc_biker", "wolfsbane" },
	{ "YOUGA2", "sssa_dlc_biker", "youga2" },
	{ "ZOMBIEA", "sssa_dlc_biker", "zombiea" },
	{ "ZOMBIEB", "sssa_dlc_biker", "zombieb" },
	{ "HAKUCHOU2", "lgm_dlc_biker", "hakuchou2" },
	{ "RAPTOR", "lgm_dlc_biker", "raptor" },
	{ "SHOTARO", "lgm_dlc_biker", "shotaro" },
	// Import-Export 
	{ "VOLTIC2", "candc_importexport", "voltic2" },
	{ "RUINER2", "candc_importexport", "ruiner2" },
	{ "DUNE4", "candc_importexport", "dune5" },
	{ "DUNE5", "candc_importexport", "dune5" },
	{ "PHANTOM2", "candc_importexport", "phantom2" },
	{ "TECHNICAL2", "candc_importexport", "technical2" },
	{ "BOXVILLE5", "candc_importexport", "boxville5" },
	{ "WASTELANDER", "candc_importexport", "wastlndr" }, //could be: "wastelander", but wastlndr image name
	{ "BLAZER5", "candc_importexport", "blazer5" },
	{ "COMET3", "lsc_dlc_import_export", "comet3_a" },
	{ "DIABLOUS", "lsc_dlc_import_export", "diablous2_a" },
	{ "DIABLOUS2", "lsc_dlc_import_export", "diablous2_a" },
	{ "ELEGY", "lsc_dlc_import_export", "elegy_a" },
	{ "FCR", "lsc_dlc_import_export", "fcr2_a" },
	{ "FCR2", "lsc_dlc_import_export", "fcr2_b" },
	{ "ITALIGTB", "lsc_dlc_import_export", "italigtb2_a" },
	{ "ITALIGTB2", "lsc_dlc_import_export", "italigtb2_b" },
	{ "NERO", "lsc_dlc_import_export", "nero2_a" },
	{ "NERO2", "lsc_dlc_import_export", "nero2_b" },
	{ "PENETRATOR", "lgm_dlc_importexport", "penetrator" },
	{ "SPECTER", "lsc_dlc_import_export", "specter2_a" },
	{ "SPECTER2", "lsc_dlc_import_export", "specter2_b" },
	{ "TEMPESTA", "lgm_dlc_importexport", "tempesta" },
	//Special Races
	{ "GP1", "lgm_dlc_specialraces", "gp1" },
	{ "INFERNUS2", "lgm_dlc_specialraces", "infernus2" },
	{ "RUSTON", "lgm_dlc_specialraces", "ruston" },
	{ "TURISMO2", "lgm_dlc_specialraces", "turismo2" },
	//Gun Running
	{ "APC", "candc_gunrunning", "apc" },
	{ "ARDENT", "candc_gunrunning", "ardent" },
	{ "CADDY3", "foreclosures_bunker", "transportation_1" },
	{ "CHEETAH2", "lgm_dlc_gunrunning", "cheetah2" },
	{ "DUNE3", "candc_gunrunning", "dune3" },
	{ "HALFTRACK", "candc_gunrunning", "halftrack" },
	//{ "HAULER2", "candc_truck", "cab_1" }, //no image yet
	//{ "INSURGENT3", "lgm_dlc_specialraces", "gp1" }, //no image yet
	{ "TRAILERLARGE", "candc_truck", "thumbnail" }, //no image yet - could also be TRAILERS4
	{ "NIGHTSHARK", "candc_gunrunning", "nightshark" },
	{ "OPPRESSOR", "candc_gunrunning", "oppressor" },
	//{ "PHANTOM3", "lgm_dlc_specialraces", "gp1" }, //no image yet
	{ "TAMPA3", "candc_gunrunning", "tampa3" },
	//{ "TECHNICAL3", "lgm_dlc_specialraces", "gp1" }, //no image yet
	{ "TORERO", "lgm_dlc_gunrunning", "torero" },
	{ "TRAILERSMALL2", "candc_gunrunning", "trsmall2" },
	{ "VAGNER", "lgm_dlc_gunrunning", "vagner" },
	{ "XA21", "lgm_dlc_gunrunning", "xa21" },
	//Smugglers Run DLC
	{ "BOMBUSHKA", "candc_smuggler", "bombushka" },
	{ "HUNTER", "candc_smuggler", "hunter" },
	{ "LAZER", "candc_smuggler",  "lazer" },
	{ "MOGUL", "candc_smuggler", "mogul" },
	{ "MOLOTOK", "candc_smuggler", "molotok" },
	{ "NOKOTA", "candc_smuggler", "nokota" },
	{ "PYRO", "candc_smuggler", "pyro" },
	{ "ROGUE", "candc_smuggler", "rogue" },
	{ "STARLING", "candc_smuggler", "starling" },
	{ "TULA", "candc_smuggler", "tula" },
	{ "VIGILANTE", "candc_smuggler", "vigilante" },
	{ "CYCLONE", "lgm_dlc_smuggler", "cyclone" },
	{ "RAPIDGT3", "lgm_dlc_smuggler", "rapidgt3" },
	{ "VISIONE", "lgm_dlc_smuggler", "visione" },
	{ "RETINUE", "sssa_dlc_smuggler", "retinue" },
	{ "ALPHAZ1", "elt_dlc_smuggler", "alphaz1" },
	{ "HAVOK", "elt_dlc_smuggler", "havok" },
	{ "HOWARD", "elt_dlc_smuggler", "howard" },
	{ "MICROLIGHT", "elt_dlc_smuggler", "microlight" },
	{ "SEABREEZE", "elt_dlc_smuggler", "seabreeze" },
	//Doomsday Heist
	{ "AKULA", "candc_xmas2017", "akula" },
	{ "BARRAGE", "candc_xmas2017", "barrage" },
	{ "CHERNOBOG", "candc_xmas2017", "chernobog" },
	{ "DELUXO", "candc_xmas2017", "deluxo" },
	{ "KHANJALI", "candc_xmas2017", "khanjali" },
	{ "RIOT2", "candc_xmas2017", "riot2" },
	{ "STROMBERG", "candc_xmas2017", "stromberg" },
	{ "THRUSTER", "candc_xmas2017", "thruster" },
	{ "VOLATOL", "candc_xmas2017", "volatol" },
	{ "HERMES", "sssa_dlc_xmas2017", "hermes" },
	{ "KAMACHO", "sssa_dlc_xmas2017", "kamacho" },
	{ "RIATA", "sssa_dlc_xmas2017", "riata" },
	{ "SENTINEL3", "sssa_dlc_xmas2017", "sentinel3" },
	{ "STREITER", "sssa_dlc_xmas2017", "streiter" }, //Also in lgm_dlc_xmas2017
	{ "YOSEMITE", "sssa_dlc_xmas2017", "yosemite" },
	{ "AUTARCH", "lgm_dlc_xmas2017", "autarch" },
	{ "AVENGER", "candc_chopper", "banner_0" },
	{ "COMET4", "lgm_dlc_xmas2017", "comet4" },
	{ "COMET5", "lgm_dlc_xmas2017", "comet5" },
	{ "GT500", "lgm_dlc_xmas2017", "gt500" },
	{ "HUSTLER", "lgm_dlc_xmas2017", "hustler" },
	{ "NEON", "lgm_dlc_xmas2017", "neon" },
	{ "PARIAH", "lgm_dlc_xmas2017", "pariah" },
	{ "RAIDEN", "lgm_dlc_xmas2017", "raiden" },
	{ "REVOLTER", "lgm_dlc_xmas2017", "revolter" },
	{ "SAVESTRA", "lgm_dlc_xmas2017", "savestra" },
	{ "SC1", "lgm_dlc_xmas2017", "sc1" },
	{ "VISERIS", "lgm_dlc_xmas2017", "viseris" },
	{ "Z190", "lgm_dlc_xmas2017", "z190" },
	//San-Andreas Super Sport
	{ "CHEBUREK", "sssa_dlc_assault", "cheburek" },
	{ "DOMINATOR3", "sssa_dlc_assault", "dominator3" },
	{ "ELLIE", "sssa_dlc_assault", "ellie" },
	{ "FAGALOA", "sssa_dlc_assault", "fagaloa" },
	{ "HOTRING", "sssa_dlc_assault", "hotring" },
	{ "ISSI3", "sssa_dlc_assault", "issi3" },
	{ "MICHELLI", "sssa_dlc_assault", "michelli" },
	{ "SEASPARROW", "elt_dlc_assault", "sparrow" },
	{ "CARACARA", "candc_assault", "caracara" },
	{ "ENTITY2", "lgm_dlc_assault", "entity2" },
	{ "FLASHGT", "lgm_dlc_assault", "flashgt" },
	{ "GB200", "lgm_dlc_assault", "gb200" },
	{ "JESTER3", "lgm_dlc_assault", "jester3" },
	{ "TAIPAN", "lgm_dlc_assault", "taipan" },
	{ "TEZERACT", "lgm_dlc_assault", "tezeract" },
	{ "TYRANT", "lgm_dlc_assault", "tyrant" },
	//After Hours
	{ "PBUS2", "sssa_dlc_battle", "pbus2" },
	{ "PATRIOT", "sssa_dlc_battle", "patriot" },
	{ "PATRIOT2", "sssa_dlc_battle", "patriot2" },
	{ "TERBYTE", "candc_hacker", "banner0" },
	{ "POUNDER2", "candc_battle", "pounder2" },
	{ "MULE4", "candc_battle", "mule4" },
	{ "MENACER", "candc_battle", "menacer" },
	{ "OPPRESSOR2", "candc_battle", "oppressor2" },
	{ "SCRAMJET", "candc_battle", "scramjet" },
	{ "STRIKEFORCE", "candc_battle", "strikeforce" },
	{ "SWINGER", "lgm_dlc_battle", "swinger" },
	{ "STAFFORD", "lgm_dlc_battle", "stafford" },
	{ "FREECRAWLER", "lgm_dlc_battle", "freecrawler" },
	{ "BLIMP3", "elt_dlc_battle", "blimp3" },
	{ "JESTER3", "sssa_dlc_christmas_2", "jester2" },
	//Arena War
	{ "BLISTA3", "sssa_dlc_arena", "blista3" },
	{ "BRUTUS", "mba_vehicles", "brutus1" },
	{ "BRUTUS2", "mba_vehicles", "brutus2" },
	{ "BRUTUS3", "mba_vehicles", "brutus3" },
	{ "CERBERUS", "mba_vehicles", "cerberus1" },
	{ "CERBERUS2", "mba_vehicles", "cerberus2" },
	{ "CERBERUS3", "mba_vehicles", "cerberus3" },
	{ "CLIQUE", "lgm_dlc_arena", "clique" },
	{ "DEVESTE", "lgm_dlc_arena", "deveste" },
	{ "DEVIANT", "lgm_dlc_arena", "deviant" },
	{ "IMPALER", "mba_vehicles", "impaler" },
	{ "IMPERATOR", "mba_vehicles", "imperator1" },
	{ "IMPERATOR2", "mba_vehicles", "imperator2" },
	{ "IMPERATOR3", "mba_vehicles", "imperator3" },
	{ "ITALIGTO", "lgm_dlc_arena", "italigto" },
	{ "RCBANDITO", "sssa_dlc_arena", "rcbandito" },
	{ "SCARAB", "mba_vehicles", "scarab1" },
	{ "SCARAB2", "mba_vehicles", "scarab2" },
	{ "SCARAB3", "mba_vehicles", "scarab3" },
	{ "SCHLAGEN", "lgm_dlc_arena", "schlagen" },
	{ "TOROS", "lgm_dlc_arena", "toros" },
	{ "TULIP", "sssa_dlc_arena", "tulip" },
	{ "VAMOS", "sssa_dlc_arena", "vamos" },
	{ "ZR380", "mba_vehicles", "zr3801" },
	{ "ZR3802", "mba_vehicles", "zr3802" },
	{ "ZR3803", "mba_vehicles", "zr3803" },
	//Diamond Casino
	{ "CARACARA2", "sssa_dlc_vinewood", "caracara2" },
	{ "DRAFTER", "lgm_dlc_vinewood", "drafter" },
	{ "DYNASTY", "sssa_dlc_vinewood", "dynasty" },
	{ "EMERUS", "lgm_dlc_vinewood", "emerus" },
	{ "GAUNTLET3", "sssa_dlc_vinewood", "gauntlet3" },
	{ "GAUNTLET4", "sssa_dlc_vinewood", "gauntlet4" },
	{ "HELLION", "sssa_dlc_vinewood", "hellion" },
	{ "ISSI7", "sssa_dlc_vinewood", "issi7" },
	{ "JUGULAR", "lgm_dlc_vinewood", "jugular" },
	{ "KRIEGER", "lgm_dlc_vinewood", "krieger" },
	{ "LOCUST", "lgm_dlc_vinewood", "locust" },
	{ "NEBULA", "sssa_dlc_vinewood", "nebula" },
	{ "NEO", "lgm_dlc_vinewood", "neo" },
	{ "NOVAK", "lgm_dlc_vinewood", "novak" },
	{ "PARAGON", "lgm_dlc_vinewood", "paragon" },
	{ "PARAGON2", "lgm_dlc_vinewood", "paragon" },
	{ "PEYOTE2", "sssa_dlc_vinewood", "peyote2" },
	{ "RROCKET", "lgm_dlc_vinewood", "rrocket" },
	{ "S80", "lgm_dlc_vinewood", "s80" },
	{ "THRAX", "lgm_dlc_vinewood", "thrax" },
	{ "ZION3", "sssa_dlc_vinewood", "zion3" },
	{ "ZORRUSSO", "lgm_dlc_vinewood", "zorrusso" },
	//Diamond Casino Heist
	{ "FORMULA", "lgm_dlc_casinoheist", "formula" },
	{ "FORMULA2", "lgm_dlc_casinoheist", "formula2" },
	{ "FURIA", "lgm_dlc_casinoheist", "furia" },
	{ "IMORGON", "lgm_dlc_casinoheist", "imorgon" },
	{ "KOMODA", "lgm_dlc_casinoheist", "komoda" },
	{ "REBLA", "lgm_dlc_casinoheist", "rebla" },
	{ "STRYDER", "lgm_dlc_casinoheist", "stryder" },
	{ "VSTR", "lgm_dlc_casinoheist", "vstr" },
	{ "ASBO", "sssa_dlc_casinoheist", "asbo" },
	{ "EVERON", "sssa_dlc_casinoheist", "everon" },
	{ "KANJO", "sssa_dlc_casinoheist", "kanjo" },
	{ "OUTLAW", "sssa_dlc_casinoheist", "outlaw" },
	{ "RETINUE2", "sssa_dlc_casinoheist", "retinue2" },
	{ "SUGOI", "sssa_dlc_casinoheist", "sugoi" },
	{ "SULTAN2", "sssa_dlc_casinoheist", "sultan2" },
	{ "VAGRANT", "sssa_dlc_casinoheist", "vagrant" },
	{ "YOSEMITE2", "sssa_dlc_casinoheist", "yosemite2" },
	{ "BLAZER2", "candc_casinoheist", "blazer2" },
	{ "BOXVILLE", "candc_casinoheist", "boxville" },
	{ "BURRITO2", "candc_casinoheist", "burrito2" },
	{ "FIRETRUK", "candc_casinoheist", "firetruk" },
	{ "JB7002", "candc_casinoheist", "jb7002" },
	{ "LGUARD", "candc_casinoheist", "lguard" },
	{ "MINITANK", "candc_casinoheist", "minitank" },
	{ "ZHABA", "candc_casinoheist", "zhaba" },
	{ "STOCKADE", "candc_casinoheist", "stockade" }
};

/***
* METHODS
*/

std::string get_vehicle_make_and_model(int modelHash);

void process_veh_menu();

void process_speed_menu();

void process_visualize_menu();

bool process_savedveh_menu();

bool process_savedveh_sort_menu();

bool process_savedveh_slot_menu(int slot);

bool process_vehmod_menu();

bool process_carspawn_menu();

void update_vehicle_features(BOOL playerExists, Ped playerPed);

void update_veh_weapons_features();

void reset_vehicle_globals();

void reset_veh_weapons_globals();

bool process_spawn_menu_cars();

bool onconfirm_spawn_menu_cars(MenuItem<int> choice);

bool process_spawn_menu_indus();

void vehicle_flip();

void speedlimiter_switching();

void invincibility_switching();

void keyboard_tip_message(char* curr_message);

void doorslocked_switching();

void seat_change_hotkey();

void engineonoff_switching();

void vehicle_brake();

void vehicle_burnout();

void vehicle_set_alarm();

void engine_damage();

void engine_kill();

void process_window_roll();

void interior_light();

void police_light();

void search_light();

void start_bombing_run();

void damage_door();

void enter_damaged_vehicle();

Vehicle find_nearest_vehicle();

Ped find_nearest_ped();

void toggle_tractioncontrol();

bool process_spawn_menu_generic(int topMenuSelection);

bool spawn_saved_car(int slot, std::string caption);

void save_current_vehicle(int slot);

bool onconfirm_spawn_menu_vehicle_selection(MenuItem<std::string> choice);

bool do_spawn_vehicle(std::string modelName, std::string modelTitle);

bool do_spawn_vehicle_hash(int modelName, std::string modelTitle);

Vehicle do_spawn_vehicle(DWORD modelHash, std::string modelTitle, bool cleanup);

bool process_vehmod_wheel_selection_menu();

bool process_vehmod_wheel_selection();

//Paint menus

bool process_paint_menu();

bool process_paint_menu_type();

bool process_paint_menu_liveries();

bool onconfirm_color_menu_selection(MenuItem<int> choice);

void onhighlight_color_menu_selection(MenuItem<int> choice);

void apply_paint(PaintColor whichpaint);

//Vehicle mod getters and setters

bool is_custom_tyres(std::vector<int> extras);

void set_custom_tyres(bool applied, std::vector<int> extras);

bool is_turbocharged(std::vector<int> extras);

void set_turbocharged(bool applied, std::vector<int> extras);

bool is_bulletproof_tyres(std::vector<int> extras);

void set_bulletproof_tyres(bool applied, std::vector<int> extras);

bool is_xenon_headlights(std::vector<int> extras);

void set_xenon_headlights(bool applied, std::vector<int> extras);

bool is_extra_enabled(std::vector<int> extras);

void set_extra_enabled(bool applied, std::vector<int> extras);

void set_plate_text(MenuItem<int> choice);

bool is_convertible_roofdown(std::vector<int> extras);

void set_convertible_roofdown(bool applied, std::vector<int> extras);

int find_menu_index_to_restore(int category, int actualCategory, Vehicle veh);

bool vehicle_menu_interrupt();

bool vehicle_save_menu_interrupt();

bool vehicle_save_sort_menu_interrupt();

bool vehicle_save_slot_menu_interrupt();

void add_vehicle_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_vehicle_weapons_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_teleporter_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_vehicle_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_vehicle(std::vector<StringPairSettingDBRow>* settings);

bool onconfirm_paintfade(MenuItem<float> choice);

void onhighlight_paintfade(MenuItem<float> choice);

int get_current_veh_invincibility_mode();

void onchange_veh_invincibility_mode(int value, SelectFromListMenuItem* source);

void onchange_veh_never_dirty(int value, SelectFromListMenuItem* source);

void onchange_veh_speed_boost_index(int value, SelectFromListMenuItem *source);

int get_current_veh_eng_pow_index();

void onchange_veh_eng_pow_index(int value, SelectFromListMenuItem* source);

void onchange_veh_mass_index(int value, SelectFromListMenuItem* source);

void onchange_player_forceshield_mode(int value, SelectFromListMenuItem* source);

void onchange_veh_infiniteboost_index(int value, SelectFromListMenuItem* source);

void onchange_veh_nitrous_index(int value, SelectFromListMenuItem* source);

void onchange_speed_size_index(int value, SelectFromListMenuItem* source);

void onchange_speed_position_index(int value, SelectFromListMenuItem* source);

void onchange_door_autolock_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_blips_index(int value, SelectFromListMenuItem* source);

void onchange_idle_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_background_opacity_index(int value, SelectFromListMenuItem* source);

void onchange_veh_remember_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipsize_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipcolour_index(int value, SelectFromListMenuItem* source);

void onchange_world_npc_vehicles_colour_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipsymbol_index(int value, SelectFromListMenuItem* source);

void onchange_veh_blipflash_index(int value, SelectFromListMenuItem* source);

void onchange_speeding_city_index(int value, SelectFromListMenuItem* source);

void onchange_detection_range_index(int value, SelectFromListMenuItem* source);

void onchange_pirsuit_range_index(int value, SelectFromListMenuItem* source);

void onchange_stars_punish_index(int value, SelectFromListMenuItem* source);

void onchange_speeding_speedway_index(int value, SelectFromListMenuItem* source);

void onchange_fine_size_index(int value, SelectFromListMenuItem* source);

void onchange_car_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_bike_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_boat_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_plane_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_heli_consumption_index(int value, SelectFromListMenuItem* source);

void onchange_refuelspeed_index(int value, SelectFromListMenuItem* source);

void onchange_fuelprice_index(int value, SelectFromListMenuItem* source);

void onchange_canprice_index(int value, SelectFromListMenuItem* source);

void onchange_random1_index(int value, SelectFromListMenuItem* source);

void onchange_random2_index(int value, SelectFromListMenuItem* source);

void onchange_barposition_index(int value, SelectFromListMenuItem* source);

void onchange_car_enginehealth_index(int value, SelectFromListMenuItem* source);

void onchange_bike_enginehealth_index(int value, SelectFromListMenuItem* source);

void onchange_boat_enginehealth_index(int value, SelectFromListMenuItem* source);

void onchange_plane_enginehealth_index(int value, SelectFromListMenuItem* source);

void onchange_heli_enginehealth_index(int value, SelectFromListMenuItem* source);

void onchange_car_enginedegrade_index(int value, SelectFromListMenuItem* source);

void onchange_bike_enginedegrade_index(int value, SelectFromListMenuItem* source);

void onchange_boat_enginedegrade_index(int value, SelectFromListMenuItem* source);

void onchange_plane_enginedegrade_index(int value, SelectFromListMenuItem* source);

void onchange_heli_enginedegrade_index(int value, SelectFromListMenuItem* source);

void onchange_restoration_speed_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_r_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_g_index(int value, SelectFromListMenuItem* source);

void onchange_fuel_colours_b_index(int value, SelectFromListMenuItem* source);

void onchange_veh_turn_signals_index(int value, SelectFromListMenuItem* source);

void onchange_veh_turn_signals_angle_index(int value, SelectFromListMenuItem* source);

void onchange_veh_nitrous_power_index(int value, SelectFromListMenuItem* source);

void onchange_veh_turn_signals_acceleration_index(int value, SelectFromListMenuItem* source);

void onchange_veh_speedlimiter_index(int value, SelectFromListMenuItem* source);

void onchange_veh_cityspeedlimiter_index(int value, SelectFromListMenuItem* source);

void onchange_veh_countryspeedlimiter_index(int value, SelectFromListMenuItem* source);

void onchange_veh_enginerunning_index(int value, SelectFromListMenuItem* source);

void onchange_veh_autoshutengine_index(int value, SelectFromListMenuItem* source);

void onchange_veh_lightsOff_index(int value, SelectFromListMenuItem* source);

void onchange_veh_jumpy_index(int value, SelectFromListMenuItem* source);

void onchange_heavy_veh_index(int value, SelectFromListMenuItem* source);

void onchange_veh_hydraulics_index(int value, SelectFromListMenuItem* source);

void onchange_veh_vislight_index(int value, SelectFromListMenuItem* source);

void onchange_veh_vislight3d_index(int value, SelectFromListMenuItem* source);

void set_old_vehicle_state(bool updatedState);

MenuItemImage* vehicle_image_preview_finder(MenuItem<std::string> choice);

void init_vehicle_feature();

void fix_vehicle();

void clean_vehicle();

void apply_neon_colors(int colorIndex);

void onhighlight_neon_lights_selection(MenuItem<int> colorIndex);

bool onconfirm_neon_lights_selection(MenuItem<int> choice);

bool is_neonLights(std::vector<int> extras);

void set_neonLights(bool applied, std::vector<int> extras);

bool process_neon_lights_menu();

//Smoke related code

void apply_smoke_colors(int colorIndex);

void onhighlight_smoke_selection(MenuItem<int> choice);

bool onconfirm_smoke_selection(MenuItem<int> choice);

void set_smoke(bool applied, std::vector<int> extras);

bool process_smoke_colour_menu();

//End of smoke related code

// Vehicle Interior Colours

void apply_trim_colors(int colorIndex);

void onhighlight_trim_colour_selection(MenuItem<int> choice);

bool onconfirm_trim_colour_selection(MenuItem<int> choice);

bool process_trim_colour_menu();

void apply_dash_colors(int colorIndex);

void onhighlight_dash_colour_selection(MenuItem<int> choice);

bool onconfirm_dash_colour_selection(MenuItem<int> choice);

bool process_dash_colour_menu();

bool process_interior_colour_menu();

//Xenon colours

void apply_xenon_colors(int colorIndex);

void onhighlight_xenon_selection(MenuItem<int> choice);

bool onconfirm_xenon_selection(MenuItem<int> choice);

bool process_xenon_colour_menu();

bool inline is_this_a_car(Vehicle veh){
	// Return true if the current vehicle is a car, e.g. as certain vehicles don't support neon lights
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_CAR(et) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(et);
}

bool inline is_this_a_motorcycle(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return (BOOL) VEHICLE::IS_THIS_MODEL_A_BIKE(et);
}

bool inline is_this_a_heli_or_plane(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_HELI(et) || VEHICLE::IS_THIS_MODEL_A_PLANE(et);
}

bool inline is_this_a_bicycle(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_BICYCLE(et);
}

bool inline is_this_a_boat_or_sub(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_BOAT(et) || VEHICLE::_IS_THIS_MODEL_A_SUBMERSIBLE(et);
}

bool inline is_this_a_train(Vehicle veh){
	Entity et = ENTITY::GET_ENTITY_MODEL(veh);
	return VEHICLE::IS_THIS_MODEL_A_TRAIN(et);
}

bool did_player_just_enter_vehicle(Ped playerPed);

void fully_tune_vehicle(Vehicle veh, bool optics = true);

void reset_vehicle(Vehicle veh);

void randomize_vehicle_upgrades(Vehicle veh);

void vehicle_anchor();

bool onconfirm_seat_menu(MenuItem<int> choice);

bool process_veh_seat_menu();

extern char* curr_message;
extern bool keyboard_on_screen_already;

void GenerateVehicleModelList();
void PopulateVehicleModelsArray();