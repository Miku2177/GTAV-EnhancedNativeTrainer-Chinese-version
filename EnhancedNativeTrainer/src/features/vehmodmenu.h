#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"
#include "..\ui_support\menu_functions.h"

#include <string>

#include "vehicles.h"

#include "..\debug\debuglog.h"

extern int current_picked_engine_sound;

extern bool featureEngineSound;

const static std::string ENGINE_SOUND[] = { "\"ADDER\"", "\"AIRTUG\"", "\"AKUMA\"", "\"ALPHA\"", "\"AMBULANCE\"", "\"APC\"", "\"ARDENT\"", "\"ASEA\"", "\"ASTEROPE\"", "\"AUTARCH\"", "\"AVARUS\"", "\"BAGGER\"", "\"BALLER3\"", "\"BALLER\"", 
"\"BANSHEE2\"", "\"BANSHEE\"", "\"BARRAGE\"", "\"BATI\"", "\"BESTIAGTS\"", "\"BF400\"", "\"BFINJECTION\"", "\"BIFTA\"", "\"BISON3\"", "\"BJXL\"", "\"BLADE\"", "\"BLAZER3\"", "\"BLAZER5\"", "\"BLAZER\"", "\"BLISTA2\"", "\"BLISTA\"", 
"\"BOBCATXL\"", "\"BODHI2\"", "\"BOXVILLE2\"", "\"BOXVILLE5\"", "\"BRAWLER\"", "\"BRICKADE\"", "\"BRIOSO\"", "\"BTYPE2\"", "\"BTYPE\"", "\"BUCCANEER\"", "\"BUFFALO2\"", "\"BUFFALO\"", "\"BULLDOZER\"", "\"BULLET\"", "\"BURRITO3\"", "\"BUS\"", 
"\"CADDY\"", "\"CAMPER\"", "\"CARACARA\"", "\"CARBONIZZARE\"", "\"CARBONRS\"", "\"CASCO\"", "\"CAVALCADE\"", "\"CHEBUREK\"", "\"CHEETAH2\"", "\"CHEETAH\"", "\"CHERNOBOG\"", "\"CHIMERA\"", "\"CHINO\"", "\"CLIFFHANGER\"", "\"COG55\"", 
"\"COGCABRIO\"", "\"COGNOSCENTI\"", "\"COMET2\"", "\"CONTENDER\"", "\"COQUETTE2\"", "\"COQUETTE3\"", "\"CRUSADER\"", "\"CYCLONE\"", "\"DAEMON2\"", "\"DAEMON\"", "\"DEFILER\"", "\"DELUXO\"", "\"DIABLOUS2\"", "\"DIABLOUS\"", "\"DLOADER\"",
"\"DOMINATOR2\"", "\"DOMINATOR\"", "\"DOUBLE\"", "\"DUBSTA3\"", "\"DUKES2\"", "\"DUKES\"", "\"DUMP\"", "\"DUNE2\"", "\"DUNE3\"", "\"DUNE\"", "\"ELEGY2\"", "\"ELEGY\"", "\"ELLIE\"", "\"EMPEROR2\"", "\"EMPEROR\"", "\"ENDURO\"", "\"ENTITY2\"", 
"\"ENTITYXF\"", "\"ESSKEY\"", "\"EXEMPLAR\"", "\"F620\"", "\"FACTION\"", "\"FAGALOA\"", "\"FAGGIO3\"", "\"FAGGIO\"", "\"FBI2\"", "\"FBI\"", "\"FCR\"", "\"FELON\"", "\"FELTZER2\"", "\"FELTZER3\"", "\"FIRETRUK\"", "\"FLASHGT\"", "\"FMJ\"", 
"\"FORKLIFT\"", "\"FQ2\"", "\"FUGITIVE\"", "\"FUROREGT\"", "\"FUSILADE\"", "\"FUTO\"", "\"GARGOYLE\"", "\"GAUNTLET2\"", "\"GAUNTLET\"", "\"GB200\"", "\"GLENDALE\"", "\"GP1\"", "\"GRANGER\"", "\"GRESLEY\"", "\"GT500\"", "\"GUARDIAN\"", 
"\"HABANERO\"", "\"HAKUCHOU2\"", "\"HAKUCHOU\"", "\"HALFTRACK\"", "\"HANDLER\"", "\"HAULER2\"", "\"HAULER\"", "\"HERMES\"", "\"HEXER\"", "\"HOTKNIFE\"", "\"HOTRING\"", "\"HUNTLEY\"", "\"HUSTLER\"", "\"INFERNUS2\"", "\"INFERNUS\"", "\"INGOT\"", 
"\"INNOVATION\"", "\"INSURGENT3\"", "\"INTRUDER\"", "\"ISSI2\"", "\"ISSI3\"", "\"ITALIGTB\"", "\"JACKAL\"", "\"JB700\"", "\"JESTER3\"", "\"JESTER\"", "\"JOURNEY\"", "\"KALAHARI\"", "\"KAMACHO\"", "\"KHAMELION\"", "\"KHANJALI\"", "\"KURUMA\"", 
"\"LANDSTALKER\"", "\"LE7B\"", "\"LECTRO\"", "\"LGUARD\"", "\"LIMO2\"", "\"LURCHER\"", "\"LYNX\"", "\"MAMBA\"", "\"MANANA\"", "\"MANCHEZ\"", "\"MARSHALL\"", "\"MASSACRO2\"", "\"MESA3\"", "\"MICHELLI\"", "\"MINIVAN\"", "\"MONROE\"", "\"MONSTER\"", 
"\"MOONBEAM\"","\"MOWER\"", "\"NEMESIS\"", "\"NEON\"", "\"NERO\"", "\"NIGHTBLADE\"", "\"NIGHTSHADE\"", "\"NIGHTSHARK\"", "\"NINEF2\"", "\"NINEF\"", "\"OMNIS\"", "\"OPPRESSOR\"", "\"ORACLE2\"", "\"ORACLE\"", "\"OSIRIS\"", "\"PACKER\"", "\"PANTO\"", 
"\"PARADISE\"", "\"PARIAH\"", "\"PATRIOT\"", "\"PCJ\"", "\"PENETRATOR\"", "\"PENUMBRA\"", "\"PEYOTE\"", "\"PFISTER811\"", "\"PHANTOM2\"", "\"PHANTOM3\"", "\"PHANTOM\"", "\"PHOENIX\"", "\"PICADOR\"", "\"PIGALLE\"", "\"POLICE2\"", "\"POLICEB\"", 
"\"POLICEOLD1\"", "\"POLICEOLD2\"", "\"POLICET\"", "\"POLICE\"", "\"PONY2\"", "\"POUNDER\"", "\"PRAIRIE\"", "\"PRANGER\"", "\"PREMIER\"", "\"PRIMO\"", "\"PROTOTIPO\"", "\"RADI\"", "\"RAIDEN\"", "\"RALLYTRUCK\"", "\"RANCHERXL\"", "\"RAPIDGT3\"", 
"\"RAPIDGT\"", "\"RAPTOR\"", "\"RATBIKE\"", "\"RATLOADER2\"", "\"RATLOADER\"", "\"REAPER\"", "\"REBEL2\"", "\"REBEL\"", "\"REGINA\"", "\"RETINUE\"", "\"REVOLTER\"", "\"RHAPSODY\"", "\"RHINO\"", "\"RIATA\"", "\"ROCOTO\"", "\"ROMERO\"", 
"\"RUFFIAN\"", "\"RUINER2\"", "\"RUINER\"", "\"RUMPO2\"", "\"RUMPO3\"", "\"RUSTON\"", "\"SABREGT\"", "\"SADLER\"", "\"SANCHEZ\"", "\"SANCTUS\"", "\"SANDKING\"", "\"SAVESTRA\"", "\"SC1\"", "\"SCHAFTER2\"", "\"SCHAFTER3\"", "\"SCHWARZER\"", 
"\"SEMINOLE\"", "\"SENTINEL3\"", "\"SENTINEL\"", "\"SERRANO\"", "\"SEVEN70\"", "\"SHEAVA\"", "\"SHERIFF2\"", "\"SHERIFF\"", "\"SHOTARO\"", "\"SLAMVAN3\"", "\"SLAMVAN\"", "\"SOVEREIGN\"", "\"SPECTER2\"", "\"SPEEDO\"", "\"STALION\"", "\"STANIER\"", 
"\"STINGER\"", "\"STOCKADE\"", "\"STRATUM\"", "\"STREITER\"", "\"STRETCH\"", "\"STROMBERG\"", "\"SULTANRS\"", "\"SULTAN\"", "\"SUPERD\"", "\"SURANO\"", "\"SURFER\"", "\"SURGE\"", "\"T20\"", "\"TACO\"", "\"TAILGATER\"", "\"TAIPAN\"", "\"TAMPA2\"", 
"\"TAMPA\"", "\"TAXI\"", "\"TECHNICAL2\"", "\"TECHNICAL\"", "\"TEMPESTA\"", "\"TEZERACT\"", "\"THRUSTER\"", "\"THRUST\"", "\"TIPTRUCK\"", "\"TORERO\"", "\"TORNADO4\"", "\"TORNADO6\"", "\"TORNADO\"", "\"TOURBUS\"", "\"TOWTRUCK2\"", "\"TOWTRUCK\"", 
"\"TRACTOR2\"", "\"TRACTOR\"", "\"TRASH\"", "\"TROPHYTRUCK2\"", "\"TROPHYTRUCK\"", "\"TROPOS\"", "\"TURISMO2\"", "\"TURISMOR\"", "\"TYRANT\"", "\"TYRUS\"", "\"UTILLITRUCK2\"", "\"UTILLITRUCK3\"", "\"VACCA\"", "\"VADER\"", "\"VAGNER\"", 
"\"VERLIERER2\"", "\"VIGERO\"", "\"VIGILANTE\"", "\"VINDICATOR\"", "\"VIRGO3\"", "\"VIRGO\"", "\"VISERIS\"", "\"VISIONE\"", "\"VOLTIC\"", "\"VOODOO2\"", "\"VORTEX\"", "\"WARRENER\"", "\"WASHINGTON\"", "\"WASTELANDER\"", "\"WINDSOR\"", "\"WOLFSBANE\"", 
"\"XA21\"", "\"XLS\"", "\"YOSEMITE\"", "\"YOUGA2\"", "\"YOUGA\"", "\"Z190\"", "\"ZENTORNO\"", "\"ZION\"", "\"ZOMBIEB\"", "\"ZTYPE\"" };

const int ENGINE_SOUND_NUMBERS[] = { 34, 263, 304, 40, 282, 294, 109, 214, 221, 17, 309, 336, 199, 198, 2, 1, 283, 319, 67, 315, 164, 160, 247, 200, 149, 180, 181, 179, 64, 63, 259, 166, 253, 254, 171, 269, 231, 92, 93, 127,
52, 51, 235, 37, 258, 264, 268, 255, 182, 68, 316, 105, 188, 113, 9, 8, 288, 317, 150, 337, 219, 119, 218, 84, 203, 71, 72, 287, 4, 339, 338, 331, 103, 328, 327, 165, 152, 151, 305, 159, 143, 144, 236, 163, 162, 161, 42, 41, 153, 206, 205,
306, 19, 18, 320, 118, 122, 158, 117, 321, 322, 285, 284, 323, 121, 43, 44, 293, 90, 38, 266, 197, 212, 76, 86, 73, 340, 133, 132, 91, 209, 30, 193, 191, 101, 237, 195, 333, 332, 286, 234, 240, 239, 128, 310, 154, 57, 196, 155, 22, 21, 225,
311, 173, 222, 232, 233, 31, 123, 100, 66, 65, 262, 167, 168, 69, 297, 74, 194, 0, 329, 289, 210, 129, 82, 96, 94, 313, 170, 58, 169, 106, 260, 111, 187, 137, 274, 330, 85, 35, 341, 145, 174, 80, 79, 81, 324, 124, 125, 23, 244, 228, 248,
83, 201, 334, 15, 78, 115, 29, 243, 242, 241, 146, 136, 107, 298, 303, 300, 301, 302, 299, 256, 245, 229, 290, 215, 207, 13, 204, 54, 270, 172, 60, 59, 50, 342, 134, 135, 24, 175, 176, 216, 116, 87, 230, 296, 185, 202, 211, 325, 148, 147,
249, 250, 70, 138, 279, 314, 312, 186, 95, 36, 45, 46, 47, 192, 88, 89, 189, 61, 7, 292, 291, 318, 157, 156, 343, 62, 261, 139, 224, 102, 238, 227, 48, 217, 110, 14, 75, 220, 49, 246, 213, 32, 257, 223, 3, 55, 56, 275, 178, 177, 25, 26, 295,
307, 276, 112, 98, 99, 97, 265, 281, 280, 272, 273, 267, 183, 184, 77, 11, 10, 20, 33, 278, 277, 27, 335, 6, 53, 140, 39, 308, 131, 130, 108, 12, 5, 141, 326, 226, 208, 271, 120, 344, 16, 190, 142, 252, 251, 104, 28, 126, 345, 114 };
//const static std::vector<int> ENGINE_SOUND_NUMBERS{ 34, 263, 304, 40, 282, 294, 109, 214, 221, 17, 309, 336, 199, 198, 2, 1, 283, 319, 67, 315, 164, 160, 247, 200, 149, 180, 181, 179, 64, 63, 259, 166, 253, 254, 171, 269, 231, 92, 93, 127,
//52, 51, 235, 37, 258, 264, 268, 255, 182, 68, 316, 105, 188, 113, 9, 8, 288, 317, 150, 337, 219, 119, 218, 84, 203, 71, 72, 287, 4, 339, 338, 331, 103, 328, 327, 165, 152, 151, 305, 159, 143, 144, 236, 163, 162, 161, 42, 41, 153, 206, 205,
//306, 19, 18, 320, 118, 122, 158, 117, 321, 322, 285, 284, 323, 121, 43, 44, 293, 90, 38, 266, 197, 212, 76, 86, 73, 340, 133, 132, 91, 209, 30, 193, 191, 101, 237, 195, 333, 332, 286, 234, 240, 239, 128, 310, 154, 57, 196, 155, 22, 21, 225,
//311, 173, 222, 232, 233, 31, 123, 100, 66, 65, 262, 167, 168, 69, 297, 74, 194, 0, 329, 289, 210, 129, 82, 96, 94, 313, 170, 58, 169, 106, 260, 111, 187, 137, 274, 330, 85, 35, 341, 145, 174, 80, 79, 81, 324, 124, 125, 23, 244, 228, 248, 
//83, 201, 334, 15, 78, 115, 29, 243, 242, 241, 146, 136, 107, 298, 303, 300, 301, 302, 299, 256, 245, 229, 290, 215, 207, 13, 204, 54, 270, 172, 60, 59, 50, 342, 134, 135, 24, 175, 176, 216, 116, 87, 230, 296, 185, 202, 211, 325, 148, 147,
//249, 250, 70, 138, 279, 314, 312, 186, 95, 36, 45, 46, 47, 192, 88, 89, 189, 61, 7, 292, 291, 318, 157, 156, 343, 62, 261, 139, 224, 102, 238, 227, 48, 217, 110, 14, 75, 220, 49, 246, 213, 32, 257, 223, 3, 55, 56, 275, 178, 177, 25, 26, 295,
//307, 276, 112, 98, 99, 97, 265, 281, 280, 272, 273, 267, 183, 184, 77, 11, 10, 20, 33, 278, 277, 27, 335, 6, 53, 140, 39, 308, 131, 130, 108, 12, 5, 141, 326, 226, 208, 271, 120, 344, 16, 190, 142, 252, 251, 104, 28, 126, 345, 114 };



/***
* METHODS
*/

void reset_vehmodmenu_globals();

void add_vehmodmenu_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void update_vehmodmenu_features(BOOL playerExists, Ped playerPed);