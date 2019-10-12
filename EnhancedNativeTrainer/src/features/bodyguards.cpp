/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith and fellow contributors 2015
*/

#include "..\io\config_io.h"
#include <string.h>

#include "bodyguards.h"

int activeLineIndexBodyguards = 0;

int myGroup = -1;
int const BODYGUARD_LIMIT = 7;

int pop, all_selected = -1;
std::vector<Hash> WEAPONS;

bool stop_b = false;
bool featureBodyguardInvincible = false;
bool featureBodyguardInvincibleUpdated = false;
bool featureBodyguardHelmet = false;
bool featureBodyguardDespawn = true;
bool featureDifferentWeapons = false;
bool featureBodyguardOnMap = false;
bool featureBodyguardInfAmmo = false;

bool added_nearest_b = false;
Ped bodyGuard, temp_bodyguard = -1;

bool requireRefreshOfBodyguardMainMenu = false;
int activeLineIndexBodyguardBlips = 0;
bool featureBodyBlipNumber = false;

// Bodyguards Blips Option Variables
Blip blip_body[1];
std::vector<Blip> BLIPTABLE_BODYGUARD;
bool animal_in_group = false;
//

//first index is which category, second is position in that category
int skinTypesBodyguardMenuPositionMemory[2] = {0, 0};

//first index is which category, second is position in that category
int skinTypesBodyguardMenuLastConfirmed[2] = {0, 0};

std::vector<Ped> spawnedBodyguards;

std::vector<bool *> bodyguardWeaponsToggle[8];
bool bodyguardWeaponsToggleInitialized = false;

//Blip Size
const std::vector<std::string> BODY_BLIPSIZE_CAPTIONS{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
const double BODY_BLIPSIZE_VALUES[] = { 0.3, 0.5, 0.8, 1.0, 1.2, 1.5, 1.7, 2.0, 2.5, 3.0 };
int BodyBlipSizeIndex = 2;
bool BodyBlipSize_Changed = true;

//Blip Colour
const std::vector<std::string> BODY_BLIPCOLOUR_CAPTIONS{ "White", "Red", "Green", "Blue", "Orange", "Purple", "Grey", "Brown", "Pink", "Dark Green", "Dark Purple", "Dark Blue" };
const int BODY_BLIPCOLOUR_VALUES[] = { 0, 1, 2, 3, 17, 19, 20, 21, 23, 25, 27, 29 };
int BodyBlipColourIndex = 0;
bool BodyBlipColour_Changed = true;

//Blip Symbol
const std::vector<std::string> BODY_BLIPSYMBOL_CAPTIONS{ "Standard", "Player", "North", "Waypoint", "BigCircleOutline", "ArrowUpOutlined", "ArrowDownOutlined", "ArrowUp", "ArrowDown", "PoliceHelicopterAnimated", "Jet" }; 
const std::vector<int> BODY_BLIPSYMBOL_VALUES{ 1, 6, 7, 8, 10, 11, 12, 13, 14, 15, 16 }; 
int BodyBlipSymbolIndex = 0;
bool BodyBlipSymbol_Changed = true;

//Blip Flashing
const std::vector<std::string> BODY_BLIPFLASH_CAPTIONS{ "OFF", "Mode One", "Mode Two" };
const int BODY_BLIPFLASH_VALUES[] = { 0, 1, 2 };
int BodyBlipFlashIndex = 0;
bool BodyBlipFlash_Changed = true;

const std::vector<std::string> SKINS_ANIMALS_CAPTIONS{ "Chop", "German Shepherd", "Husky", "Mountain Lion", "Retriever" };
const std::vector<std::string> SKINS_ANIMALS_VALUES{ "a_c_chop", "a_c_shepherd", "a_c_husky", "a_c_mtlion", "a_c_retriever" };

bool process_bodyguard_skins_menu(){
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "Players";
	item->value = 0;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "NPCs";
	item->value = 1;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Animals";
	item->value = 2;
	item->isLeaf = false;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, &skinTypesBodyguardMenuPositionMemory[0], "Bodyguard Skins", onconfirm_bodyguard_skins_menu, NULL, NULL);
}

bool onconfirm_bodyguard_skins_menu(MenuItem<int> choice){
	switch(choice.value){
		case 0:
			return process_player_skins_menu();
		case 1:
			return process_npc_skins_menu();
		case 2:
			return process_animal_skins_menu();
		default:
			break;
	}
	return false;
}

std::string get_current_model_name(){
	std::string value;
	switch(skinTypesBodyguardMenuLastConfirmed[0]){
		case 0:
			value = SKINS_PLAYER_CAPTIONS[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 1:
			value = SKINS_GENERAL_CAPTIONS[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 2:
			value = SKINS_ANIMALS_CAPTIONS[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		default:
			value = SKINS_GENERAL_CAPTIONS[0];
			break;
	}
	return value;
}

Hash get_current_model_hash(){
	std::string value;
	switch(skinTypesBodyguardMenuLastConfirmed[0]){
		case 0:
			value = SKINS_PLAYER_VALUES[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 1:
			value = SKINS_GENERAL_VALUES[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 2:
			value = SKINS_ANIMALS_VALUES[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		default:
			value = SKINS_GENERAL_VALUES[0];
			break;
	}
	return GAMEPLAY::GET_HASH_KEY((char*) value.c_str());
}

bool onconfirm_bodyguards_skins_players(MenuItem<std::string> choice){
	skinTypesBodyguardMenuPositionMemory[0] = 0;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 0;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_npcs(MenuItem<std::string> choice){
	skinTypesBodyguardMenuPositionMemory[0] = 1;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 1;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;

	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_animals(MenuItem<std::string> choice){ 
	skinTypesBodyguardMenuPositionMemory[0] = 2;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 2;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;

	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool process_player_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	for(int i = 0; i < SKINS_PLAYER_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_PLAYER_CAPTIONS[i];
		item->value = SKINS_PLAYER_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "Player Skins", onconfirm_bodyguards_skins_players, NULL, NULL);
}

bool process_npc_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	for(int i = 0; i < SKINS_GENERAL_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_GENERAL_CAPTIONS[i];
		item->value = SKINS_GENERAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "NPC Skins", onconfirm_bodyguards_skins_npcs, NULL, NULL);
}

bool process_animal_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	for (int i = 0; i < SKINS_ANIMALS_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_ANIMALS_CAPTIONS[i];
		item->value = SKINS_ANIMALS_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "Animal Skins", onconfirm_bodyguards_skins_animals, NULL, NULL);
}

bool onconfirm_bodyguard_weapons_category_menu(MenuItem<int> choice){
	int category = choice.sortval;

	switch(choice.value){
		case -1:
			for(int a = 0; a < bodyguardWeaponsToggle[category].size(); a++){
				*bodyguardWeaponsToggle[category].at(a) = !*bodyguardWeaponsToggle[category].at(a);
			}
			set_status_text(std::string("All bodyguard ") + MENU_WEAPON_CATEGORIES.at(category) + std::string(" weapons toggled"));
			break;
		default:
			break;
	}

	return false;
}

bool process_bodyguard_weapons_category_menu(int category){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	ToggleMenuItem<int> *toggleItem;
	int index = -1;

	item = new MenuItem<int>();
	item->caption = "Toggle All Weapons in Category";
	item->value = index++;
	item->isLeaf = true;
	item->sortval = category;
	menuItems.push_back(item);

	for(auto a : VOV_WEAPON_CAPTIONS[category]){
		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = a;
		toggleItem->value = index;
		toggleItem->toggleValue = bodyguardWeaponsToggle[category].at(index++);
		toggleItem->toggleValueUpdated = nullptr;
		menuItems.push_back(toggleItem);
	}

	return draw_generic_menu<int>(menuItems, nullptr, MENU_WEAPON_CATEGORIES.at(category), onconfirm_bodyguard_weapons_category_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_bodyguard_weapons_menu(MenuItem<int> choice){
	int cs = MENU_WEAPON_CATEGORIES.size();
	if(choice.value >= 0 && choice.value < cs){
		process_bodyguard_weapons_category_menu(choice.value);
	}
	else if(choice.value == cs){
		for(int a = 0; a < cs; a++){
			for(int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++){
				*bodyguardWeaponsToggle[a].at(b) = true;
			}
		}
		set_status_text("All bodyguard weapons enabled");
	}
	else if(choice.value == cs + 1){
		for(int a = 0; a < cs; a++){
			for(int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++){
				*bodyguardWeaponsToggle[a].at(b) = false;
			}
		}
		set_status_text("All bodyguard weapons disabled");
	}

	return false;
}

bool process_bodyguard_weapons_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int index = 0;

	for(auto a : MENU_WEAPON_CATEGORIES){
		item = new MenuItem<int>();
		item->caption = a;
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "Toggle All Weapons Off";
	item->value = index + 1;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	item = new MenuItem<int>();
	item->caption = "Toggle All Weapons On";
	item->value = index;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	return draw_generic_menu<int>(menuItems, nullptr, "Choose Bodyguard Weapons", onconfirm_bodyguard_weapons_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_bodyguard_blips_menu(MenuItem<int> choice)
{
	return false;
}

void process_bodyguard_blips_menu(){
	std::string caption = "Mark On Map Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Enabled";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureBodyguardOnMap;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(BODY_BLIPSIZE_CAPTIONS, onchange_body_blipsize_index);
	listItem->wrap = false;
	listItem->caption = "Blip Size";
	listItem->value = BodyBlipSizeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(BODY_BLIPCOLOUR_CAPTIONS, onchange_body_blipcolour_index);
	listItem->wrap = false;
	listItem->caption = "Blip Colour";
	listItem->value = BodyBlipColourIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(BODY_BLIPSYMBOL_CAPTIONS, onchange_body_blipsymbol_index);
	listItem->wrap = false;
	listItem->caption = "Blip Symbol";
	listItem->value = BodyBlipSymbolIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(BODY_BLIPFLASH_CAPTIONS, onchange_body_blipflash_index);
	listItem->wrap = false;
	listItem->caption = "Blip Flashing";
	listItem->value = BodyBlipFlashIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Show Blip Number";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureBodyBlipNumber;
	menuItems.push_back(toggleItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexBodyguardBlips, caption, onconfirm_bodyguard_blips_menu, NULL, NULL);
}

void dismiss_bodyguards(){

	if (!BLIPTABLE_BODYGUARD.empty()) {
		for (int i = 0; i < BLIPTABLE_BODYGUARD.size(); i++) {
			if (UI::DOES_BLIP_EXIST(BLIPTABLE_BODYGUARD[i])) {
				UI::REMOVE_BLIP(&BLIPTABLE_BODYGUARD[i]);
			}
		}
		BLIPTABLE_BODYGUARD.clear();
		BLIPTABLE_BODYGUARD.shrink_to_fit();
	}
		
	animal_in_group = false;

	if(spawnedBodyguards.size() == 0){
		set_status_text("You don't have any bodyguards");
		spawnedBodyguards.clear();
		spawnedBodyguards.shrink_to_fit();
		return;
	}

	for(int i = 0; i < spawnedBodyguards.size(); i++){
		ENTITY::SET_ENTITY_INVINCIBLE(spawnedBodyguards[i], false);
		PED::SET_PED_NEVER_LEAVES_GROUP(spawnedBodyguards[i], false);
		PED::REMOVE_PED_FROM_GROUP(spawnedBodyguards[i]);
		if (featureBodyguardDespawn) {
			AI::CLEAR_PED_TASKS(spawnedBodyguards[i]);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedBodyguards[i]);
		}
	}

	spawnedBodyguards.clear();
	spawnedBodyguards.shrink_to_fit();
	requireRefreshOfBodyguardMainMenu = true;

	pop = -1;
	all_selected = -1;
	if (!WEAPONS.empty()) {
		WEAPONS.clear();
		WEAPONS.shrink_to_fit();
	}

	set_status_text("Bodyguards dismissed");
}

void do_spawn_bodyguard(){
	requireRefreshOfBodyguardMainMenu = true;
	bool bodyguard_animal = false;
	bodyGuard = -1;
	bool exist_already = false;

	if(spawnedBodyguards.size() >= BODYGUARD_LIMIT){
		set_status_text("Cannot spawn any more bodyguards");
		return;
	}

	DWORD bodyGuardModel = get_current_model_hash();

	if((STREAMING::IS_MODEL_IN_CDIMAGE(bodyGuardModel) && STREAMING::IS_MODEL_VALID(bodyGuardModel)) || added_nearest_b == true){
		STREAMING::REQUEST_MODEL(bodyGuardModel);
		while(!STREAMING::HAS_MODEL_LOADED(bodyGuardModel)){
			make_periodic_feature_call();
			WAIT(0);
		}

		myGroup = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID());

		Vector3 spawnCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 2.5, 2.5, 0.0);
		Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		if (added_nearest_b == false) bodyGuard = PED::CREATE_PED(25, bodyGuardModel, spawnCoords.x, spawnCoords.y, spawnCoords.z, 0, 0, 0);
		if (added_nearest_b == true) {
			const int arrSize33 = 1024;
			Ped surr_peds[arrSize33];
			int count_surr_peds = worldGetAllPeds(surr_peds, arrSize33);
			float dist_diff = -1.0;
			float temp_dist = 20.0;
			for (int i = 0; i < count_surr_peds; i++) {
				if (PED::GET_PED_TYPE(surr_peds[i]) != 0 && PED::GET_PED_TYPE(surr_peds[i]) != 1 && PED::GET_PED_TYPE(surr_peds[i]) != 2 &&
					PED::GET_PED_TYPE(surr_peds[i]) != 3) {
					Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_peds[i], true);
					dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
					exist_already = false;
					if (!spawnedBodyguards.empty()) {
						for (int j = 0; j < spawnedBodyguards.size(); j++) {
							if (spawnedBodyguards[j] == surr_peds[i]) exist_already = true;
						}
					}
					if (temp_dist > dist_diff && exist_already == false) {
						temp_dist = dist_diff;
						temp_bodyguard = surr_peds[i];
					}
				}
			}
			exist_already = false;
			if (!spawnedBodyguards.empty()) {
				for (int i = 0; i < spawnedBodyguards.size(); i++) {
					if (spawnedBodyguards[i] == temp_bodyguard) exist_already = true;
				}
			}
			if (exist_already == false) bodyGuard = temp_bodyguard;
		}

		if (bodyGuard != -1) {
			for (int i = 0; i < SKINS_ANIMALS_VALUES.size(); i++) {
				char *currAnimal = new char[SKINS_ANIMALS_VALUES[i].length() + 1];
				strcpy(currAnimal, SKINS_ANIMALS_VALUES[i].c_str());
				DWORD curr_animal_body = GAMEPLAY::GET_HASH_KEY(currAnimal);
				if (bodyGuardModel == curr_animal_body)
				{
					bodyguard_animal = true;
					animal_in_group = true;
				}
			}

			spawnedBodyguards.push_back(bodyGuard); // save the current bodyguard

			PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), myGroup);
			PED::SET_PED_AS_GROUP_MEMBER(bodyGuard, myGroup);
			PED::SET_PED_NEVER_LEAVES_GROUP(bodyGuard, true);

			if (featureBodyguardInvincible) {
				ENTITY::SET_ENTITY_INVINCIBLE(bodyGuard, true);
			}

			if (featureBodyguardHelmet && bodyguard_animal == false) {
				PED::GIVE_PED_HELMET(bodyGuard, 1, 4096, -1);
			}

			if (featureBodyguardOnMap) {
				blip_body[0] = UI::ADD_BLIP_FOR_ENTITY(bodyGuard);
				UI::SET_BLIP_AS_FRIENDLY(blip_body[0], true);
				if (BODY_BLIPSYMBOL_VALUES[BodyBlipSymbolIndex] != NULL) UI::SET_BLIP_SPRITE(blip_body[0], BODY_BLIPSYMBOL_VALUES[BodyBlipSymbolIndex]);
				else UI::SET_BLIP_SPRITE(blip_body[0], BODY_BLIPSYMBOL_VALUES[0]);
				UI::SET_BLIP_CATEGORY(blip_body[0], 2);
				if (featureBodyBlipNumber) UI::SHOW_NUMBER_ON_BLIP(blip_body[0], BLIPTABLE_BODYGUARD.size());
				if (BODY_BLIPFLASH_VALUES[BodyBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_body[0], true);
				if (BODY_BLIPFLASH_VALUES[BodyBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_body[0], true);
				UI::SET_BLIP_SCALE(blip_body[0], BODY_BLIPSIZE_VALUES[BodyBlipSizeIndex]);
				UI::SET_BLIP_COLOUR(blip_body[0], BODY_BLIPCOLOUR_VALUES[BodyBlipColourIndex]);
				UI::SET_BLIP_AS_SHORT_RANGE(blip_body[0], true);
				BLIPTABLE_BODYGUARD.push_back(blip_body[0]);
			}
			else {
				if (!BLIPTABLE_BODYGUARD.empty()) {
					for (int i = 0; i < BLIPTABLE_BODYGUARD.size(); i++) {
						if (UI::DOES_BLIP_EXIST(BLIPTABLE_BODYGUARD[i])) {
							UI::REMOVE_BLIP(&BLIPTABLE_BODYGUARD[i]);
						}
					}
					BLIPTABLE_BODYGUARD.clear();
					BLIPTABLE_BODYGUARD.shrink_to_fit();
				}
				std::vector<int> emptyVec;
				if (!BODY_BLIPSYMBOL_VALUES.empty()) std::vector<int>(BODY_BLIPSYMBOL_VALUES).swap(emptyVec);
			}

			PED::SET_PED_COMBAT_ABILITY(bodyGuard, 2);
			PED::SET_PED_COMBAT_RANGE(bodyGuard, 2);
			PED::SET_PED_COMBAT_MOVEMENT(bodyGuard, 3);
			PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 5, true);
			//
			if (bodyguard_animal == true) {
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 46, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 17, true);
				PED::SET_PED_FLEE_ATTRIBUTES(bodyGuard, 0, false);
				Hash temp_p = PED::GET_PED_RELATIONSHIP_GROUP_HASH(bodyGuard);
				PED::ADD_RELATIONSHIP_GROUP("DogChop", &temp_p);
				PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, myGroup, temp_p); // 0
				PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, temp_p, myGroup);
				PED::SET_PED_CAN_BE_TARGETTED(bodyGuard, false);
			}
			//
			if (bodyguard_animal == false) PED::SET_PED_CAN_SWITCH_WEAPON(bodyGuard, true);
			PED::SET_GROUP_FORMATION(myGroup, 1); // 1
			PED::SET_CAN_ATTACK_FRIENDLY(bodyGuard, false, false);

			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(bodyGuard, 10000, 0);
			PED::SET_PED_KEEP_TASK(bodyGuard, true);

			if (bodyguard_animal == false) PED::SET_PED_FIRING_PATTERN(bodyGuard, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_FULL_AUTO")); // 0xC6EE6B4C

			// Different Weapons
			if (featureDifferentWeapons) {
				if (WEAPONS.empty()) {
					for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
						for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
							if (*bodyguardWeaponsToggle[a].at(b)) {
								all_selected = all_selected + 1;
								WEAPONS.push_back(GAMEPLAY::GET_HASH_KEY((char *)VOV_WEAPON_VALUES[a].at(b).c_str()));
							}
						}
					}
				}
				pop = pop + 1;
				if (bodyguard_animal == false) WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, WEAPONS[pop], 1000, false, true);
				if (added_nearest_b == true && !WEAPON::IS_PED_ARMED(bodyGuard, 7)) {
					WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, WEAPONS[pop], 999, false, true);
					WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, WEAPONS[pop], 1);
					WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
				}
				if (pop == all_selected) pop = -1;
			}
			
			if (!featureDifferentWeapons) {
				if (bodyguard_animal == false) {
					for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
						for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
							if (*bodyguardWeaponsToggle[a].at(b)) {
								Hash tmp = GAMEPLAY::GET_HASH_KEY((char *)VOV_WEAPON_VALUES[a].at(b).c_str());
								if (!WEAPON::HAS_PED_GOT_WEAPON(bodyGuard, tmp, false)) {
									WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, tmp, 1000, false, true);
								}
							}
						}
					}
				}

				if (added_nearest_b == true && !WEAPON::IS_PED_ARMED(bodyGuard, 7)) {
					for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
						for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
							Hash tmp = GAMEPLAY::GET_HASH_KEY((char *)VOV_WEAPON_VALUES[a].at(b).c_str());
							WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, tmp, 999, false, true);
							WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, tmp, 1);
							WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
						}
					}
				}
			}

			if (featureBodyguardInfAmmo && bodyguard_animal == false) WEAPON::SET_PED_INFINITE_AMMO_CLIP(bodyGuard, true);

			if (added_nearest_b == false) PED::SET_PED_DEFAULT_COMPONENT_VARIATION(bodyGuard);
			WAIT(0);

			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

				for (int i = 0; i < VEHICLE::GET_VEHICLE_MAX_NUMBER_OF_PASSENGERS(veh); i++) {
					if (VEHICLE::IS_VEHICLE_SEAT_FREE(veh, i)) {
						AI::TASK_WARP_PED_INTO_VEHICLE(bodyGuard, veh, i);
						break;
					}
				}
			}
		}

		WAIT(50);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(bodyGuardModel);
	}
	added_nearest_b = false;
	return;
}

void do_add_near_bodyguard() {
	added_nearest_b = true;
	do_spawn_bodyguard();
	return;
}

void maintain_bodyguards(){
	if (spawnedBodyguards.size() == 0)
	{
		if (!BLIPTABLE_BODYGUARD.empty()) {
			for (int i = 0; i < BLIPTABLE_BODYGUARD.size(); i++) {
				if (UI::DOES_BLIP_EXIST(BLIPTABLE_BODYGUARD[i])) {
					UI::REMOVE_BLIP(&BLIPTABLE_BODYGUARD[i]);
				}
			}
			BLIPTABLE_BODYGUARD.clear();
			BLIPTABLE_BODYGUARD.shrink_to_fit();
		}
		animal_in_group = false;

		pop = -1;
		all_selected = -1;
		if (!WEAPONS.empty()) {
			WEAPONS.clear();
			WEAPONS.shrink_to_fit();
		}
	}
	
	if (!spawnedBodyguards.empty()) { 
		for (int i = 0; i < spawnedBodyguards.size(); i++) {
			// bodyguards swimming ability
			if (ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 1 && PED::IS_PED_SWIMMING(PLAYER::PLAYER_PED_ID())) {
				Vector3 my_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
				if (/*ENTITY::IS_ENTITY_IN_WATER(spawnedBodyguards[i]) == 1 && */!PED::IS_PED_SWIMMING(spawnedBodyguards[i])) ENTITY::SET_ENTITY_COORDS(spawnedBodyguards[i], my_coords.x + (0.1 * (rand() % 30)), my_coords.y + (0.1 * (rand() % 30)), my_coords.z - 0.9, 1, 0, 0, 1); // 0.8 1.0
			}
			//
			PED::SET_PED_KEEP_TASK(spawnedBodyguards[i], true);
			if (animal_in_group == true && PED::IS_PED_FLEEING(spawnedBodyguards[i])) AI::TASK_STAND_STILL(spawnedBodyguards[i], 10000); //  || AI::IS_PED_RUNNING(*iter))
			
			if (stop_b == false) {
				PED::SET_PED_AS_GROUP_MEMBER(spawnedBodyguards[i], PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()));
				PED::SET_PED_NEVER_LEAVES_GROUP(spawnedBodyguards[i], true);
			}
			if (stop_b == true) {
				PED::REMOVE_PED_FROM_GROUP(spawnedBodyguards[i]);
				AI::CLEAR_PED_TASKS(spawnedBodyguards[i]);
			}
			if (PED::IS_PED_DEAD_OR_DYING(spawnedBodyguards[i], true)) {
				PED::SET_PED_NEVER_LEAVES_GROUP(spawnedBodyguards[i], false);
				PED::REMOVE_PED_FROM_GROUP(spawnedBodyguards[i]);
				if (featureBodyguardDespawn) ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedBodyguards[i]);
				spawnedBodyguards.erase(spawnedBodyguards.begin() + i);
				requireRefreshOfBodyguardMainMenu = true;
			}
		}
	}
}

bool process_bodyguard_menu(){
	do{
		requireRefreshOfBodyguardMainMenu = false;
		int i = 0;

		std::string caption = "Bodyguard Options";

		std::vector<MenuItem<int>*> menuItems;
		MenuItem<int> *item;
		ToggleMenuItem<int>* toggleItem;

		item = new MenuItem<int>();
		std::ostringstream ss;
		ss << "Spawn Bodyguard: " << get_current_model_name(); 
		item->caption = ss.str();
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Add Nearest Ped As Bodyguard";
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Dismiss All Bodyguards";
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Toggle Bodyguards To Follow Player";
		item->value = i++;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Choose Model";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Choose Weapons";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Mark On Map";
		item->value = i++;
		item->isLeaf = false;
		menuItems.push_back(item);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Invincible";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInvincible;
		toggleItem->toggleValueUpdated = &featureBodyguardInvincibleUpdated;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Infinite Ammo";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInfAmmo;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Spawn With Helmet";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardHelmet;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Despawn When Dead/Dismissed";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardDespawn;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Different Weapons";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureDifferentWeapons;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		if(!bodyguardWeaponsToggleInitialized){
			for(int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++){
				for(int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++){
					bodyguardWeaponsToggle[a].push_back(new bool(true));
				}
			}
			bodyguardWeaponsToggleInitialized = true;
		}

		draw_generic_menu<int>(menuItems, &activeLineIndexBodyguards, caption, onconfirm_bodyguard_menu, NULL, NULL, bodyguards_main_menu_interrupt);
	}
	while(requireRefreshOfBodyguardMainMenu);
	return false;
}

bool onconfirm_bodyguard_menu(MenuItem<int> choice){
	// common variables
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	switch(choice.value){
		case 0:
			do_spawn_bodyguard();
			break;
		case 1:
			do_add_near_bodyguard();
			break;
		case 2:
			dismiss_bodyguards();
			break;
		case 3:
			stop_b = !stop_b;
			if (stop_b) set_status_text("Stay Put");
			else set_status_text("Follow");
			break;
		case 4:
			process_bodyguard_skins_menu();
			break;
		case 5:
			process_bodyguard_weapons_menu();
			break;
		case 6:
			process_bodyguard_blips_menu();
			break;
		default:
			break;
	}
	return false;
}

void update_bodyguard_features(){
	if(featureBodyguardInvincibleUpdated){
		for(int i = 0; i < spawnedBodyguards.size(); i++){
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedBodyguards[i], featureBodyguardInvincible);
		}
	}
}

void add_bodyguards_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardInvincible", &featureBodyguardInvincible});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardHelmet", &featureBodyguardHelmet});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardDespawn", &featureBodyguardDespawn});
	results->push_back(FeatureEnabledLocalDefinition{"featureDifferentWeapons", &featureDifferentWeapons});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardOnMap", &featureBodyguardOnMap});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyBlipNumber", &featureBodyBlipNumber});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardInfAmmo", &featureBodyguardInfAmmo});
}

void add_bodyguards_feature_enablements2(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "BodyBlipSizeIndex", std::to_string(BodyBlipSizeIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipColourIndex", std::to_string(BodyBlipColourIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipSymbolIndex", std::to_string(BodyBlipSymbolIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipFlashIndex", std::to_string(BodyBlipFlashIndex) });
}

void add_bodyguards_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory0", std::to_string(skinTypesBodyguardMenuPositionMemory[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory1", std::to_string(skinTypesBodyguardMenuPositionMemory[1])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed0", std::to_string(skinTypesBodyguardMenuLastConfirmed[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed1", std::to_string(skinTypesBodyguardMenuLastConfirmed[1])});
}

void handle_generic_settings_bodyguards(std::vector<StringPairSettingDBRow>* settings){
	for(int i = 0; i < settings->size(); i++){
		StringPairSettingDBRow setting = settings->at(i);
		if(setting.name.compare("skinTypesBodyguardMenuPositionMemory0") == 0){
			skinTypesBodyguardMenuPositionMemory[0] = stoi(setting.value);
		}
		else if(setting.name.compare("skinTypesBodyguardMenuPositionMemory1") == 0){
			skinTypesBodyguardMenuPositionMemory[1] = stoi(setting.value);
		}
		else if(setting.name.compare("skinTypesBodyguardMenuLastConfirmed0") == 0){
			skinTypesBodyguardMenuLastConfirmed[0] = stoi(setting.value);
		}
		else if(setting.name.compare("skinTypesBodyguardMenuLastConfirmed1") == 0){
			skinTypesBodyguardMenuLastConfirmed[1] = stoi(setting.value);
		}
		else if (setting.name.compare("BodyBlipSizeIndex") == 0){
			BodyBlipSizeIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyBlipColourIndex") == 0){
			BodyBlipColourIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyBlipSymbolIndex") == 0 && BODY_BLIPSYMBOL_VALUES.size() > 0 && BodyBlipSymbolIndex < BODY_BLIPSYMBOL_VALUES.size()){
			BodyBlipSymbolIndex = stoi(setting.value);
		}
		else if (BODY_BLIPSYMBOL_VALUES.size() > 0 && BodyBlipSymbolIndex >= BODY_BLIPSYMBOL_VALUES.size()) { // setting.name.compare("BodyBlipSymbolIndex") == 0 && 
			BodyBlipSymbolIndex = 0;
		}
		else if (setting.name.compare("BodyBlipFlashIndex") == 0){
			BodyBlipFlashIndex = stoi(setting.value);
		}
	}
}

bool bodyguards_main_menu_interrupt(){
	if(requireRefreshOfBodyguardMainMenu){
		return true;
	}
	return false;
}

void reset_bodyguards_globals(){
	activeLineIndexBodyguardBlips = 0;
	featureBodyBlipNumber = false;
	featureBodyguardOnMap = false;
	featureBodyguardInvincible = false;
	featureBodyguardInvincibleUpdated = false;
	featureBodyguardHelmet = false;
	featureBodyguardDespawn = true;
	featureBodyguardInfAmmo = false;
	featureDifferentWeapons = false;
	BodyBlipSizeIndex = 2;
	BodyBlipColourIndex = 0;
	BodyBlipSymbolIndex = 0;
	BodyBlipFlashIndex = 0;
}

void onchange_body_blipsize_index(int value, SelectFromListMenuItem* source){
	BodyBlipSizeIndex = value;
	BodyBlipSize_Changed = true;
}

void onchange_body_blipcolour_index(int value, SelectFromListMenuItem* source){
	BodyBlipColourIndex = value;
	BodyBlipColour_Changed = true;
}

void onchange_body_blipsymbol_index(int value, SelectFromListMenuItem* source){
	BodyBlipSymbolIndex = value;
	BodyBlipSymbol_Changed = true;
}

void onchange_body_blipflash_index(int value, SelectFromListMenuItem* source){
	BodyBlipFlashIndex = value;
	BodyBlipFlash_Changed = true;
}