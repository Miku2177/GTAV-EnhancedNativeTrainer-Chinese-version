#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"
#include "..\ui_support\menu_functions.h"

#include <string>

#include "vehicles.h"

#include "..\debug\debuglog.h"

extern std::string current_picked_engine_sound;
extern bool featureEngineSound;

const static std::string ENGINE_SOUND[] = { "\"ADDER\"", "\"AIRTUG\"", "\"AKUMA\"", "\"ALPHA\"", "\"AMBULANCE\"", "\"APC\"", "\"ARDENT\"", "\"ASEA\"", "\"ASTEROPE\"", "\"AUTARCH\"", "\"AVARUS\"", "\"BAGGER\"", "\"BALLER3\"", "\"BALLER\"", 
"\"BANSHEE2\"", "\"BANSHEE\"", "\"BARRAGE\"", "\"BATI\"", "\"BESTIAGTS\"", "\"BF400\"", "\"BFINJECTION\"", "\"BIFTA\"", "\"BISON3\"", "\"BJXL\"", "\"BLADE\"", "\"BLAZER3\"", "\"BLAZER5\"", "\"BLAZER\"", "\"BLISTA2\"", "\"BLISTA\"", 
"\"BOBCATXL\"", "\"BODHI2\"", "\"BOXVILLE2\"", "\"BOXVILLE5\"", "\"BRAWLER\"", "\"BRICKADE\"", "\"BRIOSO\"", "\"BRUISER\"", "\"BRUTUS\"", "\"BTYPE2\"", "\"BTYPE\"", "\"BUCCANEER\"", "\"BUFFALO2\"", "\"BUFFALO\"", "\"BULLDOZER\"", "\"BULLET\"", 
"\"BURRITO3\"", "\"BUS\"", "\"CADDY\"", "\"CAMPER\"", "\"CARACARA\"", "\"CARBONIZZARE\"", "\"CARBONRS\"", "\"CASCO\"", "\"CAVALCADE\"", "\"CERBERUS\"", "\"CHEBUREK\"", "\"CHEETAH2\"", "\"CHEETAH\"", "\"CHERNOBOG\"", "\"CHIMERA\"", "\"CHINO\"", 
"\"CLIFFHANGER\"", "\"COG55\"", "\"COGCABRIO\"", "\"COGNOSCENTI\"", "\"COMET2\"", "\"CONTENDER\"", "\"COQUETTE2\"", "\"COQUETTE3\"", "\"CRUSADER\"", "\"CYCLONE\"", "\"DAEMON2\"", "\"DAEMON\"", "\"DEATHBIKE\"", "\"DEFILER\"", "\"DELUXO\"", 
"\"DEVESTE\"", "\"DEVIANT\"", "\"DIABLOUS2\"", "\"DIABLOUS\"", "\"DLOADER\"", "\"DOMINATOR2\"", "\"DOMINATOR\"", "\"DOMINATOR4\"", "\"DOUBLE\"", "\"DRAFTER\"", "\"DUBSTA3\"", "\"DUKES2\"", "\"DUKES\"", "\"DUMP\"", "\"DUNE2\"", "\"DUNE3\"", "\"DUNE\"",
"\"ELEGY2\"", "\"ELEGY\"", "\"ELLIE\"", "\"EMPEROR2\"", "\"EMPEROR\"", "\"ENDURO\"", "\"ENTITY2\"", "\"ENTITYXF\"", "\"ESSKEY\"", "\"EXEMPLAR\"", "\"F620\"", "\"FACTION\"", "\"FAGALOA\"", "\"FAGGIO3\"", "\"FAGGIO\"", "\"FBI2\"", "\"FBI\"", 
"\"FCR\"", "\"FELON\"", "\"FELTZER2\"", "\"FELTZER3\"", "\"FIRETRUK\"", "\"FLASHGT\"", "\"FMJ\"", "\"FORKLIFT\"", "\"FQ2\"", "\"FREECRAWLER\"", "\"FUGITIVE\"", "\"FUROREGT\"", "\"FUSILADE\"", "\"FUTO\"", "\"GARGOYLE\"", "\"GAUNTLET2\"", 
"\"GAUNTLET\"", "\"GB200\"", "\"GLENDALE\"", "\"GP1\"", "\"GRANGER\"", "\"GRESLEY\"", "\"GT500\"", "\"GUARDIAN\"", "\"HABANERO\"", "\"HAKUCHOU2\"", "\"HAKUCHOU\"", "\"HALFTRACK\"", "\"HANDLER\"", "\"HAULER2\"", "\"HAULER\"", "\"HERMES\"", 
"\"HEXER\"", "\"HOTKNIFE\"", "\"HOTRING\"", "\"HUNTLEY\"", "\"HUSTLER\"", "\"IMPALER2\"", "\"IMPERATOR\"", "\"INFERNUS2\"", "\"INFERNUS\"", "\"INGOT\"", "\"INNOVATION\"", "\"INSURGENT3\"", "\"INTRUDER\"", "\"ISSI2\"", "\"ISSI3\"",
"\"ITALIGTB\"", "\"ITALIGTO\"", "\"JACKAL\"", "\"JB700\"", "\"JESTER3\"", "\"JESTER\"", "\"JOURNEY\"", "\"KALAHARI\"", "\"KAMACHO\"", "\"KHAMELION\"", "\"KHANJALI\"", "\"KURUMA\"", "\"LANDSTALKER\"", "\"LE7B\"", "\"LECTRO\"", "\"LGUARD\"", 
"\"LIMO2\"", "\"LURCHER\"", "\"LYNX\"", "\"MAMBA\"", "\"MANANA\"", "\"MANCHEZ\"", "\"MARSHALL\"", "\"MASSACRO2\"", "\"MESA3\"", "\"MICHELLI\"", "\"MINIVAN\"", "\"MONROE\"", "\"MONSTER\"", "\"MONSTER3\"", "\"MOONBEAM\"","\"MOWER\"", 
"\"NEMESIS\"", "\"NEON\"", "\"NERO\"", "\"NIGHTBLADE\"", "\"NIGHTSHADE\"", "\"NIGHTSHARK\"", "\"NINEF2\"", "\"NINEF\"", "\"OMNIS\"", "\"OPPRESSOR\"", "\"OPPRESSOR2\"", "\"ORACLE2\"", "\"ORACLE\"", "\"OSIRIS\"", "\"PACKER\"", "\"PANTO\"",
"\"PARADISE\"", "\"PARIAH\"", "\"PATRIOT\"", "\"PCJ\"", "\"PENETRATOR\"", "\"PENUMBRA\"", "\"PEYOTE\"", "\"PFISTER811\"", "\"PHANTOM2\"", "\"PHANTOM3\"", "\"PHANTOM\"", "\"PHOENIX\"", "\"PICADOR\"", "\"PIGALLE\"", "\"POLICE2\"", 
"\"POLICEB\"", "\"POLICEOLD1\"", "\"POLICEOLD2\"", "\"POLICET\"", "\"POLICE\"", "\"PONY2\"", "\"POUNDER\"", "\"PRAIRIE\"", "\"PRANGER\"", "\"PREMIER\"", "\"PRIMO\"", "\"PROTOTIPO\"", "\"RADI\"", "\"RAIDEN\"", "\"RALLYTRUCK\"", "\"RANCHERXL\"", 
"\"RAPIDGT3\"", "\"RAPIDGT\"", "\"RAPTOR\"", "\"RATBIKE\"", "\"RATLOADER2\"", "\"RATLOADER\"", "\"RCBANDITO\"", "\"REAPER\"", "\"REBEL2\"", "\"REBEL\"", "\"REGINA\"", "\"RETINUE\"", "\"REVOLTER\"", "\"RHAPSODY\"", "\"RHINO\"", "\"RIATA\"", 
"\"ROCOTO\"", "\"ROMERO\"", "\"RUFFIAN\"", "\"RUINER2\"", "\"RUINER\"", "\"RUMPO2\"", "\"RUMPO3\"", "\"RUSTON\"", "\"SABREGT\"", "\"SADLER\"", "\"SANCHEZ\"", "\"SANCTUS\"", "\"SANDKING\"", "\"SAVESTRA\"", "\"SC1\"", "\"SCHAFTER2\"", "\"SCHAFTER3\"", 
"\"SCHWARZER\"", "\"SCRAMJET\"", "\"SEMINOLE\"", "\"SENTINEL3\"", "\"SENTINEL\"", "\"SERRANO\"", "\"SEVEN70\"", "\"SHEAVA\"", "\"SHERIFF2\"", "\"SHERIFF\"", "\"SHOTARO\"", "\"SLAMVAN3\"", "\"SLAMVAN\"", "\"SOVEREIGN\"", 
"\"SPECTER2\"", "\"SPEEDO\"", "\"STAFFORD\"", "\"STALION\"", "\"STANIER\"", "\"STINGER\"", "\"STOCKADE\"", "\"STRATUM\"", "\"STREITER\"", "\"STRETCH\"", "\"STROMBERG\"", "\"SULTANRS\"", "\"SULTAN\"", "\"SUPERD\"", "\"SURANO\"", "\"SURFER\"", 
"\"SURGE\"", "\"SWINGER\"", "\"T20\"", "\"TACO\"", "\"TAILGATER\"", "\"TAIPAN\"", "\"TAMPA2\"", "\"TAMPA\"", "\"TAXI\"", "\"TECHNICAL2\"", "\"TECHNICAL\"", "\"TEMPESTA\"", "\"TERBYTE\"", "\"TEZERACT\"", "\"THRUSTER\"", "\"THRUST\"", "\"TIPTRUCK\"", 
"\"TORERO\"", "\"TORNADO4\"", "\"TORNADO6\"", "\"TORNADO\"", "\"TOURBUS\"", "\"TOWTRUCK2\"", "\"TOWTRUCK\"", "\"TRACTOR2\"", "\"TRACTOR\"", "\"TRASH\"", "\"TROPHYTRUCK2\"", "\"TROPHYTRUCK\"", "\"TROPOS\"", "\"TULIP\"", "\"TURISMO2\"", "\"TURISMOR\"", 
"\"TYRANT\"", "\"TYRUS\"", "\"UTILLITRUCK2\"", "\"UTILLITRUCK3\"", "\"VACCA\"", "\"VADER\"", "\"VAGNER\"", "\"VERLIERER2\"", "\"VIGERO\"", "\"VIGILANTE\"", "\"VINDICATOR\"", "\"VIRGO3\"", "\"VIRGO\"", "\"VISERIS\"", "\"VISIONE\"", "\"VOLTIC\"", 
"\"VOODOO2\"", "\"VORTEX\"", "\"WARRENER\"", "\"WASHINGTON\"", "\"WASTELANDER\"", "\"WINDSOR\"", "\"WOLFSBANE\"", "\"XA21\"", "\"XLS\"", "\"YOSEMITE\"", "\"YOUGA2\"", "\"YOUGA\"", "\"Z190\"", "\"ZENTORNO\"", "\"ZION\"", "\"ZOMBIEB\"", "\"ZR380\"", 
"\"ZTYPE\"" };

/***
* METHODS
*/

void reset_vehmodmenu_globals();

void add_vehmodmenu_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_vehmodmenu_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_vehmodmenu(std::vector<StringPairSettingDBRow>* settings);

void update_vehmodmenu_features(BOOL playerExists, Ped playerPed);

void add_engine_sound(Vehicle veh);