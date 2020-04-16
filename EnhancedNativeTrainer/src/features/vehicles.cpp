/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "..\..\resource.h"
#include "vehicles.h"
#include "fuel.h"
#include "enginedegrade.h"
#include "road_laws.h"
#include "speed_altitude.h"
#include "..\features\vehmodmenu.h"
#include "hotkeys.h"
#include "script.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"
#include "..\debug\debuglog.h"
#include "area_effect.h"
#include <fstream>
#include "vehicle_weapons.h"
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <cstdlib>

using namespace std;

const static int ENGINE_SOUND_COUNT_VEHICLES = 367; // 366

bool featureVehInvincible = false;
bool featureVehInvincibleUpdated = false;

bool featureVehNoDamage = false;

bool featureVehInvulnIncludesCosmetic = false;

bool feature3rdpersonviewonly, featureDaytimeonly = false;
bool featureHazards = true;

bool window_up = true;
bool high_speed = false;

int veh_jumped_n = 0;

int Accel_secs_passed, Accel_secs_curr, Accel_seconds = 0;

bool reversing_c = false;
bool accelerating_c = false; 

Vehicle current_veh_e = -1;
Vehicle temp_vehicle, playerVehicle_s = -1;
Ped temp_ped = -1;

std::vector<Object> SPIKES;
bool s_message = false;

bool speed_limit_e = false;

bool airstrike = false;
Object nuke1, nuke2, nuke3 = -1;
float nuke_h1_coord, nuke_h2_coord, nuke_h3_coord = -1;

bool viz_veh_ind_left, viz_veh_ind_right = false;

bool turn_check_left, turn_check_right = false;
bool controllightsenabled_l = false;
bool controllightsenabled_r = false;
bool autocontrol = false;
bool speedlimiter_switch = true;
bool LightAlwaysOff = true;

bool alarmischarged = false;
bool alarm_enabled = false;

bool nitro_e = false;

int turn_angle = 0;

int traction_tick = 0;
int Time_tick_mileage = 0;

float mileage, signal_meters = 0;

bool featureNoVehFallOff = false;
bool featureVehSpeedBoost = false;
bool featureVehSteerAngle = false;
bool featureRollWhenShoot = false;
bool featureTractionControl = false;
bool featureSticktoground = false;
bool featureDropSpikes = false;
bool featureAirStrike = false;
bool featureReverseWhenBraking = false;
bool featureEngineRunning = false;
bool featureNoVehFlip = false;
bool featureAutoToggleLights = false;
bool featureMileage = false;
bool featureSeasharkLights = false;
bool featureVehMassMult = false;
bool featureVehSpawnInto = false;
bool featureVehSpawnTuned = false;
bool featureVehSpawnOptic = false;
bool featureVehicleDoorInstant = false;
bool featureLockVehicleDoors = false;
bool featureLockVehicleDoorsUpdated = false;
bool featureWearHelmetOff = false;
bool featureWearHelmetOffUpdated = false;
bool featureVehLightsOn = false, featureVehLightsOnUpdated = false;
bool window_roll, interior_lights, veh_searching, veh_alarm, veh_brake_toggle, vehicle_burnout_toggle = false;
int lights = -1, highbeams = -1;

Vehicle vehicle_been_used = -1;
Vehicle veh_l = -1;

bool steered_left, steered_right = false;
Vehicle veh_steering;
std::vector<Vehicle> STEERING;

int currseat = -1;

int engine_tick = 0;
int engine_secs_passed, engine_secs_curr = 0;

BOOL lightsAutoOn = -1;
BOOL highbeamsAutoOn = -1;
bool no_autotoggle = false;
bool autotoggle_temp = false;

bool being_in_city = false;
bool being_on_motorway = false;

// Remember Vehicles Option Variables
Blip blip_veh[1];
std::vector<Blip> BLIPTABLE_VEH;
std::vector<Vehicle> VEHICLES_REMEMBER;
bool been_already = false;
Vehicle curr_veh_remember;
Ped old_playerPed_Tracking = -1;
bool featureRememberVehicles = false;
bool featureDeleteTrackedVehicles = true;
bool featureDeleteTrackedVehicles_Emptied = false;
bool featureDeleteTrackedVehicles_CharacterChanged = true;
bool featureBlipNumber = true;
bool featureAutoalarm = false;
Vehicle alarmed_veh = -1;
bool near_enough = false;
int a_counter_tick = 0;

int Shut_seconds = -1; 

int nitrous_m = -2;

int sheshark_light_toogle = 1;

Vehicle ped_temp_veh = -1;

bool featureDespawnScriptDisabled = false;
bool featureDespawnScriptDisabledUpdated = false;
bool featureDespawnScriptDisabledWasLastOn = false; //do not persist this particular var in the DB - it is local only

int activeLineIndexVeh = 0;
int activeSavedVehicleIndex = -1;
int activeLineIndexSpeed = 0;
int activeLineIndexVisualize = 0;
int activeLineIndexSpeedlimit = 0;
int activeLineIndexFuel = 0;
int activeLineIndexEngineDegrade = 0;
int activeLineIndexRemember = 0;
int activeLineIndexRoadLaws = 0;
std::string activeSavedVehicleSlotName;
int lastKnownSavedVehicleCount = 0;
bool vehSaveMenuInterrupt = false;
bool vehSaveSlotMenuInterrupt = false;
bool requireRefreshOfVehSaveSlots = false;
bool requireRefreshOfVehSlotMenu = false;

// Drop Anchor Variables
Vector3 coords_b;
Object b_rope = -1;
Vehicle veh_anchor = -1;
bool anchor_dropped = false;

const int PED_FLAG_THROUGH_WINDSCREEN = 32;

const std::vector<std::string> VEH_INVINC_MODE_CAPTIONS{"OFF", "Mech. Only", "Mech. + Visual", "Mech. + Vis. + Cosmetic"};

const std::vector<std::string> VEH_SPEED_BOOST_CAPTIONS{"Only When Already Moving", "Nothing Can Stop Me", "Fastest in the World"};
int speedBoostIndex = 0;

// engine power stuff
int engPowMultIndex = 0;
bool powChanged = true;

bool burnoutApplied = false;

//vehicle mass stuff
int VehMassMultIndex = 0;
bool massChanged = true;
int current_player_forceshield = 0;
bool player_massChanged = true;

//Turn Signals
const std::vector<std::string> VEH_TURN_SIGNALS_CAPTIONS{ "OFF", "Manual Only", "< 10 (MPH)", "< 20 (MPH)", "< 30 (MPH)", "< 40 (MPH)", "< 60 (MPH)", "< 80 (MPH)", "< 100 (MPH)", "< 120 (MPH)", "< 140 (MPH)", "< 160 (MPH)", "< 180 (MPH)", "< 200 (MPH)" };
const std::vector<int> VEH_TURN_SIGNALS_VALUES{ 0, 1, 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
int turnSignalsIndex = 0;
bool turnSignalsChanged = true;

//Turn Signals Angle
const std::vector<std::string> VEH_TURN_SIGNALS_ANGLE_CAPTIONS{ "0", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100" };
const std::vector<int> VEH_TURN_SIGNALS_ANGLE_VALUES{ 0, 1, 5, 10, 15, 17, 20, 25, 30, 40, 50 };
int turnSignalsAngleIndex = 5;
bool turnSignalsAngleChanged = true;

//Turn Signals Off Acceleration
const std::vector<std::string> VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS{ "OFF", "1", "2", "3", "4", "5", "7", "10" };
const std::vector<int> VEH_TURN_SIGNALS_ACCELERATION_VALUES{ 0, 1, 2, 3, 4, 5, 7, 10 };
int turnSignalsAccelerationIndex = 4;
bool turnSignalsAccelerationChanged = true;
int JumpyVehIndex = 0;
bool JumpyVehChanged = true;
int HeavyVehIndex = 0;
bool HeavyVehChanged = true;

//Visualize Vehicle Indicators (Sprite)
const std::vector<std::string> VEH_VISLIGHT_CAPTIONS{ "OFF", "1x", "3x", "5x", "7x", "10x", "12x" };
const std::vector<double> VEH_VISLIGHT_VALUES{ 0, 0.01, 0.03, 0.05, 0.07, 0.1, 0.2 };
int VisLightIndex = 0;
bool VisLight_Changed = true;

//Visualize Vehicle Indicators (Vector)
int VisLight3dIndex = 0;
bool VisLight3d_Changed = true;

//Speed Limiter
int speedLimiterIndex = 0;
bool speedLimiterChanged = true;
int DoorAutolockIndex = 0;
bool DoorAutolockChanged = true;
int speedCityLimiterIndex = 0;
bool speedCityLimiterChanged = true;
int speedCountryLimiterIndex = 0;
bool speedCountryLimiterChanged = true;

//Lights OFF
const std::vector<std::string> VEH_LIGHTSOFF_CAPTIONS{ "Never", "Daytime Only", "Always" };
const std::vector<int> VEH_LIGHTSOFF_VALUES{ 0, 1, 2 };
int lightsOffIndex = 0;
bool lightsOffChanged = true;

//Number Of Vehicles To Remember
const std::vector<std::string> VEH_VEHREMEMBER_CAPTIONS{ "3", "5", "7", "10", "15", "20", "30", "40", "50", "60" };
const std::vector<int> VEH_VEHREMEMBER_VALUES{ 3, 5, 7, 10, 15, 20, 30, 40, 50, 60 };
int VehRememberIndex = 3;
bool VehRemember_Changed = true;

//Blip Size
const std::vector<std::string> VEH_BLIPSIZE_CAPTIONS{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
const std::vector<double> VEH_BLIPSIZE_VALUES{ 0.3, 0.5, 0.8, 1.0, 1.2, 1.5, 1.7, 2.0, 2.5, 3.0 };
int VehBlipSizeIndex = 2;
bool VehBlipSize_Changed = true;

//Blip Colour
int VehBlipColourIndex = 4;
bool VehBlipColour_Changed = true;
int VehColourIndex = 0;
bool VehColour_Changed = true;

//Blip Symbol
const std::vector<std::string> VEH_BLIPSYMBOL_CAPTIONS{ "Standard", "Player", "North", "Waypoint", "BigCircleOutline", "ArrowUpOutlined", "ArrowDownOutlined", "ArrowUp", "ArrowDown", "PoliceHelicopterAnimated", "Jet" };
const std::vector<int> VEH_BLIPSYMBOL_VALUES{ 1, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16 }; 
int VehBlipSymbolIndex = 0;
bool VehBlipSymbol_Changed = true;

//Blip Flashing
int VehBlipFlashIndex = 0;
bool VehBlipFlash_Changed = true;

//Keep The Engine Running
const std::vector<std::string> VEH_ENGINERUNNING_CAPTIONS{ "Never", "Always", "Hold Exit To Kill Engine" };
int EngineRunningIndex = 0;
bool EngineRunning_Changed = true;

//Infinite Rocket Boost
const std::vector<std::string> VEH_INFINITEBOOST_CAPTIONS{ "OFF", "Hold", "Always" };
const std::vector<int> VEH_INFINITEBOOST_VALUES{ 0, 1, 2 };
int InfiniteBoostIndex = 0;
bool InfiniteBoost_Changed = true;

//Auto-shut engine after
const std::vector<std::string> VEH_AUTO_SHUT_ENGINE_CAPTIONS{ "OFF", "5", "10", "20", "30" };
const std::vector<int> VEH_AUTO_SHUT_ENGINE_VALUES{ 0, 5, 10, 20, 30 };
int AutoShutEngineIndex = 0;
bool AutoShutEngineChanged = true;

// Hydraulics
const std::vector<std::string> VEH_HYDRAULICS_CAPTIONS{ "OFF", "-0.20", "-0.10", "0.10", "0.20" };
const std::vector<float> VEH_HYDRAULICS_VALUES{ 0.0f, -0.20f, -0.10f, 0.10f, 0.20f };
int HydraulicsIndex = 0;

// Nitrous
int NitrousIndex = 0;
bool NitrousChanged = true;

// player in vehicle state... assume true initially since our quicksave might have us in a vehicle already, in which case we can't check if we just got into one
bool oldVehicleState = true;

//Door Options list + struct
struct struct_door_options{
	std::string text;
	bool *pState;
};

int doorOptionsMenuIndex, vehSeatIndexMenuIndex = 0;

int savedVehicleListSortMethod = 0;
bool vehSaveSortMenuInterrupt = false;

//Top Level

const std::vector<std::string> MENU_VEHICLE_CATEGORIES{ "Cars", "Industrial and Services", "Emergency and Military", "Motorcycles", "Planes", "Helicopters", "Boats", "Bicycles", "Enter Name Manually" };

//Cars

const std::vector<std::string> MENU_CAR_CATEGORIES{ "Supercars", "Sports", "Sports Classics", "Coupes", "Muscle", "Offroad", "SUVs", "Sedans", "Compacts"/*, "Lowriders"*/ };

const std::vector<std::string> CAPTIONS_SUPERCARS{ "Annis RE-7B (Race)", "Annis S80RR (Diamond Casino)", "Benefactor Krieger (Diamond Casino)", "Bravado Banshee 900R", "Cheval Taipan (Super Sports)", "Coil Cyclone", "Coil Rocket Voltic (Import-Export)", "Coil Voltic", "Declasse Scramjet (After Hours)", "Dewbauchee Vagner (Gun Runner)", "Emperor ETR1 (Race)", "Grotti Cheetah", "Grotti Furia (Casino Heist)", "Grotti Turismo R", "Grotti Visione (Smuggler's Run)", "Grotti X80 Proto (Executive)", "Karin Sultan RS", "Ocelot Penetrator (Import-Export)", "Ocelot R88 (Casino Heist)", "Ocelot XA-21 (Gun Runner)", "Overflod Autarch (Doomsday Heist)", "Overflod Entity XF", "Overflod Entity XXR (Super Sports)", "Overflod Tyrant (Super Sports)", "Pegassi Infernus", "Pegassi Osiris", "Pegassi Reaper (Executive)", "Pegassi Tempesta (Import-Export)", "Pegassi Tezeract (Super Sport)", "Pegassi Vacca", "Pegassi Zentorno", "Pegassi Zorrusso (Diamond Casino)", "Pfister 811 (Executive)", "Principe Deveste Eight (Arena War)", "Progen Emerus (Diamond Casino)" ,"Progen GP1 (Special Races)", "Progen Itali GTB (Import-Export)", "Progen Itali GTB Custom (Import - Export)", "Progen PR4 (Casino Heist)", "Progen T20", "Progen Tyrus (Race)", "Truffade Adder", "Truffade Nero (Import-Export)", "Truffade Nero Custom (Import-Export)", "Truffade Thrax (Diamond Casino)", "Ubermacht SC1 (Doomsday Heist)", "Vapid Bullet", "Vapid FMJ (Executive)", "Vigilante (Smuggler's Run)" };

const std::vector<std::string> CAPTIONS_SPORTS{ "Albany Alpha", "Albany V-STR (Casino Heist)", "Annis Elegy Retro Custom (Import-Export)", "Annis Elegy RH8", "Annis ZR380 Apocalypse (Arena War)", "Annis ZR380 Future Shock (Arena War)", "Annis ZR380 Nightmare (Arena War)", "Benefactor Feltzer", "Benefactor Schafter LWB", "Benefactor Schafter V12", "Benefactor Schlagen GT (Arena War)", "Benefactor Schwartzer", "Benefactor Streiter (Doomsday Heist)", "Benefactor Surano", "BF Raptor (Bikers)", "Bravado Banshee", "Bravado Buffalo", "Bravado Buffalo S", "Bravado Buffalo S (Race)", "Bravado Verlierer", "Coil Raiden (Doomsday Heist)", "Declasse Drift Tampa (Race)", "Declasse Hotring Sabre (Super Sports)", "Dewbauchee Massacro", "Dewbauchee Massacro Racecar (Race)", "Dewbauchee Rapid GT", "Dewbauchee Rapid GT Cabrio", "Dewbauchee Seven-70 (Executive)", "Dewbauchee Spector (Import-Export)", "Dewbauchee Spector Custom (Import-Export)", "Dinka Blista Compact", "Dinka Blista Compact (Go Go Monkey Race)", "Dinka Jester", "Dinka Jester Classic (After Hours)", "Dinka Jester (Race)", "Dinka Sugoi (Casino Heist)", "Enus Paragon R (Diamond Casino)", "Enus Paragon R (Armored - Diamond Casino)", "Grotti Bestia GTS (Executive)", "Grotti Carbonizzare", "Grotti Itali GTO (Arena War)", "Hijak Khamelion", "Hijak Ruston (Special Races)", "Invetero Coquette", "Karin Futo", "Karin Kuruma", "Karin Kuruma (Armoured)", "Karin Sultan", "Karin Sultan Classic (Casino Heist)", "Lampadati Furore GT", "Lampadati Komoda (Casino Heist)", "Lampadati Tropos Rallye (Race)", "Maibatsu Penumbra", "Obey 8F Drafter (Diamond Casino)", "Obey 9F", "Obey 9F Cabrio", "Obey Omnis (Rally)", "Ocelot Jugular (Diamond Casino)", "Ocelot Locust (Diamond Casino)", "Ocelot Lynx (Race)", "Ocelot Pariah (Doomsday Heist)", "Overflod Imorgon (Casino Heist)", "Phister Comet", "Pfister Comet Retro Custom (Import-Export)", "Pfister Comet Safari (Doomsday Heist)", "Pfister Comet SR (Doomsday Heist)", "Pfister Neon (Doomsday Heist)", "Schyster Fusilade", "Ubermarcht Revolter (Doomsday Heist)", "Ubermacht Sentinel Classic (Doomsday Heist)", "Vapid Flash GT (Super Sports)", "Vapid GB200 (Super Sports)", "Vysser Neo (Diamond Casino)", "Weeny Issi Sport" };

const std::vector<std::string> CAPTIONS_SPORTSCLASSICS{ "Albany Franken Strange", "Albany Manana", "Albany Roosevelt", "Albany Roosevelt Valor", "Annis Savestra (Doomsday Heist)", "Benefactor Stirling GT", "Declasse Mamba", "Declasse Tornado", "Declasse Tornado (Custom)", "Declasse Tornado (Rusty)", "Declasse Tornado Cabrio", "Declasse Tornado Cabrio (Rusty)", "Declasse Tornado Rat Rod (Bikers)", "Dewbauchee JB 700", "Dewbauchee JB 700W (Casino Heist)", "Dewbauchee Rapid GT Classic (Smuggler's Run)", "Dinka Jester Classic (Sports Classics)", "Grotti Cheetah Classic (Gun Runner)",  "Grotti GT500 (Doomsday Heist)", "Grotti Stinger", "Grotti Stinger GT", "Grotti Turismo Classic (Special Races)", "Invetero Coquette Classic", "Imponte Deluxo (Doomsday Heist)", "Karin 190Z (Doomsday Heist)", "Lampadati Casco", "Lampadati Michelli GT (Super Sports)", "Lampadati Pigalle", "Lampadati Viseriss (Doomsday Heist)", "Ocelot Ardent (Gun Runner)", "Ocelot Stromberg (Doomsday Heist)", "Ocelot Swinger (After Hours)", "Pegassi Infernus Classic (Special Races)", "Pegassi Monroe", "Pegassi Torero", "Rune Cheburek (Super Sports)", "Truffade Z-Type", "Ubermacht Zion Classic (Diamond Casino)", "Vapid Peyote", "Vapid Retinue (Smuggler's Run)", "Vapid Retinue Mk2 (Casino Heist)", "Vulcar Fagaloa (Super Sports)", "Vulcar Nebula Turbo (Diamond Casino)", "Weeny Dynasty (Diamond Casino)" };

const std::vector<std::string> CAPTIONS_COUPES{ "Dewbauchee Exemplar", "Enus Cognoscenti Cabrio", "Enus Windsor", "Enus Windsor Drop (Executive)", "Lampadati Felon", "Lampadati Felon GT", "Ocelot F620", "Ocelot Jackal", "Ubermacht Oracle", "Ubermacht Oracle XS", "Ubermacht Sentinel", "Ubermacht Sentinel XS", "Ubermacht Zion", "Ubermacht Zion Cabrio" };

const std::vector<std::string> CAPTIONS_MUSCLE{ "Albany Buccaneer", "Albany Buccaneer (Custom)", "Albany Hermes (Doomsday Heist)", "Albany Lurcher", "Albany Virgo", "Bravado Gauntlet", "Bravado Gauntlet Classic (Diamond Casino)", "Bravado Gauntlet Hellfire (Diamond Casino)", "Bravado Gauntlet Redwood (Race)", "Bravado Rat-Truck", "Bravado Rat-Loader (Rusty)", "Cheval Picador", "Declasse Impaler", "Declasse Impaler Apocalypse (Arena War)", "Declasse Impaler Future Shock (Arena War)", "Declasse Impaler Nightmare (Arena War)", "Declasse Moonbeam", "Declasse Moonbeam (Custom)", "Declasse Sabre Turbo", "Declasse Sabre Turbo (Custom)", "Declasse Stallion", "Declasse Stallion (Race)", "Declasse Tampa", "Declasse Tampa Weaponized (Gun Runner)", "Declasse Tulip (Arena War)", "Declasse Vamos (Arena War)", "Declasse Vigero", "Declasse Voodoo", "Declasse Voodoo (Custom)", "Declasse Yosemite (Doomsday Heist)", "Declasse Yosemite Drift (Casino Heist)", "Dundreary Virgo Classic (Custom Donk)", "Dundreary Virgo Classic Custom", "Imponte Duke O' Death", "Imponte Dukes", "Imponte Nightshade", "Imponte Phoenix", "Imponte Ruiner", "Imponte Ruiner 2000 (Import-Export)", "Invetero Coquette BlackFin", "Schyster Deviant (Arena War)", "Vapid Blade", "Vapid Chino", "Vapid Chino (Custom)", "Vapid Clique (Arena War)", "Vapid Dominator", "Vapid Dominator GTX (Super Sports)", "Vapid Dominator (Race)", "Vapid Dominator Apocalypse (Arena War)", "Vapid Dominator Future Shock (Arena War)", "Vapid Dominator Nightmare (Arena War)", "Vapid Ellie (Super Sports)", "Vapid Hotknife", "Vapid Hustler (Doomsday Heist)", "Vapid Imperator Apocalypse (Arena War)", "Vapid Imperator Future Shock (Arena War)", "Vapid Imperator Nightmare (Arena War)", "Vapid Peyote Gasser (Diamond Casino)", "Vapid Slamvan", "Vapid Slamvan (Custom)", "Vapid Slamvan (Lost MC)", "Vapid Slamvan Apocalypse (Arena War)", "Vapid Slamvan Future Shock (Arena War)", "Vapid Slamvan Nightmare (Arena War)", "Willard Faction", "Willard Faction (Custom)", "Willard Faction (Custom Donk)" };

const std::vector<std::string> CAPTIONS_OFFROAD{ "Annis Hellion (Diamond Casino)", "Benefactor Bruiser Apocalypse (Arena War)", "Benefactor Bruiser Future Shock (Arena War)", "Benefactor Bruiser Nightmare (Arena War)", "Benefactor Dubsta 6x6", "BF Bifta", "BF Dune Buggy", "BF Dune FAV (Gun Runner)", "BF Injection", "BF Space Docker", "Bravado Duneloader", "Bravado Sasquatch Apocalypse (Arena War)", "Bravado Sasquatch Future Shock (Arena War)", "Bravado Sasquatch Nightmare (Arena War)", "Canis Bodhi", "Canis Freecrawler (After Hours)", "Canis Kalahari", "Canis Kamacho (Doomsday Heist)", "Canis Mesa (Off-Road)", "Cheval Marshall", "Coil Brawler", "Declasse Brutus Apocalypse (Arena War)", "Declasse Brutus Future Shock (Arena War)", "Declasse Brutus Nightmare (Arena War)", "Declasse Rancher XL", "Declasse Rancher XL (Snow)", "HVY Insurgent", "HVY Insurgent Pick-Up (Gun Runner)", "HVY Insurgent Pick-Up Custom (Gun Mount)", "HVY Menacer (After Hours)", "HVY Nightshark (Gun Runner)", "Karin Everon (Casino Heist)", "Karin Rebel", "Karin Rebel (Rusty)", "Karin Technical", "Karin Technical Aqua (Import-Export)", "Karin Technical Custom (Gun Runner)", "Maxwell Vagrant (Casino Heist)", "Nagasaki Blazer", "Nagasaki Blazer (Hot Rod)", "Nagasaki Blazer (Lifeguard)", "Nagasaki Blazer Aqua (Import-Export)", "Nagasaki Outlaw (Casino Heist)", "Nagasaki Street Blazer (Bikers)", "Ramp Buggy (Import-Export)", "Ramp Buggy Spoilerless (Import-Export)", "RC Bandito (Arena War)", "Rune Zhaba (Casino Heist)", "Vapid Caracara (Super Sports)", "Vapid Caracara 4x4 (Diamond Casino)", "Vapid Desert Raid (Race)", "Vapid Riata (Doomsday Heist)", "Vapid Sandking SWB", "Vapid Sandking XL", "Vapid Trophy Truck (Race)", "Vapid Liberator" };

const std::vector<std::string> CAPTIONS_SUVS{ "Albany Cavalcade", "Albany Cavalcade Mk2", "Benefactor Dubsta", "Benefactor Dubsta (Flat Black)", "Benefactor Serrano", "Benefactor XLS (Executive)", "Benefactor XLS Armoured (Executive)", "Bravado Gresley", "Canis Mesa", "Canis Mesa (Snow)", "Canis Seminole", "Declasse Granger", "Dundreary Landstalker", "Emperor Habanero", "Enus Huntley S", "Fathom FQ 2", "Gallivanter Baller (Large)", "Gallivanter Baller (Small)", "Gallivanter Baller LE (Large)", "Gallivanter Baller LE LWB (Small)", "Gallivanter Baller LE (Armoured)", "Gallivanter Baller LE LWB (Armoured)", "Karin BeeJay XL", "Lampadati Novak (Diamond Casino)", "Mammoth Patriot", "Mammoth Patriot Stretch (After Hours)", "Obey Rocoto", "Pegassi Toros (Arena War)", "Ubermacht Rebla GTS (Casino Heist)", "Vapid Contender (Race)", "Vapid Radius" };

const std::vector<std::string> CAPTIONS_SEDANS{ "Albany Emperor", "Albany Emperor (Rusty)", "Albany Emperor (Snow)", "Albany Primo", "Albany Primo (Custom)", "Albany Washington", "Benefactor Glendale", "Benefactor Schafter", "Benefactor Schafter LWB Armoured", "Benefactor Schafter V12 Armoured", "Benefactor Turreted Limo", "Chariot Romero Hearse", "Cheval Fugitive", "Cheval Surge", "Declasse Asea", "Declasse Asea (Snow)", "Declasse Premier", "Dundreary Regina", "Dundreary Stretch", "Enus Cognoscenti", "Enus Cognoscenti Armoured", "Enus Cognoscenti 55", "Enus Cognoscenti 55 Armoured", "Enus Stafford (After Hours)", "Enus Super Diamond", "Karin Asterope", "Karin Intruder", "Obey Tailgater", "Vapid Stanier", "Vulcan Ingot", "Vulcar Warrener", "Zirconium Stratum" };

const std::vector<std::string> CAPTIONS_COMPACTS{ "Benefactor Panto", "Bollokan Prairie", "Declasse Rhapsody", "Dinka Blista", "Dinka Blista Kanjo (Casino Heist)", "Grotti Brioso R/A (Race)",  "Karin Dilettante", "Karin Dilettante (Liveried)", "Maxwell Asbo (Casino Heist)", "Weeny Issi", "Weeny Issi Classic (Super Sports)", "Weeny Issi Apocalypse (Arena War)", "Weeny Issi Future Shock (Arena War)", "Weeny Issi Nightmare (Arena War)" };

const std::vector<std::string> VALUES_SUPERCARS{ "LE7B", "S80", "KRIEGER", "BANSHEE2", "TAIPAN", "CYCLONE", "VOLTIC2", "VOLTIC", "SCRAMJET", "VAGNER", "SHEAVA", "CHEETAH", "FURIA", "TURISMOR", "VISIONE", "PROTOTIPO", "SULTANRS", "PENETRATOR", "FORMULA2", "XA21", "AUTARCH", "ENTITYXF", "ENTITY2", "TYRANT", "INFERNUS", "OSIRIS", "REAPER", "TEMPESTA", "TEZERACT", "VACCA", "ZENTORNO", "ZORRUSSO", "PFISTER811", "DEVESTE", "EMERUS", "GP1", "ITALIGTB", "ITALIGTB2", "FORMULA", "T20", "TYRUS", "ADDER", "NERO", "NERO2", "THRAX", "SC1", "BULLET", "FMJ", "VIGILANTE" };

const std::vector<std::string> VALUES_SPORTS{ "ALPHA", "VSTR", "ELEGY", "ELEGY2", "ZR380", "ZR3802", "ZR3803", "FELTZER2", "SCHAFTER4", "SCHAFTER3", "SCHLAGEN", "SCHWARZER", "STREITER", "SURANO", "RAPTOR", "BANSHEE", "BUFFALO", "BUFFALO2", "BUFFALO3", "VERLIERER2", "RAIDEN", "TAMPA2", "HOTRING", "MASSACRO", "MASSACRO2", "RAPIDGT", "RAPIDGT2", "SEVEN70", "SPECTER", "SPECTER2", "BLISTA2", "BLISTA3", "JESTER", "JESTER3", "JESTER2", "SUGOI", "PARAGON", "PARAGON2", "BESTIAGTS", "CARBONIZZARE", "ITALIGTO", "KHAMELION", "RUSTON", "COQUETTE", "FUTO", "KURUMA", "KURUMA2", "SULTAN", "SULTAN2", "FUROREGT", "KOMODA", "TROPOS", "PENUMBRA", "DRAFTER", "NINEF", "NINEF2", "OMNIS", "JUGULAR", "LOCUST", "LYNX", "PARIAH", "IMORGON", "COMET2", "COMET3", "COMET4", "COMET5", "NEON", "FUSILADE", "REVOLTER", "SENTINEL3", "FLASHGT", "GB200", "NEO", "ISSI7" };

const std::vector<std::string> VALUES_SPORTSCLASSICS{ "BTYPE2", "MANANA", "BTYPE", "BTYPE3", "SAVESTRA", "FELTZER3", "MAMBA", "TORNADO", "TORNADO5", "TORNADO3", "TORNADO2", "TORNADO4", "TORNADO6", "JB700", "JB7002", "RAPIDGT3", "JESTER3", "CHEETAH2", "GT500", "STINGER", "STINGERGT", "TURISMO2", "COQUETTE2", "DELUXO", "Z190", "CASCO", "MICHELLI", "PIGALLE", "VISERIS", "ARDENT", "STROMBERG", "SWINGER", "INFERNUS2", "MONROE", "TORERO", "CHEBUREK", "ZTYPE", "ZION3", "PEYOTE", "RETINUE", "RETINUE2", "FAGALOA", "NEBULA", "DYNASTY" };

const std::vector<std::string> VALUES_COUPES{ "EXEMPLAR", "COGCABRIO", "WINDSOR", "WINDSOR2", "FELON", "FELON2", "F620", "JACKAL", "ORACLE2", "ORACLE", "SENTINEL2", "SENTINEL", "ZION", "ZION2" };

const std::vector<std::string> VALUES_MUSCLE{ "BUCCANEER", "BUCCANEER2", "HERMES", "LURCHER", "VIRGO", "GAUNTLET", "GAUNTLET3", "GAUNTLET4", "GAUNTLET2", "RATLOADER2", "RATLOADER", "PICADOR", "IMPALER", "IMPALER2", "IMPALER3", "IMPALER4", "MOONBEAM", "MOONBEAM2", "SABREGT", "SABREGT2", "STALION", "STALION2", "TAMPA", "TAMPA3", "TULIP", "VAMOS", "VIGERO", "VOODOO2", "VOODOO", "YOSEMITE", "YOSEMITE2", "VIRGO3", "VIRGO2", "DUKES2", "DUKES", "NIGHTSHADE", "PHOENIX", "RUINER", "RUINER2", "COQUETTE3", "DEVIANT", "BLADE", "CHINO", "CHINO2", "CLIQUE", "DOMINATOR", "DOMINATOR3", "DOMINATOR2", "DOMINATOR4", "DOMINATOR5", "DOMINATOR6", "ELLIE", "HOTKNIFE", "HUSTLER", "IMPERATOR", "IMPERATOR2", "IMPERATOR3", "PEYOTE2", "SLAMVAN", "SLAMVAN3", "SLAMVAN2", "SLAMVAN4", "SLAMVAN5", "SLAMVAN6", "FACTION", "FACTION2", "FACTION3" };

const std::vector<std::string> VALUES_OFFROAD{ "HELLION", "BRUISER", "BRUISER2", "BRUISER3", "DUBSTA3", "BIFTA", "DUNE", "DUNE3", "BFINJECTION", "DUNE2", "DLOADER", "MONSTER3", "MONSTER4", "MONSTER5", "BODHI2", "FREECRAWLER", "KALAHARI", "KAMACHO", "MESA3", "MARSHALL", "BRAWLER", "BRUTUS", "BRUTUS2", "BRUTUS3", "RANCHERXL", "RANCHERXL2", "INSURGENT2", "INSURGENT", "INSURGENT3", "MENACER", "NIGHTSHARK", "EVERON", "REBEL2", "REBEL", "TECHNICAL", "TECHNICAL2", "TECHNICAL3", "VAGRANT", "BLAZER", "BLAZER3", "BLAZER2", "BLAZER5", "OUTLAW", "BLAZER4", "DUNE4", "DUNE5", "RCBANDITO", "ZHABA", "CARACARA", "CARACARA2", "TROPHYTRUCK2", "RIATA", "SANDKING2", "SANDKING", "TROPHYTRUCK", "MONSTER" };

const std::vector<std::string> VALUES_SUVS{ "CAVALCADE", "CAVALCADE2", "DUBSTA", "DUBSTA2", "SERRANO", "XLS", "XLS2", "GRESLEY", "MESA", "MESA2", "SEMINOLE", "GRANGER", "LANDSTALKER", "HABANERO", "HUNTLEY", "FQ2", "BALLER", "BALLER2", "BALLER3", "BALLER4", "BALLER5", "BALLER6", "BJXL", "NOVAK", "PATRIOT", "PATRIOT2", "ROCOTO", "TOROS", "REBLA", "CONTENDER", "RADI" };

const std::vector<std::string> VALUES_SEDANS{ "EMPEROR", "EMPEROR2", "EMPEROR3", "PRIMO", "PRIMO2", "WASHINGTON", "GLENDALE", "SCHAFTER2", "SCHAFTER6", "SCHAFTER5", "LIMO2", "ROMERO", "FUGITIVE", "SURGE", "ASEA", "ASEA2", "PREMIER", "REGINA", "STRETCH", "COGNOSCENTI", "COGNOSCENTI2", "COG55", "COG552", "STAFFORD", "SUPERD", "ASTEROPE", "INTRUDER", "TAILGATER", "STANIER", "INGOT", "WARRENER", "STRATUM" };

const std::vector<std::string> VALUES_COMPACTS{ "PANTO", "PRAIRIE", "RHAPSODY", "BLISTA", "KANJO", "BRIOSO", "DILETTANTE", "DILETTANTE2", "ASBO", "ISSI2", "ISSI3", "ISSI4", "ISSI5", "ISSI6" };

const std::vector<std::string> VOV_CAR_CAPTIONS[] = { CAPTIONS_SUPERCARS, CAPTIONS_SPORTS, CAPTIONS_SPORTSCLASSICS, CAPTIONS_COUPES, CAPTIONS_MUSCLE, CAPTIONS_OFFROAD, CAPTIONS_SUVS, CAPTIONS_SEDANS, CAPTIONS_COMPACTS/*, CAPTIONS_LOWRIDERS*/ };

const std::vector<std::string> VOV_CAR_VALUES[] = { VALUES_SUPERCARS, VALUES_SPORTS, VALUES_SPORTSCLASSICS, VALUES_COUPES, VALUES_MUSCLE, VALUES_OFFROAD, VALUES_SUVS, VALUES_SEDANS, VALUES_COMPACTS/*, VALUES_LOWRIDERS*/ };

//Industrial

const std::vector<std::string> MENU_INDUS_CATEGORIES{ "Industrial", "Commercial", "Vans", "Service/Utility", "Trailers", "Trains (Undriveable)" };

const std::vector<std::string> CAPTIONS_INDUSTRIAL{ "Brute Tipper (2 Axle)", "Brute Tipper (3 Axle)", "Dock Handler", "HVY Cutter", "HVY Dozer", "HVY Dump", "HVY Mixer", "HVY Mixer (Support Wheel)", "JoBuilt Rubble", "MTL Flatbed", "Vapid Guardian" };

const std::vector<std::string> CAPTIONS_COMMERCIAL{ "Benefactor Terrorbyte (After Hours)", "Brute Stockade", "Brute Stockade (Snow)", "HVY Biff", "JoBuilt Hauler", "JoBuilt Hauler Custom (Gun Runner)", "JoBuilt Phantom", "JoBuilt Phantom Custom (Gun Runner)", "JoBuilt Phantom Wedge (Import-Export)", "Maibatsu Mule (Graphics 1)", "Maibatsu Mule (Graphics 2)", "Maibatsu Mule (Plain)", "Maibatsu Mule Custom (After Hours)", "MTL Cerberus Apocalypse (Arena War)", "MTL Cerberus Future Shock (Arena War)", "MTL Cerberus Nightmare (Arena War)", "MTL Packer", "MTL Pounder", "MTL Pounder Custom (After Hours)", "Vapid Benson" };

const std::vector<std::string> CAPTIONS_VANS{ "BF Surfer", "BF Surfer (Rusty)", "Bravado Bison", "Bravado Bison (Backrack)", "Bravado Bison (Construction)", "Bravado Paradise", "Bravado Rumpo (Deludamol)", "Bravado Rumpo (Plain/Weazel)", "Bravado Rumpo Custom (Executive)", "Bravado Youga", "Bravado Youga Classic (Bikers)", "Brute Boxville (Go Postal)", "Brute Boxville (Humane Labs)", "Brute Boxville Armored (Import-Export)", "Brute Boxville (Post OP)", "Brute Boxville (Water & Power)", "Brute Camper", "Brute Pony (Business)", "Brute Pony (Cannibus Shop)", "Brute Taco Van", "Declasse Burrito", "Declasse Burrito (Bug Stars)", "Declasse Burrito (Construction)", "Declasse Burrito (Gang)", "Declasse Burrito (Graphics)", "Declasse Burrito (Snow)", "Declasse Burrito Gang (The Lost)", "Vapid Bobcat XL", "Vapid Minivan", "Vapid Minivan (Custom)", "Vapid Speedo", "Vapid Speedo Clown Van", "Vapid Speedo Custom (After Hours)", "Zirconium Journey" };

const std::vector<std::string> CAPTIONS_SERVICES{ "Airtug", "Brute Airport Bus", "Brute Bus (City Bus)", "Brute Rental Shuttle Bus", "Brute Tourbus", "Cable Car (Mt. Chilliad)", "Dashound Coach", "Docktug", "HVY Forklift", "Jack Sheepe Lawn Mower", "JoBuilt Trashmaster", "JoBuilt Trashmaster (Rusty)", "MTL Brickade (Executive)", "MTL Dune (Race)", "MTL Wastelander (Import-Export)", "Nagasaki Caddy (New)", "Nagasaki Caddy (Gun Runner)", "Nagasaki Caddy (Old)", "Ripley (Airport Tug)", "Stanley Fieldmaster Tractor", "Stanley Fieldmaster Tractor (Snow)", "Stanley Tractor (Rusty)", "Taxi", "Vapid Festival Bus (After Hours)", "Vapid Pickup Utility", "Vapid Plumbing Utility Truck", "Vapid Sadler", "Vapid Sadler (Snow)", "Vapid Scrap Truck", "Vapid Telephone Utility Truck", "Vapid Tow Truck", "Vapid Tow Truck (Old)" };

const std::vector<std::string> CAPTIONS_TRAILERS{ "Army Flatbed Trailer (Empty)", "Army Flatbed Trailer (With Drill)", "Army Fuel Tanker", "Boat Trailer", "Boat Trailer (With Boat)", "Car Transport Trailer", "Car Transport Trailer (Empty)", "Commercial Trailer (Graphics 1)", "Commercial Trailer (Graphics 2)", "Container Trailer", "Fame or Shame Trailer", "Flatbed Trailer 1", "Flatbed Trailer 2", "Gun Runner Trailer (Large)", "Gun Runner Trailer (Small)", "Grain Trailer", "Hay Bale Trailer", "Logging Trailer", "Meth Lab Trailer", "Petrol Tanker Trailer (Plain)", "Petrol Tanker Trailer (RON)", "Plain Trailer", "Rake Trailer", "Small Trailer" };

const std::vector<std::string> CAPTIONS_TRAINS{ "Container Car 1", "Container Car 2", "Flatbed Car", "Freight Train Cab", "Grain Car", "Metro Train (Half)", "Oil Tanker Car" };

const std::vector<std::string> VALUES_INDUSTRIAL{ "TIPTRUCK", "TIPTRUCK2", "HANDLER", "CUTTER", "BULLDOZER", "DUMP", "MIXER", "MIXER2", "RUBBLE", "FLATBED", "GUARDIAN" };

const std::vector<std::string> VALUES_COMMERCIAL{ "TERBYTE", "STOCKADE", "STOCKADE3", "BIFF", "HAULER", "HAULER2", "PHANTOM", "PHANTOM3", "PHANTOM2", "MULE", "MULE2", "MULE3", "MULE4", "CERBERUS", "CERBERUS2","CERBERUS3", "PACKER", "POUNDER", "POUNDER2", "BENSON" };

const std::vector<std::string> VALUES_VANS{ "SURFER", "SURFER2", "BISON", "BISON3", "BISON2", "PARADISE", "RUMPO2", "RUMPO", "RUMPO3", "YOUGA", "YOUGA2", "BOXVILLE2", "BOXVILLE3", "BOXVILLE5", "BOXVILLE4", "BOXVILLE", "CAMPER", "PONY", "PONY2", "TACO", "BURRITO3", "BURRITO2", "BURRITO4", "GBURRITO2", "BURRITO", "BURRITO5", "GBURRITO", "BOBCATXL", "MINIVAN", "MINIVAN2", "SPEEDO", "SPEEDO2", "SPEEDO4", "JOURNEY" };

const std::vector<std::string> VALUES_SERVICES{ "AIRTUG", "AIRBUS", "BUS", "RENTALBUS", "TOURBUS", "CABLECAR", "COACH", "DOCKTUG", "FORKLIFT", "MOWER", "TRASH", "TRASH2", "BRICKADE", "RALLYTRUCK", "WASTELANDER", "CADDY", "CADDY3", "CADDY2", "RIPLEY", "TRACTOR2", "TRACTOR3", "TRACTOR", "TAXI", "PBUS2", "UTILLITRUCK3", "UTILLITRUCK2", "SADLER", "SADLER2", "SCRAP", "UTILLITRUCK", "TOWTRUCK", "TOWTRUCK2" };

const std::vector<std::string> VALUES_TRAILERS{ "ARMYTRAILER", "ARMYTRAILER2", "ARMYTANKER", "BOATTRAILER", "TR3", "TR4", "TR2", "TRAILERS2", "TRAILERS3", "DOCKTRAILER", "TVTRAILER", "FREIGHTTRAILER", "TRFLAT", "TRAILERLARGE" /*Could also be "TRAILERS4"*/, "TRAILERSMALL2", "GRAINTRAILER", "BALETRAILER", "TRAILERLOGS", "PROPTRAILER", "TANKER2", "TANKER", "TRAILERS", "RAKETRAILER", "TRAILERSMALL" };

const std::vector<std::string> VALUES_TRAINS{ "FREIGHTCONT1", "FREIGHTCONT2", "FREIGHTCAR", "FREIGHT", "FREIGHTGRAIN", "METROTRAIN", "TANKERCAR" };

const std::vector<std::string> VOV_INDUS_CAPTIONS[] = { CAPTIONS_INDUSTRIAL, CAPTIONS_COMMERCIAL, CAPTIONS_VANS, CAPTIONS_SERVICES, CAPTIONS_TRAILERS, CAPTIONS_TRAINS };

const std::vector<std::string> VOV_INDUS_VALUES[] = { VALUES_INDUSTRIAL, VALUES_COMMERCIAL, VALUES_VANS, VALUES_SERVICES, VALUES_TRAILERS, VALUES_TRAINS };

//Everything else

const std::vector<std::string> CAPTIONS_EMERGENCY{ "Albany Police Roadcruiser (Snow)", "Ambulance", "Barracks", "Barrage (Doomsday Heist)", "Bravado Buffalo (FIB)", "Bravado Half-Track (Gun Runner)", "Brute Police Riot Van", "Canis Crusader (Army Mesa)", "Chernobog (Doomsday Heist)", "Declasse Granger (FIB)", "Declasse Lifeguard", "Declasse Park Ranger", "Declasse Police Rancher (Snow)", "Declasse Police Transporter", "Declasse Sheriff SUV", "Firetruck", "HVY APC (Gun Runner)", "HVY Barracks Semi", "HVY Scarab Apocalypse (Arena War)", "HVY Scarab Future Shock (Arena War)", "HVY Scarab Nightmare (Arena War)", "Invade And Persuade Tank (Casino Heist)", "Mammoth Thruster (Doomsday Heist)", "Prison Bus", "RCV (Doomsday Heist)", "Rhino Tank", "TM-02 Khanjli (Doomsday Heist)", "Vapid Police Buffalo", "Vapid Police Cruiser", "Vapid Police Interceptor", "Vapid Sheriff Cruiser", "Vapid Unmarked Police Cruiser", "Western Police Bike" };

const std::vector<std::string> CAPTIONS_MOTORCYCLES{ "Dinka Akuma", "Dinka Double-T", "Dinka Enduro", "Dinka Thrust", "Dinka Vindicator", "LCC Avarus (Bikers)", "LCC Hexer", "LCC Innovation", "LCC Sanctus (Bikers)", "Maibatsu Manchez (Bikers)", "Maibatsu Sanchez", "Maibatsu Sanchez (Livery)", "Nagasaki BF400 (Race)", "Nagasaki Carbon RS", "Nagasaki Chimera Trike (Bikers)", "Nagasaki Shotaro (Bikers)", "Nagasaki Stryder (Casino Heist)", "Pegassi Bati 801", "Pegassi Bati 801RR (Race)", "Pegassi Esskey (Bikers)", "Pegassi Faggio", "Pegassi Faggio Mod (Bikers)", "Pegassi Faggio Sport (Bikers)", "Pegassi FCR 1000 (Import-Export)", "Pegassi FCR 1000 Custom (Import-Export)", "Pegassi Oppressor (Gun Runner)", "Pegassi Oppressor MK II (After Hours)", "Pegassi Ruffian", "Pegassi Vortex (Bikers)", "Pincipe Diabolus (Import-Export)", "Pincipe Diabolus Custom (Import - Export)", "Principe Lectro", "Principe Nemesis", "Shitzu Defiler (Bikers)", "Shitzu Hakuchou", "Shitzu Hakuchou Drag (Bikers)", "Shitzu PCJ 600", "Shitzu Vader", "Western Bagger", /*"Western Bagger (Bikers)",*/ "Western Cliffhanger (Race)", "Western Daemon", "Western Daemon (Bikers)", "Western Deathbike Apocalypse (Arena War)", "Western Deathbike Future Shock (Arena War)", "Western Deathbike Nightmare (Arena War)", "Western Gargoyle (Race)", "Western Nightblade (Bikers)", "Western Rat Bike (Bikers)", "Western Rampant Rocket (Diamond Casino)", "Western Sovereign", "Western Wolfsbane (Bikers)", "Western Zombie Bobber (Bikers)", "Western Zombie Chopper (Bikers)" };

const std::vector<std::string> CAPTIONS_PLANES{ "Atomic Blimp", "Atomic Blimp - Club Advert (After Hours)", "B-11 Strikeforce (After Hours)", "Buckingham Alpha-Z1 (Smugglers Run)", "Buckingham Cargo Plane (An-225)", "Buckingham Howard NX-25 (Smuggler's Run)", "Buckingham Jet (B747)", "Buckingham Luxor", "Buckingham Luxor Deluxe", "Buckingham Miljet", "Buckingham Nimbus (Executive)", "Buckingham Pyro (Smuggler's Run)", "Buckingham Shamal", "Buckingham Vestra", "JoBuilt Mammatus", "JoBuilt P-996 Lazer", "JoBuilt Velum (4 Seater)", "JoBuilt Velum (5 Seater)", "LF-22 Starling (Smuggler's Run)", "Mammoth Avenger (Doomsday Heist)", "Mammoth Dodo", "Mammoth Hydra", "Mammoth Mogul (Smuggler's Run)", "Mammoth Titan", "Mammoth Tula (Smuggler's Run)", "P-45 Nokota (Smuggler's Run)", "RM-10 Bombushka (Smuggler's Run)", "V-65 Molotok (Smuggler's Run)", "Volatol (Doomsday Heist)", "Western Besra", "Western Company Rogue (Smuggler's Run)", "Western Company Seabreeze (Smuggler's run)", "Western Cuban 800", "Western Duster", "Western Mallard Stunt Plane", "Xero Blimp" };

const std::vector<std::string> CAPTIONS_HELOS{ "Akula (Doomsday Heist)", "Buckingham Savage", "Buckingham SuperVolito", "Buckingham SuperVolito Carbon", "Buckingham Swift", "Buckingham Swift Deluxe", "Buckingham Valkyrie", "Buckingham Volatus (Executive)", "FH-1 Hunter (Smuggler's Run)", "HVY Skylift", "Maibatsu Frogger", "Maibatsu Frogger (TPE/FIB)", "Nagasaki Buzzard (Unarmed)", "Nagasaki Buzzard Attack Chopper", "Nagasaki Havok (Smuggler's Run)", "Nagasaki Ultralight (Smuggler's Run)", "Sea Sparrow (Super Sports)", "Western Annihilator", "Western Cargobob (Desert Camo)", "Western Cargobob (Jetsam)", "Western Cargobob (TPE)", "Western Maverick", "Western Maverick (Police/Medical)" };

const std::vector<std::string> CAPTIONS_BOATS{ "Buckingham Tug (Executive)", "Dinka Marquis", "Kraken Sub", "Lampadati Toro", "Nagasaki Dinghy (2 Seater)", "Nagasaki Dinghy (4 Seater, Black)", "Nagasaki Dinghy (4 Seater, Red)", "Pegassi Speeder", "Shitzu Jetmax", "Shitzu Predator (Police)", "Shitzu Squalo", "Shitzu Suntrap", "Shitzu Tropic", "Speedophile Seashark", "Speedophile Seashark (Lifeguard)", "Submersible" };

const std::vector<std::string> CAPTIONS_BICYCLES{ "BMX", "Cruiser", "Endurex Race", "Fixter", "Scorcher", "Tri-Cycles Race", "Whippet Race" };

const std::vector<std::string> VALUES_EMERGENCY{ "POLICEOLD2", "AMBULANCE", "BARRACKS", "BARRAGE", "FBI", "HALFTRACK", "RIOT", "CRUSADER", "CHERNOBOG", "FBI2", "LGUARD", "PRANGER", "POLICEOLD1", "POLICET", "SHERIFF2", "FIRETRUK", "APC", "BARRACKS2", "SCARAB", "SCARAB2", "SCARAB3", "MINITANK", "THRUSTER", "PBUS", "RIOT2", "RHINO", "KHANJALI", "POLICE2", "POLICE", "POLICE3", "SHERIFF", "POLICE4", "POLICEB" };

const std::vector<std::string> VALUES_MOTORCYCLES{ "AKUMA", "DOUBLE", "ENDURO", "THRUST", "VINDICATOR", "AVARUS", "HEXER", "INNOVATION", "SANCTUS", "MANCHEZ", "SANCHEZ2", "SANCHEZ", "BF400", "CARBONRS", "CHIMERA", "SHOTARO", "STRYDER", "BATI", "BATI2", "ESSKEY", "FAGGIO2", "FAGGIO3", "FAGGIO", "FCR", "FCR2", "OPPRESSOR", "OPPRESSOR2", "RUFFIAN", "VORTEX", "DIABLOUS", "DIABLOUS2", "LECTRO", "NEMESIS", "DEFILER", "HAKUCHOU", "HAKUCHOU2", "PCJ", "VADER", "BAGGER", /*"BAGGER2",*/ "CLIFFHANGER", "DAEMON", "DAEMON2", "DEATHBIKE", "DEATHBIKE2", "DEATHBIKE3", "GARGOYLE", "NIGHTBLADE", "RATBIKE", "RROCKET", "SOVEREIGN", "WOLFSBANE", "ZOMBIEA", "ZOMBIEB" };

const std::vector<std::string> VALUES_PLANES{ "BLIMP", "BLIMP3", "STRIKEFORCE", "ALPHAZ1", "CARGOPLANE", "HOWARD", "JET", "LUXOR", "LUXOR2", "MILJET", "NIMBUS", "PYRO", "SHAMAL", "VESTRA", "MAMMATUS", "LAZER", "VELUM", "VELUM2", "STARLING", "AVENGER", "DODO", "HYDRA", "MOGUL", "TITAN", "TULA", "NOKOTA", "BOMBUSHKA", "MOLOTOK", "VOLATOL", "BESRA", "ROGUE", "SEABREEZE", "CUBAN800", "DUSTER", "STUNT", "BLIMP2" };

const std::vector<std::string> VALUES_HELOS{ "AKULA", "SAVAGE", "SUPERVOLITO", "SUPERVOLITO2", "SWIFT", "SWIFT2", "VALKYRIE", "VOLATUS", "HUNTER", "SKYLIFT", "FROGGER", "FROGGER2", "BUZZARD2", "BUZZARD", "HAVOK", "MICROLIGHT", "SEASPARROW", "ANNIHILATOR", "CARGOBOB", "CARGOBOB2", "CARGOBOB3", "MAVERICK", "POLMAV" };

const std::vector<std::string> VALUES_BOATS{ "TUG", "MARQUIS", "SUBMERSIBLE2", "TORO", "DINGHY2", "DINGHY3", "DINGHY", "SPEEDER", "JETMAX", "PREDATOR", "SQUALO", "SUNTRAP", "TROPIC", "SEASHARK", "SEASHARK2", "SUBMERSIBLE" };

const std::vector<std::string> VALUES_BICYCLES{ "BMX", "CRUISER", "TRIBIKE2", "FIXTER", "SCORCHER", "TRIBIKE3", "TRIBIKE" };

const std::vector<std::string> VOV_SHALLOW_CAPTIONS[] = { CAPTIONS_EMERGENCY, CAPTIONS_MOTORCYCLES, CAPTIONS_PLANES, CAPTIONS_HELOS, CAPTIONS_BOATS, CAPTIONS_BICYCLES };

const std::vector<std::string> VOV_SHALLOW_VALUES[] = { VALUES_EMERGENCY, VALUES_MOTORCYCLES, VALUES_PLANES, VALUES_HELOS, VALUES_BOATS, VALUES_BICYCLES };

std::string lastCustomVehicleSpawn;

Vector3 RotationToDirection2(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)std::sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radiansZ)) * (double)num);
	dir.z = (float)std::sin((double)radiansX);
	return dir;
}

void process_window_roll() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_roll = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_roll = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_roll = temp_vehicle;
	}
	
	if (window_roll == false) {
		VEHICLE::ROLL_DOWN_WINDOW(veh_roll, 0);
	}
	if (window_roll == true) {
		VEHICLE::ROLL_UP_WINDOW(veh_roll, 0); 
	}

	window_roll = !window_roll;
}

void interior_light() { 
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_interior = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	interior_lights = !interior_lights;
	VEHICLE::SET_VEHICLE_INTERIORLIGHT(veh_interior, interior_lights);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		VEHICLE::SET_VEHICLE_INTERIORLIGHT(temp_vehicle, interior_lights);
	}
}

void search_light() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_search = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	veh_searching = !veh_searching;
	VEHICLE::SET_VEHICLE_SEARCHLIGHT(veh_search, veh_searching, veh_searching);
}

void vehicle_alarm() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_alarming = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_alarming = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_alarming = temp_vehicle;
	}
	
	if (!VEHICLE::IS_VEHICLE_ALARM_ACTIVATED(veh_alarming)) veh_alarm = false;
	veh_alarm = !veh_alarm;
	VEHICLE::SET_VEHICLE_ALARM(veh_alarming, veh_alarm);
	VEHICLE::START_VEHICLE_ALARM(veh_alarming);
	WAIT(100);
}

void vehicle_set_alarm() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	if (!featureAutoalarm) {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) alarmed_veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			find_nearest_vehicle();
			alarmed_veh = temp_vehicle;
		}
	}
	if (!featureAutoalarm) alarm_enabled = !alarm_enabled;
	alarmischarged = true;
}

void doorslocked_switching() {
	featureLockVehicleDoors = !featureLockVehicleDoors;
	if (featureLockVehicleDoors) set_status_text("Doors Locked");
	else set_status_text("Doors Unlocked");
	WAIT(100);
}

void vehicle_brake() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_brake = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_brake = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_brake = temp_vehicle;
	}

	veh_brake_toggle = !veh_brake_toggle;
	VEHICLE::SET_VEHICLE_HANDBRAKE(veh_brake, veh_brake_toggle);
	WAIT(100);
}

void vehicle_burnout() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_burnout = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_burnout = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_burnout = temp_vehicle;
	}

	vehicle_burnout_toggle = !vehicle_burnout_toggle;
	VEHICLE::SET_VEHICLE_BURNOUT(veh_burnout, vehicle_burnout_toggle);

	WAIT(100);
}

void damage_door() {
	Player playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle veh_damage = -1;
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_damage = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_damage = temp_vehicle;
	}

	std::string::size_type sz;
	std::string result_damage = show_keyboard(NULL, NULL);
	if (!result_damage.empty()) {
		int dec_result = std::stoi(result_damage, &sz);
		VEHICLE::SET_VEHICLE_DOOR_BROKEN(veh_damage, dec_result, false);
	}
}

void toggle_tractioncontrol() {
	featureTractionControl = !featureTractionControl;
	if (featureTractionControl) {
		set_status_text("Traction Control Enabled");
	}
	else {
		set_status_text("Traction Control Disabled");
	}
}

void enter_damaged_vehicle() {
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) find_nearest_vehicle();
	AI::TASK_ENTER_VEHICLE(PLAYER::PLAYER_PED_ID(), temp_vehicle, 1000, -1, 1.0, 1, 0);
}

void vehicle_anchor() {
	if (anchor_dropped == false) {
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_anchor = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			find_nearest_vehicle();
			veh_anchor = temp_vehicle;
		}
		if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(veh_anchor)) || ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE") || ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE2") ||
			ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("DODO")) {
			coords_b = ENTITY::GET_ENTITY_COORDS(veh_anchor, true);
			b_rope = ROPE::ADD_ROPE(coords_b.x, coords_b.y, coords_b.z, 0.0, 0.0, 0.0, 20.0, 4, 20.0, 1.0, 0.0, false, false, false, 5.0, false, NULL);
			ROPE::START_ROPE_WINDING(b_rope);
			ROPE::ATTACH_ROPE_TO_ENTITY(b_rope, veh_anchor, coords_b.x, coords_b.y, coords_b.z, 1);
		}
	}
	if (anchor_dropped == true) ROPE::DELETE_ROPE(&b_rope);
	if (VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(veh_anchor)) || ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE") || ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE2") ||
		ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("DODO")) {
		anchor_dropped = !anchor_dropped;
		if (anchor_dropped) set_status_text("Anchor dropped");
		else set_status_text("Anchor raised");
	}
	WAIT(100);
}

Vehicle find_nearest_vehicle() {
	Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	const int arrSize33 = 1024;
	Ped surr_vehs[arrSize33];
	int count_surr_vehs = worldGetAllVehicles(surr_vehs, arrSize33);
	float dist_diff = -1.0;
	float temp_dist = 2000.0;
	for (int i = 0; i < count_surr_vehs; i++) {
		Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_vehs[i], true);
		dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
		if (temp_dist > dist_diff) {
			temp_dist = dist_diff;
			temp_vehicle = surr_vehs[i];
		}
	}
	return temp_vehicle;
}

Ped find_nearest_ped() {
	Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	const int arrSize33 = 1024;
	Ped surr_peds[arrSize33];
	int count_surr_vehs = worldGetAllPeds(surr_peds, arrSize33);
	float dist_diff = -1.0;
	float temp_dist = 2000.0;
	for (int i = 0; i < count_surr_vehs; i++) {
		if (surr_peds[i] != PLAYER::PLAYER_PED_ID()) { 
			Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_peds[i], true);
			dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
			if (temp_dist > dist_diff) {
				temp_dist = dist_diff;
				temp_ped = surr_peds[i];
			}
		}
	}
	return temp_ped;
}

void eject_seat() { // eject seat
	Vehicle veh_eject = -1;
	Ped PedToEject = -1;
	Hash currVehModel = -1;
	Vehicle veh = -1;
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		find_nearest_vehicle();
		veh_eject = temp_vehicle;
		PedToEject = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh_eject, -1);
	}
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
		veh_eject = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
		PedToEject = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh_eject, -1);
	}
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(veh_eject, true, true);
	Vector3 coordsmetoeject = ENTITY::GET_ENTITY_COORDS(PedToEject, true);
	Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 direction = RotationToDirection2(&Rot);
	direction.x = 1 * direction.x;
	direction.y = 1 * direction.y;
	direction.z = 1 * direction.z;
			   
	ENTITY::GET_ENTITY_HEADING(PedToEject);
	currVehModel = ENTITY::GET_ENTITY_MODEL(veh_eject);
	Vector3 coords_veh2 = ENTITY::GET_ENTITY_COORDS(veh_eject, true);
	float rot = (ENTITY::GET_ENTITY_ROTATION(veh_eject, 0)).z;
	Vector3 vehspeed = ENTITY::GET_ENTITY_VELOCITY(veh_eject);
		
	if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) FIRE::ADD_EXPLOSION(coordsmetoeject.x, coordsmetoeject.y, coordsmetoeject.z, 28, 0, 1, 1, 100);
	else FIRE::ADD_EXPLOSION(coordsmetoeject.x, coordsmetoeject.y, coordsmetoeject.z, 28, 0, 1, 1, 10);
	AI::TASK_LEAVE_VEHICLE(PedToEject, veh_eject, 16);
	veh = VEHICLE::CREATE_VEHICLE(currVehModel, coords_veh2.x, coords_veh2.y, coords_veh2.z + 10, rot, 1, 0);
	ENTITY::SET_ENTITY_VELOCITY(veh, vehspeed.x, vehspeed.y, vehspeed.z);
	ENTITY::SET_ENTITY_VISIBLE(veh, false);
	ENTITY::SET_ENTITY_COLLISION(veh, false, false);
	if (ENTITY::DOES_ENTITY_EXIST(veh)) PED::SET_PED_INTO_VEHICLE(PedToEject, veh, -1);
	AUDIO::SET_VEHICLE_RADIO_ENABLED(veh, false);

	int PedEjectHash = GAMEPLAY::GET_HASH_KEY("prop_ejector_seat_01"); // prop_car_seat // prop_ejector_seat_01 
	Object seat_obj = OBJECT::CREATE_OBJECT(PedEjectHash, coordsmetoeject.x, coordsmetoeject.y, coordsmetoeject.z, 1, true, 1);
	int PlayerEjectIndex = PED::GET_PED_BONE_INDEX(PedToEject, 0x2e28);
	ENTITY::ATTACH_ENTITY_TO_ENTITY(seat_obj, PedToEject, PlayerEjectIndex, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, true, 0, true);

	ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, direction.x, direction.y, 90.275, Rot.x, Rot.y, Rot.z, false, true, true, true, false, true);
	WAIT(1500);
	OBJECT::SET_OBJECT_PHYSICS_PARAMS(seat_obj, 100.0f, 100.2f, -1.0f, -1.0f, -1.0f, 100.0f, -1.0f, -1.0f, -1.0f, -1.0f, 2.0f); 
	ENTITY::DETACH_ENTITY(seat_obj, true, true);

	WAIT(5000); // 4000
	VEHICLE::DELETE_VEHICLE(&veh);
}

bool onconfirm_vehdoor_menu(MenuItem<int> choice){

	if(choice.value == -1) {
		return false;
	}

	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(choice.value >= 0){
		if(bPlayerExists){ 
			Vehicle veh = -1;
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
				find_nearest_vehicle();
				veh = temp_vehicle;
			}

			int value = choice.value;

			float doorAngle = VEHICLE::GET_VEHICLE_DOOR_ANGLE_RATIO(veh, value); //Best way I could figure out to detect if the part is animated.
			if(doorAngle < 0.01){
				VEHICLE::SET_VEHICLE_DOOR_OPEN(veh, value, false, featureVehicleDoorInstant);
			}
			else{
				VEHICLE::SET_VEHICLE_DOOR_SHUT(veh, value, featureVehicleDoorInstant);
			}
		}
		else{
			set_status_text("Player isn't in a vehicle");
		}
	}
	/*else if(choice.value == -2)//bomb bay open
	{
		if(bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::OPEN_BOMB_BAY_DOORS(veh);
		}
	}
	else if(choice.value == -3)//bomb bay close
	{
		if(bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
			VEHICLE::CLOSE_BOMB_BAY_DOORS(veh);
		}
	}*/
	else if (choice.value == -5)//driver window roll
	{
		process_window_roll(); 
	}
	else if (choice.value == -6)//all windows down
	{
		Vehicle veh_roll = -1;
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_roll = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			find_nearest_vehicle();
			veh_roll = temp_vehicle;
		}

		VEHICLE::ROLL_DOWN_WINDOWS(veh_roll);
	}
	else if (choice.value == -7)//interior light on/off
	{
		interior_light();
	}
	else if (choice.value == -8)//search light on/off
	{
		search_light();
	}
	else if (choice.value == -9)//engine on/off 
	{
		engineonoff_switching(); 
	}
	else if (choice.value == -10)//damage the engine
	{
		engine_damage(); 
	}
	else if (choice.value == -11)//kill the engine
	{
		engine_kill(); 
	}
	else if (choice.value == -12)//vehicle alarm
	{
		vehicle_alarm();
	}
	else if (choice.value == -13)//vehicle set alarm
	{
		vehicle_set_alarm();
	}
	else if (choice.value == -14)//vehicle brake
	{
		vehicle_brake();
	}
	else if (choice.value == -15)//vehicle burnout
	{
		vehicle_burnout();
	}
	else if (choice.value == -16)//damage door 
	{
		damage_door();
	}
	else if (choice.value == -17)//eject seat
	{
		eject_seat();
	}
	else if (choice.value == -18)//detach windscreen
	{
		Vehicle veh_detach = -1;
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) veh_detach = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 1)) {
			find_nearest_vehicle();
			veh_detach = temp_vehicle;
		}
		VEHICLE::_DETACH_VEHICLE_WINDSCREEN(veh_detach);
	}
	else if (choice.value == -19)//enter damaged vehicle
	{
	enter_damaged_vehicle();
	}
	else if (choice.value == -20)//drop anchor
	{
	vehicle_anchor();
	}
	return false;
}

bool process_veh_door_menu(){
	std::string caption = "Vehicle Options";

	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

	std::vector<MenuItem<int>*> menuItems;
	std::vector<int>menuIndexes;

	std::vector<std::string> DOOR_NAMES = {
		"Front Left Door",
		"Front Right Door",
		"Rear Left Door",
		"Rear Right Door",
		"Hood",
		"Trunk ",
		"Trunk 2"
	};

	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Toggle Open Door Instantly";
	toggleItem->toggleValue = &featureVehicleDoorInstant;
	menuItems.push_back(toggleItem);

	for(int i = 0; i < DOOR_NAMES.size(); i++){

		MenuItem<int> *item = new MenuItem<int>();
		item->value = i;
		item->caption = DOOR_NAMES[i];
		menuItems.push_back(item);
	}

	if(VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, false)){
		FunctionDrivenToggleMenuItem<int>* toggleItem = new FunctionDrivenToggleMenuItem<int>();
		toggleItem->caption = "Convertible Roof Down?";
		toggleItem->getter_call = is_convertible_roofdown;
		toggleItem->setter_call = set_convertible_roofdown;
		toggleItem->value = -1;
		menuItems.push_back(toggleItem);
	}

	/*Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh);
	if(GAMEPLAY::GET_HASH_KEY("CUBAN800") == currVehModel){
		MenuItem<int>* bombBayItem1 = new MenuItem<int>();
		bombBayItem1->caption = "Open Bomb Bay";
		bombBayItem1->value = -2;
		menuItems.push_back(bombBayItem1);

		MenuItem<int>* bombBayItem2 = new MenuItem<int>();
		bombBayItem2->caption = "Close Bomb Bay";
		bombBayItem2->value = -3;
		menuItems.push_back(bombBayItem2);
	}*/

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Lock Vehicle Doors";
	toggleItem->value = -4;
	toggleItem->toggleValue = &featureLockVehicleDoors;
	menuItems.push_back(toggleItem);

	std::vector<MenuItem<int>*> menuItemsRoll;

	MenuItem<int> *item;
	int i = 0;

	item = new MenuItem<int>();
	item->caption = "Driver Window Roll Up/Down";
	item->value = -5;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "All Windows Down";
	item->value = -6;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Interior Light On/Off";
	item->value = -7;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Search Light On/Off";
	item->value = -8;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Engine Start/Stop";
	item->value = -9;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Damage The Engine";
	item->value = -10;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Kill The Engine";
	item->value = -11;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Set Alarm On/Off";
	item->value = -12;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Toggle Vehicle Alarm";
	item->value = -13;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Handbrake On/Off";
	item->value = -14;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Burnout On/Off";
	item->value = -15;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Damage Door (0-5)";
	item->value = -16;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Eject Driver Seat";
	item->value = -17;
	item->isLeaf = true;
	menuItems.push_back(item); 

	item = new MenuItem<int>();
	item->caption = "Detach Windscreen";
	item->value = -18;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Enter Damaged Vehicle";
	item->value = -19;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Drop Anchor";
	item->value = -20;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, &doorOptionsMenuIndex, caption, onconfirm_vehdoor_menu, NULL, NULL);
}

void seat_change_hotkey()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		Hash currHotkeyVehModel = ENTITY::GET_ENTITY_MODEL(veh);
		int seats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(currHotkeyVehModel);

		if (currseat == (seats - 2)) currseat = -2;
		currseat = currseat + 1;

		PED::SET_PED_INTO_VEHICLE(playerPed, veh, currseat);
	}
	else {
		set_status_text("Player isn't in a vehicle");
	}
}

bool onconfirm_seat_menu(MenuItem<int> choice) {
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
		
	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		int value = choice.value;

		PED::SET_PED_INTO_VEHICLE(playerPed, veh, value);
	}
	return false;
}

bool process_veh_seat_menu() 
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::vector<MenuItem<int>*> menuItems;
	
	if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(playerPed))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh);
		int maxSeats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(currVehModel);

		std::vector<std::string> SEAT_NAMES = {
			"Driver",
			"Front Passenger"			
		};

		for (int i = 0; i < maxSeats; i++) 
		{
			SEAT_NAMES.push_back("Rear Passenger " + std::to_string(i + 1));

			MenuItem<int> *item = new MenuItem<int>();
			item->value = i - 1;
			item->caption = SEAT_NAMES[i];
			menuItems.push_back(item);
		}
	}
	else 
	{
		set_status_text("Player not in vehicle");
	}

	return draw_generic_menu<int>(menuItems, &vehSeatIndexMenuIndex, "Seat Options", onconfirm_seat_menu, NULL, NULL);
}

void on_toggle_invincibility(MenuItem<int> choice){
	featureVehInvincibleUpdated = true;
}

bool onconfirm_colours_menu(MenuItem<int> choice)
{
	return false;
}

bool onconfirm_colours2_menu(MenuItem<int> choice)
{
	return false;
}

bool onconfirm_speed_menu(MenuItem<int> choice)
{
	return false;
}

void process_speed_menu(){
	std::string caption = "Speed And Altitude Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "KM/H";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureKMH;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Altitude";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureAltitude;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "On Foot";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureSpeedOnFoot;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Any Non Flying Vehicle";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureSpeedOnGround;
	menuItems.push_back(toggleItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Plane / Heli";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureSpeedInAir;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(SPEED_SIZE_CAPTIONS, onchange_speed_size_index);
	listItem->wrap = false;
	listItem->caption = "Size:";
	listItem->value = SpeedSizeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(SPEED_POSITION_CAPTIONS, onchange_speed_position_index);
	listItem->wrap = false;
	listItem->caption = "Position:";
	listItem->value = SpeedPositionIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexSpeed, caption, onconfirm_speed_menu, NULL, NULL);
}

bool onconfirm_visualize_menu(MenuItem<int> choice)
{
	return false;
}

void process_visualize_menu() {
	std::string caption = "Vehicle Indicators Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_CAPTIONS, onchange_veh_turn_signals_index);
	listItem->wrap = false;
	listItem->caption = "Enable Indicators";
	listItem->value = turnSignalsIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ANGLE_CAPTIONS, onchange_veh_turn_signals_angle_index);
	listItem->wrap = false;
	listItem->caption = "Turn Indicators On If Turn Angle Is";
	listItem->value = turnSignalsAngleIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS, onchange_veh_turn_signals_acceleration_index);
	listItem->wrap = false;
	listItem->caption = "Turn Indicators Off If Accelerated For (sec)";
	listItem->value = turnSignalsAccelerationIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable Hazard Lights On Damage";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureHazards;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_VISLIGHT_CAPTIONS, onchange_veh_vislight_index);
	listItem->wrap = false;
	listItem->caption = "2D Sprite";
	listItem->value = VisLightIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_VISLIGHT_CAPTIONS, onchange_veh_vislight3d_index);
	listItem->wrap = false;
	listItem->caption = "3D Vector";
	listItem->value = VisLight3dIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Third Person View Only";
	toggleItem->value = i++;
	toggleItem->toggleValue = &feature3rdpersonviewonly;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Daytime Only";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDaytimeonly;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexVisualize, caption, onconfirm_visualize_menu, NULL, NULL);
}

bool onconfirm_speedlimit_menu(MenuItem<int> choice)
{
	return false;
}

void process_speedlimit_menu() {
	std::string caption = "Speed Limit Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	int i = 0;

	listItem = new SelectFromListMenuItem(VEH_SPEEDLIMITER_CAPTIONS, onchange_veh_speedlimiter_index);
	listItem->wrap = false;
	listItem->caption = "Common Speed Limit";
	listItem->value = speedLimiterIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_SPEEDLIMITER_CAPTIONS, onchange_veh_cityspeedlimiter_index);
	listItem->wrap = false;
	listItem->caption = "City Auto Speed Limit";
	listItem->value = speedCityLimiterIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_SPEEDLIMITER_CAPTIONS, onchange_veh_countryspeedlimiter_index);
	listItem->wrap = false;
	listItem->caption = "Country Auto Speed Limit";
	listItem->value = speedCountryLimiterIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexSpeedlimit, caption, onconfirm_speedlimit_menu, NULL, NULL);
}

bool onconfirm_fuel_colour_menu(MenuItem<int> choice)
{
	return false;
}

bool process_fuel_colour_menu(){
	std::string caption = "RGB Settings";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;


	int i = 0;

	listItem = new SelectFromListMenuItem(FUEL_COLOURS_R_CAPTIONS, onchange_fuel_colours_r_index);
	listItem->wrap = false;
	listItem->caption = "R:";
	listItem->value = FuelColours_R_Index;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(FUEL_COLOURS_R_CAPTIONS, onchange_fuel_colours_g_index);
	listItem->wrap = false;
	listItem->caption = "G:";
	listItem->value = FuelColours_G_Index;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(FUEL_COLOURS_R_CAPTIONS, onchange_fuel_colours_b_index);
	listItem->wrap = false;
	listItem->caption = "B:";
	listItem->value = FuelColours_B_Index;
	menuItems.push_back(listItem);

	return draw_generic_menu<int>(menuItems, 0, "Fuel Bar Colour", onconfirm_fuel_colour_menu, NULL, NULL);
}

bool onconfirm_enginedegrade_menu(MenuItem<int> choice)
{
	return false;
}

void process_engine_degrade_menu() {
	std::string caption = "Engine Damage Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enable";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureEngineDegrade;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Health Bar";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureEngineHealthBar;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Limp Mode";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureLimpMode;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_car_enginehealth_index);
	listItem->wrap = false;
	listItem->caption = "Car Engine Health (Min %)";
	listItem->value = CarEngineHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_bike_enginehealth_index);
	listItem->wrap = false;
	listItem->caption = "Bike Engine Health (Min %)";
	listItem->value = BikeEngineHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_boat_enginehealth_index);
	listItem->wrap = false;
	listItem->caption = "Boat Engine Health (Min %)";
	listItem->value = BoatEngineHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_plane_enginehealth_index);
	listItem->wrap = false;
	listItem->caption = "Plane Engine Health (Min %)";
	listItem->value = PlaneEngineHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_heli_enginehealth_index);
	listItem->wrap = false;
	listItem->caption = "Heli Engine Health (Min %)";
	listItem->value = HeliEngineHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEDEGRADE_CAPTIONS, onchange_car_enginedegrade_index);
	listItem->wrap = false;
	listItem->caption = "Car Engine Damage Speed (% Per Mile)";
	listItem->value = CarEngineDegradeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEDEGRADE_CAPTIONS, onchange_bike_enginedegrade_index);
	listItem->wrap = false;
	listItem->caption = "Bike Engine Damage Speed (% Per Mile)";
	listItem->value = BikeEngineDegradeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEDEGRADE_CAPTIONS, onchange_boat_enginedegrade_index);
	listItem->wrap = false;
	listItem->caption = "Boat Engine Damage Speed (% Per Mile)";
	listItem->value = BoatEngineDegradeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEDEGRADE_CAPTIONS, onchange_plane_enginedegrade_index);
	listItem->wrap = false;
	listItem->caption = "Plane Engine Damage Speed (% Per Mile)";
	listItem->value = PlaneEngineDegradeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEDEGRADE_CAPTIONS, onchange_heli_enginedegrade_index);
	listItem->wrap = false;
	listItem->caption = "Heli Engine Damage Speed (% Per Mile)";
	listItem->value = HeliEngineDegradeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENGINEHEALTH_CAPTIONS, onchange_restoration_speed_index);
	listItem->wrap = false;
	listItem->caption = "Engine Recovery Speed (% Per Minute)";
	listItem->value = RestorationSpeedIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexEngineDegrade, caption, onconfirm_enginedegrade_menu, NULL, NULL);
}

bool onconfirm_fuel_menu(MenuItem<int> choice)
{
	switch (activeLineIndexFuel){
	case 15:
		if (process_fuel_colour_menu()) return false;
		break;
	}
	return false;
}

void DrawSprite(char * Streamedtexture, char * textureName, float x, float y, float width, float height, float rotation, int r, int g, int b, int a) // This is fo the 'Visualize Indicators' feature
{
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(Streamedtexture, false);
	GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(Streamedtexture);
	GRAPHICS::DRAW_SPRITE(Streamedtexture, textureName, x, y, width, height, rotation, r, g, b, a);
}

void process_fuel_menu(){
	std::string caption = "Fuel Consumption Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enabled";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureFuel;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_FUELBLIPS_CAPTIONS, onchange_fuel_blips_index);
	listItem->wrap = false;
	listItem->caption = "Blips";
	listItem->value = FuelBlipsIndex;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_idle_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Idle Consumption";
	listItem->value = IdleConsumptionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_car_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Car Fuel Consumption";
	listItem->value = CarConsumptionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_bike_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Bike Fuel Consumption";
	listItem->value = BikeConsumptionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_boat_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Boat Fuel Consumption";
	listItem->value = BoatConsumptionIndex;
	menuItems.push_back(listItem);
	
	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_plane_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Plane Fuel Consumption";
	listItem->value = PlaneConsumptionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_CARFUEL_CAPTIONS, onchange_heli_consumption_index);
	listItem->wrap = false;
	listItem->caption = "Heli Fuel Consumption";
	listItem->value = HeliConsumptionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_REFUELSPEED_CAPTIONS, onchange_refuelspeed_index);
	listItem->wrap = false;
	listItem->caption = "Refueling Speed";
	listItem->value = RefuelingSpeedIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FUELPRICE_CAPTIONS, onchange_fuelprice_index);
	listItem->wrap = false;
	listItem->caption = "Gas Station Fuel Price";
	listItem->value = FuelPriceIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FUELPRICE_CAPTIONS, onchange_canprice_index);
	listItem->wrap = false;
	listItem->caption = "Jerry Can Fuel Price";
	listItem->value = JerrycanPriceIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FUELRANDOM1_CAPTIONS, onchange_random1_index);
	listItem->wrap = false;
	listItem->caption = "Random Vehicle Fuel From (%)";
	listItem->value = Random1Index;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FUELRANDOM2_CAPTIONS, onchange_random2_index);
	listItem->wrap = false;
	listItem->caption = "Random Vehicle Fuel Up To (%)";
	listItem->value = Random2Index;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FUELBARPOSITION_CAPTIONS, onchange_barposition_index);
	listItem->wrap = false;
	listItem->caption = "Fuel Bar Position";
	listItem->value = BarPositionIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(FUEL_COLOURS_R_CAPTIONS, onchange_fuel_background_opacity_index);
	listItem->wrap = false;
	listItem->caption = "Fuel Bar Background Opacity";
	listItem->value = FuelBackground_Opacity_Index;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "Fuel Bar Colour";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, &activeLineIndexFuel, caption, onconfirm_fuel_menu, NULL, NULL);
}

std::string blipDelete;

void blip_delete_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "blipDelete", blipDelete });
}

void del_sel_blip() { 
	std::string result = show_keyboard(NULL, (char*)blipDelete.c_str());
	if (!result.empty()) {
		result = trim(result);
		blipDelete = result;
		Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());

		std::string a = (char*)result.c_str();
		std::string::size_type sz;

		int blip_delete = std::stof(a, &sz);

		if (blip_delete < VEHICLES_REMEMBER.size()) {
			if (UI::DOES_BLIP_EXIST(BLIPTABLE_VEH[blip_delete])) UI::REMOVE_BLIP(&BLIPTABLE_VEH[blip_delete]);
			VEHICLE::DELETE_VEHICLE(&VEHICLES_REMEMBER[blip_delete]);
			BLIPTABLE_VEH.erase(BLIPTABLE_VEH.begin() + blip_delete);
			VEHICLES_REMEMBER.erase(VEHICLES_REMEMBER.begin() + blip_delete);
			if (featureBlipNumber){
				for (int i = 0; i < BLIPTABLE_VEH.size(); i++) {
					UI::SHOW_NUMBER_ON_BLIP(BLIPTABLE_VEH[i], i);
				}
			}
		}
		else set_status_text("Not A Valid Number");
	}
}

bool onconfirm_vehicle_remember_menu(MenuItem<int> choice)
{
	switch (activeLineIndexRemember){
	case 9:
		del_sel_blip();
		break;
	default:
		break;
	}
	return false;
}

void process_remember_vehicles_menu() {
	std::string caption = "Vehicle Tracking Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enabled";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRememberVehicles;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Delete Tracked Vehicles On Disable";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDeleteTrackedVehicles;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Del Tracked Vehicles On Character Change";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDeleteTrackedVehicles_CharacterChanged; 
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_VEHREMEMBER_CAPTIONS, onchange_veh_remember_index);
	listItem->wrap = false;
	listItem->caption = "Number Of Vehicles To Track";
	listItem->value = VehRememberIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPSIZE_CAPTIONS, onchange_veh_blipsize_index);
	listItem->wrap = false;
	listItem->caption = "Blip Size";
	listItem->value = VehBlipSizeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPCOLOUR_CAPTIONS, onchange_veh_blipcolour_index);
	listItem->wrap = false;
	listItem->caption = "Blip Colour";
	listItem->value = VehBlipColourIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPSYMBOL_CAPTIONS, onchange_veh_blipsymbol_index);
	listItem->wrap = false;
	listItem->caption = "Blip Symbol";
	listItem->value = VehBlipSymbolIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_veh_blipflash_index);
	listItem->wrap = false;
	listItem->caption = "Blip Flashing";
	listItem->value = VehBlipFlashIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Blip Number";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureBlipNumber;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Delete Blip By Number";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Toggle Vehicle Alarm Automatically";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureAutoalarm;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexRemember, caption, onconfirm_vehicle_remember_menu, NULL, NULL);
}

bool onconfirm_road_laws_menu(MenuItem<int> choice)
{
	return false;
}

void process_road_laws_menu(){
	std::string caption = "Road Laws Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enabled";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRoadLaws;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_DETECTIONRANGE_CAPTIONS, onchange_detection_range_index);
	listItem->wrap = false;
	listItem->caption = "Detection Range";
	listItem->value = DetectionRangeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_PIRSUITRANGE_CAPTIONS, onchange_pirsuit_range_index);
	listItem->wrap = false;
	listItem->caption = "Pursuit Range";
	listItem->value = PirsuitRangeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_STARSPUNISH_CAPTIONS, onchange_stars_punish_index);
	listItem->wrap = false;
	listItem->caption = "Wanted Level For Evading Arrest";
	listItem->value = StarsPunishIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_FINESIZE_CAPTIONS, onchange_fine_size_index);
	listItem->wrap = false;
	listItem->caption = "Fine Amount";
	listItem->value = FineSizeIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Cop Vehicles Never Flip";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePoliceNoFlip;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Cop Vehicles Don't Take Damage";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePoliceNoDamage;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Cop Vehicle Blip";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePoliceVehicleBlip;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Cops Use Radio (Hardcore Mode)";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureCopsUseRadio;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_SPEEDINGCITY_CAPTIONS, onchange_speeding_city_index);
	listItem->wrap = false;
	listItem->caption = "Speeding In City";
	listItem->value = SpeedingCityIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_SPEEDINGCITY_CAPTIONS, onchange_speeding_speedway_index);
	listItem->wrap = false;
	listItem->caption = "Speeding On Freeway";
	listItem->value = SpeedingSpeedwayIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Running Red Light";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRunningRedLight;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Pavement Driving";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurePavementDriving;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Driving Against Traffic";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDrivingAgainstTraffic;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Vehicle Collision";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureCarCollision;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Using Phone While Driving";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureUsingMobilePhone;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Vehicle Heavily Damaged";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehicleHeavilyDamaged;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Helmet While Driving";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoHelmetOnBike;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Stolen Vehicle";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureStolenVehicle;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Driving Without Headlights At Night"; 
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoLightsNightTime;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Evading Police";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureEscapingPolice;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexRoadLaws, caption, onconfirm_road_laws_menu, NULL, NULL);
}

bool onconfirm_veh_menu(MenuItem<int> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch(activeLineIndexVeh){
		case 0:
			if(process_carspawn_menu()) return false;
			break;
		case 1:
			if(process_savedveh_menu()) return false;
			break;
		case 2: // fix
			fix_vehicle();
			break;
		case 4: // clean
			clean_vehicle();
			break;
		case 6: // paint
			if(process_paint_menu()) return false;
			break;
		case 7: // mods
			if(process_vehmod_menu()) return false;
			break;
		case 20: // door menu
			if(process_veh_door_menu()) return false;
			break;
		case 21: // seat menu
			if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(playerPed))
				if(process_veh_seat_menu()) return false;
			break;
		case 22: // speed menu
			process_speed_menu();
			break;
		case 23: // vehicle indicators menu
			process_visualize_menu();
			break;
		case 26: // vehicle indicators menu
			process_speedlimit_menu();
			break;
		case 29: // fuel menu
			process_fuel_menu();
			break;
		case 30: // remember vehicles menu
			process_remember_vehicles_menu();
			break;
		case 31: // road laws menu
			process_road_laws_menu();
			break;
		case 36: // engine can degrade
			process_engine_degrade_menu();
			break;
		case 47: // Plane bombs -- incomplete so commenting out in mean time
		{
			if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				set_status_text("Player isn't in a vehicle");
				//return true;
			}
			Hash currVehModel = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_USING(playerPed));
			if (GAMEPLAY::GET_HASH_KEY("CUBAN800") == currVehModel) {
				if (process_veh_weapons_menu()) return false;
			}
			else set_status_text("Error: Bomb doors require Cuban 800");
		}
			break;
		default:
			break;
	}
	return false;
}

void process_veh_menu(){
	std::string caption = "Vehicle Options";

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	item = new MenuItem<int>();
	item->caption = "Vehicle Spawner";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Saved Vehicles";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Fix";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_INVINC_MODE_CAPTIONS, onchange_veh_invincibility_mode);
	listItem->wrap = false;
	listItem->caption = "Vehicle Invincibility";
	listItem->value = get_current_veh_invincibility_mode();
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "Clean";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(WORLD_REDUCEDGRIP_SNOWING_CAPTIONS, onchange_veh_never_dirty);
	listItem->wrap = false;
	listItem->caption = "Vehicle Never Gets Dirty";
	listItem->value = featureNeverDirty;
	menuItems.push_back(listItem);
	
	item = new MenuItem<int>();
	item->caption = "Paint";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Modifications";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Falling Off/Out";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoVehFallOff;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Don't Wear Helmet";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureWearHelmetOff;
	toggleItem->toggleValueUpdated = &featureWearHelmetOffUpdated;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Into Vehicle";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnInto;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Vehicles Fully Tuned";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnTuned; 
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Vehicles Fully Pimped";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpawnOptic;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Speed Boost";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSpeedBoost;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_SPEED_BOOST_CAPTIONS, onchange_veh_speed_boost_index);
	listItem->wrap = false;
	listItem->caption = "Speed Boost Mode";
	listItem->value = speedBoostIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_ENG_POW_CAPTIONS, onchange_veh_eng_pow_index);
	listItem->wrap = false;
	listItem->caption = "Engine Power Multiplier";
	listItem->value = engPowMultIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_MASS_CAPTIONS, onchange_veh_mass_index);
	listItem->wrap = false;
	listItem->caption = "Vehicle Force Shield";
	listItem->value = VehMassMultIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_INFINITEBOOST_CAPTIONS, onchange_veh_infiniteboost_index);
	listItem->wrap = false;
	listItem->caption = "Infinite Rocket Boost";
	listItem->value = InfiniteBoostIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_veh_nitrous_index);
	listItem->wrap = false;
	listItem->caption = "Nitrous";
	listItem->value = NitrousIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Disable Despawn Of DLC Cars";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDespawnScriptDisabled;
	toggleItem->toggleValueUpdated = &featureDespawnScriptDisabledUpdated;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Vehicle Control";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "Vehicle Seats";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Show Speed / Altitude";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "Vehicle Indicators";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_ENGINERUNNING_CAPTIONS, onchange_veh_enginerunning_index);
	listItem->wrap = false;
	listItem->caption = "Keep Engine Running";
	listItem->value = EngineRunningIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_AUTO_SHUT_ENGINE_CAPTIONS, onchange_veh_autoshutengine_index);
	listItem->wrap = false;
	listItem->caption = "Shut Engine After (s)";
	listItem->value = AutoShutEngineIndex;
	menuItems.push_back(listItem);

	item = new MenuItem<int>();
	item->caption = "Speed Limiter";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(VEH_LIGHTSOFF_CAPTIONS, onchange_veh_lightsOff_index);
	listItem->wrap = false;
	listItem->caption = "Vehicle Lights Off By Default";
	listItem->value = lightsOffIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "No Headlights In The Evening";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureAutoToggleLights;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Fuel Consumption";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "Vehicle Tracking";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Road Laws";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Realistic Crashes";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureNoVehFlip;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Force Vehicle Lights On";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehLightsOn;
	toggleItem->toggleValueUpdated = &featureVehLightsOnUpdated;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Remember Wheel Angle";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureVehSteerAngle;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Current Mileage";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureMileage;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Engine Damage";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Seashark Spotlight";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureSeasharkLights;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Roll Driver Window Down When Shooting";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureRollWhenShoot;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Traction Control";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureTractionControl;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS, onchange_veh_jumpy_index);
	listItem->wrap = false;
	listItem->caption = "Vehicle Jump";
	listItem->value = JumpyVehIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_HYDRAULICS_CAPTIONS, onchange_veh_hydraulics_index);
	listItem->wrap = false;
	listItem->caption = "Suspension Height";
	listItem->value = HydraulicsIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Stick Vehicle To Ground";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureSticktoground;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_TURN_SIGNALS_ACCELERATION_CAPTIONS, onchange_heavy_veh_index);
	listItem->wrap = false;
	listItem->caption = "Heavy Vehicle";
	listItem->value = HeavyVehIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_SPEEDLIMITER_CAPTIONS, onchange_door_autolock_index);
	listItem->wrap = false;
	listItem->caption = "Autolock Driver Door At";
	listItem->value = DoorAutolockIndex;
	menuItems.push_back(listItem);
	
	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Drop Road Spikes";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureDropSpikes;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Airstrike";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureAirStrike;
	menuItems.push_back(toggleItem);

	item = new MenuItem<int>();
	item->caption = "Drop Bombs";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Disable Reverse When Braking";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureReverseWhenBraking;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexVeh, caption, onconfirm_veh_menu, NULL, NULL);
}

void speedlimiter_switching(){
	speedlimiter_switch = !speedlimiter_switch;
	if (speedlimiter_switch) set_status_text("Speed Limiter ON");
	else set_status_text("Speed Limiter OFF");
	WAIT(100);
}

void update_vehicle_features(BOOL bPlayerExists, Ped playerPed){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	int globalindex = 0;

	eGameVersion version = getGameVersion();

	switch (version) 
	{
		case VER_1_0_678_1_STEAM:
		case VER_1_0_678_1_NOSTEAM:
			*getGlobalPtr(2558120) = 1;
			break;

		case VER_1_0_757_4_STEAM:
		case VER_1_0_757_4_NOSTEAM:
			*getGlobalPtr(0x271803) = 1;
			break;

		case VER_1_0_791_2_STEAM:
		case VER_1_0_791_2_NOSTEAM:
			*getGlobalPtr(0x272A34) = 1;
			break;

		case VER_1_0_877_1_STEAM:
		case VER_1_0_877_1_NOSTEAM:
			*getGlobalPtr(0x2750BD) = 1;
			break;

		case VER_1_0_944_2_STEAM:
		case VER_1_0_944_2_NOSTEAM:
			*getGlobalPtr(0x279476) = 1;
			break;

		case VER_1_0_1011_1_STEAM:
		case VER_1_0_1011_1_NOSTEAM:
		case VER_1_0_1032_1_STEAM:
		case VER_1_0_1032_1_NOSTEAM:
			*getGlobalPtr(2593970) = 1;
			break;

		case VER_1_0_1103_2_STEAM:
		case VER_1_0_1103_2_NOSTEAM:
			*getGlobalPtr(2599337) = 1;
			break;

		case VER_1_0_1180_2_STEAM:
		case VER_1_0_1180_2_NOSTEAM:
			*getGlobalPtr(2606794) = 1;
			break;

		case VER_1_0_1290_1_STEAM:
		case VER_1_0_1290_1_NOSTEAM:
		case VER_1_0_1365_1_STEAM:
		case VER_1_0_1365_1_NOSTEAM:
			*getGlobalPtr(4265719) = 1;
			break;

		case VER_1_0_1493_0_STEAM:
		case VER_1_0_1493_0_NOSTEAM:
		case VER_1_0_1493_1_STEAM:
		case VER_1_0_1493_1_NOSTEAM:
			*getGlobalPtr(4266042) = 1;
			break;

		case VER_1_0_1604_0_STEAM:
		case VER_1_0_1604_0_NOSTEAM:
		case VER_1_0_1604_1_STEAM:
		case VER_1_0_1604_1_NOSTEAM:
			*getGlobalPtr(4266905) = 1;
			break;

		case VER_1_0_1734_0_STEAM:
		case VER_1_0_1734_0_NOSTEAM:
		case VER_1_0_1737_0_STEAM:
		case VER_1_0_1737_0_NOSTEAM:
			*getGlobalPtr(4267883) = 1;
			break;

		case VER_1_0_1868_0_STEAM:
		case VER_1_0_1868_0_NOSTEAM:
		case VER_1_0_1868_1_STEAM:
		case VER_1_0_1868_1_NOSTEAM:
			*getGlobalPtr(4268190) = 1;
			break;
	}
	/*if (version < 20) *getGlobalPtr(2558120) = 1;

	if ((version < 22 && version > 19)) *getGlobalPtr(2562051) = 1; 

	if (version < 26 && version > 21) *getGlobalPtr(2566708) = 1;

	if ((version > 25) && (version < 28)) *getGlobalPtr(2576573) = 1;

	if ((version > 27) && (version < 30)) *getGlobalPtr(2593910) = 1;

	if ((version > 29) && (version < 34)) *getGlobalPtr(2593970) = 1;

	if ((version > 33) && (version < 36)) *getGlobalPtr(2599337) = 1;

	if ((version > 35) && (version < 38)) *getGlobalPtr(2606794) = 1;

	if ((version > 37) && (version < 40)) *getGlobalPtr(4265719) = 1;

	if ((version > 39) && (version < 43)) *getGlobalPtr(4265719) = 1; //Same pointer for 1.42?

	if ((version > 41) && (version < 45)) *getGlobalPtr(4266042) = 1;

	if ((version > 44) && (version < 48)) *getGlobalPtr(4266905) = 1; //Global can be found in shop_controller.c

	//if (version > 38) *getGlobalPtr(2606794) = 1; //2606794*/

	if (featureDespawnScriptDisabledUpdated){
		featureDespawnScriptDisabledUpdated = false;
		if (featureDespawnScriptDisabled){
			set_status_text("~r~Note:~r~ in-game shops will not work until you turn off the 'disable despawn' option");
		}
	}
	if (featureDespawnScriptDisabled){
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("shop_controller");
		featureDespawnScriptDisabledWasLastOn = true;
	}
	else if (featureDespawnScriptDisabledWasLastOn == true) {
		SCRIPT::REQUEST_SCRIPT("shop_controller");
		SYSTEM::START_NEW_SCRIPT("shop_controller", 1424);
		featureDespawnScriptDisabledWasLastOn = false;
	}

	// Toggle Vehicle Alarm Check
	if (alarmischarged == true) { 
		if (featureAutoalarm && !VEHICLES_REMEMBER.empty()) { // Toggle Vehicle Alarm Automatically
			float dist_diff = -1.0;
			Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(playerPed, true);
			for (int i = 0; i < VEHICLES_REMEMBER.size(); i++) {
				Vector3 coordsveh = ENTITY::GET_ENTITY_COORDS(VEHICLES_REMEMBER[i], true);
				dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsveh.x, coordsveh.y, coordsveh.z);
				if (dist_diff < 10.0) {
					alarmed_veh = VEHICLES_REMEMBER[i];
					alarm_enabled = false;
				}
				if (dist_diff > 10.0 && alarmed_veh == VEHICLES_REMEMBER[i]) { 
					alarm_enabled = true;
				}
			}
		}
		if (alarm_enabled == true && alarmed_veh != -1) { // set the alarm
			engine_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - engine_secs_curr) != 0) {
				a_counter_tick = a_counter_tick + 1;
				engine_secs_curr = engine_secs_passed;
			}
			for (int j = 0; j < 6; j++) VEHICLE::SET_VEHICLE_DOOR_SHUT(alarmed_veh, j, true);
			VEHICLE::SET_VEHICLE_ENGINE_ON(alarmed_veh, false, true);
			if (a_counter_tick > 71) {
				VEHICLE::SET_VEHICLE_DOORS_LOCKED(alarmed_veh, 4);
				VEHICLE::SET_VEHICLE_ALARM(alarmed_veh, true);
				alarmed_veh = -1;
				near_enough = false;
				a_counter_tick = 0;
				if (!featureAutoalarm) alarmischarged = false;
			}
		}
		if (alarmed_veh != -1 && near_enough == false && alarm_enabled == false) { // disable the alarm
			a_counter_tick = a_counter_tick + 1;
			VEHICLE::SET_VEHICLE_ENGINE_ON(alarmed_veh, true, true);
			if (a_counter_tick > 71) {
				VEHICLE::SET_VEHICLE_DOORS_LOCKED(alarmed_veh, 0);
				VEHICLE::SET_VEHICLE_ALARM(alarmed_veh, false);
				near_enough = true;
				a_counter_tick = 0;
				if (!featureAutoalarm) alarmischarged = false;
			}
		}
		if (a_counter_tick > 5 && a_counter_tick < 70) {
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(alarmed_veh, 1, true); // Left Signal 
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(alarmed_veh, 0, true); // Right Signal
			VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(alarmed_veh, true);
			if ((a_counter_tick > 5 && a_counter_tick < 20) || (a_counter_tick > 40 && a_counter_tick < 60)) VEHICLE::SET_VEHICLE_LIGHTS(alarmed_veh, 2);
			if (a_counter_tick == 15) AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "SIREN_BLIP", alarmed_veh, "BIG_SCORE_3A_SOUNDS", 0, 0);
			if (a_counter_tick > 20 && a_counter_tick < 40 || (a_counter_tick > 60 && a_counter_tick < 70)) VEHICLE::SET_VEHICLE_LIGHTS(alarmed_veh, 1);
		}
		if (a_counter_tick > 70) {
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(alarmed_veh, 1, false); // Left Signal 
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(alarmed_veh, 0, false); // Right Signal
			VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(alarmed_veh, false);
			VEHICLE::SET_VEHICLE_LIGHTS(alarmed_veh, 0);
		}
	}

	// player's vehicle invincible
	if (featureVehInvincibleUpdated){
		if (bPlayerExists && !featureVehInvincible && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			ENTITY::SET_ENTITY_INVINCIBLE(veh, FALSE);
			ENTITY::SET_ENTITY_PROOFS(veh, 0, 0, 0, 0, 0, 0, 0, 0);
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 1);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 1);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, 1);
			for (int i = 0; i < 6; i++){
				VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, TRUE); //(Vehicle, doorIndex, isBreakable)
			}
			featureVehInvincibleUpdated = false;
		}
	}

	if (featureVehInvincible){
		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			if (FIRE::IS_ENTITY_ON_FIRE(veh)){
				FIRE::STOP_ENTITY_FIRE(veh);
			}

			ENTITY::SET_ENTITY_HEALTH(veh, 10000.0f);
			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 10000.0);
			VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 10000.0);

			ENTITY::SET_ENTITY_PROOFS(veh, 1, 1, 1, featureVehNoDamage, 1, 1, 1, 1); 
			VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, 0);
			VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, 0);

			VEHICLE::SET_VEHICLE_CAN_BREAK(veh, !featureVehNoDamage);
			ENTITY::SET_ENTITY_INVINCIBLE(veh, featureVehNoDamage);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(veh, !featureVehNoDamage);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, !featureVehNoDamage);

			for (int i = 0; i < 6; i++){
				VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, !featureVehNoDamage); //(Vehicle, doorIndex, isBreakable)
			}

			if (featureVehNoDamage){
				ENTITY::SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(veh, 1);

				VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 10000.0f);

				/*
				* This API seems to be a damage check - don't just continually repair the
				* vehicle as it causes glitches.
				*/

				if (VEHICLE::_IS_VEHICLE_DAMAGED(veh) && featureVehNoDamage && featureVehInvulnIncludesCosmetic){
					VEHICLE::SET_VEHICLE_FIXED(veh);
				}
			}
		}
	}

	// fall off
	if (bPlayerExists && !featureNoVehFallOff && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		PED::SET_PED_CONFIG_FLAG(playerPed, PED_FLAG_THROUGH_WINDSCREEN, TRUE);
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(playerPed, 0); // can
	}
	if (bPlayerExists && featureNoVehFallOff && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		for (int i = -1; i < 3; i++) {
			if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i) != 0) {
				PED::SET_PED_CONFIG_FLAG(VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i), PED_FLAG_THROUGH_WINDSCREEN, FALSE);
				PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i), 1); // can't
			}
		}
	}

	// player's vehicle boost
	if (bPlayerExists && featureVehSpeedBoost && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		bool bUp = IsKeyDown(KeyConfig::KEY_VEH_BOOST) || IsControllerButtonDown(KeyConfig::KEY_VEH_BOOST);
		bool bDown = IsKeyDown(KeyConfig::KEY_VEH_STOP) || IsControllerButtonDown(KeyConfig::KEY_VEH_STOP);

		if (bUp || bDown){
			if (bUp){
				float speed = ENTITY::GET_ENTITY_SPEED(veh);
				switch (speedBoostIndex){
				case 0:
					speed = speed * 1.05f;
					break;
				case 1:
					speed = speed * 1.02f + 4.0f;
					break;
				case 2:
					speed = 200.0f;
					break;
				default:
					std::ostringstream ss;
					ss << "speed boost index: " << speedBoostIndex;
					set_status_text_centre_screen(ss.str(), 1000UL);
					break;
				}
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, speed);
			}
			else{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, 0.0f);
				Vector3 rotation = ENTITY::GET_ENTITY_ROTATION(veh, 0);
				ENTITY::SET_ENTITY_ROTATION(veh, rotation.x, rotation.y, rotation.z, 0, 0);
			}
		}
	}

	// Hotkey for Seats
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) currseat = -1;

	//Prevents player from wearing a helmet
	if (bPlayerExists){
		if (featureWearHelmetOffUpdated || did_player_just_enter_vehicle(playerPed)){
			PED::SET_PED_HELMET(playerPed, !featureWearHelmetOff);
			featureWearHelmetOffUpdated = false;
		}
	}

	if (bPlayerExists && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)){
		oldVehicleState = false; // player is NOT in a vehicle, set state to false
	}

	if (is_hotkey_held_veh_burnout() && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		VEHICLE::SET_VEHICLE_BURNOUT(veh, true);
		burnoutApplied = true;
	}
	else if (burnoutApplied){
		VEHICLE::SET_VEHICLE_BURNOUT(veh, false);
		burnoutApplied = false;
	}

	if (is_hotkey_held_veh_extrapower() && bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(veh, 1.8f);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 250.0f);
		powChanged = true;
	}
	else if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && VEH_ENG_POW_VALUES[engPowMultIndex] >= 0) { // engine power multiplier
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(veh, 1.0f);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, VEH_ENG_POW_VALUES[engPowMultIndex]);
		powChanged = true;
	}
	
	// Seashark has head lights
	if (featureSeasharkLights) {
		int time = TIME::GET_CLOCK_HOURS();
		if (!PED::IS_PED_IN_ANY_BOAT(playerPed)) sheshark_light_toogle = 1;
		if (!PED::IS_PED_IN_ANY_BOAT(playerPed) && ((VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 0 && VEH_LIGHTSOFF_VALUES[lightsOffIndex] < 2 && time > 6 && time < 21) || (VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 1))) sheshark_light_toogle = 0;
		if (PED::IS_PED_IN_ANY_BOAT(playerPed)) {
			Vehicle veh_boat = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
			if (ENTITY::GET_ENTITY_MODEL(veh_boat) == GAMEPLAY::GET_HASH_KEY("SEASHARK") || ENTITY::GET_ENTITY_MODEL(veh_boat) == GAMEPLAY::GET_HASH_KEY("SEASHARK2")) {
				int bone_boat_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh_boat, "windscreen");
				int bone2_boat_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(veh_boat, "bodyshell");
				Vector3 bone_boat_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh_boat, bone_boat_index);
				Vector3 bone2_boat_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(veh_boat, bone2_boat_index);
				float dirVector_lf_lr_x = bone_boat_coord.x - bone2_boat_coord.x;
				float dirVector_lf_lr_y = bone_boat_coord.y - bone2_boat_coord.y;
				float dirVector_lf_lr_z = bone_boat_coord.z - (bone2_boat_coord.z + 1);
				if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(veh_boat)) {
					if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 74)) sheshark_light_toogle = sheshark_light_toogle + 1;
					if (sheshark_light_toogle == 3) sheshark_light_toogle = 0;
					if (sheshark_light_toogle == 1) GRAPHICS::_DRAW_SPOT_LIGHT_WITH_SHADOW(bone_boat_coord.x, bone_boat_coord.y, bone_boat_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 40.0, 1, 50, 31, 2.7, 5);
					if (sheshark_light_toogle == 2) GRAPHICS::_DRAW_SPOT_LIGHT_WITH_SHADOW(bone_boat_coord.x, bone_boat_coord.y, bone_boat_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 60.0, 1, 50, 41, 2.7, 10);
				}
			}
		}
	} 

	// Traction Control 
	if (featureTractionControl && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vector3 vehspeed = ENTITY::GET_ENTITY_VELOCITY(PED::GET_VEHICLE_PED_IS_IN(playerPed, false));
		if (vehspeed.x < 0) vehspeed.x = (vehspeed.x * -1);
		if (vehspeed.y < 0) vehspeed.y = (vehspeed.y * -1);
		if (vehspeed.x < 5 && vehspeed.y < 5 && high_speed == false) {
			traction_tick = 0;
			high_speed = true;
		}
		if (vehspeed.x > 4 || vehspeed.y > 4 || vehspeed.x < 0.1 || vehspeed.y < 0.1) high_speed = false;
		if (CONTROLS::IS_CONTROL_PRESSED(2, 71) || CONTROLS::IS_CONTROL_PRESSED(2, 62) || CONTROLS::IS_CONTROL_PRESSED(2, 72)) {
			engine_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - engine_secs_curr) != 0) {
				traction_tick = traction_tick + 1;
				engine_secs_curr = engine_secs_passed;
			}
		}
		if (traction_tick < 100) {
			if (traction_tick > 1 && traction_tick < 100 && CONTROLS::IS_CONTROL_PRESSED(2, 71)) VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), false);
			if (traction_tick > 1 && traction_tick < 50) {
				if (CONTROLS::IS_CONTROL_PRESSED(2, 71)) CONTROLS::_SET_CONTROL_NORMAL(0, 72, 0.5f);
				if (CONTROLS::IS_CONTROL_PRESSED(2, 72) || CONTROLS::IS_CONTROL_PRESSED(2, 62)) CONTROLS::_SET_CONTROL_NORMAL(0, 71, 0.5f);
			}
			if (traction_tick > 49 && traction_tick < 100) {
				if (CONTROLS::IS_CONTROL_PRESSED(2, 71)) CONTROLS::_SET_CONTROL_NORMAL(0, 72, 0.2f);
				if (CONTROLS::IS_CONTROL_PRESSED(2, 72) || CONTROLS::IS_CONTROL_PRESSED(2, 62)) CONTROLS::_SET_CONTROL_NORMAL(0, 71, 0.2f);
			}
		}
	}
	
	// Vehicle Never Gets Dirty
	if (WORLD_REDUCEDGRIP_SNOWING_VALUES[featureNeverDirty] > 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0.0);
		if (WORLD_REDUCEDGRIP_SNOWING_VALUES[featureNeverDirty] == 2) GRAPHICS::WASH_DECALS_FROM_VEHICLE(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 100.0);
	}

	// Infinite Rocket Boost
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && getGameVersion() > 36) {
		if (VEHICLE::_HAS_VEHICLE_ROCKET_BOOST(PED::GET_VEHICLE_PED_IS_IN(playerPed, false))) {
			if (VEH_INFINITEBOOST_VALUES[InfiniteBoostIndex] == 1 && CONTROLS::IS_CONTROL_PRESSED(2, 103)) {
				VEHICLE::_SET_VEHICLE_ROCKET_BOOST_REFILL_TIME(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0.0f);
				VEHICLE::_SET_VEHICLE_ROCKET_BOOST_PERCENTAGE(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 100.0f);
			}
			if (VEH_INFINITEBOOST_VALUES[InfiniteBoostIndex] == 2) {
				VEHICLE::_SET_VEHICLE_ROCKET_BOOST_REFILL_TIME(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0.0f);
				VEHICLE::_SET_VEHICLE_ROCKET_BOOST_PERCENTAGE(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 100.0f);
			}
		}
	}

	// Disable Reverse When Braking
	if (featureReverseWhenBraking && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vehicle brakecar = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(brakecar)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(brakecar)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(brakecar)) ||
			VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(brakecar))) {
			float veh_c_s = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed));
			if (CONTROLS::IS_CONTROL_PRESSED(2, 71) && veh_c_s > 2.0) accelerating_c = true; // accelerating
			if (veh_c_s < 2.1) accelerating_c = false;
			if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, 72) && veh_c_s > 2.0 && accelerating_c == true) reversing_c = true; // reversing/braking
			if (veh_c_s < 2.1 && reversing_c == true) {
				AI::TASK_VEHICLE_TEMP_ACTION(playerPed, PED::GET_VEHICLE_PED_IS_USING(playerPed), 6, 100);
				VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(PED::GET_VEHICLE_PED_IS_USING(playerPed), true);
			}
			if (CONTROLS::IS_DISABLED_CONTROL_JUST_RELEASED(2, 72)) {
				accelerating_c = false;
				reversing_c = false;
			}
		}
	}
	
	// Nitrous
	if (nitrous_m == -2) nitrous_m = NitrousIndex;
	if (NitrousIndex == 0 && nitrous_m != 0) nitrous_m = NitrousIndex;

	if (LIMP_IF_INJURED_VALUES[NitrousIndex] > 0) { // VehicleMoveUpOnly 61 VehicleSubAscend 131
		bool assigned = false;
		for (int i = 1; i < 10; i++) {
			if (get_hotkey_function_index(i) == 47) assigned = true;
		}

		if (nitrous_m != NitrousIndex) {
			if (NitrousIndex == 1) set_status_text("Sound ON");
			if (NitrousIndex == 2) set_status_text("Sound OFF");
			nitrous_m = NitrousIndex;
		}

		if (((CONTROLS::IS_CONTROL_PRESSED(2, 131) && assigned == false) || is_hotkey_held_veh_nitrous()) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			Vehicle my_veh = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
			if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(my_veh)) {
				STREAMING::REQUEST_NAMED_PTFX_ASSET("core");
				while (!STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("core")) WAIT(0);
				char* Exhausts[] = { "exhaust", "exhaust_2", "exhaust_3", "exhaust_4", "exhaust_5", "exhaust_6", "exhaust_7", "exhaust_8", "exhaust_9", "exhaust_10", "exhaust_11", "exhaust_12", "exhaust_13", "exhaust_14", "exhaust_15", "exhaust_16" };
				for (char* exhaust : Exhausts) {
					if (ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(my_veh, exhaust) > -1) {
						Vector3 exhaust_p = ENTITY::_GET_ENTITY_BONE_COORDS(my_veh, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(my_veh, exhaust)); // "exhaust"
						Vector3 exhaust_p_off = ENTITY::GET_OFFSET_FROM_ENTITY_GIVEN_WORLD_COORDS(my_veh, exhaust_p.x, exhaust_p.y, exhaust_p.z);
						GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("core");
						if (!is_this_a_heli_or_plane(my_veh)) GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("veh_backfire", my_veh, exhaust_p_off.x, exhaust_p_off.y, exhaust_p_off.z, 0.0f, ENTITY::GET_ENTITY_PITCH(my_veh), 0.0f, 1.0f, false, false, false);
						if (is_this_a_heli_or_plane(my_veh)) GRAPHICS::START_PARTICLE_FX_NON_LOOPED_ON_ENTITY("veh_backfire", my_veh, exhaust_p_off.x, exhaust_p_off.y, exhaust_p_off.z, 0.0f, ENTITY::GET_ENTITY_PITCH(my_veh), 0.0f, 4.0f, false, false, false);
					}
				}
				if (LIMP_IF_INJURED_VALUES[NitrousIndex] == 1) AUDIO::SET_VEHICLE_BOOST_ACTIVE(my_veh, true);
				if (!is_this_a_heli_or_plane(my_veh)) VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(my_veh, 10.0);
				if (is_this_a_heli_or_plane(my_veh) && CONTROLS::IS_CONTROL_PRESSED(2, 32)) {
					Vector3 MyRot = ENTITY::GET_ENTITY_ROTATION(my_veh, 2);
					float p_force = 5; //5;
					float rad = 2 * 3.14 * (MyRot.z / 360);
					float v_x = -(sin(rad) * p_force * 10);
					float v_y = (cos(rad) * p_force * 10);
					float v_z = p_force * (MyRot.x * 0.2);
					ENTITY::APPLY_FORCE_TO_ENTITY(my_veh, 1, v_x / 35, v_y / 35, v_z / 35, 0, 0, 0, true, false, true, true, true, true);
				}
				nitro_e = true;
			}
		}
	}
	if (LIMP_IF_INJURED_VALUES[NitrousIndex] > 0 && CONTROLS::IS_CONTROL_RELEASED(2, 61) && !is_hotkey_held_veh_nitrous() && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && nitro_e == true) {
		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 1.0);
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(veh, 0.0f);
		nitro_e = false;
	}

	// outside vehicle control
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		vehicle_been_used = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		Shut_seconds = 0;
	}
	
	// Shut the engine with time
	if (VEH_AUTO_SHUT_ENGINE_VALUES[AutoShutEngineIndex] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(vehicle_been_used)) {
		engine_secs_passed = clock() / CLOCKS_PER_SEC;
		if (((clock() / CLOCKS_PER_SEC) - engine_secs_curr) != 0) {
			Shut_seconds = Shut_seconds + 1;
			engine_secs_curr = engine_secs_passed;
		}
		if (Shut_seconds == VEH_AUTO_SHUT_ENGINE_VALUES[AutoShutEngineIndex]) VEHICLE::SET_VEHICLE_ENGINE_ON(vehicle_been_used, false, true);
	}

	// Stick vehicle to ground
	if (featureSticktoground) {
		Vehicle groundcar = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(groundcar)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(groundcar)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(groundcar)) ||
			VEHICLE::IS_THIS_MODEL_A_BOAT(ENTITY::GET_ENTITY_MODEL(groundcar))) {
			Vector3 vehstickspeed = ENTITY::GET_ENTITY_VELOCITY(PED::GET_VEHICLE_PED_IS_USING(playerPed));
			if (((vehstickspeed.x > 1) || (vehstickspeed.y > 1) || (vehstickspeed.z > 1)) && (ENTITY::GET_ENTITY_ROLL(groundcar) > 20 || ENTITY::GET_ENTITY_ROLL(groundcar) < -20)) VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(groundcar);
		}
	}

	// Drop Anchor
	if (anchor_dropped == true) {
		float height = -1.0;
		Vector3 coords_b_m = ENTITY::GET_ENTITY_COORDS(veh_anchor, true);
		float b_dist_diff = SYSTEM::VDIST(coords_b.x, coords_b.y, coords_b.z, coords_b_m.x, coords_b_m.y, coords_b_m.z);
		WATER::GET_WATER_HEIGHT(coords_b_m.x, coords_b_m.y, coords_b_m.z, &height);
		if (b_dist_diff > 5 && (coords_b_m.z >= height || ((coords_b_m.z < height) && ((height - coords_b_m.z) < 3)))) {
			if (coords_b_m.x < coords_b.x) coords_b_m.x = coords_b_m.x + 0.1; // 0.2
			if (coords_b_m.x > coords_b.x) coords_b_m.x = coords_b_m.x - 0.1; // 0.2
			if (coords_b_m.y < coords_b.y) coords_b_m.y = coords_b_m.y + 0.1; // 0.2
			if (coords_b_m.y > coords_b.y) coords_b_m.y = coords_b_m.y - 0.1; // 0.2
			if (ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE") || ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("SUBMERSIBLE2"))
				ENTITY::SET_ENTITY_COORDS(veh_anchor, coords_b_m.x, coords_b_m.y, height - 3, 1, 0, 0, 1); // - (coords_b_m.z - height)
			else if (ENTITY::GET_ENTITY_MODEL(veh_anchor) == GAMEPLAY::GET_HASH_KEY("DODO")) ENTITY::SET_ENTITY_COORDS(veh_anchor, coords_b_m.x, coords_b_m.y, height - 0.6, 1, 0, 0, 1); // -0.7
			else ENTITY::SET_ENTITY_COORDS(veh_anchor, coords_b_m.x, coords_b_m.y, height - (coords_b_m.z - height), 1, 0, 0, 1); // - 1
		}
		if ((coords_b_m.z < height) && ((height - coords_b_m.z) > 2)) ENTITY::SET_ENTITY_COORDS(veh_anchor, coords_b.x, coords_b.y, coords_b.z, 1, 0, 0, 1);
	}

	//////////////////////////////////////////////////// PLAYER/VEHICLE FORCE SHIELD ////////////////////////////////////////////////////////

	if ((VEH_MASS_VALUES[VehMassMultIndex] > 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && !PED::IS_PED_IN_ANY_PLANE(playerPed) && !PED::IS_PED_IN_ANY_HELI(playerPed)) || 
		(VEH_MASS_VALUES[current_player_forceshield] > 0 && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0))) { 
		const int OBJ_ARR_SIZE = 1024;
		Object nearbyObj[OBJ_ARR_SIZE];
		int veh_distance_x = 100;
		int veh_distance_y = 100;
		Object my_shield = -1;
		if (VEH_MASS_VALUES[VehMassMultIndex] > 0) my_shield = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		if (VEH_MASS_VALUES[current_player_forceshield] > 0) my_shield = PLAYER::PLAYER_PED_ID();
		Vector3 CamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(my_shield, true);
		int p_force = 5;
		float rad = 2 * 3.14 * (CamRot.z / 360);
		float v_x = -(sin(rad) * p_force * 10);
		float v_y = (cos(rad) * p_force * 10);
		float v_z = p_force * (CamRot.x * 0.2);
		
		int count_v = worldGetAllVehicles(nearbyObj, OBJ_ARR_SIZE); // vehicles
		for (int i = 0; i < count_v; i++) {
			Vector3 coordsveh = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
			veh_distance_x = (coordsme.x - coordsveh.x);
			veh_distance_y = (coordsme.y - coordsveh.y);
			if (veh_distance_x < 0) veh_distance_x = (veh_distance_x * -1);
			if (veh_distance_y < 0) veh_distance_y = (veh_distance_y * -1);
			if (nearbyObj[i] != PED::GET_VEHICLE_PED_IS_USING(playerPed) && ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) { // my_shield
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[VehMassMultIndex] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[current_player_forceshield] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
			if (nearbyObj[i] != PED::GET_VEHICLE_PED_IS_USING(playerPed) && !ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) { // my_shield
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
		} // end of for
		
		int count_p = worldGetAllPeds(nearbyObj, OBJ_ARR_SIZE); // pedestrians
		for (int i = 0; i < count_p; i++) {
			Vector3 coordsveh = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
			veh_distance_x = (coordsme.x - coordsveh.x);
			veh_distance_y = (coordsme.y - coordsveh.y);
			if (veh_distance_x < 0) veh_distance_x = (veh_distance_x * -1);
			if (veh_distance_y < 0) veh_distance_y = (veh_distance_y * -1);
			if (nearbyObj[i] != playerPed && ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) {
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					PED::SET_PED_CAN_RAGDOLL(nearbyObj[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(nearbyObj[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(nearbyObj[i]);
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[VehMassMultIndex] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[current_player_forceshield] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
			if (nearbyObj[i] != playerPed && !ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) {
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					PED::SET_PED_CAN_RAGDOLL(nearbyObj[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(nearbyObj[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(nearbyObj[i]);
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
		} // end of for
		
		int count_o = worldGetAllObjects(nearbyObj, OBJ_ARR_SIZE); // objects
		for (int i = 0; i < count_o; i++) {
			Vector3 coordsveh = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
			veh_distance_x = (coordsme.x - coordsveh.x);
			veh_distance_y = (coordsme.y - coordsveh.y);
			if (veh_distance_x < 0) veh_distance_x = (veh_distance_x * -1);
			if (veh_distance_y < 0) veh_distance_y = (veh_distance_y * -1);
			if (nearbyObj[i] != my_shield && ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) {
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[VehMassMultIndex] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, VEH_MASS_VALUES[current_player_forceshield] / 3, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
			if (nearbyObj[i] != my_shield && !ENTITY::IS_ENTITY_ON_SCREEN(nearbyObj[i])) {
				if (((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[VehMassMultIndex] / 1)) || ((veh_distance_x + veh_distance_y) < (VEH_MASS_VALUES[current_player_forceshield] / 1))) {
					if (VEH_MASS_VALUES[VehMassMultIndex] > 0 && VEH_MASS_VALUES[VehMassMultIndex] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[VehMassMultIndex] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[VehMassMultIndex]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
					if (VEH_MASS_VALUES[current_player_forceshield] > 0 && VEH_MASS_VALUES[current_player_forceshield] < 101) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					if (VEH_MASS_VALUES[current_player_forceshield] == 50000) ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 4, (ENTITY::GET_ENTITY_SPEED(my_shield) * VEH_MASS_VALUES[current_player_forceshield]), 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				}
			}
		} // end of for
	}
	else {
		std::vector<int> emptyVec;
		if (!VEH_MASS_VALUES.empty()) std::vector<int>(VEH_MASS_VALUES).swap(emptyVec); 
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/////////////////// HEAVY VEHICLE /////////////////////

	if (VEH_TURN_SIGNALS_ACCELERATION_VALUES[HeavyVehIndex] > 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && !PED::IS_PED_IN_ANY_PLANE(playerPed) && !PED::IS_PED_IN_ANY_HELI(playerPed)) {
		const int OBJ_ARR_SIZE = 1024;
		Object nearbyObj[OBJ_ARR_SIZE];
		Object my_shield = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		float vehspeed = ENTITY::GET_ENTITY_SPEED(my_shield);
		Vector3 CamRot = ENTITY::GET_ENTITY_ROTATION(playerPed, 0);
		int p_force = VEH_TURN_SIGNALS_ACCELERATION_VALUES[HeavyVehIndex]; // 1; // 3; // vehspeed; // / 10; // 5; 
		float rad = 2 * 3.14 * (CamRot.z / 360);
		float v_x = -(sin(rad) * p_force * 10);
		float v_y = (cos(rad) * p_force * 10);
		float v_z = p_force * (CamRot.x * 0.2);
		
		int count_v = worldGetAllVehicles(nearbyObj, OBJ_ARR_SIZE); // vehicles
		for (int i = 0; i < count_v; i++) {
			if (ENTITY::IS_ENTITY_TOUCHING_ENTITY(PED::GET_VEHICLE_PED_IS_USING(playerPed), nearbyObj[i]) && nearbyObj[i] != PED::GET_VEHICLE_PED_IS_USING(playerPed)) {
				bool v_atfront = false;
				bool v_behind = false;
				Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(my_shield, true);
				Vector3 coordsentity = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 135 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 225) { // south
					if (coordsentity.y < coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 315 || ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 45) { // north
					if (coordsentity.y > coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 46 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 134) { // west
					if (coordsentity.x < coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 226 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 314) { // east
					if (coordsentity.x > coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (v_atfront == true) {
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_atfront = false;
				}
				if (v_behind == true) {
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_behind = false;
				}
			}
		} // end of for

		int count_p = worldGetAllPeds(nearbyObj, OBJ_ARR_SIZE); // pedestrians
		for (int i = 0; i < count_p; i++) {
			if (ENTITY::IS_ENTITY_TOUCHING_ENTITY(PED::GET_VEHICLE_PED_IS_USING(playerPed), nearbyObj[i]) && nearbyObj[i] != playerPed) {
				bool v_atfront = false;
				bool v_behind = false;
				Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(my_shield, true);
				Vector3 coordsentity = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 135 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 225) { // south
					if (coordsentity.y < coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 315 || ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 45) { // north
					if (coordsentity.y > coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 46 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 134) { // west
					if (coordsentity.x < coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 226 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 314) { // east
					if (coordsentity.x > coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (v_atfront == true) {
					PED::SET_PED_CAN_RAGDOLL(nearbyObj[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(nearbyObj[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(nearbyObj[i]);
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_atfront = false;
				}
				if (v_behind == true) {
					PED::SET_PED_CAN_RAGDOLL(nearbyObj[i], true);
					PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(nearbyObj[i], true);
					PED::SET_PED_RAGDOLL_FORCE_FALL(nearbyObj[i]);
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_behind = false;
				}
			}
		} // end of for

		int count_o = worldGetAllObjects(nearbyObj, OBJ_ARR_SIZE); // objects
		for (int i = 0; i < count_o; i++) {
			if (ENTITY::IS_ENTITY_TOUCHING_ENTITY(PED::GET_VEHICLE_PED_IS_USING(playerPed), nearbyObj[i]) && nearbyObj[i] != my_shield) {
				bool v_atfront = false;
				bool v_behind = false;
				Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(my_shield, true);
				Vector3 coordsentity = ENTITY::GET_ENTITY_COORDS(nearbyObj[i], true);
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 135 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 225) { // south
					if (coordsentity.y < coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 315 || ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 45) { // north
					if (coordsentity.y > coordsme.y) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 46 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 134) { // west
					if (coordsentity.x < coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) >= 226 && ENTITY::_GET_ENTITY_PHYSICS_HEADING(playerPed) <= 314) { // east
					if (coordsentity.x > coordsme.x) v_atfront = true;
					else v_behind = true;
				}
				if (v_atfront == true) {
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, v_x, v_y, v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_atfront = false;
				}
				if (v_behind == true) {
					ENTITY::APPLY_FORCE_TO_ENTITY(nearbyObj[i], 1, -v_x, -v_y, -v_z, 0, 0, 0, false, false, true, true, false, true);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(PED::GET_VEHICLE_PED_IS_USING(playerPed), vehspeed);
					v_behind = false;
				}
			}
		} // end of for
	}

	////////////////////////////////////////////////////
		
	//////////////////////////////////////////////////// TURN SIGNALS ///////////////////////////////////////////////////////////

	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1)) {
		controllightsenabled_l = false;
		controllightsenabled_r = false;
		viz_veh_ind_left = false;
		viz_veh_ind_right = false;
	}

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0 || featureHazards) {
		Vehicle vehturn = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		int vehturnspeed = ENTITY::GET_ENTITY_SPEED(vehturn);
		int steer_turn = CONTROLS::GET_CONTROL_VALUE(0, 9);
		bool leftKey = IsKeyJustUp(KeyConfig::KEY_VEH_LEFTBLINK) || IsControllerButtonJustUp(KeyConfig::KEY_VEH_LEFTBLINK); // Left Key
		bool rightKey = IsKeyJustUp(KeyConfig::KEY_VEH_RIGHTBLINK) || IsControllerButtonJustUp(KeyConfig::KEY_VEH_RIGHTBLINK); // Right Key
		bool emergencyKey = IsKeyJustUp(KeyConfig::KEY_VEH_EMERGENCYBLINK) || IsControllerButtonJustUp(KeyConfig::KEY_VEH_EMERGENCYBLINK); // Emergency Signal Key

		if (leftKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) { // Manual Left Turn Signal
			turn_check_left = !turn_check_left;
			turn_check_right = false;
			controllightsenabled_l = turn_check_left;
			controllightsenabled_r = false;
		}
		if (rightKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) { // Manual Right Turn Signal
			turn_check_right = !turn_check_right;
			turn_check_left = false;
			controllightsenabled_r = turn_check_right;
			controllightsenabled_l = false;
		}
		if (emergencyKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) {
			if (turn_check_left == true && turn_check_right == true) {
				turn_check_left = false;
				turn_check_right = false;
			}
			else {
				turn_check_left = true;
				turn_check_right = true;
			}
			controllightsenabled_l = turn_check_left;
			controllightsenabled_r = turn_check_right;
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && turn_check_left && !controllightsenabled_l) {
			turn_check_left = false;
		}
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && turn_check_right && !controllightsenabled_r) {
			turn_check_right = false;
		}
		if (featureHazards && (VEHICLE::IS_VEHICLE_DOOR_DAMAGED(vehturn, 0) || VEHICLE::IS_VEHICLE_DOOR_DAMAGED(vehturn, 1) || VEHICLE::IS_VEHICLE_DOOR_DAMAGED(vehturn, 2) || VEHICLE::IS_VEHICLE_DOOR_DAMAGED(vehturn, 3))) {
			turn_check_right = true;
			turn_check_left = true;
		}
		if (PED::IS_PED_JUMPING_OUT_OF_VEHICLE(playerPed)) {
			turn_check_right = true;
			turn_check_left = true;
		}
		if (VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] != 1) { // Auto Blinkers
			if (vehturnspeed < VEH_TURN_SIGNALS_VALUES[turnSignalsIndex]) {
				if (steer_turn == 0 && !turn_check_left) { // Wheel Turned Left
					turn_check_left = true;
					turn_check_right = false;
					controllightsenabled_l = turn_check_left;
					controllightsenabled_r = false;
					autocontrol = true;
				}
				if (steer_turn == 254 && !turn_check_right) { // Wheel Turned Right
					turn_check_right = true;
					turn_check_left = false;
					controllightsenabled_r = turn_check_right;
					controllightsenabled_l = false;
					autocontrol = true;
				}
			}
		}
		if (steer_turn == 254 || steer_turn == 0) {
			engine_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - engine_secs_curr) != 0) {
				turn_angle = turn_angle + 1;
				engine_secs_curr = engine_secs_passed;
			}
		}
		else turn_angle = 0;
		if (CONTROLS::IS_CONTROL_PRESSED(2, 71) && VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex] > 0 && turn_angle < 15) { 
			Accel_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - Accel_secs_curr) != 0) {
				Accel_seconds = Accel_seconds + 1;
				Accel_secs_curr = Accel_secs_passed;
			}
		}
		else Accel_seconds = 0;
		if (VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex] > 0 && turn_angle < 15 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
			if ((GAMEPLAY::GET_GAME_TIMER() - Time_tick_mileage) > 200) {
				signal_meters = signal_meters + ((ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 1)) * (1.60934 * 0.02)) * 6.6);
				Time_tick_mileage = GAMEPLAY::GET_GAME_TIMER();
			}
		}
		else signal_meters = 0;
		if (ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_IN(playerPed, 1)) < 1) signal_meters = 0;

		if ((vehturnspeed > (VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] + 10) || Accel_seconds > VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex] || signal_meters > (VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex] * 22)) && autocontrol &&
			VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] != 1) {
			turn_check_left = false;
			turn_check_right = false;
			autocontrol = false;
		}
		if (turn_angle > VEH_TURN_SIGNALS_ANGLE_VALUES[turnSignalsAngleIndex] || (leftKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) || (rightKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) || 
			(emergencyKey && VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] > 0) || vehturnspeed > (VEH_TURN_SIGNALS_VALUES[turnSignalsIndex] + 10) || signal_meters > (VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex] * 22) ||
			Accel_seconds > VEH_TURN_SIGNALS_ACCELERATION_VALUES[turnSignalsAccelerationIndex]) {
			if (turn_check_left) viz_veh_ind_left = true;
			else viz_veh_ind_left = false;
			if (turn_check_right) viz_veh_ind_right = true;
			else viz_veh_ind_right = false;
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(vehturn, 1, turn_check_left);  // Left Signal 
			VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(vehturn, 0, turn_check_right); // Right Signal	
		}
	}
	else {
		std::vector<int> emptyVec;
		if (!VEH_TURN_SIGNALS_VALUES.empty()) std::vector<int>(VEH_TURN_SIGNALS_VALUES).swap(emptyVec);
		if (!VEH_TURN_SIGNALS_ANGLE_VALUES.empty()) std::vector<int>(VEH_TURN_SIGNALS_ANGLE_VALUES).swap(emptyVec);
		if (!VEH_TURN_SIGNALS_ACCELERATION_VALUES.empty()) std::vector<int>(VEH_TURN_SIGNALS_ACCELERATION_VALUES).swap(emptyVec);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////// VISUALIZE VEHICLE INDICATORS //////////////////////////////////////////////////////////

	if (bPlayerExists && (VEH_VISLIGHT_VALUES[VisLightIndex] > 0 || VEH_VISLIGHT_VALUES[VisLight3dIndex] > 0) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		Vector3 veh_indicators = ENTITY::GET_ENTITY_COORDS(playerVehicle, true);
		int time_indicators = TIME::GET_CLOCK_HOURS();

		if (viz_veh_ind_left) {
			if (!featureDaytimeonly) {
				if (!feature3rdpersonviewonly && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowleft", 0.4500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (feature3rdpersonviewonly && CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() != 4 && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowleft", 0.4500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (VEH_VISLIGHT_VALUES[VisLight3dIndex] > 0) GRAPHICS::DRAW_MARKER(2/*int type*/, veh_indicators.x + 0.5/*float posX*/, veh_indicators.y + 0.5/*float posY*/, veh_indicators.z + 3/*float posZ*/, 20/*float dirX*/, 20/*float dirY*/, 20/*float dirZ*/, 0/*float rotX*/, 270/*float rotY*/,
					0/*float rotZ*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleX*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleY*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleZ*/,
					44/*int red*/, 255/*int green*/, 32/*int blue*/, 155/*int alpha*/,
					50/*BOOL bobUpAndDown*/, 1/*BOOL faceCamera*/, 1/*int p19*/, 0/*BOOL rotate*/, 0/*char* textureDict*/, 0/*char* textureName*/, 0/*BOOL drawOnEnts*/);
			}
			if (featureDaytimeonly && time_indicators > 6 && time_indicators < 20) {
				if (!feature3rdpersonviewonly && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowleft", 0.4500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (feature3rdpersonviewonly && CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() != 4 && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowleft", 0.4500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (VEH_VISLIGHT_VALUES[VisLight3dIndex] > 0) GRAPHICS::DRAW_MARKER(2/*int type*/, veh_indicators.x + 0.5/*float posX*/, veh_indicators.y + 0.5/*float posY*/, veh_indicators.z + 3/*float posZ*/, 20/*float dirX*/, 20/*float dirY*/, 20/*float dirZ*/, 0/*float rotX*/, 270/*float rotY*/,
					0/*float rotZ*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleX*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleY*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleZ*/,
					44/*int red*/, 255/*int green*/, 32/*int blue*/, 155/*int alpha*/,
					50/*BOOL bobUpAndDown*/, 1/*BOOL faceCamera*/, 1/*int p19*/, 0/*BOOL rotate*/, 0/*char* textureDict*/, 0/*char* textureName*/, 0/*BOOL drawOnEnts*/);
			}
		}
		
		if (viz_veh_ind_right) {
			if (!featureDaytimeonly) {
				if (!feature3rdpersonviewonly && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowright", 0.5500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (feature3rdpersonviewonly && CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() != 4 && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowright", 0.5500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (VEH_VISLIGHT_VALUES[VisLight3dIndex] > 0) GRAPHICS::DRAW_MARKER(2/*int type*/, veh_indicators.x - 0.5/*float posX*/, veh_indicators.y - 0.5/*float posY*/, veh_indicators.z + 3/*float posZ*/, 20/*float dirX*/, 20/*float dirY*/, 20/*float dirZ*/, 0/*float rotX*/, 90/*float rotY*/,
					0/*float rotZ*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleX*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleY*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleZ*/,
					44/*int red*/, 255/*int green*/, 32/*int blue*/, 155/*int alpha*/,
					50/*BOOL bobUpAndDown*/, 1/*BOOL faceCamera*/, 1/*int p19*/, 0/*BOOL rotate*/, 0/*char* textureDict*/, 0/*char* textureName*/, 0/*BOOL drawOnEnts*/);
			}
			if (featureDaytimeonly && time_indicators > 6 && time_indicators < 20) {
				if (!feature3rdpersonviewonly && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowright", 0.5500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (feature3rdpersonviewonly && CAM::GET_FOLLOW_VEHICLE_CAM_VIEW_MODE() != 4 && VEH_VISLIGHT_VALUES[VisLightIndex] > 0) DrawSprite("commonmenu", "arrowright", 0.5500, 0.95, VEH_VISLIGHT_VALUES[VisLightIndex], VEH_VISLIGHT_VALUES[VisLightIndex], 1, 44, 255, 32, 255);
				if (VEH_VISLIGHT_VALUES[VisLight3dIndex] > 0) GRAPHICS::DRAW_MARKER(2/*int type*/, veh_indicators.x - 0.5/*float posX*/, veh_indicators.y - 0.5/*float posY*/, veh_indicators.z + 3/*float posZ*/, 20/*float dirX*/, 20/*float dirY*/, 20/*float dirZ*/, 0/*float rotX*/, 90/*float rotY*/,
					0/*float rotZ*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleX*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleY*/, VEH_VISLIGHT_VALUES[VisLight3dIndex] * 10/*float scaleZ*/,
					44/*int red*/, 255/*int green*/, 32/*int blue*/, 155/*int alpha*/,
					50/*BOOL bobUpAndDown*/, 1/*BOOL faceCamera*/, 1/*int p19*/, 0/*BOOL rotate*/, 0/*char* textureDict*/, 0/*char* textureName*/, 0/*BOOL drawOnEnts*/);
			}
		}
	}
	else {
		std::vector<double> emptyVec_d;
		if (!VEH_VISLIGHT_VALUES.empty()) std::vector<double>(VEH_VISLIGHT_VALUES).swap(emptyVec_d);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////// KEEP THE ENGINE RUNNING ///////////////////////////////////////////////////////////////

	if (bPlayerExists && LIMP_IF_INJURED_VALUES[EngineRunningIndex] > 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (CONTROLS::IS_CONTROL_PRESSED(2, 75) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 1) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(playerVehicle, true);
		if (CONTROLS::IS_CONTROL_PRESSED(2, 75) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2) {
			engine_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - engine_secs_curr) != 0) {
				engine_tick = engine_tick + 1;
				engine_secs_curr = engine_secs_passed;
			}
		}
		if (CONTROLS::IS_CONTROL_PRESSED(2, 75) && (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(playerVehicle)) || VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(playerVehicle)))) current_veh_e = playerVehicle;
	}
	
	if (engine_tick < 11 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && (!featureVehSteerAngle /*|| PED::IS_PED_ON_ANY_BIKE(playerPed)*/)) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), true);
	if (engine_tick > 10 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && (!featureVehSteerAngle /*|| PED::IS_PED_ON_ANY_BIKE(playerPed)*/)) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), false);
	// Remember Wheel Angle feature compatibility lines
	if (engine_tick < 3 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && featureVehSteerAngle /*&& !PED::IS_PED_ON_ANY_BIKE(playerPed)*/) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), true);
	if (engine_tick > 2 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && featureVehSteerAngle /*&& !PED::IS_PED_ON_ANY_BIKE(playerPed)*/) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), false);
	//
	if (bPlayerExists && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && CONTROLS::IS_CONTROL_RELEASED(2, 75)) engine_tick = 0;

	if (bPlayerExists && LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 0 && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && CONTROLS::IS_CONTROL_PRESSED(2, 75)) VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), false);
	
	// Helicopter's lines
	if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && current_veh_e != -1 && LIMP_IF_INJURED_VALUES[EngineRunningIndex] > 0) {
		if (LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 1) {
			VEHICLE::SET_HELI_BLADES_SPEED(current_veh_e, 1.0f);
			VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(current_veh_e, true);
		}
		if (LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && engine_tick < 3) {
			VEHICLE::SET_HELI_BLADES_SPEED(current_veh_e, 1.0f);
			VEHICLE::_SET_VEHICLE_JET_ENGINE_ON(current_veh_e, true);
		}
		if (LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2 && engine_tick > 2) current_veh_e = -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////// SPEED LIMIT ////////////////////////////////////////////////////////////////

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && (VEH_SPEEDLIMITER_VALUES[speedLimiterIndex] > 0) && speedlimiter_switch && !PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_HELI(PLAYER::PLAYER_PED_ID())) {
		Vehicle vehlimit = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		ENTITY::SET_ENTITY_MAX_SPEED(vehlimit, VEH_SPEEDLIMITER_VALUES[speedLimiterIndex]);
		speed_limit_e = true;
	}
	 
	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && ((VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] > 0) || (VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] > 0)) && 
		!PED::IS_PED_IN_ANY_PLANE(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_HELI(PLAYER::PLAYER_PED_ID())) {
		Vehicle vehlimit = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		Vector3 vehme_coords = ENTITY::GET_ENTITY_COORDS(vehlimit, true);

		char* temp_zone_name = ZONE::GET_NAME_OF_ZONE(vehme_coords.x, vehme_coords.y, vehme_coords.z);
		if (strcmp(temp_zone_name, "PALETO") == 0 || strcmp(temp_zone_name, "PALFOR") == 0 || strcmp(temp_zone_name, "GRAPES") == 0 || strcmp(temp_zone_name, "SANDY") == 0 || strcmp(temp_zone_name, "RICHM") == 0 ||
			strcmp(temp_zone_name, "GOLF") == 0 || strcmp(temp_zone_name, "MORN") == 0 || strcmp(temp_zone_name, "ROCKF") == 0 || strcmp(temp_zone_name, "WVINE") == 0 || strcmp(temp_zone_name, "BURTON") == 0 ||
			strcmp(temp_zone_name, "DTVINE") == 0 || strcmp(temp_zone_name, "HAWICK") == 0 || strcmp(temp_zone_name, "ALTA") == 0 || strcmp(temp_zone_name, "EAST_V") == 0 || strcmp(temp_zone_name, "DELBE") == 0 ||
			strcmp(temp_zone_name, "DELPE") == 0 || strcmp(temp_zone_name, "MOVIE") == 0 || strcmp(temp_zone_name, "KOREAT") == 0 || strcmp(temp_zone_name, "VINE") == 0 || strcmp(temp_zone_name, "DOWNT") == 0 ||
			strcmp(temp_zone_name, "VESP") == 0 || strcmp(temp_zone_name, "PBOX") == 0 || strcmp(temp_zone_name, "TEXTI") == 0 || strcmp(temp_zone_name, "SKID") == 0 || strcmp(temp_zone_name, "LMESA") == 0 ||
			strcmp(temp_zone_name, "MIRR") == 0 || strcmp(temp_zone_name, "MURRI") == 0 || strcmp(temp_zone_name, "BEACH") == 0 || strcmp(temp_zone_name, "DELSOL") == 0 || strcmp(temp_zone_name, "STRAW") == 0 ||
			strcmp(temp_zone_name, "CHAMH") == 0 || strcmp(temp_zone_name, "STAD") == 0 || strcmp(temp_zone_name, "DAVIS") == 0 || strcmp(temp_zone_name, "RANCHO") == 0 || strcmp(temp_zone_name, "BANNING") == 0 ||
			strcmp(temp_zone_name, "CYPRE") == 0 || strcmp(temp_zone_name, "HARMO") == 0 || strcmp(temp_zone_name, "PBLUFF") == 0) {
			being_in_city = true;
			being_on_motorway = false;
		}

		if (strcmp(temp_zone_name, "PALHIGH") == 0 || strcmp(temp_zone_name, "TATAMO") == 0 || strcmp(temp_zone_name, "CHIL") == 0 || strcmp(temp_zone_name, "WINDF") == 0 || strcmp(temp_zone_name, "SANCHIA") == 0 ||
			strcmp(temp_zone_name, "MTGORDO") == 0 || strcmp(temp_zone_name, "BRADP") == 0 || strcmp(temp_zone_name, "MTCHIL") == 0 || strcmp(temp_zone_name, "CMSW") == 0 || strcmp(temp_zone_name, "NCHU") == 0 ||
			strcmp(temp_zone_name, "LAGO") == 0 || strcmp(temp_zone_name, "TONGVAH") == 0 || strcmp(temp_zone_name, "CHU") == 0 || strcmp(temp_zone_name, "BANHAMC") == 0 || strcmp(temp_zone_name, "DESRT") == 0 ||
			strcmp(temp_zone_name, "BHAMCA") == 0) {
			being_on_motorway = true;
			being_in_city = false;
		}

		if (strcmp(temp_zone_name, "AIRP") == 0 || strcmp(temp_zone_name, "ALAMO") == 0 || strcmp(temp_zone_name, "ARMYB") == 0 || strcmp(temp_zone_name, "BRADT") == 0 || strcmp(temp_zone_name, "CALAFB") == 0 ||
			strcmp(temp_zone_name, "CANNY") == 0 || strcmp(temp_zone_name, "CCREAK") == 0 || strcmp(temp_zone_name, "EBURO") == 0 || strcmp(temp_zone_name, "ELGORL") == 0 || strcmp(temp_zone_name, "ELYSIAN") == 0 ||
			strcmp(temp_zone_name, "GALFISH") == 0 || strcmp(temp_zone_name, "GREATC") == 0 || strcmp(temp_zone_name, "HORS") == 0 || strcmp(temp_zone_name, "HUMLAB") == 0 || strcmp(temp_zone_name, "JAIL") == 0 ||
			strcmp(temp_zone_name, "LACT") == 0 || strcmp(temp_zone_name, "LDAM") == 0 || strcmp(temp_zone_name, "LEGSQU") == 0 || strcmp(temp_zone_name, "LOSPUER") == 0 || strcmp(temp_zone_name, "MTJOSE") == 0 ||
			strcmp(temp_zone_name, "NOOSE") == 0 || strcmp(temp_zone_name, "OCEANA") == 0 || strcmp(temp_zone_name, "PALCOV") == 0 || strcmp(temp_zone_name, "PALMPOW") == 0 || strcmp(temp_zone_name, "PROCOB") == 0 ||
			strcmp(temp_zone_name, "RGLEN") == 0 || strcmp(temp_zone_name, "RTRAK") == 0 || strcmp(temp_zone_name, "SANAND") == 0 || strcmp(temp_zone_name, "SLAB") == 0 || strcmp(temp_zone_name, "TERMINA") == 0 ||
			strcmp(temp_zone_name, "TONGVAV") == 0 || strcmp(temp_zone_name, "VCANA") == 0 || strcmp(temp_zone_name, "ZANCUDO") == 0 || strcmp(temp_zone_name, "ZP_ORT") == 0 || strcmp(temp_zone_name, "ZQ_UAR") == 0)
		{
			being_in_city = false;
			being_on_motorway = false;
		}

		if ((VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] > 0) && being_in_city == true) ENTITY::SET_ENTITY_MAX_SPEED(vehlimit, VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex]);
		if ((VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] > 0) && being_on_motorway == true) ENTITY::SET_ENTITY_MAX_SPEED(vehlimit, VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex]);
		if (being_in_city == false && being_on_motorway == false) ENTITY::SET_ENTITY_MAX_SPEED(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 15000.0);
		speed_limit_e = true;
	}

	if (((((VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] == 0 && being_in_city == true && VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] > 0 && being_on_motorway == false) || 
		(VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] > 0 && being_in_city == false && VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] == 0 && being_on_motorway == true)) &&
		((VEH_SPEEDLIMITER_VALUES[speedLimiterIndex] > 0 && !speedlimiter_switch) || VEH_SPEEDLIMITER_VALUES[speedLimiterIndex] == 0)) ||
		(VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] == 0 && VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] == 0 && VEH_SPEEDLIMITER_VALUES[speedLimiterIndex] == 0) ||
		(VEH_SPEEDLIMITER_VALUES[speedLimiterIndex] > 0 && !speedlimiter_switch && VEH_SPEEDLIMITER_VALUES[speedCityLimiterIndex] == 0 && VEH_SPEEDLIMITER_VALUES[speedCountryLimiterIndex] == 0)) && speed_limit_e == true) {
		ENTITY::SET_ENTITY_MAX_SPEED(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 15000.0);
		std::vector<int> emptyVec;
		if (!VEH_SPEEDLIMITER_VALUES.empty()) std::vector<int>(VEH_SPEEDLIMITER_VALUES).swap(emptyVec);
		speed_limit_e = false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////// AUTOLOCK DRIVER DOOR ///////////////////////////////////////////////////////////

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && (VEH_SPEEDLIMITER_VALUES[DoorAutolockIndex] > 0)) { 
		Vehicle vehautolock = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		int vehcurrautospeed = ENTITY::GET_ENTITY_SPEED(vehautolock);

		if (vehcurrautospeed > VEH_SPEEDLIMITER_VALUES[DoorAutolockIndex]) {
			VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehautolock, 4);
		} else VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehautolock, 0);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////// LIGHTS OFF BY DEFAULT ///////////////////////////////////////////////////////

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && (VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 0)) {

		Vehicle vehlights = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		int time = TIME::GET_CLOCK_HOURS();

		if ((VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 0 && VEH_LIGHTSOFF_VALUES[lightsOffIndex] < 2 && time > 6 && time < 21)) {
			if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(vehlights)) {
				if (LightAlwaysOff)	{
					VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 1);
					LightAlwaysOff = false;
				}

				if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 74)) {
					WAIT(100);
					if (LightAlwaysOff)	{
						LightAlwaysOff = false;
					}
					else {
						VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 0);
						LightAlwaysOff = false;
					}
				}
			}
			else {
				LightAlwaysOff = true;
			}
		}

		if ((VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 0 && VEH_LIGHTSOFF_VALUES[lightsOffIndex] < 2 && (time < 7 || time > 20))) {
			if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(vehlights)) {
				if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 74)) {
					WAIT(100);
					if (LightAlwaysOff) {
						LightAlwaysOff = false;
					}
					else {
					VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 0);
					LightAlwaysOff = false;
					}
				}
			}
		}

		if ((VEH_LIGHTSOFF_VALUES[lightsOffIndex] > 1)) {
			if (VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(vehlights)) {
				if (LightAlwaysOff) {
					VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 1);
					LightAlwaysOff = false;
				}

				if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 74)) {
					WAIT(100);
					if (LightAlwaysOff) {
						LightAlwaysOff = false;
					}
					else {
						VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 0);
						LightAlwaysOff = false;
					}
				}
			}
			else {
				LightAlwaysOff = true;
			}
		}
	}
	else {
		std::vector<int> emptyVec;
		if (!VEH_LIGHTSOFF_VALUES.empty()) std::vector<int>(VEH_LIGHTSOFF_VALUES).swap(emptyVec);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////// NO LIGHTS ON AT NIGHT AUTO TOGGLE ///////////////////////////////////////////////////////

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && featureAutoToggleLights) { 
		Vehicle vehlights = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		int autotime = TIME::GET_CLOCK_HOURS();
		bool autolights_state = VEHICLE::GET_VEHICLE_LIGHTS_STATE(vehlights, &lightsAutoOn, &highbeamsAutoOn);
		if (autotime > 18 && autotime < 20 && !lightsAutoOn && !highbeamsAutoOn) {
			no_autotoggle = true;
			autotoggle_temp = true;
		}
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 74) && autotime > 19) {
			no_autotoggle = !no_autotoggle;
			autotoggle_temp = true;
		}
		if (no_autotoggle == true && autotoggle_temp == true) {
			VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 1);
			autotoggle_temp = false;
		}
		if (no_autotoggle == false && autotoggle_temp == true) {
			VEHICLE::SET_VEHICLE_LIGHTS(vehlights, 0);
			autotoggle_temp = false;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	fuel(); ///// <--- FUEL CONSUMPTION /////
	
	road_laws(); ///// <--- ROAD LAWS /////

	engine_can_degrade(); ///// <--- ENGINE CAN DAMAGE /////

/////////////////////////////////////////////  REMEMBER VEHICLES /////////////////////////////////////////////////////////////
	
	if (featureRememberVehicles && GAMEPLAY::GET_MISSION_FLAG() == 1) {
		if (!BLIPTABLE_VEH.empty()) {
			for (int i = 0; i < BLIPTABLE_VEH.size(); i++) {
				if (UI::DOES_BLIP_EXIST(BLIPTABLE_VEH[i])) {
					UI::REMOVE_BLIP(&BLIPTABLE_VEH[i]);
				}
			}
			BLIPTABLE_VEH.clear();
			BLIPTABLE_VEH.shrink_to_fit();
			featureDeleteTrackedVehicles_Emptied = false;
		}
	}

	if (GAMEPLAY::GET_MISSION_FLAG() == 0) {
		Ped playerPed = PLAYER::PLAYER_PED_ID();

		if (!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1) && been_already == true) been_already = false;

		if (!VEHICLES_REMEMBER.empty() && !featureDeleteTrackedVehicles && featureDeleteTrackedVehicles_Emptied == false) {
			for (int i = 0; i < VEHICLES_REMEMBER.size(); i++) {
				blip_veh[0] = UI::ADD_BLIP_FOR_ENTITY(VEHICLES_REMEMBER[i]);
				UI::SET_BLIP_AS_FRIENDLY(blip_veh[0], true);
				if (VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex] != NULL) UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex]);
				else UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[0]);
				UI::SET_BLIP_CATEGORY(blip_veh[0], 2);
				if (featureBlipNumber) UI::SHOW_NUMBER_ON_BLIP(blip_veh[0], BLIPTABLE_VEH.size());
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_veh[0], true);
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_veh[0], true);
				UI::SET_BLIP_SCALE(blip_veh[0], VEH_BLIPSIZE_VALUES[VehBlipSizeIndex]);
				UI::SET_BLIP_COLOUR(blip_veh[0], VEH_BLIPCOLOUR_VALUES[VehBlipColourIndex]);
				UI::SET_BLIP_AS_SHORT_RANGE(blip_veh[0], true);
				BLIPTABLE_VEH.push_back(blip_veh[0]);
				featureDeleteTrackedVehicles_Emptied = true;
			}
		}

		if (featureRememberVehicles && featureDeleteTrackedVehicles_CharacterChanged && playerPed != old_playerPed_Tracking) {
			if (!BLIPTABLE_VEH.empty()) {
				for (int i = 0; i < BLIPTABLE_VEH.size(); i++) {
					if (UI::DOES_BLIP_EXIST(BLIPTABLE_VEH[i])) {
						UI::REMOVE_BLIP(&BLIPTABLE_VEH[i]);
					}
				}
				BLIPTABLE_VEH.clear();
				BLIPTABLE_VEH.shrink_to_fit();
				featureDeleteTrackedVehicles_Emptied = false;
			}

			if (!VEHICLES_REMEMBER.empty()) {
				for (int i = 0; i < VEHICLES_REMEMBER.size(); i++) {
					VEHICLE::DELETE_VEHICLE(&VEHICLES_REMEMBER[i]);
				}
				VEHICLES_REMEMBER.clear();
				VEHICLES_REMEMBER.shrink_to_fit();
			}
		}

		if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && featureRememberVehicles) { 
			Vehicle veh_rem = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

			// save in garage
			if (VEHICLE::IS_VEHICLE_IN_GARAGE_AREA("Michael - Beverly Hills", PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID())) || VEHICLE::IS_VEHICLE_IN_GARAGE_AREA("Franklin - Hills", PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID())) ||
				VEHICLE::IS_VEHICLE_IN_GARAGE_AREA("Franklin - Aunt", PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID())) || VEHICLE::IS_VEHICLE_IN_GARAGE_AREA("Trevor - Countryside", PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID())))
			{
				Vehicle my_c_v = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&my_c_v);
			}
			else ENTITY::SET_ENTITY_AS_MISSION_ENTITY(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true, true);
			//

			if (VEHICLES_REMEMBER.empty()) {
				blip_veh[0] = UI::ADD_BLIP_FOR_ENTITY(veh_rem);
				UI::SET_BLIP_AS_FRIENDLY(blip_veh[0], true);
				if (VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex] != NULL) UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex]);
				else UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[0]);
				UI::SET_BLIP_CATEGORY(blip_veh[0], 2);
				if (featureBlipNumber) UI::SHOW_NUMBER_ON_BLIP(blip_veh[0], BLIPTABLE_VEH.size());
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_veh[0], true);
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_veh[0], true);
				UI::SET_BLIP_SCALE(blip_veh[0], VEH_BLIPSIZE_VALUES[VehBlipSizeIndex]);
				UI::SET_BLIP_COLOUR(blip_veh[0], VEH_BLIPCOLOUR_VALUES[VehBlipColourIndex]);
				UI::SET_BLIP_AS_SHORT_RANGE(blip_veh[0], true);
				BLIPTABLE_VEH.push_back(blip_veh[0]);
				VEHICLES_REMEMBER.push_back(veh_rem);
				curr_veh_remember = veh_rem;
				featureDeleteTrackedVehicles_Emptied = true;
				old_playerPed_Tracking = playerPed;
			}

			for (int i = 0; i < VEHICLES_REMEMBER.size(); i++) {
				if (VEHICLES_REMEMBER[i] == veh_rem) {
					been_already = true;
					curr_veh_remember = veh_rem;
				}
			}

			if (curr_veh_remember != veh_rem) {
				been_already = false;
				curr_veh_remember = veh_rem;
			}

			if (been_already == false) {
				blip_veh[0] = UI::ADD_BLIP_FOR_ENTITY(veh_rem);
				UI::SET_BLIP_AS_FRIENDLY(blip_veh[0], true);
				if (VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex] != NULL) UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[VehBlipSymbolIndex]);
				else UI::SET_BLIP_SPRITE(blip_veh[0], VEH_BLIPSYMBOL_VALUES[0]);
				UI::SET_BLIP_CATEGORY(blip_veh[0], 2);
				if (featureBlipNumber) UI::SHOW_NUMBER_ON_BLIP(blip_veh[0], BLIPTABLE_VEH.size());
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_veh[0], true);
				if (LIMP_IF_INJURED_VALUES[VehBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_veh[0], true);
				UI::SET_BLIP_SCALE(blip_veh[0], VEH_BLIPSIZE_VALUES[VehBlipSizeIndex]);
				UI::SET_BLIP_COLOUR(blip_veh[0], VEH_BLIPCOLOUR_VALUES[VehBlipColourIndex]);
				UI::SET_BLIP_AS_SHORT_RANGE(blip_veh[0], true);
				BLIPTABLE_VEH.push_back(blip_veh[0]);
				VEHICLES_REMEMBER.push_back(veh_rem);

				if (VEHICLES_REMEMBER.size() > VEH_VEHREMEMBER_VALUES[VehRememberIndex]) {
					UI::REMOVE_BLIP(&BLIPTABLE_VEH[0]);
					VEHICLE::DELETE_VEHICLE(&VEHICLES_REMEMBER[0]);
					BLIPTABLE_VEH.erase(BLIPTABLE_VEH.begin());
					VEHICLES_REMEMBER.erase(VEHICLES_REMEMBER.begin());
					if (featureBlipNumber) {
						for (int i = 0; i < BLIPTABLE_VEH.size(); i++) {
							UI::SHOW_NUMBER_ON_BLIP(BLIPTABLE_VEH[i], i);
						}
					}
				}
			}
		} // end of the main body

		if (!featureRememberVehicles) {
			if (!BLIPTABLE_VEH.empty()) {
				for (int i = 0; i < BLIPTABLE_VEH.size(); i++) {
					if (UI::DOES_BLIP_EXIST(BLIPTABLE_VEH[i])) {
						UI::REMOVE_BLIP(&BLIPTABLE_VEH[i]);
					}
				}
				BLIPTABLE_VEH.clear();
				BLIPTABLE_VEH.shrink_to_fit();
				featureDeleteTrackedVehicles_Emptied = false;
			}

			if (featureDeleteTrackedVehicles && !VEHICLES_REMEMBER.empty()) {
				for (int i = 0; i < VEHICLES_REMEMBER.size(); i++) {
					VEHICLE::DELETE_VEHICLE(&VEHICLES_REMEMBER[i]);
				}
				VEHICLES_REMEMBER.clear();
				VEHICLES_REMEMBER.shrink_to_fit();
			}
			std::vector<int> emptyVec;
			std::vector<double> emptyVec_d;
			if (!VEH_VEHREMEMBER_VALUES.empty()) std::vector<int>(VEH_VEHREMEMBER_VALUES).swap(emptyVec);
			if (!VEH_BLIPSIZE_VALUES.empty()) std::vector<double>(VEH_BLIPSIZE_VALUES).swap(emptyVec_d);
			if (!VEH_BLIPCOLOUR_VALUES.empty()) std::vector<int>(VEH_BLIPCOLOUR_VALUES).swap(emptyVec);
			if (!VEH_BLIPSYMBOL_VALUES.empty()) std::vector<int>(VEH_BLIPSYMBOL_VALUES).swap(emptyVec);
		}
		
		if (featureAutoalarm) {
			alarmischarged = true;
			vehicle_set_alarm();
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
////////////////////////////////////////////////////// MILEAGE OPTION ///////////////////////////////////////////////////////

	if (featureMileage && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true))
	{
		Vehicle vehmileage = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		float veh_mileage_speed = ENTITY::GET_ENTITY_SPEED(vehmileage);
		int screen_w, screen_h;
		GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

		if ((GAMEPLAY::GET_GAME_TIMER() - Time_tick_mileage) > 200) {
			mileage = mileage + ((veh_mileage_speed * (1.60934 * 0.02)) * 6.6); 
			Time_tick_mileage = GAMEPLAY::GET_GAME_TIMER();
		}

		std::string MileageStatusLines[1];
		std::stringstream ss;
		ss << fixed << setprecision(2) << "\n" << mileage << " m" << endl;
		int index = 0;
		MileageStatusLines[index++] = ss.str();
		int numActualLines = 0;
		for (int i = 0; i < 1; i++) {
			numActualLines++;
			UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
			UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)MileageStatusLines[i].c_str());
			UI::SET_TEXT_FONT(0);
			UI::SET_TEXT_SCALE(0.5, 0.5);
			UI::SET_TEXT_WRAP(0.0, 1.0);
			UI::SET_TEXT_COLOUR(255, 255, 255, 255); // 255, 128, 64, 255
			UI::SET_TEXT_CENTRE(0);
			UI::SET_TEXT_DROPSHADOW(20, 20, 20, 20, 20);
			UI::SET_TEXT_EDGE(100, 100, 100, 100, 205);
			UI::SET_TEXT_LEADING(1);
			UI::END_TEXT_COMMAND_DISPLAY_TEXT(0.7, 0.9);
		}

		float rectXScaled = 1 - ((1230 / (float)screen_w) / 4); // 1130
		float rectYScaled = 1 - (((6 + (1 * 1)) / (float)screen_h) * 5);
		float rectWidthScaled = (360 / (float)screen_w) / 2; // 460
		float rectHeightScaled = (25 + (1 * 18)) / (float)screen_h; // 30
		int rect_col[4] = { 0, 0, 0, 255 }; // 128, 128, 128, 75
		GRAPHICS::DRAW_RECT(rectXScaled, rectYScaled, rectWidthScaled, rectHeightScaled, rect_col[0], rect_col[1], rect_col[2], rect_col[3]);
	}
	else mileage = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////// REALISTIC CRASHES //////////////////////////////////////////////////////

	if (featureNoVehFlip) {
		Vehicle vehnoflip = PED::GET_VEHICLE_PED_IS_IN(playerPed, 1);
		Vector3 veh_flip = ENTITY::GET_ENTITY_COORDS(vehnoflip, true);
		Hash currVehModel_noflip = ENTITY::GET_ENTITY_MODEL(vehnoflip);
		float veh_flips_speed = ENTITY::GET_ENTITY_SPEED(vehnoflip);
		
		if (VEHICLE::IS_THIS_MODEL_A_CAR(currVehModel_noflip)) {
			if (ENTITY::GET_ENTITY_ROLL(vehnoflip) > 90 || ENTITY::GET_ENTITY_ROLL(vehnoflip) < -90) {
				VEHICLE::SET_VEHICLE_CAN_BREAK(vehnoflip, true);
				VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(vehnoflip, false, false);
			}
			float height_a_g = -1;
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(veh_flip.x, veh_flip.y, veh_flip.z, &height_a_g);
			if (veh_flip.z - height_a_g > 2.0) VEHICLE::SET_VEHICLE_OUT_OF_CONTROL(vehnoflip, false, false);

			if ((veh_flips_speed * 2.3) > 50 && (ENTITY::GET_ENTITY_ROLL(vehnoflip) > 50 || ENTITY::GET_ENTITY_ROLL(vehnoflip) < -50)) { // (veh_flips_speed * 3.6) > 50
				VEHICLE::SET_VEHICLE_CEILING_HEIGHT(vehnoflip, 0.0);
				VEHICLE::SET_VEHICLE_DAMAGE(vehnoflip, veh_flip.x, veh_flip.y, veh_flip.z, 1000, 100, true);
			}
			if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(vehnoflip)) {
				float t_coord = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				VEHICLE::SET_VEHICLE_CAN_BREAK(vehnoflip, true);
				if ((veh_flips_speed * 2.3) > 60) {
					VEHICLE::SET_VEHICLE_CEILING_HEIGHT(vehnoflip, 0.0);
					VEHICLE::SET_VEHICLE_DAMAGE(vehnoflip, veh_flip.x - t_coord, veh_flip.y - t_coord, veh_flip.z, 500, 100, true);
				}
				if ((veh_flips_speed * 2.3) > 90) {
					VEHICLE::SET_VEHICLE_CEILING_HEIGHT(vehnoflip, 0.0);
					VEHICLE::SET_VEHICLE_DAMAGE(vehnoflip, veh_flip.x - t_coord, veh_flip.y - t_coord, veh_flip.z, 1000, 100, true);
					if (ENTITY::GET_ENTITY_HEALTH(vehnoflip) < 200) {
						int randomize = rand() % 5 + 1;
						VEHICLE::SET_VEHICLE_TYRE_BURST(vehnoflip, randomize, true, 1000.0);
						VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(vehnoflip, true);
					}
				}
				if (ENTITY::GET_ENTITY_HEALTH(vehnoflip) < 1) {
					VEHICLE::SET_VEHICLE_ENGINE_ON(vehnoflip, false, true);
					VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehnoflip, -4000);
				}
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////// REMEMBER STEERING ANGLE //////////////////////////// ORIGINAL CODE BY MRGTAMODSGERMAN 
	
	if (featureVehSteerAngle && !STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("BMX"))) STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("BMX"));

	if (featureVehSteerAngle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && CONTROLS::IS_CONTROL_PRESSED(2, 75)) { //  && !PED::IS_PED_ON_ANY_BIKE(playerPed)
		Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);

		Vector3 myvehicle_coords = ENTITY::GET_ENTITY_COORDS(myVehicle, true);
		float myvehicle_heading = ENTITY::GET_ENTITY_HEADING(myVehicle);

		Vehicle temp_object = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("BMX"), myvehicle_coords.x, myvehicle_coords.y, myvehicle_coords.z + 10, myvehicle_heading, 1, 1); // 20, 1
		ENTITY::ATTACH_ENTITY_TO_ENTITY_PHYSICALLY(/*ENTITY_1*/myVehicle, /*ENTITY_2*/temp_object, /*BONE_INDEX_1*/0, /*BONE_INDEX_2*/0.0, /*XPOS_1*/50.0, /*YPOS_1*/50.0, /*ZPOS_1*/+10.0,
			/*XPOS_2*/0.0, /*YPOS_2*/0.0, /*ZPOS_2*/0.0, /*XROT*/0.0, /*YROT*/0.0, /*ZROT*/0.0, /*BREAKFORCE*/1.0, /*FIXEDROT*/true, /*P15*/false, /*COLLISION*/false, /*P17*/1, /*P18*/true);
		ENTITY::SET_ENTITY_ALPHA(temp_object, 0, 0);
		if (LIMP_IF_INJURED_VALUES[EngineRunningIndex] < 2) WAIT(1000);
		if (LIMP_IF_INJURED_VALUES[EngineRunningIndex] == 2) WAIT(100); // Keep Engine Running feature compatibility line 

		ENTITY::DETACH_ENTITY(myVehicle, true, true);
		VEHICLE::DELETE_VEHICLE(&temp_object);
	} 

///////////////////////////////////////////////////////////////////////////////////
	
///////////////////////////////////// AIRSTRIKE ///////////////////////////////////

	if (featureAirStrike && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) s_message = false;

	if (featureAirStrike) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && s_message == false) {
			set_status_text("Press your ~g~ horn button ~w~ for an airstrike");
			s_message = true;
		}
		Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		Hash currVeh = ENTITY::GET_ENTITY_MODEL(playerVehicle);
		char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(currVeh);
		Hash veh_a = GAMEPLAY::GET_HASH_KEY(name);
		Vector3 minimum;
		Vector3 maximum;
		GAMEPLAY::GET_MODEL_DIMENSIONS(veh_a, &minimum, &maximum);
		Vector3 entitySCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerVehicle, 0.0, -maximum.y, 0.0);
		Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		Vector3 direction = RotationToDirection2(&Rot);
		direction.x = 1 * direction.x;
		direction.y = 1 * direction.y;
		STREAMING::HAS_NAMED_PTFX_ASSET_LOADED("scr_agencyheistb");
		STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_agencyheistb");
		GRAPHICS::_SET_PTFX_ASSET_NEXT_CALL("scr_agencyheistb");

		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 86) && airstrike == false && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
			nuke1 = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_anim"), entitySCoords.x, entitySCoords.y, entitySCoords.z + 100, false, false, false);
			ROPE::ACTIVATE_PHYSICS(nuke1);
			nuke_h1_coord = entitySCoords.z;
			nuke2 = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_anim"), entitySCoords.x + 2, entitySCoords.y + 2, entitySCoords.z + 90, false, false, false);
			ROPE::ACTIVATE_PHYSICS(nuke2);
			nuke_h2_coord = entitySCoords.z;
			nuke3 = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("prop_ld_bomb_anim"), entitySCoords.x - 2, entitySCoords.y - 2, entitySCoords.z + 110, false, false, false);
			ROPE::ACTIVATE_PHYSICS(nuke3);
			nuke_h3_coord = entitySCoords.z;
			airstrike = true;
		}

		if (airstrike == true) {
			Vector3 nuke1_coords = ENTITY::GET_ENTITY_COORDS(nuke1, true);
			nuke1_coords.z = nuke1_coords.z - 1;
			ENTITY::SET_ENTITY_COORDS(nuke1, nuke1_coords.x, nuke1_coords.y, nuke1_coords.z, 1, 0, 0, 1);
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke1_coords.x, nuke1_coords.y, nuke1_coords.z, 0.0f, 0.0f, 0.0f, 1.0f, false, false, false); // 0.5f 6.0f
			if (nuke1_coords.z - nuke_h1_coord < 5) { // ENTITY::GET_LAST_MATERIAL_HIT_BY_ENTITY(Nuke) != 0
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke1_coords.x, nuke1_coords.y, nuke1_coords.z, 0.0f, 0.0f, 0.0f, 25.0f, false, false, false); // 8.0f 6.0f
				FIRE::ADD_OWNED_EXPLOSION(playerPed, nuke1_coords.x, nuke1_coords.y, nuke1_coords.z, 29, 1500.0f, true, false, 2.0f); // 100.0f
			}
			Vector3 nuke2_coords = ENTITY::GET_ENTITY_COORDS(nuke2, true);
			nuke2_coords.z = nuke2_coords.z - 1;
			ENTITY::SET_ENTITY_COORDS(nuke2, nuke2_coords.x, nuke2_coords.y, nuke2_coords.z, 1, 0, 0, 1);
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke2_coords.x, nuke2_coords.y, nuke2_coords.z, 0.0f, 0.0f, 0.0f, 1.0f, false, false, false); // 0.5f 6.0f
			if (nuke2_coords.z - nuke_h2_coord < 5) { // ENTITY::GET_LAST_MATERIAL_HIT_BY_ENTITY(Nuke) != 0
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke2_coords.x, nuke2_coords.y, nuke2_coords.z, 0.0f, 0.0f, 0.0f, 25.0f, false, false, false); // 8.0f 6.0f
				FIRE::ADD_OWNED_EXPLOSION(playerPed, nuke2_coords.x, nuke2_coords.y, nuke2_coords.z, 29, 1500.0f, true, false, 2.0f); // 100.0f
			}
			Vector3 nuke3_coords = ENTITY::GET_ENTITY_COORDS(nuke3, true);
			nuke3_coords.z = nuke3_coords.z - 1;
			ENTITY::SET_ENTITY_COORDS(nuke3, nuke3_coords.x, nuke3_coords.y, nuke3_coords.z, 1, 0, 0, 1);
			GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke3_coords.x, nuke3_coords.y, nuke3_coords.z, 0.0f, 0.0f, 0.0f, 1.0f, false, false, false); // 0.5f 6.0f
			if (nuke3_coords.z - nuke_h3_coord < 5) { // ENTITY::GET_LAST_MATERIAL_HIT_BY_ENTITY(Nuke) != 0
				GRAPHICS::START_PARTICLE_FX_NON_LOOPED_AT_COORD("scr_agency3b_heli_expl", nuke3_coords.x, nuke3_coords.y, nuke3_coords.z, 0.0f, 0.0f, 0.0f, 25.0f, false, false, false); // 8.0f 6.0f
				FIRE::ADD_OWNED_EXPLOSION(playerPed, nuke3_coords.x, nuke3_coords.y, nuke3_coords.z, 29, 1500.0f, true, false, 2.0f); // 100.0f
				airstrike = false;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////// DROP ROAD SPIKES //////////////////////////////////

	if (featureDropSpikes && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) s_message = false;

	if (featureDropSpikes) {
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && s_message == false) {
			set_status_text("Press your ~g~ horn button ~w~ to deploy road spikes");
			s_message = true;
		}
		if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) playerVehicle_s = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if ((VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(playerVehicle_s)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(playerVehicle_s)) ||
			VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(playerVehicle_s))) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 86) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) { // horn 
			Hash currVeh_m = ENTITY::GET_ENTITY_MODEL(playerVehicle_s);
			char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(currVeh_m);
			Hash veh_h = GAMEPLAY::GET_HASH_KEY(name);
			Vector3 minimum;
			Vector3 maximum;
			GAMEPLAY::GET_MODEL_DIMENSIONS(veh_h, &minimum, &maximum);
			Vector3 entitySCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerVehicle_s, 0.0, -maximum.y, 0.0);
			Vector3 my_rot = ENTITY::GET_ENTITY_ROTATION(playerVehicle_s, 2);
			Object stinger = OBJECT::CREATE_OBJECT(GAMEPLAY::GET_HASH_KEY("p_ld_stinger_s"), entitySCoords.x, entitySCoords.y, entitySCoords.z, 1, true, 1);
			ENTITY::SET_ENTITY_ROTATION(stinger, my_rot.x, my_rot.y, my_rot.z - 90, 2, true);
			ROPE::ACTIVATE_PHYSICS(stinger);
			if (SPIKES.empty()) SPIKES.push_back(stinger);
			else SPIKES.push_back(stinger);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(stinger, true, true);
			STREAMING::REQUEST_ANIM_DICT("p_ld_stinger_s");
			while (!STREAMING::HAS_ANIM_DICT_LOADED("p_ld_stinger_s")) WAIT(0);
			AI::TASK_PLAY_ANIM(stinger, "p_ld_stinger_s", "p_stinger_s_idle_deployed", 8.0, 0.0, -1, 9, 0, 1, 1, 1);
			AI::STOP_ANIM_TASK(stinger, "p_ld_stinger_s", "p_stinger_s_idle_deployed", 1.0);
		}
		if (!SPIKES.empty() && SPIKES.size() > 20) {
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&SPIKES[0]);
			OBJECT::DELETE_OBJECT(&SPIKES[0]);
			SPIKES.erase(SPIKES.begin());
		}
		if (!SPIKES.empty()) {
			const int arrSize_sp = 1024;
			Vehicle surr_vehicles[arrSize_sp];
			int count_surr_sp = worldGetAllVehicles(surr_vehicles, arrSize_sp);
			for (int i = 0; i < count_surr_sp; i++) {
				if (surr_vehicles[i] != playerVehicle_s) {
					int t_b_lf = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(surr_vehicles[i], "wheel_lf"); // left front wheel
					Vector3 w_lf = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(surr_vehicles[i], t_b_lf);
					int t_b_lr = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(surr_vehicles[i], "wheel_lr"); // left rear wheel
					Vector3 w_lr = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(surr_vehicles[i], t_b_lr);
					int t_b_rf = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(surr_vehicles[i], "wheel_rf"); // right front wheel
					Vector3 w_rf = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(surr_vehicles[i], t_b_rf);
					int t_b_rr = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(surr_vehicles[i], "wheel_rr"); // right rear wheel
					Vector3 w_rr = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(surr_vehicles[i], t_b_rr);
					for (int j = 0; j < SPIKES.size(); j++) {
						VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(surr_vehicles[i], true);
						Vector3 spike_coords_l = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(SPIKES[j], 0.0, -0.9, 0.0); // -0.7
						Vector3 spike_coords_r = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(SPIKES[j], 0.0, 0.9, 0.0); // 0.7
						float dist_lf = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_lf.x, w_lf.y, w_lf.z, spike_coords_l.x, spike_coords_l.y, spike_coords_l.z, TRUE);
						float dist_lr = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_lr.x, w_lr.y, w_lr.z, spike_coords_l.x, spike_coords_l.y, spike_coords_l.z, TRUE);
						float dist_rf = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_rf.x, w_rf.y, w_rf.z, spike_coords_l.x, spike_coords_l.y, spike_coords_l.z, TRUE);
						float dist_rr = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_rr.x, w_rr.y, w_rr.z, spike_coords_l.x, spike_coords_l.y, spike_coords_l.z, TRUE);
						if (dist_lf < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 0, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 0, true, 1000.0);
						if (dist_lr < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 4, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 4, true, 1000.0);
						if (dist_rf < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 1, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 1, true, 1000.0);
						if (dist_rr < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 5, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 5, true, 1000.0);
						dist_lf = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_lf.x, w_lf.y, w_lf.z, spike_coords_r.x, spike_coords_r.y, spike_coords_r.z, TRUE);
						dist_lr = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_lr.x, w_lr.y, w_lr.z, spike_coords_r.x, spike_coords_r.y, spike_coords_r.z, TRUE);
						dist_rf = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_rf.x, w_rf.y, w_rf.z, spike_coords_r.x, spike_coords_r.y, spike_coords_r.z, TRUE);
						dist_rr = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(w_rr.x, w_rr.y, w_rr.z, spike_coords_r.x, spike_coords_r.y, spike_coords_r.z, TRUE);
						if (dist_lf < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 0, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 0, true, 1000.0);
						if (dist_lr < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 4, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 4, true, 1000.0);
						if (dist_rf < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 1, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 1, true, 1000.0);
						if (dist_rr < 1 && !VEHICLE::IS_VEHICLE_TYRE_BURST(surr_vehicles[i], 5, true)) VEHICLE::SET_VEHICLE_TYRE_BURST(surr_vehicles[i], 5, true, 1000.0);
					}
				}
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////////////

///////////////////////////// AUTOROLL DRIVER WINDOW WHEN SHOOT /////////////////////

	if (featureRollWhenShoot && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), -1) == playerPed) {
		if (CONTROLS::IS_CONTROL_PRESSED(2, 70) && VEHICLE::IS_VEHICLE_WINDOW_INTACT(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0)) {
			VEHICLE::ROLL_DOWN_WINDOW(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0);
			window_up = false;
		}
		if (!CONTROLS::IS_CONTROL_PRESSED(2, 70) && window_up == false) {
			VEHICLE::ROLL_UP_WINDOW(PED::GET_VEHICLE_PED_IS_IN(playerPed, false), 0);
			window_up = true;
		}
	}

///////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////// VEHICLE HYDRAULICS ////////////////////////////

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh))) && 
		VEH_HYDRAULICS_VALUES[HydraulicsIndex] != 0.0f) {
		Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle)) {
			if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, VEH_HYDRAULICS_VALUES[HydraulicsIndex], 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
			else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, VEH_HYDRAULICS_VALUES[HydraulicsIndex] / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
		}
	}

///////////////////////////////////////////////////////////////////////////////////

	// 'Reduced Grip If Snowing' and 'Slippery When Wet' code for bikes (mostly)
	Vector3 coords_slip = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] > 0 && featureSnow && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (((ENTITY::GET_ENTITY_SPEED(myVehicle) * 3.6) < 1) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 71) && !is_in_airbrake_mode()) {
			AI::TASK_VEHICLE_TEMP_ACTION(playerPed, myVehicle, 30, 700); // 500
			WAIT(800); // 400
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(myVehicle, 1); // 2
		}
		if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 1) { // arcade
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle) && VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(myVehicle))) {
				if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.15f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.15f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
			}
		}
		if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 2) { // realistic
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle) && VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(myVehicle))) {
				if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.18f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.18f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
			}
		}
	}
	if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripRainingIndex] > 0 && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)) {
		Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (((ENTITY::GET_ENTITY_SPEED(myVehicle) * 3.6) < 1) && CONTROLS::IS_CONTROL_JUST_PRESSED(2, 71) && !is_in_airbrake_mode() && (GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 1420204096 || GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 3061285535 || 
			GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 1840358669 || GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 3373937154)) {
			AI::TASK_VEHICLE_TEMP_ACTION(playerPed, myVehicle, 30, 400);
			WAIT(400);
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(myVehicle, 1);
		}
		if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripRainingIndex] == 1) { // arcade
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle) && VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(myVehicle))) {
				if (GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 1420204096) {
					if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.09f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
					else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.09f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				}
				if (GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 3061285535) {
					if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.10f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
					else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.10f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				}
			}
		}
		if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripRainingIndex] == 2) { // realistic
			if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle) && VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(myVehicle))) {
				if (GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 1420204096) {
					if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.11f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
					else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.11f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				}
				if (GAMEPLAY::GET_PREV_WEATHER_TYPE_HASH_NAME() == 3061285535) {
					if (!featureWorldMoonGravity) ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.12f, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
					else ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0.0, 0.0, 0.12f / 4, 0.0, 0.0, 0.0, 1, 1, 1, 1, 0, 1);
				}
			}
		}
	}

/////////////////////////////////// JUMPY VEHICLE /////////////////////////////////

	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && (VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh))) && 
		VEH_TURN_SIGNALS_ACCELERATION_VALUES[JumpyVehIndex] > 0) {
		Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(2, 22)) {
			veh_jumped_n = veh_jumped_n + 1;
			if (veh_jumped_n < 4) {
				ENTITY::APPLY_FORCE_TO_ENTITY(myVehicle, 1, 0, 0, VEH_TURN_SIGNALS_ACCELERATION_VALUES[JumpyVehIndex], 0, 0, 0, true, false, true, true, true, true);
				Vector3 curr_rot = ENTITY::GET_ENTITY_ROTATION(myVehicle, 1);
				ENTITY::SET_ENTITY_ROTATION(myVehicle, curr_rot.x, curr_rot.y, curr_rot.z, 1, 1);
			}
		}
		if (VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle)) veh_jumped_n = 0; // (veh_jumped_n > 3 && VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(myVehicle)) || 
	}

///////////////////////////////////////////////////////////////////////////////////

	if(bPlayerExists) {
		if(featureVehLightsOnUpdated || did_player_just_enter_vehicle(playerPed)){
			if(featureVehLightsOn){
				VEHICLE::SET_VEHICLE_LIGHTS(veh, 2); // 0 = normal, 1 = force off, 2 = forced on (visual_night), 3 = forced on (blink), 4 = forced off (blink), 5+ = normal
				featureVehLightsOnUpdated = false;
			}
			else{
				VEHICLE::SET_VEHICLE_LIGHTS(veh, 0);
				featureVehLightsOnUpdated = false;
			}
		}
	}

	// testing code; DO NOT DELETE
	//if(bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && IsKeyJustUp(KeyConfig::KEY_VEH_STOP)){
		//std::ofstream ofs("_colors.txt", std::ios::app | std::ios::out);
		//int primary, secondary, pearl, wheel, mod11, mod12, mod13, mod21, mod22;

		//VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
		//VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);
		//VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &mod11, &mod12, &mod13);
		//VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &mod21, &mod22);

		//ofs << primary << "\t" << secondary << "\t" << pearl << "\t" << wheel << "\t" << mod11 << "\t" << mod12 << "\t" << mod13 << "\t" << mod21 << "\t" << mod22 << "\t" << UI::_GET_LABEL_TEXT(VEHICLE::_GET_VEHICLE_MOD_COLOR_1_TEXT_LABEL(veh, false)) << "\t" << UI::_GET_LABEL_TEXT(VEHICLE::_GET_VEHICLE_MOD_COLOR_2_TEXT_LABEL(veh)) << "\n";

		//ofs.close();

		//std::ofstream ofs("_mods.txt", std::ios::app | std::ios::out);

		//VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		//for(int a = 0; a < 60; a++){
		//	ofs << a << "\t" << VEHICLE::GET_MOD_SLOT_NAME(veh, a) << "\t" << UI::_GET_LABEL_TEXT(VEHICLE::GET_MOD_SLOT_NAME(veh, a)) << "\n";
		//	for(int b = 0; b < VEHICLE::GET_NUM_VEHICLE_MODS(veh, a); b++){
		//		ofs << "\t" << b << "\t" << VEHICLE::GET_MOD_TEXT_LABEL(veh, a, b) << "\t" << UI::_GET_LABEL_TEXT(VEHICLE::GET_MOD_TEXT_LABEL(veh, a, b)) << "\n";
		//	}

		//	ofs.flush();
		//}

		//ofs.close();

		//std::ofstream ofs("_wheels.txt", std::ios::app | std::ios::out);

		//for(int a = 0; a < 10; a++){
		//	VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, a);
		//	ofs << a << "\n";
		//	for(int b = 0; b < VEHICLE::GET_NUM_VEHICLE_MODS(veh, 23); b++){
		//		ofs << "\t" << b << "\t" << VEHICLE::GET_MOD_TEXT_LABEL(veh, 23, b) << "\t" << UI::_GET_LABEL_TEXT(VEHICLE::GET_MOD_TEXT_LABEL(veh, 23, b)) << "\n";
		//	}
		//	ofs.flush();
		//}

		//ofs.close();
	//}
}

bool did_player_just_enter_vehicle(Ped playerPed){
	if(oldVehicleState == false && PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)){ // if we weren't in a car before, but we are now...
		oldVehicleState = true;
		return true;
	}
	return false;
}

void set_old_vehicle_state(bool updatedState){ // used by other functions, like teleporting into cars
	oldVehicleState = updatedState;
}

void vehicle_flip()
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Vehicle playerVehicle = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
	Vector3 orig_rot = ENTITY::GET_ENTITY_ROTATION(playerVehicle, 0);

	if (bPlayerExists && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
		ENTITY::SET_ENTITY_ROTATION(playerVehicle, orig_rot.x, orig_rot.y - 180, orig_rot.z, 0, true);
	}
}

void reset_vehicle_globals() {
	activeLineIndexSpeed = 0;
	activeLineIndexVisualize = 0;
	activeLineIndexSpeedlimit = 0;
	activeLineIndexFuel = 0;
	activeLineIndexEngineDegrade = 0;
	activeLineIndexRemember = 0;
	activeLineIndexRoadLaws = 0;

	FuelColours_R_Index = 26;
	FuelColours_G_Index = 18;
	FuelColours_B_Index = 12;
	turnSignalsIndex = 0;
	turnSignalsAngleIndex = 5;
	DoorAutolockIndex = 0;
	turnSignalsAccelerationIndex = 4;
	JumpyVehIndex = 0;
	HeavyVehIndex = 0;
	speedLimiterIndex = 0;
	speedCityLimiterIndex = 0;
	speedCountryLimiterIndex = 0;
	lightsOffIndex = 0;
	speedBoostIndex = 0;
	featureNeverDirty = 0;
	engPowMultIndex = 0;
	VehMassMultIndex = 0;
	current_player_forceshield = 0;
	InfiniteBoostIndex = 0;
	NitrousIndex = 0;
	SpeedSizeIndex = 0;
	SpeedPositionIndex = 0;

	FuelBlipsIndex = 0;
	IdleConsumptionIndex = 7;
	FuelBackground_Opacity_Index = 5;
	VehRememberIndex = 3;
	VehBlipSizeIndex = 2;
	VehBlipColourIndex = 4;
	VehColourIndex = 0;
	SpeedingCityIndex = 3;
	DetectionRangeIndex = 3;
	PirsuitRangeIndex = 4;
	StarsPunishIndex = 0;
	EngineRunningIndex = 0;
	AutoShutEngineIndex = 0;
	HydraulicsIndex = 0;
	VisLightIndex = 0;
	VisLight3dIndex = 0;
	SpeedingSpeedwayIndex = 5;
	FineSizeIndex = 1;
	VehBlipSymbolIndex = 0;
	VehBlipFlashIndex = 0;
	CarConsumptionIndex = 11;
	BikeConsumptionIndex = 12;
	BoatConsumptionIndex = 5;
	PlaneConsumptionIndex = 10;
	HeliConsumptionIndex = 9;
	RefuelingSpeedIndex = 6;
	FuelPriceIndex = 7;
	JerrycanPriceIndex = 12;
	Random1Index = 1;
	Random2Index = 1;
	BarPositionIndex = 0;
	
	CarEngineHealthIndex = 11;
	BikeEngineHealthIndex = 11;
	PlaneEngineHealthIndex = 11;
	HeliEngineHealthIndex = 11;
	BoatEngineHealthIndex = 11;
	RestorationSpeedIndex = 2;
	CarEngineDegradeIndex = 5;
	BikeEngineDegradeIndex = 5;
	PlaneEngineDegradeIndex = 5;
	HeliEngineDegradeIndex = 5;
	BoatEngineDegradeIndex = 5;

	featureSpeedOnFoot =
	featureKMH =
	featureAltitude =
	feature3rdpersonviewonly = 
	featureDaytimeonly =
	featureSpeedOnGround =
	featureSpeedInAir =
	
	featureVehInvincible =
		featureVehSpeedBoost =
		featureVehSteerAngle = 
		featureRollWhenShoot =
		featureTractionControl =
		featureSticktoground =
		featureDropSpikes =
		featureAirStrike =
		featureReverseWhenBraking =
		featureEngineRunning =
		featureNoVehFlip =
		featureAutoToggleLights =
		featureMileage = 
		featureSeasharkLights =
		featureRememberVehicles =
		featureRoadLaws =
		featureFuel = 
		featureBlips =
		featureBlipsPhone =
		featureVehMassMult =
		featureVehicleDoorInstant =
		featureLockVehicleDoors =
		featureVehSpawnInto = 
		featureNoVehFallOff =
		featureWearHelmetOff =
		featureEngineDegrade = 
		featureEngineHealthBar = 
		featureLimpMode = 
		featureVehSpawnTuned =
		featureVehSpawnOptic =
		featureAutoalarm =
		featureVehLightsOn = false;

	featureLockVehicleDoorsUpdated = false;
	featureDeleteTrackedVehicles_CharacterChanged = true;
		featureBlipNumber = true;
		featureHazards = true;
		featureWearHelmetOffUpdated = true;
		featureVehInvincibleUpdated = true;
		featureDeleteTrackedVehicles = true;
		featurePoliceVehicleBlip = true;
		featurePoliceNoFlip = true;
		featurePoliceNoDamage = true;
		featureCopsUseRadio = false;
		featureRunningRedLight = true;
		featurePavementDriving = true;
		featureDrivingAgainstTraffic = true;
		featureCarCollision = true;
		featureUsingMobilePhone = true;
		featureVehicleHeavilyDamaged = true;
		featureNoHelmetOnBike = true;
		featureStolenVehicle = true;
		featureNoLightsNightTime = true;
		featureEscapingPolice = true;
		featureVehLightsOnUpdated = true;

	featureDespawnScriptDisabled = false;
	featureDespawnScriptDisabledUpdated = true;
	featureDespawnScriptDisabledWasLastOn = false;

	featureVehNoDamage = false;
	featureVehInvulnIncludesCosmetic = false;
}

bool onconfirm_carspawn_menu(MenuItem<int> choice){
	if (choice.value == MENU_VEHICLE_CATEGORIES.size() - 1){
		// custom spawn
		std::string result = show_keyboard(NULL, (char*) lastCustomVehicleSpawn.c_str());
		if (!result.empty()){
			result = trim(result);
			lastCustomVehicleSpawn = result;
			Hash hash = GAMEPLAY::GET_HASH_KEY((char*) result.c_str());
			if (lastCustomVehicleSpawn != "random" && lastCustomVehicleSpawn != "Random" && lastCustomVehicleSpawn != "RANDOM" && (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_A_VEHICLE(hash))){
				std::ostringstream ss;
				ss << "~r~Error: Couldn't find model " << result;
				set_status_text(ss.str());
				return false;
			}
			if (lastCustomVehicleSpawn == "random" || lastCustomVehicleSpawn == "Random" || lastCustomVehicleSpawn == "RANDOM" || (STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_A_VEHICLE(hash))) {
				// random vehicle
				if (lastCustomVehicleSpawn == "random" || lastCustomVehicleSpawn == "Random" || lastCustomVehicleSpawn == "RANDOM") {
					int random_category, random_veh = -1;
					random_category = (rand() % 20 + 0); // UP MARGIN + DOWN MARGIN
					if (random_category == 14) random_category = 13;
					if (random_category < 9) {
						random_veh = (rand() % VOV_CAR_VALUES[random_category].size() + 0);
						result = VOV_CAR_VALUES[random_category][random_veh];
					}
					if (random_category > 8 && random_category < 15) {
						random_category = random_category - 9;
						random_veh = (rand() % VOV_INDUS_VALUES[random_category].size() + 0);
						result = VOV_INDUS_VALUES[random_category][random_veh];
					}
					if (random_category > 14 && random_category < 21) {
						random_category = random_category - 15;
						random_veh = (rand() % VOV_SHALLOW_VALUES[random_category].size() + 0);
						result = VOV_SHALLOW_VALUES[random_category][random_veh];
					}
				}
				//
				do_spawn_vehicle(result, result);
			}
		}
		return false;
	}

	switch(choice.value){
		case 0:
			process_spawn_menu_cars();
			break;
		case 1:
			process_spawn_menu_indus();
			break;
		default:
			process_spawn_menu_generic(choice.value);
			break;
	}
	return false;
}

bool process_carspawn_menu() {
	std::vector<MenuItem<int>*> menuItems;
	for(int i = 0; i < MENU_VEHICLE_CATEGORIES.size(); i++){
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_VEHICLE_CATEGORIES[i];
		item->value = i;
		item->isLeaf = (i == MENU_VEHICLE_CATEGORIES.size() - 1);
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Vehicle Categories", onconfirm_carspawn_menu, NULL, NULL);
}

bool onconfirm_spawn_menu_cars(MenuItem<int> choice){
	std::string category = choice.caption;

	std::vector<MenuItem<std::string>*> menuItems;
	for(int i = 0; i < VOV_CAR_VALUES[choice.value].size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_CAR_CAPTIONS[choice.value][i];
		item->value = VOV_CAR_VALUES[choice.value][i];
		menuItems.push_back(item);
	}

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
}

bool process_spawn_menu_cars(){
	std::vector<MenuItem<int>*> menuItems;
	for(int i = 0; i < MENU_CAR_CATEGORIES.size(); i++){
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_CAR_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Car Categories", onconfirm_spawn_menu_cars, NULL, NULL);
}

bool onconfirm_spawn_menu_indus(MenuItem<int> choice){
	int selection = choice.value;

	std::string category;
	if(choice.value == 3){
		category = "Service And Utility";
	}
	else if(choice.value == 5){
		category = "Trains";
	}
	else{
		category = choice.caption;
	}

	std::vector<MenuItem<std::string>*> menuItems;
	for(int i = 0; i < VOV_INDUS_CAPTIONS[selection].size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_INDUS_CAPTIONS[selection][i];
		item->value = VOV_INDUS_VALUES[selection][i];
		menuItems.push_back(item);
	}

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
}

bool process_spawn_menu_indus(){
	std::vector<MenuItem<int>*> menuItems;
	for(int i = 0; i < MENU_INDUS_CATEGORIES.size(); i++){
		MenuItem<int> *item = new MenuItem<int>();
		item->caption = MENU_INDUS_CATEGORIES[i];
		item->value = i;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, 0, "Industrial Categories", onconfirm_spawn_menu_indus, NULL, NULL);
}

bool onconfirm_spawn_menu_vehicle_selection(MenuItem<std::string> choice){
	do_spawn_vehicle(choice.value, choice.caption);
	return false;
}

bool process_spawn_menu_generic(int topMenuSelection){
	int selection = topMenuSelection - 2;

	std::string category = MENU_VEHICLE_CATEGORIES[topMenuSelection];

	std::vector<MenuItem<std::string>*> menuItems;
	for(int i = 0; i < VOV_SHALLOW_CAPTIONS[selection].size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = VOV_SHALLOW_CAPTIONS[selection][i];
		item->value = VOV_SHALLOW_VALUES[selection][i];
		menuItems.push_back(item);
	}

	MenuParameters<std::string> params(menuItems, category);
	params.menuSelectionPtr = 0;
	params.onConfirmation = onconfirm_spawn_menu_vehicle_selection;
	params.lineImageProvider = vehicle_image_preview_finder;

	return draw_generic_menu<std::string>(params);
}

bool do_spawn_vehicle(std::string modelName, std::string modelTitle){
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *) modelName.c_str());
	Vehicle veh = do_spawn_vehicle(model, modelTitle, true);
	if(veh != -1){
		return true;
	}
	return false;
}

Vehicle do_spawn_vehicle(DWORD model, std::string modelTitle, bool cleanup){
	if(STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model)){
		STREAMING::REQUEST_MODEL(model);
		while(!STREAMING::HAS_MODEL_LOADED(model)){
			make_periodic_feature_call();
			WAIT(0);
		}

		Vector3 minDimens;
		Vector3 maxDimens;
		GAMEPLAY::GET_MODEL_DIMENSIONS(model, &minDimens, &maxDimens);
		float spawnOffY = max(5.0f, 2.0f + 0.5f * (maxDimens.y - minDimens.y));

		FLOAT lookDir = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, spawnOffY, 0.0);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, lookDir, 1, 0);

		//if we're mid-air, don't put it on the ground
		if(!ENTITY::IS_ENTITY_IN_AIR(PLAYER::PLAYER_PED_ID())){
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);
		}

		if(featureVehSpawnTuned){
			fully_tune_vehicle(veh, featureVehSpawnOptic);
		}

		if(featureVehSpawnInto){
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
			oldVehicleState = false; // set old vehicle state to false since we changed cars but didn't actually exit the last one

			if(is_this_a_heli_or_plane(veh)){
				VEHICLE::SET_HELI_BLADES_FULL_SPEED(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
			}
		}

		if (!featureVehSpawnInto && (ENTITY::GET_ENTITY_MODEL(veh) == GAMEPLAY::GET_HASH_KEY("MINITANK") || ENTITY::GET_ENTITY_MODEL(veh) == GAMEPLAY::GET_HASH_KEY("RCBANDITO"))) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
			oldVehicleState = false;
		}

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, 0.0f);

		WAIT(0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		if(cleanup){
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
		}

		std::ostringstream ss;
		ss << modelTitle << " spawned";
		set_status_text(ss.str());

		return veh;
	}
	return -1;
}

void add_vehicle_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featureNoVehFallOff", &featureNoVehFallOff}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureVehicleDoorInstant", &featureVehicleDoorInstant});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehInvincible", &featureVehInvincible, &featureVehInvincibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehNoDamage", &featureVehNoDamage, &featureVehInvincibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehSpawnInto", &featureVehSpawnInto});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehSpeedBoost", &featureVehSpeedBoost});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehSteerAngle", &featureVehSteerAngle});
	results->push_back(FeatureEnabledLocalDefinition{"featureRollWhenShoot", &featureRollWhenShoot});
	results->push_back(FeatureEnabledLocalDefinition{"featureTractionControl", &featureTractionControl});
	results->push_back(FeatureEnabledLocalDefinition{"featureSticktoground", &featureSticktoground});
	results->push_back(FeatureEnabledLocalDefinition{"featureDropSpikes", &featureDropSpikes});
	results->push_back(FeatureEnabledLocalDefinition{"featureAirStrike", &featureAirStrike});
	results->push_back(FeatureEnabledLocalDefinition{"featureReverseWhenBraking", &featureReverseWhenBraking});
	results->push_back(FeatureEnabledLocalDefinition{"featureEngineRunning", &featureEngineRunning});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoVehFlip", &featureNoVehFlip});
	results->push_back(FeatureEnabledLocalDefinition{"featureAutoToggleLights", &featureAutoToggleLights});
	results->push_back(FeatureEnabledLocalDefinition{"featureMileage", &featureMileage});
	results->push_back(FeatureEnabledLocalDefinition{"featureSeasharkLights", &featureSeasharkLights});
	results->push_back(FeatureEnabledLocalDefinition{"featureRememberVehicles", &featureRememberVehicles});
	results->push_back(FeatureEnabledLocalDefinition{"featureRoadLaws", &featureRoadLaws});
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceVehicleBlip", &featurePoliceVehicleBlip});
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceNoFlip", &featurePoliceNoFlip});
	results->push_back(FeatureEnabledLocalDefinition{"featurePoliceNoDamage", &featurePoliceNoDamage});
	results->push_back(FeatureEnabledLocalDefinition{"featureCopsUseRadio", &featureCopsUseRadio});
	results->push_back(FeatureEnabledLocalDefinition{"featureRunningRedLight", &featureRunningRedLight});
	results->push_back(FeatureEnabledLocalDefinition{"featurePavementDriving", &featurePavementDriving});
	results->push_back(FeatureEnabledLocalDefinition{"featureDrivingAgainstTraffic", &featureDrivingAgainstTraffic});
	results->push_back(FeatureEnabledLocalDefinition{"featureCarCollision", &featureCarCollision});
	results->push_back(FeatureEnabledLocalDefinition{"featureUsingMobilePhone", &featureUsingMobilePhone});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehicleHeavilyDamaged", &featureVehicleHeavilyDamaged});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoHelmetOnBike", &featureNoHelmetOnBike});
	results->push_back(FeatureEnabledLocalDefinition{"featureStolenVehicle", &featureStolenVehicle});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoLightsNightTime", &featureNoLightsNightTime});
	results->push_back(FeatureEnabledLocalDefinition{"featureEscapingPolice", &featureEscapingPolice});
	results->push_back(FeatureEnabledLocalDefinition{"featureDeleteTrackedVehicles", &featureDeleteTrackedVehicles});
	results->push_back(FeatureEnabledLocalDefinition{"featureDeleteTrackedVehicles_CharacterChanged", &featureDeleteTrackedVehicles_CharacterChanged});
	results->push_back(FeatureEnabledLocalDefinition{"featureBlipNumber", &featureBlipNumber});
	results->push_back(FeatureEnabledLocalDefinition{"featureAutoalarm", &featureAutoalarm});
	results->push_back(FeatureEnabledLocalDefinition{"featureFuel", &featureFuel});
	results->push_back(FeatureEnabledLocalDefinition{"featureBlips", &featureBlips});
	results->push_back(FeatureEnabledLocalDefinition{"featureBlipsPhone", &featureBlipsPhone});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehMassMult", &featureVehMassMult});
	results->push_back(FeatureEnabledLocalDefinition{"featureSpeedOnFoot", &featureSpeedOnFoot});
	results->push_back(FeatureEnabledLocalDefinition{"featureKMH", &featureKMH});
	results->push_back(FeatureEnabledLocalDefinition{"feature3rdpersonviewonly", &feature3rdpersonviewonly});
	results->push_back(FeatureEnabledLocalDefinition{"featureDaytimeonly", &featureDaytimeonly});
	results->push_back(FeatureEnabledLocalDefinition{"featureHazards", &featureHazards});
	results->push_back(FeatureEnabledLocalDefinition{"featureAltitude", &featureAltitude });
	results->push_back(FeatureEnabledLocalDefinition{"featureSpeedOnGround", &featureSpeedOnGround });
	results->push_back(FeatureEnabledLocalDefinition{"featureSpeedInAir", &featureSpeedInAir });
	results->push_back(FeatureEnabledLocalDefinition{"featureVehSpawnTuned", &featureVehSpawnTuned});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehSpawnOptic", &featureVehSpawnOptic});
	results->push_back(FeatureEnabledLocalDefinition{"featureWearHelmetOff", &featureWearHelmetOff, &featureWearHelmetOffUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehInvulnIncludesCosmetic", &featureVehInvulnIncludesCosmetic, &featureVehInvincibleUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureDespawnScriptDisabled", &featureDespawnScriptDisabled, &featureDespawnScriptDisabledUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureVehLightsOn", &featureVehLightsOn, &featureVehLightsOnUpdated});
	results->push_back(FeatureEnabledLocalDefinition{"featureEngineDegrade", &featureEngineDegrade});
	results->push_back(FeatureEnabledLocalDefinition{"featureEngineHealthBar", &featureEngineHealthBar});
	results->push_back(FeatureEnabledLocalDefinition{"featureLimpMode", &featureLimpMode});
}

bool spawn_saved_car(int slot, std::string caption){
	ENTDatabase* database = get_database();

	std::vector<SavedVehicleDBRow*> savedVehs = database->get_saved_vehicles(slot);

	lastKnownSavedVehicleCount = savedVehs.size();

	SavedVehicleDBRow* savedVeh = savedVehs.at(0);
	database->populate_saved_vehicle(savedVeh);

	Vehicle veh = do_spawn_vehicle(savedVeh->model, caption, false);
	if(veh == -1){
		set_status_text("Spawn failed");
	}
	else{
		VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);

		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, (savedVeh->burstableTyres == 1) ? TRUE : FALSE);

		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh, savedVeh->plateType);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(veh, (char*) savedVeh->plateText.c_str());

		VEHICLE::SET_VEHICLE_WINDOW_TINT(veh, savedVeh->windowTint); 

		VEHICLE::SET_VEHICLE_WHEEL_TYPE(veh, savedVeh->wheelType);
		
		for each (SavedVehicleExtraDBRow *extra in savedVeh->extras){
			VEHICLE::SET_VEHICLE_EXTRA(veh, extra->extraID, (extra->extraState == 1) ? 0 : -1);
		}

		for each (SavedVehicleModDBRow *mod in savedVeh->mods){
			if(mod->isToggle){
				VEHICLE::TOGGLE_VEHICLE_MOD(veh, mod->modID, mod->modState);
			}
			else{
				VEHICLE::SET_VEHICLE_MOD(veh, mod->modID, mod->modState, 0);
			}
		}

		int currmod = VEHICLE::GET_VEHICLE_MOD(veh, 23);
		VEHICLE::SET_VEHICLE_MOD(veh, 23, currmod, savedVeh->customTyres);
		VEHICLE::SET_VEHICLE_MOD(veh, 24, currmod, savedVeh->customTyres);

		if(savedVeh->livery != -1){
			VEHICLE::SET_VEHICLE_LIVERY(veh, savedVeh->livery);
		}

		VEHICLE::SET_VEHICLE_MOD_COLOR_1(veh, savedVeh->colourMod1Type, savedVeh->colourMod1Colour, savedVeh->colourMod1P3);
		VEHICLE::SET_VEHICLE_MOD_COLOR_2(veh, savedVeh->colourMod2Type, savedVeh->colourMod2Colour);
		VEHICLE::SET_VEHICLE_COLOURS(veh, savedVeh->colourPrimary, savedVeh->colourSecondary);
		VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, savedVeh->colourExtraPearl, savedVeh->colourExtraWheel);

		if(savedVeh->colourCustom1RGB[0] != -1 &&
		   savedVeh->colourCustom1RGB[1] != -1 &&
		   savedVeh->colourCustom1RGB[2] != -1){
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, savedVeh->colourCustom1RGB[0], savedVeh->colourCustom1RGB[1], savedVeh->colourCustom1RGB[2]);
		}

		if(savedVeh->colourCustom2RGB[0] != -1 &&
		   savedVeh->colourCustom2RGB[1] != -1 &&
		   savedVeh->colourCustom2RGB[2] != -1){
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, savedVeh->colourCustom2RGB[0], savedVeh->colourCustom2RGB[1], savedVeh->colourCustom2RGB[2]);
		}

		VEHICLE::SET_VEHICLE_ENVEFF_SCALE(veh, savedVeh->fadeLevel);

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(veh, savedVeh->dirtLevel);

		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 0, savedVeh->neonEnablement[0] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 1, savedVeh->neonEnablement[1] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 2, savedVeh->neonEnablement[2] == 1);
		VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(veh, 3, savedVeh->neonEnablement[3] == 1);

		VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(veh, savedVeh->neonRGB[0], savedVeh->neonRGB[1], savedVeh->neonRGB[2]);

		if(savedVeh->tyreSmokeRGB[0] != -1 &&
		   savedVeh->tyreSmokeRGB[1] != -1 &&
		   savedVeh->tyreSmokeRGB[2] != -1){
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, savedVeh->tyreSmokeRGB[0], savedVeh->tyreSmokeRGB[1], savedVeh->tyreSmokeRGB[2]);
		}

		if(VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, 0)){
			if(savedVeh->convertibleRoofUp){
				VEHICLE::SET_CONVERTIBLE_ROOF(veh, 1);
			}
			else{
				VEHICLE::SET_CONVERTIBLE_ROOF(veh, 0);
			}
		}

		if(savedVeh->dashboardColour != -1){
			VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(veh, savedVeh->dashboardColour);
		}
		if(savedVeh->interiorColour != -1){
			VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(veh, savedVeh->interiorColour);
		}

		// Loading of an engine sound
		if (savedVeh->engineSound > -1 && featureEngineSound) {
			bool correct_name_to_load = false;
			for (int i = 0; i < ENGINE_SOUND_COUNT_VEHICLES; i++)
			{
				if (ENGINE_SOUND_NUMBERS[i] == savedVeh->engineSound) {
					correct_name_to_load = true;
					current_picked_engine_sound = i;
				}
			}
			if (correct_name_to_load == true) {
				char *currSound = new char[ENGINE_SOUND[current_picked_engine_sound].length() + 1];
				strcpy(currSound, ENGINE_SOUND[current_picked_engine_sound].c_str());
				VEHICLE::SET_VEHICLE_MOD_KIT(veh, 0);
				AUDIO::_SET_VEHICLE_AUDIO(veh, currSound);
				current_picked_engine_sound = -1;
			}
		}
		//
		if (savedVeh->xenonColour > -1) {
			VEHICLE::SET_VEHICLE_XENON_COLOUR(veh, savedVeh->xenonColour); 
		}
	
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
	}

	for(std::vector<SavedVehicleDBRow*>::iterator it = savedVehs.begin(); it != savedVehs.end(); ++it){
		delete (*it);
	}
	savedVehs.clear();

	return false;
}

bool onconfirm_savedveh_slot_menu(MenuItem<int> choice){
	switch(choice.value){
		case 1: //spawn
			spawn_saved_car(activeSavedVehicleIndex, activeSavedVehicleSlotName);
			break;
		case 2: //overwrite
		{
			save_current_vehicle(activeSavedVehicleIndex);
			requireRefreshOfVehSaveSlots = true;
			requireRefreshOfVehSlotMenu = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
		case 3: //rename
		{
			std::string result = show_keyboard(NULL, (char*) activeSavedVehicleSlotName.c_str());
			if(!result.empty()){
				ENTDatabase* database = get_database();
				database->rename_saved_vehicle(result, activeSavedVehicleIndex);

				activeSavedVehicleSlotName = result;
			}
			requireRefreshOfVehSaveSlots = true;
			requireRefreshOfVehSlotMenu = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
		case 4: //delete
		{
			ENTDatabase* database = get_database();
			database->delete_saved_vehicle(activeSavedVehicleIndex);

			requireRefreshOfVehSlotMenu = false;
			requireRefreshOfVehSaveSlots = true;
			vehSaveSlotMenuInterrupt = true;
			vehSaveMenuInterrupt = true;
		}
		break;
	/*case 5: // testing
		ENTDatabase *database = get_database();
		auto tmp = database->get_saved_vehicles(activeSavedVehicleIndex);
		set_status_text(std::to_string(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(tmp.at(0)->model)));

		break;*/
	}
	return false;
}

bool process_savedveh_slot_menu(int slot){
	do{
		vehSaveSlotMenuInterrupt = false;
		requireRefreshOfVehSlotMenu = false;

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 1;
		item->caption = "Spawn";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 2;
		item->caption = "Overwrite With Current";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 3;
		item->caption = "Rename";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 4;
		item->caption = "Delete";
		menuItems.push_back(item);

		/*item = new MenuItem<int>();
		item->caption = "Display Type";
		item->value = 5;
		item->isLeaf = true;
		menuItems.push_back(item);*/

		draw_generic_menu<int>(menuItems, 0, activeSavedVehicleSlotName, onconfirm_savedveh_slot_menu, NULL, NULL, vehicle_save_slot_menu_interrupt);
	}
	while(requireRefreshOfVehSlotMenu);
	return false;
}

bool onconfirm_savedveh_sort_menu(MenuItem<int> choice){
	savedVehicleListSortMethod = choice.value;
	requireRefreshOfVehSaveSlots = vehSaveMenuInterrupt = vehSaveSortMenuInterrupt = true;

	return false;
}

bool process_savedveh_sort_menu(){
	std::vector<MenuItem<int> *> menuItems;
	int method = 0;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "By Save Order (Default)";
	item->value = method++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "By Saved Name";
	item->value = method++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "By Class, then Saved Name";
	item->value = method++;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, nullptr, "Sort Saved Vehicles List", onconfirm_savedveh_sort_menu, nullptr, nullptr, vehicle_save_sort_menu_interrupt);
}

void save_current_vehicle(int slot){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(bPlayerExists){
		if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			std::ostringstream ss;
			
			Hash currVehModelS = ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
			if (slot == -1 && STREAMING::IS_MODEL_IN_CDIMAGE(currVehModelS) && STREAMING::IS_MODEL_A_VEHICLE(currVehModelS) && STREAMING::IS_MODEL_VALID(currVehModelS)) {
				//Hash vehVal = GAMEPLAY::GET_HASH_KEY(currVehModel);
				char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(currVehModelS);
				std::string displayName = UI::_GET_LABEL_TEXT(name);
				ss << displayName;
			}
			if (slot == -1 && !STREAMING::IS_MODEL_IN_CDIMAGE(currVehModelS)) { //  && !STREAMING::IS_MODEL_A_VEHICLE(currVehModelS) && !STREAMING::IS_MODEL_VALID(currVehModelS)
				ss << "Saved Vehicle " << (lastKnownSavedVehicleCount + 1);
			}
			if(slot != -1){
				ss << activeSavedVehicleSlotName;
			}
						
			auto existingText = ss.str();
			std::string result = show_keyboard(NULL, (char*) existingText.c_str());
			if(!result.empty()){
				ENTDatabase* database = get_database();
				if(database->save_vehicle(veh, result, slot)){
					set_status_text("Vehicle saved");
					activeSavedVehicleSlotName = result;
				}
				else{
					set_status_text("Save error");
				}
			}
		}
		else{
			set_status_text("Player isn't in a vehicle");
		}
	}
}

bool onconfirm_savedveh_menu(MenuItem<int> choice){
	if(choice.value == -1){
		save_current_vehicle(-1);
		requireRefreshOfVehSaveSlots = true;
		vehSaveMenuInterrupt = true;
		return false;
	}

	if(choice.value == -2){
		return process_savedveh_sort_menu();
	}

	activeSavedVehicleIndex = choice.value;
	activeSavedVehicleSlotName = choice.caption;

	return process_savedveh_slot_menu(choice.value);
}

bool process_savedveh_menu(){
	do{
		vehSaveMenuInterrupt = false;
		requireRefreshOfVehSlotMenu = false;
		requireRefreshOfVehSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedVehicleDBRow*> savedVehs = database->get_saved_vehicles();

		lastKnownSavedVehicleCount = savedVehs.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = -1;
		item->caption = "Create New Vehicle Save";
		item->sortval = -2;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Sort Saved Vehicles";
		item->value = -2;
		item->isLeaf = false;
		item->sortval = -1;
		menuItems.push_back(item);

		for each (SavedVehicleDBRow *sv in savedVehs){
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			switch(savedVehicleListSortMethod){
				case 0:
					item->sortval = sv->rowID;
					break;
				case 1:
					item->sortval = 0;
					break;
				case 2:
					item->sortval = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(sv->model);
					break;
				default:
					item->sortval = 0;
					break;
			}
			menuItems.push_back(item);
		}

		switch(savedVehicleListSortMethod){
			case 0:
				std::stable_sort(menuItems.begin(), menuItems.end(),
								 [](const MenuItem<int> *a, const MenuItem<int> *b) -> bool{
					return a->sortval < b->sortval;
				});
				break;
			case 1:
				std::stable_sort(menuItems.begin(), menuItems.end(),
								 [](const MenuItem<int> *a, const MenuItem<int> *b) -> bool{
					return a->sortval == b->sortval ? a->caption < b->caption : (a->sortval < b->sortval);
				});
				break;
			case 2:
				std::stable_sort(menuItems.begin(), menuItems.end(),
								 [](const MenuItem<int> *a, const MenuItem<int> *b) -> bool{
					return a->sortval == b->sortval ? a->caption < b->caption : (a->sortval < b->sortval);
				});
				break;
			default:
				break;
		}

		draw_generic_menu<int>(menuItems, 0, "Saved Vehicles", onconfirm_savedveh_menu, NULL, NULL, vehicle_save_menu_interrupt);

		for(std::vector<SavedVehicleDBRow*>::iterator it = savedVehs.begin(); it != savedVehs.end(); ++it){
			delete (*it);
		}
		savedVehs.clear();
	}
	while(requireRefreshOfVehSaveSlots);

	return false;
}

bool vehicle_save_menu_interrupt(){
	if(vehSaveMenuInterrupt){
		vehSaveMenuInterrupt = false;
		return true;
	}
	return false;
}

bool vehicle_save_sort_menu_interrupt(){
	return vehSaveSortMenuInterrupt ? vehSaveSortMenuInterrupt = false, true : false;
}

bool vehicle_save_slot_menu_interrupt(){
	if(vehSaveSlotMenuInterrupt){
		vehSaveSlotMenuInterrupt = false;
		return true;
	}
	return false;
}

void add_vehicle_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"lastCustomVehicleSpawn", lastCustomVehicleSpawn});
	results->push_back(StringPairSettingDBRow{"speedBoostIndex", std::to_string(speedBoostIndex)});
	results->push_back(StringPairSettingDBRow{"savedVehicleListSortMethod", std::to_string(savedVehicleListSortMethod)});
	results->push_back(StringPairSettingDBRow{"featureNeverDirty", std::to_string(featureNeverDirty)});
	results->push_back(StringPairSettingDBRow{"engPowMultIndex", std::to_string(engPowMultIndex)});
	results->push_back(StringPairSettingDBRow{"VehMassMultIndex", std::to_string(VehMassMultIndex)});
	results->push_back(StringPairSettingDBRow{"current_player_forceshield", std::to_string(current_player_forceshield)});
	results->push_back(StringPairSettingDBRow{"InfiniteBoostIndex", std::to_string(InfiniteBoostIndex)});
	results->push_back(StringPairSettingDBRow{"NitrousIndex", std::to_string(NitrousIndex)});
	results->push_back(StringPairSettingDBRow{"TurnSignalsIndex", std::to_string(turnSignalsIndex)});
	results->push_back(StringPairSettingDBRow{"turnSignalsAngleIndex", std::to_string(turnSignalsAngleIndex)});
	results->push_back(StringPairSettingDBRow{"DoorAutolockIndex", std::to_string(DoorAutolockIndex)});
	results->push_back(StringPairSettingDBRow{"turnSignalsAccelerationIndex", std::to_string(turnSignalsAccelerationIndex)});
	results->push_back(StringPairSettingDBRow{"JumpyVehIndex", std::to_string(JumpyVehIndex)});
	results->push_back(StringPairSettingDBRow{"HeavyVehIndex", std::to_string(HeavyVehIndex)});
	results->push_back(StringPairSettingDBRow{"speedLimiterIndex", std::to_string(speedLimiterIndex)});
	results->push_back(StringPairSettingDBRow{"speedCityLimiterIndex", std::to_string(speedCityLimiterIndex)});
	results->push_back(StringPairSettingDBRow{"speedCountryLimiterIndex", std::to_string(speedCountryLimiterIndex)});
	results->push_back(StringPairSettingDBRow{"lightsOffIndex", std::to_string(lightsOffIndex)});
	results->push_back(StringPairSettingDBRow{"SpeedSizeIndex", std::to_string(SpeedSizeIndex)});
	results->push_back(StringPairSettingDBRow{"SpeedPositionIndex", std::to_string(SpeedPositionIndex)});
	results->push_back(StringPairSettingDBRow{"FuelBlipsIndex", std::to_string(FuelBlipsIndex)});
	results->push_back(StringPairSettingDBRow{"IdleConsumptionIndex", std::to_string(IdleConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"FuelBackground_Opacity_Index", std::to_string(FuelBackground_Opacity_Index)});
	results->push_back(StringPairSettingDBRow{"VehRememberIndex", std::to_string(VehRememberIndex)});
	results->push_back(StringPairSettingDBRow{"VehBlipSizeIndex", std::to_string(VehBlipSizeIndex)});
	results->push_back(StringPairSettingDBRow{"VehBlipColourIndex", std::to_string(VehBlipColourIndex)});
	results->push_back(StringPairSettingDBRow{"VehColourIndex", std::to_string(VehColourIndex)});
	results->push_back(StringPairSettingDBRow{"SpeedingCityIndex", std::to_string(SpeedingCityIndex)});
	results->push_back(StringPairSettingDBRow{"DetectionRangeIndex", std::to_string(DetectionRangeIndex)});
	results->push_back(StringPairSettingDBRow{"PirsuitRangeIndex", std::to_string(PirsuitRangeIndex)});
	results->push_back(StringPairSettingDBRow{"StarsPunishIndex", std::to_string(StarsPunishIndex)});
	results->push_back(StringPairSettingDBRow{"EngineRunningIndex", std::to_string(EngineRunningIndex)});
	results->push_back(StringPairSettingDBRow{"AutoShutEngineIndex", std::to_string(AutoShutEngineIndex)});
	results->push_back(StringPairSettingDBRow{"HydraulicsIndex", std::to_string(HydraulicsIndex)});
	results->push_back(StringPairSettingDBRow{"VisLightIndex", std::to_string(VisLightIndex)});
	results->push_back(StringPairSettingDBRow{"VisLight3dIndex", std::to_string(VisLight3dIndex)});
	results->push_back(StringPairSettingDBRow{"SpeedingSpeedwayIndex", std::to_string(SpeedingSpeedwayIndex)});
	results->push_back(StringPairSettingDBRow{"FineSizeIndex", std::to_string(FineSizeIndex)});
	results->push_back(StringPairSettingDBRow{"VehBlipSymbolIndex", std::to_string(VehBlipSymbolIndex)});
	results->push_back(StringPairSettingDBRow{"VehBlipFlashIndex", std::to_string(VehBlipFlashIndex)});
	results->push_back(StringPairSettingDBRow{"CarConsumptionIndex", std::to_string(CarConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"BikeConsumptionIndex", std::to_string(BikeConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"BoatConsumptionIndex", std::to_string(BoatConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"PlaneConsumptionIndex", std::to_string(PlaneConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"HeliConsumptionIndex", std::to_string(HeliConsumptionIndex)});
	results->push_back(StringPairSettingDBRow{"CarEngineHealthIndex", std::to_string(CarEngineHealthIndex)});
	results->push_back(StringPairSettingDBRow{"BikeEngineHealthIndex", std::to_string(BikeEngineHealthIndex)});
	results->push_back(StringPairSettingDBRow{"BoatEngineHealthIndex", std::to_string(BoatEngineHealthIndex)});
	results->push_back(StringPairSettingDBRow{"PlaneEngineHealthIndex", std::to_string(PlaneEngineHealthIndex)});
	results->push_back(StringPairSettingDBRow{"HeliEngineHealthIndex", std::to_string(HeliEngineHealthIndex)});
	results->push_back(StringPairSettingDBRow{"CarEngineDegradeIndex", std::to_string(CarEngineDegradeIndex)});
	results->push_back(StringPairSettingDBRow{"BikeEngineDegradeIndex", std::to_string(BikeEngineDegradeIndex)});
	results->push_back(StringPairSettingDBRow{"BoatEngineDegradeIndex", std::to_string(BoatEngineDegradeIndex)});
	results->push_back(StringPairSettingDBRow{"PlaneEngineDegradeIndex", std::to_string(PlaneEngineDegradeIndex)});
	results->push_back(StringPairSettingDBRow{"HeliEngineDegradeIndex", std::to_string(HeliEngineDegradeIndex)});
	results->push_back(StringPairSettingDBRow{"RestorationSpeedIndex", std::to_string(RestorationSpeedIndex)});
	results->push_back(StringPairSettingDBRow{"RefuelingSpeedIndex", std::to_string(RefuelingSpeedIndex)});
	results->push_back(StringPairSettingDBRow{"FuelPriceIndex", std::to_string(FuelPriceIndex)});
	results->push_back(StringPairSettingDBRow{"JerrycanPriceIndex", std::to_string(JerrycanPriceIndex)});
	results->push_back(StringPairSettingDBRow{"Random1Index", std::to_string(Random1Index)});
	results->push_back(StringPairSettingDBRow{"Random2Index", std::to_string(Random2Index)});
	results->push_back(StringPairSettingDBRow{"BarPositionIndex", std::to_string(BarPositionIndex)});
	results->push_back(StringPairSettingDBRow{"FuelColours_R_Index", std::to_string(FuelColours_R_Index)});
	results->push_back(StringPairSettingDBRow{"FuelColours_G_Index", std::to_string(FuelColours_G_Index)});
	results->push_back(StringPairSettingDBRow{"FuelColours_B_Index", std::to_string(FuelColours_B_Index)});
}

void handle_generic_settings_vehicle(std::vector<StringPairSettingDBRow>* settings){
	for(int i = 0; i < settings->size(); i++){
		StringPairSettingDBRow setting = settings->at(i);
		if(setting.name.compare("lastCustomVehicleSpawn") == 0){
			lastCustomVehicleSpawn = setting.value;
		}
		else if(setting.name.compare("speedBoostIndex") == 0){
			speedBoostIndex = stoi(setting.value);
		}
		else if (setting.name.compare("featureNeverDirty") == 0) {
			featureNeverDirty = stoi(setting.value);
		}
		else if (setting.name.compare("savedVehicleListSortMethod") == 0) {
			savedVehicleListSortMethod = stoi(setting.value);
		}
		else if(setting.name.compare("engPowMultIndex") == 0){
			engPowMultIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VehMassMultIndex") == 0){
			VehMassMultIndex = stoi(setting.value);
		}
		else if (setting.name.compare("current_player_forceshield") == 0) {
			current_player_forceshield = stoi(setting.value);
		}
		else if (setting.name.compare("InfiniteBoostIndex") == 0) {
			InfiniteBoostIndex = stoi(setting.value);
		}
		else if (setting.name.compare("NitrousIndex") == 0) {
			NitrousIndex = stoi(setting.value);
		}
		else if (setting.name.compare("TurnSignalsIndex") == 0){
			turnSignalsIndex = stoi(setting.value);
		}
		else if (setting.name.compare("turnSignalsAngleIndex") == 0) {
			turnSignalsAngleIndex = stoi(setting.value);
		}
		else if (setting.name.compare("DoorAutolockIndex") == 0) {
			DoorAutolockIndex = stoi(setting.value);
		}
		else if (setting.name.compare("turnSignalsAccelerationIndex") == 0) {
			turnSignalsAccelerationIndex = stoi(setting.value);
		}
		else if (setting.name.compare("JumpyVehIndex") == 0) {
			JumpyVehIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HeavyVehIndex") == 0) {
			HeavyVehIndex = stoi(setting.value);
		}
		else if (setting.name.compare("speedLimiterIndex") == 0){
			speedLimiterIndex = stoi(setting.value);
		}
		else if (setting.name.compare("speedCityLimiterIndex") == 0) {
			speedCityLimiterIndex = stoi(setting.value);
		}
		else if (setting.name.compare("speedCountryLimiterIndex") == 0) {
			speedCountryLimiterIndex = stoi(setting.value);
		}
		else if (setting.name.compare("lightsOffIndex") == 0){
			lightsOffIndex = stoi(setting.value);
		}
		else if (setting.name.compare("SpeedSizeIndex") == 0){
			SpeedSizeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("SpeedPositionIndex") == 0){
			SpeedPositionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FuelBlipsIndex") == 0){
			FuelBlipsIndex = stoi(setting.value);
		}
		else if (setting.name.compare("IdleConsumptionIndex") == 0) {
			IdleConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FuelBackground_Opacity_Index") == 0){
			FuelBackground_Opacity_Index = stoi(setting.value);
		}
		else if (setting.name.compare("VehRememberIndex") == 0){
			VehRememberIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VehBlipSizeIndex") == 0){
			VehBlipSizeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VehBlipColourIndex") == 0){
			VehBlipColourIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VehColourIndex") == 0) {
			VehColourIndex = stoi(setting.value);
		}
		else if (setting.name.compare("SpeedingCityIndex") == 0){
			SpeedingCityIndex = stoi(setting.value);
		}
		else if (setting.name.compare("DetectionRangeIndex") == 0){
			DetectionRangeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PirsuitRangeIndex") == 0){
			PirsuitRangeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("StarsPunishIndex") == 0){
			StarsPunishIndex = stoi(setting.value);
		}
		else if (setting.name.compare("EngineRunningIndex") == 0){
			EngineRunningIndex = stoi(setting.value);
		}
		else if (setting.name.compare("AutoShutEngineIndex") == 0) {
			AutoShutEngineIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HydraulicsIndex") == 0) {
			HydraulicsIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VisLightIndex") == 0) {
			VisLightIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VisLight3dIndex") == 0) {
			VisLight3dIndex = stoi(setting.value);
		}
		else if (setting.name.compare("SpeedingSpeedwayIndex") == 0){
			SpeedingSpeedwayIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FineSizeIndex") == 0){
			FineSizeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("VehBlipSymbolIndex") == 0 && VEH_BLIPSYMBOL_VALUES.size() > 0 && VehBlipSymbolIndex < VEH_BLIPSYMBOL_VALUES.size()){
			VehBlipSymbolIndex = stoi(setting.value);
		} 
		else if (VEH_BLIPSYMBOL_VALUES.size() > 0 && VehBlipSymbolIndex >= VEH_BLIPSYMBOL_VALUES.size()) { 
			VehBlipSymbolIndex = 0;
		}
		else if (setting.name.compare("VehBlipFlashIndex") == 0){
			VehBlipFlashIndex = stoi(setting.value);
		}
		else if (setting.name.compare("CarConsumptionIndex") == 0){
			CarConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BikeConsumptionIndex") == 0){
			BikeConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BoatConsumptionIndex") == 0){
			BoatConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PlaneConsumptionIndex") == 0){
			PlaneConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HeliConsumptionIndex") == 0){
			HeliConsumptionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("CarEngineHealthIndex") == 0) {
			CarEngineHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BikeEngineHealthIndex") == 0) {
			BikeEngineHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BoatEngineHealthIndex") == 0) {
			BoatEngineHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PlaneEngineHealthIndex") == 0) {
			PlaneEngineHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HeliEngineHealthIndex") == 0) {
			HeliEngineHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("CarEngineDegradeIndex") == 0) {
			CarEngineDegradeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BikeEngineDegradeIndex") == 0) {
			BikeEngineDegradeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BoatEngineDegradeIndex") == 0) {
			BoatEngineDegradeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PlaneEngineDegradeIndex") == 0) {
			PlaneEngineDegradeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HeliEngineDegradeIndex") == 0) {
			HeliEngineDegradeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("RestorationSpeedIndex") == 0) {
			RestorationSpeedIndex = stoi(setting.value);
		}
		else if (setting.name.compare("RefuelingSpeedIndex") == 0){
			RefuelingSpeedIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FuelPriceIndex") == 0){
			FuelPriceIndex = stoi(setting.value);
		}
		else if (setting.name.compare("JerrycanPriceIndex") == 0){
			JerrycanPriceIndex = stoi(setting.value);
		}
		else if (setting.name.compare("Random1Index") == 0){
			Random1Index = stoi(setting.value);
		}
		else if (setting.name.compare("Random2Index") == 0){
			Random2Index = stoi(setting.value);
		}
		else if (setting.name.compare("BarPositionIndex") == 0){
			BarPositionIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FuelColours_R_Index") == 0){
			FuelColours_R_Index = stoi(setting.value);
		}
		else if (setting.name.compare("FuelColours_G_Index") == 0){
			FuelColours_G_Index = stoi(setting.value);
		}
		else if (setting.name.compare("FuelColours_B_Index") == 0){
			FuelColours_B_Index = stoi(setting.value);
		}
	}
}

int get_current_veh_invincibility_mode(){
	if(!featureVehInvincible){
		return 0;
	}
	else if(!featureVehNoDamage){
		return 1;
	}
	else if(!featureVehInvulnIncludesCosmetic){
		return 2;
	}
	return 3;
}

void onchange_veh_invincibility_mode(int value, SelectFromListMenuItem* source){
	featureVehInvincible = (value > 0);
	featureVehNoDamage = (value > 1);
	featureVehInvulnIncludesCosmetic = (value > 2);

	featureVehInvincibleUpdated = true;
}

void onchange_veh_speed_boost_index(int value, SelectFromListMenuItem *source){
	speedBoostIndex = value;
}

int get_current_veh_eng_pow_index(){
	return engPowMultIndex;
}

void onchange_veh_eng_pow_index(int value, SelectFromListMenuItem* source){
	engPowMultIndex = value;
	powChanged = true;
}

void onchange_veh_never_dirty(int value, SelectFromListMenuItem* source) {
	featureNeverDirty = value;
	NeverDirtyChanged = true;
}

void onchange_veh_mass_index(int value, SelectFromListMenuItem* source){
	VehMassMultIndex = value;
	massChanged = true;
}

void onchange_player_forceshield_mode(int value, SelectFromListMenuItem* source) {
	current_player_forceshield = value;
	player_massChanged = true;
}

void onchange_veh_infiniteboost_index(int value, SelectFromListMenuItem* source) {
	InfiniteBoostIndex = value;
	InfiniteBoost_Changed = true;
}

void onchange_veh_nitrous_index(int value, SelectFromListMenuItem* source) {
	NitrousIndex = value;
	NitrousChanged = true;
}

void onchange_veh_turn_signals_index(int value, SelectFromListMenuItem* source){
	turnSignalsIndex = value;
	turnSignalsChanged = true;
}

void onchange_veh_turn_signals_angle_index(int value, SelectFromListMenuItem* source) {
	turnSignalsAngleIndex = value;
	turnSignalsAngleChanged = true;
}

void onchange_veh_turn_signals_acceleration_index(int value, SelectFromListMenuItem* source) {
	turnSignalsAccelerationIndex = value;
	turnSignalsAccelerationChanged = true;
}

void onchange_veh_jumpy_index(int value, SelectFromListMenuItem* source) {
	JumpyVehIndex = value;
	JumpyVehChanged = true;
}

void onchange_heavy_veh_index(int value, SelectFromListMenuItem* source) {
	HeavyVehIndex = value;
	HeavyVehChanged = true;
}

void onchange_veh_lightsOff_index(int value, SelectFromListMenuItem* source){
	lightsOffIndex = value;
	lightsOffChanged = true;
}

void onchange_veh_speedlimiter_index(int value, SelectFromListMenuItem* source){
	speedLimiterIndex = value;
	speedLimiterChanged = true;
}

void onchange_veh_cityspeedlimiter_index(int value, SelectFromListMenuItem* source) {
	speedCityLimiterIndex = value;
	speedCityLimiterChanged = true;
}

void onchange_veh_countryspeedlimiter_index(int value, SelectFromListMenuItem* source) {
	speedCountryLimiterIndex = value;
	speedCountryLimiterChanged = true;
}

void onchange_speed_size_index(int value, SelectFromListMenuItem* source){
	SpeedSizeIndex = value;
	SizeChanged = true;
}

void onchange_speed_position_index(int value, SelectFromListMenuItem* source){
	SpeedPositionIndex = value;
	PositionChanged = true;
}

void onchange_door_autolock_index(int value, SelectFromListMenuItem* source) {
	DoorAutolockIndex = value;
	DoorAutolockChanged = true;
}

void onchange_fuel_background_opacity_index(int value, SelectFromListMenuItem* source){
	FuelBackground_Opacity_Index = value;
	PositionChanged = true;
}
void onchange_veh_remember_index(int value, SelectFromListMenuItem* source){
	VehRememberIndex = value;
	PositionChanged = true;
}
void onchange_veh_blipsize_index(int value, SelectFromListMenuItem* source){
	VehBlipSizeIndex = value;
	PositionChanged = true;
}
void onchange_veh_blipcolour_index(int value, SelectFromListMenuItem* source){
	VehBlipColourIndex = value;
	PositionChanged = true;
}
void onchange_world_npc_vehicles_colour_index(int value, SelectFromListMenuItem* source) {
	VehColourIndex = value;
	PositionChanged = true;
}
void onchange_speeding_city_index(int value, SelectFromListMenuItem* source){
	SpeedingCityIndex = value;
	PositionChanged = true;
}
void onchange_detection_range_index(int value, SelectFromListMenuItem* source){
	DetectionRangeIndex = value;
	PositionChanged = true;
}
void onchange_pirsuit_range_index(int value, SelectFromListMenuItem* source){
	PirsuitRangeIndex = value;
	PositionChanged = true;
}
void onchange_stars_punish_index(int value, SelectFromListMenuItem* source){
	StarsPunishIndex = value;
	PositionChanged = true;
}
void onchange_veh_enginerunning_index(int value, SelectFromListMenuItem* source){
	EngineRunningIndex = value;
	PositionChanged = true;
}
void onchange_veh_autoshutengine_index(int value, SelectFromListMenuItem* source) {
	AutoShutEngineIndex = value;
	PositionChanged = true;
}
void onchange_veh_hydraulics_index(int value, SelectFromListMenuItem* source) {
	HydraulicsIndex = value;
	PositionChanged = true;
}
void onchange_veh_vislight_index(int value, SelectFromListMenuItem* source) {
	VisLightIndex = value;
	PositionChanged = true;
}
void onchange_veh_vislight3d_index(int value, SelectFromListMenuItem* source) {
	VisLight3dIndex = value;
	PositionChanged = true;
}
void onchange_speeding_speedway_index(int value, SelectFromListMenuItem* source){
	SpeedingSpeedwayIndex = value;
	PositionChanged = true;
}
void onchange_fine_size_index(int value, SelectFromListMenuItem* source){
	FineSizeIndex = value;
	PositionChanged = true;
}
void onchange_veh_blipsymbol_index(int value, SelectFromListMenuItem* source){
	VehBlipSymbolIndex = value;
	PositionChanged = true;
}
void onchange_veh_blipflash_index(int value, SelectFromListMenuItem* source){
	VehBlipFlashIndex = value;
	PositionChanged = true;
}
void onchange_fuel_blips_index(int value, SelectFromListMenuItem* source){
	FuelBlipsIndex = value;
	PositionChanged = true;
}

void onchange_idle_consumption_index(int value, SelectFromListMenuItem* source) {
	IdleConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_car_consumption_index(int value, SelectFromListMenuItem* source){
	CarConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_bike_consumption_index(int value, SelectFromListMenuItem* source){
	BikeConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_boat_consumption_index(int value, SelectFromListMenuItem* source){
	BoatConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_plane_consumption_index(int value, SelectFromListMenuItem* source){
	PlaneConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_heli_consumption_index(int value, SelectFromListMenuItem* source){
	HeliConsumptionIndex = value;
	PositionChanged = true;
}

void onchange_car_enginehealth_index(int value, SelectFromListMenuItem* source) {
	CarEngineHealthIndex = value;
	PositionChanged = true;
}

void onchange_bike_enginehealth_index(int value, SelectFromListMenuItem* source) {
	BikeEngineHealthIndex = value;
	PositionChanged = true;
}

void onchange_boat_enginehealth_index(int value, SelectFromListMenuItem* source) {
	BoatEngineHealthIndex = value;
	PositionChanged = true;
}

void onchange_plane_enginehealth_index(int value, SelectFromListMenuItem* source) {
	PlaneEngineHealthIndex = value;
	PositionChanged = true;
}

void onchange_heli_enginehealth_index(int value, SelectFromListMenuItem* source) {
	HeliEngineHealthIndex = value;
	PositionChanged = true;
}

void onchange_car_enginedegrade_index(int value, SelectFromListMenuItem* source) {
	CarEngineDegradeIndex = value;
	PositionChanged = true;
}

void onchange_bike_enginedegrade_index(int value, SelectFromListMenuItem* source) {
	BikeEngineDegradeIndex = value;
	PositionChanged = true;
}

void onchange_boat_enginedegrade_index(int value, SelectFromListMenuItem* source) {
	BoatEngineDegradeIndex = value;
	PositionChanged = true;
}

void onchange_plane_enginedegrade_index(int value, SelectFromListMenuItem* source) {
	PlaneEngineDegradeIndex = value;
	PositionChanged = true;
}

void onchange_heli_enginedegrade_index(int value, SelectFromListMenuItem* source) {
	HeliEngineDegradeIndex = value;
	PositionChanged = true;
}

void onchange_restoration_speed_index(int value, SelectFromListMenuItem* source) {
	RestorationSpeedIndex = value;
	PositionChanged = true;
}

void onchange_refuelspeed_index(int value, SelectFromListMenuItem* source){
	RefuelingSpeedIndex = value;
	PositionChanged = true;
}

void onchange_fuelprice_index(int value, SelectFromListMenuItem* source){
	FuelPriceIndex = value;
	PositionChanged = true;
}

void onchange_canprice_index(int value, SelectFromListMenuItem* source){
	JerrycanPriceIndex = value;
	PositionChanged = true;
}

void onchange_random1_index(int value, SelectFromListMenuItem* source){
	Random1Index = value;
	PositionChanged = true;
}

void onchange_random2_index(int value, SelectFromListMenuItem* source){
	Random2Index = value;
	PositionChanged = true;
}

void onchange_barposition_index(int value, SelectFromListMenuItem* source){
	BarPositionIndex = value;
	PositionChanged = true;
}

void onchange_fuel_colours_r_index(int value, SelectFromListMenuItem* source){
	FuelColours_R_Index = value;
	FuelColours_R_Changed = true;
}

void onchange_fuel_colours_g_index(int value, SelectFromListMenuItem* source){
	FuelColours_G_Index = value;
	FuelColours_G_Changed = true;
}

void onchange_fuel_colours_b_index(int value, SelectFromListMenuItem* source){
	FuelColours_B_Index = value;
	FuelColours_B_Changed = true;
}

struct VehicleImage{
	char* modelName;
	char* dict;
	char* imgName;
	int localID;
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
	{ "STOCKADE", "candc_casinoheist", "stockade" },
};

static std::vector<VehicleImage> ALL_VEH_IMAGES;

MenuItemImage* vehicle_image_preview_finder(MenuItem<std::string> choice){
	if(!is_vehicle_preview_enabled()){
		return NULL;
	}

	for each (VehicleImage vimg in ALL_VEH_IMAGES){
		if(strcmp(vimg.modelName, choice.value.c_str()) == 0){
			MenuItemImage* image = new MenuItemImage();
			image->dict = vimg.dict;
			if(image->is_local()){
				image->localID = vimg.localID;
			}
			else{
				image->name = vimg.imgName;
			}
			return image;
		}
	}

	std::ostringstream ss;
	ss << "Couldn't find preview for " << choice.value;
	write_text_to_log_file(ss.str());
	return NULL;
}

void unpack_veh_preview(char* model, int resRef, std::string bitmapName){
	WAIT(0);
	make_periodic_feature_call();

	HMODULE module = GetENTModuleHandle();
	if(module == NULL){
		write_text_to_log_file("Can't load images because module is null");
		return;
	}

	std::ostringstream filenameSS;
	filenameSS << bitmapName << ".png";
	auto filename = filenameSS.str();
	WCHAR* filePath = get_temp_dir_path((char*) filename.c_str());

	char* filePathNonW = new char[MAX_PATH];
	wcstombs(filePathNonW, filePath, MAX_PATH);

	if(!does_file_exist(filePathNonW)){
		// Locate the resource in the application's executable.
		HRSRC imageResHandle = FindResource(
			module,             // This component.
			MAKEINTRESOURCE(resRef),   // Resource name.
			_T("PNG"));        // Resource type.

		if(imageResHandle == NULL){
			std::ostringstream ss;
			int e = GetLastError();
			ss << "Couldn't find resource " << bitmapName << " and code " << resRef << ", error " << e;
			write_text_to_log_file(ss.str());
			return;
		}

		HGLOBAL hRes = LoadResource(module, imageResHandle);
		LPVOID memRes = LockResource(hRes);
		DWORD sizeRes = SizeofResource(module, imageResHandle);

		std::transform(bitmapName.begin(), bitmapName.end(), bitmapName.begin(), ::tolower);

		HANDLE newFile = CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if(newFile == INVALID_HANDLE_VALUE){
			write_text_to_log_file("Couldn't create img file in temp");
			return;
		}

		DWORD written = 0;
		if(!WriteFile(newFile, hRes, sizeRes, &written, NULL)){
			write_text_to_log_file("Couldn't write to img file in temp");
			CloseHandle(newFile);
			return;
		}
		else{
			CloseHandle(newFile);
		}
	}

	if(does_file_exist(filePathNonW)){
		int textureID = createTexture(filePathNonW);
		ALL_VEH_IMAGES.push_back({model, (char*) LOCAL_TEXTURE_DICT, NULL, textureID});
	}

	delete filePathNonW;
}

void init_vehicle_feature(){
	//copy all the ingame images
	ALL_VEH_IMAGES.insert(ALL_VEH_IMAGES.end(), INGAME_VEH_IMAGES.begin(), INGAME_VEH_IMAGES.end());

	// spawn name, defined name, image file name
	unpack_veh_preview("AIRTUG", VP_AIRTUG, "VP_AIRTUG");
	unpack_veh_preview("AMBULANCE", VP_AMBULANCE, "VP_AMBULANCE");
	unpack_veh_preview("ARMYTANKER", VP_ARMYTANKER, "VP_ARMYTANKER");
	unpack_veh_preview("ARMYTRAILER", VP_ARMYTRAILER, "VP_ARMYTRAILER");
	unpack_veh_preview("ARMYTRAILER2", VP_ARMYTRAILER2, "VP_ARMYTRAILER2");
	unpack_veh_preview("ASEA2", VP_ASEA_SNOW, "VP_ASEA_SNOW");
	unpack_veh_preview("BAGGER", VP_BAGGER, "VP_BAGGER");
	unpack_veh_preview("BALETRAILER", VP_BALETRAILER, "VP_BALETRAILER");
	unpack_veh_preview("BALLER2", VP_BALLER_SMALL, "VP_BALLER_SMALL");
	unpack_veh_preview("BARRACKS2", VP_BARRACKS2, "VP_BARRACKS2");
	unpack_veh_preview("BENSON", VP_BENSON, "VP_BENSON");
	unpack_veh_preview("BIFF", VP_BIFF, "VP_BIFF");
	unpack_veh_preview("BISON2", VP_BISON2, "VP_BISON2");
	unpack_veh_preview("BISON3", VP_BISON3, "VP_BISON3");
	unpack_veh_preview("BJXL", VP_BJXL, "VP_BJXL");
	//unpack_veh_preview("BLAZER2", VP_LIFEGUARD, "VP_LIFEGUARD");
	unpack_veh_preview("BLIMP", VP_BLIMP, "VP_BLIMP");
	unpack_veh_preview("BLIMP2", VP_BLIMP2, "VP_BLIMP2");
	unpack_veh_preview("BLISTA", VP_BLISTA, "VP_BLISTA");
	unpack_veh_preview("BOATTRAILER", VP_BOATTRAILER, "VP_BOATTRAILER");
	//unpack_veh_preview("BOXVILLE", VP_BOXVILLE, "VP_BOXVILLE");
	unpack_veh_preview("BOXVILLE2", VP_BOXVILLE2, "VP_BOXVILLE2");
	unpack_veh_preview("BOXVILLE3", VP_BOXVILLE3, "VP_BOXVILLE3");
	unpack_veh_preview("BLISTA3", VP_MONKEY_BLISTA, "VP_MONKEY_BLISTA");
	unpack_veh_preview("BTYPE", VP_ROOSEVELT, "VP_ROOSEVELT");
	unpack_veh_preview("BUCCANEER", VP_BUCCANEER, "VP_BUCCANEER");
	unpack_veh_preview("BURRITO", VP_BURRITO, "VP_BURRITO");
	//unpack_veh_preview("BURRITO2", VP_BURRITO2, "VP_BURRITO2");
	unpack_veh_preview("BURRITO3", VP_BURRITO3, "VP_BURRITO3");
	unpack_veh_preview("BURRITO4", VP_BURRITO4, "VP_BURRITO4");
	unpack_veh_preview("BURRITO5", VP_BURRITO5, "VP_BURRITO5");
	unpack_veh_preview("BUZZARD2", VP_BUZZARD2, "VP_BUZZARD2");
	unpack_veh_preview("CABLECAR", VP_CABLECAR, "VP_CABLECAR");
	unpack_veh_preview("CADDY", VP_CADDY, "VP_CADDY");
	unpack_veh_preview("CADDY2", VP_CADDY2, "VP_CADDY2");
	unpack_veh_preview("CAMPER", VP_CAMPER, "VP_CAMPER");
	unpack_veh_preview("CARGOBOB2", VP_CARGOBOB2, "VP_CARGOBOB2");
	unpack_veh_preview("CARGOBOB3", VP_CARGOBOB3, "VP_CARGOBOB3");
	unpack_veh_preview("CUTTER", VP_CUTTER, "VP_CUTTER");
	unpack_veh_preview("CARGOPLANE", VP_CARGO, "VP_CARGO");
	unpack_veh_preview("DAEMON", VP_DAEMON, "VP_DAEMON");
	unpack_veh_preview("DILETTANTE2", VP_DILET_LIVERY, "VP_DILET_LIVERY");
	unpack_veh_preview("DINGHY", VP_DINGHY, "VP_DINGHY");
	unpack_veh_preview("DINGHY2", VP_DINGHY2, "VP_DINGHY2");
	unpack_veh_preview("HANDLER", VP_DOCKHANDLER, "VP_DOCKHANDLER");
	unpack_veh_preview("DOCKTRAILER", VP_DOCKTRAILER, "VP_DOCKTRAILER");
	unpack_veh_preview("DOCKTUG", VP_DOCKTUG, "VP_DOCKTUG");
	unpack_veh_preview("BULLDOZER", VP_DOZER, "VP_DOZER");
	unpack_veh_preview("DLOADER", VP_DUNELOADER, "VP_DUNELOADER");
	unpack_veh_preview("DUBSTA", VP_DUBSTA, "VP_DUBSTA");
	unpack_veh_preview("DUBSTA2", VP_DUBSTA_BLACK, "VP_DUBSTA_BLACK");
	unpack_veh_preview("DUKES", VP_DUKES, "VP_DUKES");
	unpack_veh_preview("DUNE2", VP_SPACEDOCKER, "VP_SPACEDOCKER");
	unpack_veh_preview("EMPEROR", VP_EMPEROR, "VP_EMPEROR");
	unpack_veh_preview("EMPEROR2", VP_EMPEROR_RUSTY, "VP_EMPEROR_RUSTY");
	unpack_veh_preview("EMPEROR3", VP_EMPEROR_SNOW, "VP_EMPEROR_SNOW");
	unpack_veh_preview("FBI", VP_FBI, "VP_FBI");
	unpack_veh_preview("FBI2", VP_FBI2, "VP_FBI2");
	//unpack_veh_preview("FIRETRUK", VP_FIRETRUK, "VP_FIRETRUK");
	unpack_veh_preview("FIXTER", VP_FIXTER, "VP_FIXTER");
	unpack_veh_preview("FLATBED", VP_FLATBED, "VP_FLATBED");
	unpack_veh_preview("FORKLIFT", VP_FORKLIFT, "VP_FORKLIFT");
	unpack_veh_preview("FQ2", VP_FQ2, "VP_FQ2");
	unpack_veh_preview("FREIGHT", VP_FREIGHT, "VP_FREIGHT");
	unpack_veh_preview("FREIGHTCAR", VP_FREIGHTCAR, "VP_FREIGHTCAR");
	unpack_veh_preview("FREIGHTCONT1", VP_FREIGHTCONT1, "VP_FREIGHTCONT1");
	unpack_veh_preview("FREIGHTCONT2", VP_FREIGHTCONT2, "VP_FREIGHTCONT2");
	unpack_veh_preview("FREIGHTGRAIN", VP_FREIGHTGRAIN, "VP_FREIGHTGRAIN");
	unpack_veh_preview("FREIGHTTRAILER", VP_FREIGHTTRAILER, "VP_FREIGHTTRAILER");
	unpack_veh_preview("FROGGER2", VP_FROGGER2, "VP_FROGGER2");
	unpack_veh_preview("FUTO", VP_FUTO, "VP_FUTO");
	unpack_veh_preview("GBURRITO", VP_GBURRITO, "VP_GBURRITO");
	unpack_veh_preview("GRAINTRAILER", VP_GRAINTRAILER, "VP_GRAINTRAILER");
	unpack_veh_preview("GRANGER", VP_GRANGER, "VP_GRANGER");
	unpack_veh_preview("GUARDIAN", VP_GUARDIAN, "VP_GUARDIAN");
	unpack_veh_preview("HABANERO", VP_HABANERO, "VP_HABANERO");
	unpack_veh_preview("HAULER", VP_HAULER, "VP_HAULER");
	unpack_veh_preview("JET", VP_JET, "VP_JET");
	unpack_veh_preview("LAZER", VP_LAZER, "VP_LAZER");
	unpack_veh_preview("LECTRO", VP_LECTRO, "VP_LECTRO");
	//unpack_veh_preview("LGUARD", VP_LGUARD, "VP_LGUARD");
	unpack_veh_preview("MANANA", VP_MANANA, "VP_MANANA");
	unpack_veh_preview("MESA2", VP_MESA_SNOW, "VP_MESA_SNOW");
	unpack_veh_preview("METROTRAIN", VP_METROTRAIN, "VP_METROTRAIN");
	unpack_veh_preview("MIXER", VP_MIXER, "VP_MIXER");
	unpack_veh_preview("MIXER2", VP_MIXER2, "VP_MIXER2");
	unpack_veh_preview("MOWER", VP_MOWER, "VP_MOWER");
	unpack_veh_preview("MULE2", VP_MULE2, "VP_MULE2");
	unpack_veh_preview("ORACLE2", VP_ORACLE2, "VP_ORACLE2");
	unpack_veh_preview("PACKER", VP_PACKER, "VP_PACKER");
	unpack_veh_preview("PATRIOT", VP_PATRIOT, "VP_PATRIOT");
	unpack_veh_preview("PEYOTE", VP_PEYOTE, "VP_PEYOTE");
	unpack_veh_preview("PHANTOM", VP_PHANTOM, "VP_PHANTOM");
	unpack_veh_preview("PHOENIX", VP_PHOENIX, "VP_PHOENIX");
	unpack_veh_preview("POLICE", VP_POLICE, "VP_POLICE");
	unpack_veh_preview("POLICE2", VP_POLICE2, "VP_POLICE2");
	unpack_veh_preview("POLICE3", VP_POLICE3, "VP_POLICE3");
	unpack_veh_preview("POLICE4", VP_POLICE4, "VP_POLICE4");
	unpack_veh_preview("POLICEB", VP_POLICEB, "VP_POLICEB");
	unpack_veh_preview("POLICEOLD1", VP_POLICEOLD1, "VP_POLICEOLD1");
	unpack_veh_preview("POLICEOLD2", VP_POLICEOLD2, "VP_POLICEOLD2");
	unpack_veh_preview("POLICET", VP_POLICET, "VP_POLICET");
	unpack_veh_preview("POLMAV", VP_POLMAV, "VP_POLMAV");
	unpack_veh_preview("PONY", VP_PONY, "VP_PONY");
	unpack_veh_preview("PONY2", VP_PONY2, "VP_PONY2");
	unpack_veh_preview("POUNDER", VP_POUNDER, "VP_POUNDER");
	unpack_veh_preview("PRAIRIE", VP_PRAIRIE, "VP_PRAIRIE");
	unpack_veh_preview("PRANGER", VP_PRANGER, "VP_PRANGER");
	unpack_veh_preview("PREDATOR", VP_PREDATOR, "VP_PREDATOR");
	unpack_veh_preview("PROPTRAILER", VP_PROPTRAILER, "VP_PROPTRAILER");
	unpack_veh_preview("RAKETRAILER", VP_RAKETRAILER, "VP_RAKETRAILER");
	unpack_veh_preview("RANCHERXL2", VP_RANCHERXL_SNOW, "VP_RANCHERXL_SNOW");
	unpack_veh_preview("RIOT", VP_RIOT, "VP_RIOT");
	unpack_veh_preview("RIPLEY", VP_RIPLEY, "VP_RIPLEY");
	unpack_veh_preview("ROMERO", VP_HEARSE, "VP_HEARSE");
	unpack_veh_preview("RUINER", VP_RUINER, "VP_RUINER");
	unpack_veh_preview("RUBBLE", VP_RUBBLE, "VP_RUBBLE");
	unpack_veh_preview("RUMPO2", VP_RUMPO2, "VP_RUMPO2");
	unpack_veh_preview("SABREGT", VP_SABRE, "VP_SABRE");
	unpack_veh_preview("SADLER2", VP_SADLER_SNOW, "VP_SADLER_SNOW");
	unpack_veh_preview("SCRAP", VP_SCRAP, "VP_SCRAP");
	unpack_veh_preview("SEASHARK2", VP_SEASHARK2, "VP_SEASHARK2");
	unpack_veh_preview("SENTINEL2", VP_SENTINEL, "VP_SENTINEL");
	unpack_veh_preview("SERRANO", VP_SERRANO, "VP_SERRANO");
	unpack_veh_preview("SHERIFF", VP_SHERIFF, "VP_SHERIFF");
	unpack_veh_preview("SHERIFF2", VP_SHERIFF2, "VP_SHERIFF2");
	unpack_veh_preview("SKYLIFT", VP_SKYLIFT, "VP_SKYLIFT");
	unpack_veh_preview("SLAMVAN2", VP_SLAMVAN_LOSTMC, "VP_SLAMVAN_LOSTMC");
	unpack_veh_preview("SPEEDO", VP_SPEEDO, "VP_SPEEDO");
	unpack_veh_preview("SPEEDO2", VP_SPEEDO2, "VP_SPEEDO2");
	unpack_veh_preview("SPEEDO4", VP_SPEEDO, "VP_SPEEDO");
	//unpack_veh_preview("STOCKADE", VP_STOCKADE, "VP_STOCKADE");
	unpack_veh_preview("STOCKADE3", VP_STOCKADE3, "VP_STOCKADE3");
	unpack_veh_preview("SUBMERSIBLE", VP_SUB, "VP_SUB");
	unpack_veh_preview("SULTAN", VP_SULTAN, "VP_SULTAN");
	unpack_veh_preview("SURFER2", VP_SURFER2, "VP_SURFER2");
	unpack_veh_preview("TACO", VP_TACO, "VP_TACO");
	unpack_veh_preview("TANKER", VP_TANKER, "VP_TANKER");
	unpack_veh_preview("TANKER2", VP_TANKER2, "VP_TANKER2");
	unpack_veh_preview("TANKERCAR", VP_TANKERCAR, "VP_TANKERCAR");
	unpack_veh_preview("TAXI", VP_TAXI, "VP_TAXI");
	unpack_veh_preview("TIPTRUCK", VP_TIPTRUCK, "VP_TIPTRUCK");
	unpack_veh_preview("TIPTRUCK2", VP_TIPTRUCK2, "VP_TIPTRUCK2");
	unpack_veh_preview("TORNADO", VP_TORNADO, "VP_TORNADO");
	unpack_veh_preview("TORNADO2", VP_TORNADO_CAB, "VP_TORNADO_CAB");
	unpack_veh_preview("TORNADO3", VP_TORNADO_RUSTY, "VP_TORNADO_RUSTY");
	unpack_veh_preview("TORNADO4", VP_TORNADO_CAB_RUSTY, "VP_TORNADO_CAB_RUSTY");
	unpack_veh_preview("TOURBUS", VP_TOURBUS, "VP_TOURBUS");
	unpack_veh_preview("TOWTRUCK", VP_TOWTRUCK, "VP_TOWTRUCK");
	unpack_veh_preview("TOWTRUCK2", VP_TOWTRUCK2, "VP_TOWTRUCK2");
	unpack_veh_preview("TR2", VP_TR2, "VP_TR2");
	unpack_veh_preview("TR3", VP_TR3, "VP_TR3");
	unpack_veh_preview("TR4", VP_TR4, "VP_TR4");
	unpack_veh_preview("TRACTOR", VP_TRACTOR, "VP_TRACTOR");
	unpack_veh_preview("TRACTOR2", VP_TRACTOR2, "VP_TRACTOR2");
	unpack_veh_preview("TRACTOR3", VP_TRACTOR3, "VP_TRACTOR3");
	unpack_veh_preview("TRAILERLOGS", VP_TRAILERLOGS, "VP_TRAILERLOGS");
	unpack_veh_preview("TRAILERS", VP_TRAILERS, "VP_TRAILERS");
	unpack_veh_preview("TRAILERS2", VP_TRAILERS2, "VP_TRAILERS2");
	unpack_veh_preview("TRAILERS3", VP_TRAILERS3, "VP_TRAILERS3");
	unpack_veh_preview("TRAILERSMALL", VP_TRAILERSMALL, "VP_TRAILERSMALL");
	unpack_veh_preview("TRASH", VP_TRASH, "VP_TRASH");
	unpack_veh_preview("TRASH2", VP_TRASH2, "VP_TRASH2");
	unpack_veh_preview("TRFLAT", VP_TRFLAT, "VP_TRFLAT");
	unpack_veh_preview("TVTRAILER", VP_TVTRAILER, "VP_TVTRAILER");
	unpack_veh_preview("UTILLITRUCK", VP_UTILLITRUCK, "VP_UTILLITRUCK");
	unpack_veh_preview("UTILLITRUCK2", VP_UTILLITRUCK2, "VP_UTILLITRUCK2");
	unpack_veh_preview("UTILLITRUCK3", VP_UTILLITRUCK3, "VP_UTILLITRUCK3");
	unpack_veh_preview("VOODOO2", VP_VOODOO, "VP_VOODOO");

	unpack_veh_preview("BALLER5", VP_BALLER5, "VP_BALLER5");
	unpack_veh_preview("BALLER6", VP_BALLER6, "VP_BALLER6");
	unpack_veh_preview("SCHAFTER2", VP_SCHAFTER2, "VP_SCHAFTER2");
	unpack_veh_preview("SCHAFTER3", VP_SCHAFTER3, "VP_SCHAFTER3");
	unpack_veh_preview("SCHAFTER4", VP_SCHAFTER4, "VP_SCHAFTER4");
	unpack_veh_preview("SCHAFTER5", VP_SCHAFTER5, "VP_SCHAFTER5");
	unpack_veh_preview("SCHAFTER6", VP_SCHAFTER6, "VP_SCHAFTER6");
	unpack_veh_preview("MOONBEAM", VP_MOONBEAM, "VP_MOONBEAM");
	unpack_veh_preview("COGNOSCENTI2", VP_COGNOSCENTI2, "VP_COGNOSCENTI2");
	unpack_veh_preview("COG552", VP_COG552, "VP_COG552");

	unpack_veh_preview("VIRGO3", VP_VIRGO3, "VP_VIRGO3");
	unpack_veh_preview("FACTION", VP_FACTION, "VP_FACTION");

	unpack_veh_preview("HAULER2", VP_HAULER2, "VP_HAULER2");
	unpack_veh_preview("INSURGENT3", VP_INSURGENT3, "VP_INSURGENT3");
	unpack_veh_preview("PHANTOM3", VP_PHANTOM3, "VP_PHANTOM3");
	unpack_veh_preview("TECHNICAL3", VP_TECHNICAL3, "VP_TECHNICAL3");
	unpack_veh_preview("CLIFFHANGER", VP_CLIFFHANGER, "VP_CLIFFHANGER");
	
	unpack_veh_preview("ISSI4", VP_ISSI1, "VP_ISSI4");
	unpack_veh_preview("ISSI5", VP_ISSI2, "VP_ISSI5");
	unpack_veh_preview("ISSI6", VP_ISSI3, "VP_ISSI6");
	unpack_veh_preview("BRUISER", VP_BRUISER1, "VP_BRUISER");
	unpack_veh_preview("BRUISER2", VP_BRUISER2, "VP_BRUISER2");
	unpack_veh_preview("BRUISER3", VP_BRUISER3, "VP_BRUISER3");
	unpack_veh_preview("DEATHBIKE", VP_DEATHBIKE1, "VP_DEATHBIKE");
	unpack_veh_preview("DEATHBIKE2", VP_DEATHBIIKE2, "VP_DEATHBIIKE2");
	unpack_veh_preview("DEATHBIKE3", VP_DEATHBIKE3, "VP_DEATHBIKE3");
	unpack_veh_preview("DOMINATOR4", VP_DOMINATOR1, "VP_DOMINATOR4");
	unpack_veh_preview("DOMINATOR5", VP_DOMINATOR2, "VP_DOMINATOR5");
	unpack_veh_preview("DOMINATOR6", VP_DOMINATOR3, "VP_DOMINATOR6");
	unpack_veh_preview("IMPALER2", VP_IMPALER1, "VP_IMPALER1");
	unpack_veh_preview("IMPALER3", VP_IMPALER2, "VP_IMPALER2");
	unpack_veh_preview("IMPALER4", VP_IMPALER3, "VP_IMPALER3");
	unpack_veh_preview("MONSTER3", VP_MONSTER1, "VP_MONSTER1");
	unpack_veh_preview("MONSTER4", VP_MONSTER2, "VP_MONSTER2");
	unpack_veh_preview("MONSTER5", VP_MONSTER3, "VP_MONSTER3");
	unpack_veh_preview("SLAMVAN4", VP_SLAMVAN1, "VP_SLAMVAN4");
	unpack_veh_preview("SLAMVAN5", VP_SLAMVAN2, "VP_SLAMVAN5");
	unpack_veh_preview("SLAMVAN6", VP_SLAMVAN3, "VP_SLAMVAN6");
}

void fix_vehicle(){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(bPlayerExists){
		if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

			VEHICLE::SET_VEHICLE_FIXED(veh);
			VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(veh);
			VEHICLE::SET_VEHICLE_NEEDS_TO_BE_HOTWIRED(veh, false);

			if(FIRE::IS_ENTITY_ON_FIRE(veh)){
				FIRE::STOP_ENTITY_FIRE(veh);
			}

			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
			VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 1000.0f);
			VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 1000.f);

			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
			VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(veh, false);

			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true);

			set_status_text("Vehicle repaired");
		}
	}
}

void clean_vehicle(){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(bPlayerExists){
		if(PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(PED::GET_VEHICLE_PED_IS_USING(playerPed), 0);

			set_status_text("Vehicle cleaned");
		}
	}
}

bool is_convertible_roofdown(std::vector<int> extras){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	int roofState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh);
	return (roofState == 2 || roofState == 1);
}

void set_convertible_roofdown(bool applied, std::vector<int> extras){
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	if(applied){
		VEHICLE::LOWER_CONVERTIBLE_ROOF(veh, featureVehicleDoorInstant);
	}
	else{
		VEHICLE::RAISE_CONVERTIBLE_ROOF(veh, featureVehicleDoorInstant);
	}
}
