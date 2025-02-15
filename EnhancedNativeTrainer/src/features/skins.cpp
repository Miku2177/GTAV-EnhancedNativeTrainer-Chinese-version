/*
这段代码的部分内容最初是作为 GTA V SCRIPT HOOK SDK 的一部分开始的。
http://dev-c.com
(C) Alexander Blade 2015

现在它是增强原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 和其他贡献者 2015
*/

#include "script.h"
#include "skins.h"
#include "..\ui_support\menu_functions.h"
#include "weapons.h"

#pragma warning(disable : 4192)

/***
* VARS
*/

DWORD model_to_restore = -1;

bool featurenoblood = false;
bool featurepersprops = false;

// 自动皮肤变量
bool auto_skin = false;
bool reset_skin = false;
int skin_tick, skin_tick_secs_passed, skin_tick_secs_curr = 0;
Ped oldplayerSkin = -1;

int skinDetailMenuIndex = 0;
int skinDetailMenuValue = 0;

int skinMainMenuPosition = 0;

bool helmet_on = false;

int skinPropsMenuPosition = 0;
int skinPropsCategoryValue = 0;
int skinPropsDrawablePosition[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

int activeSavedSkinIndex = -1;
int activeLineIndexSkinChanger = 0;

std::string activeSavedSkinSlotName;
int lastKnownSavedSkinCount = 0;
bool skinSaveMenuInterrupt = false;
bool skinSaveSlotMenuInterrupt = false;
bool requireRefreshOfSkinSaveSlots = false;
bool requireRefreshOfSkinSlotMenu = false;

std::string lastCustomSkinSpawn;

int skinTypesMenuPositionMemory[4] = { 0, 0, 0, 0 }; //玩家，动物，一般，测试

int ped_prop_idx_0 = -1;
int ped_prop_idx_1 = -1;
int choicevalue = -2;
int skinPropsCategoryValueC = -2;
int clear_props_m = -2;

// 在角色死亡时重置玩家模型
int ResetSkinOnDeathIdx = 0;
bool ResetSkinOnDeathChanged = true;

// 自动应用最后保存的皮肤
const std::vector<std::string> SKINS_AUTO_SKIN_SAVED_CAPTIONS{ "关", "恢复角色", "仅限已保存角色" };
int AutoApplySkinSavedIndex = 0;
bool AutoApplySkinSavedChanged = true;

/***
* 方法
*/

void onchange_skins_reset_skin_ondeath_index(int value, SelectFromListMenuItem* source) {
	ResetSkinOnDeathIdx = value;
	ResetSkinOnDeathChanged = true;
}

void onchange_auto_apply_skin_saved_index(int value, SelectFromListMenuItem* source) {
	AutoApplySkinSavedIndex = value;
	AutoApplySkinSavedChanged = true;
}

void reset_skin_globals()
{
	activeLineIndexSkinChanger = 0;
	featurenoblood = false;
	featurepersprops = false;
	ResetSkinOnDeathIdx = 0;
	AutoApplySkinSavedIndex = 0;
}

/*
* ===============
* 工作方法
* =================
*/

bool applyChosenSkin(std::string skinName)
{
	DWORD model = GAMEPLAY::GET_HASH_KEY((char *)skinName.c_str());
	return applyChosenSkin(model);
}

bool applyChosenSkin(DWORD model) 
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		Vehicle veh = NULL;
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		}

		save_player_weapons(PLAYER::PLAYER_PED_ID());

		PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
		WAIT(0);

		if (veh != NULL)
		{
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);
		}

		restore_player_weapons(PLAYER::PLAYER_PED_ID());

		// 重置皮肤细节选项
		skinDetailMenuIndex = 0;
		skinDetailMenuValue = 0;

		WAIT(100);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

		return true;
	}
	return false;
}

std::string getSkinDetailAttribDescription(int i)
{
	switch (i)
	{
	case 0:
		return "头部/面部";
	case 1:
		return "胡须/面具";
	case 2:
		return "头发/帽子";
	case 3:
		return "上衣";
	case 4:
		return "裤子";
	case 5:
		return "配饰/手套";
	case 6:
		return "配饰/鞋子";
	case 7:
	case 8:
	case 9:
		return "物品";
	case 10:
		return "徽章";
	case 11:
		return "衬衫/夹克";
	default:
		return std::to_string(i);
	}
}

std::string getPropDetailAttribDescription(int i)
{
	switch (i)
	{
	case 0:
		return "帽子/面具/头盔";
	case 1:
		return "眼镜";
	case 2:
		return "耳环";
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

/*
* ===============
* 纹理菜单
* =================
*/

void onhighlight_skinchanger_texture_menu(MenuItem<int> choice)
{
	if (true)//PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, value))
	{
		int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue);
		PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, currentDrawable, choice.value, 0);
	}
	WAIT(100);
}

bool onconfirm_skinchanger_texture_menu(MenuItem<int> choice)
{
	onhighlight_skinchanger_texture_menu(choice);

	return true;
}

void onexit_skinchanger_texture_menu(bool returnValue)
{
	/*
	//恢复已应用的选择
	int texture;
	if (skinTextureMenuValue == -1)
	{
	texture = findFirstValidPedTexture(skinDrawableMenuValue);
	}
	else
	{
	texture = skinTextureMenuValue;
	}

	if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, texture))
	{
	PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, skinDrawableMenuValue, texture, 0);
	}
	*/
}

void update_skin_features() {
	// 没有血迹，没有弹孔
	if (featurenoblood) PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID()); 

	// 持久化道具
	if (featurepersprops && ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 0/* && (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) > -1 || PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) > -1)*/) {
		if ((ped_prop_idx_0 > -1 && PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) == -1) || (ped_prop_idx_1 > -1 && PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) == -1)) {
			Vector3 me_c = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			GAMEPLAY::CLEAR_AREA_OF_OBJECTS(me_c.x, me_c.y, me_c.z, 1.0, 0);
		}
		if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) > -1) ped_prop_idx_0 = PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0);
		if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) > -1) ped_prop_idx_1 = PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1);
		if (ped_prop_idx_0 > -1 && PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) == -1) PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0, ped_prop_idx_0, 0, 0);
		if (ped_prop_idx_1 > -1 && PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) == -1) PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1, ped_prop_idx_1, 0, 0);
		
		if (choicevalue == -1 && skinPropsCategoryValueC == 0) {
			ped_prop_idx_0 = -1;
			if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) != -1) PED::CLEAR_PED_PROP(PLAYER::PLAYER_PED_ID(), 0);
		}
		if (choicevalue == -1 && skinPropsCategoryValueC == 1) {
			ped_prop_idx_1 = -1;
			if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) != -1) PED::CLEAR_PED_PROP(PLAYER::PLAYER_PED_ID(), 1);
		}
		if (clear_props_m == -1) {
			ped_prop_idx_0 = -1;
			ped_prop_idx_1 = -1;
			if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 0) != -1) PED::CLEAR_PED_PROP(PLAYER::PLAYER_PED_ID(), 0);
			if (PED::GET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), 1) != -1) PED::CLEAR_PED_PROP(PLAYER::PLAYER_PED_ID(), 1);
			clear_props_m = -2;
		}
	}

	// 自动应用最后保存的皮肤
	if (NPC_RAGDOLL_VALUES[AutoApplySkinSavedIndex] > 0) {
		if (auto_skin == false) {
			skin_tick_secs_passed = clock() / CLOCKS_PER_SEC;
			if (((clock() / (CLOCKS_PER_SEC / 1000)) - skin_tick_secs_curr) != 0) {
				skin_tick = skin_tick + 1;
				skin_tick_secs_curr = skin_tick_secs_passed;
			}
			if (skin_tick > 200) {
				ENTDatabase* database = get_database();
				std::vector<SavedSkinDBRow*> savedSkins = database->get_saved_skins();
				
				if (!savedSkins.empty()) {
					Hash model = -1;
					if (reset_skin == false) {
						GAMEPLAY::_RESET_LOCALPLAYER_STATE();
						reset_skin = true;
						model = 1;
					}
					
					if (savedSkins.at(savedSkins.size() - 1) != NULL) {
						SavedSkinDBRow* savedSkin = savedSkins.at(savedSkins.size() - 1);
						database->populate_saved_skin(savedSkin);

						bool right_model = false;

						if (model != -1) {
							if (NPC_RAGDOLL_VALUES[AutoApplySkinSavedIndex] == 1) applyChosenSkin(savedSkin->model);
							if (NPC_RAGDOLL_VALUES[AutoApplySkinSavedIndex] == 2 && ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) == savedSkin->model) applyChosenSkin(PLAYER::PLAYER_PED_ID()); // applyChosenSkin(savedSkin->model);
							if (NPC_RAGDOLL_VALUES[AutoApplySkinSavedIndex] == 2 && ENTITY::GET_ENTITY_MODEL(PLAYER::PLAYER_PED_ID()) != savedSkin->model) right_model = true;

							if (right_model == false) {
								Ped ped = PLAYER::PLAYER_PED_ID();
								for each (SavedSkinComponentDBRow * comp in savedSkin->components) {
									PED::SET_PED_COMPONENT_VARIATION(ped, comp->slotID, comp->drawable, comp->texture, 0);
								}
								PED::CLEAR_ALL_PED_PROPS(ped);
								for each (SavedSkinPropDBRow * prop in savedSkin->props) {
									PED::SET_PED_PROP_INDEX(ped, prop->propID, prop->drawable, prop->texture, 0);
								}
								for (std::vector<SavedSkinDBRow*>::iterator it = savedSkins.begin(); it != savedSkins.end(); ++it) {
									delete (*it);
								}
								savedSkins.clear();
							}

							oldplayerSkin = PLAYER::PLAYER_PED_ID();
							skin_tick = 0;
							auto_skin = true;
							reset_skin = false;
						}
					}
				} // 结束! empty
			} // 结束! skin_tick
		} // 结束! auto_skin

		if (PLAYER::PLAYER_PED_ID() != oldplayerSkin) auto_skin = false;
		if ((time_since_d > -1 && time_since_d < 2000) || (player_died == true && !featureNoAutoRespawn)) auto_skin = false;
		if (DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) auto_skin = false;

	} // 自动皮肤功能结束
}

bool process_skinchanger_texture_menu(std::string caption)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue);
		int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, currentDrawable);
		for (int i = 0; i < textures; i++)
		{
			std::ostringstream ss;
			ss << "款式项 #" << i;
			MenuItem<int> *item = new MenuItem<int>();
			item->caption = ss.str();
			item->value = i;
			menuItems.push_back(item);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "可用款式项";

	int currentTexture = PED::GET_PED_TEXTURE_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue);
	draw_generic_menu<int>(menuItems, &currentTexture, ss.str(), onconfirm_skinchanger_texture_menu, onhighlight_skinchanger_texture_menu, onexit_skinchanger_texture_menu);
	return false;
}

/*
* ===============
* 皮肤可绘制项菜单
* =================
*/

bool onconfirm_skinchanger_drawable_menu(MenuItem<int> choice)
{
	if (choice.isLeaf)
	{
		return false;
	}
	return process_skinchanger_texture_menu(choice.caption);
}

void onhighlight_skinchanger_drawable_menu(MenuItem<int> choice)
{
	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue);
	if (choice.value != currentDrawable)
	{
		PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), skinDetailMenuValue, choice.value, 0, 0);
	}
	WAIT(100);
}

void onexit_skinchanger_drawable_menu(bool returnValue)
{
}

bool process_skinchanger_drawable_menu(std::string caption, int component)
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component);
		for (int i = 0; i < drawables; i++)
		{
			int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), component, i);
			std::ostringstream ss;
			ss << "皮肤项 #" << i << " ~HUD_COLOUR_GREYLIGHT~(" << textures << ")";

			MenuItem<int> *item = new MenuItem<int>();
			item->caption = ss.str();
			item->value = i;
			item->isLeaf = (textures <= 1);
			menuItems.push_back(item);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::ostringstream ss;
	ss << "可用皮肤项";

	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(PLAYER::PLAYER_PED_ID(), component);
	draw_generic_menu<int>(menuItems, &currentDrawable, ss.str(), onconfirm_skinchanger_drawable_menu, onhighlight_skinchanger_drawable_menu, onexit_skinchanger_drawable_menu);
	return false;
}

/*
* ===============
* 皮肤细节菜单
* =================
*/

void onhighlight_skinchanger_detail_menu(MenuItem<int> choice)
{
	// 什么都不做
}

int lastTriedComponentIndex = 0;
int lastTriedPalette = 0;

bool onconfirm_skinchanger_detail_menu(MenuItem<int> choice)
{
	skinDetailMenuIndex = choice.currentMenuIndex;
	skinDetailMenuValue = choice.value;

	return process_skinchanger_drawable_menu(choice.caption, choice.value);
}

bool process_skinchanger_detail_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int fixedChoices = 0;
	const int partVariations = 12;

	int i = 0;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);

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

			int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex);
			int textures = 0;
			if (drawables == 1)
			{
				textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex, 0);
			}
			if (drawables > 1 || textures != 0)
			{
				std::ostringstream ss;
				std::string itemText = getSkinDetailAttribDescription(compIndex);
				ss << "槽 " << (compIndex + 1) << ": " << itemText << " ~HUD_COLOUR_GREYLIGHT~(" << drawables << ")";

				MenuItem<int> *item = new MenuItem<int>();
				item->caption = ss.str();
				item->value = compIndex;
				item->isLeaf = false;
				menuItems.push_back(item);
			}
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	return draw_generic_menu<int>(menuItems, &skinDetailMenuIndex, "皮肤类型", onconfirm_skinchanger_detail_menu, onhighlight_skinchanger_detail_menu, NULL);
}

/*
* ===============
* 玩家皮肤选择
* =================
*/

bool onconfirm_skinchanger_choices_players(MenuItem<std::string> choice)
{
	skinTypesMenuPositionMemory[0] = choice.currentMenuIndex;
	applyChosenSkin(choice.value);
	return false;
}

bool process_skinchanger_choices_players()
{
	std::vector<MenuItem<std::string>*> menuItems;

	for (int i = 0; i < SKINS_PLAYER_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_PLAYER_CAPTIONS[i];
		item->value = SKINS_PLAYER_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesMenuPositionMemory[0], "主角模型", onconfirm_skinchanger_choices_players, NULL, NULL);
}

/*
* ===============
* 在线玩家皮肤选择
* =================
*/

bool onconfirm_skinchanger_choices_online_npc(MenuItem<std::string> choice)
{
	skinTypesMenuPositionMemory[0] = choice.currentMenuIndex;
	applyChosenSkin(choice.value);
	return false;
}

bool process_skinchanger_choices_online_npc()
{
	std::vector<MenuItem<std::string>*> menuItems;

	for (int i = 0; i < SKINS_ONLINE_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_ONLINE_CAPTIONS[i];
		item->value = SKINS_ONLINE_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesMenuPositionMemory[0], "在线 NPC 角色", onconfirm_skinchanger_choices_online_npc, NULL, NULL);
}


/*
* ===============
* 动物皮肤选择
* =================
*/

bool onconfirm_skinchanger_choices_animals(MenuItem<std::string> choice)
{
	skinTypesMenuPositionMemory[1] = choice.currentMenuIndex;
	
	Vector3 coords_me = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	float height = -1.0;

	if (choice.value != "a_c_dolphin" && choice.value != "a_c_sharkhammer" && choice.value != "a_c_humpback" &&
		choice.value != "a_c_killerwhale" && choice.value != "a_c_stingray" &&
		choice.value != "a_c_sharktiger" && choice.value != "a_c_fish" && choice.value != "a_c_whalegrey") {
		WATER::GET_WATER_HEIGHT(coords_me.x, coords_me.y, coords_me.z, &height);
		if (coords_me.z > height) applyChosenSkin(choice.value);
	}
	else {
		WATER::GET_WATER_HEIGHT(coords_me.x, coords_me.y, coords_me.z, &height);
		if ((coords_me.z < height) && ((height - coords_me.z) > 1)) applyChosenSkin(choice.value);
	}
	
	return false;
}

bool process_skinchanger_choices_animals()
{
	std::vector<MenuItem<std::string>*> menuItems;

	for (int i = 0; i < SKINS_ANIMAL_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_ANIMAL_CAPTIONS[i];
		item->value = SKINS_ANIMAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesMenuPositionMemory[1], "动物模型", onconfirm_skinchanger_choices_animals, NULL, NULL);
}

/*
* ===============
* 普通皮肤选择
* =================
*/

bool onconfirm_skinchanger_choices_misc(MenuItem<std::string> choice)
{
	skinTypesMenuPositionMemory[2] = choice.currentMenuIndex;
	applyChosenSkin(choice.value);
	return false;
}

bool process_skinchanger_choices_misc()
{
	std::vector<MenuItem<std::string>*> menuItems;

	for (int i = 0; i < SKINS_GENERAL_CAPTIONS.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_GENERAL_CAPTIONS[i];
		item->value = SKINS_GENERAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesMenuPositionMemory[2], "普通 NPC 角色", onconfirm_skinchanger_choices_misc, NULL, NULL);
}

bool onconfirm_skinchanger_choices_test(MenuItem<std::string> choice)
{
	skinTypesMenuPositionMemory[3] = choice.currentMenuIndex;
	applyChosenSkin(choice.value);
	return false;
}

/*bool process_skinchanger_choices_test()
{
	std::vector<MenuItem<std::string>*> menuItems;

	for (int i = 0; i < SKINS_TEST_VALUES.size(); i++)
	{
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_TEST_VALUES[i];
		item->value = SKINS_TEST_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesMenuPositionMemory[3], "Test Skins", onconfirm_skinchanger_choices_test, NULL, NULL);
}*/

/*
* ===============
* 皮肤主菜单
* =================
*/

bool onconfirm_skinchanger_category_menu(MenuItem<int> choice)
{
	switch (choice.value) {
		case 0: //玩家
			process_skinchanger_choices_players();
			break;
		case 1: //动物
			process_skinchanger_choices_animals();
			break;
		case 2: //杂项
			process_skinchanger_choices_misc();
			break;
		case 3: //在线 NPC
			process_skinchanger_choices_online_npc();
			break;
		case 4: //自定义条目
		{
			keyboard_on_screen_already = true;
			curr_message = "输入人物名称代码: (例如: ig_hao)"; // 改变你的皮肤
			std::string result = show_keyboard("手动输入名称", (char*)lastCustomSkinSpawn.c_str());
			if (!result.empty())
			{
				result = trim(result);
				lastCustomSkinSpawn = result;
				Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());
				if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_VALID(hash))
				{
					std::ostringstream ss;
					ss << "~r~错误！~s~找不到此模型 '" << result << "'";
					set_status_text(ss.str());
					return false;
				}
				else
				{
					return applyChosenSkin(hash);
				}
			}
			return false;
		}
	}
	return false;
}

bool onconfirm_skinchanger_menu(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	std::ostringstream ss;
	int index = PED::GET_PED_PROP_INDEX(playerPed, 0);

	switch (activeLineIndexSkinChanger) { // 选项值
		case 0:
			process_savedskin_menu();
			break;
		case 1: //更换皮肤
			process_skinchanger_category_menu();
			break;
		case 2: //细节
			process_skinchanger_detail_menu();
			break;
		case 3:
			process_prop_menu();
			break;
		case 4: //重置
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
			set_status_text("已重置为默认皮肤！");
			break;
		case 5:
			PED::CLEAR_ALL_PED_PROPS(playerPed);
			clear_props_m = -1;
			ped_prop_idx = -1;
			break;
		case 6:
			PED::CLEAR_ALL_PED_PROPS(playerPed);
			PED::SET_PED_RANDOM_COMPONENT_VARIATION(playerPed, true);
			PED::SET_PED_RANDOM_PROPS(playerPed);
			break;
		case 7:
			PED::CLEAR_ALL_PED_PROPS(playerPed);
			PED::SET_PED_RANDOM_PROPS(playerPed);
			break;
		case 8:
			if (helmet_on == false) {
				Hash model = -1;
				if (PED::GET_PED_TYPE(playerPed) == 0) model = GAMEPLAY::GET_HASH_KEY("player_zero");
				if (PED::GET_PED_TYPE(playerPed) == 1) model = GAMEPLAY::GET_HASH_KEY("player_one");
				if (PED::GET_PED_TYPE(playerPed) == 2 || PED::GET_PED_TYPE(playerPed) == 3) model = GAMEPLAY::GET_HASH_KEY("player_two");
				applyChosenSkin(model);
				helmet_on = true;
			}
			int temp_choice = rand() % 15 + 0;
			PED::GIVE_PED_HELMET(playerPed, 1, 4096, temp_choice);
			break;
	}
	return false;
}

int skinCategoryPosition = 0;

bool process_skinchanger_category_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;

	int i = 0;

	item = new MenuItem<int>();
	item->caption = "主角";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "动物";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "普通 NPC";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "在线 NPC";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "手动输入名称";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, &skinCategoryPosition, "角色模型分类", onconfirm_skinchanger_category_menu, NULL, NULL);
}

bool process_skinchanger_menu()
{
	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;
	MenuItem<int> *item;
	
	int i = 0;

	item = new MenuItem<int>();
	item->caption = "保存的皮肤";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "更改角色模型";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "修改当前皮肤";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "修改饰品";
	item->value = i++;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "重置当前皮肤";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "删除饰品";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "随机外观皮肤";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "随机头部饰品";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "佩戴头盔";
	item->value = i++;
	item->isLeaf = true;
	menuItems.push_back(item);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "饰品永久化 (保留)";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurepersprops;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(SKINS_RESET_SKIN_ONDEATH_CAPTIONS, onchange_skins_reset_skin_ondeath_index);
	listItem->wrap = false;
	listItem->caption = "玩家模型";
	listItem->value = ResetSkinOnDeathIdx;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "没有血迹和弹孔";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featurenoblood;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(SKINS_AUTO_SKIN_SAVED_CAPTIONS, onchange_auto_apply_skin_saved_index);
	listItem->wrap = false;
	listItem->caption = "自动应用上次保存的皮肤";
	listItem->value = AutoApplySkinSavedIndex;
	menuItems.push_back(listItem);

  // 皮肤主菜单位置
	return draw_generic_menu<int>(menuItems, &activeLineIndexSkinChanger, "玩家外观选项", onconfirm_skinchanger_menu, NULL, NULL); 
}

/**
* 道具相关内容
*/

bool onconfirm_props_texture_menu(MenuItem<int> choice)
{
	onhighlight_props_texture_menu(choice);
	return true;
}

void onhighlight_props_texture_menu(MenuItem<int> choice)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	PED::SET_PED_PROP_INDEX(playerPed, skinPropsCategoryValue, skinPropsDrawablePosition[skinPropsCategoryValue]-1, choice.value, 0);
}

bool process_prop_texture_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int thisDrawable = skinPropsDrawablePosition[skinPropsCategoryValue] - 1;
	int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinPropsCategoryValue, thisDrawable);

	for (int i=0; i < textures; i++)
	{
		bool iFound = false;
		int compIndex = i;

		MenuItem<int> *item = new MenuItem<int>();

		std::ostringstream ss;
		ss << "款式项 #" << (i + 1);
		item->caption = ss.str();

		item->value = i;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int lastTexturePosition = PED::GET_PED_PROP_TEXTURE_INDEX(playerPed, skinPropsCategoryValue);
	return draw_generic_menu<int>(menuItems, &lastTexturePosition, "可用款式项", onconfirm_props_texture_menu, onhighlight_props_texture_menu, NULL);
}

bool onconfirm_props_drawable_menu(MenuItem<int> choice)
{
	skinPropsDrawablePosition[skinPropsCategoryValue] = choice.currentMenuIndex;
	if (choice.value != -1)
	{
		int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinPropsCategoryValue, choice.value);
		if (textures > 1)
		{
			process_prop_texture_menu();
		}
	}
	return false;
}

void onhighlight_props_drawable_menu(MenuItem<int> choice)
{
	skinPropsDrawablePosition[skinPropsCategoryValue] = choice.currentMenuIndex;

	Ped playerPed = PLAYER::PLAYER_PED_ID();
	int currentProp = PED::GET_PED_PROP_INDEX(playerPed, skinPropsCategoryValue);
	if (currentProp != choice.value) // 如果选定的可绘制对象与当前的不一致
	{
		PED::CLEAR_PED_PROP(playerPed, skinPropsCategoryValue);
		if (choice.value != -1)
		{
			PED::SET_PED_PROP_INDEX(playerPed, skinPropsCategoryValue, choice.value, 0, 0);
		}
	}

	choicevalue = choice.value;
	clear_props_m = -2;
	skinPropsCategoryValueC = skinPropsCategoryValue;
}

bool process_prop_drawable_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int drawables = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinPropsCategoryValue);

	int i = -1;

	for (; i < drawables; i++)
	{
		bool iFound = false;
		int compIndex = i;

		int textures = 0;
		//if (drawables > 1 || textures != 0)
		//示例（可绘制项 > 1 或 纹理 != 0）
		{
			MenuItem<int> *item = new MenuItem<int>();

			if (i == -1)
			{
				item->caption = "没有";
				item->isLeaf = true;
			}
			else
			{
				std::ostringstream ss;
				ss << "饰品项 #" << (i + 1);
				item->caption = ss.str();
				int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(PLAYER::PLAYER_PED_ID(), skinPropsCategoryValue, i);
				item->isLeaf = (textures <= 1);
			}

			item->value = i;
			menuItems.push_back(item);
		}
	}

	return draw_generic_menu<int>(menuItems, &skinPropsDrawablePosition[skinPropsCategoryValue], "可用饰品项", onconfirm_props_drawable_menu, onhighlight_props_drawable_menu, NULL);
}

bool onconfirm_props_menu(MenuItem<int> choice)
{
	skinPropsCategoryValue = choice.value;
	process_prop_drawable_menu();
	return false;
}

bool process_prop_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int fixedChoices = 0;
	const int partVariations = 10;

	int i = 0;
	int count = 0;

	for (; i < partVariations + fixedChoices; i++)
	{
		bool iFound = false;
		int compIndex = i - fixedChoices;

		int drawables = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(PLAYER::PLAYER_PED_ID(), compIndex);
		if (drawables > 0)
		{
			MenuItem<int> *item = new MenuItem<int>();

			std::ostringstream ss;
			
				std::string itemText = getPropDetailAttribDescription(compIndex);
				ss << "槽 " << (compIndex + 1) << ": " << itemText << " ~HUD_COLOUR_GREYLIGHT~(" << drawables << ")";
				item->caption = ss.str();

			item->value = compIndex;
			item->isLeaf = false;
			menuItems.push_back(item);
			count++;
		}
	}

	if (count == 0)
	{
		set_status_text("该模型没有可用的内容");
		return false;
	}

	return draw_generic_menu<int>(menuItems, &skinPropsMenuPosition, "饰品类型", onconfirm_props_menu, NULL, NULL);
}

bool skin_save_menu_interrupt()
{
	if (skinSaveMenuInterrupt)
	{
		skinSaveMenuInterrupt = false;
		return true;
	}
	return false;
}

bool skin_save_slot_menu_interrupt()
{
	if (skinSaveSlotMenuInterrupt)
	{
		skinSaveSlotMenuInterrupt = false;
		return true;
	}
	return false;
}

bool onconfirm_savedskin_menu(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		save_current_skin(-1);
		requireRefreshOfSkinSaveSlots = true;
		skinSaveMenuInterrupt = true;
		return false;
	}

	activeSavedSkinIndex = choice.value;
	activeSavedSkinSlotName = choice.caption;
	return process_savedskin_slot_menu(choice.value);
}

bool onconfirm_savedskin_slot_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 1: //生成
		spawn_saved_skin(activeSavedSkinIndex, activeSavedSkinSlotName);
		break;
	case 2: //重写
	{
		save_current_skin(activeSavedSkinIndex);
		requireRefreshOfSkinSaveSlots = true;
		requireRefreshOfSkinSlotMenu = true;
		skinSaveSlotMenuInterrupt = true;
		skinSaveMenuInterrupt = true;
	}
	break;
	case 3: //重命名
	{
		keyboard_on_screen_already = true;
		curr_message = "输入新的名称："; // 重命名已保存的皮肤
		std::string result = show_keyboard("手动输入名称", (char*)activeSavedSkinSlotName.c_str());
		if (!result.empty())
		{
			ENTDatabase* database = get_database();
			database->rename_saved_skin(result, activeSavedSkinIndex);
			activeSavedSkinSlotName = result;
		}
		requireRefreshOfSkinSaveSlots = true;
		requireRefreshOfSkinSlotMenu = true;
		skinSaveSlotMenuInterrupt = true;
		skinSaveMenuInterrupt = true;
	}
	break;
	case 4: //删除
	{
		ENTDatabase* database = get_database();
		database->delete_saved_skin(activeSavedSkinIndex);
		requireRefreshOfSkinSlotMenu = false;
		requireRefreshOfSkinSaveSlots = true;
		skinSaveSlotMenuInterrupt = true;
		skinSaveMenuInterrupt = true;
	}
	break;
	}
	return false;
}

bool process_savedskin_menu()
{
	do
	{
		skinSaveMenuInterrupt = false;
		requireRefreshOfSkinSlotMenu = false;
		requireRefreshOfSkinSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedSkinDBRow*> savedSkins = database->get_saved_skins();

		lastKnownSavedSkinCount = savedSkins.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = -1;
		item->caption = "创建新的皮肤存档";
		menuItems.push_back(item);

		for each (SavedSkinDBRow *sv in savedSkins)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "保存的皮肤", onconfirm_savedskin_menu, NULL, NULL, skin_save_menu_interrupt);

		for (std::vector<SavedSkinDBRow*>::iterator it = savedSkins.begin(); it != savedSkins.end(); ++it)
		{
			delete (*it);
		}
		savedSkins.clear();
	} while (requireRefreshOfSkinSaveSlots);

	return false;
}

bool process_savedskin_slot_menu(int slot)
{
	do
	{
		skinSaveSlotMenuInterrupt = false;
		requireRefreshOfSkinSlotMenu = false;

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 1;
		item->caption = "应用此皮肤";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 2;
		item->caption = "用当前内容覆盖";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 3;
		item->caption = "重命名";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 4;
		item->caption = "删除";
		menuItems.push_back(item);

		draw_generic_menu<int>(menuItems, 0, activeSavedSkinSlotName, onconfirm_savedskin_slot_menu, NULL, NULL, skin_save_slot_menu_interrupt);
	} while (requireRefreshOfSkinSlotMenu);
	return false;
}

bool spawn_saved_skin(int slot, std::string caption)
{
	ENTDatabase* database = get_database();

	std::vector<SavedSkinDBRow*> savedSkins = database->get_saved_skins(slot);

	SavedSkinDBRow* savedSkin = savedSkins.at(0);
	database->populate_saved_skin(savedSkin);

	applyChosenSkin(savedSkin->model);

	Ped ped = PLAYER::PLAYER_PED_ID();

	for each (SavedSkinComponentDBRow *comp in savedSkin->components)
	{
		PED::SET_PED_COMPONENT_VARIATION( ped, comp->slotID, comp->drawable, comp->texture, 0);
	}

	PED::CLEAR_ALL_PED_PROPS(ped);
	for each (SavedSkinPropDBRow *prop in savedSkin->props)
	{
		PED::SET_PED_PROP_INDEX(ped, prop->propID, prop->drawable, prop->texture, 0);
	}

	for (std::vector<SavedSkinDBRow*>::iterator it = savedSkins.begin(); it != savedSkins.end(); ++it)
	{
		delete (*it);
	}
	savedSkins.clear();

	return false;
}

void save_current_skin(int slot)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (bPlayerExists)
	{
		std::ostringstream ss;
		if (slot != -1)
		{
			ss << activeSavedSkinSlotName;
		}
		else
		{
			ss << "新建皮肤存档 " << (lastKnownSavedSkinCount + 1);
		}

		keyboard_on_screen_already = true;
		curr_message = "输入保存名称:"; // save a skin
		auto existingText = ss.str();
		std::string result = show_keyboard("手动输入名称", (char*)existingText.c_str());
		if (!result.empty())
		{
			ENTDatabase* database = get_database();
			
			if (database->save_skin(playerPed, result, slot))
			{
				activeSavedSkinSlotName = result;
				set_status_text("皮肤已保存！");
			}
			else
			{
				set_status_text("保存错误！");
			}
		}
	}
}

void add_skin_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "lastCustomSkinSpawn", lastCustomSkinSpawn });
	results->push_back(StringPairSettingDBRow{ "ResetSkinOnDeathIdx", std::to_string(ResetSkinOnDeathIdx) });
	results->push_back(StringPairSettingDBRow{ "AutoApplySkinSavedIndex", std::to_string(AutoApplySkinSavedIndex) });
}

void add_player_skin_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results) {
	results->push_back(FeatureEnabledLocalDefinition{ "featurenoblood", &featurenoblood });
	results->push_back(FeatureEnabledLocalDefinition{ "featurepersprops", &featurepersprops });
}

void handle_generic_settings_skin(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("lastCustomSkinSpawn") == 0)
		{
			lastCustomSkinSpawn = setting.value;
		}
		else if (setting.name.compare("ResetSkinOnDeathIdx") == 0) {
			ResetSkinOnDeathIdx = stoi(setting.value);
		}
		else if (setting.name.compare("AutoApplySkinSavedIndex") == 0) {
			AutoApplySkinSavedIndex = stoi(setting.value);
		}
	}
}

