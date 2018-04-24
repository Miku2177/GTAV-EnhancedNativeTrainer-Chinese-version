#include "vehicles.h"

#include "..\debug\debuglog.h"

extern int current_picked_engine_sound;

const static std::string ENGINE_SOUND[] = { "\"LE7B\"", "\"BANSHEE\"", "\"BANSHEE2\"", "\"TAIPAN\"", "\"CYCLONE\"", "\"VOLTIC\"", "\"VAGNER\"", "\"SHEAVA\"", "\"CHEETAH\"", "\"CHEETAH2\"", "\"TURISMOR\"", "\"TURISMO2\"", "\"VISIONE\"", 
"\"PROTOTIPO\"", "\"SULTANRS\"", "\"PENETRATOR\"", "\"XA21\"", "\"AUTARCH\"", "\"ENTITYXF\"", "\"ENTITY2\"", "\"TYRANT\"", "\"INFERNUS\"", "\"INFERNUS2\"", "\"OSIRIS\"", "\"REAPER\"", "\"TEMPESTA\"", "\"TEZERACT\"", "\"VACCA\"", 
"\"ZENTORNO\"", "\"PFISTER811\"", "\"GP1\"", "\"ITALIGTB\"", "\"T20\"", "\"TYRUS\"", "\"ADDER\"", "\"NERO\"", "\"SC1\"", "\"BULLET\"", "\"FMJ\"", "\"VIGILANTE\"", "\"ALPHA\"", "\"ELEGY\"", "\"ELEGY2\"", "\"FELTZER2\"", "\"FELTZER3\"", 
"\"SCHAFTER2\"", "\"SCHAFTER3\"", "\"SCHWARZER\"", "\"STREITER\"", "\"SURANO\"", "\"RAPTOR\"", "\"BUFFALO\"", "\"BUFFALO2\"", "\"VERLIERER2\"", "\"RAIDEN\"", "\"TAMPA2\"", "\"TAMPA\"", "\"HOTRING\"", "\"MASSACRO2\"", "\"RAPIDGT\"", 
"\"RAPIDGT3\"", "\"SEVEN70\"", "\"SPECTER2\"", "\"BLISTA\"", "\"BLISTA2\"", "\"JESTER\"", "\"JESTER3\"", "\"BESTIAGTS\"", "\"CARBONIZZARE\"", "\"KHAMELION\"", "\"RUSTON\"", "\"COQUETTE2\"", "\"COQUETTE3\"", "\"FUTO\"", "\"KURUMA\"", 
"\"SULTAN\"", "\"FUROREGT\"", "\"TROPOS\"", "\"PENUMBRA\"", "\"NINEF\"", "\"NINEF2\"", "\"OMNIS\"", "\"LYNX\"", "\"PARIAH\"", "\"COMET2\"", "\"NEON\"", "\"FUSILADE\"", "\"REVOLTER\"", "\"SENTINEL3\"", "\"SENTINEL\"", "\"FLASHGT\"", 
"\"GB200\"", "\"BTYPE2\"", "\"BTYPE\"", "\"MANANA\"", "\"SAVESTRA\"", "\"MAMBA\"", "\"TORNADO\"", "\"TORNADO4\"", "\"TORNADO6\"", "\"JB700\"", "\"GT500\"", "\"STINGER\"", "\"DELUXO\"", "\"Z190\"", "\"CASCO\"", "\"MICHELLI\"", 
"\"PIGALLE\"", "\"VISERIS\"", "\"ARDENT\"", "\"STROMBERG\"", "\"MONROE\"", "\"TORERO\"", "\"CHEBUREK\"", "\"ZTYPE\"", "\"PEYOTE\"", "\"RETINUE\"", "\"FAGALOA\"", "\"EXEMPLAR\"", "\"COGCABRIO\"", "\"WINDSOR\"", "\"FELON\"", "\"F620\"", 
"\"JACKAL\"", "\"ORACLE2\"", "\"ORACLE\"", "\"ZION\"", "\"BUCCANEER\"", "\"HERMES\"", "\"LURCHER\"", "\"VIRGO\"", "\"VIRGO3\"", "\"GAUNTLET\"", "\"GAUNTLET2\"", "\"RATLOADER2\"", "\"RATLOADER\"", "\"PICADOR\"", "\"MOONBEAM\"",
"\"SABREGT\"", "\"STALION\"", "\"VIGERO\"", "\"VOODOO2\"", "\"YOSEMITE\"", "\"DUKES2\"", "\"DUKES\"", "\"NIGHTSHADE\"", "\"PHOENIX\"", "\"RUINER\"", "\"RUINER2\"", "\"BLADE\"", "\"CHINO\"", "\"DOMINATOR\"", "\"DOMINATOR2\"", "\"ELLIE\"", 
"\"HOTKNIFE\"", "\"HUSTLER\"", "\"SLAMVAN\"", "\"SLAMVAN3\"", "\"FACTION\"", "\"DUBSTA3\"", "\"BIFTA\"", "\"DUNE\"", "\"DUNE3\"", "\"DUNE2\"", "\"BFINJECTION\"", "\"DLOADER\"", "\"BODHI2\"", "\"KALAHARI\"", "\"KAMACHO\"", "\"MESA3\"", 
"\"MARSHALL\"", "\"BRAWLER\"", "\"RANCHERXL\"", "\"INSURGENT3\"", "\"NIGHTSHARK\"", "\"REBEL2\"", "\"REBEL\"", "\"TECHNICAL\"", "\"TECHNICAL2\"", "\"BLAZER\"", "\"BLAZER3\"", "\"BLAZER5\"", "\"CARACARA\"", "\"TROPHYTRUCK2\"", 
"\"TROPHYTRUCK\"", "\"RIATA\"", "\"SANDKING\"", "\"MONSTER\"", "\"CAVALCADE\"", "\"SERRANO\"", "\"XLS\"", "\"GRESLEY\"", "\"SEMINOLE\"", "\"GRANGER\"", "\"LANDSTALKER\"", "\"HABANERO\"", "\"HUNTLEY\"", "\"FQ2\"", "\"BALLER\"", 
"\"BALLER3\"", "\"BJXL\"", "\"PATRIOT\"", "\"ROCOTO\"", "\"CONTENDER\"", "\"RADI\"", "\"EMPEROR\"", "\"EMPEROR2\"", "\"PRIMO\"", "\"WASHINGTON\"", "\"GLENDALE\"", "\"LIMO2\"", "\"ROMERO\"", "\"FUGITIVE\"", "\"SURGE\"", "\"ASEA\"", 
"\"PREMIER\"", "\"REGINA\"", "\"STRETCH\"", "\"COGNOSCENTI\"", "\"COG55\"", "\"SUPERD\"", "\"ASTEROPE\"", "\"INTRUDER\"", "\"TAILGATER\"", "\"STANIER\"", "\"INGOT\"", "\"WARRENER\"", "\"STRATUM\"", "\"PANTO\"", "\"PRAIRIE\"", 
"\"RHAPSODY\"", "\"BRIOSO\"", "\"ISSI2\"", "\"ISSI3\"", "\"HANDLER\"",  "\"BULLDOZER\"", "\"DUMP\"", "\"GUARDIAN\"", "\"STOCKADE\"", "\"HAULER\"", "\"HAULER2\"", "\"PHANTOM\"", "\"PHANTOM3\"", "\"PHANTOM2\"", "\"PACKER\"", "\"POUNDER\"", 
"\"SURFER\"", "\"BISON3\"", "\"PARADISE\"", "\"RUMPO2\"", "\"RUMPO3\"", "\"YOUGA\"", "\"YOUGA2\"", "\"BOXVILLE2\"", "\"BOXVILLE5\"", "\"CAMPER\"", "\"PONY2\"", "\"TACO\"", "\"BURRITO3\"", "\"BOBCATXL\"", "\"MINIVAN\"", "\"SPEEDO\"", 
"\"JOURNEY\"", "\"AIRTUG\"", "\"BUS\"", "\"TOURBUS\"", "\"FORKLIFT\"", "\"TRASH\"", "\"CADDY\"", "\"BRICKADE\"", "\"RALLYTRUCK\"", "\"WASTELANDER\"", "\"TRACTOR2\"", "\"TRACTOR\"", "\"MOWER\"", "\"TAXI\"", "\"TIPTRUCK\"", 
"\"UTILLITRUCK3\"", "\"UTILLITRUCK2\"", "\"SADLER\"", "\"TOWTRUCK\"", "\"TOWTRUCK2\"", "\"AMBULANCE\"", "\"BARRAGE\"", "\"FBI\"", "\"FBI2\"", "\"HALFTRACK\"", "\"CRUSADER\"", "\"CHERNOBOG\"", "\"LGUARD\"", "\"PRANGER\"", "\"SHERIFF\"", 
"\"SHERIFF2\"", "\"FIRETRUK\"", "\"APC\"", "\"THRUSTER\"", "\"RHINO\"", "\"KHANJALI\"", "\"POLICE2\"", "\"POLICE\"", "\"POLICEOLD1\"", "\"POLICEOLD2\"", "\"POLICET\"", "\"POLICEB\"", "\"AKUMA\"", "\"DOUBLE\"", "\"ENDURO\"", "\"THRUST\"", 
"\"VINDICATOR\"", "\"AVARUS\"", "\"HEXER\"", "\"INNOVATION\"", "\"SANCTUS\"", "\"MANCHEZ\"", "\"SANCHEZ\"", "\"BF400\"", "\"CARBONRS\"", "\"CHIMERA\"", "\"SHOTARO\"", "\"BATI\"", "\"ESSKEY\"", "\"FAGGIO3\"", "\"FAGGIO\"", "\"FCR\"", 
"\"OPPRESSOR\"", "\"RUFFIAN\"", "\"VORTEX\"", "\"DIABLOUS\"", "\"DIABLOUS2\"", "\"LECTRO\"", "\"NEMESIS\"", "\"DEFILER\"", "\"HAKUCHOU\"", "\"HAKUCHOU2\"", "\"PCJ\"", "\"VADER\"", "\"BAGGER\"", "\"CLIFFHANGER\"", "\"DAEMON\"", "\"DAEMON2\"", 
"\"GARGOYLE\"", "\"NIGHTBLADE\"", "\"RATBIKE\"", "\"SOVEREIGN\"", "\"WOLFSBANE\"", "\"ZOMBIEB\"" };