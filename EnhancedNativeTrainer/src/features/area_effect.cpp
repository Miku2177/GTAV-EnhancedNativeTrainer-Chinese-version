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

int callsA = 0;
int callsB = 0;
int callsC = 0;
int callsD = 0;
int callsE = 0;

std::deque<ENTTrackedPedestrian*> trackedPeds;
std::deque<ENTTrackedVehicle*> trackedVehicles;

//features
bool featurePlayerIgnoredByAll = false;
bool featureAreaPedsInvincible = false;
bool featureAreaVehiclesInvincible = false;
bool featureAreaVehiclesBroken = false;
bool featureAreaPedsRioting = false;
bool featureAreaVehiclesExploded = false;
bool featureAreaPedsHeadExplode = false;
bool featureAngryPedsUseCover = false;
bool featureAngryPedsTargetYou = false;
bool featurePedsIncludeDrivers = false;
bool featurePedsIncludePilots = false;
bool featureNPCNoLights = false;
bool featureNPCNeonLights = false;
bool featureNPCFullBeam = false;
bool featureDirtyVehicles = false;
bool featureNPCNoGravityVehicles = false;
bool featureNPCNoGravityPeds = false;
bool featureNPCReducedGripVehicles = false;
bool featureBoostNPCRadio = false;

int pedWeaponSetIndex = 0;

std::set<Ped> allWorldPedsThisFrame;
bool allWorldPedsThisFrameFilled = false;

std::set<Vehicle> allWorldVehiclesThisFrame;
bool allWorldVehiclesThisFrameFilled = false;

std::set<Ped> releasedPeds;
std::set<Vehicle> releasedVehicles;

const std::vector<std::string> PED_WEAPONS_SELECTIVE_CAPTIONS{ "\"WEAPON_UNARMED\"", "\"WEAPON_NIGHTSTICK\"", "\"WEAPON_FLASHLIGHT\"", "\"WEAPON_KNIFE\"", "\"WEAPON_DAGGER\"", "\"WEAPON_HAMMER\"", "\"WEAPON_BAT\"", "\"WEAPON_GOLFCLUB\"",
"\"WEAPON_CROWBAR\"", "\"WEAPON_POOLCUE\"", "\"WEAPON_WRENCH\"", "\"WEAPON_MACHETE\"", "\"WEAPON_BOTTLE\"", "\"WEAPON_PISTOL\"", "\"WEAPON_APPISTOL\"", "\"WEAPON_REVOLVER\"", "\"WEAPON_STUNGUN\"", "\"WEAPON_FLAREGUN\"",
"\"WEAPON_MACHINEPISTOL\"", "\"WEAPON_MARKSMANPISTOL\"", "\"WEAPON_MINISMG\"", "\"WEAPON_ASSAULTSMG\"", "\"WEAPON_ASSAULTRIFLE\"", "\"WEAPON_CARBINERIFLE\"", "\"WEAPON_ADVANCEDRIFLE\"", "\"WEAPON_COMPACTRIFLE\"", "\"WEAPON_HEAVYSHOTGUN\"",
"\"WEAPON_DBSHOTGUN\"", "\"WEAPON_AUTOSHOTGUN\"", "\"WEAPON_MUSKET\"", "\"WEAPON_SAWNOFFSHOTGUN\"", "\"WEAPON_COMBATMG\"", "\"WEAPON_MINIGUN\"", "\"WEAPON_GUSENBERG\"", "\"WEAPON_SNIPERRIFLE\"", "\"WEAPON_HEAVYSNIPER\"",
"\"WEAPON_GRENADELAUNCHER\"", "\"WEAPON_GRENADELAUNCHER_SMOKE\"", "\"WEAPON_RPG\"", "\"WEAPON_HOMINGLAUNCHER\"", "\"WEAPON_COMPACTLAUNCHER\"", "\"WEAPON_RAILGUN\"", "\"WEAPON_FIREWORK\"" };
//const int PED_WEAPONS_SELECTIVE_VALUES[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44 };
int PedWeaponsSelectiveIndex = 0;
bool PedWeaponsSelective1Changed = true;

// NPC Damaged Vehicles
const std::vector<std::string> WORLD_DAMAGED_VEHICLES_CAPTIONS{ "OFF", "5", "7", "10", "12", "15", "20", "30", "50", "100", "500", "1000" };
const int WORLD_DAMAGED_VEHICLES_VALUES[] = { 0, 5, 7, 10, 12, 15, 20, 30, 50, 100, 500, 1000 };
int DamagedVehiclesIndex = 0;
bool DamagedVehiclesChanged = true;

// NPC Vehicle Speed
const std::vector<std::string> WORLD_NPC_VEHICLESPEED_CAPTIONS{ "OFF", "0.001", "1", "3", "5", "10", "15", "30", "50", "70", "100" };
const float WORLD_NPC_VEHICLESPEED_VALUES[] = { 0.0, 0.001, 1.0, 3.0, 5.0, 10.0, 15.0, 30.0, 50.0, 70.0, 100.0 };
int NPCVehicleSpeedIndex = 0;
bool NPCVehicleSpeedChanged = true;
int PedAccuracyIndex = 0;
bool PedAccuracyChanged = true;

//For onscreen debug info
bool featureShowDebugInfo = false;

void add_areaeffect_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featurePlayerIgnoredByAll", &featurePlayerIgnoredByAll}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsInvincible", &featureAreaPedsInvincible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsHeadExplode", &featureAreaPedsHeadExplode});
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesInvincible", &featureAreaVehiclesInvincible}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesBroken", &featureAreaVehiclesBroken}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaVehiclesExploded", &featureAreaVehiclesExploded});
	results->push_back(FeatureEnabledLocalDefinition{"featureAreaPedsRioting", &featureAreaPedsRioting}); 
	results->push_back(FeatureEnabledLocalDefinition{"featureAngryPedsUseCover", &featureAngryPedsUseCover});
	results->push_back(FeatureEnabledLocalDefinition{"featureAngryPedsTargetYou", &featureAngryPedsTargetYou});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedsIncludeDrivers", &featurePedsIncludeDrivers});
	results->push_back(FeatureEnabledLocalDefinition{"featurePedsIncludePilots", &featurePedsIncludePilots});
	results->push_back(FeatureEnabledLocalDefinition{"featureShowDebugInfo", &featureShowDebugInfo}); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCNoLights", &featureNPCNoLights }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCNeonLights", &featureNPCNeonLights }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCFullBeam", &featureNPCFullBeam });
	results->push_back(FeatureEnabledLocalDefinition{ "featureDirtyVehicles", &featureDirtyVehicles }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCNoGravityVehicles", &featureNPCNoGravityVehicles }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCNoGravityPeds", &featureNPCNoGravityPeds }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureNPCReducedGripVehicles", &featureNPCReducedGripVehicles }); 
	results->push_back(FeatureEnabledLocalDefinition{ "featureBoostNPCRadio", &featureBoostNPCRadio });
}

void reset_areaeffect_globals(){
	featurePlayerIgnoredByAll = false;
	featureAreaVehiclesBroken = false;
	featureAreaPedsInvincible = false;
	featureAreaPedsHeadExplode = false;
	featureAreaVehiclesExploded = false;
	featureAreaPedsRioting = false;
	featureAngryPedsTargetYou = false;
	featureAngryPedsUseCover = false;
	featurePedsIncludeDrivers = false;
	featurePedsIncludePilots = false;
	featureShowDebugInfo = false;
	featureNPCNoLights = false;
	featureNPCNeonLights = false;
	featureNPCFullBeam = false;
	featureDirtyVehicles = false;
	featureNPCNoGravityVehicles = false;
	featureNPCNoGravityPeds = false;
	featureNPCReducedGripVehicles = false;
	featureBoostNPCRadio = false;

	DamagedVehiclesIndex = 0;
	NPCVehicleSpeedIndex = 0;
	PedAccuracyIndex = 0;
	pedWeaponSetIndex = 0;
}

void process_areaeffect_peds_menu(){
	std::vector<MenuItem<int>*> menuItems;

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
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

	listItem = new SelectFromListMenuItem(WORLD_NPC_VEHICLESPEED_CAPTIONS, onchange_ped_accuracy_index);
	listItem->wrap = false;
	listItem->caption = "Peds Accuracy";
	listItem->value = PedAccuracyIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Peds Angry";
	togItem->value = 1;
	togItem->toggleValue = &featureAreaPedsRioting;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "NPC No Gravity Peds";
	togItem->value = 1;
	togItem->toggleValue = &featureNPCNoGravityPeds;
	menuItems.push_back(togItem);

	MenuItem<int> *item = new MenuItem<int>();
	item->caption = "Advanced Config";
	item->value = -1;
	item->isLeaf = false;
	menuItems.push_back(item);

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
	togItem->caption = "Angry Peds Use Cover";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsUseCover;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Angry Peds All Target You";
	togItem->value = 1;
	togItem->toggleValue = &featureAngryPedsTargetYou;
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

	draw_generic_menu<int>(menuItems, &areaeffect_ped_advconfig_menu_index, "Advanced Ped Config", NULL, NULL, NULL);
}

bool onconfirm_areaeffect_ped_menu(MenuItem<int> choice){
	switch (choice.value) {
	case -1:
		process_areaeffect_advanced_ped_menu();
		break;
	default:
		break;
	}
	return false;
	//if(choice.value == 1) //advance options
	//{
	//	process_areaeffect_advanced_ped_menu();
	//}
	//return false;
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
	item->caption = "Pedestrians";
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

	//		{ "Everyone Ignores You", &featurePlayerIgnoredByAll, &featurePlayerIgnoredByAllUpdated, true },
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
	if(tped->lastTarget == 0 || !ENTITY::DOES_ENTITY_EXIST(tped->lastTarget) || ENTITY::IS_ENTITY_DEAD(tped->lastTarget)){
		tped->lastTarget = 0;
		while(tped->lastTarget == 0){
			int randIndex = rand() % (trackedPeds.size() + 1); //add one to the random range
			randIndex--;
			if(randIndex < 0 || featureAngryPedsTargetYou) //chance of fighting the player
			{
				otherPed = PLAYER::PLAYER_PED_ID();
				PED::SET_PED_AS_ENEMY(otherPed, true);
			}
			else{
				otherPed = trackedPeds.at(randIndex)->ped;
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
			//if(get_frame_number() % 99 == 0){
				set_all_nearby_peds_to_calm();
			//}
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

	if((featureAreaPedsRioting && pedWeaponSetIndex != 0) || pedWeaponSetIndex != 0){
		give_all_nearby_peds_a_weapon(pedWeaponSetIndex); //  != 0
	}

	if ((featureAreaPedsRioting && PedWeaponsSelectiveIndex != 0) || PedWeaponsSelective1Changed || PedWeaponsSelectiveIndex != 0){
		give_all_nearby_peds_a_weapon(PedWeaponsSelectiveIndex); //  != 0
		PedWeaponsSelective1Changed = false;
	}

	if(featureShowDebugInfo){ 
		show_debug_info_on_screen(featureShowDebugInfo);
	}

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
	const int IGN_ARR_PED_SIZE = 1024;
	Ped ign_ped[IGN_ARR_PED_SIZE];
	int found_ign_ped = worldGetAllPeds(ign_ped, IGN_ARR_PED_SIZE);
	for (int i = 0; i < found_ign_ped; i++) {
		if (ign_ped[i] != PLAYER::PLAYER_PED_ID()) {
			if (!PED::IS_PED_GROUP_MEMBER(ign_ped[i], PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()))) { // Only calm down peds if they're NOT in our group (keeps our bodyguards from chilling out and being lazy)
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ign_ped[i], true);
				//AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ign_ped[i], true);
				PED::SET_PED_FLEE_ATTRIBUTES(ign_ped[i], 0, 0);
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

				//if (WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex] > 0) PED::SET_PED_ACCURACY(xped, WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex]); //peds accuracy

				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(xped, true);

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
			//VEHICLE::SET_VEHICLE_ENGINE_HEALTH(veh, 1000.0f);
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
	//pedWeaponSetUpdated = true;
}

void onchange_world_damaged_vehicles_index(int value, SelectFromListMenuItem* source) {
	DamagedVehiclesIndex = value;
	DamagedVehiclesChanged = true;
}

void onchange_world_npc_vehicles_speed_index(int value, SelectFromListMenuItem* source) {
	NPCVehicleSpeedIndex = value;
	NPCVehicleSpeedChanged = true;
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
						WEAPON::SET_PED_INFINITE_AMMO_CLIP(xped, true);
						PED::SET_PED_CAN_SWITCH_WEAPON(xped, true);
						if (WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex] > 0.0) PED::SET_PED_ACCURACY(xped, WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex]); //peds accuracy
						if (WEAPON::HAS_PED_GOT_WEAPON(xped, weapHash, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false) && !PED::IS_PED_INJURED(xped)){
							WEAPON::SET_CURRENT_PED_WEAPON(xped, weapHash, 0);
						}
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
				WEAPON::SET_PED_INFINITE_AMMO_CLIP(xped, true);
				PED::SET_PED_CAN_SWITCH_WEAPON(xped, true);
				if (WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex] > 0.0) PED::SET_PED_ACCURACY(xped, WORLD_NPC_VEHICLESPEED_VALUES[PedAccuracyIndex]); //peds accuracy
				if (!featurePedsIncludeDrivers && WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0) && !PED::IS_PED_IN_ANY_VEHICLE(xped, false)) WEAPON::SET_CURRENT_PED_WEAPON(xped, Ped_Selective_Weapon, 0);
				if (featurePedsIncludeDrivers && WEAPON::HAS_PED_GOT_WEAPON(xped, Ped_Selective_Weapon, 0)) WEAPON::SET_CURRENT_PED_WEAPON(xped, Ped_Selective_Weapon, 0);
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
			//pedWeaponSetUpdated = true;
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
