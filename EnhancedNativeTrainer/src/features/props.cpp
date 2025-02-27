/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "props.h"
#include "script.h"
#include "propplacement.h"
#include "..\datasets\data_props.h"
#include "..\ui_support\file_dialog.h"
#include "..\xml\xml_import_export.h"
#include <set>

const int PROP_LIMIT = 250;

int lastSelectedCategoryIndex = 0;
int lastSelectedPropIndex = 0;
int lastKnownSavedPropSetCount = 0;

bool requireRefreshOfPropInstanceMenu = false;
bool propInstanceMenuInterruptFlag = false;

bool requireRefreshOfPropsSlotMenu = false;
bool requireRefreshOfPropsSaveSlots = false;

static std::vector<SpawnedPropInstance> propsWeCreated;

static std::set<SaveFileDialogCallback*> activeSaveFileCallbacks;

static std::set<LoadFileDialogCallback*> activeLoadFileCallbacks;

std::string lastCustomPropSpawn;

int explosionID = 0;

SpawnedPropInstance lastHighlightedProp;

const std::vector<std::string> ALPHA_LABELS = { "正常", "90%", "80%", "70%", "60%", "50%", "40%", "30%", "20%", "10%" };
const int ALPHA_VALUES[] = { 255, 229, 204, 178, 153, 127, 102, 76, 51, 25 };

bool creationParam1 = true;
bool creationParam2 = true;
bool creationParam3 = true;

bool propCreationIsInvincible = false;
bool propCreationIsImmovable = true;
bool propCreationHasGravity = true;
int propCreationAlphaIndex = 0;

void reset_prop_globals()
{
	lastSelectedCategoryIndex = 0;
	propCreationIsInvincible = false;
	propCreationIsImmovable = true;
	propCreationHasGravity = true;
	propCreationAlphaIndex = 0;
	lastCustomPropSpawn = "";
}

float vectRads(float degs)
{
	float radialConv = degs*3.1415926536 / 180;
	return radialConv;
}

void manage_prop_set()
{
	write_text_to_log_file("manage_prop_set called");
	std::vector<SpawnedPropInstance>::iterator it;
	for (it = propsWeCreated.begin(); it != propsWeCreated.end();)
	{
		if (!ENTITY::DOES_ENTITY_EXIST((*it).instance))
		{
			write_text_to_log_file("manage_prop_set deleting a prop");
			if ((*it) == currentProp)
			{
				currentProp = SpawnedPropInstance();
			}
			it = propsWeCreated.erase(it);
		}
		else
		{
			++it;
		}
	}
}

/**
* 许多道具无法与 PLACE_OBJECT_ON_GROUND_PROPERLY 正确配合使用。
* 因此，我们使用已知能正常工作的道具来确定地面高度，
* 从而为其他道具确定位置——具体方式为生成一个临时道具后删除它。
*/
bool get_ground_height_at_position(Vector3 coords, float* result)
{
	return GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, coords.z, result) == 1;
}

void do_spawn_model_by_player(Hash propHash, char* model, std::string title, bool silent)
{
	float alpha = ALPHA_VALUES[propCreationAlphaIndex];

	STREAMING::REQUEST_MODEL(propHash);
	DWORD now = GetTickCount();
	while (!STREAMING::HAS_MODEL_LOADED(propHash) && GetTickCount() < now + 5000)
	{
		make_periodic_feature_call();
		WAIT(0);
	}

	if (!STREAMING::HAS_MODEL_LOADED(propHash))
	{
		std::ostringstream ss2;
		ss2 << "超时: " << model << " 哈希值 " << propHash;
		write_text_to_log_file(ss2.str());
		return;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	FLOAT spawnOffX = 0.0f;
	FLOAT spawnOffY = 3.5f;
	FLOAT spawnOffZ = 0.0f;

	Vector3 minDimens;
	Vector3 maxDimens;
	GAMEPLAY::GET_MODEL_DIMENSIONS(propHash, &minDimens, &maxDimens);
	spawnOffY = max(3.5f, 2.0f + 0.5f * (maxDimens.y - minDimens.y));
	spawnOffZ = 0.0f;

	Vector3 r_coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, spawnOffX, spawnOffY, spawnOffZ);
	SimpleVector3 coords = { r_coords.x, r_coords.y, r_coords.z };

	float objZBase = 0;
	bool translatable = get_ground_height_at_position(r_coords, &objZBase);
	if (translatable)
	{
		coords.z = objZBase;
		if (minDimens.z < 0)
		{
			coords.z -= minDimens.z;
		}
	}

	do_spawn_model(
		propHash,
		model,
		title,
		&coords,
		0.0f, 0.0f, 0.0f,
		propCreationIsInvincible,
		propCreationIsImmovable,
		propCreationHasGravity,
		alpha,
		silent);
}

void do_spawn_model(Hash propHash, char* model, std::string title, SimpleVector3* coords, float pitch, float roll, float heading,
	bool invincible, bool immovable, bool gravity, float alpha, bool silent)
{
	if (propsWeCreated.size() >= PROP_LIMIT)
	{
		std::ostringstream ss;
		ss << "物体最高上限 (" << PROP_LIMIT << ") 已达到！\n请先移除一些，再生成吧！";
		set_status_text(ss.str());
	}

	STREAMING::REQUEST_MODEL(propHash);

	DWORD now = GetTickCount();

	while (!STREAMING::HAS_MODEL_LOADED(propHash) && GetTickCount() < now + 5000 )
	{
		make_periodic_feature_call();
		WAIT(0);
	}

	if (!STREAMING::HAS_MODEL_LOADED(propHash))
	{
		std::ostringstream ss2;
		ss2 << "超时: " << model;
		write_text_to_log_file(ss2.str());
		return;
	}

	Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(propHash, coords->x, coords->y, coords->z, creationParam1, creationParam2, creationParam3);

	if (ENTITY::DOES_ENTITY_EXIST(obj))
	{
		ENTITY::SET_ENTITY_VELOCITY(obj, 0.0f, 0.0f, 0.0f);
		ENTITY::SET_ENTITY_ROTATION(obj, pitch, roll, heading, 0, false); 

		ENTITY::SET_ENTITY_COLLISION(obj, 1, 0);

		ENTITY::SET_ENTITY_HAS_GRAVITY(obj, gravity);

		ENTITY::FREEZE_ENTITY_POSITION(obj, immovable);

		if (!immovable)
		{
			// 这将解冻它
			ENTITY::APPLY_FORCE_TO_ENTITY(obj, 3, 0, 0, 0.1, 0, 0, 0, 0, 1, 1, 0, 0, 1);
			OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(obj, TRUE);
		}

		if (invincible)
		{
			//ENTITY::SET_ENTITY_INVINCIBLE(obj, TRUE);
			//ENTITY::SET_ENTITY_PROOFS(obj, 1, 1, 1, 1, 1, 1, 1, 1);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(obj, FALSE);
		}

		ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(obj, true);

		ENTITY::SET_ENTITY_ALPHA(obj, alpha, false);

		SpawnedPropInstance record;
		record.instance = obj;

		record.title = title;
		record.counter = find_highest_instance_num_of_prop(propHash) + 1;
		record.isInvincible = invincible;
		record.isImmovable = immovable;
		record.hasGravity = gravity;

		propsWeCreated.push_back(record);
	}
	else
	{
		if (!silent)
		{
			std::ostringstream ss;
			ss << "创建失败: " << title;
			set_status_text(ss.str());
		}

		std::ostringstream ss2;
		ss2 << "无效属性: " << model;
		write_text_to_log_file(ss2.str());
		return;
	}

	if (!silent)
	{
		std::ostringstream ss;
		ss << "已生成: " << title;
		set_status_text(ss.str());
	}

	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(propHash);
	//ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&obj);
}

void do_spawn_model_by_player(PropInfo prop, bool silent)
{
	Hash propHash = GAMEPLAY::GET_HASH_KEY((char *)prop.model);

	if (!STREAMING::IS_MODEL_IN_CDIMAGE(propHash) || !STREAMING::IS_MODEL_VALID(propHash))
	{
		if (!silent)
		{
			std::ostringstream ss;
			ss << "模型: " << prop.model << " 数据无效！";
			set_status_text(ss.str());
		}

		std::ostringstream ss2;
		ss2 << "无效属性: " << prop.model;
		write_text_to_log_file(ss2.str());
		return;
	}

	do_spawn_model_by_player(propHash, prop.model, prop.label, silent);
}

bool onconfirm_prop_selection(MenuItem<int> choice)
{
	std::string category = PROP_CATEGORIES[lastSelectedCategoryIndex];

	std::vector<PropInfo> filtered;
	for each (PropInfo prop in ALL_PROPS)
	{
		if (prop.category.compare(category) == 0)
		{
			filtered.push_back(prop);
		}
	}

	if (choice.value == -1) // 生成该分类下的所有对象
	{
		int i = 0;
		for each (PropInfo prop  in filtered)
		{
			std::ostringstream ss;
			ss << "已完成 " << i++ << " 项，\n总数 " << filtered.size() << " 项！";
			set_status_text_centre_screen(ss.str());
			WAIT(0);

			do_spawn_model_by_player(prop, true);
		}
		manage_prop_set();
		return false;
	}

	PropInfo prop = filtered.at(choice.value);
	do_spawn_model_by_player(prop, false);
	manage_prop_set();

	return false;
}

int propCategorySelection = 0;
int propSelection = 0;

void process_props_menu_incategory(int categoryIndex)
{
	std::string category = PROP_CATEGORIES[categoryIndex];

	std::vector<PropInfo> filtered;
	int count = 0;
	int total = 0;
	for each (PropInfo prop in ALL_PROPS)
	{
		total++;
		if (prop.category.compare(category) == 0)
		{
			count++;
			filtered.push_back(prop);
		}
	}

	/*
	std::ostringstream ssd;
	ssd << count << " of " << total << " and " << ALL_PROPS.size();
	set_status_text_centre_screen(ssd.str());
	*/

	std::vector<MenuItem<int>*> menuItems;

	/*
	MenuItem<int>* item = new MenuItem<int>();
	item->value = -1;
	item->caption = "Spawn All In Category";
	item->isLeaf = true;
	menuItems.push_back(item);
	*/

	int i = 0;
	for each (PropInfo prop in filtered)
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->value = i;
		item->caption = prop.label;
		item->isLeaf = true;
		menuItems.push_back(item);
		i++;
	}

	draw_generic_menu<int>(menuItems, &propSelection, category, onconfirm_prop_selection, NULL, NULL, NULL);
}

bool onconfirm_prop_category(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		keyboard_on_screen_already = true;
		curr_message = "请输入物体名称（例如：prop_fruit_basket）"; // 生成物体
		std::string result = show_keyboard("手动输入名称", (char*)lastCustomPropSpawn.c_str());
		if (!result.empty())
		{
			result = trim(result);
			lastCustomPropSpawn = result;
			Hash hash = GAMEPLAY::GET_HASH_KEY((char*)result.c_str());
			if (!STREAMING::IS_MODEL_IN_CDIMAGE(hash) || !STREAMING::IS_MODEL_VALID(hash))
			{
				std::ostringstream ss;
				ss << "找不到此模型: '" << result << "'";
				set_status_text(ss.str());
				return false;
			}
			else
			{
				do_spawn_model_by_player(GAMEPLAY::GET_HASH_KEY((char*)result.c_str()), (char*)result.c_str(), result, false);
			}
		}
		return false;
	}
	/*else if (choice.value == -2)
	{
		std::string result = show_keyboard(NULL, (char*)lastCustomPropSpawn.c_str());
		if (!result.empty())
		{
			result = trim(result);
			auto ipl = result.c_str();
			STREAMING::REQUEST_IPL(ipl);
			DWORD now = GetTickCount();
			while (!STREAMING::IS_IPL_ACTIVE(ipl) && GetTickCount() < now + 5000)
			{
				WAIT(0);
				make_periodic_feature_call();
			}
			if (!STREAMING::IS_IPL_ACTIVE(ipl))
			{
				set_status_text("Failed to load IPL");
			}
			else
			{
				set_status_text("Loaded IPL");
			}
		}
		return false;
	}*/

	if (choice.value != lastSelectedCategoryIndex)
	{
		propSelection = 0;
		lastSelectedCategoryIndex = choice.value;
	}
	process_props_menu_incategory(choice.value);
	return false;
}

void process_props_spawn_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int i = 0;
	for each (std::string category in PROP_CATEGORIES)
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->value = i;
		item->caption = category;
		item->isLeaf = false;
		menuItems.push_back(item);
		i++;
	}

	MenuItem<int>* item = new MenuItem<int>();
	item->value = -1;
	item->caption = "手动输入名称";
	item->isLeaf = true;
	menuItems.push_back(item);
	i++;

	/*
	item = new MenuItem<int>();
	item->value = -2;
	item->caption = "Request IPL Manually";
	item->isLeaf = false;
	menuItems.push_back(item);
	i++;
	*/

	/*
	ToggleMenuItem<int>* toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Param 1";
	toggleItem->toggleValue = &creationParam1;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Param 2";
	toggleItem->toggleValue = &creationParam2;
	menuItems.push_back(toggleItem);

	toggleItem = new ToggleMenuItem<int>();
	toggleItem->caption = "Spawn Param 3";
	toggleItem->toggleValue = &creationParam3;
	menuItems.push_back(toggleItem);
	*/

	draw_generic_menu<int>(menuItems, &propCategorySelection, "物体分类", onconfirm_prop_category, NULL, NULL, NULL);
}

void onchange_spawn_alpha(int value, SelectFromListMenuItem* source)
{
	//if we've been supplied with an extra, we're applying this to an instance
	if (source->extras.size() > 0)
	{
		SpawnedPropInstance prop = get_prop_at_index(source->extras.at(0));
		ENTITY::SET_ENTITY_ALPHA(prop.instance, ALPHA_VALUES[value], false);
	}
	//otherwise we're changing the new spawn option
	else
	{
		propCreationAlphaIndex = value;
	}
}

int prop_spawnopt_selection = 0;

bool prop_spawn_options_menu()
{
	/*
	bool propCreationIsInvincible = false;
	bool propCreationIsOnFire = false;
	bool propCreationIsImmovable = false;
	bool propCreationHasPhysics = true;
	bool propCreationHasGravity = true;
	bool propCreationHasGravity = true;
	*/

	std::vector<MenuItem<int>*> menuItems;

	int i = 0;

	ToggleMenuItem<int>* item = new ToggleMenuItem<int>();
	item->toggleValue = &propCreationIsInvincible;
	item->caption = "开启无敌";
	menuItems.push_back(item);
	i++;

	item = new ToggleMenuItem<int>();
	item->toggleValue = &propCreationIsImmovable;
	item->caption = "禁止移动";
	menuItems.push_back(item);
	i++;

	item = new ToggleMenuItem<int>();
	item->toggleValue = &propCreationHasGravity;
	item->caption = "有无重力";
	menuItems.push_back(item);
	i++;

	/*
	item = new ToggleMenuItem<int>();
	item->toggleValue = &propCreationIsOnFire;
	item->caption = "On Fire?";
	menuItems.push_back(item);
	i++;
	*/

	SelectFromListMenuItem* alphaItem = new SelectFromListMenuItem(ALPHA_LABELS, onchange_spawn_alpha);
	alphaItem->value = propCreationAlphaIndex;
	alphaItem->caption = "透明度 (不透明度)";
	alphaItem->wrap = false;
	menuItems.push_back(alphaItem);
	i++;

	draw_generic_menu<int>(menuItems, &prop_spawnopt_selection, "物体生成设置", NULL, NULL, NULL, NULL);
	return false;
}

bool onconfirm_prop_menu(MenuItem<int> choice)
{
	if (choice.value == 0)
	{
		process_props_spawn_menu();
	}
	else if (choice.value == 1) //remove all
	{
		int count = 0;
		for each (SpawnedPropInstance prop in propsWeCreated)
		{
			Object obj = prop.instance;
			if (ENTITY::DOES_ENTITY_EXIST(obj))
			{
				count++;
				OBJECT::DELETE_OBJECT(&obj);
			}
		}
		manage_prop_set();
		std::ostringstream ss;
		ss << count << " 个物体，已成功删除！";
		set_status_text(ss.str());
		return false;
	}
	else if (choice.value == 2)
	{
		prop_spawn_options_menu();
	}
	else if (choice.value == 3)
	{
		manage_prop_set();

		if (propsWeCreated.size() == 0)
		{
			set_status_text("您还没有生成任何物体！\n请您先创建一些物体吧！");
		}
		else
		{
			prop_spawned_instances_menu();
		}
	}
	else if (choice.value == 4)
	{
		process_savedprops_menu();
	}
	else if (choice.value == 5)
	{
		manage_prop_set();
		std::ostringstream ss;
		int size = propsWeCreated.size();
		ss << "当前已生成物体 " << size << " 个！\n最高物体上限 " << PROP_LIMIT << " 个。";
		set_status_text(ss.str());
	}
	return false;
}

int prop_menu_selection = 0;

void process_props_menu()
{
	std::vector<MenuItem<int>*> menuItems;

	int i = 0;

	MenuItem<int>* item = new MenuItem<int>();
	item->value = 0;
	item->caption = "生成物体";
	item->isLeaf = false;
	menuItems.push_back(item);
	i++;

	item = new MenuItem<int>();
	item->value = 4;
	item->caption = "保存的物体";
	item->isLeaf = false;
	menuItems.push_back(item);
	i++;

	item = new MenuItem<int>();
	item->value = 2;
	item->caption = "物体生成设置";
	item->isLeaf = false;
	menuItems.push_back(item);
	i++;

	item = new MenuItem<int>();
	item->value = 3;
	item->caption = "编辑已生成的物体";
	item->isLeaf = false;
	menuItems.push_back(item);
	i++;

	item = new MenuItem<int>();
	item->value = 1;
	item->caption = "删除所有生成的物体";
	item->isLeaf = true;
	menuItems.push_back(item);
	i++;

	item = new MenuItem<int>();
	item->value = 5;
	item->caption = "检查物体的数量上限";
	item->isLeaf = true;
	menuItems.push_back(item);
	i++;
	
	draw_generic_menu<int>(menuItems, &prop_menu_selection, "物体选项", onconfirm_prop_menu, NULL, NULL, NULL);
}

void add_props_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "propCreationHasGravity", &propCreationHasGravity });
	results->push_back(FeatureEnabledLocalDefinition{ "propCreationIsImmovable", &propCreationIsImmovable });
	results->push_back(FeatureEnabledLocalDefinition{ "propCreationIsInvincible", &propCreationIsInvincible });
}

void add_props_generic_settings(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "propCreationAlphaIndex", std::to_string(propCreationAlphaIndex)});
	results->push_back(StringPairSettingDBRow{ "lastCustomPropSpawn", lastCustomPropSpawn });
}

void handle_generic_settings_props(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("propCreationAlphaIndex") == 0)
		{
			propCreationAlphaIndex = stoi(setting.value);
		}
		else if (setting.name.compare("lastCustomPropSpawn") == 0)
		{
			lastCustomPropSpawn = setting.value;
		}
	}
}

void cleanup_props()
{
	write_text_to_log_file("cleanup_props called");
	for each (SpawnedPropInstance prop in propsWeCreated)
	{
		write_text_to_log_file("cleanup_props deleting a prop");
		if (ENTITY::DOES_ENTITY_EXIST(prop.instance))
		{
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&prop.instance);
		}
	}
	propsWeCreated.clear();
}

int find_highest_instance_num_of_prop(Hash model)
{
	int highestFound = 0;
	for each (SpawnedPropInstance prop in propsWeCreated)
	{
		Hash entryModel = ENTITY::GET_ENTITY_MODEL(prop.instance);
		if (model == entryModel && prop.counter > highestFound)
		{
			highestFound = prop.counter;
		}
	}
	return highestFound;
}

int menu_spawned_instance_index = 0;

bool prop_spawned_instances_menu()
{
	requireRefreshOfPropInstanceMenu = false;
	propInstanceMenuInterruptFlag = false;

	do
	{
		requireRefreshOfPropInstanceMenu = false;

		manage_prop_set();

		std::vector<MenuItem<int>*> menuItems;

		int i = 0;

		for each(SpawnedPropInstance prop in propsWeCreated)
		{
			MenuItem<int>* item = new MenuItem<int>();
			item->value = i;
			std::ostringstream ss;
			ss << prop.title << "~HUD_COLOUR_MENU_YELLOW~ #" << prop.counter;
			item->caption = ss.str();
			item->isLeaf = false;
			menuItems.push_back(item);
			i++;
		}

		set_menu_per_frame_call(flash_prop_callback);

		draw_generic_menu<int>(menuItems, &menu_spawned_instance_index, "已生成的物体", onconfirm_prop_instance_menu, onhighlight_prop_instance_menu, NULL, prop_instance_menu_interrupt);

		clear_menu_per_frame_call();
		if (!lastHighlightedProp.isEmpty() && ENTITY::DOES_ENTITY_EXIST(lastHighlightedProp.instance))
		{
			ENTITY::SET_ENTITY_VISIBLE(lastHighlightedProp.instance, TRUE);
		}

		WAIT(0);
	}
	while (requireRefreshOfPropInstanceMenu && propsWeCreated.size() > 0 );

	return false;
}

void onhighlight_prop_instance_menu(MenuItem<int> choice)
{
	if (!lastHighlightedProp.isEmpty() && ENTITY::DOES_ENTITY_EXIST(lastHighlightedProp.instance))
	{
		ENTITY::SET_ENTITY_VISIBLE(lastHighlightedProp.instance, TRUE);
	}

	SpawnedPropInstance prop = get_prop_at_index(choice.value);
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 J");
		return;
	}
	lastHighlightedProp = prop;
}


bool onconfirm_prop_instance_menu(MenuItem<int> choice)
{
	clear_menu_per_frame_call();
	if (!lastHighlightedProp.isEmpty() && ENTITY::DOES_ENTITY_EXIST(lastHighlightedProp.instance))
	{
		ENTITY::SET_ENTITY_VISIBLE(lastHighlightedProp.instance, TRUE);
	}
	prop_spawned_single_instance_menu(choice.value);
	set_menu_per_frame_call(flash_prop_callback);
	return false;
}

bool prop_instance_menu_interrupt()
{
	if (propInstanceMenuInterruptFlag)
	{
		propInstanceMenuInterruptFlag = false;
		return true;
	}
	return false;
}

SpawnedPropInstance get_prop_at_index(int i)
{
	if (i < 0 || i > propsWeCreated.size() - 1)
	{
		return SpawnedPropInstance();
	}
	SpawnedPropInstance prop = propsWeCreated.at(i);
	return prop;
}

bool is_prop_invincible(std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 A");
		return false;
	}
	return prop.isInvincible;
}

void set_prop_invincible(bool applied, std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 B");
		return;
	}

	//ENTITY::SET_ENTITY_INVINCIBLE(prop.instance, applied);
	ENTITY::SET_ENTITY_CAN_BE_DAMAGED(prop.instance, !applied);
	//ENTITY::SET_ENTITY_PROOFS(prop.instance, applied, applied, applied, applied, applied, applied, applied, applied);

	prop.isInvincible = applied;
}

bool is_prop_immovable(std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 C");
		return false;
	}
	return prop.isImmovable;
}

void set_prop_immovable(bool applied, std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 D");
		return;
	}

	OBJECT::SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN(prop.instance, !applied);
	ENTITY::FREEZE_ENTITY_POSITION(prop.instance, applied);
	if (!applied)
	{
		// 这会解冻它
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(prop.instance, FALSE);
		ENTITY::APPLY_FORCE_TO_ENTITY(prop.instance, 3, 0, 0, 0.1, 0, 0, 0, 0, 1, 1, 0, 0, 1);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(prop.instance, !prop.isInvincible);
	}
	prop.isImmovable = applied;
}

bool is_prop_on_fire(std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 E");
		return false;
	}
	return FIRE::IS_ENTITY_ON_FIRE(prop.instance) == 1;
}

void set_prop_on_fire(bool applied, std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 F");
		return;
	}
	if (applied)
	{
		bool isInvinc = prop.isInvincible;
		//ENTITY::SET_ENTITY_PROOFS(prop.instance, isInvinc, false, isInvinc, isInvinc, isInvinc, isInvinc, isInvinc, isInvinc);
		Vector3 curLocation = ENTITY::GET_ENTITY_COORDS(prop.instance, 0);
		FIRE::ADD_EXPLOSION(curLocation.x, curLocation.y, curLocation.z, 14, 3.0f, true, false, 0); // 引发气体火灾
	}
	else
	{
		FIRE::STOP_ENTITY_FIRE(prop.instance);
		Vector3 position = ENTITY::GET_ENTITY_COORDS(prop.instance, TRUE);
		FIRE::STOP_FIRE_IN_RANGE(position.x, position.y, position.z, 5.0f);
	}
}

bool is_prop_gravity_enabled(std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 G");
		return false;
	}
	return prop.hasGravity;
}

void set_prop_gravity_enabled(bool applied, std::vector<int> extras)
{
	SpawnedPropInstance prop = get_prop_at_index(extras.at(0));
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签 H");
		return;
	}
	ENTITY::SET_ENTITY_HAS_GRAVITY(prop.instance, applied);
	prop.hasGravity = applied;
}

std::string get_explosion_name(int id)
{
	switch (id)
	{
	case EXPLOSION_DEFAULT:
		return "默认";
	case EXPLOSION_MOLOTOV:
		return "燃烧物体";
	case EXPLOSION_WATER_SPRAY_TALL:
		return "水喷射 (高)";
	case EXPLOSION_WATER_SPRAY_SHORT:
		return "水喷射 (矮)";
	case EXPLOSION_WATER_SPRAY_BRIEF:
		return "水喷射 (短暂)";
	case EXPLOSION_GAS_JET_BRIEF:
		return "短火焰 喷射";
	case EXPLOSION_GAS_JET_PROLONGED:
		return "长火焰 喷射";
	case EXPLOSION_GRENADE:
		return "手榴弹";
	case EXPLOSION_SMOKE:
		return "烟雾弹";
	case EXPLOSION_CS_GAS_1:
		return "毒气弹 1";
	case EXPLOSION_CS_GAS_2:
		return "毒气弹 2";
	case EXPLOSION_FLARE:
		return "信号弹";
	case EXPLOSION_MINE:
		return "地雷";
	case EXPLOSION_BLIMP:
		return "飞艇爆炸";
	case EXPLOSION_MINI:
		return "迷你爆炸";
	case EXPLOSION_FIREBALL_SMALL_W_RING:
		return "小型火球爆炸 带冲击波";
	case EXPLOSION_FIREBALL_SMALL_1:
		return "小型火球爆炸 #1";
	case EXPLOSION_FIREBALL_SMALL_2:
		return "小型火球爆炸 #2";
	case EXPLOSION_FIREBALL_SMALL_3:
		return "小型火球爆炸 #3";
	case EXPLOSION_FIREBALL_MEDIUM_1:
		return "中型火球爆炸 #1";
	case EXPLOSION_FIREBALL_MEDIUM_2:
		return "中型火球爆炸 #2";
	case EXPLOSION_FIREBALL_MEDIUM_3:
		return "中型火球爆炸 #3";
	case EXPLOSION_FIREBALL_LARGE_1:
		return "大型火球爆炸 #1";
	case EXPLOSION_FIREBALL_LARGE_2:
		return "大型火球爆炸 #2";
	case EXPLOSION_FIREBALL_LARGE_3:
		return "大型火球爆炸 #3";
	case EXPLOSION_FIREBALL_LARGE_4:
		return "大型火球爆炸 #4";
	default:
		{
		std::ostringstream ss;
		ss << "未知的爆炸 ID" << id;
		auto result = ss.str();
		return result;
		}
	}
}

void teleport_to_last_prop()
{
	SpawnedPropInstance prop = get_prop_at_index(lastSelectedPropIndex);
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Vector3 coords = ENTITY::GET_ENTITY_COORDS(prop.instance, 1);
	Hash objModel = ENTITY::GET_ENTITY_MODEL(prop.instance);
	Hash playerModel = ENTITY::GET_ENTITY_MODEL(playerPed);

	Vector3 minDimens, maxDimens;
	GAMEPLAY::GET_MODEL_DIMENSIONS(objModel, &minDimens, &maxDimens);
	coords.y -= max(3.5f, 2.0f + 0.5f * (maxDimens.y - minDimens.y));

	float newZ;
	if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, coords.z + 3.0f, &newZ))
	{
		coords.z = newZ;
	}

	GAMEPLAY::GET_MODEL_DIMENSIONS(playerModel, &minDimens, &maxDimens);
	coords.z += ((maxDimens.z - minDimens.z) / 2.0f);
	if (minDimens.z < 0)
	{
		coords.z -= minDimens.z;
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(playerPed, coords.x, coords.y, coords.z, 0, 0, 1);
	ENTITY::SET_ENTITY_HEADING(playerPed, 0.0f);
	WAIT(0);
	set_status_text("已传送至此物体旁边！");
}

void explode_last_prop(int explosionID)
{
	SpawnedPropInstance prop = get_prop_at_index(lastSelectedPropIndex);
	if (explosionID == -1)
	{
		explosionID = 0; //默认
	}
	Vector3 position = ENTITY::GET_ENTITY_COORDS(prop.instance, TRUE);
	FIRE::ADD_EXPLOSION(position.x, position.y, position.z, explosionID, 3.0f, true, false, 0);
}

bool onconfirm_prop_single_instance_menu(MenuItem<int> choice)
{
	SpawnedPropInstance prop = get_prop_at_index(lastSelectedPropIndex);
	if (prop.isEmpty())
	{
		set_status_text_centre_screen("无效的 - 标签K");
		return true;
	}

	if (choice.value == 1) // 删除物品
	{
		OBJECT::DELETE_OBJECT(&prop.instance);
		manage_prop_set();
		propInstanceMenuInterruptFlag = true;
		requireRefreshOfPropInstanceMenu = true;
		set_status_text("物体成功删除了！");
		return true;
	}
	else if (choice.value == 2)
	{
		begin_prop_placement(prop);
	}
	else if (choice.value == 3) // 默认爆炸
	{
		explode_last_prop(-1);
	}
	else if (choice.value == 4) // 自定义爆炸
	{
		process_prop_explosion_choices();
	}
	else if (choice.value == 5) // 传送到该位置
	{
		teleport_to_last_prop();
	}
	return false;
}

int singleInstanceMenuIndex = 0;

void flash_prop_callback()
{
	if (!lastHighlightedProp.isEmpty())
	{
		int frame = get_frame_number() % 30;
		if (frame == 0)
		{
			ENTITY::SET_ENTITY_VISIBLE(lastHighlightedProp.instance, FALSE);
		}
		else if (frame == 10)
		{
			ENTITY::SET_ENTITY_VISIBLE(lastHighlightedProp.instance, TRUE);
		}
	}
}

bool prop_spawned_single_instance_menu(int index)
{
	lastSelectedPropIndex = index;

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int>* item = new MenuItem<int>();
	item->value = 1;
	item->caption = "删除此物体";
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->value = 2;
	item->caption = "移动此物体";
	item->isLeaf = false;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->value = 5;
	item->caption = "传送到此物体";
	item->isLeaf = true;
	menuItems.push_back(item);

	FunctionDrivenToggleMenuItem<int>* togItem = new FunctionDrivenToggleMenuItem<int>();
	togItem->getter_call = is_prop_invincible;
	togItem->setter_call = set_prop_invincible;
	togItem->extra_arguments.push_back(index);
	togItem->caption = "开启无敌";
	menuItems.push_back(togItem);

	togItem = new FunctionDrivenToggleMenuItem<int>();
	togItem->getter_call = is_prop_immovable;
	togItem->setter_call = set_prop_immovable;
	togItem->extra_arguments.push_back(index);
	togItem->caption = "禁止移动";
	menuItems.push_back(togItem);

	togItem = new FunctionDrivenToggleMenuItem<int>();
	togItem->getter_call = is_prop_gravity_enabled;
	togItem->setter_call = set_prop_gravity_enabled;
	togItem->extra_arguments.push_back(index);
	togItem->caption = "有无重力";
	menuItems.push_back(togItem);

	togItem = new FunctionDrivenToggleMenuItem<int>();
	togItem->getter_call = is_prop_on_fire;
	togItem->setter_call = set_prop_on_fire;
	togItem->extra_arguments.push_back(index);
	togItem->caption = "燃烧物体";
	menuItems.push_back(togItem);

	SelectFromListMenuItem* alphaItem = new SelectFromListMenuItem(ALPHA_LABELS, onchange_spawn_alpha);
	alphaItem->value = propCreationAlphaIndex;
	alphaItem->caption = "透明度 (不透明度)";
	alphaItem->wrap = false;
	alphaItem->extras.push_back(index);
	menuItems.push_back(alphaItem);

	item = new MenuItem<int>();
	item->value = 3;
	item->caption = "爆炸此物体  (默认)";
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->value = 4;
	item->caption = "自定义爆炸";
	item->isLeaf = false;
	menuItems.push_back(item);
	
	draw_generic_menu<int>(menuItems, &singleInstanceMenuIndex, "编辑物体选项", onconfirm_prop_single_instance_menu, NULL, NULL, NULL);

	return false;
}


bool onconfirm_prop_explosion(MenuItem<int> choice)
{
	explode_last_prop(choice.value);
	return false;
}

int explosionSelection;

void process_prop_explosion_choices()
{
	std::vector<MenuItem<int>*> menuItems;

	for each (int var in OrderedExplosions)
	{
		MenuItem<int>* item = new MenuItem<int>();
		item->value = var;
		item->caption = get_explosion_name(var);
		item->isLeaf = true;
		menuItems.push_back(item);
	}

	draw_generic_menu<int>(menuItems, &explosionSelection, "爆炸选项", onconfirm_prop_explosion, NULL, NULL, NULL);
}

//Save menus

int activeSavedPropSetIndex = 0;
std::string activeSavedPropSlotName;

void spawn_individual_object(SavedPropDBRow* row)
{
	SimpleVector3 coords = { row->posX, row->posY, row->posZ };
	do_spawn_model(row->model, (char*)row->title.c_str(), row->title,
        &coords, row->pitch, row->roll, row->yaw,
        row->isInvincible == 1, row->isImmovable == 1,
        row->hasGravity == 1, row->alpha, true
    );
}

bool spawn_saved_props(int slot, std::string caption)
{
	std::ostringstream ss;
	ss << "正在努力尝试生成！\n此保存集的所有物体...";
	set_status_text(ss.str());

	ENTDatabase* database = get_database();

	manage_prop_set();

	std::vector<SavedPropSet*> saveProps = database->get_saved_prop_sets(slot);
	SavedPropSet* savedSet = saveProps.at(0);
	database->populate_saved_prop_set(savedSet);

	lastKnownSavedPropSetCount = savedSet->size();

	int i = 0;
	for each (SavedPropDBRow* row in savedSet->items)
	{
		spawn_individual_object(row);
		WAIT(0);
		make_periodic_feature_call();
	}

	manage_prop_set();

	delete savedSet;

	std::ostringstream ss2;
	ss2 << "此物体集，生成已完成！";
	set_status_text(ss2.str());

	return false;
}

void save_current_props(int slot)
{
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());
	
	if (bPlayerExists)
	{
		std::ostringstream ss;
		if (slot != -1)
		{
			ss << activeSavedPropSlotName;
		}
		else
		{
			ss << "新建物体集 " << (lastKnownSavedPropSetCount + 1);
		}

		keyboard_on_screen_already = true;
		curr_message = "输入保存名称："; // 保存物体
		auto existingText = ss.str();
		std::string result = show_keyboard("手动输入名称", (char*)existingText.c_str());
		if (!result.empty())
		{
			std::vector<SavedPropDBRow*> dbProps;

			manage_prop_set();

			std::ostringstream ss;
			ss << "已成功保存 " << propsWeCreated.size() << " 个物体！";
			set_status_text(ss.str());

			for each (SpawnedPropInstance prop in propsWeCreated)
			{
				SavedPropDBRow* dbProp = new SavedPropDBRow();

				dbProp->model = ENTITY::GET_ENTITY_MODEL(prop.instance);
				dbProp->title = prop.title;
				dbProp->counter = prop.counter;

				Vector3 position = ENTITY::GET_ENTITY_COORDS(prop.instance, 1);
				dbProp->posX = position.x;
				dbProp->posY = position.y;
				dbProp->posZ = position.z;

				dbProp->pitch = ENTITY::GET_ENTITY_PITCH(prop.instance);
				dbProp->roll = ENTITY::GET_ENTITY_ROLL(prop.instance);
				dbProp->yaw = ENTITY::GET_ENTITY_HEADING(prop.instance);

				dbProp->isImmovable = prop.isImmovable ? 1 : 0;
				dbProp->isInvincible = prop.isInvincible ? 1 : 0;
				dbProp->hasGravity = prop.hasGravity ? 1 : 0;
				dbProp->alpha = ENTITY::GET_ENTITY_ALPHA(prop.instance);

				dbProps.push_back(dbProp);

				make_periodic_feature_call();
				WAIT(0);
			}

			ENTDatabase* database = get_database();
			if (database->save_props(dbProps, result, slot))
			{
				set_status_text("物体已成功保存了！");
				activeSavedPropSlotName = result;
			}
			else
			{
				set_status_text("保存物体时出错了！");
			}

			for (std::vector<SavedPropDBRow*>::iterator it = dbProps.begin(); it != dbProps.end();)
			{
				it = dbProps.erase(it);
			}
		}
	}

	
}

bool onconfirm_savedprops_slot_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case 1: //生成
	{
		spawn_saved_props(activeSavedPropSetIndex, activeSavedPropSlotName);
		break;
	}
	case 2: //覆盖
	{
		save_current_props(activeSavedPropSetIndex);
		requireRefreshOfPropsSaveSlots = true;
		requireRefreshOfPropsSlotMenu = true;
		break;
	}
	case 3: //重命名
	{
		keyboard_on_screen_already = true;
		curr_message = "输入新的名称："; // 重命名已保存的物体
		std::string result = show_keyboard("手动输入名称", (char*)activeSavedPropSlotName.c_str());
		if (!result.empty())
		{
			ENTDatabase* database = get_database();
			database->rename_saved_propset(result, activeSavedPropSetIndex);

			activeSavedPropSlotName = result;
		}
		requireRefreshOfPropsSaveSlots = true;
		requireRefreshOfPropsSlotMenu = true;
		break;
	}
	case 4: //删除
	{
		ENTDatabase* database = get_database();
		database->delete_saved_propset(activeSavedPropSetIndex);

		requireRefreshOfPropsSlotMenu = false;
		requireRefreshOfPropsSaveSlots = true;

		return true;
	}
	case 5:
	{
		std::ostringstream ss;
		ss << "保存的物体集 \"" << activeSavedPropSlotName << "\"";
		auto title = ss.str();

		SaveFileDialogCallback* cb = new SaveFileDialogCallback();
		activeSaveFileCallbacks.insert(cb);

		ENTDatabase* database = get_database();
		std::vector<SavedPropSet*> sets = database->get_saved_prop_sets(activeSavedPropSetIndex);
		SavedPropSet* set = sets.at(0);
		database->populate_saved_prop_set(set);
		cb->data = set;

		set_status_text("保存对话框应该很快会出现！");

		show_save_dialog_in_thread(title, cb);
		break;
	}
	default:
		break;
	}
	return false;
}

bool process_savedprops_slot_menu(int slot)
{
	do
	{
		requireRefreshOfPropsSlotMenu = false;

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 1;
		item->caption = "生成物体";
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

		item = new MenuItem<int>();
		item->isLeaf = true;
		item->value = 5;
		item->caption = "保存为 XML 文件";
		menuItems.push_back(item);

		draw_generic_menu<int>(menuItems, 0, activeSavedPropSlotName, onconfirm_savedprops_slot_menu, NULL, NULL, props_individual_slot_menu_interrupt);
	} while (requireRefreshOfPropsSlotMenu);
	return false;
}

bool onconfirm_savedprops_menu(MenuItem<int> choice)
{
	if (choice.value == -1)
	{
		manage_prop_set();
		if (propsWeCreated.size() == 0)
		{
			set_status_text("您还没有生成任何物体！\n请您先创建一些物体吧！");
		}
		else
		{
			save_current_props(-1);
			requireRefreshOfPropsSaveSlots = true;
		}
		return false;
	}
	else if (choice.value == -2)
	{
		std::ostringstream ss;
		ss << "从 XML 文件加载物体集";
		auto title = ss.str();

		LoadFileDialogCallback* cb = new LoadFileDialogCallback();
		activeLoadFileCallbacks.insert(cb);

		set_status_text("加载对话框应该很快会出现...");

		show_load_dialog_in_thread(title, cb);
		return false;
	}

	ENTDatabase* database = get_database();
	std::vector<SavedPropSet*> savedSets = database->get_saved_prop_sets(choice.value);
	SavedPropSet* savedSet = savedSets.at(0);

	activeSavedPropSlotName = savedSet->saveName;
	activeSavedPropSetIndex = choice.value;
	return process_savedprops_slot_menu(choice.value);
}

bool process_savedprops_menu()
{
	do
	{
		requireRefreshOfPropsSlotMenu = false;
		requireRefreshOfPropsSaveSlots = false;

		ENTDatabase* database = get_database();
		std::vector<SavedPropSet*> savedSets = database->get_saved_prop_sets();
		lastKnownSavedPropSetCount = savedSets.size();

		std::vector<MenuItem<int>*> menuItems;

		MenuItem<int> *item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = -1;
		item->caption = "创建新的物体集存档";
		menuItems.push_back(item);

		item = new MenuItem<int>();
		item->isLeaf = false;
		item->value = -2;
		item->caption = "从 XML 文件导入物体集";
		menuItems.push_back(item);

		for each (SavedPropSet *sv in savedSets)
		{
			std::ostringstream ss;
			ss << sv->saveName << " (" << sv->dbSize << ")";
			MenuItem<int> *item = new MenuItem<int>();
			item->isLeaf = false;
			item->value = sv->rowID;
			item->caption = ss.str();
			menuItems.push_back(item);
		}

		draw_generic_menu<int>(menuItems, 0, "保存的物体集", onconfirm_savedprops_menu, NULL, NULL, props_save_slots_menu_interrupt);

		for (std::vector<SavedPropSet*>::iterator it = savedSets.begin(); it != savedSets.end(); ++it)
		{
			delete (*it);
		}
		savedSets.clear();
	} while (requireRefreshOfPropsSaveSlots);

	return false;
}

bool props_individual_slot_menu_interrupt()
{
	if (requireRefreshOfPropsSlotMenu)
	{
		return true;
	}
	return false;
}

bool props_save_slots_menu_interrupt()
{
	if (requireRefreshOfPropsSaveSlots)
	{
		return true;
	}
	return false;
}

void update_props_pending_dialogs()
{
	std::set<SaveFileDialogCallback*>::iterator it;
	for (it = activeSaveFileCallbacks.begin(); it != activeSaveFileCallbacks.end();)
	{
		SaveFileDialogCallback* saveCB = *it;
		if (saveCB->complete)
		{
			if (saveCB->success)
			{
				set_status_text("正在保存物体中....");
				SavedPropSet* set = static_cast<SavedPropSet*>(saveCB->data);
				bool success = generate_xml_for_propset(set, saveCB->filePath);
				if (success)
				{
					set_status_text("成功保存到 XML 文件！");
				}
				else
				{
					set_status_text("保存到 XML 文件失败了！");
				}
			}
			else
			{
				set_status_text("保存取消了！");
			}
			it = activeSaveFileCallbacks.erase(it);
			delete saveCB->data;
			delete saveCB;
		}
		else
		{
			it++;
		}
	}

	std::set<LoadFileDialogCallback*>::iterator it2;
	for (it2 = activeLoadFileCallbacks.begin(); it2 != activeLoadFileCallbacks.end();)
	{
		LoadFileDialogCallback* loadCB = *it2;
		if (loadCB->complete && !loadCB->processed)
		{
			loadCB->processed = true;
			if (loadCB->success)
			{
				set_status_text("物体文件已成功找到！\n正在解析内容，请稍等...");
				SavedPropSet* set = new SavedPropSet();
				bool success = parse_xml_for_propset(loadCB->filePath, set);
				if (success)
				{
					std::ostringstream ss;
					ss << set->saveName;
					keyboard_on_screen_already = true;
					curr_message = "输入名称："; // 导入对象
					auto existingText = ss.str();
					std::string result = show_keyboard("手动输入名称", (char*)existingText.c_str());
					if (!result.empty())
					{
						ENTDatabase* database = get_database();
						bool dbLoaded = database->save_props(set->items, result, -1);
						if (dbLoaded)
						{
							set_status_text("物体导入成功！");
							requireRefreshOfPropsSaveSlots = true;
						}
						else
						{
							set_status_text("物体导入数据库失败！");
						}
					}
					else
					{
						write_text_to_log_file("键盘没有返回任何值！");
						set_status_text("物体加载失败！");
					}
				}
				else
				{
					set_status_text("物体加载取消！");
				}
			}
			else
			{
				set_status_text("物体加载，已取消或失败！");
			}
			it2 = activeLoadFileCallbacks.erase(it2);
			delete loadCB;
		}
		else
		{
			it2++;
		}
	}
}