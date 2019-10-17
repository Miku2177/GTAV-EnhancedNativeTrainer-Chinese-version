/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#include "area_effect.h"
#include "script.h"
#include "../../inc/nativeCaller.h"

int areaeffect_top_level_menu_index = 0;
int areaeffect_ped_level_menu_index = 0;
int areaeffect_veh_level_menu_index = 0;
int areaeffect_ped_advconfig_menu_index = 0;
int areaeffect_peds_weapons_menu_index = 0;

int callsA = 0;
int callsB = 0;
int callsC = 0;
int callsD = 0;
int callsE = 0;

std::deque<ENTTrackedPedestrian*> trackedPeds;
std::deque<ENTTrackedVehicle*> trackedVehicles;

//features
bool featurePlayerIgnoredByAll = false;
bool featureNPCShowHealth = false;
bool featureAreaPedsInvincible = false;
bool featureAreaVehiclesInvincible = false;
bool featureAreaVehiclesBroken = false;
bool featureAreaPedsRioting = false;
bool featureAreaVehiclesExploded = false;
bool featureAreaPedsHeadExplode = false;
bool featureAngryPedsUseCover = false;
bool featureAngryPedsTargetYou = false;
bool featurePedsWeapons = false;
bool featureAngryMenOnly = false;
bool featurePedsIncludeDrivers = false;
bool featurePedsIncludePilots = false;
bool featureAggressiveDrivers = false;
bool featureLawAbidingCitizens = false;
bool featureNPCNoLights = false;
bool featureNPCNeonLights = false;
bool featureNPCFullBeam = false;
bool featureDirtyVehicles = false;
bool featureNPCNoGravityVehicles = false;
bool featureNPCReducedGripVehicles = false;
bool featureBoostNPCRadio = false;

int pedWeaponSetIndex = 0;

std::set<Ped> allWorldPedsThisFrame;
bool allWorldPedsThisFrameFilled = false;

std::set<Vehicle> allWorldVehiclesThisFrame;
bool allWorldVehiclesThisFrameFilled = false;

std::set<Ped> releasedPeds;
std::set<Vehicle> releasedVehicles;

bool v_collision_check = false;
bool time_to_chase = false;
bool time_to_attack = true;
std::vector<Ped> pursuer;
std::vector<Vehicle> v_collided;
int s_secs_passed, s_secs_curr, s_seconds = 0;

const std::vector<std::string> PED_WEAPONS_SELECTIVE_CAPTIONS{ "\"WEAPON_UNARMED\"", "\"WEAPON_NIGHTSTICK\"", "\"WEAPON_FLASHLIGHT\"", "\"WEAPON_KNIFE\"", "\"WEAPON_DAGGER\"", "\"WEAPON_HAMMER\"", "\"WEAPON_BAT\"", "\"WEAPON_GOLFCLUB\"",
"\"WEAPON_CROWBAR\"", "\"WEAPON_POOLCUE\"", "\"WEAPON_WRENCH\"", "\"WEAPON_MACHETE\"", "\"WEAPON_BOTTLE\"", "\"WEAPON_PISTOL\"", "\"WEAPON_APPISTOL\"", "\"WEAPON_REVOLVER\"", "\"WEAPON_STUNGUN\"", "\"WEAPON_FLAREGUN\"",
"\"WEAPON_MACHINEPISTOL\"", "\"WEAPON_MARKSMANPISTOL\"", "\"WEAPON_MINISMG\"", "\"WEAPON_ASSAULTSMG\"", "\"WEAPON_ASSAULTRIFLE\"", "\"WEAPON_CARBINERIFLE\"", "\"WEAPON_ADVANCEDRIFLE\"", "\"WEAPON_COMPACTRIFLE\"", "\"WEAPON_HEAVYSHOTGUN\"",
"\"WEAPON_DBSHOTGUN\"", "\"WEAPON_AUTOSHOTGUN\"", "\"WEAPON_MUSKET\"", "\"WEAPON_SAWNOFFSHOTGUN\"", "\"WEAPON_COMBATMG\"", "\"WEAPON_MINIGUN\"", "\"WEAPON_GUSENBERG\"", "\"WEAPON_SNIPERRIFLE\"", "\"WEAPON_HEAVYSNIPER\"",
"\"WEAPON_GRENADELAUNCHER\"", "\"WEAPON_GRENADELAUNCHER_SMOKE\"", "\"WEAPON_RPG\"", "\"WEAPON_HOMINGLAUNCHER\"", "\"WEAPON_COMPACTLAUNCHER\"", "\"WEAPON_RAILGUN\"", "\"WEAPON_FIREWORK\"" };
int PedWeaponsSelectiveIndex = 0;
bool PedWeaponsSelective1Changed = true;

// NPC Damaged Vehicles
const std::vector<std::string> WORLD_DAMAGED_VEHICLES_CAPTIONS{ "OFF", "5", "7", "10", "12", "15", "20", "30", "50", "100", "500", "1000" };
const int WORLD_DAMAGED_VEHICLES_VALUES[] = { 0, 5, 7, 10, 12, 15, 20, 30, 50, 100, 500, 1000 };
int DamagedVehiclesIndex = 0;
bool DamagedVehiclesChanged = true;

// NPC Vehicle Speed
const std::vector<std::string> WORLD_NPC_VEHICLESPEED_CAPTIONS{ "OFF", "1", "5", "10", "15", "30", "50", "70", "100" };
const int WORLD_NPC_VEHICLESPEED_VALUES[] = { -1, 1, 5, 10, 15, 30, 50, 70, 100 };
int NPCVehicleSpeedIndex = 0;
bool NPCVehicleSpeedChanged = true;
int PedAccuracyIndex = 0;
bool PedAccuracyChanged = true;

// Reduced Grip If Heavy Snow && Slippery When Wet && No Peds Gravity
const std::vector<std::string> WORLD_REDUCEDGRIP_SNOWING_CAPTIONS{ "OFF", "Simple", "Advanced" };
const int WORLD_REDUCEDGRIP_SNOWING_VALUES[] = { 0, 1, 2 };
int RadarReducedGripSnowingIndex = 0;
bool RadarReducedGripSnowingChanged = true;
int RadarReducedGripRainingIndex = 0;
bool RadarReducedGripRainingChanged = true;
int NoPedsGravityIndex = 0;
bool NoPedsGravityChanged = true;
int featureNeverDirty = 0;
bool NeverDirtyChanged = true;

//For onscreen debug info
bool featureShowDebugInfo = false;

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCShowHealth", &featureNPCShowHealth});
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsInvincible", &featureAreaPedsInvincible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsHeadExplode", &featureAreaPedsHeadExplode});
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesInvincible", &featureAreaVehiclesInvincible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesBroken", &featureAreaVehiclesBroken}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesExploded", &featureAreaVehiclesExploded});
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsRioting", &featureAreaPedsRioting}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAngryPedsUseCover", &featureAngryPedsUseCover});
	results->push_back(FeatureEnabledLocalDefinition{"featureAngryPedsTargetYou", &featureAngryPedsTargetYou});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedsWeapons", &featurePedsWeapons});
	results->push_back(FeatureEnabledLocalDefinition{"featureAngryMenOnly", &featureAngryMenOnly});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedsIncludeDrivers", &featurePedsIncludeDrivers});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedsIncludePilots", &featurePedsIncludePilots});
	results->push_back(FeatureEnabledLocalDefinition{"featureAggressiveDrivers", &featureAggressiveDrivers});
	results->push_back(FeatureEnabledLocalDefinition{"featureLawAbidingCitizens", &featureLawAbidingCitizens});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowDebugInfo", &featureShowDebugInfo}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCNoLights", &featureNPCNoLights}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCNeonLights", &featureNPCNeonLights}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCFullBeam", &featureNPCFullBeam});
	results->push_back(FeatureEnabledLocalDefinition{"featureDirtyVehicles", &featureDirtyVehicles}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCNoGravityVehicles", &featureNPCNoGravityVehicles}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureNPCReducedGripVehicles", &featureNPCReducedGripVehicles}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureBoostNPCRadio", &featureBoostNPCRadio});
}

void reset_areaeffect_globals(){
	featurePlayerIgnoredByAll = false;
	featureNPCShowHealth = false;
	featureAreaVehiclesBroken = false;
	featureAreaPedsInvincible = false;
	featureAreaPedsHeadExplode = false;
	featureAreaVehiclesExploded = false;
	featureAreaPedsRioting = false;
	featureAngryPedsTargetYou = false;
	featurePedsWeapons = false;
	featureAngryMenOnly = false;
	featureAngryPedsUseCover = false;
	featurePedsIncludeDrivers = false;
	featurePedsIncludePilots = false;
	featureAggressiveDrivers = false;
	featureLawAbidingCitizens = false;
	featureShowDebugInfo = false;
	featureNPCNoLights = false;
	featureNPCNeonLights = false;
	featureNPCFullBeam = false;
	featureDirtyVehicles = false;
	featureNPCNoGravityVehicles = false;
	featureNPCReducedGripVehicles = false;
	featureBoostNPCRadio = false;

	DamagedVehiclesIndex = 0;
	NPCVehicleSpeedIndex = 0;
	PedAccuracyIndex = 0;
	pedWeaponSetIndex = 0;
	
	RadarReducedGripSnowingIndex = 0;
	RadarReducedGripRainingIndex = 0;
	NoPedsGravityIndex = 0;
	PedsHealthIndex = 0;
}

void process_areaeffect_peds_menu(){
	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Peds Angry";
	item->value = -1;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Peds Weapons";
	item->value = -2;
	item->isLeaf = false;
	menuItems.push_back(item);

	listItem = new SelectFromListMenuItem(PLAYER_HEALTH_CAPTIONS, onchange_peds_health_index);
	listItem->wrap = false;
	listItem->caption = "Peds Health";
	listItem->value = PedsHealthIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(WORLD_NPC_VEHICLESPEED_CAPTIONS, onchange_ped_accuracy_index);
	listItem->wrap = false;
	listItem->caption = "Peds Accuracy";
	listItem->value = PedAccuracyIndex;
	menuItems.push_back(listItem);

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Show Current Health";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCShowHealth;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Permanently Calm";
	togItem->value = 1;
	togItem->toggleValue = &featurePlayerIgnoredByAll;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Invincible";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsInvincible;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Everyone Dies";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsHeadExplode;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Effects Include Drivers";
	togItem->value = 1;
	togItem->toggleValue = &featurePedsIncludeDrivers;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Effects Include Pilots";
	togItem->value = 1;
	togItem->toggleValue = &featurePedsIncludePilots;
	menuItems.push_back(togItem);

	listItem = new SelectFromListMenuItem(WORLD_REDUCEDGRIP_SNOWING_CAPTIONS, onchange_world_no_peds_gravity_index);
	listItem->wrap = false;
	listItem->caption = "NPC No Gravity Peds";
	listItem->value = NoPedsGravityIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Aggressive Drivers";
	togItem->value = 1;
	togItem->toggleValue = &featureAggressiveDrivers;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Vigilante Citizens";
	togItem->value = 1;
	togItem->toggleValue = &featureLawAbidingCitizens;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_ped_level_menu_index, "Ped Effects", onconfirm_areaeffect_ped_menu, NULL, NULL);
}

void process_areaeffect_vehicle_menu(){
	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Invincible";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesInvincible;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Abandoned";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesBroken;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "All Vehicles Exploded";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaVehiclesExploded;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Vehicles No Lights";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCNoLights;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Vehicles Have Neon/Xenon Lights";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCNeonLights;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Vehicles Use Full Beam";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCFullBeam;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Dirty Vehicles";
	togItem->value = 1;
	togItem->toggleValue = &featureDirtyVehicles;
	menuItems.push_back(togItem);

	listItem = new SelectFromListMenuItem(WORLD_DAMAGED_VEHICLES_CAPTIONS, onchange_world_damaged_vehicles_index);
	listItem->wrap = false;
	listItem->caption = "NPC Damaged Vehicles";
	listItem->value = DamagedVehiclesIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC No Gravity Vehicles";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCNoGravityVehicles;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC Vehicles Reduced Grip";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCReducedGripVehicles;
	menuItems.push_back(togItem);

	listItem = new SelectFromListMenuItem(WORLD_NPC_VEHICLESPEED_CAPTIONS, onchange_world_npc_vehicles_speed_index);
	listItem->wrap = false;
	listItem->caption = "NPC Vehicles Forced Speed";
	listItem->value = NPCVehicleSpeedIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Boost NPC Radio Volume";
	togItem->value = 1;
	togItem->toggleValue = &featureBoostNPCRadio;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_veh_level_menu_index, "Vehicle Effects", NULL, NULL, NULL);
}

void process_areaeffect_advanced_ped_menu(){
	std::vector<MenuItem<int>*> menuItems;
	
	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Peds Angry";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsRioting;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Angry Peds Use Cover";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsUseCover;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Angry Peds Also Target You";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsTargetYou;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Only Men Are Angry";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryMenOnly;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_ped_advconfig_menu_index, "Peds Angry Options", NULL, NULL, NULL);
}

void process_areaeffect_peds_weapons_menu() {
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Enabled";
	togItem->value = 1;
	togItem->toggleValue = &featurePedsWeapons;
	menuItems.push_back(togItem);

	SelectFromListMenuItem* listItem = new SelectFromListMenuItem(PED_WEAPON_TITLES, onchange_areaeffect_ped_weapons);
	listItem->wrap = false;
	listItem->caption = "Peds Armed With...";
	listItem->value = pedWeaponSetIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(PED_WEAPONS_SELECTIVE_CAPTIONS, onchange_ped_weapons_selective_index);
	listItem->wrap = false;
	listItem->caption = "Custom Weapon";
	listItem->value = PedWeaponsSelectiveIndex;
	menuItems.push_back(listItem);

	draw_generic_menu<int>(menuItems, &areaeffect_peds_weapons_menu_index, "Peds Weapons Options", NULL, NULL, NULL);
}

bool onconfirm_areaeffect_ped_menu(MenuItem<int> choice){
	switch (choice.value) {
	case -1:
		process_areaeffect_advanced_ped_menu();
		break;
	case -2:
		process_areaeffect_peds_weapons_menu();
		break;
	default:
		break;
	}
	return false;
}

bool onconfirm_areaeffect_menu(MenuItem<int> choice){
	switch(choice.value){
		case -1:
			process_areaeffect_peds_menu();
			break;
		case -2:
			process_areaeffect_vehicle_menu();
			break;
	}
	return false;
}

void process_areaeffect_menu(){
	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "People";
	item->value = -1;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Vehicles";
	item->value = -2;
	item->isLeaf = false;
	menuItems.push_back(item);

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Show Area Effect Debug Info";
	togItem->value = 1;
	togItem->toggleValue = &featureShowDebugInfo;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &areaeffect_top_level_menu_index, "Area Effects", onconfirm_areaeffect_menu, NULL, NULL);
}

void do_maintenance_on_tracked_entities(){
	for each (ENTTrackedPedestrian* tped in trackedPeds){
		//only apply this on average every 20 frames to save effort
		int randNum = rand() % 20;
		if(tped->angryApplied && randNum == 1){
			findRandomTargetForPed(tped);
		}
		if (featureAreaPedsRioting && featureAngryPedsTargetYou) {
			PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), true);
			PED::REGISTER_TARGET(tped->ped, PLAYER::PLAYER_PED_ID());
			AI::TASK_COMBAT_PED(tped->ped, PLAYER::PLAYER_PED_ID(), 0, 16);
		}
	}
}

void findRandomTargetForPed(ENTTrackedPedestrian* tped){
	Ped otherPed = 0;

	if (!featureAngryPedsTargetYou) PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), false); // if (trackedPeds.at(randIndex)->ped == PLAYER::PLAYER_PED_ID() && !featureAngryPedsTargetYou) 

	if(tped->lastTarget == 0 || !ENTITY::DOES_ENTITY_EXIST(tped->lastTarget) || ENTITY::IS_ENTITY_DEAD(tped->lastTarget)){
		tped->lastTarget = 0;
		while(tped->lastTarget == 0){
			int randIndex = rand() % (trackedPeds.size() + 1); //add one to the random range
			randIndex--;
			if(randIndex < 0 || featureAngryPedsTargetYou) //chance of fighting the player
			{
				otherPed = PLAYER::PLAYER_PED_ID();
				if (featureAngryPedsTargetYou) PED::SET_PED_AS_ENEMY(otherPed, true);
			}
			else{
				otherPed = trackedPeds.at(randIndex)->ped;
				if (!featureAngryPedsTargetYou && otherPed != PLAYER::PLAYER_PED_ID()) PED::SET_PED_AS_ENEMY(otherPed, true);
			}

			//if we've found ourselves
			if(otherPed == tped->ped){
				continue;
			}

			PED::REGISTER_TARGET(tped->ped, otherPed);
			AI::TASK_COMBAT_PED(tped->ped, otherPed, 0, 16);
			tped->lastTarget = otherPed;
		}
	}
}

void update_area_effects(Ped playerPed){
	//callsPerFrame = 0;

	allWorldVehiclesThisFrameFilled = false;
	allWorldPedsThisFrameFilled = false;
	allWorldVehiclesThisFrame.clear();
	allWorldPedsThisFrame.clear();

	Player player = PLAYER::PLAYER_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	clear_up_missionised_entitities();

	do_maintenance_on_tracked_entities();

	// everyone ignores player
	if(featurePlayerIgnoredByAll || featurePlayerInvisible || (featurePlayerInvisibleInVehicle && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 1))){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, true);
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, true);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, false);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, true);

			set_all_nearby_peds_to_calm();
		}
	}
	if(!featurePlayerIgnoredByAll && !featurePlayerInvisible && !featurePlayerInvisibleInVehicle){
		if(bPlayerExists){
			PLAYER::SET_POLICE_IGNORE_PLAYER(player, is_player_ignored_by_police());
			PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, false);
			PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, true);
			PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, false);
		}
	}

	if(featureAreaPedsInvincible){ 
		if(get_frame_number() % 5 == 0){
			set_all_nearby_peds_to_invincible(featureAreaPedsInvincible);
		}
	}

	if(featureAreaPedsHeadExplode){
		kill_all_nearby_peds_continuous();
	}

	if(featureAreaVehiclesInvincible){ 
		set_all_nearby_vehs_to_invincible(featureAreaVehiclesInvincible, false);
	}

	if(featureAreaVehiclesBroken){ 
		set_all_nearby_vehs_to_broken(featureAreaVehiclesBroken);
	}

	if(featureAreaVehiclesExploded){
		kill_all_nearby_vehicles_continuous();
	}

	if(featureAreaPedsRioting){ 
		set_all_nearby_peds_to_angry(featureAreaPedsRioting);
	}

	if (featurePedsWeapons) {
		if((featureAreaPedsRioting && pedWeaponSetIndex != 0) || pedWeaponSetIndex != 0){
			give_all_nearby_peds_a_weapon(pedWeaponSetIndex); //  != 0
		}
		if ((featureAreaPedsRioting && PedWeaponsSelectiveIndex != 0) || PedWeaponsSelective1Changed || PedWeaponsSelectiveIndex != 0) {
			give_all_nearby_peds_a_weapon(PedWeaponsSelectiveIndex); //  != 0
			PedWeaponsSelective1Changed = false;
		}
	}

	if(featureShowDebugInfo){ 
		show_debug_info_on_screen(featureShowDebugInfo);
	}

	// Aggressive Drivers && Vigilante Citizens
	if (featureAggressiveDrivers || featureLawAbidingCitizens) {
		Vehicle veh_me = PED::GET_VEHICLE_PED_IS_IN(playerPed, false);
		Vector3 veh_me_coords = ENTITY::GET_ENTITY_COORDS(veh_me, true);
		float veh_me_speed = ENTITY::GET_ENTITY_SPEED(veh_me);
		const int arrSize_laws = 1024;
		Vehicle veh_agressive[arrSize_laws];
		int count_veh = worldGetAllPeds(veh_agressive, arrSize_laws);
		for (int i = 0; i < count_veh; i++) {
			if (PED::GET_PED_TYPE(veh_agressive[i]) != 0 && PED::GET_PED_TYPE(veh_agressive[i]) != 1 && PED::GET_PED_TYPE(veh_agressive[i]) != 2 && PED::GET_PED_TYPE(veh_agressive[i]) != 3 &&
				PED::GET_PED_TYPE(veh_agressive[i]) != 6 && PED::GET_PED_TYPE(veh_agressive[i]) != 27 && PED::GET_PED_TYPE(veh_agressive[i]) != 29) {
				Vehicle veh_coll_with = PED::GET_VEHICLE_PED_IS_IN(veh_agressive[i], false);
				Vector3 veh_coll_with_coords = ENTITY::GET_ENTITY_COORDS(veh_coll_with, true);

				int vehcoll_with_dist_x = (veh_me_coords.x - veh_coll_with_coords.x);
				int vehcoll_with_dist_y = (veh_me_coords.y - veh_coll_with_coords.y);
				if (vehcoll_with_dist_x < 0) vehcoll_with_dist_x = (vehcoll_with_dist_x * -1);
				if (vehcoll_with_dist_y < 0) vehcoll_with_dist_y = (vehcoll_with_dist_y * -1);

				if (featureAggressiveDrivers && veh_coll_with != PED::GET_VEHICLE_PED_IS_IN(playerPed, false) && ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(veh_me) && ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(veh_coll_with) &&
					vehcoll_with_dist_x < 5 && vehcoll_with_dist_y < 5) {
					v_collision_check = true;
					if (ENTITY::DOES_ENTITY_EXIST(veh_agressive[i]) && !ENTITY::IS_ENTITY_DEAD(veh_agressive[i])) {
						if (pursuer.empty()) pursuer.push_back(veh_agressive[i]);
						if (v_collided.empty()) v_collided.push_back(veh_coll_with);
						if (!pursuer.empty()) {
							bool found = false;
							for (int k = 0; k < pursuer.size(); k++) {
								if (pursuer[k] == veh_agressive[i]) found = true;
							}
							if (found == false) {
								pursuer.push_back(veh_agressive[i]);
								v_collided.push_back(veh_coll_with);
							}
						}
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pursuer.back(), 1, 1);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(v_collided.back(), 1, 1);
					}
					s_seconds = 0;
				} // aggressive drivers
				
				if (featureLawAbidingCitizens) {
					Vector3 me_coords = ENTITY::GET_ENTITY_COORDS(playerPed, true);
					Vector3 npc_abid_coords = ENTITY::GET_ENTITY_COORDS(veh_agressive[i], true);
					int lawabidped_with_dist_x = (me_coords.x - npc_abid_coords.x);
					int lawabidped_with_dist_y = (me_coords.y - npc_abid_coords.y);
					if (lawabidped_with_dist_x < 0) lawabidped_with_dist_x = (lawabidped_with_dist_x * -1);
					if (lawabidped_with_dist_y < 0) lawabidped_with_dist_y = (lawabidped_with_dist_y * -1);
					if (((PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(playerPed) != 0 && VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(playerPed), -1) != 0) ||
						(PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(playerPed) != 0 && VEHICLE::IS_VEHICLE_ALARM_ACTIVATED(PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(playerPed))))
						&& ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(veh_agressive[i], playerPed, 17)) {
						if (lawabidped_with_dist_x < 10 && lawabidped_with_dist_y < 10) {
							if (!PED::IS_PED_IN_ANY_VEHICLE(veh_agressive[i], true)/* && VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(playerPed), -1) != veh_agressive[i]*/) {
								PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), true);
								PED::REGISTER_TARGET(veh_agressive[i], PLAYER::PLAYER_PED_ID());
								AI::TASK_COMBAT_PED(veh_agressive[i], PLAYER::PLAYER_PED_ID(), 0, 16);
								AUDIO::_PLAY_AMBIENT_SPEECH1(veh_agressive[i], "PROVOKE_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
							}
							time_to_chase = true;
						}
					}
					if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, true) && PED::IS_PED_IN_ANY_VEHICLE(veh_agressive[i], true) &&
						lawabidped_with_dist_x < 10 && lawabidped_with_dist_y < 10 && time_to_chase == true && veh_me_speed < 1) {
						if (pursuer.empty()) pursuer.push_back(veh_agressive[i]);
						if (v_collided.empty()) v_collided.push_back(PED::GET_VEHICLE_PED_IS_IN(veh_agressive[i], false));
						if (!pursuer.empty()) {
							bool found = false;
							for (int k = 0; k < pursuer.size(); k++) {
								if (pursuer[k] == veh_agressive[i]) found = true;
							}
							if (found == false) {
								pursuer.push_back(veh_agressive[i]);
								v_collided.push_back(PED::GET_VEHICLE_PED_IS_IN(veh_agressive[i], false));
							}
						}
						PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), true);
						PED::REGISTER_TARGET(pursuer.back(), PLAYER::PLAYER_PED_ID());
						AI::TASK_COMBAT_PED(pursuer.back(), PLAYER::PLAYER_PED_ID(), 0, 16);
						AUDIO::_PLAY_AMBIENT_SPEECH1(pursuer.back(), "PROVOKE_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pursuer.back(), 1, 1);
						ENTITY::SET_ENTITY_AS_MISSION_ENTITY(v_collided.back(), 1, 1);
						AI::SET_DRIVE_TASK_CRUISE_SPEED(pursuer.back(), 300.0);
						AI::TASK_VEHICLE_CHASE(pursuer.back(), playerPed);
						AI::SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(pursuer.back(), 60.0f);
						AI::SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(pursuer.back(), 32, true);
						PED::SET_DRIVER_ABILITY(pursuer.back(), 0.9f);
						AUDIO::_PLAY_AMBIENT_SPEECH1(pursuer.back(), "PROVOKE_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
						time_to_attack = false;
					}
					if (veh_me_speed > 1/* || !PED::IS_PED_IN_ANY_VEHICLE(playerPed, true)*/) time_to_chase = false; 
				} // vigilante citizens
			}
		} // end of for
		if (v_collision_check == true) {
			AI::SET_DRIVE_TASK_CRUISE_SPEED(pursuer.back(), 300.0);
			AI::TASK_VEHICLE_CHASE(pursuer.back(), playerPed);
			AI::SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(pursuer.back(), 60.0f);
			AI::SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(pursuer.back(), 32, true);
			PED::SET_DRIVER_ABILITY(pursuer.back(), 0.9f);
			AUDIO::_PLAY_AMBIENT_SPEECH1(pursuer.back(), "PROVOKE_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
			time_to_attack = false;
			v_collision_check = false;
		}
		if (time_to_attack == false && veh_me_speed < 1) {
			s_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / CLOCKS_PER_SEC) - s_secs_curr) != 0) {
				s_seconds = s_seconds + 1;
				s_secs_curr = s_secs_passed;
			}
		}
		if (veh_me_speed > 1) s_seconds = 0;
		if ((!PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) || s_seconds > 3) && time_to_attack == false) {
			for (int j = 0; j < pursuer.size(); j++) {
				if (ENTITY::DOES_ENTITY_EXIST(pursuer[j]) && !ENTITY::IS_ENTITY_DEAD(pursuer[j])) {
					PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), true);
					PED::REGISTER_TARGET(pursuer[j], PLAYER::PLAYER_PED_ID());
					AI::TASK_COMBAT_PED(pursuer[j], PLAYER::PLAYER_PED_ID(), 0, 16);
				}
			}
			s_seconds = 0;
			time_to_attack = true;
		}
		if (((PLAYER::GET_TIME_SINCE_LAST_DEATH() > 100 && PLAYER::GET_TIME_SINCE_LAST_DEATH() < 5000) || (PLAYER::GET_TIME_SINCE_LAST_ARREST() > 100 && PLAYER::GET_TIME_SINCE_LAST_ARREST() < 5000) || player_died == true) && !pursuer.empty()) {
			for (int j = 0; j < pursuer.size(); j++) {
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&pursuer[j]);
				if (ENTITY::DOES_ENTITY_EXIST(pursuer[j])) PED::DELETE_PED(&pursuer[j]);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&v_collided[j]);
				if (ENTITY::DOES_ENTITY_EXIST(v_collided[j])) VEHICLE::DELETE_VEHICLE(&v_collided[j]);
				player_died = false;
			}
			pursuer.clear();
			pursuer.shrink_to_fit();
			v_collided.clear();
			v_collided.shrink_to_fit();
		}
		if (!pursuer.empty() && pursuer.size() > 10) { // 10
			ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&pursuer[0]);
			PED::DELETE_PED(&pursuer[0]);
			pursuer.erase(pursuer.begin());
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&v_collided[0]);
			VEHICLE::DELETE_VEHICLE(&v_collided[0]);
			v_collided.erase(v_collided.begin());
		}
	} // end of aggressive drivers && vigilante citizens

}

void show_debug_info_on_screen(bool enabled){
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0);
	std::ostringstream ss;
	ss << "Peds: " << trackedPeds.size() << "; Vehs: " << trackedVehicles.size() << "\nCalls Total: " << callsPerFrame << ", A: " << callsA << ", B: " << callsB << "\nWP: " << allWorldPedsThisFrame.size() << ", WV: " << allWorldVehiclesThisFrame.size();
	ss << "\nX: " << coords.x << "\nY: " << coords.y << "\nZ: " << coords.z;
	callsPerFrame = 0;
	set_status_text_centre_screen(ss.str());
}

void set_all_nearby_peds_to_calm(){
	//const int IGN_ARR_PED_SIZE = 1024;
	//Ped ign_ped[IGN_ARR_PED_SIZE];
	//int found_ign_ped = worldGetAllPeds(ign_ped, IGN_ARR_PED_SIZE);
	//for (int i = 0; i < found_ign_ped; i++) {
	//	if (ign_ped[i] != PLAYER::PLAYER_PED_ID()) {
	//		if (!PED::IS_PED_GROUP_MEMBER(ign_ped[i], PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))) { // Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
	//			PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ign_ped[i], true);
	//			PED::SET_PED_FLEE_ATTRIBUTES(ign_ped[i], 0, 0);
	//		}
	//	}
	//}
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());
	for each (Ped xped in peds) {
		if (xped != PLAYER::PLAYER_PED_ID()) {
			if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))) { // Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);
				PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);
			}
		}
	}
}

void set_all_nearby_peds_to_invincible(bool enabled){
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());
	for each (Ped xped in peds){
		// Only apply to non-bodyguards
		if(!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))){
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			if((enabled && !trackedPed->madeInvincible) || (!enabled && trackedPed->madeInvincible)){
				PED::SET_PED_DIES_WHEN_INJURED(xped, !enabled);

				PED::SET_PED_MAX_HEALTH(xped, enabled ? 10000 : 100);
				ENTITY::SET_ENTITY_HEALTH(xped, enabled ? 10000 : 100);
				PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, !enabled);

				PED::SET_PED_COMBAT_ABILITY(xped, enabled ? 1 : 0);
				ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, !enabled);

				trackedPed->madeInvincible = enabled;
			}
		}
	}
}

void set_all_nearby_peds_to_angry(bool enabled){  
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds){
		// Only apply to non-bodyguards
		if(!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))){
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);

			if(enabled && !trackedPed->angryApplied){
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 0, featureAngryPedsUseCover ? 1 : 0); //use cover

				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 46, 1); //always fight
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 5, 1); //fight armed peds when unarmed
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 3, featurePedsIncludeDrivers ? 1 : 0); //can leave vehicle
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 2, featurePedsIncludeDrivers ? 1 : 0); //can do driveby
				PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);

				findRandomTargetForPed(trackedPed);

				trackedPed->angryApplied = true;
				trackedPed->missionise();
			}
			else if(!enabled && trackedPed->angryApplied){
				//stop them being angry
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 0, 0); //use cover
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 46, 0); //always fight
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 5, 0); //fight armed peds when unarmed 
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 3, 0); //can leave vehicle
				PED::SET_PED_COMBAT_ATTRIBUTES(xped, 2, 0); //can do driveby

				PED::SET_PED_AS_ENEMY(xped, enabled);
				PED::SET_PED_FLEE_ATTRIBUTES(xped, 0, 0);

				trackedPed->angryApplied = false;
				trackedPed->demissionise();
				releasedPeds.insert(xped);
			}
		}
	}
}

void set_all_nearby_vehs_to_invincible(bool enabled, bool force){
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle veh in vehicles){
		int chanceOfSelection = rand() % 5;
		if(chanceOfSelection != 1 || force){
			continue;
		}

		if(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == veh){
			continue;
		}

		ENTITY::SET_ENTITY_HEALTH(veh, 10000.0f);
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 10000.0);
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(veh, 10000.0);

		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(veh, enabled);
		VEHICLE::SET_VEHICLE_WHEELS_CAN_BREAK(veh, enabled);

		VEHICLE::SET_VEHICLE_CAN_BREAK(veh, !enabled);
		ENTITY::SET_ENTITY_INVINCIBLE(veh, enabled);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(veh, !enabled);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(veh, !enabled);

		for(int i = 0; i < 6; i++){
			VEHICLE::_SET_VEHICLE_DOOR_BREAKABLE(veh, i, !enabled); //(Vehicle, doorIndex, isBreakable)
		}

		if(enabled){
			ENTITY::SET_ENTITY_ONLY_DAMAGED_BY_PLAYER(veh, 1);

			VEHICLE::SET_VEHICLE_BODY_HEALTH(veh, 10000.0f);
		}
	}
}

void set_all_nearby_vehs_to_broken(bool enabled){
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle veh in vehicles){
		int chanceOfSelection = rand() % 5;
		if(chanceOfSelection != 1){
			continue;
		}

		if(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) == veh){
			continue;
		}

		BOOL isAircraft = is_this_a_heli_or_plane(veh);
		BOOL isWeird = is_this_a_bicycle(veh) || is_this_a_boat_or_sub(veh) || is_this_a_train(veh);
		if(isAircraft || isWeird){
			continue;
		}

		if(enabled){
			VEHICLE::SET_VEHICLE_ENGINE_ON(veh, false, true);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, true);

			Object taskHdl;

			for(int i = -1; i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh); i++){
				Ped passenger = VEHICLE::GET_PED_IN_VEHICLE_SEAT(veh, i);
				if(ENTITY::DOES_ENTITY_EXIST(passenger)){
					if(passenger != PLAYER::PLAYER_PED_ID()){
						AI::CLEAR_PED_TASKS(passenger);

						AI::OPEN_SEQUENCE_TASK(&taskHdl);
						AI::TASK_LEAVE_VEHICLE(passenger, veh, 1);
						AI::TASK_WANDER_STANDARD(passenger, 100.0, 1);
						AI::CLOSE_SEQUENCE_TASK(taskHdl);

						AI::TASK_PERFORM_SEQUENCE(passenger, taskHdl);
						AI::CLEAR_SEQUENCE_TASK(&taskHdl);
					}
				}
			}
		}
		else{
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(veh, false);
		}
	}
}

std::set<Ped> get_nearby_peds(Ped playerPed){
	if(allWorldPedsThisFrameFilled){
		return allWorldPedsThisFrame;
	}

	const int ARR_SIZE = 1024;
	Ped peds[ARR_SIZE];
	int found = worldGetAllPeds(peds, ARR_SIZE);

	callsA = 0;
	callsB = found;

	for(int i = 0; i < found; i++){
		Ped item = peds[i];

		if(releasedPeds.find(item) != releasedPeds.end()){
			continue;
		}

		if(ENTITY::IS_ENTITY_DEAD(item)){
			continue;
		}
		else if(playerPed == item){
			continue;
		}
		else if(!PED::IS_PED_HUMAN(item)){
			continue;
		}
		else if(ENTITY::IS_ENTITY_A_MISSION_ENTITY(item) && !ENTITY::DOES_ENTITY_BELONG_TO_THIS_SCRIPT(item, true)){
			continue;
		}
		else if (featureAngryMenOnly && PED::GET_PED_TYPE(item) == 5) {
			continue;
		}

		//filter out drivers/pilots if necessary
		if((!featurePedsIncludePilots || !featurePedsIncludeDrivers) && PED::IS_PED_IN_ANY_VEHICLE(item, false)){
			Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(item, false);

			bool isAircraft = is_this_a_heli_or_plane(veh);
			if(!featurePedsIncludePilots && isAircraft){
				continue;
			}

			bool isWeird = is_this_a_bicycle(veh) || is_this_a_boat_or_sub(veh) || is_this_a_train(veh);
			if(!featurePedsIncludeDrivers && !isAircraft && !isWeird){
				continue;
			}
		}

		allWorldPedsThisFrame.insert(item);
	}

	allWorldPedsThisFrameFilled = true;
	return allWorldPedsThisFrame;
}

std::set<Vehicle> get_nearby_vehicles(Ped playerPed){
	if(allWorldVehiclesThisFrameFilled){
		return allWorldVehiclesThisFrame;
	}

	BOOL playerInVehicle = PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0);
	Vehicle playerVehicle = playerInVehicle ? PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()) : 0;

	const int ARR_SIZE = 1024;
	Vehicle vehs[ARR_SIZE];
	int found = worldGetAllVehicles(vehs, ARR_SIZE);

	for(int i = 0; i < found; i++){
		Vehicle item = vehs[i];

		if(releasedVehicles.find(item) != releasedVehicles.end()){
			continue;
		}

		//don't return dead items
		if(ENTITY::IS_ENTITY_DEAD(item)){
			continue;
		}
		//don't return the vehicle we're using
		else if(playerInVehicle && playerVehicle == item){
			continue;
		}
		//don't do stuff to mission entities
		else if(ENTITY::IS_ENTITY_A_MISSION_ENTITY(item) && !ENTITY::DOES_ENTITY_BELONG_TO_THIS_SCRIPT(item, true)){
			continue;
		}

		allWorldVehiclesThisFrame.insert(item);
	}

	allWorldVehiclesThisFrameFilled = true;
	return allWorldVehiclesThisFrame;
}

void cleanup_area_effects(){
	for each (ENTTrackedVehicle* veh in trackedVehicles){
		delete veh;
	}
	trackedVehicles.clear();

	for each (ENTTrackedPedestrian* ped in trackedPeds){
		delete ped;
	}
	trackedPeds.clear();
}

void kill_all_nearby_peds_now(){
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds){
		if(!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))){
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(xped, true, true);
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			trackedPed->missionise();

			//remove invincibility
			PED::SET_PED_DIES_WHEN_INJURED(xped, true);
			PED::SET_PED_MAX_HEALTH(xped, 1);
			ENTITY::SET_ENTITY_HEALTH(xped, 1);
			PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, true);
			PED::SET_PED_COMBAT_ABILITY(xped, 1);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, true);

			PED::EXPLODE_PED_HEAD(xped, GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE"));
		}
	}
}

void kill_all_nearby_peds_continuous(){
	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	for each (Ped xped in peds){
		int chanceOfSelection = rand() % 5;
		if(chanceOfSelection != 1){
			continue;
		}

		if(!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))){
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(xped, true, true);
			ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);
			trackedPed->missionise();

			//remove invincibility
			PED::SET_PED_DIES_WHEN_INJURED(xped, true);
			PED::SET_PED_MAX_HEALTH(xped, 1);
			ENTITY::SET_ENTITY_HEALTH(xped, 1);
			PED::SET_PED_SUFFERS_CRITICAL_HITS(xped, true);
			PED::SET_PED_COMBAT_ABILITY(xped, 1);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(xped, true);

			PED::EXPLODE_PED_HEAD(xped, GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE"));
		}
	}
}

void kill_all_nearby_vehicles_now(){
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	set_all_nearby_vehs_to_invincible(false, true);

	for each (Vehicle vehicle in vehicles){
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		ENTTrackedVehicle* trackedVeh = findOrCreateTrackedVehicle(vehicle);
		trackedVeh->missionise();

		VEHICLE::EXPLODE_VEHICLE(vehicle, true, false);
	}
}

void kill_all_nearby_vehicles_continuous(){
	std::set<Vehicle> vehicles = get_nearby_vehicles(PLAYER::PLAYER_PED_ID());

	for each (Vehicle vehicle in vehicles){
		int chanceOfBeingExploded = rand() % 5;
		if(chanceOfBeingExploded != 1){
			continue;
		}

		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(vehicle, true, true);
		ENTTrackedVehicle* trackedVeh = findOrCreateTrackedVehicle(vehicle);
		trackedVeh->missionise();

		VEHICLE::EXPLODE_VEHICLE(vehicle, true, false);
	}
}

void clear_up_missionised_entitities(){
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(playerPed);

	std::deque<ENTTrackedVehicle*>::iterator vit;
	for(vit = trackedVehicles.begin(); vit != trackedVehicles.end();){
		Vehicle actualVeh = (*vit)->vehicle;
		if(!ENTITY::DOES_ENTITY_EXIST(actualVeh) || is_entity_to_be_disposed(playerPed, actualVeh)){
			delete *vit;
			releasedVehicles.insert(actualVeh);
			vit = trackedVehicles.erase(vit);
		}
		else{
			++vit;
		}
	}

	std::deque<ENTTrackedPedestrian*>::iterator pit;
	for(pit = trackedPeds.begin(); pit != trackedPeds.end();){
		Ped actualPed = (*pit)->ped;
		if(!ENTITY::DOES_ENTITY_EXIST(actualPed) || is_entity_to_be_disposed(playerPed, actualPed)){
			delete *pit;
			releasedPeds.insert(actualPed);
			pit = trackedPeds.erase(pit);
		}
		else{
			++pit;
		}
	}

	std::set<Ped>::iterator rpIt;
	for(rpIt = releasedPeds.begin(); rpIt != releasedPeds.end();){
		if(!ENTITY::DOES_ENTITY_EXIST(*rpIt)){
			rpIt = releasedPeds.erase(rpIt);
		}
		else if(!is_entity_to_be_disposed(playerPed, *rpIt)) //no longer for deletion after all
		{
			rpIt = releasedPeds.erase(rpIt);
		}
		else{
			rpIt++;
		}
	}

	std::set<Vehicle>::iterator rvIt;
	for(rvIt = releasedVehicles.begin(); rvIt != releasedVehicles.end();){
		if(!ENTITY::DOES_ENTITY_EXIST(*rvIt)){
			rvIt = releasedVehicles.erase(rvIt);
		}
		else if(!is_entity_to_be_disposed(playerPed, *rvIt)) //no longer for deletion after all
		{
			rvIt = releasedVehicles.erase(rvIt);
		}
		else{
			rvIt++;
		}
	}
}

void onchange_areaeffect_ped_weapons(int value, SelectFromListMenuItem* source){
	pedWeaponSetIndex = value;
}

void onchange_world_damaged_vehicles_index(int value, SelectFromListMenuItem* source) {
	DamagedVehiclesIndex = value;
	DamagedVehiclesChanged = true;
}

void onchange_world_npc_vehicles_speed_index(int value, SelectFromListMenuItem* source) {
	NPCVehicleSpeedIndex = value;
	NPCVehicleSpeedChanged = true;
}

void onchange_world_reducedgrip_snowing_index(int value, SelectFromListMenuItem* source) {
	RadarReducedGripSnowingIndex = value;
	RadarReducedGripSnowingChanged = true;
}

void onchange_world_reducedgrip_raining_index(int value, SelectFromListMenuItem* source) {
	RadarReducedGripRainingIndex = value;
	RadarReducedGripRainingChanged = true;
}

void onchange_peds_health_index(int value, SelectFromListMenuItem* source) {
	PedsHealthIndex = value;
	PedsHealthChanged = true;
}

void onchange_world_no_peds_gravity_index(int value, SelectFromListMenuItem* source) {
	NoPedsGravityIndex = value;
	NoPedsGravityChanged = true;
}

void onchange_ped_accuracy_index(int value, SelectFromListMenuItem* source) {
	PedAccuracyIndex = value;
	PedAccuracyChanged = true;
}

void onchange_ped_weapons_selective_index(int value, SelectFromListMenuItem* source){
	PedWeaponsSelectiveIndex = value;
	PedWeaponsSelective1Changed = true;
}

void give_all_nearby_peds_a_weapon(bool enabled){ 
	//callsPerFrame = 0;

	std::set<Ped> peds = get_nearby_peds(PLAYER::PLAYER_PED_ID());

	//callsA = callsPerFrame;
	//callsPerFrame = 0;

	for each (Ped xped in peds){
		int chanceOfGettingWeapon = rand() % 5;
		if(chanceOfGettingWeapon != 1){
			continue;
		}
		if (PED_WEAPON_TITLES[pedWeaponSetIndex] != "Custom Weapon")
		{
			if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))){
				ENTTrackedPedestrian* trackedPed = findOrCreateTrackedPed(xped);

				if (enabled && !trackedPed->weaponSetApplied != pedWeaponSetIndex){
					std::vector<std::string> weaponSet = VOV_PED_WEAPONS[pedWeaponSetIndex];

					int index = rand() % weaponSet.size();
					std::string weapon = weaponSet.at(index);
					Hash weapHash = GAMEPLAY::GET_HASH_KEY((char *)weapon.c_str());

					bool foundWeapon = false;
					for (std::string searchStr : weaponSet){
						Hash searchHash = GAMEPLAY::GET_HASH_KEY((char *)searchStr.c_str());
						if (trackedPed->lastWeaponApplied == searchHash){
							foundWeapon = true;
							break;
						}
					}

					if (!foundWeapon){
						WEAPON::GIVE_WEAPON_TO_PED(xped, weapHash, 9999, FALSE, TRUE);
						if (WEAPON::HAS_PED_GOT_WEAPON(xped, weapHash, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false) && !PED::IS_PED_INJURED(xped)) WEAPON::SET_CURRENT_PED_WEAPON(xped, weapHash, 0);
						WEAPON::SET_PED_INFINITE_AMMO_CLIP(xped, true);
						PED::SET_PED_CAN_SWITCH_WEAPON(xped, true);
						trackedPed->lastWeaponApplied = weapHash;
					}

					trackedPed->weaponSetApplied = pedWeaponSetIndex;
				}
				else if (!enabled && trackedPed->weaponSetApplied != 0){
					//TODO: take all their weapons from that set away?

					trackedPed->weaponSetApplied = 0;
				}
			}
		}
		else
		{
			if (!PED::IS_PED_GROUP_MEMBER(xped, PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))) {
				char *currWeapon = new char[PED_WEAPONS_SELECTIVE_CAPTIONS[PedWeaponsSelectiveIndex].length() + 1];
				strcpy(currWeapon, PED_WEAPONS_SELECTIVE_CAPTIONS[PedWeaponsSelectiveIndex].c_str());
				Hash Ped_Selective_Weapon = GAMEPLAY::GET_HASH_KEY(currWeapon);
				if (!featurePedsIncludeDrivers && !WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false)) WEAPON::GIVE_WEAPON_TO_PED(xped, Ped_Selective_Weapon, 9999, FALSE, TRUE);
				if (featurePedsIncludeDrivers && !WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0)) WEAPON::GIVE_WEAPON_TO_PED(xped, Ped_Selective_Weapon, 9999, FALSE, TRUE);
				if (!featurePedsIncludeDrivers && WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false)) WEAPON::SET_CURRENT_PED_WEAPON(xped, Ped_Selective_Weapon, 0);
				if (featurePedsIncludeDrivers && WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0)) WEAPON::SET_CURRENT_PED_WEAPON(xped, Ped_Selective_Weapon, 0);
				WEAPON::SET_PED_INFINITE_AMMO_CLIP(xped, true);
				PED::SET_PED_CAN_SWITCH_WEAPON(xped, true);
			}
		}
	}
}

void add_areaeffect_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"pedWeaponSetIndex", std::to_string(pedWeaponSetIndex)});
	results->push_back(StringPairSettingDBRow{"PedWeaponsSelectiveIndex", std::to_string(PedWeaponsSelectiveIndex)});
}

void handle_generic_settings_areaeffect(std::vector<StringPairSettingDBRow>* settings){
	for(int i = 0; i < settings->size(); i++){
		StringPairSettingDBRow setting = settings->at(i);
		if(setting.name.compare("pedWeaponSetIndex") == 0){
			pedWeaponSetIndex = stoi(setting.value);
		}
		else if (setting.name.compare("PedWeaponsSelectiveIndex") == 0){
			PedWeaponsSelectiveIndex = stoi(setting.value);
			PedWeaponsSelective1Changed = true;
		}
	}
}

ENTTrackedPedestrian* findOrCreateTrackedPed(Ped searchPed){
	std::deque<ENTTrackedPedestrian*>::iterator pit;
	for(pit = trackedPeds.begin(); pit != trackedPeds.end(); pit++){
		Ped queuePed = (*pit)->ped;

		if(searchPed == queuePed){
			return *pit;
		}
	}
	//if nowt found
	ENTTrackedPedestrian* result = new ENTTrackedPedestrian(searchPed);
	trackedPeds.push_back(result);

	return result;
}

ENTTrackedVehicle* findOrCreateTrackedVehicle(Vehicle searchVeh){
	std::deque<ENTTrackedVehicle*>::iterator vit;
	for(vit = trackedVehicles.begin(); vit != trackedVehicles.end(); vit++){
		Vehicle queueVeh = (*vit)->vehicle;
		if(searchVeh == queueVeh){
			return *vit;
		}
	}
	//if nowt found
	ENTTrackedVehicle* result = new ENTTrackedVehicle(searchVeh);
	trackedVehicles.push_back(result);
	return result;
}


bool is_entity_to_be_disposed(Ped playerPed, Entity entity){
	return (!ENTITY::IS_ENTITY_ON_SCREEN(entity) && !ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(playerPed, entity, 17));
}
