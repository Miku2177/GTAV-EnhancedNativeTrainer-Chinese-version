/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Sondai Smith 及其他贡献者 2015
*/

#include "..\io\config_io.h"
#include <string.h>
#include "bodyguards.h"
#include "script.h"
#include "weapons.h"
#include "vehicles.h"
#include "hotkeys.h"
#include "area_effect.h"
#include "skins.h"

int activeLineIndexBodyguards = 0;

int lastSelectedBodWeaponCategory = 0;
int lastSelectedBodWeapon = 0;

int myENTGroup = -1;
int groupID = -1;
const int BODYGUARD_LIMIT = 7;// 保镖数量上限，默认设置为7

Ped cop_to_kill;
float dist_diff = -1;

int pop, all_selected = -1;
std::vector<Hash> WEAPONS;
std::vector<Ped> spawnedENTBodyguards;
std::vector<Hash> spawnedBodyguardsSecWeap;
std::vector<bool*> bodyguardWeaponsToggle[8];
bool bodyguardWeaponsToggleInitialized = false;
bool bod_pass, me_to_follow = false;
std::vector<Vehicle> B_VEHICLE;
Hash tmp_w = -1;

std::string lastCustomBodyguardSpawn;
std::string lastCustomBodyguardPedName; // For tracking custom ped display name
std::string value;

std::string selBodyWeapons;
bool under_weapon_menu = false;

bool spawning_a_ped = false;

int someone_shooting = 0;

int skinBodPropsMenuPosition = 0;
int skinBodPropsCategoryValue = 0;
int skinBodPropsDrawablePosition[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int choicebvalue = -2;
int skinBodPropsCategoryValueC = -2;
int clear_bod_props_m = -2;

bool stop_b = false;
bool c_armed = true;
bool featureBodyguardInvincible = false;
bool featureNoBodBlood = false;
bool featureBAggressivePed = false;
bool featureBCannotBeHeadshot = false;
bool featureBodyguardHelmet = false;
bool featureBodyguardDespawn = true;
bool featureDifferentWeapons = false;
bool featureRandomApp = false;
bool featureBodyguardYourWeapon = false;
bool featureAddRemoveWeapon = false;
bool featureBodyguardWeaponAttach = false;
bool featureBodyguardOnMap = false;
bool featureBodyguardInfAmmo = false;

int BodyWeaponSetIndex = 0;

bool hotkey_b = false;

int b_follow_m = -2;

bool added_nearest_b = false;
Ped bodyGuard, temp_bodyguard = -1;

// 修改皮肤
int skinBodDetailMenuIndex = 0;
int skinBodDetailMenuValue = 0;
float b_curr_num = -1;
bool is_it_n = false;
char temp_n[10];

// 保存/加载保镖
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

// 保镖雷达标记选项变量
Blip blip_body[1];
std::vector<Blip> BLIPTABLE_BODYGUARD;
bool animal_in_group = false;

// 第一个索引是类别，第二个是该类别中的位置
int skinTypesBodyguardMenuPositionMemory[2] = {0, 0};

// 第一个索引表示类别，第二个索引表示该类别中的位置。
int skinTypesBodyguardMenuLastConfirmed[2] = {0, 0};

// 雷达标记大小
int BodyBlipSizeIndex = 2;
bool BodyBlipSize_Changed = true;
int BodyDistanceIndex = 7;
bool BodyDistance_Changed = true;

// 雷达标记颜色
int BodyBlipColourIndex = 0;
bool BodyBlipColour_Changed = true;

// 雷达标记符号
int BodyBlipSymbolIndexN = 0;
bool BodyBlipSymbol_Changed = true;

// 保镖编队阵型
const std::vector<std::string> BODY_GROUPFORMATION_CAPTIONS{ "默认", "环形围绕玩家", "以玩家为中心并排" };
const int BODY_GROUPFORMATION_VALUES[] = { 0, 1, 3 };
int BodyGroupFormationIndex = 1;
bool BodyGroupFormationChanged = true;

// 显示编号
const std::vector<std::string> BODY_SHOWNUMBERS_CAPTIONS{ "当菜单打开时", "始终", "从不" };
int BodyShowNumbersIndex = 0;
bool BodyShowNumbersChanged = true;

// 雷达标记闪烁
int BodyBlipFlashIndex = 0;
bool BodyBlipFlash_Changed = true;
int FollowInVehicleIndex = 0;
bool FollowInVehicleChanged = true;

// 保镖生命值
int BodyHealthIndex = 6;
bool BodyHealthChanged = true;

const std::vector<std::string> SKINS_ANIMALS_CAPTIONS{ "罗威纳犬", "德国牧羊犬", "哈士奇", "美洲狮", "黑豹", "猎犬" };
const std::vector<std::string> SKINS_ANIMALS_VALUES{ "a_c_chop", "a_c_shepherd", "a_c_husky", "a_c_mtlion", "a_c_Panther", "a_c_retriever" };

// 修改皮肤
std::string getBodSkinDetailAttribDescription(int i)
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
		return "饰品";
	case 10:
		return "徽章";
	case 11:
		return "衬衫/夹克";
	default:
		return std::to_string(i);
	}
}

std::string getBodPropDetailAttribDescription(int i)
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

void onexit_bod_skinchanger_texture_menu(bool returnValue)
{
}

void onhighlight_bod_skinchanger_texture_menu(MenuItem<int> choice)
{
	if (true)
	{
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

	Hash model = ENTITY::GET_ENTITY_MODEL(spawnedENTBodyguards[b_curr_num]);

	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
		int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue, currentDrawable);

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

	int currentTexture = PED::GET_PED_TEXTURE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
	draw_generic_menu<int>(menuItems, &currentTexture, ss.str(), onconfirm_bod_skinchanger_texture_menu, onhighlight_bod_skinchanger_texture_menu, onexit_bod_skinchanger_texture_menu);
	return false;
}

void onexit_bod_skinchanger_drawable_menu(bool returnValue)
{
}

void onhighlight_bod_skinchanger_drawable_menu(MenuItem<int> choice)
{
	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], skinBodDetailMenuValue);
	if (choice.value != currentDrawable)
	{
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

	Hash model = ENTITY::GET_ENTITY_MODEL(spawnedENTBodyguards[b_curr_num]);
	
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			make_periodic_feature_call();
			WAIT(0);
		}

		int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], component);
		for (int i = 0; i < drawables; i++)
		{
			int textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], component, i);
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

	int currentDrawable = PED::GET_PED_DRAWABLE_VARIATION(spawnedENTBodyguards[b_curr_num], component);
	draw_generic_menu<int>(menuItems, &currentDrawable, ss.str(), onconfirm_bod_skinchanger_drawable_menu, onhighlight_bod_skinchanger_drawable_menu, onexit_bod_skinchanger_drawable_menu, b_skin_menu_interrupt);
	return false;
}

void onhighlight_bodskinchanger_detail_menu(MenuItem<int> choice)
{
	// 什么也不做
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
		set_status_text("~r~所有保镖都已经死亡了！");
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

			int drawables = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], compIndex);
			int textures = 0;
			if (drawables == 1)
			{
				textures = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], compIndex, 0);
			}
			if (drawables > 1 || textures != 0)
			{
				std::ostringstream ss;
				std::string itemText = getBodSkinDetailAttribDescription(compIndex);
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
	return draw_generic_menu<int>(menuItems, &skinBodDetailMenuIndex, "皮肤类型", onconfirm_bodskinchanger_detail_menu, onhighlight_bodskinchanger_detail_menu, NULL, b_skin_menu_interrupt);
}
// 结束 '修改皮肤'

void onhighlight_bod_props_texture_menu(MenuItem<int> choice)
{
	PED::SET_PED_PROP_INDEX(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue, skinBodPropsDrawablePosition[skinBodPropsCategoryValue] - 1, choice.value, 0);
}

bool onconfirm_bod_props_texture_menu(MenuItem<int> choice)
{
	onhighlight_bod_props_texture_menu(choice);
	return true;
}

bool process_bod_prop_texture_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int thisDrawable = skinBodPropsDrawablePosition[skinBodPropsCategoryValue] - 1;
	int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue, thisDrawable);

	for (int i = 0; i < textures; i++)
	{
		bool iFound = false;
		int compIndex = i;

		MenuItem<int>* item = new MenuItem<int>();

		std::ostringstream ss;
		ss << "款式项 #" << (i + 1);
		item->caption = ss.str();

		item->value = i;
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	int lastTexturePosition = PED::GET_PED_PROP_TEXTURE_INDEX(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue);
	return draw_generic_menu<int>(menuItems, &lastTexturePosition, "可用款式项", onconfirm_bod_props_texture_menu, onhighlight_bod_props_texture_menu, NULL);
}

bool onconfirm_bod_props_drawable_menu(MenuItem<int> choice)
{
	skinBodPropsDrawablePosition[skinBodPropsCategoryValue] = choice.currentMenuIndex;
	if (choice.value != -1)
	{
		int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue, choice.value);
		if (textures > 1)
		{
			process_bod_prop_texture_menu();
		}
	}
	return false;
}

void onhighlight_bod_props_drawable_menu(MenuItem<int> choice)
{
	skinBodPropsDrawablePosition[skinBodPropsCategoryValue] = choice.currentMenuIndex;

	int currentBodProp = PED::GET_PED_PROP_INDEX(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue);
	if (currentBodProp != choice.value) // 如果当前选中的可绘制项与现有项不符
	{
		PED::CLEAR_PED_PROP(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue);
		if (choice.value != -1)
		{
			PED::SET_PED_PROP_INDEX(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue, choice.value, 0, 0);
		}
	}

	choicebvalue = choice.value;
	clear_bod_props_m = -2;
	skinBodPropsCategoryValueC = skinBodPropsCategoryValue;
}

bool process_bod_prop_drawable_menu()
{
	DWORD waitTime = 150;
	int foundTextures = 0;
	std::vector<MenuItem<int>*> menuItems;

	int drawables = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue);

	int i = -1;

	for (; i < drawables; i++)
	{
		bool iFound = false;
		int compIndex = i;

		int textures = 0;
		//if (drawables > 1 || textures != 0)
		{
			MenuItem<int>* item = new MenuItem<int>();

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
				int textures = PED::GET_NUMBER_OF_PED_PROP_TEXTURE_VARIATIONS(spawnedENTBodyguards[b_curr_num], skinBodPropsCategoryValue, i);
				item->isLeaf = (textures <= 1);
			}

			item->value = i;
			menuItems.push_back(item);
		}
	}

	return draw_generic_menu<int>(menuItems, &skinBodPropsDrawablePosition[skinBodPropsCategoryValue], "可用饰品项", onconfirm_bod_props_drawable_menu, onhighlight_bod_props_drawable_menu, NULL);
}

bool onconfirm_bod_props_menu(MenuItem<int> choice)
{
	skinBodPropsCategoryValue = choice.value;
	process_bod_prop_drawable_menu();
	return false;
}

bool process_bod_prop_menu()
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

		int drawables = PED::GET_NUMBER_OF_PED_PROP_DRAWABLE_VARIATIONS(spawnedENTBodyguards[b_curr_num], compIndex);
		if (drawables > 0)
		{
			MenuItem<int>* item = new MenuItem<int>();

			std::ostringstream ss;

			std::string itemText = getBodPropDetailAttribDescription(compIndex);
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
		set_status_text("该模型没有任何可用内容！");
		return false;
	}

	return draw_generic_menu<int>(menuItems, &skinBodPropsMenuPosition, "饰品类型", onconfirm_bod_props_menu, NULL, NULL);
}
// 结束 '配饰部分'

bool process_bod_individual_weapon_menu() {
	Ped playerPed = equip_ped;

	for (int a = 0; a < 8; a++) { // VOV_WEAPON_VALUES.size()
		for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
			tmp_w = GAMEPLAY::GET_HASH_KEY((char*)VOV_WEAPON_VALUES[a].at(b).c_str());
			if (WEAPON::GET_SELECTED_PED_WEAPON(playerPed) == tmp_w) {
				lastSelectedBodWeaponCategory = a;
				lastSelectedBodWeapon = b;
			}
		}
	}

	std::string label = VOV_WEAPON_CAPTIONS[lastSelectedBodWeaponCategory].at(lastSelectedBodWeapon); // 武器索引
	std::string label_caption = UI::_GET_LABEL_TEXT(&label[0]);

	if (label_caption.compare("手枪口径 .50") == 0) {
		label_caption = "手枪口径 .50"; // 菜单标题不支持符号
	}

	std::vector<MenuItem<int>*> menuItems;
			
	int moddableIndex = -1;
	for (int i = 0; i < WEAPONTYPES_MOD.size(); i++) {
		if (WEAPON::GET_SELECTED_PED_WEAPON(equip_ped) == GAMEPLAY::GET_HASH_KEY((char*)WEAPONTYPES_MOD.at(i).c_str())) {
			moddableIndex = i;
			break;
		}
	}

	if (moddableIndex != -1) {
		std::vector<std::string> modCaptions = VOV_WEAPONMOD_CAPTIONS[moddableIndex];
		for (int i = 0; i < modCaptions.size(); i++) {
			FunctionDrivenToggleMenuItem<int>* item = new FunctionDrivenToggleMenuItem<int>();
			std::string label_caption = modCaptions.at(i);
			item->caption = UI::_GET_LABEL_TEXT(&label_caption[0]);
			item->getter_call = is_weaponmod_equipped;
			item->setter_call = set_weaponmod_equipped;
			item->extra_arguments.push_back(lastSelectedBodWeaponCategory);
			item->extra_arguments.push_back(lastSelectedBodWeapon); // 武器索引
			item->extra_arguments.push_back(moddableIndex);
			item->extra_arguments.push_back(i);
			menuItems.push_back(item);
		}
	}

	int tintableIndex = -1;
	for (int i = 0; i < WEAPONTYPES_TINT.size(); i++) {
		if (WEAPON::GET_SELECTED_PED_WEAPON(equip_ped) == GAMEPLAY::GET_HASH_KEY((char*)WEAPONTYPES_TINT.at(i).c_str())) {
			tintableIndex = i;
			break;
		}
	}

	if (tintableIndex != -1) {
		MenuItem<int>* tintItem = new MenuItem<int>();
		tintItem->caption = "武器涂装";
		tintItem->value = 4;
		tintItem->isLeaf = false;
		tintItem->onConfirmFunction = onconfirm_open_tint_menu;
		menuItems.push_back(tintItem);
	}

	draw_generic_menu<int>(menuItems, 0, label_caption, NULL, NULL, NULL, weapon_reequip_interrupt);

	return false;
}

// 保存/加载保镖
bool applyChosenBodSkin(DWORD model)
{
	if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_VALID(model))
	{
		STREAMING::REQUEST_MODEL(model);
		while (!STREAMING::HAS_MODEL_LOADED(model))
		{
			//make_periodic_feature_call();
			WAIT(0);
		}

		load_saved_bodyguard = true;
		temp_bodyguard = model;
		do_spawn_bodyguard();
		load_saved_bodyguard = false;

		PED::SET_PED_DEFAULT_COMPONENT_VARIATION(bodyGuard);
		WAIT(0);
		
		// 重置皮肤细节选择
		bodskinDetailMenuIndex = 0;
		bodskinDetailMenuValue = 0;

		WAIT(100);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(bodyGuard);
		
		return true;
	}
	return false;
}

bool spawn_saved_bod_skin(int slot, std::string caption)
{
	if (!spawnedENTBodyguards.empty() && spawnedENTBodyguards.size() >= BODYGUARD_LIMIT) {
		set_status_text("无法再生成更多保镖了! ");
		return false;
	}

	ENTDatabase* database = get_database();
	std::vector<SavedBodSkinDBRow*> savedBodSkins = database->get_saved_bod_skins(slot);
	SavedBodSkinDBRow* savedBodSkin = savedBodSkins.at(0);
	database->populate_saved_bod_skin(savedBodSkin);

	applyChosenBodSkin(savedBodSkin->model);

	PED::CLEAR_ALL_PED_PROPS(savedBodSkin->model);

	for each (SavedBodSkinComponentDBRow *comp in savedBodSkin->components)
	{
		PED::SET_PED_COMPONENT_VARIATION(bodyGuard, comp->slotID, comp->drawable, comp->texture, 0);
	}

	PED::CLEAR_ALL_PED_PROPS(bodyGuard);

	for each (SavedBodSkinPropDBRow *prop in savedBodSkin->props)
	{
		PED::SET_PED_PROP_INDEX(bodyGuard, prop->propID, prop->drawable, prop->texture, 0);
	}

	if (!featureDifferentWeapons) {
		int clipMax = WEAPON::GET_MAX_AMMO_IN_CLIP(bodyGuard, savedBodSkin->weapon, true); clipMax = min(clipMax, 250);
		if (WEAPON::HAS_PED_GOT_WEAPON(bodyGuard, savedBodSkin->weapon, 0)) {
			WEAPON::REMOVE_WEAPON_FROM_PED(bodyGuard, savedBodSkin->weapon);
			WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, savedBodSkin->weapon, clipMax * 2, false, true);
		}
		else WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, savedBodSkin->weapon, clipMax * 2, false, true);

		if (savedBodSkin->bcomp0 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp0);
		if (savedBodSkin->bcomp1 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp1);
		if (savedBodSkin->bcomp2 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp2);
		if (savedBodSkin->bcomp3 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp3);
		if (savedBodSkin->bcomp4 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp4);
		if (savedBodSkin->bcomp5 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp5);
		if (savedBodSkin->bcomp6 != -1) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(bodyGuard, savedBodSkin->weapon, savedBodSkin->bcomp6);
		if (savedBodSkin->bw_tint != -1) WEAPON::SET_PED_WEAPON_TINT_INDEX(bodyGuard, savedBodSkin->weapon, savedBodSkin->bw_tint);

		WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, savedBodSkin->weapon, 1);
		int maxAmmo = 0;
		WEAPON::GET_MAX_AMMO(bodyGuard, savedBodSkin->weapon, &maxAmmo);
		int maxClipAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(bodyGuard, savedBodSkin->weapon, false);

		WEAPON::SET_AMMO_IN_CLIP(bodyGuard, savedBodSkin->weapon, maxClipAmmo);
		WEAPON::SET_PED_AMMO(bodyGuard, savedBodSkin->weapon, maxAmmo);

		WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
		spawnedBodyguardsSecWeap.push_back(savedBodSkin->weapon);
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
	if (!WEAPON::IS_PED_ARMED(PLAYER::PLAYER_PED_ID(), 7)) CONTROLS::_SET_CONTROL_NORMAL(0, 37, 1);

	std::string result_b_s = "";

	if (spawnedENTBodyguards.size() > 1) {
		keyboard_on_screen_already = true;
		curr_message = "输入你想要保存的,  保镖编号!  (显示在其头顶上的数字)"; // 选择你想保存的保镖
		result_b_s = show_keyboard("手动输入名称", NULL);
	}
	if (spawnedENTBodyguards.size() == 1) result_b_s = "0";
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
			
			std::ostringstream ss;
			if (slot != -1)
			{
				ss << activeSavedBodSkinSlotName;
			}
			else
			{
				ss << "新建保镖存档 " << (lastKnownSavedBodSkinCount + 1);
			}

			keyboard_on_screen_already = true;
			curr_message = "请输入保存名称"; // 为选中的保镖输入一个保存名称
			auto existingText = ss.str();
			std::string result = show_keyboard("手动输入名称", (char*)existingText.c_str());
			if (!result.empty())
			{
				ENTDatabase* database = get_database();

				if (database->save_bod_skin(spawnedENTBodyguards[b_curr_num], result, slot))
				{
					activeSavedBodSkinSlotName = result;
					set_status_text("保存保镖成功了！");
				}
				else
				{
					set_status_text("保存保镖失败了！");
				}
			}
		}
		else {
			if (spawnedENTBodyguards.empty()) {
				std::ostringstream ss;
				ss << "未找到任何保镖！";
				set_status_text(ss.str());
			}
			if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
				std::ostringstream ss;
				ss << "编号错误！";
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
	case 1: //生成
		spawn_saved_bod_skin(activeSavedBodSkinIndex, activeSavedBodSkinSlotName);
		break;
	case 2: //覆盖
	{
		save_current_bod_skin(activeSavedBodSkinIndex);
		requireRefreshOfBodSkinSaveSlots = true;
		requireRefreshOfBodSkinSlotMenu = true;
		bodskinSaveSlotMenuInterrupt = true;
		bodskinSaveMenuInterrupt = true;
	}
	break;
	case 3: //重命名
	{
		keyboard_on_screen_already = true;
		curr_message = "请输入新的名称"; // 重命名已保存的保镖
		std::string result = show_keyboard("手动输入名称", (char*)activeSavedBodSkinSlotName.c_str());
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
	case 4: //删除
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
		item->caption = "创建新的保镖存档";
		menuItems.push_back(item);

		for each (SavedBodSkinDBRow *sv in savedBodSkins)
		{
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = sv->saveName;
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "保存的保镖", onconfirm_bod_savedskin_menu, NULL, NULL, bod_skin_save_menu_interrupt);

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
		item->caption = "生成";
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

		draw_generic_menu<int>(menuItems, 0, activeSavedBodSkinSlotName, onconfirm_bod_savedskin_slot_menu, NULL, NULL, bod_skin_save_slot_menu_interrupt);
	} while (requireRefreshOfBodSkinSlotMenu);
	return false;
}
// 结束保存/加载保镖部分

bool process_bodyguard_skins_menu(){
	std::vector<MenuItem<int>*> menuItems;
	MenuItem<int> *item;

	item = new MenuItem<int>();
	item->caption = "主角";
	item->value = 0;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "普通 NPC";
	item->value = 1;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "在线 NPC";
	item->value = 2;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "动物";
	item->value = 3;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "新增角色模型";
	item->value = 4;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "手动输入名称";
	item->value = 5;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "修改保镖皮肤";
	item->value = 6;
	item->isLeaf = false;
	menuItems.push_back(item);
	
	item = new MenuItem<int>();
	item->caption = "修改保镖饰品";
	item->value = 7;
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "修改保镖武器";
	item->value = 8;
	item->isLeaf = false;
	menuItems.push_back(item);

	return draw_generic_menu<int>(menuItems, &skinTypesBodyguardMenuPositionMemory[0], "更改保镖模型", onconfirm_bodyguard_skins_menu, NULL, NULL);
}

bool onconfirm_bodyguard_skins_menu(MenuItem<int> choice){
	switch(choice.value){
		case 0:
			return process_player_skins_menu();
		case 1:
			return process_npc_skins_menu();
		case 2:
			return process_online_skins_menu();
		case 3:
			return process_animal_skins_menu();
		case 4:
			return process_custom_peds_bodyguard_menu();
		case 5:
		{
			keyboard_on_screen_already = true;
			curr_message = "输入保镖模型名称: ( random 随机 random_story 随机多个 saved_bodyguards 已保存的 )"; // 生成一个保镖
			
			// Don't pre-fill with custom ped model name to avoid confusion
			std::string defaultInput = "";
			if (skinTypesBodyguardMenuLastConfirmed[0] != 4) {
				// Only pre-fill if not coming from custom peds
				defaultInput = lastCustomBodyguardSpawn;
			}
			
			std::string result = show_keyboard("手动输入名称", (char*)defaultInput.c_str());
			if (!result.empty())
			{
				result = trim(result);
				lastCustomBodyguardSpawn = result;
				// Clear custom ped name since we're using manual input
				lastCustomBodyguardPedName = "";
				Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());
				// 添加对中文指令的判断支持
				if (lastCustomBodyguardSpawn != "random" && lastCustomBodyguardSpawn != "Random" && lastCustomBodyguardSpawn != "RANDOM" && 
					lastCustomBodyguardSpawn != "saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_Bodyguards" && 
					lastCustomBodyguardSpawn != "random_story" && lastCustomBodyguardSpawn != "Random_story" && lastCustomBodyguardSpawn != "Random_Story" &&
					// 增加中文指令支持
					lastCustomBodyguardSpawn != "随机" && lastCustomBodyguardSpawn != "SJ" && lastCustomBodyguardSpawn != "sj" &&
					lastCustomBodyguardSpawn != "随机多个" && lastCustomBodyguardSpawn != "SJDG" && lastCustomBodyguardSpawn != "sjdg" &&
					lastCustomBodyguardSpawn != "已保存的" && lastCustomBodyguardSpawn != "YBCD" && lastCustomBodyguardSpawn != "ybcd" &&
					(!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_VALID(hash)))
				{
					std::ostringstream ss;
					ss << "找不到此模型 [" << result << "]";
					set_status_text(ss.str());
					lastCustomBodyguardSpawn = "";
					return false;
				}
				// 同样增加对中文指令的支持
				if ((STREAMING::IS_MODEL_IN_CDIMAGE(hash) && STREAMING::IS_MODEL_VALID(hash)) || 
					lastCustomBodyguardSpawn == "random" || lastCustomBodyguardSpawn == "Random" || lastCustomBodyguardSpawn == "RANDOM" ||
					lastCustomBodyguardSpawn == "saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_Bodyguards" ||
					lastCustomBodyguardSpawn == "random_story" || lastCustomBodyguardSpawn == "Random_story" || lastCustomBodyguardSpawn == "Random_Story" ||
					// 增加中文指令支持
					lastCustomBodyguardSpawn == "随机" || lastCustomBodyguardSpawn == "SJ" || lastCustomBodyguardSpawn == "sj" ||
					lastCustomBodyguardSpawn == "随机多个" || lastCustomBodyguardSpawn == "SJDG" || lastCustomBodyguardSpawn == "sjdg" ||
					lastCustomBodyguardSpawn == "已保存的" || lastCustomBodyguardSpawn == "YBCD" || lastCustomBodyguardSpawn == "ybcd")
				{
					// Set to manual input mode (not custom peds)
					skinTypesBodyguardMenuPositionMemory[0] = 5;
					skinTypesBodyguardMenuLastConfirmed[0] = 5;
					get_current_model_name();
					requireRefreshOfBodyguardMainMenu = true;
				}
			}
			return false;
		}
		case 6:
		{
			std::string result_b = "";

			if (spawnedENTBodyguards.size() > 1) {
				keyboard_on_screen_already = true;
				curr_message = "输入你想要修改 [皮肤或配饰] 的保镖编号!  (该编号显示在其头顶上)"; // 修改保镖的皮肤
				result_b = show_keyboard("手动输入名称", NULL);
			}
			if (spawnedENTBodyguards.size() == 1) result_b = "0";
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
						ss << "未找到任何保镖！";
						set_status_text(ss.str());
						return false;
					}
					if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
						std::ostringstream ss;
						ss << "编号错误！";
						set_status_text(ss.str());
						return false;
					}
				}
			}
			return false;
		}
		case 7:
		{
			std::string result_b = "";

			if (spawnedENTBodyguards.size() > 1) {
				keyboard_on_screen_already = true;
				curr_message = "输入你想要修改 [皮肤或配饰] 的保镖编号!  (该编号显示在其头顶上)"; // 修改保镖的皮肤
				result_b = show_keyboard("手动输入名称", NULL);
			}
			if (spawnedENTBodyguards.size() == 1) result_b = "0";
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
				if (!spawnedENTBodyguards.empty() && b_curr_num > -1 && b_curr_num < spawnedENTBodyguards.size()) return process_bod_prop_menu();
				else {
					if (spawnedENTBodyguards.empty()) {
						std::ostringstream ss;
						ss << "未找到任何保镖！";
						set_status_text(ss.str());
						return false;
					}
					if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
						std::ostringstream ss;
						ss << "编号错误！";
						set_status_text(ss.str());
						return false;
					}
				}
			}
			return false;
		}
		case 8:
		{
			if (!WEAPON::IS_PED_ARMED(PLAYER::PLAYER_PED_ID(), 7)) CONTROLS::_SET_CONTROL_NORMAL(0, 37, 1);
			std::string result_b = "";

			if (spawnedENTBodyguards.size() > 1) {
				keyboard_on_screen_already = true;
				curr_message = "输入你想要，修改武器的保镖编号!（该编号显示在其头顶上）"; // 修改保镖的武器
				result_b = show_keyboard("手动输入名称", NULL);
			}
			if (spawnedENTBodyguards.size() == 1) result_b = "0";
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
				if (!spawnedENTBodyguards.empty() && b_curr_num > -1 && b_curr_num < spawnedENTBodyguards.size()) {
					equip_ped = spawnedENTBodyguards[b_curr_num];
					if (!WEAPON::IS_PED_ARMED(equip_ped, 7)) CONTROLS::_SET_CONTROL_NORMAL(0, 37, 1);
					return process_bod_individual_weapon_menu();
				}
				else {
					if (spawnedENTBodyguards.empty()) {
						std::ostringstream ss;
						ss << "未找到任何保镖！";
						set_status_text(ss.str());
						return false;
					}
					if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
						std::ostringstream ss;
						ss << "编号错误！";
						set_status_text(ss.str());
						return false;
					}
				}
			}
			return false;
		}
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
			value = SKINS_ONLINE_CAPTIONS[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 3:
			value = SKINS_ANIMALS_CAPTIONS[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 4:
			// For custom peds, always show the title if available, fallback to model name
			if (!lastCustomBodyguardPedName.empty()) {
				value = lastCustomBodyguardPedName;
			} else if (!lastCustomBodyguardSpawn.empty()) {
				value = lastCustomBodyguardSpawn;
			} else {
				value = "未选择";
			}
			break;
		case 5:
			// Manual input mode
			if (!lastCustomBodyguardSpawn.empty()) {
				value = lastCustomBodyguardSpawn;
			} else {
				value = "未输入";
			}
			break;
		default:
			value = SKINS_GENERAL_CAPTIONS[0];
			break;
		}
	}
	else {
		// For manual input or when lastCustomBodyguardSpawn is set
		if (skinTypesBodyguardMenuLastConfirmed[0] == 4 && !lastCustomBodyguardPedName.empty()) {
			// Custom ped mode - show title
			value = lastCustomBodyguardPedName;
		} else {
			// Manual input or other modes - show model name/input
			value = lastCustomBodyguardSpawn;
		}
	}
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
			value = SKINS_ONLINE_VALUES[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 3:
			value = SKINS_ANIMALS_VALUES[skinTypesBodyguardMenuLastConfirmed[1]];
			break;
		case 4:
			value = lastCustomBodyguardSpawn;
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
	lastCustomBodyguardSpawn = "";  // Clear manual input when switching categories
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching categories
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_npcs(MenuItem<std::string> choice){
	skinTypesBodyguardMenuPositionMemory[0] = 1;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 1;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;
	lastCustomBodyguardSpawn = "";  // Clear manual input when switching categories
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching categories
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_online(MenuItem<std::string> choice) {
	skinTypesBodyguardMenuPositionMemory[0] = 2;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 2;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;
	lastCustomBodyguardSpawn = "";  // Clear manual input when switching categories
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching categories
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool onconfirm_bodyguards_skins_animals(MenuItem<std::string> choice){ 
	skinTypesBodyguardMenuPositionMemory[0] = 3;
	skinTypesBodyguardMenuPositionMemory[1] = choice.currentMenuIndex;
	skinTypesBodyguardMenuLastConfirmed[0] = 3;
	skinTypesBodyguardMenuLastConfirmed[1] = choice.currentMenuIndex;
	lastCustomBodyguardSpawn = "";  // Clear manual input when switching categories
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching categories
	requireRefreshOfBodyguardMainMenu = true;

	return true;
}

bool process_player_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	lastCustomBodyguardSpawn = "";
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching to other categories
	for(int i = 0; i < SKINS_PLAYER_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_PLAYER_CAPTIONS[i];
		item->value = SKINS_PLAYER_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "主角模型", onconfirm_bodyguards_skins_players, NULL, NULL);
}

bool process_npc_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	lastCustomBodyguardSpawn = "";
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching to other categories
	for(int i = 0; i < SKINS_GENERAL_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_GENERAL_CAPTIONS[i];
		item->value = SKINS_GENERAL_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "普通 NPC 模型", onconfirm_bodyguards_skins_npcs, NULL, NULL);
}

bool process_online_skins_menu() {
	std::vector<MenuItem<std::string>*> menuItems;
	lastCustomBodyguardSpawn = "";
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching to other categories
	for (int i = 0; i < SKINS_ONLINE_CAPTIONS.size(); i++) {
		MenuItem<std::string>* item = new MenuItem<std::string>();
		item->caption = SKINS_ONLINE_CAPTIONS[i];
		item->value = SKINS_ONLINE_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "在线 NPC 模型", onconfirm_bodyguards_skins_online, NULL, NULL);
}

bool process_animal_skins_menu(){
	std::vector<MenuItem<std::string>*> menuItems;
	lastCustomBodyguardSpawn = "";
	lastCustomBodyguardPedName = "";  // Clear custom ped name when switching to other categories
	for (int i = 0; i < SKINS_ANIMALS_CAPTIONS.size(); i++){
		MenuItem<std::string> *item = new MenuItem<std::string>();
		item->caption = SKINS_ANIMALS_CAPTIONS[i];
		item->value = SKINS_ANIMALS_VALUES[i];
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	return draw_generic_menu<std::string>(menuItems, &skinTypesBodyguardMenuPositionMemory[1], "动物模型", onconfirm_bodyguards_skins_animals, NULL, NULL);
}

bool onconfirm_bodyguard_weapons_category_menu(MenuItem<int> choice){
	int category = choice.sortval;
	
	switch(choice.value){
		case -1:
			for(int a = 0; a < bodyguardWeaponsToggle[category].size(); a++){
				*bodyguardWeaponsToggle[category].at(a) = !*bodyguardWeaponsToggle[category].at(a);
			}
			set_status_text(std::string("所有保镖已切换 [") + MENU_WEAPON_CATEGORIES.at(category) + std::string("] 类型的武器"));
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
	under_weapon_menu = true;

	item = new MenuItem<int>();
	item->caption = "此类别所有武器: 装备/卸载";
	item->value = index++;
	item->isLeaf = true;
	item->sortval = category;
	menuItems.push_back(item);

	for(auto a : VOV_WEAPON_CAPTIONS[category]){
		toggleItem = new ToggleMenuItem<int>();
		if (UI::DOES_TEXT_LABEL_EXIST((char*)a.c_str())) toggleItem->caption = UI::_GET_LABEL_TEXT((char*)a.c_str());
		else toggleItem->caption = a;
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
		set_status_text("所有保镖已启用武器！");
	}
	else if(choice.value == cs + 1){
		for(int a = 0; a < cs; a++){
			for(int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++){
				*bodyguardWeaponsToggle[a].at(b) = false;
			}
		}
		set_status_text("所有保镖已禁用武器！");
	}

	return false;
}

bool process_bodyguard_weapons_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int index = 0;
	under_weapon_menu = true;

	for(auto a : MENU_WEAPON_CATEGORIES){
		item = new MenuItem<int>();
		item->caption = a;
		item->value = index++;
		item->isLeaf = false;
		menuItems.push_back(item);
	}

	item = new MenuItem<int>();
	item->caption = "禁用所有的武器";
	item->value = index + 1;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	item = new MenuItem<int>();
	item->caption = "启用所有的武器";
	item->value = index;
	item->isLeaf = true;
	menuItems.insert(menuItems.begin(), item);

	return draw_generic_menu<int>(menuItems, nullptr, "选择保镖武器", onconfirm_bodyguard_weapons_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_bodyguard_blips_menu(MenuItem<int> choice)
{
	return false;
}

void process_bodyguard_blips_menu(){
	const std::string caption = "保镖位置标记";

	std::vector<MenuItem<int>*> menuItems;
	SelectFromListMenuItem *listItem;
	ToggleMenuItem<int>* toggleItem;

	int i = 0;

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "启用";
	toggleItem->value = i++;
	toggleItem->toggleValue = &featureBodyguardOnMap;
	menuItems.push_back(toggleItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPSIZE_CAPTIONS, onchange_body_blipsize_index);
	listItem->wrap = false;
	listItem->caption = "标记大小";
	listItem->value = BodyBlipSizeIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPCOLOUR_CAPTIONS, onchange_body_blipcolour_index);
	listItem->wrap = false;
	listItem->caption = "标记颜色";
	listItem->value = BodyBlipColourIndex;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(VEH_BLIPSYMBOL_CAPTIONS, onchange_body_blipsymbol_index);
	listItem->wrap = false;
	listItem->caption = "标记样式";
	listItem->value = BodyBlipSymbolIndexN;
	menuItems.push_back(listItem);

	listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_body_blipflash_index);
	listItem->wrap = false;
	listItem->caption = "标记闪烁 ";
	listItem->value = BodyBlipFlashIndex;
	menuItems.push_back(listItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "标记显示编号";
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
	c_armed = true;
	stop_b = false;

	if(spawnedENTBodyguards.size() == 0){
		set_status_text("你还没有任何的保镖！");
		spawnedENTBodyguards.clear();
		spawnedENTBodyguards.shrink_to_fit();
		return;
	}

	for(int i = 0; i < spawnedENTBodyguards.size(); i++){
		if (PED::IS_PED_IN_ANY_VEHICLE(spawnedENTBodyguards[i], 0)) AI::TASK_SMART_FLEE_PED(spawnedENTBodyguards[i], PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
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
	spawnedBodyguardsSecWeap.clear();
	spawnedBodyguardsSecWeap.shrink_to_fit();
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

	set_status_text("您的所有保镖已解散！");
}

void do_spawn_bodyguard(){
	requireRefreshOfBodyguardMainMenu = true;
	bool bodyguard_animal = false;
	bodyGuard = -1;
	bool exist_already = false;
	DWORD bodyGuardModel = -1;
	float random_category, random_bodyguard = -1;

	if (hotkey_boddyguard == true && hotkey_b == false) {
		hotkey_b = true;
		process_bodyguard_menu();
		WAIT(10);
		process_main_menu();
	}

	// 随机保镖生成 - 添加中文支持
	if ((lastCustomBodyguardSpawn == "random" || lastCustomBodyguardSpawn == "Random" || lastCustomBodyguardSpawn == "RANDOM" ||
		lastCustomBodyguardSpawn == "随机" || lastCustomBodyguardSpawn == "SJ" || lastCustomBodyguardSpawn == "sj") && added_nearest_b == false) {
		random_category = (rand() % 10 + 0); // 上边距 + 下边距
		if (random_category == 0) {
			random_bodyguard = (rand() % SKINS_PLAYER_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_PLAYER_VALUES[random_bodyguard].c_str());
		}
		if (random_category > 0 && random_category < 8) {
			random_bodyguard = (rand() % SKINS_GENERAL_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_GENERAL_VALUES[random_bodyguard].c_str());
		}
		if (random_category > 7 && random_category < 10) {
			random_bodyguard = (rand() % SKINS_ONLINE_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_ONLINE_VALUES[random_bodyguard].c_str());
		}
		if (random_category == 10) {
			random_bodyguard = (rand() % SKINS_ANIMALS_VALUES.size() + 0);
			bodyGuardModel = GAMEPLAY::GET_HASH_KEY((char*)SKINS_ANIMALS_VALUES[random_bodyguard].c_str());
		}
		// 添加随机保镖生成成功提示
		set_status_text("随机保镖或路人生成完成！");
	} // 随机保镖生成结束
	
	  // 修改判断条件，增加中文支持
	if (lastCustomBodyguardSpawn != "random" && lastCustomBodyguardSpawn != "Random" && lastCustomBodyguardSpawn != "RANDOM" && 
		lastCustomBodyguardSpawn != "saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_Bodyguards" &&
		lastCustomBodyguardSpawn != "random_story" && lastCustomBodyguardSpawn != "Random_story" && lastCustomBodyguardSpawn != "Random_Story" && 
		// 添加中文支持
		lastCustomBodyguardSpawn != "随机" && lastCustomBodyguardSpawn != "SJ" && lastCustomBodyguardSpawn != "sj" &&
		lastCustomBodyguardSpawn != "随机多个" && lastCustomBodyguardSpawn != "SJDG" && lastCustomBodyguardSpawn != "sjdg" &&
		lastCustomBodyguardSpawn != "已保存的" && lastCustomBodyguardSpawn != "YBCD" && lastCustomBodyguardSpawn != "ybcd" &&
		added_nearest_b == false) 
	{
		bodyGuardModel = get_current_model_hash(); // hotkey_boddyguard == false && 
	}

	if (load_saved_bodyguard == true && added_nearest_b == false) {
		bodyGuardModel = temp_bodyguard;
		// 添加已保存保镖加载成功提示
		if (lastCustomBodyguardSpawn == "saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_Bodyguards" ||
			lastCustomBodyguardSpawn == "已保存的" || lastCustomBodyguardSpawn == "YBCD" || lastCustomBodyguardSpawn == "ybcd") {
			set_status_text("已保存的保镖加载完成！");
		}
	}

	if (spawning_a_ped == false && spawnedENTBodyguards.size() >= BODYGUARD_LIMIT) {
		set_status_text("无法再生成更多保镖了！");
		return;
	}

	if (added_nearest_b == true) bodyGuardModel = GAMEPLAY::GET_HASH_KEY("player_one");

	if((STREAMING::IS_MODEL_IN_CDIMAGE(bodyGuardModel) && STREAMING::IS_MODEL_VALID(bodyGuardModel)) || added_nearest_b == true){
		STREAMING::REQUEST_MODEL(bodyGuardModel);
		while(!STREAMING::HAS_MODEL_LOADED(bodyGuardModel)){
			make_periodic_feature_call();
			WAIT(0);
		}

		myENTGroup = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID());

		Vector3 spawnCoords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 2.0, 2.0, 0.0); // 2.5 2.5
		Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		
		// 将最近的NPC添加为保镖
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
		} // 最近的保镖逻辑结束
		
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

			if (spawning_a_ped == false) {
				spawnedENTBodyguards.push_back(bodyGuard); // 保存当前保镖

				PED::SET_PED_AS_GROUP_LEADER(PLAYER::PLAYER_PED_ID(), myENTGroup);
				PED::SET_PED_AS_GROUP_MEMBER(bodyGuard, myENTGroup);
				PED::SET_PED_NEVER_LEAVES_GROUP(bodyGuard, true);
				PED::SET_PED_DIES_IN_WATER(bodyGuard, false);
				PED::SET_PED_DIES_INSTANTLY_IN_WATER(bodyGuard, false);
				PED::SET_ENABLE_SCUBA(bodyGuard, true);
			}

			if (featureBodyguardHelmet && bodyguard_animal == false) {
				PED::GIVE_PED_HELMET(bodyGuard, 1, 4096, -1);
			}

			if (featureBodyguardOnMap && spawning_a_ped == false) {
				add_body_blip();
			}
			if (!featureBodyguardOnMap) {
				if (!BLIPTABLE_BODYGUARD.empty()) {
					for (int i = 0; i < BLIPTABLE_BODYGUARD.size(); i++) {
						if (UI::DOES_BLIP_EXIST(BLIPTABLE_BODYGUARD[i])) {
							UI::REMOVE_BLIP(&BLIPTABLE_BODYGUARD[i]);
						}
					}
					BLIPTABLE_BODYGUARD.clear();
					BLIPTABLE_BODYGUARD.shrink_to_fit();
				}
			}
			
			if (spawning_a_ped == false) {
				PED::SET_PED_COMBAT_ABILITY(bodyGuard, 2);
				PED::SET_PED_COMBAT_RANGE(bodyGuard, 2);
				PED::SET_PED_COMBAT_MOVEMENT(bodyGuard, 3);
				//PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 0, true); // BF_CanUseCover 
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 1, true); // BF_CanUseVehicles 
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 2, true); // BF_CanDoDrivebys 
				//PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 3, true); // BF_CanLeaveVehicle 
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 5, true); // BF_CanFightArmedPedsWhenNotArmed
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 20, true); // BF_CanTauntInVehicle 
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 46, true); // BF_AlwaysFight 
				//PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 1424, true); // BF_PlayerCanUseFiringWeapons - crashes when equipping a weapon with a bodyguard nearby
				//PED::SET_PED_ALERTNESS(bodyGuard, 3);
				//PED::SET_PED_SEEING_RANGE(bodyGuard, 1000);
				
				PED::SET_PED_SHOOT_RATE(bodyGuard, 999);
				PED::SET_COMBAT_FLOAT(bodyGuard, 0, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 1, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 3, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 4, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 5, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 8, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 11, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 12, 1.0);
				PED::SET_COMBAT_FLOAT(bodyGuard, 16, 1.0);

				// 动物
				if (bodyguard_animal == true) {
					PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 46, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 5, true);
					PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 17, true);
					PED::SET_PED_FLEE_ATTRIBUTES(bodyGuard, 0, false);
					PED::SET_PED_CAN_BE_TARGETTED(bodyGuard, true);
				} // 动物逻辑结束

				if (bodyguard_animal == false) {
					PED::SET_PED_CAN_SWITCH_WEAPON(bodyGuard, true);
					PED::SET_GROUP_FORMATION(myENTGroup, BODY_GROUPFORMATION_VALUES[BodyGroupFormationIndex]); // 1 
					PED::SET_GROUP_FORMATION_SPACING(myENTGroup, VEH_BLIPSIZE_VALUES[BodyDistanceIndex], VEH_BLIPSIZE_VALUES[BodyDistanceIndex], VEH_BLIPSIZE_VALUES[BodyDistanceIndex]); // 2.0, 2.0, 2.0
				}
				PED::SET_CAN_ATTACK_FRIENDLY(bodyGuard, false, false);
			}

			//AI::TASK_COMBAT_HATED_TARGETS_AROUND_PED(bodyGuard, 100.0f, 0);
			PED::SET_PED_KEEP_TASK(bodyGuard, true);

			if (bodyguard_animal == false) PED::SET_PED_FIRING_PATTERN(bodyGuard, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_FULL_AUTO")); // 0xC6EE6B4C

			// 不同的武器
			if (featureDifferentWeapons && PED_WEAPON_TITLES[BodyWeaponSetIndex] == "自定义武器") {
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
				if (bodyguard_animal == false) {
					WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, WEAPONS[pop], 1000, false, true);
					spawnedBodyguardsSecWeap.push_back(WEAPONS[pop]);
				}
				if (added_nearest_b == true && !WEAPON::IS_PED_ARMED(bodyGuard, 7)) {
					WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, WEAPONS[pop], 999, false, true);
					WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, WEAPONS[pop], 1);
					WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
					spawnedBodyguardsSecWeap.push_back(WEAPONS[pop]);
				}
				if (pop == all_selected) pop = -1;
			}
			if (featureDifferentWeapons && PED_WEAPON_TITLES[BodyWeaponSetIndex] != "自定义武器") {
				std::vector<std::string> weaponBSet = VOV_PED_WEAPONS[BodyWeaponSetIndex];
				int index = rand() % weaponBSet.size();
				std::string weaponB = weaponBSet.at(index);
				Hash weapBHash = GAMEPLAY::GET_HASH_KEY((char *)weaponB.c_str());

				if (bodyguard_animal == false) {
					WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, weapBHash, 1000, false, true);
					spawnedBodyguardsSecWeap.push_back(weapBHash);
				}
				if (added_nearest_b == true && !WEAPON::IS_PED_ARMED(bodyGuard, 7)) {
					WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, weapBHash, 999, false, true);
					WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, weapBHash, 1);
					WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
					spawnedBodyguardsSecWeap.push_back(weapBHash);
				}
			} // 不同武器逻辑结束
			
			if (spawning_a_ped == false) {
				if (!featureDifferentWeapons && load_saved_bodyguard == false) {
					if (bodyguard_animal == false) {
						for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
							for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
								if (*bodyguardWeaponsToggle[a].at(b)) {
									tmp_w = GAMEPLAY::GET_HASH_KEY((char *)VOV_WEAPON_VALUES[a].at(b).c_str());
									if (!WEAPON::HAS_PED_GOT_WEAPON(bodyGuard, tmp_w, false)) {
										WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, tmp_w, 1000, false, true);
									}
								}
							}
						}
						spawnedBodyguardsSecWeap.push_back(WEAPON::GET_BEST_PED_WEAPON(bodyGuard, 0));
					}
					if (added_nearest_b == true && !WEAPON::IS_PED_ARMED(bodyGuard, 7)) {
						for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
							for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
								tmp_w = GAMEPLAY::GET_HASH_KEY((char *)VOV_WEAPON_VALUES[a].at(b).c_str());
								WEAPON::GIVE_WEAPON_TO_PED(bodyGuard, tmp_w, 999, false, true);
								WEAPON::SET_CURRENT_PED_WEAPON(bodyGuard, tmp_w, 1);
								WEAPON::SET_PED_CURRENT_WEAPON_VISIBLE(bodyGuard, true, false, 1, 1);
							}
						}
						spawnedBodyguardsSecWeap.push_back(WEAPON::GET_BEST_PED_WEAPON(bodyGuard, 0));
					}
				}
			}

			if (featureBodyguardInfAmmo && bodyguard_animal == false) WEAPON::SET_PED_INFINITE_AMMO_CLIP(bodyGuard, true);

			if (featureBodyguardWeaponAttach) add_all_weapons_attachments(bodyGuard);

			if (added_nearest_b == false) PED::SET_PED_DEFAULT_COMPONENT_VARIATION(bodyGuard);
			WAIT(0);

			// 随机化外观
			if (featureRandomApp) {
				PED::CLEAR_ALL_PED_PROPS(bodyGuard);
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(bodyGuard, true);
				PED::SET_PED_RANDOM_PROPS(bodyGuard);
				WAIT(0);
			} // 随机化外观逻辑结束

			// 保镖生命值
			if (PLAYER_HEALTH_VALUES[BodyHealthIndex] > 0) {
				PED::SET_PED_MAX_HEALTH(bodyGuard, PLAYER_HEALTH_VALUES[BodyHealthIndex]);
				ENTITY::SET_ENTITY_HEALTH(bodyGuard, PLAYER_HEALTH_VALUES[BodyHealthIndex]);
			} // 保镖生命值逻辑结束

			// 生成敌对NPC
			if (featureBAggressivePed && spawning_a_ped == true) {
				PED::SET_PED_AS_ENEMY(PLAYER::PLAYER_PED_ID(), true);
				PED::REGISTER_TARGET(bodyGuard, PLAYER::PLAYER_PED_ID());
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 5, true);
				PED::SET_PED_COMBAT_ATTRIBUTES(bodyGuard, 46, true);
				AI::TASK_COMBAT_PED(bodyGuard, PLAYER::PLAYER_PED_ID(), 0, 16);
			}

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
	spawning_a_ped = false;
	return;
}

void do_add_near_bodyguard() {
	added_nearest_b = true;
	do_spawn_bodyguard();
	return;
}

void add_body_blip() {
	if (!BLIPTABLE_BODYGUARD.empty()) {
		for (int j = 0; j < BLIPTABLE_BODYGUARD.size(); j++) UI::REMOVE_BLIP(&BLIPTABLE_BODYGUARD[j]);
	}
	BLIPTABLE_BODYGUARD.clear();
	BLIPTABLE_BODYGUARD.shrink_to_fit();
	for (int j = 0; j < spawnedENTBodyguards.size(); j++) {
		blip_body[0] = UI::ADD_BLIP_FOR_ENTITY(spawnedENTBodyguards[j]);
		UI::SET_BLIP_AS_FRIENDLY(blip_body[0], true);
		if (VEH_BLIPSYMBOL_VALUES[BodyBlipSymbolIndexN] != NULL) UI::SET_BLIP_SPRITE(blip_body[0], VEH_BLIPSYMBOL_VALUES[BodyBlipSymbolIndexN]);
		else UI::SET_BLIP_SPRITE(blip_body[0], VEH_BLIPSYMBOL_VALUES[0]);
		UI::SET_BLIP_CATEGORY(blip_body[0], 2);
		if (featureBodyBlipNumber) UI::SHOW_NUMBER_ON_BLIP(blip_body[0], BLIPTABLE_BODYGUARD.size());
		if (NPC_RAGDOLL_VALUES[BodyBlipFlashIndex] == 1) UI::SET_BLIP_FLASHES(blip_body[0], true);
		if (NPC_RAGDOLL_VALUES[BodyBlipFlashIndex] == 2) UI::SET_BLIP_FLASHES_ALTERNATE(blip_body[0], true);
		UI::SET_BLIP_SCALE(blip_body[0], VEH_BLIPSIZE_VALUES[BodyBlipSizeIndex]);
		UI::SET_BLIP_COLOUR(blip_body[0], VEH_BLIPCOLOUR_VALUES[BodyBlipColourIndex]);
		BLIPTABLE_BODYGUARD.push_back(blip_body[0]);
	}
}

bool not_bodyguards_in_vehicle()
{
	for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
		if (!PED::IS_PED_IN_ANY_VEHICLE(spawnedENTBodyguards[n], false) && PED::GET_PED_TYPE(spawnedENTBodyguards[n]) != 28) return true; // !PED::IS_PED_SITTING_IN_ANY_VEHICLE(spawnedENTBodyguards[n])
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
	
	// 武器选择
	if (under_weapon_menu == true && (IsKeyDown(KeyConfig::KEY_MENU_SELECT) || CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, controller_binds["KEY_MENU_SELECT"].first) || IsKeyDown(KeyConfig::KEY_MENU_BACK) || IsKeyDown(KeyConfig::KEY_TOGGLE_MAIN_MENU))) {
		selBodyWeapons = "";
		for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
			for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
				if (*bodyguardWeaponsToggle[a].at(b) == true) {
					selBodyWeapons = selBodyWeapons + std::to_string(a) + "," + std::to_string(b) + " ";
				};
			}
		}
	}

	// '跟随载具' 消息
	if (b_follow_m == -2) b_follow_m = FollowInVehicleIndex;
	if (FollowInVehicleIndex == 0 && b_follow_m != 0) b_follow_m = FollowInVehicleIndex;
	if (NPC_RAGDOLL_VALUES[FollowInVehicleIndex] > 0) {
		if (b_follow_m != FollowInVehicleIndex) {
			if (FollowInVehicleIndex == 1) set_status_text("鲁莽驾驶");
			if (FollowInVehicleIndex == 2) set_status_text("谨慎驾驶");
			b_follow_m = FollowInVehicleIndex;
		}
	}

	if (!spawnedENTBodyguards.empty()) {
		// 保镖在任务中无需攻击同伴
		if (GAMEPLAY::GET_MISSION_FLAG() == 1) {
			groupID = PLAYER::GET_PLAYER_GROUP(PLAYER::PLAYER_PED_ID());
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, groupID, myENTGroup);
			PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, myENTGroup, groupID);

			const int arrSize_coop = 1024;
			Ped surr_coop[arrSize_coop];
			int count_coop = worldGetAllPeds(surr_coop, arrSize_coop);
			for (int k = 0; k < count_coop; k++) {
				if ((PED::GET_PED_TYPE(surr_coop[k]) == 0 || PED::GET_PED_TYPE(surr_coop[k]) == 1 || PED::GET_PED_TYPE(surr_coop[k]) == 2 || PED::GET_PED_TYPE(surr_coop[k]) == 3 ||
					ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_stretch") || ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_lamardavis") ||
					ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("u_m_m_edtoh") || ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_chef2") ||
					ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_talina") || ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_wade") ||
					ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_brad") || ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("ig_davenorton") ||
					ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("hc_driver") || ENTITY::GET_ENTITY_MODEL(surr_coop[k]) == GAMEPLAY::GET_HASH_KEY("hc_gunman")) &&
					!PED::IS_PED_GROUP_MEMBER(surr_coop[k], groupID) && !PED::IS_PED_IN_ANY_VEHICLE(surr_coop[k], 0)) {
					PED::SET_PED_AS_GROUP_MEMBER(surr_coop[k], groupID);
					ENTITY::SET_ENTITY_INVINCIBLE(surr_coop[k], false);
				}
			}
		}
		
		Vector3 my_coords = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		for (int i = 0; i < spawnedENTBodyguards.size(); i++) {
			// 保镖无敌
			if (featureBodyguardInvincible) ENTITY::SET_ENTITY_INVINCIBLE(spawnedENTBodyguards[i], true);
			else ENTITY::SET_ENTITY_INVINCIBLE(spawnedENTBodyguards[i], false);
			// 无法被爆头
			if (featureBCannotBeHeadshot) PED::SET_PED_SUFFERS_CRITICAL_HITS(spawnedENTBodyguards[i], false); // 无爆头
			else PED::SET_PED_SUFFERS_CRITICAL_HITS(spawnedENTBodyguards[i], true);
			// 无血迹且无弹孔
			if (featureNoBodBlood) PED::CLEAR_PED_BLOOD_DAMAGE(spawnedENTBodyguards[i]);
			// 与保镖共享武器
			if (featureBodyguardYourWeapon && WEAPON::GET_SELECTED_PED_WEAPON(spawnedENTBodyguards[i]) != WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID())) {
				if (WEAPON::IS_PED_ARMED(PLAYER::PLAYER_PED_ID(), 7) && !spawnedENTBodyguards.empty()) WEAPON::REMOVE_ALL_PED_WEAPONS(spawnedENTBodyguards[i], false);
				if (!spawnedENTBodyguards.empty()) WEAPON::GIVE_WEAPON_TO_PED(spawnedENTBodyguards[i], WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), 999, false, true);
				if (featureBodyguardInfAmmo) WEAPON::SET_PED_INFINITE_AMMO_CLIP(spawnedENTBodyguards[i], true);
				for (int a = 0; a < WEAPONTYPES_MOD.size(); a++) {
					for (int b = 0; b < VOV_WEAPONMOD_VALUES[a].size(); b++) {
						char* weaponName = (char*)WEAPONTYPES_MOD.at(a).c_str(), * compName = (char*)VOV_WEAPONMOD_VALUES[a].at(b).c_str();
						Hash weaponHash = GAMEPLAY::GET_HASH_KEY(weaponName), compHash = GAMEPLAY::GET_HASH_KEY(compName);
						if (weaponHash == WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID())) {
							if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(PLAYER::PLAYER_PED_ID(), weaponHash, compHash)) {
								WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(spawnedENTBodyguards[i], WEAPON::GET_SELECTED_PED_WEAPON(spawnedENTBodyguards[i]), compHash);
							}
							WEAPON::SET_PED_WEAPON_TINT_INDEX(spawnedENTBodyguards[i], WEAPON::GET_SELECTED_PED_WEAPON(spawnedENTBodyguards[i]), WEAPON::GET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), weaponHash));
						}
					}
				}
				if (featureBodyguardWeaponAttach && !spawnedENTBodyguards.empty()) add_all_weapons_attachments(spawnedENTBodyguards[i]);
			}
			// 添加/移除武器
			if (!spawnedBodyguardsSecWeap.empty() && !spawnedENTBodyguards.empty()) {
				if (featureAddRemoveWeapon) {
					if (c_armed == true) {
						if (PED::IS_PED_IN_ANY_VEHICLE(spawnedENTBodyguards[i], 1) && WEAPON::GET_SELECTED_PED_WEAPON(spawnedENTBodyguards[i]) != GAMEPLAY::GET_HASH_KEY("WEAPON_MICROSMG")) {
							WEAPON::REMOVE_ALL_PED_WEAPONS(spawnedENTBodyguards[i], false);
							WEAPON::GIVE_WEAPON_TO_PED(spawnedENTBodyguards[i], GAMEPLAY::GET_HASH_KEY("WEAPON_MICROSMG"), 999, false, true);
						}
						if (!featureBodyguardYourWeapon && !PED::IS_PED_IN_ANY_VEHICLE(spawnedENTBodyguards[i], 1) && WEAPON::GET_SELECTED_PED_WEAPON(spawnedENTBodyguards[i]) != spawnedBodyguardsSecWeap[i]) {
							WEAPON::REMOVE_ALL_PED_WEAPONS(spawnedENTBodyguards[i], false);
							WEAPON::GIVE_WEAPON_TO_PED(spawnedENTBodyguards[i], spawnedBodyguardsSecWeap[i], 999, false, true);
							if (featureBodyguardWeaponAttach) add_all_weapons_attachments(spawnedENTBodyguards[i]);
						}
					}
					if (c_armed == false) WEAPON::REMOVE_ALL_PED_WEAPONS(spawnedENTBodyguards[i], false);
				}
			}
			// 保镖的游泳能力
			if (ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 1 && !is_in_airbrake_mode() && PED::GET_PED_TYPE(spawnedENTBodyguards[i]) != 28 && stop_b == false) {
				float height = -1.0;
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
			} //
			// 动物
			if (animal_in_group == true) {
				Vector3 cop_coords;
				const int arrSize_animals = 1024;
				Ped surr_animals[arrSize_animals];
				int count_animals = worldGetAllPeds(surr_animals, arrSize_animals);
				for (int k = 0; k < count_animals; k++) {
					PED::SET_PED_CAN_RAGDOLL(spawnedENTBodyguards[i], 0);
					if (dist_diff != -1) PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[i], true);
					else PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[i], false);
					if (surr_animals[k] != PLAYER::PLAYER_PED_ID() && surr_animals[k] != spawnedENTBodyguards[i] && !PED::IS_PED_GROUP_MEMBER(surr_animals[k], myENTGroup) && PED::IS_PED_IN_MELEE_COMBAT(surr_animals[k])) {
						if (PED::GET_PED_TYPE(spawnedENTBodyguards[i]) == 28) {
							PED::SET_PED_AS_ENEMY(surr_animals[k], true);
							AI::TASK_COMBAT_PED_TIMED(spawnedENTBodyguards[i], surr_animals[k], 50000, 16); // 50000
						}
					}
					if (surr_animals[k] != PLAYER::PLAYER_PED_ID() && surr_animals[k] != spawnedENTBodyguards[i] && !PED::IS_PED_GROUP_MEMBER(surr_animals[k], myENTGroup) && PED::IS_PED_SHOOTING(surr_animals[k])) {
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
								AI::TASK_COMBAT_PED_TIMED(spawnedENTBodyguards[i], cop_to_kill, 50000, 16); 
								AI::TASK_WRITHE(cop_to_kill, spawnedENTBodyguards[i], 50000, 0);
							}
							if (ENTITY::IS_ENTITY_DEAD(cop_to_kill) || ENTITY::IS_ENTITY_DEAD(spawnedENTBodyguards[i]) || !ENTITY::DOES_ENTITY_EXIST(cop_to_kill)) dist_diff = -1;
						} // 结束是否是动物
					}
				}
				if (PED::IS_PED_FLEEING(spawnedENTBodyguards[i])) AI::TASK_STAND_STILL(spawnedENTBodyguards[i], 10000);
			}
			// 在头顶显示数字
			if ((NPC_RAGDOLL_VALUES[BodyShowNumbersIndex] == 0 && menu_showing == true) || NPC_RAGDOLL_VALUES[BodyShowNumbersIndex] == 1/* && GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() != 0*/) {
				Vector3 head_c = PED::GET_PED_BONE_COORDS(spawnedENTBodyguards[i], 31086, 0, 0, 0);
				std::string curr_i = std::to_string(i);
				GRAPHICS::SET_DRAW_ORIGIN(head_c.x, head_c.y, head_c.z + 0.5, 0);
				UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
				UI::_ADD_TEXT_COMPONENT_SCALEFORM((char *)curr_i.c_str());
				text_parameters(0.5, 0.5, 255, 242, 0, 255);
				UI::END_TEXT_COMMAND_DISPLAY_TEXT(0, 0);
				GRAPHICS::CLEAR_DRAW_ORIGIN();
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
				spawnedBodyguardsSecWeap.erase(spawnedBodyguardsSecWeap.begin() + i);
				add_body_blip();
				requireRefreshOfBodyguardMainMenu = true;
			}
		} // 结束 for 循环 (int i = 0; i < spawnedENTBodyguards.size(); i++)
		
		// 在车辆中跟随
		if (NPC_RAGDOLL_VALUES[FollowInVehicleIndex] > 0) {
			Vector3 coordsme = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
			Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
			if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID())) {
				Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh);
				int maxSeats = VEHICLE::GET_VEHICLE_MODEL_NUMBER_OF_SEATS(currVehModel);

				if (VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(veh) == (maxSeats - 1) && bod_pass == false) {
					if (not_bodyguards_in_vehicle() && someone_shooting > 200) { // 100
						const int arrSize33 = 1024;
						Ped surr_vehs[arrSize33];
						int count_surr_vehs = worldGetAllVehicles(surr_vehs, arrSize33);
						float dist_diff = -1.0;
						float temp_dist = 2000.0;
						for (int t = 0; t < count_surr_vehs; t++) {
							if (ENTITY::DOES_ENTITY_EXIST(surr_vehs[t]) && VEHICLE::ARE_ANY_VEHICLE_SEATS_FREE(surr_vehs[t])) {
								Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(surr_vehs[t], true);
								dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
								if (temp_dist > dist_diff) {
									temp_dist = dist_diff;
									temp_vehicle = surr_vehs[t];
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
					if (bod_pass == true && someone_shooting > 200) {
						for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
							if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(spawnedENTBodyguards[n])) {
								for (int m = 0; m < B_VEHICLE.size(); m++) {
									bool added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 2)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 2);
										AI::TASK_LEAVE_VEHICLE(temp_bodyguard_d, B_VEHICLE[m], 16);
										WAIT(0);
										if (!PED::IS_PED_FLEEING(temp_bodyguard_d) && !AI::IS_PED_RUNNING(temp_bodyguard_d)) AI::TASK_SMART_FLEE_PED(temp_bodyguard_d, PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
										PED::SET_PED_INTO_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 2);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 1)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 1);
										AI::TASK_LEAVE_VEHICLE(temp_bodyguard_d, B_VEHICLE[m], 16);
										WAIT(0);
										if (!PED::IS_PED_FLEEING(temp_bodyguard_d) && !AI::IS_PED_RUNNING(temp_bodyguard_d)) AI::TASK_SMART_FLEE_PED(temp_bodyguard_d, PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
										PED::SET_PED_INTO_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 1);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 0)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 0);
										AI::TASK_LEAVE_VEHICLE(temp_bodyguard_d, B_VEHICLE[m], 16);
										WAIT(0);
										if (!PED::IS_PED_FLEEING(temp_bodyguard_d) && !AI::IS_PED_RUNNING(temp_bodyguard_d)) AI::TASK_SMART_FLEE_PED(temp_bodyguard_d, PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
										PED::SET_PED_INTO_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], 0);
									}
									added_already = false;
									for (int tmp = 0; tmp < spawnedENTBodyguards.size(); tmp++) {
										if (spawnedENTBodyguards[tmp] == VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1)) added_already = true;
									}
									if (added_already == false) {
										Ped temp_bodyguard_d = VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1);
										AI::TASK_LEAVE_VEHICLE(temp_bodyguard_d, B_VEHICLE[m], 16);
										WAIT(0);
										if (!PED::IS_PED_FLEEING(temp_bodyguard_d) && !AI::IS_PED_RUNNING(temp_bodyguard_d)) AI::TASK_SMART_FLEE_PED(temp_bodyguard_d, PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
										PED::SET_PED_INTO_VEHICLE(spawnedENTBodyguards[n], B_VEHICLE[m], -1);
									}
								}
							}
						}
					}
					if (not_bodyguards_in_vehicle() && someone_shooting > 200) {
						for (int m = 0; m < B_VEHICLE.size(); m++) {
							if (ENTITY::DOES_ENTITY_EXIST(B_VEHICLE[m]) && !VEHICLE::ARE_ANY_VEHICLE_SEATS_FREE(B_VEHICLE[m]) && bod_pass == true) {
								bod_pass = false;
								me_to_follow = false;
							}
						}
					}
					if (me_to_follow == false) {
						for (int m = 0; m < B_VEHICLE.size(); m++) {
							VEHICLE::SET_VEHICLE_ENGINE_ON(B_VEHICLE[m], true, true, false);
							AI::SET_DRIVE_TASK_CRUISE_SPEED(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 300.0);
							if (NPC_RAGDOLL_VALUES[FollowInVehicleIndex] == 1) AI::TASK_VEHICLE_CHASE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), PLAYER::PLAYER_PED_ID());
							if (NPC_RAGDOLL_VALUES[FollowInVehicleIndex] == 2) AI::TASK_VEHICLE_ESCORT(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), B_VEHICLE[m], veh, -1, 140.0f, 32, 10, 1, 1); // 786468 32
							AI::SET_TASK_VEHICLE_CHASE_IDEAL_PURSUIT_DISTANCE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 60.0f);
							AI::SET_TASK_VEHICLE_CHASE_BEHAVIOR_FLAG(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 32, true); // 786603
							PED::SET_DRIVER_AGGRESSIVENESS(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 0.1f);
							PED::SET_DRIVER_ABILITY(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1), 0.9f);
						}
						me_to_follow = true;
					}
				}
			} // 结束车辆内
			if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID())) {
				bod_pass = false;
				me_to_follow = false;
				if (!B_VEHICLE.empty()) {
					for (int g = 0; g < B_VEHICLE.size(); g++) {
						if (ENTITY::DOES_ENTITY_EXIST(B_VEHICLE[g]) && VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[g], -1) != 0) {
							if (!VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(B_VEHICLE[g], false)))) AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[g], -1), B_VEHICLE[g], 1);
							else AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[g], -1), B_VEHICLE[g], 16);
						}
						ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&B_VEHICLE[g]);
					}
					B_VEHICLE.clear();
					B_VEHICLE.shrink_to_fit();
				}
			}
			if (PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID())) {
				someone_shooting = someone_shooting + 1;
				if (PED::IS_ANY_PED_SHOOTING_IN_AREA(coordsme.x - 200, coordsme.y - 200, coordsme.z - 200, coordsme.x + 200, coordsme.y + 200, coordsme.z + 200, true, true)) someone_shooting = 0;
			}
			for (int n = 0; n < spawnedENTBodyguards.size(); n++) {
				Vector3 coordsped = ENTITY::GET_ENTITY_COORDS(spawnedENTBodyguards[n], true);
				float height_b_g = -1;
				GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coordsped.x, coordsped.y, coordsped.z, &height_b_g);
				float height_a_g = -1;
				GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coordsme.x, coordsme.y, coordsme.z, &height_a_g);
				dist_diff = SYSTEM::VDIST(coordsme.x, coordsme.y, coordsme.z, coordsped.x, coordsped.y, coordsped.z);
				if ((VEHICLE::IS_THIS_MODEL_A_CAR(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_BIKE(ENTITY::GET_ENTITY_MODEL(veh)) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(ENTITY::GET_ENTITY_MODEL(veh)))) {
					if (dist_diff > 350) { // 450 250
						AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[n], true);
						Vector3 closestRoad;
						if (PATHFIND::GET_CLOSEST_ROAD(coordsme.x - 70, coordsme.y - 70, coordsme.z, 1.f, 1, &closestRoad, &closestRoad, 0, 0, 0, 0))
						{
							ENTITY::SET_ENTITY_COORDS(PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), closestRoad.x, closestRoad.y, closestRoad.z, 0, 0, 0, 1);
							me_to_follow = false;
						}
					}
				}
				if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(veh)) && VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false), -1) == spawnedENTBodyguards[n] &&
					PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false) != PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)) {
					if (coordsme.z - height_a_g < 2 && coordsped.z - height_b_g > 7 && coordsped.z - height_b_g < 15)
						ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false), 1, 0, 0, -0.1, 0, 0, 0, true, false, true, true, true, true);
					if (coordsme.z - height_a_g < 2 && coordsped.z - height_b_g < 8 && VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false))))
						VEHICLE::SET_VEHICLE_ENGINE_ON(PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false), false, false, false);
				}
				if (VEHICLE::IS_THIS_MODEL_A_PLANE(ENTITY::GET_ENTITY_MODEL(veh)) && VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false), -1) == spawnedENTBodyguards[n] &&
					PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false) != PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false)) {
					float planecurrspeed = ENTITY::GET_ENTITY_SPEED(PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]));
					float curr_roll = ENTITY::GET_ENTITY_ROLL(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
					float curr_pitch = ENTITY::GET_ENTITY_PITCH(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
					float curr_yaw = ENTITY::GET_ENTITY_HEADING(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()));
					if (coordsme.z - height_a_g > 5) {
						if (planecurrspeed > 10 && coordsped.z - height_b_g < 10) {
							ENTITY::APPLY_FORCE_TO_ENTITY(PED::GET_VEHICLE_PED_IS_IN(spawnedENTBodyguards[n], false), 1, 0, 0, 0.3, 0, 0, 0, true, false, true, true, true, true);
							ENTITY::SET_ENTITY_ROTATION(PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), curr_pitch + 0.2, curr_roll, curr_yaw, 1, true);
						}
						if (coordsped.z - height_b_g > 9) {
							AI::TASK_PLANE_MISSION(spawnedENTBodyguards[n], PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), 0, 0, coordsme.x, coordsme.y, coordsme.z, 4, 2000, 0, 90, 2600, 300);
							ENTITY::SET_ENTITY_ROTATION(PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), curr_pitch, curr_roll, curr_yaw, 1, true);
						}
					}
					if (coordsme.z - height_a_g < 6/* && coordsped.z - height_b_g < 10*/) {
						if (ENTITY::HAS_ENTITY_COLLIDED_WITH_ANYTHING(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()))) {
							AI::TASK_PLANE_MISSION(spawnedENTBodyguards[n], PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), 0, 0, coordsme.x, coordsme.y, coordsme.z, 4, 20, 0, 90, 2600, 300);
							AI::TASK_PLANE_LAND(spawnedENTBodyguards[n], PED::GET_VEHICLE_PED_IS_USING(spawnedENTBodyguards[n]), coordsme.x, coordsme.y, coordsme.z, coordsme.x + 5, coordsme.y + 5, coordsme.z);
						}
					}
				}
				if (PED::IS_PED_SHOOTING(spawnedENTBodyguards[n])) me_to_follow = false;
				if (!PED::IS_PED_SITTING_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_SITTING_IN_ANY_VEHICLE(spawnedENTBodyguards[n]) && dist_diff > 50) { //  && AI::IS_PED_STILL(spawnedENTBodyguards[n])
					AI::TASK_SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[n], true);
					Vector3 closestRoad;
					if (ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 0) {
						PATHFIND::GET_CLOSEST_ROAD(coordsme.x - 30, coordsme.y - 30, coordsme.z, 1.f, 1, &closestRoad, &closestRoad, 0, 0, 0, 0);
						ENTITY::SET_ENTITY_COORDS(spawnedENTBodyguards[n], closestRoad.x, closestRoad.y, closestRoad.z, 1, 0, 0, 1);
					}
					if (ENTITY::IS_ENTITY_IN_WATER(PLAYER::PLAYER_PED_ID()) == 1) ENTITY::SET_ENTITY_COORDS(spawnedENTBodyguards[n], coordsme.x - 30, coordsme.y - 30, coordsme.z, 1, 0, 0, 1);
				}
			}
			if (!B_VEHICLE.empty() && someone_shooting > 200) {
				for (int m = 0; m < B_VEHICLE.size(); m++) {
					if (VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], -1) == 0) {
						AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 0), B_VEHICLE[m], 16);
						AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 1), B_VEHICLE[m], 16);
						AI::TASK_LEAVE_VEHICLE(VEHICLE::GET_PED_IN_VEHICLE_SEAT(B_VEHICLE[m], 2), B_VEHICLE[m], 16);
						bod_pass = false;
						me_to_follow = false;
					}
				}
			}
			if (not_bodyguards_in_vehicle() && someone_shooting > 200) {
				bod_pass = false;
				me_to_follow = false;
			}
		} // 结束跟随车辆
	} // 结束 if (!spawnedENTBodyguards.empty())
} // 结束 void maintain_bodyguards()

bool process_bodyguard_menu(){
	do{
		requireRefreshOfBodyguardMainMenu = false;
		
		const std::string caption = "保镖选项";

		std::vector<MenuItem<int>*> menuItems;
		MenuItem<int> *item;
		ToggleMenuItem<int>* toggleItem;
		SelectFromListMenuItem *listItem;

		int i = 0;

		item = new MenuItem<int>();
		std::ostringstream ss0;
		ss0 << "生成保镖: " << get_current_model_name(); 
		item->caption = ss0.str();
		item->value = 0;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "添加最近的路人为保镖";
		item->value = 1;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "解散所有的保镖";
		item->value = 2;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "自定义解散保镖";
		item->value = 3;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "切换保镖跟随玩家的方式";
		item->value = 4;
		item->isLeaf = true;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		std::ostringstream ss3;
		ss3 << "生成为路人: " << get_current_model_name();
		item->caption = ss3.str();
		item->value = 5;
		item->isLeaf = true;
		menuItems.push_back(item);
		
		item = new MenuItem<int>();
		item->caption = "保存的保镖";
		item->value = 6;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "更改保镖模型";
		item->value = 7;
		item->isLeaf = false;
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->caption = "选择保镖武器";
		item->value = 8;
		item->isLeaf = false;
		menuItems.push_back(item);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "保镖不同的武器";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureDifferentWeapons;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		listItem = new SelectFromListMenuItem(PED_WEAPON_TITLES, onchange_bodyguards_body_weapons);
		listItem->wrap = false;
		listItem->caption = "保镖携带的武器";
		listItem->value = BodyWeaponSetIndex;
		menuItems.push_back(listItem);

		item = new MenuItem<int>();
		item->caption = "保镖位置标记";
		item->value = 11;
		item->isLeaf = false;
		menuItems.push_back(item);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "保镖无敌";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInvincible;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "无限弹药";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardInfAmmo;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "生成时佩戴头盔";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardHelmet;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "保镖死亡解散时消失";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardDespawn;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		listItem = new SelectFromListMenuItem(VEH_BLIPSIZE_CAPTIONS, onchange_body_distance_index);
		listItem->wrap = false;
		listItem->caption = "保镖生成距离";
		listItem->value = BodyDistanceIndex;
		menuItems.push_back(listItem);

		listItem = new SelectFromListMenuItem(BODY_GROUPFORMATION_CAPTIONS, onchange_body_groupformation_index);
		listItem->wrap = false;
		listItem->caption = "保镖排列的队形";
		listItem->value = BodyGroupFormationIndex;
		menuItems.push_back(listItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "保镖随机外观";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureRandomApp;
		toggleItem->toggleValueUpdated = NULL;
		menuItems.push_back(toggleItem);

		listItem = new SelectFromListMenuItem(LIMP_IF_INJURED_CAPTIONS, onchange_follow_invehicle_index);
		listItem->wrap = false;
		listItem->caption = "跟随驾乘车辆";
		listItem->value = FollowInVehicleIndex;
		menuItems.push_back(listItem);

		listItem = new SelectFromListMenuItem(PLAYER_HEALTH_CAPTIONS, onchange_body_health_index);
		listItem->wrap = false;
		listItem->caption = "保镖的生命值";
		listItem->value = BodyHealthIndex;
		menuItems.push_back(listItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "启用添加/移除武器选项";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureAddRemoveWeapon;
		menuItems.push_back(toggleItem);

		item = new MenuItem<int>();
		item->caption = "添加/移除武器";
		item->value = 22;
		item->isLeaf = true;
		menuItems.push_back(item);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "与保镖共享/共用武器";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardYourWeapon;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "添加所有的武器配件";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBodyguardWeaponAttach;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "没有血迹和弹孔";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureNoBodBlood;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "生成攻击性的行人";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBAggressivePed;
		menuItems.push_back(toggleItem);

		toggleItem = new ToggleMenuItem<int>();
		toggleItem->caption = "无法被爆头";
		toggleItem->value = i++;
		toggleItem->toggleValue = &featureBCannotBeHeadshot;
		menuItems.push_back(toggleItem);

		listItem = new SelectFromListMenuItem(BODY_SHOWNUMBERS_CAPTIONS, onchange_body_shownumber_index);
		listItem->wrap = false;
		listItem->caption = "显示保镖的编号";
		listItem->value = BodyShowNumbersIndex;
		menuItems.push_back(listItem);

		if(!bodyguardWeaponsToggleInitialized){
			for(int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++){
				for(int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++){
					bodyguardWeaponsToggle[a].push_back(new bool(true));
				}
			}

			if (selBodyWeapons != "") {
				std::string tmp_a_row, tmp_b_row;
				int found_separator = 0;

				for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
					for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
						*bodyguardWeaponsToggle[a].at(b) = false;
					}
				}

				for (int k = 0; k < selBodyWeapons.size(); k++) {
					if (found_separator == 0 && selBodyWeapons[k] != *"," && selBodyWeapons[k] != *" ") {
						tmp_a_row = tmp_a_row + selBodyWeapons[k];
					}
					if (found_separator == 0 && selBodyWeapons[k] == *",") {
						found_separator = 1;
					}
					if (found_separator == 1 && selBodyWeapons[k] != *" " && selBodyWeapons[k] != *",") {
						tmp_b_row = tmp_b_row + selBodyWeapons[k];
					}
					if (found_separator == 1 && (selBodyWeapons[k] == *" " || (k == (selBodyWeapons.size() - 1)))) {
						std::string::size_type sz;
						for (int a = 0; a < MENU_WEAPON_CATEGORIES.size(); a++) {
							for (int b = 0; b < VOV_WEAPON_VALUES[a].size(); b++) {
								if (a == std::stoi(tmp_a_row, &sz) && b == std::stoi(tmp_b_row, &sz)) {
									*bodyguardWeaponsToggle[a].at(b) = true;
								};
							}
						}

						found_separator = 0;
						tmp_a_row = "";
						tmp_b_row = "";
					}
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
	under_weapon_menu = false;

	switch (activeLineIndexBodyguards) {
	case 0:
		// 添加中文支持逻辑
		// 检查是否不是已保存的保镖或随机多个保镖
		if (lastCustomBodyguardSpawn != "saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_bodyguards" && lastCustomBodyguardSpawn != "Saved_Bodyguards" && 
			lastCustomBodyguardSpawn != "random_story" && lastCustomBodyguardSpawn != "Random_story" && lastCustomBodyguardSpawn != "Random_Story" &&
			lastCustomBodyguardSpawn != "已保存的" && lastCustomBodyguardSpawn != "YBCD" && lastCustomBodyguardSpawn != "ybcd" &&
			lastCustomBodyguardSpawn != "随机多个" && lastCustomBodyguardSpawn != "SJDG" && lastCustomBodyguardSpawn != "sjdg") 
		{
			do_spawn_bodyguard();
		}

		// 处理已保存的保镖逻辑
		if (lastCustomBodyguardSpawn == "saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_bodyguards" || lastCustomBodyguardSpawn == "Saved_Bodyguards" ||
			lastCustomBodyguardSpawn == "已保存的" || lastCustomBodyguardSpawn == "YBCD" || lastCustomBodyguardSpawn == "ybcd") 
		{
			ENTDatabase* database = get_database();
			std::vector<SavedBodSkinDBRow*> savedBodSkins = database->get_saved_bod_skins();
			for each (SavedBodSkinDBRow * sv in savedBodSkins) {
				spawn_saved_bod_skin(sv->rowID, "");
			}
			// 添加生成完成提示
			set_status_text("已保存的保镖生成完成！");
		}

		// 处理随机多个保镖逻辑
		if (lastCustomBodyguardSpawn == "random_story" || lastCustomBodyguardSpawn == "Random_story" || lastCustomBodyguardSpawn == "Random_Story" ||
			lastCustomBodyguardSpawn == "随机多个" || lastCustomBodyguardSpawn == "SJDG" || lastCustomBodyguardSpawn == "sjdg") 
		{
			std::string lastCustomBodyguardSpawn_tmp = lastCustomBodyguardSpawn;
			for (int i = 0; i < 7; i++) {// 默认循环7次（可能是保镖数量上限或某种操作次数）
				int random_story_bodyguard = (rand() % 24 + 0);
				if (random_story_bodyguard == 0) lastCustomBodyguardSpawn = "player_zero";
				if (random_story_bodyguard == 1) lastCustomBodyguardSpawn = "player_one";
				if (random_story_bodyguard == 2) lastCustomBodyguardSpawn = "player_two";
				if (random_story_bodyguard == 3) lastCustomBodyguardSpawn = "ig_amandatownley";
				if (random_story_bodyguard == 4) lastCustomBodyguardSpawn = "ig_davenorton";
				if (random_story_bodyguard == 5) lastCustomBodyguardSpawn = "ig_devin";
				if (random_story_bodyguard == 6) lastCustomBodyguardSpawn = "ig_jimmydisanto";
				if (random_story_bodyguard == 7) lastCustomBodyguardSpawn = "ig_lamardavis";
				if (random_story_bodyguard == 8) lastCustomBodyguardSpawn = "ig_lestercrest";
				if (random_story_bodyguard == 9) lastCustomBodyguardSpawn = "ig_nervousron";
				if (random_story_bodyguard == 10) lastCustomBodyguardSpawn = "ig_stevehains";
				if (random_story_bodyguard == 11) lastCustomBodyguardSpawn = "ig_stretch";
				if (random_story_bodyguard == 12) lastCustomBodyguardSpawn = "ig_tracydisanto";
				if (random_story_bodyguard == 13) lastCustomBodyguardSpawn = "ig_wade";
				if (random_story_bodyguard == 14) lastCustomBodyguardSpawn = "ig_chengsr";
				if (random_story_bodyguard == 15) lastCustomBodyguardSpawn = "ig_andreas";
				if (random_story_bodyguard == 16) lastCustomBodyguardSpawn = "ig_brad";
				if (random_story_bodyguard == 17) lastCustomBodyguardSpawn = "ig_drfriedlander";
				if (random_story_bodyguard == 18) lastCustomBodyguardSpawn = "ig_floyd";
				if (random_story_bodyguard == 19) lastCustomBodyguardSpawn = "cs_martinmadrazo";
				if (random_story_bodyguard == 20) lastCustomBodyguardSpawn = "ig_molly";
				if (random_story_bodyguard == 21) lastCustomBodyguardSpawn = "ig_patricia";
				if (random_story_bodyguard == 22) lastCustomBodyguardSpawn = "ig_siemonyetarian";
				if (random_story_bodyguard == 23) lastCustomBodyguardSpawn = "ig_solomon";
				if (random_story_bodyguard == 24) lastCustomBodyguardSpawn = "ig_taocheng";
				do_spawn_bodyguard();
			}
			lastCustomBodyguardSpawn = lastCustomBodyguardSpawn_tmp;
			// 添加随机多个保镖生成完成提示
			set_status_text("随机多个保镖生成完成！");
		}
			break;
		case 1:
			do_add_near_bodyguard();
			break;
		case 2:
			dismiss_bodyguards();
			break;
		case 3:
		{
			keyboard_on_screen_already = true;
			curr_message = "输入你想要解散的, 保镖编号!  (显示在保镖头顶上的数字) ";
			std::string result_bod = show_keyboard("手动输入名称", NULL);
			if (!result_bod.empty())
			{
				result_bod = trim(result_bod);
				std::string::size_type sz;
				for (int i = 0; i < 7; i++) {
					sprintf(temp_n, "%i", i);
					if (result_bod == temp_n) is_it_n = true;
				}
				if (is_it_n == true) {
					b_curr_num = std::stof(result_bod, &sz);
					is_it_n = false;
				}
				if (!spawnedENTBodyguards.empty() && b_curr_num > -1 && b_curr_num < spawnedENTBodyguards.size()) {
					if (PED::IS_PED_IN_ANY_VEHICLE(spawnedENTBodyguards[b_curr_num], 0)) AI::TASK_SMART_FLEE_PED(spawnedENTBodyguards[b_curr_num], PLAYER::PLAYER_PED_ID(), 1000, -1, true, true);
					ENTITY::SET_ENTITY_INVINCIBLE(spawnedENTBodyguards[b_curr_num], false);
					PED::SET_PED_NEVER_LEAVES_GROUP(spawnedENTBodyguards[b_curr_num], false);
					PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(spawnedENTBodyguards[b_curr_num], false);
					PED::REMOVE_PED_FROM_GROUP(spawnedENTBodyguards[b_curr_num]);
					if (featureBodyguardDespawn) {
						AI::CLEAR_PED_TASKS(spawnedENTBodyguards[b_curr_num]);
						ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&spawnedENTBodyguards[b_curr_num]);
					}
					spawnedENTBodyguards.erase(spawnedENTBodyguards.begin() + b_curr_num);
					if (featureBodyguardOnMap && !BLIPTABLE_BODYGUARD.empty()) {
						add_body_blip();
					}
					return false;
				}
				else {
					if (spawnedENTBodyguards.empty()) {
						std::ostringstream ss;
						ss << "未找到任何保镖！";
						set_status_text(ss.str());
						return false;
					}
					if (b_curr_num < 0 || b_curr_num >= spawnedENTBodyguards.size()) {
						std::ostringstream ss;
						ss << "编号错误！";
						set_status_text(ss.str());
						return false;
					}
				}
			}
			return false;
			//中断;
		}
		case 4:
			stop_b = !stop_b;
			if (stop_b) set_status_text("原地待命！");
			else set_status_text("跟随玩家！");
			break;
		case 5:
			spawning_a_ped = true;
			do_spawn_bodyguard();
			break;
		case 6:
			process_bod_savedskin_menu();
			break;
		case 7:
			process_bodyguard_skins_menu();
			break;
		case 8:
			process_bodyguard_weapons_menu();
			break;
		case 11:
			process_bodyguard_blips_menu();
			break;
		case 22:
			if (featureAddRemoveWeapon) {
				c_armed = !c_armed;
				if (c_armed) set_status_text("保镖武器已添加！");
				else set_status_text("保镖武器已移除！");
			}
			break;
		default:
			break;
	}
	return false;
}

void add_bodyguards_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results){
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardInvincible", &featureBodyguardInvincible});
	results->push_back(FeatureEnabledLocalDefinition{"featureNoBodBlood", &featureNoBodBlood});
	results->push_back(FeatureEnabledLocalDefinition{"featureBAggressivePed", &featureBAggressivePed});
	results->push_back(FeatureEnabledLocalDefinition{"featureBCannotBeHeadshot", &featureBCannotBeHeadshot});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardHelmet", &featureBodyguardHelmet});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardDespawn", &featureBodyguardDespawn});
	results->push_back(FeatureEnabledLocalDefinition{"featureDifferentWeapons", &featureDifferentWeapons});
	results->push_back(FeatureEnabledLocalDefinition{"featureRandomApp", &featureRandomApp});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardYourWeapon", &featureBodyguardYourWeapon});
	results->push_back(FeatureEnabledLocalDefinition{"featureAddRemoveWeapon", &featureAddRemoveWeapon});
	results->push_back(FeatureEnabledLocalDefinition{"featureBodyguardWeaponAttach", &featureBodyguardWeaponAttach});
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
	results->push_back(StringPairSettingDBRow{ "BodyBlipSymbolIndexN", std::to_string(BodyBlipSymbolIndexN) });
	results->push_back(StringPairSettingDBRow{ "BodyBlipFlashIndex", std::to_string(BodyBlipFlashIndex) });
	results->push_back(StringPairSettingDBRow{ "FollowInVehicleIndex", std::to_string(FollowInVehicleIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyWeaponSetIndex", std::to_string(BodyWeaponSetIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyHealthIndex", std::to_string(BodyHealthIndex) });
	results->push_back(StringPairSettingDBRow{ "BodyShowNumbersIndex", std::to_string(BodyShowNumbersIndex) });
}

void add_bodyguards_generic_settings(std::vector<StringPairSettingDBRow>* results){
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory0", std::to_string(skinTypesBodyguardMenuPositionMemory[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuPositionMemory1", std::to_string(skinTypesBodyguardMenuPositionMemory[1])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed0", std::to_string(skinTypesBodyguardMenuLastConfirmed[0])});
	results->push_back(StringPairSettingDBRow{"skinTypesBodyguardMenuLastConfirmed1", std::to_string(skinTypesBodyguardMenuLastConfirmed[1])});
	results->push_back(StringPairSettingDBRow{"lastCustomBodyguardSpawn", lastCustomBodyguardSpawn});
	results->push_back(StringPairSettingDBRow{"lastCustomBodyguardPedName", lastCustomBodyguardPedName});
	results->push_back(StringPairSettingDBRow{"selBodyWeapons", selBodyWeapons});
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
		else if (setting.name.compare("BodyBlipSymbolIndexN") == 0 && VEH_BLIPSYMBOL_VALUES.size() > 0 && BodyBlipSymbolIndexN < VEH_BLIPSYMBOL_VALUES.size()){
			BodyBlipSymbolIndexN = stoi(setting.value);
		}
		else if (VEH_BLIPSYMBOL_VALUES.size() > 0 && BodyBlipSymbolIndexN >= VEH_BLIPSYMBOL_VALUES.size()) { // setting.name.compare("BodyBlipSymbolIndex") == 0 && 
			BodyBlipSymbolIndexN = 0;
		}
		else if (setting.name.compare("BodyBlipFlashIndex") == 0){
			BodyBlipFlashIndex = stoi(setting.value);
		}
		else if (setting.name.compare("FollowInVehicleIndex") == 0) {
			FollowInVehicleIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyWeaponSetIndex") == 0) {
			BodyWeaponSetIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyHealthIndex") == 0) {
			BodyHealthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("BodyShowNumbersIndex") == 0) {
			BodyShowNumbersIndex = stoi(setting.value);
		}
		else if (setting.name.compare("lastCustomBodyguardSpawn") == 0) {
			lastCustomBodyguardSpawn = setting.value;
		}
		else if (setting.name.compare("lastCustomBodyguardPedName") == 0) {
			lastCustomBodyguardPedName = setting.value;
		}
		else if (setting.name.compare("selBodyWeapons") == 0) {
			selBodyWeapons = setting.value;
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
	featureNoBodBlood = false;
	featureBAggressivePed = false;
	featureBCannotBeHeadshot = false;
	featureBodyguardHelmet = false;
	featureBodyguardDespawn = true;
	featureBodyguardInfAmmo = false;
	featureDifferentWeapons = false;
	featureRandomApp = false;
	featureBodyguardYourWeapon = false;
	featureAddRemoveWeapon = false;
	featureBodyguardWeaponAttach = false;
	BodyBlipSizeIndex = 2;
	BodyDistanceIndex = 7;
	BodyGroupFormationIndex = 1;
	BodyBlipColourIndex = 0;
	BodyBlipSymbolIndexN = 0;
	BodyBlipFlashIndex = 0;
	FollowInVehicleIndex = 0;
	BodyWeaponSetIndex = 0;
	BodyHealthIndex = 6;
	BodyShowNumbersIndex = 0;
	skinTypesBodyguardMenuLastConfirmed[0] = 0;
	skinTypesBodyguardMenuLastConfirmed[1] = 0;
	lastCustomBodyguardPedName = "";
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

void onchange_bodyguards_body_weapons(int value, SelectFromListMenuItem* source) {
	BodyWeaponSetIndex = value;
}

void onchange_body_health_index(int value, SelectFromListMenuItem* source) {
	BodyHealthIndex = value;
	BodyHealthChanged = true;
}

void onchange_body_shownumber_index(int value, SelectFromListMenuItem* source) {
	BodyShowNumbersIndex = value;
	BodyShowNumbersChanged = true;
}

void onchange_body_blipcolour_index(int value, SelectFromListMenuItem* source){
	BodyBlipColourIndex = value;
	BodyBlipColour_Changed = true;
}

void onchange_body_blipsymbol_index(int value, SelectFromListMenuItem* source){
	BodyBlipSymbolIndexN = value;
	BodyBlipSymbol_Changed = true;
}

void onchange_body_blipflash_index(int value, SelectFromListMenuItem* source){
	BodyBlipFlashIndex = value;
	BodyBlipFlash_Changed = true;
}

// Custom peds functions for bodyguards
bool onconfirm_bodyguards_custom_peds_category(MenuItem<std::string> choice) {
	// Copy logic from skins.cpp for custom peds category selection
	auto customPeds = get_custom_peds_map();
	auto it = customPeds.find(choice.value);
	if (it == customPeds.end()) return false;
	
	std::vector<MenuItem<std::string>*> items;
	for (auto& pr : it->second) {
		MenuItem<std::string>* m = new MenuItem<std::string>();
		m->caption = pr.second;
		m->value = pr.first;
		m->isLeaf = true;
		items.push_back(m);
	}
	
	static int selectedPed = 0;
	auto onconfirm = [](MenuItem<std::string> choice) -> bool {
		// Set the custom bodyguard model
		lastCustomBodyguardSpawn = choice.value;
		lastCustomBodyguardPedName = choice.caption;
		skinTypesBodyguardMenuPositionMemory[0] = 4;
		skinTypesBodyguardMenuLastConfirmed[0] = 4;
		requireRefreshOfBodyguardMainMenu = true;
		return true;
	};
	
	return draw_generic_menu<std::string>(items, &selectedPed, choice.value, onconfirm, NULL, NULL);
}

bool process_custom_peds_bodyguard_menu() {
	// Copy logic from skins.cpp process_custom_peds_menu but adapt for bodyguards
	if (!ensure_custom_peds_loaded()) {
		set_status_text("自定义角色模型 XML 读取失败!");
		return false;
	}
	
	auto categories = get_custom_ped_categories();
	if (categories.empty()) {
		set_status_text("未找到角色模型分类，请检查 ent-Peds.xml");
		return false;
	}
	
	std::vector<MenuItem<std::string>*> items;
	for (size_t i = 0; i < categories.size(); ++i) {
		MenuItem<std::string>* m = new MenuItem<std::string>();
		m->caption = categories[i];
		m->value = categories[i];
		m->isLeaf = false;
		items.push_back(m);
	}
	
	static int selCat = 0;
	auto onconfirm = [](MenuItem<std::string> choice) -> bool {
		return onconfirm_bodyguards_custom_peds_category(choice);
	};
	
	return draw_generic_menu<std::string>(items, &selCat, "新增角色模型分类", onconfirm, NULL, NULL);
}