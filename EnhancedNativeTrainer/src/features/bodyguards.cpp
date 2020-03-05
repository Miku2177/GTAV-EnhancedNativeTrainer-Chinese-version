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
#include "script.h"
#include "hotkeys.h"

int activeLineIndexBodyguards = 0;

int myENTGroup = -1;
int const BODYGUARD_LIMIT = 7;

Ped cop_to_kill;
float dist_diff = -1;

int pop, all_selected = -1;
std::vector<Hash> WEAPONS;

bool bod_pass, me_to_follow = false;
std::vector<Vehicle> B_VEHICLE;

std::string lastCustomBodyguardSpawn;
std::string value;

bool stop_b = false;
bool featureBodyguardInvincible = false;
bool featureBodyguardInvincibleUpdated = false;
bool featureBodyguardHelmet = false;
bool featureBodyguardDespawn = true;
bool featureDifferentWeapons = false;
bool featureRandomApp = false;
//bool featureFollowInVehicle = false;
bool featureBodyguardOnMap = false;
bool featureBodyguardInfAmmo = false;

bool hotkey_b = false;

bool added_nearest_b = false;
Ped bodyGuard, temp_bodyguard = -1;

// modify skin
int skinBodDetailMenuIndex = 0;
int skinBodDetailMenuValue = 0;
float b_curr_num = -1;
bool is_it_n = false;
char temp_n[10];

// save/load bodyguard
int activeSavedBodSkinIndex = -1;
std::string activeSavedBodSkinSlotName;
int lastKnownSavedBodSkinCount = 0;
bool bodskinSaveMenuInterrupt = false;
bool bodskinSaveSlotMenuInterrupt = false;
bool requireRefreshOfBodSkinSaveSlots = false;
bool requireRefreshOfBodSkinSlotMenu = false;
int bodskinDetailMenuIndex = 0;
int bodskinDetailMenuValue = 0;
bool load_saved_bodyguard = false;

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

std::vector<Ped> spawnedENTBodyguards;

std::vector<bool *> bodyguardWeaponsToggle[8];
bool bodyguardWeaponsToggleInitialized = false;

//Blip Size
const std::vector<std::string> BODY_BLIPSIZE_CAPTIONS{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };
const double BODY_BLIPSIZE_VALUES[] = { 0.3, 0.5, 0.8, 1.0, 1.2, 1.5, 1.7, 2.0, 2.5, 3.0 };
int BodyBlipSizeIndex = 2;
bool BodyBlipSize_Changed = true;
int BodyDistanceIndex = 7;
bool BodyDistance_Changed = true;

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

//Group Formation
const std::vector<std::string> BODY_GROUPFORMATION_CAPTIONS{ "Default", "Circle Around Leader", "Line With Leader At Center" };
const std::vector<int> BODY_GROUPFORMATION_VALUES{ 0, 1, 3 };
int BodyGroupFormationIndex = 1;
bool BodyGroupFormationChanged = true;

//Blip Flashing
//const std::vector<std::string> BODY_BLIPFLASH_CAPTIONS{ "OFF", "Mode One", "Mode Two" };
//const int BODY_BLIPFLASH_VALUES[] = { 0, 1, 2 };
int BodyBlipFlashIndex = 0;
bool BodyBlipFlash_Changed = true;
int FollowInVehicleIndex = 0;
bool FollowInVehicleChanged = true;

//Bodyguard Health
int BodyHealthIndex = 6;
bool BodyHealthChanged = true;

const std::vector<std::string> SKINS_ANIMALS_CAPTIONS{ "Chop", "German Shepherd", "Husky", "Mountain Lion", "Retriever" };
const std::vector<std::string> SKINS_ANIMALS_VALUES{ "a_c_chop", "a_c_shepherd", "a_c_husky", "a_c_mtlion", "a_c_retriever" };

// Modify Skin
std::string getBodSkinDetailAttribDescription(int i)
{
	switch (i)
	{
	case 0:
		return "Head/Face";
	case 1:
		return "Beard/Mask";
	case 2:
		return "Hair/Hat";
	case 3:
		return "Top";
	case 4:
		return "Legs";
	case 5:
		return "Accessory/Gloves";
	case 6:
		return "Accessory/Shoes";
	case 7:
	case 8:
	case 9:
		return "Accessory";
	case 10:
		return "Badges";
	case 11:
		return "Shirt/Jacket";
	default:
		return std::to_string(i);
	}
}

std::string getBodPropDetailAttribDescription(int i)
{
	switch (i)
	{
	case 0:
		return "Hats/Masks/Helmets";
	case 1:
		return "Glasses";
	case 2:
		return "Earrings";
	case 3:
		return "??? 3";
	case 4:
		return "??? 4";
	case 5:
		return "??? 5";
	case 6:
		return "??? 6";
	case 7:
		return "??? 7";
	case 8:
		return "??? 8";
	case 9:
		return "??? 9";
	case 10:
		return "??? 10";
	case 11:
		return "??? 11";
	default:
		return std::to_string(i);
	}
}

void onexit_bod_skinchanger_texture_menu(bool returnValue)
{
}

void onhighlight_bod_skinchanger_texture_menu(MenuItem<int> choice)
{
	if (true)//PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, value))
	{
		//int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue);
		//PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue, currentDrawable, choice.value, 0);
		int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
		PED::SET_PED_COMPONENT_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue, currentDrawable, choice.value, 0);
	}
	WAIT(100);
}

bool onconfirm_bod_skinchanger_texture_menu(MenuItem<int> choice)
{
	onhighlight_bod_skinchanger_texture_menu(choice);

	return true;
}

bool process_bod_skinchanger_texture_menu(std::string caption)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	//Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	Hash model = ENTITY::GET_ENTITY_MODEL(spawnedENTBodyguards[b_curr_num]);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		//int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue);
		//int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue, currentDrawable);
		int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
		int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue, currentDrawable);

		for (int i = 0; i < textures; i++)
		{
			std::ostringstream ss;
			ss << "Texture #" << i;
			MenuItem<int> *item = new MenuItem<int>();
			item->caption = ss.str();
			item->value = i;
			menuItems.push_back(item);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "Available Textures";

	//int currentTexture = PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue);
	int currentTexture = PED::GET_PED_TEXTURE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
	draw_generic_menu<int>(menuItems, &currentTexture, ss.str(), onconfirm_bod_skinchanger_texture_menu, onhighlight_bod_skinchanger_texture_menu, onexit_bod_skinchanger_texture_menu);
	return false;
}

void onexit_bod_skinchanger_drawable_menu(bool returnValue)
{
}

void onhighlight_bod_skinchanger_drawable_menu(MenuItem<int> choice)
{
	//int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue);
	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
	if (choice.value != currentDrawable)
	{
		//PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinBodDetailMenuValue, choice.value, 0, 0);
		PED::SET_PED_COMPONENT_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue, choice.value, 0, 0);
	}
	WAIT(100);
}

bool onconfirm_bod_skinchanger_drawable_menu(MenuItem<int> choice)
{
	if (choice.isLeaf)
	{
		return false;
	}
	return process_bod_skinchanger_texture_menu(choice.caption);
}

bool process_bod_skinchanger_drawable_menu(std::string caption, int component)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	//Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	Hash model = ENTITY::GET_ENTITY_MODEL(spawnedENTBodyguards[b_curr_num]);
	
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		//int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component);
		int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], component);
		for (int i = 0; i < drawables; i++)
		{
			//int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component, i);
			int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], component, i);
			std::ostringstream ss;
			ss << "Drawable #" << i << " ~HUD_COLOUR_GREYLIGHT~(" << textures << ")";

			MenuItem<int> *item = new MenuItem<int>();
			item->caption = ss.str();
			item->value = i;
			item->isLeaf = (textures <= 1);
			menuItems.push_back(item);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "Available Drawables";

	//int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), component);
	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], component);
	draw_generic_menu<int>(menuItems, &currentDrawable, ss.str(), onconfirm_bod_skinchanger_drawable_menu, onhighlight_bod_skinchanger_drawable_menu, onexit_bod_skinchanger_drawable_menu, b_skin_menu_interrupt);
	return false;
}

void onhighlight_bodskinchanger_detail_menu(MenuItem<int> choice)
{
	//do nothing
}

bool onconfirm_bodskinchanger_detail_menu(MenuItem<int> choice)
{
	skinBodDetailMenuIndex = choice.currentMenuIndex;
	skinBodDetailMenuValue = choice.value;

	return process_bod_skinchanger_drawable_menu(choice.caption, choice.value);
}

bool b_skin_menu_interrupt() {
	if (!ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID())) return true;

	if (spawnedENTBodyguards.size() == 0) {
		set_status_text("~r~All bodyguards are dead");
		return true;
	}

	return false;
}

bool process_bod_skinchanger_detail_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;
	int fixedChoices = 0;
	const int partVariations = 12;
	int i = 0;
	//Ped playerPed = PLAYER::PLAYER_PED_ID();
	//Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	Hash model = ENTITY::GET_ENTITY_MODEL(spawnedENTBodyguards[b_curr_num]);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}
		for (; i < partVariations + fixedChoices; i++)
		{
			bool iFound = false;
			int compIndex = i - fixedChoices;

			//int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex);
			int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], compIndex);
			int textures = 0;
			if (drawables == 1)
			{
				//textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex, 0);
				textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], compIndex, 0);
			}
			if (drawables > 1 || textures != 0)
			{
				std::ostringstream ss;
				std::string itemText = getBodSkinDetailAttribDescription(compIndex);
				ss << "Slot " << (compIndex + 1) << ": " << itemText << " ~HUD_COLOUR_GREYLIGHT~(" << drawables << ")";

				MenuItem<int> *item = new MenuItem<int>();
				item->caption = ss.str();
				item->value = compIndex;
				item->isLeaf = false;
				menuItems.push_back(item);
			}
		}
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}
	return draw_generic_menu<int>(menuItems, &skinBodDetailMenuIndex, "Skin Details", onconfirm_bodskinchanger_detail_menu, onhighlight_bodskinchanger_detail_menu, NULL, b_skin_menu_interrupt);
}
// end of 'modify skin'

// save/load bodyguard
bool applyChosenBodSkin(DWORD model)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		//Vehicle veh = NULL;
		//if (PED::IS_PED_IN_ANY_VEHICLE(model, 0))
		//{
		//	veh = PED::GET_VEHICLE_PED_IS_USING(model);
		//}

		//save_player_weapons(model);

		load_saved_bodyguard = true;
		temp_bodyguard = model;
		do_spawn_bodyguard();
		load_saved_bodyguard = false;

		//PLAYER::SET_PLAYER_MODEL(model, model);
		
		//PED::SET_PED_DEFAULT_COMPONENT_VARIATION(model);
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(bodyGuard);
		WAIT(0);

		//if (veh != NULL)
		//{
		//	PED::SET_PED_INTO_VEHICLE(model, veh, -1);
		//}

		//restore_player_weapons(model);

		//reset the skin detail choice
		bodskinDetailMenuIndex = 0;
		bodskinDetailMenuValue = 0;

		WAIT(100);
		//STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(bodyGuard);

		//chosenSkinName = skinName;

		return true;
	}
	return false;
}

bool spawn_saved_bod_skin(int slot, std::string caption)
{
	ENTDatabase* database = get_database();

	std::vector<SavedBodSkinDBRow*> savedBodSkins = database->get_saved_bod_skins(slot);

	SavedBodSkinDBRow* savedBodSkin = savedBodSkins.at(0);
	database->populate_saved_bod_skin(savedBodSkin);

	applyChosenBodSkin(savedBodSkin->model);

	//Ped ped = PLAYER::PLAYER_PED_ID();

	for each (SavedBodSkinComponentDBRow *comp in savedBodSkin->components)
	{
		//PED::SET_PED_COMPONENT_VARIATION(savedBodSkin->model, comp->slotID, comp->drawable, comp->texture, 0);
		PED::SET_PED_COMPONENT_VARIATION(bodyGuard, comp->slotID, comp->drawable, comp->texture, 0);
	}

	//PED::CLEAR_ALL_PED_PROPS(savedBodSkin->model);
	PED::CLEAR_ALL_PED_PROPS(bodyGuard);
	for each (SavedBodSkinPropDBRow *prop in savedBodSkin->props)
	{
		//PED::SET_PED_PROP_INDEX(savedBodSkin->model, prop->propID, prop->drawable, prop->texture, 0);
		PED::SET_PED_PROP_INDEX(bodyGuard, prop->propID, prop->drawable, prop->texture, 0);
	}

	if (!featureDifferentWeapons) {
		WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, savedBodSkin->weapon, 999, false, true);
		WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, savedBodSkin->weapon, 1);
		WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
	}

	for (std::vector<SavedBodSkinDBRow*>::iterator it = savedBodSkins.begin(); it != savedBodSkins.end(); ++it)
	{
		delete (*it);
	}
	savedBodSkins.clear();

	return false;
}

void save_current_bod_skin(int slot)
{
	//BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	//Player player = PLAYER::PLAYER_ID();
	//Ped playerPed = PLAYER::PLAYER_PED_ID();

	std::string result_b_s = show_keyboard("Number", NULL);
	if (!result_b_s.empty())
	{
		result_b_s = trim(result_b_s);
		std::string::size_type sz;
		for (int i = 0; i < 7; i++) {
			sprintf(temp_n, "%i", i);
			if (result_b_s == temp_n) is_it_n = true;
		}
		if (is_it_n == true) {
			b_curr_num = std::stof(result_b_s, &sz);
			is_it_n = false;
		}
		if (!spawnedENTBodyguards.empty() && b_curr_num > -1 && b_curr_num < spawnedENTBodyguards.size()) {
			//if (bPlayerExists)
	//{
			std::ostringstream ss;
			if (slot != -1)
			{
				ss << activeSavedBodSkinSlotName;
			}
			else
			{
				ss << "Saved Skin " << (lastKnownSavedBodSkinCount + 1);
			}

			auto existingText = ss.str();
			std::string result = show_keyboard(NULL, (char*)existingText.c_str());
			if (!result.empty())
			{
				ENTDatabase* database = get_database();

				if (database->save_bod_skin(spawnedENTBodyguards[b_curr_num], result, slot))
				{
					activeSavedBodSkinSlotName = result;
					set_status_text("Saved skin");
				}
				else
				{
					set_status_text("Save error");
				}
			}
			//}
		}
		else {
			if (spawnedENTBodyguards.empty()) {
				std::ostringstream ss;
				ss << "No bodyguards found";
				set_status_text(ss.str());
			}
			if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
				std::ostringstream ss;
				ss << "Wrong number";
				set_status_text(ss.str());
			}
		}
	}
}

bool bod_skin_save_menu_interrupt()
{
	if (bodskinSaveMenuInterrupt)
	{
		bodskinSaveMenuInterrupt = false;
		return true;
	}
	return false;
}

bool bod_skin_save_slot_menu_interrupt()
{
	if (bodskinSaveSlotMenuInterrupt)
	{
		bodskinSaveSlotMenuInterrupt = false;
		return true;
	}
	return false;
}

bool onconfirm_bod_savedskin_slot_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 1: //spawn
		spawn_saved_bod_skin(activeSavedBodSkinIndex, activeSavedBodSkinSlotName);
		break;
	case 2: //overwrite
	{
		save_current_bod_skin(activeSavedBodSkinIndex);
		requireRefreshOfBodSkinSaveSlots = true;
		requireRefreshOfBodSkinSlotMenu = true;
		bodskinSaveSlotMenuInterrupt = true;
		bodskinSaveMenuInterrupt = true;
	}
	break;
	case 3: //rename
	{
		std::string result = show_keyboard(NULL, (char*)activeSavedBodSkinSlotName.c_str());
		if (!result.empty())
		{
			ENTDatabase* database = get_database();
			database->rename_saved_bod_skin(result, activeSavedBodSkinIndex);
			activeSavedBodSkinSlotName = result;
		}
		requireRefreshOfBodSkinSaveSlots = true;
		requireRefreshOfBodSkinSlotMenu = true;
		bodskinSaveSlotMenuInterrupt = true;
		bodskinSaveMenuInterrupt = true;
	}
	break;
	case 4: //delete
	{
		ENTDatabase* database = get_database();
		database->delete_saved_bod_skin(activeSavedBodSkinIndex);
		requireRefreshOfBodSkinSlotMenu = false;
		requireRefreshOfBodSkinSaveSlots = true;
		bodskinSaveSlotMenuInterrupt = true;
		bodskinSaveMenuInterrupt = true;
	}
	break;
	}
	return false;
}

bool onconfirm_bod_savedskin_menu(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		save_current_bod_skin(-1);
		requireRefreshOfBodSkinSaveSlots = true;
		bodskinSaveMenuInterrupt = true;
		return false;
	}

	activeSavedBodSkinIndex = choice.value;
	activeSavedBodSkinSlotName = choice.caption;
	return process_bod_savedskin_slot_menu(choice.value);
}

bool process_bod_savedskin_menu()
{
	do
	{
		bodskinSaveMenuInterrupt = false;
		requireRefreshOfBodSkinSlotMenu = false;
		requireRefreshOfBodSkinSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedBodSkinDBRow*> savedBodSkins = database->get_saved_bod_skins();

		lastKnownSavedBodSkinCount = savedBodSkins.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = -1;
		item->caption = "Create New Bodyguard Save";
		menuItems.push_back(item);

		for each (SavedBodSkinDBRow *sv in savedBodSkins)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "Saved Bodyguards", onconfirm_bod_savedskin_menu, NULL, NULL, bod_skin_save_menu_interrupt);

		for (std::vector<SavedBodSkinDBRow*>::iterator it = savedBodSkins.begin(); it != savedBodSkins.end(); ++it)
		{
			delete (*it);
		}
		savedBodSkins.clear();
	} while (requireRefreshOfBodSkinSaveSlots);

	return false;
}

bool process_bod_savedskin_slot_menu(int slot)
{
	do
	{
		bodskinSaveSlotMenuInterrupt = false;
		requireRefreshOfBodSkinSlotMenu = false;

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

		draw_generic_menu<int>(menuItems, 0, activeSavedBodSkinSlotName, onconfirm_bod_savedskin_slot_menu, NULL, NULL, bod_skin_save_slot_menu_interrupt);
	} while (requireRefreshOfBodSkinSlotMenu);
	return false;
}
// end of save/load bodyguard

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

	item = new MenuItem<int>();
	item->caption = "Enter Name Manually";
	item->value = 3;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Modify Skin";
	item->value = 4;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "Save/Load Bodyguard";
	item->value = 5;
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
		case 3:
		{
			std::string result = show_keyboard(NULL, (char*)lastCustomBodyguardSpawn.c_str());
			if (!result.empty())
			{
				result = trim(result);
				lastCustomBodyguardSpawn = result;
				Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());
				if (lastCustomBodyguardSpawn != "random" && lastCustomBodyguardSpawn != "Random" && lastCustomBodyguardSpawn != "RANDOM" && (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_VALID(hash)))
				{
					std::ostringstream ss;
					ss << "Couldn't find model '" << result << "'";
					set_status_text(ss.str());
					lastCustomBodyguardSpawn = "";
					return false;
				}
				if ((STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_VALID(hash)) || lastCustomBodyguardSpawn == "random" || lastCustomBodyguardSpawn == "Random" || lastCustomBodyguardSpawn == "RANDOM")
				{
					get_current_model_name();
					requireRefreshOfBodyguardMainMenu = true;
				}
				
			}
			return false;
		}
		case 4:
		{
			std::string result_b = show_keyboard("Number", NULL);
			if (!result_b.empty())
			{
				result_b = trim(result_b);
				std::string::size_type sz;
				for (int i = 0; i < 7; i++) {
					sprintf(temp_n, "%i", i);
					if (result_b == temp_n) is_it_n = true;
				}
				if (is_it_n == true) {
					b_curr_num = std::stof(result_b, &sz);
					is_it_n = false;
				}
				if (!spawnedENTBodyguards.empty() && b_curr_num > -1 && b_curr_num < spawnedENTBodyguards.size()) return process_bod_skinchanger_detail_menu();
				else {
					if (spawnedENTBodyguards.empty()) {
						std::ostringstream ss;
						ss << "No bodyguards found";
						set_status_text(ss.str());
						return false;
					}
					if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
						std::ostringstream ss;
						ss << "Wrong number";
						set_status_text(ss.str());
						return false;
					}
				}
			}
			return false;
		}
		case 5:
			return process_bod_savedskin_menu();
		default:
			break;
	}
	return false;
}

std::string get_current_model_name(){
	if (lastCustomBodyguardSpawn == "") {
		switch (skinTypesBodyguardMenuLastConfirmed[0]) {
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
	}
	else value = lastCustomBodyguardSpawn;
	return value;
}

Hash get_current_model_hash(){
	if (lastCustomBodyguardSpawn == "") {
		switch (skinTypesBodyguardMenuLastConfirmed[0]) {
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
	}
	else value = lastCustomBodyguardSpawn;
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
	lastCustomBodyguardSpawn = "";
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
	lastCustomBodyguardSpawn = "";
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
	lastCustomBodyguardSpawn = "";
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
	item->caption = "Toggle All Weapons In Category";
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

	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_body_blipflash_index);
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

	if(spawnedENTBodyguards.size() == 0){
		set_status_text("You don't have any bodyguards");
		spawnedENTBodyguards.clear();
		spawnedENTBodyguards.shrink_to_fit();
		return;
	}

	for(int i = 0; i < spawnedENTBodyguards.size(); i++){
		ENTITY::SET_ENTITY_INVINCIBLE(spawnedENTBodyguards[i], false);
		PED::SET_PED_NEVER_LEAVES_GROUP(spawnedENTBodyguards[i], false);
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[i], false);
		PED::REMOVE_PED_FROM_GROUP(spawnedENTBodyguards[i]);
		if (featureBodyguardDespawn) {
			AI::CLEAR_PED_TASKS(spawnedENTBodyguards[i]);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedENTBodyguards[i]);
		}
	}

	spawnedENTBodyguards.clear();
	spawnedENTBodyguards.shrink_to_fit();
	requireRefreshOfBodyguardMainMenu = true;

	pop = -1;
	all_selected = -1;
	if (!WEAPONS.empty()) {
		WEAPONS.clear();
		WEAPONS.shrink_to_fit();
	}
	
	dist_diff = -1;
	bod_pass = false;
	me_to_follow = false;
	if (!B_VEHICLE.empty()) {
		for (int g = 0; g < B_VEHICLE.size(); g++) {
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&B_VEHICLE[g]);
		}
		B_VEHICLE.clear();
		B_VEHICLE.shrink_to_fit();
	}

	set_status_text("Bodyguards dismissed");
}

void do_spawn_bodyguard(){
	requireRefreshOfBodyguardMainMenu = true;
	bool bodyguard_animal = false;
	bodyGuard = -1;
	bool exist_already = false;
	DWORD bodyGuardModel = -1;
	float random_category, random_bodyguard = -1;

	// random bodyguard
	if (lastCustomBodyguardSpawn == "random" || lastCustomBodyguardSpawn == "Random" || lastCustomBodyguardSpawn == "RANDOM") {
		random_category = (rand() % 10 + 0); // UP MARGIN + DOWN MARGIN
		if (random_category == 0) {
			random_bodyguard = (rand() % SKINS_PLAYER_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_PLAYER_VALUES[random_bodyguard].c_str());
		}
		if (random_category > 0 && random_category < 10) {
			random_bodyguard = (rand() % SKINS_GENERAL_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_GENERAL_VALUES[random_bodyguard].c_str());
		}
		if (random_category == 10) {
			random_bodyguard = (rand() % SKINS_ANIMALS_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_ANIMALS_VALUES[random_bodyguard].c_str());
		}
	}
	
	if (hotkey_boddyguard == false && lastCustomBodyguardSpawn != "random" && lastCustomBodyguardSpawn != "Random" && lastCustomBodyguardSpawn != "RANDOM") bodyGuardModel = get_current_model_hash();
	if (hotkey_boddyguard == true) {
		if (hotkey_b == false) {
			hotkey_b = true;
			process_bodyguard_menu();
			process_main_menu();
		}
		if (skinTypesBodyguardMenuLastConfirmed[0] == 0) bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_PLAYER_VALUES[skinTypesBodyguardMenuLastConfirmed[1]].c_str());
		if (skinTypesBodyguardMenuLastConfirmed[0] == 1) bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_GENERAL_VALUES[skinTypesBodyguardMenuLastConfirmed[1]].c_str());
		if (skinTypesBodyguardMenuLastConfirmed[0] == 2) bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_ANIMALS_VALUES[skinTypesBodyguardMenuLastConfirmed[1]].c_str());
		hotkey_boddyguard = false;
	}

	if (load_saved_bodyguard == true) bodyGuardModel = temp_bodyguard;

	if (spawnedENTBodyguards.size() >= BODYGUARD_LIMIT) {
		set_status_text("Cannot spawn any more bodyguards");
		return;
	}

	if((STREAMING::IS_MODEL_IN_CDIMAGE(bodyGuardModel) && STREAMING::IS_MODEL_VALID(bodyGuardModel)) || added_nearest_b == true){
		STREAMING::REQUEST_MODEL(bodyGuardModel);
		while(!STREAMING::HAS_MODEL_LOADED(bodyGuardModel)){
			make_periodic_feature_call();
			WAIT(0);
		}

		myENTGroup = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID());

		Vector3 spawnCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 2.0, 2.0, 0.0); // 2.5 2.5
		Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		
		// add nearest ped as bodyguard
		if (added_nearest_b == false) bodyGuard = PED::CREATE_PED(25, bodyGuardModel, spawnCoords.x, spawnCoords.y, spawnCoords.z, 0, 0, 0);
		if (added_nearest_b == true) { 
			const int arrSize33 = 1024;
			Ped surr_peds[arrSize33];
			int count_surr_peds = worldGetAllPeds(surr_peds, arrSize33);
			float dist_diff = -1.0;
			float temp_dist = 20.0;
			for (int i = 0; i < count_surr_peds; i++) {
				if (surr_peds[i] != PLAYER::PLAYER_PED_ID()) { 
					Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_peds[i], true);
					dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
					exist_already = false;
					if (!spawnedENTBodyguards.empty()) {
						for (int j = 0; j < spawnedENTBodyguards.size(); j++) {
							if (spawnedENTBodyguards[j] == surr_peds[i]) exist_already = true;
						}
					}
					if (temp_dist > dist_diff && exist_already == false) {
						temp_dist = dist_diff;
						temp_bodyguard = surr_peds[i];
					}
				}
			}
			exist_already = false;
			if (!spawnedENTBodyguards.empty()) {
				for (int i = 0; i < spawnedENTBodyguards.size(); i++) {
					if (spawnedENTBodyguards[i] == temp_bodyguard) exist_already = true;
				}
			}
			if (exist_already == false) {
				Hash temp_model = ENTITY::GET_ENTITY_MODEL(temp_bodyguard);
				Vector3 coords_temp_ped = ENTITY::GET_ENTITY_COORDS(temp_bodyguard, true);
				PED::DELETE_PED(&temp_bodyguard);
				bodyGuard = PED::CREATE_PED(25, temp_model, coords_temp_ped.x, coords_temp_ped.y, coords_temp_ped.z, 0, 0, 0);
			}
		}
		//
		
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

			spawnedENTBodyguards.push_back(bodyGuard); // save the current bodyguard

			PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), myENTGroup);
			PED::SET_PED_AS_GROUP_MEMBER(bodyGuard, myENTGroup);
			PED::SET_PED_NEVER_LEAVES_GROUP(bodyGuard, true);
			PED::SET_PED_DIES_IN_WATER(bodyGuard, false);
			PED::SET_PED_DIES_INSTANTLY_IN_WATER(bodyGuard, false);
			PED::SET_ENABLE_SCUBA(bodyGuard, true);

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
				if (LIMP_IF_INJURED_VALUES[BodyBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_body[0], true);
				if (LIMP_IF_INJURED_VALUES[BodyBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_body[0], true);
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
			
			// animal
			if (bodyguard_animal == true) {
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 46, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 5, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 17, true);
				PED::SET_PED_FLEE_ATTRIBUTES(bodyGuard, 0, false);
				PED::SET_PED_CAN_BE_TARGETTED(bodyGuard, true);
			}
			//
			
			if (bodyguard_animal == false) {
				PED::SET_PED_CAN_SWITCH_WEAPON(bodyGuard, true);
				PED::SET_GROUP_FORMATION(myENTGroup, BODY_GROUPFORMATION_VALUES[BodyGroupFormationIndex]); // 1 
				PED::SET_GROUP_FORMATION_SPACING(myENTGroup, BODY_BLIPSIZE_VALUES[BodyDistanceIndex], BODY_BLIPSIZE_VALUES[BodyDistanceIndex], BODY_BLIPSIZE_VALUES[BodyDistanceIndex]); // 2.0, 2.0, 2.0
			}
			PED::SET_CAN_ATTACK_FRIENDLY(bodyGuard, false, false);

			AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(bodyGuard, 100.0f, 0);
			PED::SET_PED_KEEP_TASK(bodyGuard, true);

			if (bodyguard_animal == false) PED::SET_PED_FIRING_PATTERN(bodyGuard, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_FULL_AUTO")); // 0xC6EE6B4C

			// Different Weapons
			if (featureDifferentWeapons/* && load_saved_bodyguard == false*/) {
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
			
			if (!featureDifferentWeapons && load_saved_bodyguard == false) {
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

			// randomize appearance
			if (featureRandomApp) {
				PED::CLEAR_ALL_PED_PROPS(bodyGuard);
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(bodyGuard, true);
				PED::SET_PED_RANDOM_PROPS(bodyGuard);
				WAIT(0);
			}
			//

			// bodyguard health
			if (PLAYER_HEALTH_VALUES[BodyHealthIndex] > 0) {
				PED::SET_PED_MAX_HEALTH(bodyGuard, PLAYER_HEALTH_VALUES[BodyHealthIndex]);
				ENTITY::SET_ENTITY_HEALTH(bodyGuard, PLAYER_HEALTH_VALUES[BodyHealthIndex]);
			}
			//

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

bool not_bodyguards_in_vehicle()
{
	for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
		if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(spawnedENTBodyguards[n]) && PED::GET_PED_TYPE(spawnedENTBodyguards[n]) != 28) return true;
	}
	return false;
}

void maintain_bodyguards(){
	if (spawnedENTBodyguards.size() == 0)
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
		dist_diff = -1;
		bod_pass = false;
		me_to_follow = false;
		if (!B_VEHICLE.empty()) {
			for (int g = 0; g < B_VEHICLE.size(); g++) {
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&B_VEHICLE[g]);
			}
			B_VEHICLE.clear();
			B_VEHICLE.shrink_to_fit();
		}
	}
	
	if (!spawnedENTBodyguards.empty()) { 
		Vector3 my_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		for (int i = 0; i < spawnedENTBodyguards.size(); i++) {
			// bodyguards swimming ability
			if (ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 1 && !is_in_airbrake_mode() && PED::GET_PED_TYPE(spawnedENTBodyguards[i]) != 28 && stop_b == false) {
				float height = -1.0;
				Object taskSequence;
				Vector3 bod_coords = ENTITY::GET_ENTITY_COORDS(spawnedENTBodyguards[i], true);
				WATER::GET_WATER_HEIGHT(my_coords.x, my_coords.y, my_coords.z, &height);
				if ((my_coords.z < height) && ((height - my_coords.z) > 2) && ENTITY::IS_ENTITY_IN_WATER(spawnedENTBodyguards[i]) == 1) {
					if ((bod_coords.z - my_coords.z) > 2) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 0, 0, -2.6, 0, 0, 0, true, false, true, true, true, true); // 1
					if ((bod_coords.z - my_coords.z) < -1) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 0, 0, 2.6, 0, 0, 0, true, false, true, true, true, true); // 0
					if ((bod_coords.x - my_coords.x) > 6 || (my_coords.x - bod_coords.x) > 6 || (bod_coords.y - my_coords.y) > 6 || (my_coords.y - bod_coords.y) > 6) {
						if (!PED::IS_PED_FACING_PED(spawnedENTBodyguards[i], PLAYER::PLAYER_PED_ID(), 50)) AI::TASK_TURN_PED_TO_FACE_COORD(spawnedENTBodyguards[i], my_coords.x, my_coords.y, my_coords.z, 10000);
					}
					if ((bod_coords.x - my_coords.x) > 6 && (bod_coords.x > my_coords.x)) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, -2.6, 0, 0, 0, 0, 0, true, false, true, true, true, true);
					if ((my_coords.x - bod_coords.x) > 6 && (bod_coords.x < my_coords.x)) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 2.6, 0, 0, 0, 0, 0, true, false, true, true, true, true);
					if ((bod_coords.y - my_coords.y) > 6 && (bod_coords.y > my_coords.y)) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 0, -2.6, 0, 0, 0, 0, true, false, true, true, true, true);
					if ((my_coords.y - bod_coords.y) > 6 && (bod_coords.y < my_coords.y)) ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 0, 2.6, 0, 0, 0, 0, true, false, true, true, true, true);
				}
				if (((height - my_coords.z) < 1) && ((height - bod_coords.z) > 2) && ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 1 && ENTITY::IS_ENTITY_IN_WATER(spawnedENTBodyguards[i]) == 1) 
					ENTITY::APPLY_FORCE_TO_ENTITY(spawnedENTBodyguards[i], 1, 0, 0, 2.6, 0, 0, 0, true, false, true, true, true, true);
			}
			//
			// animals
			if (animal_in_group == true) {
				Vector3 cop_coords;
				const int arrSize_animals = 1024;
				Ped surr_animals[arrSize_animals];
				int count_animals = worldGetAllPeds(surr_animals, arrSize_animals);
				for (int k = 0; k < count_animals; k++) {
					PED::SET_PED_CAN_RAGDOLL(spawnedENTBodyguards[i], 0);
					if (dist_diff != -1) PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[i], true);
					else PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[i], false);
					if (surr_animals[k] != PLAYER::PLAYER_PED_ID() && surr_animals[k] != spawnedENTBodyguards[i] && PED::IS_PED_IN_MELEE_COMBAT(surr_animals[k])) {
						if (PED::GET_PED_TYPE(spawnedENTBodyguards[i]) == 28) {
							PED::SET_PED_AS_ENEMY(surr_animals[k], true);
							AI::TASK_COMBAT_PED_TIMED(spawnedENTBodyguards[i], surr_animals[k], 50000, 16); // 50000
						}
					}
					if (surr_animals[k] != PLAYER::PLAYER_PED_ID() && surr_animals[k] != spawnedENTBodyguards[i] && PED::IS_PED_SHOOTING(surr_animals[k])) {
						if (PED::GET_PED_TYPE(spawnedENTBodyguards[i]) == 28) {
							if (AI::IS_PED_STILL(spawnedENTBodyguards[i]) && dist_diff == -1) {
								cop_to_kill = surr_animals[k];
								cop_coords = ENTITY::GET_ENTITY_COORDS(cop_to_kill, true);
								AI::TASK_GO_TO_COORD_ANY_MEANS(spawnedENTBodyguards[i], cop_coords.x, cop_coords.y, cop_coords.z, 3.0, 0, 0, 786603, 0xbf800000);
							}
							if (AI::IS_PED_STILL(spawnedENTBodyguards[i]) && dist_diff >= 2) {
								cop_coords = ENTITY::GET_ENTITY_COORDS(cop_to_kill, true);
								AI::TASK_GO_TO_COORD_ANY_MEANS(spawnedENTBodyguards[i], cop_coords.x, cop_coords.y, cop_coords.z, 3.0, 0, 0, 786603, 0xbf800000);
							}
							PED::SET_PED_AS_ENEMY(cop_to_kill, true);
							Vector3 dog_coords = ENTITY::GET_ENTITY_COORDS(spawnedENTBodyguards[i], true);
							cop_coords = ENTITY::GET_ENTITY_COORDS(cop_to_kill, true);
							dist_diff = SYSTEM::VDIST(dog_coords.x, dog_coords.y, dog_coords.z, cop_coords.x, cop_coords.y, cop_coords.z);
							if (/*AI::IS_PED_STILL(spawnedENTBodyguards[i]) && */dist_diff < 2) {
								AI::TASK_COMBAT_PED_TIMED(spawnedENTBodyguards[i], cop_to_kill, 50000, 16); // AI::TASK_COMBAT_PED(spawnedENTBodyguards[i], cop_to_kill, 0, 16); // AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(spawnedENTBodyguards[i], 100.0, 0);
								AI::TASK_WRITHE(cop_to_kill, spawnedENTBodyguards[i], 50000, 0);
							}
							if (ENTITY::IS_ENTITY_DEAD(cop_to_kill) || ENTITY::IS_ENTITY_DEAD(spawnedENTBodyguards[i]) || !ENTITY::DOES_ENTITY_EXIST(cop_to_kill)) dist_diff = -1;
						} // end of is it animal
					}
				}
				if (PED::IS_PED_FLEEING(spawnedENTBodyguards[i])) AI::TASK_STAND_STILL(spawnedENTBodyguards[i], 10000);
			}
			// modify skin
			if (menu_showing == true) {
				Vector3 head_c = PED::GET_PED_BONE_COORDS(spawnedENTBodyguards[i], 31086, 0, 0, 0);
				std::string curr_i = std::to_string(i);
				GRAPHICS::SET_DRAW_ORIGIN(head_c.x, head_c.y, head_c.z + 0.5, 0);
				UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
				UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)curr_i.c_str());
				UI::SET_TEXT_FONT(0);
				UI::SET_TEXT_SCALE(0.5, 0.5);
				UI::SET_TEXT_WRAP(0.0, 1.0);
				UI::SET_TEXT_COLOUR(255, 242, 0, 255);
				UI::SET_TEXT_CENTRE(0);
				UI::SET_TEXT_DROPSHADOW(20, 20, 20, 20, 20);
				UI::SET_TEXT_EDGE(0, 0, 0, 0, 255);
				UI::SET_TEXT_OUTLINE();
				UI::SET_TEXT_LEADING(1);
				UI::END_TEXT_COMMAND_DISPLAY_TEXT(0, 0);
			}
			//
			if (stop_b == false) {
				PED::SET_PED_AS_GROUP_MEMBER(spawnedENTBodyguards[i], PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID()));
				PED::SET_PED_NEVER_LEAVES_GROUP(spawnedENTBodyguards[i], true);
			}
			if (stop_b == true) {
				PED::REMOVE_PED_FROM_GROUP(spawnedENTBodyguards[i]);
				AI::CLEAR_PED_TASKS(spawnedENTBodyguards[i]);
			}
			if (PED::IS_PED_DEAD_OR_DYING(spawnedENTBodyguards[i], true)) {
				PED::SET_PED_NEVER_LEAVES_GROUP(spawnedENTBodyguards[i], false);
				PED::REMOVE_PED_FROM_GROUP(spawnedENTBodyguards[i]);
				if (featureBodyguardDespawn) ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedENTBodyguards[i]);
				spawnedENTBodyguards.erase(spawnedENTBodyguards.begin() + i);
				requireRefreshOfBodyguardMainMenu = true;
			}
		} // end of for (int i = 0; i < spawnedENTBodyguards.size(); i++)
		
		// bodyguards follow you in vehicle
		if (LIMP_IF_INJURED_VALUES[FollowInVehicleIndex] > 0) {
			Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID())) {
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
				Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh);
				int maxSeats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(currVehModel);
				if (VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(veh) == (maxSeats - 1) && bod_pass == false) {
					if (not_bodyguards_in_vehicle()) {
						const int arrSize33 = 1024;
						Ped surr_vehs[arrSize33];
						int count_surr_vehs = worldGetAllVehicles(surr_vehs, arrSize33);
						float dist_diff = -1.0;
						float temp_dist = 2000.0;
						for (int t = 0; t < count_surr_vehs; t++) {
							if ((VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) == 4 && (VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], -1) || VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 0) ||
								VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 1) || VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 2))) || (VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) == 3 &&
								(VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], -1) || VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 0) || VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 1))) ||
									(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) == 2 && (VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], -1) || VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], 0))) ||
								(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) == 1 && VEHICLE::IS_VEHICLE_SEAT_FREE(surr_vehs[t], -1))) {
								if (surr_vehs[t] != veh &&
									(VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(surr_vehs[t])))) {
									Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_vehs[t], true);
									dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
									if (temp_dist > dist_diff) {
										temp_dist = dist_diff;
										temp_vehicle = surr_vehs[t];
									}
								}
							}
						}
						if (!B_VEHICLE.empty()) {
							bool added_already = false;
							for (int g = 0; g < B_VEHICLE.size(); g++) {
								if (temp_vehicle == B_VEHICLE[g]) added_already = true;
							}
							if (added_already == false) B_VEHICLE.push_back(temp_vehicle);
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(B_VEHICLE.back(), 1, 1);
							bod_pass = true;
						}
						if (B_VEHICLE.empty()) {
							B_VEHICLE.push_back(temp_vehicle);
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(B_VEHICLE.back(), 1, 1);
							bod_pass = true;
						}
					}
				}
				if (!B_VEHICLE.empty()) {
					if (bod_pass == true/* && not_bodyguards_in_vehicle()*/) {
						for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
							if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(spawnedENTBodyguards[n])) {
								for (int m = 0; m < B_VEHICLE.size(); m++) {
									bool added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 2)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 2);
										PED::DELETE_PED(&temp_bodyguard_d);
										AI::TASK_ENTER_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 1000, 2, 2.0, 16, 0);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 1)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 1);
										PED::DELETE_PED(&temp_bodyguard_d);
										AI::TASK_ENTER_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 1000, 1, 2.0, 16, 0);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 0)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 0);
										PED::DELETE_PED(&temp_bodyguard_d);
										AI::TASK_ENTER_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 1000, 0, 2.0, 16, 0);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1);
										PED::DELETE_PED(&temp_bodyguard_d);
										AI::TASK_ENTER_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 1000, -1, 2.0, 16, 0);
									}
								}
							}
						}
					}
					if (not_bodyguards_in_vehicle()) {
						for (int m = 0; m < B_VEHICLE.size(); m++) {
							if ((VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(B_VEHICLE[m])) == 4 && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], -1) && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 0) &&
								!VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 1) && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 2)) || (VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(B_VEHICLE[m])) == 3 &&
									!VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], -1) && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 0) && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 1)) ||
									(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(B_VEHICLE[m])) == 2 && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], -1) && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], 0)) ||
								(VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(ENTITY::GET_ENTITY_MODEL(B_VEHICLE[m])) == 1 && !VEHICLE::IS_VEHICLE_SEAT_FREE(B_VEHICLE[m], -1)) && bod_pass == true) {
								bod_pass = false;
								me_to_follow = false;
							}
						}
					}
					if (me_to_follow == false) {
						for (int m = 0; m < B_VEHICLE.size(); m++) {
							VEHICLE::SET_VEHICLE_ENGINE_ON(B_VEHICLE[m], true, true);
							AI::SET_DRIVE_TASK_CRUISE_SPEED(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 300.0);
							if (LIMP_IF_INJURED_VALUES[FollowInVehicleIndex] == 1) AI::TASK_VEHICLE_CHASE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), PLAYER::PLAYER_PED_ID());
							if (LIMP_IF_INJURED_VALUES[FollowInVehicleIndex] == 2) AI::TASK_VEHICLE_ESCORT(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), B_VEHICLE[m], veh, -1, 140.0f, 32, 10, 1, 1); // 786468 32
							AI::SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 60.0f);
							AI::SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 32, true); // 786603
							PED::SET_DRIVER_AGGRESSIVENESS(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 0.1f);
							PED::SET_DRIVER_ABILITY(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 0.9f);
						}
						me_to_follow = true;
					}
				}
			} // end of in vehicle
			if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID())) {
				bod_pass = false;
				me_to_follow = false;
				if (!B_VEHICLE.empty()) {
					for (int g = 0; g < B_VEHICLE.size(); g++) {
						if (ENTITY::DOES_ENTITY_EXIST(B_VEHICLE[g]) && VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[g], -1) != 0) AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[g], -1), B_VEHICLE[g], 1);
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&B_VEHICLE[g]);
					}
					B_VEHICLE.clear();
					B_VEHICLE.shrink_to_fit();
				}
			}
			if (!B_VEHICLE.empty()) {
				for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
					Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(spawnedENTBodyguards[n], true);
					dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
					if (dist_diff > 450) { // 250
						Vector3 closestRoad;
						if (PATHFIND::GET_CLOSEST_ROAD(coordsme.x - 70, coordsme.y - 70, coordsme.z, 1.f, 1, &closestRoad, &closestRoad, 0, 0, 0, 0))
						{
							ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), closestRoad.x, closestRoad.y, closestRoad.z, 0, 0, 0, 1);
							me_to_follow = false;
						}
					}
					if (PED::IS_PED_SHOOTING(spawnedENTBodyguards[n])) me_to_follow = false;
				}
				if (not_bodyguards_in_vehicle()) {
					bod_pass = false;
					me_to_follow = false;
				}
			}
		} // end of bodyguards follow you in vehicle
	} // end of if (!spawnedENTBodyguards.empty())
} // end of void maintain_bodyguards()

bool process_bodyguard_menu(){
	do{
		requireRefreshOfBodyguardMainMenu = false;
		
		std::string caption = "Bodyguard Options";

		std::vector<MenuItem<int>*> menuItems;
		MenuItem<int> *item;
		ToggleMenuItem<int>* toggleItem;
		SelectFromListMenuItem *listItem;

		int i = 0;

		item = new MenuItem<int>();
		std::ostringstream ss;
		ss << "Spawn Bodyguard: " << get_current_model_name(); 
		item->caption = ss.str();
		//item->value = i++;
		item->value = 0;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Add Nearest Ped As Bodyguard";
		//item->value = i++;
		item->value = 1;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Dismiss All Bodyguards";
		//item->value = i++;
		item->value = 2;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Toggle Bodyguards To Follow Player";
		//item->value = i++;
		item->value = 3;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Choose Model";
		//item->value = i++;
		item->value = 4;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Choose Weapons";
		//item->value = i++;
		item->value = 5;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "Mark On Map";
		//item->value = i++;
		item->value = 6;
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

		listItem = new SelectFromListMenuItem(BODY_BLIPSIZE_CAPTIONS, onchange_body_distance_index);
		listItem->wrap = false;
		listItem->caption = "Spawn Distance";
		listItem->value = BodyDistanceIndex;
		menuItems.push_back(listItem);

		listItem = new SelectFromListMenuItem(BODY_GROUPFORMATION_CAPTIONS, onchange_body_groupformation_index);
		listItem->wrap = false;
		listItem->caption = "Group Formation";
		listItem->value = BodyGroupFormationIndex;
		menuItems.push_back(listItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Random Appearance";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureRandomApp;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		/*toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "Follow In Vehicle";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureFollowInVehicle;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);*/

		listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_follow_invehicle_index);
		listItem->wrap = false;
		listItem->caption = "Follow In Vehicle";
		listItem->value = FollowInVehicleIndex;
		menuItems.push_back(listItem);

		listItem = new SelectFromListMenuItem(PLAYER_HEALTH_CAPTIONS, onchange_body_health_index);
		listItem->wrap = false;
		listItem->caption = "Bodyguard Health";
		listItem->value = BodyHealthIndex;
		menuItems.push_back(listItem);

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

	//switch(choice.value){
	switch (activeLineIndexBodyguards) {
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
		for(int i = 0; i < spawnedENTBodyguards.size(); i++){
			ENTITY::SET_ENTITY_INVINCIBLE(spawnedENTBodyguards[i], featureBodyguardInvincible);
		}
	}
}

void add_bodyguards_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardInvincible", &featureBodyguardInvincible});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardHelmet", &featureBodyguardHelmet});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardDespawn", &featureBodyguardDespawn});
	results->push_back(FeatureEnabledLocalDefinition{"featureDifferentWeapons", &featureDifferentWeapons});
	results->push_back(FeatureEnabledLocalDefinition{"featureRandomApp", &featureRandomApp});
	//results->push_back(FeatureEnabledLocalDefinition{"featureFollowInVehicle", &featureFollowInVehicle});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardOnMap", &featureBodyguardOnMap});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyBlipNumber", &featureBodyBlipNumber});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardInfAmmo", &featureBodyguardInfAmmo});
}

void add_bodyguards_feature_enablements2(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "BodyBlipSizeIndex", std::to_string(BodyBlipSizeIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyDistanceIndex", std::to_string(BodyDistanceIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyGroupFormationIndex", std::to_string(BodyGroupFormationIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipColourIndex", std::to_string(BodyBlipColourIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipSymbolIndex", std::to_string(BodyBlipSymbolIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipFlashIndex", std::to_string(BodyBlipFlashIndex) });
	results->push_back(StringPairSettingDBRow{ "FollowInVehicleIndex", std::to_string(FollowInVehicleIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyHealthIndex", std::to_string(BodyHealthIndex) });
}

void add_bodyguards_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory0", std::to_string(skinTypesBodyguardMenuPositionMemory[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory1", std::to_string(skinTypesBodyguardMenuPositionMemory[1])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed0", std::to_string(skinTypesBodyguardMenuLastConfirmed[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed1", std::to_string(skinTypesBodyguardMenuLastConfirmed[1])});
	results->push_back(StringPairSettingDBRow{"lastCustomBodyguardSpawn", lastCustomBodyguardSpawn});
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
		else if (setting.name.compare("BodyDistanceIndex") == 0) {
			BodyDistanceIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyGroupFormationIndex") == 0) {
			BodyGroupFormationIndex = stoi(setting.value);
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
		else if (setting.name.compare("FollowInVehicleIndex") == 0) {
			FollowInVehicleIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyHealthIndex") == 0) {
			BodyHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("lastCustomBodyguardSpawn") == 0) {
			lastCustomBodyguardSpawn = setting.value;
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
	featureRandomApp = false;
	//featureFollowInVehicle = false;
	BodyBlipSizeIndex = 2;
	BodyDistanceIndex = 7;
	BodyGroupFormationIndex = 1;
	BodyBlipColourIndex = 0;
	BodyBlipSymbolIndex = 0;
	BodyBlipFlashIndex = 0;
	FollowInVehicleIndex = 0;
	BodyHealthIndex = 6;
}

void onchange_body_blipsize_index(int value, SelectFromListMenuItem* source){
	BodyBlipSizeIndex = value;
	BodyBlipSize_Changed = true;
}

void onchange_body_distance_index(int value, SelectFromListMenuItem* source) {
	BodyDistanceIndex = value;
	BodyDistance_Changed = true;
}

void onchange_body_groupformation_index(int value, SelectFromListMenuItem* source) {
	BodyGroupFormationIndex = value;
	BodyGroupFormationChanged = true;
}

void onchange_follow_invehicle_index(int value, SelectFromListMenuItem* source) {
	FollowInVehicleIndex = value;
	FollowInVehicleChanged = true;
}

void onchange_body_health_index(int value, SelectFromListMenuItem* source) {
	BodyHealthIndex = value;
	BodyHealthChanged = true;
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