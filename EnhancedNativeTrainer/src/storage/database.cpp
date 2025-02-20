/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "database.h"
#include "..\features\vehmodmenu.h"
#include "..\debug\debuglog.h"
#include "..\features\script.h"
#include "..\features\fuel.h"

#include <string>
#include <sstream>

#pragma warning(disable : 4267) // size_t conversion

/**每当更改架构并发布新版本时，应增加此值。
然而，你还必须在 ENTDatabase::handle_version 中添加代码以支持从旧版本升级，
因为这些旧版本已经在实际环境中部署*/
const int DATABASE_VERSION = 17;// 数据库版本号

static int singleIntResultCallback(void *data, int count, char **rows, char **azColName)
{
	if (count == 1 && rows)
	{
		*static_cast<int*>(data) = atoi(rows[0]);
		return 0;
	}
	return 1;
}

static int emptyCallback(void *data, int count, char **rows, char **azColName)
{
	return 0;
}

static int featureEnablementFetchCallback(void *data, int count, char **rows, char **azColName)
{
	std::vector<FeatureEnabledLocalDefinition> defs = *static_cast<std::vector<FeatureEnabledLocalDefinition>*>(data);
	int enabledInt = atoi(rows[1]);
	char *name = rows[0];

	for (int i = 0; i < defs.size(); i++)
	{
		FeatureEnabledLocalDefinition def = defs.at(i);

		if (strcmp(def.name, name) == 0)
		{
			*def.enabled = enabledInt ? 1 : 0;
			if (def.updateFlag != NULL)
			{
				*def.updateFlag = true;
			}
			break;
		}
	}

	return 0;
}

static int genericSettingPairsFetchCallback(void *data, int count, char **rows, char **azColName)
{
	std::vector<StringPairSettingDBRow> *results = static_cast<std::vector<StringPairSettingDBRow>*>(data);
	std::string k = std::string(rows[0]);
	std::string v = std::string(rows[1]);
	StringPairSettingDBRow setting{k,v};
	results->push_back(setting);
	return 0;
}

void ENTDatabase::handle_version(int oldVersion)
{
	if (oldVersion == -1)
	{
		write_text_to_log_file("未找到功能启用表，正在创建它");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_FEATURE_ENABLEMENT (FEATURE_NAME TEXT PRIMARY KEY NOT NULL, FEATURE_ENABLED INT NOT NULL)";
		int rcFeat = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcFeat != SQLITE_OK)
		{
			write_text_to_log_file("功能启用表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("功能启用表已创建");
		}
	}

	if (oldVersion < 2)
	{
		write_text_to_log_file("未找到通用设置对表，正在创建它");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_SETTING_PAIRS (SETTING_NAME TEXT PRIMARY KEY NOT NULL, SETTING_VALUE TEXT NOT NULL)";
		int rcGen = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcGen != SQLITE_OK)
		{
			write_text_to_log_file("通用设置对表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("通用设置对表已创建");
		}
	}

	if (oldVersion < 3)
	{
		write_text_to_log_file("未找到主车辆表，正在创建它");
		char* CREATE_VEHICLE_TABLE_QUERY = "CREATE TABLE ENT_SAVED_VEHICLES ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL, \
			colourPrimary INTEGER, \
			colourSecondary INTEGER, \
			colourExtraPearl INTEGER, \
			colourExtraWheel INTEGER, \
			colourMod1Type INTEGER, \
			colourMod1Colour INTEGER, \
			colourMod1P3 INTEGER, \
			colourMod2Type INTEGER, \
			colourMod2Colour INTEGER, \
			colourCustom1R INTEGER, \
			colourCustom1G INTEGER, \
			colourCustom1B INTEGER, \
			colourCustom2R INTEGER, \
			colourCustom2G INTEGER, \
			colourCustom2B INTEGER, \
			livery INTEGER, \
			plateText TEXT, \
			plateType INTEGER, \
			wheelType INTEGER, \
			windowTint INTEGER, \
			burstableTyres INTEGER, \
			customTyres INTEGER, \
			dirtLevel REAL DEFAULT 0, \
			fadeLevel REAL DEFAULT 0, \
			neonR INTEGER DEFAULT -1, \
			neonG INTEGER DEFAULT -1, \
			neonB INTEGER DEFAULT -1, \
			neon0Enabled INTEGER DEFAULT 0, \
			neon1Enabled INTEGER DEFAULT 0, \
			neon2Enabled INTEGER DEFAULT 0, \
			neon3Enabled INTEGER DEFAULT 0, \
			tyreSmokeR INTEGER DEFAULT -1, \
			tyreSmokeG INTEGER DEFAULT -1, \
			tyreSmokeB INTEGER DEFAULT -1, \
			convertibleRoofUp INTEGER DEFAULT 0, \
			dashColour INTEGER DEFAULT -1, \
			interiorColour INTEGER DEFAULT -1, \
			engineSound STRING DEFAULT -1, \
			xenonColour INTEGER DEFAULT -1, \
			powerMultiplier INTEGER DEFAULT -1 \
			)";
		int rcVeh1 = sqlite3_exec(db, CREATE_VEHICLE_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh1 != SQLITE_OK)
		{
			write_text_to_log_file("主车辆表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主车辆表已创建");
		}

		write_text_to_log_file("未找到车辆附加信息表，正在创建它");
		char* CREATE_VEHEXTRAS_TABLE_QUERY = "CREATE TABLE ENT_VEHICLE_EXTRAS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			extraId INTEGER NOT NULL, \
			extraState INTEGER NOT NULL, \
			UNIQUE(parentId, extraId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_VEHICLES(id) ON DELETE CASCADE \
			)";
		int rcVeh2 = sqlite3_exec(db, CREATE_VEHEXTRAS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh2 != SQLITE_OK)
		{
			write_text_to_log_file("车辆附加信息表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("车辆附加信息表已创建");
		}

		write_text_to_log_file("未找到车辆改装表，正在创建它");
		char* CREATE_VEHMODS_TABLE_QUERY = "CREATE TABLE ENT_VEHICLE_MODS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			modId INTEGER NOT NULL, \
			modState INTEGER NOT NULL, \
			isToggle INTEGER NOT NULL, \
			UNIQUE(parentId, modId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_VEHICLES(id) ON DELETE CASCADE \
			)";

		int rcVeh3 = sqlite3_exec(db, CREATE_VEHMODS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh3 != SQLITE_OK)
		{
			write_text_to_log_file("车辆改装表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("车辆改装表已创建");
		}
	}

	if (oldVersion < 4)
	{
		write_text_to_log_file("未找到主皮肤表，正在创建它");

		char* CREATE_SKIN_TABLE_QUERY = "CREATE TABLE ENT_SAVED_SKINS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL \
			)";

		int rcSkin1 = sqlite3_exec(db, CREATE_SKIN_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin1 != SQLITE_OK)
		{
			write_text_to_log_file("主皮肤表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主皮肤表已创建");
		}

		write_text_to_log_file("未找到皮肤组件表，正在创建它");
		char* CREATE_SKIN_COMPS_TABLE_QUERY = "CREATE TABLE ENT_SKIN_COMPONENTS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			slotId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, slotId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin2 = sqlite3_exec(db, CREATE_SKIN_COMPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin2 != SQLITE_OK)
		{
			write_text_to_log_file("皮肤组件表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("皮肤组件表已创建");
		}

		write_text_to_log_file("未找到皮肤道具表，正在创建它");
		char* CREATE_SKIN_PROPS_TABLE_QUERY = "CREATE TABLE ENT_SKIN_PROPS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			propId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, propId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin3 = sqlite3_exec(db, CREATE_SKIN_PROPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin3 != SQLITE_OK)
		{
			write_text_to_log_file("皮肤道具表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("皮肤道具表已创建");
		}
	}

	if (oldVersion >= 3 && oldVersion < 5)
	{
		char* ADD_TYRES_COL = "ALTER TABLE ENT_SAVED_VEHICLES ADD customTyres INTEGER DEFAULT 0";
		
		int custTyresAddition = sqlite3_exec(db, ADD_TYRES_COL, NULL, 0, &zErrMsg);
		if (custTyresAddition != SQLITE_OK)
		{
			write_text_to_log_file("无法添加自定义轮胎列");
			sqlite3_free(zErrMsg);
		}
	}

	if (oldVersion >= 3 && oldVersion < 6)
	{
		char* queries[]
		{
			"ALTER TABLE ENT_SAVED_VEHICLES ADD dirtLevel REAL DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD fadeLevel REAL DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neonR INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neonG INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neonB INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neon0Enabled INTEGER DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neon1Enabled INTEGER DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neon2Enabled INTEGER DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD neon3Enabled INTEGER DEFAULT 0",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeR INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeG INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD tyreSmokeB INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD convertibleRoofUp INTEGER DEFAULT 0"
		};

		for each (char* q in queries)
		{
			int extraColsAddition = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
			if (extraColsAddition != SQLITE_OK)
			{
				write_text_to_log_file("无法添加 v6 车辆列");
				sqlite3_free(zErrMsg);
			}
		}
	}


	if (oldVersion == 7)
	{
		{
			int dropTable = sqlite3_exec(db, "DROP TABLE ENT_PROP_INSTANCES;", NULL, 0, &zErrMsg);
			if (dropTable != SQLITE_OK)
			{
				write_text_to_log_file("无法修改道具表");
				sqlite3_free(zErrMsg);
			}
		}
		{
			int dropTable = sqlite3_exec(db, "DROP TABLE ENT_PROP_SETS;", NULL, 0, &zErrMsg);
			if (dropTable != SQLITE_OK)
			{
				write_text_to_log_file("无法修改道具表");
				sqlite3_free(zErrMsg);
			}
		}
	}

	if (oldVersion < 8)
	{
		write_text_to_log_file("未找到道具集表，正在创建它");
		char* CREATE_PROP_SETS_TABLE_QUERY = "CREATE TABLE ENT_PROP_SETS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			name TEXT NOT NULL)";

		int propSetRC = sqlite3_exec(db, CREATE_PROP_SETS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (propSetRC != SQLITE_OK)
		{
			write_text_to_log_file("道具集表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("道具集表创建问题");
		}

		write_text_to_log_file("未找到道具实例表，正在创建它");
		char* CREATE_PROP_INSTANCES_TABLE_QUERY = "CREATE TABLE ENT_PROP_INSTANCES ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			modelHash INT NOT NULL, \
			title TEXT NOT NULL, \
			counter INTEGER NOT NULL, \
			posX REAL NOT NULL, \
			posY REAL NOT NULL, \
			posZ REAL NOT NULL, \
			pitch REAL NOT NULL, \
			roll REAL NOT NULL, \
			yaw REAL NOT NULL, \
			isImmovable INT NOT NULL, \
			isInvincible INT NOT NULL, \
			hasGravity INT NOT NULL, \
			alpha INT NOT NULL, \
			FOREIGN KEY (parentId) REFERENCES ENT_PROP_SETS(id) ON DELETE CASCADE)";

		int propInstanceRC = sqlite3_exec(db, CREATE_PROP_INSTANCES_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (propInstanceRC != SQLITE_OK)
		{
			write_text_to_log_file("道具实例表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("道具实例表创建问题");
		}
	}

	if (oldVersion >= 3 && oldVersion < 9)
	{
		char* queries[]
		{
			"ALTER TABLE ENT_SAVED_VEHICLES ADD dashColour INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_VEHICLES ADD interiorColour INTEGER DEFAULT -1"
		};

		for each (char* q in queries)
		{
			int extraColsAddition = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
			if (extraColsAddition != SQLITE_OK)
			{
				write_text_to_log_file("无法添加 v9 车辆列");
				sqlite3_free(zErrMsg);
			}
		}
	}

	if (oldVersion < 10)
	{
		char* ADD_ENGINESOUND_COL = "ALTER TABLE ENT_SAVED_VEHICLES ADD engineSound STRING DEFAULT -1";

		int custTyresAddition = sqlite3_exec(db, ADD_ENGINESOUND_COL, NULL, 0, &zErrMsg);
		if (custTyresAddition != SQLITE_OK)
		{
			write_text_to_log_file("无法添加引擎声音列");
			sqlite3_free(zErrMsg);
		}
	}

	if (oldVersion < 11)
	{
		char* ADD_XENONCOLOUR_COL = "ALTER TABLE ENT_SAVED_VEHICLES ADD xenonColour INTEGER DEFAULT -1";

		int custTyresAddition = sqlite3_exec(db, ADD_XENONCOLOUR_COL, NULL, 0, &zErrMsg);
		if (custTyresAddition != SQLITE_OK)
		{
			write_text_to_log_file("无法添加氙气灯颜色列");
			sqlite3_free(zErrMsg);
		}
	}

	if (oldVersion < 12)
	{
		write_text_to_log_file("未找到主皮肤表，正在创建它");

		char* CREATE_BOD_SKIN_TABLE_QUERY = "CREATE TABLE ENT_SAVED_BOD_SKINS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL, \
			weapon INTEGER NOT NULL, \
			bcomp0 INTEGER NOT NULL, \
			bcomp1 INTEGER NOT NULL, \
			bcomp2 INTEGER NOT NULL, \
			bcomp3 INTEGER NOT NULL, \
			bcomp4 INTEGER NOT NULL, \
			bcomp5 INTEGER NOT NULL, \
			bcomp6 INTEGER NOT NULL, \
			bw_tint INTEGER NOT NULL \
			)";

		int rcSkin1 = sqlite3_exec(db, CREATE_BOD_SKIN_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin1 != SQLITE_OK)
		{
			write_text_to_log_file("主皮肤表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主皮肤表已创建");
		}

		write_text_to_log_file("未找到皮肤组件表，正在创建它");
		char* CREATE_BOD_SKIN_COMPS_TABLE_QUERY = "CREATE TABLE ENT_BOD_SKIN_COMPONENTS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			slotId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, slotId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_BOD_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin2 = sqlite3_exec(db, CREATE_BOD_SKIN_COMPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin2 != SQLITE_OK)
		{
			write_text_to_log_file("皮肤组件表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("皮肤组件表已创建");
		}

		write_text_to_log_file("未找到皮肤道具表，正在创建它");
		char* CREATE_BOD_SKIN_PROPS_TABLE_QUERY = "CREATE TABLE ENT_BOD_SKIN_PROPS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			propId INTEGER NOT NULL, \
			drawable INTEGER NOT NULL, \
			texture INTEGER NOT NULL, \
			UNIQUE(parentId, propId), \
			FOREIGN KEY (parentId) REFERENCES ENT_SAVED_BOD_SKINS(id) ON DELETE CASCADE \
			)";

		int rcSkin3 = sqlite3_exec(db, CREATE_BOD_SKIN_PROPS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin3 != SQLITE_OK)
		{
			write_text_to_log_file("皮肤道具表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("皮肤道具表已创建");
		}
	}

	if (oldVersion < 13)
	{
		write_text_to_log_file("未找到主车辆颜色表，正在创建它");

		char* CREATE_VEH_COLOUR_TABLE_QUERY = "CREATE TABLE ENT_SAVED_VEH_COLOURS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			pcolour INTEGER NOT NULL, \
			scolour INTEGER NOT NULL, \
			pearl INTEGER NOT NULL, \
			wheel INTEGER NOT NULL, \
			pcustomr INTEGER NOT NULL, \
			pcustomg INTEGER NOT NULL, \
			pcustomb INTEGER NOT NULL, \
			scustomr INTEGER NOT NULL, \
			scustomg INTEGER NOT NULL, \
			scustomb INTEGER NOT NULL \
			)";

		int rcSkin1 = sqlite3_exec(db, CREATE_VEH_COLOUR_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin1 != SQLITE_OK)
		{
			write_text_to_log_file("主颜色表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主颜色表已创建");
		}
	}

	if (oldVersion < 14)
	{
		write_text_to_log_file("未找到主武器表，正在创建它");

		char* CREATE_VEH_COLOUR_TABLE_QUERY = "CREATE TABLE ENT_SAVED_WEAPON ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			weapon INTEGER NOT NULL, \
			comp0 INTEGER NOT NULL, \
			comp1 INTEGER NOT NULL, \
			comp2 INTEGER NOT NULL, \
			comp3 INTEGER NOT NULL, \
			comp4 INTEGER NOT NULL, \
			comp5 INTEGER NOT NULL, \
			comp6 INTEGER NOT NULL, \
			w_tint INTEGER NOT NULL \
			)";
		
		int rcSkin1 = sqlite3_exec(db, CREATE_VEH_COLOUR_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcSkin1 != SQLITE_OK)
		{
			write_text_to_log_file("主武器表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主武器表已创建");
		}
	}

	if (oldVersion < 15)
	{
		write_text_to_log_file("未找到主车辆表，正在创建它");
		char* CREATE_VEHICLE_TABLE_QUERY = "CREATE TABLE ENT_TRACKED_VEHICLES ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			saveName TEXT NOT NULL, \
			model INTEGER NOT NULL, \
			colourPrimary INTEGER, \
			colourSecondary INTEGER, \
			colourExtraPearl INTEGER, \
			colourExtraWheel INTEGER, \
			colourMod1Type INTEGER, \
			colourMod1Colour INTEGER, \
			colourMod1P3 INTEGER, \
			colourMod2Type INTEGER, \
			colourMod2Colour INTEGER, \
			colourCustom1R INTEGER, \
			colourCustom1G INTEGER, \
			colourCustom1B INTEGER, \
			colourCustom2R INTEGER, \
			colourCustom2G INTEGER, \
			colourCustom2B INTEGER, \
			livery INTEGER, \
			plateText TEXT, \
			plateType INTEGER, \
			wheelType INTEGER, \
			windowTint INTEGER, \
			burstableTyres INTEGER, \
			customTyres INTEGER, \
			dirtLevel REAL DEFAULT 0, \
			fadeLevel REAL DEFAULT 0, \
			neonR INTEGER DEFAULT -1, \
			neonG INTEGER DEFAULT -1, \
			neonB INTEGER DEFAULT -1, \
			neon0Enabled INTEGER DEFAULT 0, \
			neon1Enabled INTEGER DEFAULT 0, \
			neon2Enabled INTEGER DEFAULT 0, \
			neon3Enabled INTEGER DEFAULT 0, \
			tyreSmokeR INTEGER DEFAULT -1, \
			tyreSmokeG INTEGER DEFAULT -1, \
			tyreSmokeB INTEGER DEFAULT -1, \
			convertibleRoofUp INTEGER DEFAULT 0, \
			dashColour INTEGER DEFAULT -1, \
			interiorColour INTEGER DEFAULT -1, \
			engineSound STRING DEFAULT -1, \
			xenonColour INTEGER DEFAULT -1, \
			cor_x INTEGER DEFAULT -1, \
			cor_y INTEGER DEFAULT -1, \
			cor_z INTEGER DEFAULT -1, \
			crotation INTEGER DEFAULT -1, \
			lfuel REAL DEFAULT -1 \
			)";
		int rcVeh1 = sqlite3_exec(db, CREATE_VEHICLE_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh1 != SQLITE_OK)
		{
			write_text_to_log_file("主车辆表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("主车辆表已创建");
		}

		write_text_to_log_file("未找到车辆附加项表，正在创建它");
		char* CREATE_VEHEXTRAS_TABLE_QUERY = "CREATE TABLE ENT_TRACKED_EXTRAS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			extraId INTEGER NOT NULL, \
			extraState INTEGER NOT NULL, \
			UNIQUE(parentId, extraId), \
			FOREIGN KEY (parentId) REFERENCES ENT_TRACKED_VEHICLES(id) ON DELETE CASCADE \
			)";
		int rcVeh2 = sqlite3_exec(db, CREATE_VEHEXTRAS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh2 != SQLITE_OK)
		{
			write_text_to_log_file("车辆附加项表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("车辆附加项表已创建");
		}

		write_text_to_log_file("未找到车辆改装表，正在创建它");
		char* CREATE_VEHMODS_TABLE_QUERY = "CREATE TABLE ENT_TRACKED_MODS ( \
			id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
			parentId INTEGER NOT NULL, \
			modId INTEGER NOT NULL, \
			modState INTEGER NOT NULL, \
			isToggle INTEGER NOT NULL, \
			UNIQUE(parentId, modId), \
			FOREIGN KEY (parentId) REFERENCES ENT_TRACKED_VEHICLES(id) ON DELETE CASCADE \
			)";

		int rcVeh3 = sqlite3_exec(db, CREATE_VEHMODS_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rcVeh3 != SQLITE_OK)
		{
			write_text_to_log_file("车辆改装表创建问题");
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("车辆改装表已创建");
		}
	}

	if (oldVersion < 16)
	{
		char* queries[]
		{
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp0 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp1 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp2 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp3 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp4 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp5 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bcomp6 INTEGER DEFAULT -1",
			"ALTER TABLE ENT_SAVED_BOD_SKINS ADD bw_tint INTEGER DEFAULT -1"
		};

		for each (char* q in queries)
		{
			int extraColsAddition = sqlite3_exec(db, q, NULL, 0, &zErrMsg);
			if (extraColsAddition != SQLITE_OK)
			{
				write_text_to_log_file("无法添加保镖皮肤列");
				sqlite3_free(zErrMsg);
			}
		}
	}

	if (oldVersion < 17)
	{
		char* ADD_POWERMULTIPLIER_COL = "ALTER TABLE ENT_SAVED_VEHICLES ADD powerMultiplier INTEGER DEFAULT -1";

		int custEnginePow = sqlite3_exec(db, ADD_POWERMULTIPLIER_COL, NULL, 0, &zErrMsg);
		if (custEnginePow != SQLITE_OK)
		{
			write_text_to_log_file("无法添加引擎功率倍数列");
			sqlite3_free(zErrMsg);
		}

		char* ADD_TRACKEDPOWERMULTIPLIER_COL = "ALTER TABLE ENT_TRACKED_VEHICLES ADD powerMultiplier INTEGER DEFAULT -1";

		int custTrEnginePow = sqlite3_exec(db, ADD_TRACKEDPOWERMULTIPLIER_COL, NULL, 0, &zErrMsg);
		if (custTrEnginePow != SQLITE_OK)
		{
			write_text_to_log_file("无法添加引擎功率倍数列");
			sqlite3_free(zErrMsg);
		}
	}
}

bool ENTDatabase::open()
{
	std::stringstream ss;

	write_text_to_log_file("正在初始化 DB 数据库引擎");

	sqlite3_initialize();

	db_mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_RECURSIVE);

	write_text_to_log_file("正在打开 DB 数据库文件");

	WCHAR* db_path = get_storage_dir_path("ent.db");

	std::wstring ws(db_path);
	std::string fileSS(ws.begin(), ws.end());
	write_text_to_log_file(fileSS);

	mutex_lock();

	int rc = sqlite3_open16(db_path, &db);
	delete db_path;
	if (rc == SQLITE_OK)
	{
		write_text_to_log_file("DB 数据库已打开");
	}
	else
	{
		write_text_to_log_file("DB 数据库无法打开或创建");
		ss.str(""); ss.clear();
		ss << "DB 数据库错误代码为: " << rc;
		write_text_to_log_file(ss.str());
		close();
		mutex_unlock();
		return false;
	}

	char* PRAGMA_FKEY_QUERY = "PRAGMA foreign_keys = ON";
	int rcPragma = sqlite3_exec(db, PRAGMA_FKEY_QUERY, NULL, 0, &zErrMsg);
	if (rcPragma != SQLITE_OK)
	{
		write_text_to_log_file("无法执行 PRAGMA 语句");
		sqlite3_free(zErrMsg);
	}

	int count = 0;
	char* VERSION_TABLE_EXISTS_QUERY = "select count(type) from sqlite_master where type = 'table' and name = 'ENT_DB_MANIFEST' ";
	rc = sqlite3_exec(db, VERSION_TABLE_EXISTS_QUERY, singleIntResultCallback, &count, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("无法查询 sqlite_master 表");
		sqlite3_free(zErrMsg);
	}

	if (count == 0 )
	{
		write_text_to_log_file("未找到 Manifest 表，正在创建它");
		char* CREATE_VERSION_TABLE_QUERY = "create table ENT_DB_MANIFEST (MANIFEST_KEY TEXT PRIMARY KEY NOT NULL, MANIFEST_VALUE TEXT NOT NULL)";
		rc = sqlite3_exec(db, CREATE_VERSION_TABLE_QUERY, NULL, 0, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("Manifest 表创建问题");
			sqlite3_free(zErrMsg);
			mutex_unlock();
			return false;
		}
		else
		{
			write_text_to_log_file("Manifest 表已创建");
		}
	}

	int dbversion = -1;
	char* FIND_VERSION_TABLE_QUERY = "select CAST(MANIFEST_VALUE as integer) from ENT_DB_MANIFEST where MANIFEST_KEY='VERSION'";
	rc = sqlite3_exec(db, FIND_VERSION_TABLE_QUERY, singleIntResultCallback, &dbversion, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("未找到版本");
		sqlite3_free(zErrMsg);
	}
	else
	{
		ss.str(""); ss.clear();
		ss << "未找到版本: " << dbversion;
		write_text_to_log_file(ss.str());
		handle_version(dbversion);
	}

	if (dbversion != DATABASE_VERSION)
	{
		ss.str(""); ss.clear();
		ss << "INSERT OR REPLACE INTO ENT_DB_MANIFEST (MANIFEST_KEY, MANIFEST_VALUE) VALUES('VERSION', '" << DATABASE_VERSION << "')";
		auto ssStr = ss.str();
		rc = sqlite3_exec(db, ssStr.c_str(), emptyCallback, NULL, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("无法更新版本");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			write_text_to_log_file("版本已更新");
		}
	}

	mutex_unlock();
	return true;
}

void ENTDatabase::close()
{
	write_text_to_log_file("正在关闭 DB 数据库");
	if (db != NULL)
	{
		int rc = sqlite3_close_v2(db);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("DB 数据库未正确关闭！");
		}
		db = NULL;
	}

	sqlite3_mutex_free(db_mutex);

	write_text_to_log_file("DB 数据库已关闭");
	sqlite3_shutdown();
	write_text_to_log_file("DB 数据库已停止");
}

void ENTDatabase::store_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
	bool cacheIsSame = true;
	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			cacheIsSame = false;
			break;
		}
		bool enabledInCache = featureEnablementCache[def.name];
		if ( *def.enabled != enabledInCache)
		{
			cacheIsSame = false;
			break;
		}
	}

	if (cacheIsSame)
	{
		return;
	}

	mutex_lock();
	begin_transaction();

	write_text_to_log_file("请求存储特征对");
	for (int i = 0; i < values.size(); i++)
	{
		FeatureEnabledLocalDefinition def = values.at(i);
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_FEATURE_ENABLEMENT VALUES ('" << def.name << "', " << (*def.enabled ? 1 : 0) << ")";
		auto ssStr = ss.str();
		int rc = sqlite3_exec(db, ssStr.c_str(), emptyCallback, NULL, &zErrMsg);
		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("未能插入功能启用行");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(zErrMsg);
			sqlite3_free(zErrMsg);
			break;
		}
	}

	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			std::pair<std::string, bool> pair(std::string(def.name), *def.enabled);
			featureEnablementCache.insert(pair);
		}
		else
		{
			featureEnablementCache[def.name] = *def.enabled;
		}
	}

	end_transaction();
	mutex_unlock();

	write_text_to_log_file("特征对存储完成");
}

void ENTDatabase::load_feature_enabled_pairs(std::vector<FeatureEnabledLocalDefinition> values)
{
	mutex_lock();

	{
		std::stringstream ss;
		ss << "请求加载 " << values.size() << " 特征对";
		write_text_to_log_file(ss.str());
	}
	std::vector<FeatureEnabledDBRow> dbPairs;
	char* QUERY = "select FEATURE_NAME, FEATURE_ENABLED from ENT_FEATURE_ENABLEMENT";
	int rc = sqlite3_exec(db, QUERY, featureEnablementFetchCallback, &values, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("特征对加载失败");
		write_text_to_log_file(zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		write_text_to_log_file("特征对加载完成");
	}

	for each (FeatureEnabledLocalDefinition def in values)
	{
		if (featureEnablementCache.find(def.name) == featureEnablementCache.end())
		{
			std::pair<std::string, bool> pair(std::string(def.name), *def.enabled);
			featureEnablementCache.insert(pair);
		}
		else
		{
			featureEnablementCache[def.name] = *def.enabled;
		}
	}

	mutex_unlock();
}

void ENTDatabase::store_setting_pairs(std::vector<StringPairSettingDBRow> values)
{
	bool cacheIsSame = true;
	for each (StringPairSettingDBRow row in values)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			cacheIsSame = false;
			break;
		}
		std::string valueInCache = genericSettingsCache[row.name];
		if (valueInCache.compare(row.value) != 0)
		{
			cacheIsSame = false;
			break;
		}
	}

	if (cacheIsSame)
	{
		return;
	}

	mutex_lock();
	begin_transaction();

	{
		std::stringstream ss;
		ss << "请求存储 " << values.size() << " 通用对";
		write_text_to_log_file(ss.str());
	}

	write_text_to_log_file("请求存储通用对");
	for (int i = 0; i < values.size(); i++)
	{
		StringPairSettingDBRow setting = values.at(i);
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SETTING_PAIRS VALUES (?, ?);";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc == SQLITE_OK)
		{
			{
				std::stringstream ss;
				ss << "正在存储通用对 " << setting.name << " 值为 " << setting.value;
				write_text_to_log_file(ss.str());
			}

			// 绑定值
			sqlite3_bind_text(stmt, 1, setting.name.c_str(), setting.name.length(), 0);
			sqlite3_bind_text(stmt, 2, setting.value.c_str(), setting.value.length(), 0);

			// 提交
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
		else
		{
			write_text_to_log_file("未能准备设置行");
			write_text_to_log_file(ss.str());
			write_text_to_log_file(sqlite3_errmsg(db));
			break;
		}
	}

	for each (StringPairSettingDBRow row in values)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			std::pair<std::string, std::string> pair(row.name, row.value);
			genericSettingsCache.insert(pair);
		}
		else
		{
			genericSettingsCache[row.name] = row.value;
		}
	}

	end_transaction();
	mutex_unlock();

	write_text_to_log_file("通用对存储完成");
}

std::vector<StringPairSettingDBRow> ENTDatabase::load_setting_pairs()
{
	mutex_lock();

	write_text_to_log_file("请求加载通用对");
	std::vector<StringPairSettingDBRow> dbPairs;
	char* QUERY = "select SETTING_NAME, SETTING_VALUE from ENT_SETTING_PAIRS";
	int rc = sqlite3_exec(db, QUERY, genericSettingPairsFetchCallback, &dbPairs, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("特征对加载失败");
		write_text_to_log_file(zErrMsg);
		sqlite3_free(zErrMsg);
	}

	for each (StringPairSettingDBRow row in dbPairs)
	{
		if (genericSettingsCache.find(row.name) == genericSettingsCache.end())
		{
			std::pair<std::string, std::string> pair(row.name, row.value);
			genericSettingsCache.insert(pair);
		}
		else
		{
			genericSettingsCache[row.name] = row.value;
		}
	}

	mutex_unlock();

	write_text_to_log_file("通用对加载完成");
	return dbPairs;
}

void ENTDatabase::save_vehicle_extras(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 1; i < 10; i++)
	{
		if (!VEHICLE::DOES_EXTRA_EXIST(veh, i))
		{
			continue;
		}

		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_VEHICLE_EXTRAS VALUES (?, ?, ?, ?)";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("车辆附加信息保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);
			sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, i) ? 1 : 0);

			// 提交
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}
	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_vehicle_mods(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 50; i++) //总共 50 个改装项目（包括 Benny 的改装）
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_VEHICLE_MODS VALUES (?, ?, ?, ?, ?)";

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("车辆改装信息保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			bool isToggleable = (i >= 17 && i <= 22);

			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);

			if (isToggleable)
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::IS_TOGGLE_MOD_ON(veh, i) ? 1 : 0);
			}
			else
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD(veh, i));
			}
			sqlite3_bind_int(stmt, index++, isToggleable ? 1 : 0);
			// 提交
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

bool ENTDatabase::save_vehicle(Vehicle veh, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_VEHICLES VALUES (";
	for (int i = 0; i < 43; i++)
	{
		if (i > 0)
		{
			ss << ", ";
		}
		ss << "?";
	}
	ss << ");";

	/*
	id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \ 1
			saveName TEXT NOT NULL, \ 2
			model INTEGER NOT NULL, \ 3
			colourPrimary INTEGER, \ 4
			colourSecondary INTEGER, \ 5
			colourExtraPearl INTEGER, \ 6
			colourExtraWheel INTEGER, \ 7
			colourMod1Type INTEGER, \ 8
			colourMod1Colour INTEGER, \ 9
			colourMod1P3 INTEGER, \ 10
			colourMod2Type INTEGER, \ 11
			colourMod2Colour INTEGER, \ 12
			colourCustom1R INTEGER, \ 13
			colourCustom1G INTEGER, \ 14
			colourCustom1B INTEGER, \ 15
			colourCustom2R INTEGER, \ 16
			colourCustom2G INTEGER, \ 17
			colourCustom2B INTEGER, \ 18

	*/

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("车辆信息保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}
	else
	{
		int index = 1;
		if (slot == -1)
		{
			sqlite3_bind_null(stmt, index++);
		}
		else
		{
			sqlite3_bind_int64(stmt, index++, slot);
		}
		sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
		sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(veh)); //模型

		int primaryCol, secondaryCol;
		VEHICLE::GET_VEHICLE_COLOURS(veh, &primaryCol, &secondaryCol);
		sqlite3_bind_int(stmt, index++, primaryCol);
		sqlite3_bind_int(stmt, index++, secondaryCol);

		int pearlCol, wheelCol;
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearlCol, &wheelCol);
		sqlite3_bind_int(stmt, index++, pearlCol);
		sqlite3_bind_int(stmt, index++, wheelCol);

		int mod1a, mod1b, mod1c;
		VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &mod1a, &mod1b, &mod1c);
		sqlite3_bind_int(stmt, index++, mod1a);
		sqlite3_bind_int(stmt, index++, mod1b);
		sqlite3_bind_int(stmt, index++, mod1c);

		int mod2a, mod2b;
		VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &mod2a, &mod2b);
		sqlite3_bind_int(stmt, index++, mod2a);
		sqlite3_bind_int(stmt, index++, mod2b);

		if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
		{
			int custR1, custG1, custB1;
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &custR1, &custG1, &custB1);
			sqlite3_bind_int(stmt, index++, custR1);
			sqlite3_bind_int(stmt, index++, custG1);
			sqlite3_bind_int(stmt, index++, custB1);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh))
		{
			int custR2, custG2, custB2;
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &custR2, &custG2, &custB2);
			sqlite3_bind_int(stmt, index++, custR2);
			sqlite3_bind_int(stmt, index++, custG2);
			sqlite3_bind_int(stmt, index++, custB2);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		/*
		livery INTEGER, \ 19
		plateText TEXT, \ 20
		plateType INTEGER, \ 21
		wheelType INTEGER, \ 22
		windowTint INTEGER, \ 23
		burstableTyres INTEGER \ 24
		engineSound STRING, \ 25
		xenonColour INTEGER, \ 26
		*/

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_LIVERY(veh));

		char* plateText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
		sqlite3_bind_text(stmt, index++, plateText, strlen(plateText), 0);

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh) ? 1 : 0);
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, 23) ? 1 : 0);

		//sqlite3_bind_text(stmt, index++, CURRSOUNDENGINE[0], strlen(plateText), 0);
		/*
		dirtLevel REAL DEFAULT 0, \ 25
			fadeLevel REAL DEFAULT 0, \ 26
			neonR INTEGER DEFAULT - 1, \ 27
			neonG INTEGER DEFAULT - 1, \ 28
			neonB INTEGER DEFAULT - 1, \ 29
			neon0Enabled INTEGER DEFAULT 0, \ 30
			neon1Enabled INTEGER DEFAULT 0, \ 21
			neon2Enabled INTEGER DEFAULT 0, \ 32
			neon3Enabled INTEGER DEFAULT 0 \*/

		sqlite3_bind_double(stmt, index++, VEHICLE::GET_VEHICLE_DIRT_LEVEL(veh));
		sqlite3_bind_double(stmt, index++, VEHICLE::GET_VEHICLE_ENVEFF_SCALE(veh));

		int neonR, neonG, neonB;
		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &neonR, &neonG, &neonB);
		sqlite3_bind_int(stmt, index++, neonR);
		sqlite3_bind_int(stmt, index++, neonG);
		sqlite3_bind_int(stmt, index++, neonB);

		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 0));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 1));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 2));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 3));

		int tyreSmokeR, tyreSmokeG, tyreSmokeB;
		VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &tyreSmokeR, &tyreSmokeG, &tyreSmokeB);
		sqlite3_bind_int(stmt, index++, tyreSmokeR);
		sqlite3_bind_int(stmt, index++, tyreSmokeG);
		sqlite3_bind_int(stmt, index++, tyreSmokeB);

		sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, 0) && VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh));

		//仪表盘颜色 整数类型
		//内饰颜色 整数类型
		int dashCol, interiorCol;
		VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &dashCol);
		VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &interiorCol);
		sqlite3_bind_int(stmt, index++, dashCol);
		sqlite3_bind_int(stmt, index++, interiorCol);

		char* tmp_i = (char*)current_picked_engine_sound.c_str();
		sqlite3_bind_text(stmt, index++, tmp_i, strlen(tmp_i), 0);
		current_picked_engine_sound = "";

		int xenonColour = -1;
		if (getGameVersion() > 45) xenonColour = VEHICLE::GET_VEHICLE_XENON_COLOUR(veh);
		sqlite3_bind_int(stmt, index++, xenonColour);

		int powerMultiplier = -1;
		for (int kl = 0; kl < C_ENGINE_M.size(); kl++) {
			if (C_ENGINE_VEHICLE[kl] == veh) {
				powerMultiplier = C_ENGINE_M[kl];
			}
		}
		sqlite3_bind_int(stmt, index++, powerMultiplier);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

		// 如果正在更新，则删除所有已存在的子项
		if (slot != -1)
		{
			delete_saved_vehicle_children(slot);
		}

		save_vehicle_extras(veh, newRowID);
		save_vehicle_mods(veh, newRowID);
	}

	mutex_unlock();

	return result;
}

void ENTDatabase::save_skin_components(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 12; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SKIN_COMPONENTS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_DRAWABLE_VARIATION(ped, i);
		int texture = PED::GET_PED_TEXTURE_VARIATION(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("皮肤组件保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //插槽 ID
			sqlite3_bind_int(stmt, index++, drawable); //可绘制资源 ID
			sqlite3_bind_int(stmt, index++, texture); //纹理 ID

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_skin_props(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 10; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_SKIN_PROPS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_PROP_INDEX(ped, i);
		int texture = PED::GET_PED_PROP_TEXTURE_INDEX(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("皮肤道具保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //插槽 ID
			sqlite3_bind_int(stmt, index++, drawable); //可绘制资源 ID
			sqlite3_bind_int(stmt, index++, texture); //纹理 ID

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

bool ENTDatabase::save_skin(Ped ped, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_SKINS VALUES (?, ?, ?);";

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("车辆信息保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}

	int index = 1;
	if (slot == -1)
	{
		sqlite3_bind_null(stmt, index++);
	}
	else
	{
		sqlite3_bind_int64(stmt, index++, slot);
	}
	sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
	sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(ped)); //模型

	// 提交
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

	//如果正在更新，则删除所有已存在的子项
	if (slot != -1)
	{
		delete_saved_skin_children(slot);
	}

	save_skin_components(ped, newRowID);
	save_skin_props(ped, newRowID);

	mutex_unlock();

	return result;
}

// 保存，加载，保镖
void ENTDatabase::populate_saved_bod_skin(SavedBodSkinDBRow *entry)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "select * from ENT_BOD_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			SavedBodSkinComponentDBRow *comp = new SavedBodSkinComponentDBRow();
			//0 和 1 是 ID
			comp->slotID = sqlite3_column_int(stmt, 2);
			comp->drawable = sqlite3_column_int(stmt, 3);
			comp->texture = sqlite3_column_int(stmt, 4);
			entry->components.push_back(comp);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "select * from ENT_BOD_SKIN_PROPS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			SavedBodSkinPropDBRow *prop = new SavedBodSkinPropDBRow();
			//0 和 1 是 ID
			prop->propID = sqlite3_column_int(stmt2, 2);
			prop->drawable = sqlite3_column_int(stmt2, 3);
			prop->texture = sqlite3_column_int(stmt2, 4);
			entry->props.push_back(prop);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤道具");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("已加载保存的皮肤");
	return;
}

void ENTDatabase::save_bod_skin_components(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 12; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_BOD_SKIN_COMPONENTS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_DRAWABLE_VARIATION(ped, i);
		int texture = PED::GET_PED_TEXTURE_VARIATION(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("皮肤组件保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //插槽 ID
			sqlite3_bind_int(stmt, index++, drawable); //可绘制资源 ID
			sqlite3_bind_int(stmt, index++, texture); //纹理 ID

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_bod_skin_props(Ped ped, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 10; i++)
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_BOD_SKIN_PROPS VALUES (?, ?, ?, ?, ?)";

		int drawable = PED::GET_PED_PROP_INDEX(ped, i);
		int texture = PED::GET_PED_PROP_TEXTURE_INDEX(ped, i);

		sqlite3_stmt *stmt;
		const char *pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("皮肤道具保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i); //插槽 ID
			sqlite3_bind_int(stmt, index++, drawable); //可绘制资源 ID
			sqlite3_bind_int(stmt, index++, texture); //纹理 ID

			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::delete_saved_bod_skin_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_BOD_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的保镖组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "DELETE FROM ENT_BOD_SKIN_PROPS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt2, 1, slot);

		// 提交
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能删除保存的保镖道具");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

bool ENTDatabase::save_bod_skin(Ped ped, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_BOD_SKINS VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	Hash bcomp0 = -1;
	Hash bcomp1 = -1;
	Hash bcomp2 = -1;
	Hash bcomp3 = -1;
	Hash bcomp4 = -1;
	Hash bcomp5 = -1;
	Hash bcomp6 = -1;
	int bcomp_s = 0;
	int bw_tint = -1;

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("保镖信息保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}

	for (int a = 0; a < WEAPONTYPES_MOD.size(); a++) {
		for (int b = 0; b < VOV_WEAPONMOD_VALUES[a].size(); b++) {
			char* weaponName = (char*)WEAPONTYPES_MOD.at(a).c_str(), * compName = (char*)VOV_WEAPONMOD_VALUES[a].at(b).c_str();
			Hash weaponHash = GAMEPLAY::GET_HASH_KEY(weaponName), compHash = GAMEPLAY::GET_HASH_KEY(compName);

			if (weaponHash == WEAPON::GET_SELECTED_PED_WEAPON(ped)) {
				if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(ped, weaponHash, compHash)) {
					if (bcomp_s == 0) bcomp0 = compHash;
					if (bcomp_s == 1) bcomp1 = compHash;
					if (bcomp_s == 2) bcomp2 = compHash;
					if (bcomp_s == 3) bcomp3 = compHash;
					if (bcomp_s == 4) bcomp4 = compHash;
					if (bcomp_s == 5) bcomp5 = compHash;
					if (bcomp_s == 6) bcomp6 = compHash;
					bcomp_s = bcomp_s + 1;
				}
			}
		}
	}
	bw_tint = WEAPON::GET_PED_WEAPON_TINT_INDEX(ped, WEAPON::GET_SELECTED_PED_WEAPON(ped));

	int index = 1;
	if (slot == -1)
	{
		sqlite3_bind_null(stmt, index++);
	}
	else
	{
		sqlite3_bind_int64(stmt, index++, slot);
	}
	sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
	sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(ped)); //模型
	sqlite3_bind_int(stmt, index++, WEAPON::GET_SELECTED_PED_WEAPON(ped)); // 武器
	sqlite3_bind_int(stmt, index++, bcomp0); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp1); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp2); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp3); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp4); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp5); // 武器组件
	sqlite3_bind_int(stmt, index++, bcomp6); // 武器组件
	sqlite3_bind_int(stmt, index++, bw_tint); // 武器颜色

	// 提交
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

	//如果正在更新，则删除所有已存在的子项
	if (slot != -1)
	{
		delete_saved_bod_skin_children(slot);
	}

	save_bod_skin_components(ped, newRowID);
	save_bod_skin_props(ped, newRowID);

	mutex_unlock();

	return result;
}

void ENTDatabase::rename_saved_bod_skin(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_BOD_SKINS SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的保镖");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_bod_skin(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_BOD_SKINS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的保镖");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

std::vector<SavedBodSkinDBRow*> ENTDatabase::get_saved_bod_skins(int index)
{
	write_text_to_log_file("请求加载已保存的保镖");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_BOD_SKINS";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedBodSkinDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到皮肤行");

			SavedBodSkinDBRow *skin = new SavedBodSkinDBRow();

			int index = 0;
			skin->rowID = sqlite3_column_int(stmt, index++);
			skin->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			skin->model = sqlite3_column_int(stmt, index++);
			skin->weapon = sqlite3_column_int(stmt, index++);
			skin->bcomp0 = sqlite3_column_int(stmt, index++);
			skin->bcomp1 = sqlite3_column_int(stmt, index++);
			skin->bcomp2 = sqlite3_column_int(stmt, index++);
			skin->bcomp3 = sqlite3_column_int(stmt, index++);
			skin->bcomp4 = sqlite3_column_int(stmt, index++);
			skin->bcomp5 = sqlite3_column_int(stmt, index++);
			skin->bcomp6 = sqlite3_column_int(stmt, index++);
			skin->bw_tint = sqlite3_column_int(stmt, index++);

			results.push_back(skin);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}
// 保存/加载保镖结束

// 已保存的武器
bool ENTDatabase::save_weapon(Ped ped, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_WEAPON VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	Hash comp0 = -1;
	Hash comp1 = -1;
	Hash comp2 = -1;
	Hash comp3 = -1;
	Hash comp4 = -1;
	Hash comp5 = -1;
	Hash comp6 = -1;
	int comp_s = 0;
	int w_tint = -1;

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("武器保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}

	for (int a = 0; a < WEAPONTYPES_MOD.size(); a++) {
		for (int b = 0; b < VOV_WEAPONMOD_VALUES[a].size(); b++) {
			char* weaponName = (char*)WEAPONTYPES_MOD.at(a).c_str(), * compName = (char*)VOV_WEAPONMOD_VALUES[a].at(b).c_str();
			Hash weaponHash = GAMEPLAY::GET_HASH_KEY(weaponName), compHash = GAMEPLAY::GET_HASH_KEY(compName);

			if (weaponHash == WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID())) {
				if (WEAPON::HAS_PED_GOT_WEAPON_COMPONENT(PLAYER::PLAYER_PED_ID(), weaponHash, compHash)) {
					if (comp_s == 0) comp0 = compHash;
					if (comp_s == 1) comp1 = compHash;
					if (comp_s == 2) comp2 = compHash;
					if (comp_s == 3) comp3 = compHash;
					if (comp_s == 4) comp4 = compHash;
					if (comp_s == 5) comp5 = compHash;
					if (comp_s == 6) comp6 = compHash;
					comp_s = comp_s + 1;
				}
			}
		}
	}
	w_tint = WEAPON::GET_PED_WEAPON_TINT_INDEX(PLAYER::PLAYER_PED_ID(), WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()));

	int index = 1;
	if (slot == -1)
	{
		sqlite3_bind_null(stmt, index++);
	}
	else
	{
		sqlite3_bind_int64(stmt, index++, slot);
	}
	sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
	sqlite3_bind_int(stmt, index++, WEAPON::GET_SELECTED_PED_WEAPON(ped)); // 武器
	sqlite3_bind_int(stmt, index++, comp0); // 武器组件
	sqlite3_bind_int(stmt, index++, comp1); // 武器组件
	sqlite3_bind_int(stmt, index++, comp2); // 武器组件
	sqlite3_bind_int(stmt, index++, comp3); // 武器组件
	sqlite3_bind_int(stmt, index++, comp4); // 武器组件
	sqlite3_bind_int(stmt, index++, comp5); // 武器组件
	sqlite3_bind_int(stmt, index++, comp6); // 武器组件
	sqlite3_bind_int(stmt, index++, w_tint); // 武器颜色
	
	// 提交
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

	mutex_unlock();

	return result;
}

void ENTDatabase::rename_saved_weapon(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto qStr = "UPDATE ENT_SAVED_WEAPON SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的武器");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_weapon(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_WEAPON WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的武器");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

std::vector<SavedWeaponDBRow*> ENTDatabase::get_saved_weapon(int index)
{
	write_text_to_log_file("请求加载已保存的武器");

	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_WEAPON";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedWeaponDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到武器行");

			SavedWeaponDBRow* skin = new SavedWeaponDBRow();

			int index = 0;
			skin->rowID = sqlite3_column_int(stmt, index++);
			skin->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			skin->weapon = sqlite3_column_int(stmt, index++);
			skin->comp0 = sqlite3_column_int(stmt, index++);
			skin->comp1 = sqlite3_column_int(stmt, index++);
			skin->comp2 = sqlite3_column_int(stmt, index++);
			skin->comp3 = sqlite3_column_int(stmt, index++);
			skin->comp4 = sqlite3_column_int(stmt, index++);
			skin->comp5 = sqlite3_column_int(stmt, index++);
			skin->comp6 = sqlite3_column_int(stmt, index++);
			skin->w_tint = sqlite3_column_int(stmt, index++);

			results.push_back(skin);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的武器");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}
// 已保存的武器结束

// 保存/加载车辆颜色
void ENTDatabase::rename_saved_veh_colour(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_VEH_COLOURS SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的颜色");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_veh_colour(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_VEH_COLOURS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的颜色");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

std::vector<SavedVehColourDBRow*> ENTDatabase::get_saved_veh_colours(int index)
{
	write_text_to_log_file("请求加载已保存的颜色");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_VEH_COLOURS";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedVehColourDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到皮肤行");

			SavedVehColourDBRow *skin = new SavedVehColourDBRow();

			int index = 0;
			skin->rowID = sqlite3_column_int(stmt, index++);
			skin->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			skin->pcolour = sqlite3_column_int(stmt, index++);
			skin->scolour = sqlite3_column_int(stmt, index++);
			skin->pearl = sqlite3_column_int(stmt, index++);
			skin->wheel = sqlite3_column_int(stmt, index++);
			skin->pcustomr = sqlite3_column_int(stmt, index++);
			skin->pcustomg = sqlite3_column_int(stmt, index++);
			skin->pcustomb = sqlite3_column_int(stmt, index++);
			skin->scustomr = sqlite3_column_int(stmt, index++);
			skin->scustomg = sqlite3_column_int(stmt, index++);
			skin->scustomb = sqlite3_column_int(stmt, index++);

			results.push_back(skin);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的颜色");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

bool ENTDatabase::save_veh_colour(Ped ped, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_SAVED_VEH_COLOURS VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("颜色保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}

	int index = 1;
	if (slot == -1)
	{
		sqlite3_bind_null(stmt, index++);
	}
	else
	{
		sqlite3_bind_int64(stmt, index++, slot);
	}
	int primary, secondary, pearl, wheel, pcustomr, pcustomg, pcustomb, scustomr, scustomg, scustomb;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, 1);
	VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);
	VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &pcustomr, &pcustomg, &pcustomb);
	VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &scustomr, &scustomg, &scustomb);
	sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
	sqlite3_bind_int(stmt, index++, primary); // 主色调
	sqlite3_bind_int(stmt, index++, secondary); // 次色调
	sqlite3_bind_int(stmt, index++, pearl); // 珠光色
	sqlite3_bind_int(stmt, index++, wheel); // 轮毂颜色
	sqlite3_bind_int(stmt, index++, pcustomr); // 主色调 R 值
	sqlite3_bind_int(stmt, index++, pcustomg); // 主色调 G 值
	sqlite3_bind_int(stmt, index++, pcustomb); // 主色调 B 值
	sqlite3_bind_int(stmt, index++, scustomr); // 次色调 R 值
	sqlite3_bind_int(stmt, index++, scustomg); // 次色调 G 值
	sqlite3_bind_int(stmt, index++, scustomb); // 次色调 B 值

	// 提交
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);

	sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

	// 如果正在更新，则删除所有已存在的子项
	//if (slot != -1)
	//{
	//	delete_saved_veh_colour_children(slot);
	//}

	//save_bod_skin_components(ped, newRowID);
	//save_bod_skin_props(ped, newRowID);

	mutex_unlock();

	return result;
}
// 保存/加载车辆颜色结束

std::vector<SavedSkinDBRow*> ENTDatabase::get_saved_skins(int index)
{
	write_text_to_log_file("请求加载已保存的皮肤");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_SKINS";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedSkinDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到皮肤行");

			SavedSkinDBRow *skin = new SavedSkinDBRow();

			int index = 0;
			skin->rowID = sqlite3_column_int(stmt, index++);
			skin->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			skin->model = sqlite3_column_int(stmt, index++);
			
			results.push_back(skin);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

std::vector<SavedVehicleDBRow*> ENTDatabase::get_saved_vehicles(int index)
{
	write_text_to_log_file("请求加载已保存的车辆");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_SAVED_VEHICLES";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedVehicleDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到车辆行");

			SavedVehicleDBRow *veh = new SavedVehicleDBRow();

			int index = 0;
			veh->rowID = sqlite3_column_int(stmt, index++);
			veh->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->model = sqlite3_column_int(stmt, index++);
			veh->colourPrimary = sqlite3_column_int(stmt, index++);
			veh->colourSecondary = sqlite3_column_int(stmt, index++);
			veh->colourExtraPearl = sqlite3_column_int(stmt, index++);
			veh->colourExtraWheel = sqlite3_column_int(stmt, index++);
			veh->colourMod1Type = sqlite3_column_int(stmt, index++);
			veh->colourMod1Colour = sqlite3_column_int(stmt, index++);
			veh->colourMod1P3 = sqlite3_column_int(stmt, index++);
			veh->colourMod2Type = sqlite3_column_int(stmt, index++);
			veh->colourMod2Colour = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[2] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[2] = sqlite3_column_int(stmt, index++);

			veh->livery = sqlite3_column_int(stmt, index++);
			veh->plateText = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->plateType = sqlite3_column_int(stmt, index++);
			veh->wheelType = sqlite3_column_int(stmt, index++);
			veh->windowTint = sqlite3_column_int(stmt, index++);
			veh->burstableTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;
			veh->customTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;

			veh->dirtLevel = sqlite3_column_double(stmt, index++);
			veh->fadeLevel = sqlite3_column_double(stmt, index++);

			veh->neonRGB[0] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[1] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[2] = sqlite3_column_int(stmt, index++);

			veh->neonEnablement[0] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[1] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[2] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[3] = sqlite3_column_int(stmt, index++);

			veh->tyreSmokeRGB[0] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[1] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[2] = sqlite3_column_int(stmt, index++);

			veh->convertibleRoofUp = sqlite3_column_int(stmt, index++) == 1;

			veh->dashboardColour = sqlite3_column_int(stmt, index++);
			veh->interiorColour = sqlite3_column_int(stmt, index++);

			veh->engineSound = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));

			veh->xenonColour = sqlite3_column_int(stmt, index++);

			veh->powerMultiplier = sqlite3_column_int(stmt, index++);

			results.push_back(veh);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

void ENTDatabase::populate_saved_skin(SavedSkinDBRow *entry)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "select * from ENT_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			SavedSkinComponentDBRow *comp = new SavedSkinComponentDBRow();
			//0 和 1 是 ID
			comp->slotID = sqlite3_column_int(stmt, 2);
			comp->drawable = sqlite3_column_int(stmt, 3);
			comp->texture = sqlite3_column_int(stmt, 4);
			entry->components.push_back(comp);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "select * from ENT_SKIN_PROPS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			SavedSkinPropDBRow *prop = new SavedSkinPropDBRow();
			//0 和 1 是 ID
			prop->propID = sqlite3_column_int(stmt2, 2);
			prop->drawable = sqlite3_column_int(stmt2, 3);
			prop->texture = sqlite3_column_int(stmt2, 4);
			entry->props.push_back(prop);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能获取保存的皮肤道具");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("已加载保存的皮肤完成");
	return;
}

void ENTDatabase::populate_saved_vehicle(SavedVehicleDBRow *entry) 
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "select * from ENT_VEHICLE_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			SavedVehicleExtraDBRow *extra = new SavedVehicleExtraDBRow();
			//0 和 1 是 ID
			extra->extraID = sqlite3_column_int(stmt, 2);
			extra->extraState = sqlite3_column_int(stmt, 3);
			entry->extras.push_back(extra);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的车辆附加组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "select * from ENT_VEHICLE_MODS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			SavedVehicleModDBRow *mod = new SavedVehicleModDBRow();
			//0 和 1 是 ID
			mod->modID = sqlite3_column_int(stmt2, 2);
			mod->modState = sqlite3_column_int(stmt2, 3);
			mod->isToggle = (sqlite3_column_int(stmt2, 4) == 1) ? true : false;
			entry->mods.push_back(mod);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能获取保存的车辆改装件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("保存的车辆已加载完成");
	return;
}

void ENTDatabase::delete_saved_vehicle(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_VEHICLES WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_skin(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SAVED_SKINS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的皮肤");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_vehicle(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_VEHICLES SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_propset(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_PROP_SETS SET name=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::rename_saved_skin(std::string name, sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "UPDATE ENT_SAVED_SKINS SET saveName=? WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_text(stmt, 1, name.c_str(), name.length(), 0);
		sqlite3_bind_int64(stmt, 2, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能重命名保存的皮肤");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_vehicle_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_VEHICLE_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的车辆附加组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "DELETE FROM ENT_VEHICLE_MODS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt2, 1, slot);

		// 提交
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能删除保存的车辆改装件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_skin_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_SKIN_COMPONENTS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的皮肤组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt *stmt2;
	const char *pzTest2;
	auto qStr2 = "DELETE FROM ENT_SKIN_PROPS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt2, 1, slot);

		// 提交
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能删除保存的皮肤道具");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::begin_transaction()
{
	if (has_transaction_begun == false)
	{
		has_transaction_begun = true;
		sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
	}
}
void ENTDatabase::end_transaction()
{
	if (has_transaction_begun == true)
	{
		has_transaction_begun = false;
		sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
	}
}

void ENTDatabase::mutex_lock()
{
	sqlite3_mutex_enter(db_mutex);
}

void ENTDatabase::mutex_unlock()
{
	sqlite3_mutex_leave(db_mutex);
}

void ENTDatabase::delete_saved_propset(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_PROP_SETS WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的道具集");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_saved_propset_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;
	auto qStr = "DELETE FROM ENT_PROP_INSTANCES WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除保存的道具实例");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

std::vector<SavedPropDBRow*> ENTDatabase::get_saved_prop_instances(int parentId)
{
	write_text_to_log_file("请求加载保存的道具实例");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "select * from ENT_PROP_INSTANCES";
	if (parentId != -1)
	{
		ss << " WHERE parentId = ? ";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedPropDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (parentId != -1)
		{
			sqlite3_bind_int(stmt, 1, parentId);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到道具行");

			SavedPropDBRow *prop = new SavedPropDBRow();

			int index = 0;
			prop->rowID = sqlite3_column_int(stmt, index++);
			prop->parentID = sqlite3_column_int(stmt, index++);
			prop->model = sqlite3_column_int(stmt, index++);
			prop->title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			prop->counter = sqlite3_column_int(stmt, index++);

			prop->posX = sqlite3_column_double(stmt, index++);
			prop->posY = sqlite3_column_double(stmt, index++);
			prop->posZ = sqlite3_column_double(stmt, index++);
			prop->pitch = sqlite3_column_double(stmt, index++);
			prop->roll = sqlite3_column_double(stmt, index++);
			prop->yaw = sqlite3_column_double(stmt, index++);
			
			prop->isImmovable = sqlite3_column_int(stmt, index++);
			prop->isInvincible = sqlite3_column_int(stmt, index++);
			prop->hasGravity = sqlite3_column_int(stmt, index++);

			prop->alpha = sqlite3_column_double(stmt, index++);

			results.push_back(prop);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的道具");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

std::vector<SavedPropSet*> ENTDatabase::get_saved_prop_sets(int index)
{
	write_text_to_log_file("请求加载保存的道具集");

	mutex_lock();

	sqlite3_stmt *stmt;
	const char *pzTest;

	std::stringstream ss;
	ss << "SELECT s.id, s.name, COUNT(i.id) AS size FROM ENT_PROP_SETS s LEFT JOIN ENT_PROP_INSTANCES i ON s.id = i.parentId ";
	if (index != -1)
	{
		ss << " WHERE s.id = ? ";
	}
	ss << " GROUP BY i.parentId ORDER BY s.id";

	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<SavedPropSet*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到道具集");

			SavedPropSet *set = new SavedPropSet();

			int index = 0;
			set->rowID = sqlite3_column_int(stmt, index++);
			set->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			set->dbSize = sqlite3_column_int(stmt, index++);;

			results.push_back(set);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取保存的道具集");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

void ENTDatabase::populate_saved_prop_set(SavedPropSet *entry)
{
	std::vector<SavedPropDBRow*> instances = get_saved_prop_instances(entry->rowID);
	entry->items = instances;
}

bool ENTDatabase::save_props(std::vector<SavedPropDBRow*> props, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();
	begin_transaction();

	std::stringstream ss_set;
	ss_set << "INSERT OR REPLACE INTO ENT_PROP_SETS VALUES (?, ?);";
	sqlite3_stmt *set_stmt;
	const char *pzTest;
	auto ss_set_str = ss_set.str();
	int set_rc = sqlite3_prepare_v2(db, ss_set_str.c_str(), ss_set_str.length(), &set_stmt, &pzTest);

	bool result = true;
	sqlite3_int64 newRowID;

	if (set_rc != SQLITE_OK)
	{
		write_text_to_log_file("道具集保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}
	else
	{
		int index = 1;
		if (slot == -1)
		{
			sqlite3_bind_null(set_stmt, index++);
		}
		else
		{
			sqlite3_bind_int64(set_stmt, index++, slot);
		}

		sqlite3_bind_text(set_stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
		
		// 提交
		sqlite3_step(set_stmt);
		sqlite3_finalize(set_stmt);

		newRowID = sqlite3_last_insert_rowid(db);

		//如果正在更新，则删除所有已存在的子项
		if (slot != -1)
		{
			delete_saved_propset_children(slot);;
		}
		else
		{
			slot = newRowID; //获取新的ID，用于作为子项的父级
		}
	}

	if (result)
	{
		for each (SavedPropDBRow* prop in props)
		{
			std::stringstream ss_inst;
			ss_inst << "INSERT OR REPLACE INTO ENT_PROP_INSTANCES VALUES (";
			for (int i = 0; i < 15; i++)
			{
				if (i > 0)
				{
					ss_inst << ", ";
				}
				ss_inst << "?";
			}
			ss_inst << ");";

			sqlite3_stmt *inst_stmt;
			const char *pzTest;
			auto ss_instance_str = ss_inst.str();
			int inst_rc = sqlite3_prepare_v2(db, ss_instance_str.c_str(), ss_instance_str.length(), &inst_stmt, &pzTest);;

			if (inst_rc != SQLITE_OK)
			{
				write_text_to_log_file("道具实例保存失败");
				write_text_to_log_file(sqlite3_errmsg(db));
				result = false;
			}
			else
			{
				int index = 1;

				sqlite3_bind_null(inst_stmt, index++);
				sqlite3_bind_int64(inst_stmt, index++, slot); //父级 ID
				sqlite3_bind_int(inst_stmt, index++, prop->model);
				auto propTitle = prop->title;
				sqlite3_bind_text(inst_stmt, index++, (char*)propTitle.c_str(), propTitle.length(), 0);
				sqlite3_bind_int(inst_stmt, index++, prop->counter);
				sqlite3_bind_double(inst_stmt, index++, prop->posX);
				sqlite3_bind_double(inst_stmt, index++, prop->posY);
				sqlite3_bind_double(inst_stmt, index++, prop->posZ);
				sqlite3_bind_double(inst_stmt, index++, prop->pitch);
				sqlite3_bind_double(inst_stmt, index++, prop->roll);
				sqlite3_bind_double(inst_stmt, index++, prop->yaw);
				sqlite3_bind_int(inst_stmt, index++, prop->isImmovable);
				sqlite3_bind_int(inst_stmt, index++, prop->isInvincible);
				sqlite3_bind_int(inst_stmt, index++, prop->hasGravity);
				sqlite3_bind_int(inst_stmt, index++, prop->alpha);

				sqlite3_step(inst_stmt);
				
				int finalOK = sqlite3_finalize(inst_stmt);
				if (finalOK != SQLITE_OK)
				{
					write_text_to_log_file("道具实例保存失败（最终化）");
					write_text_to_log_file(sqlite3_errmsg(db));
					result = false;
				}

				write_text_to_log_file("道具实例保存成功");
			}
		}	
	}

	end_transaction();

	mutex_unlock();

	return result;

	return false;
}

void ENTDatabase::save_tracked_extras(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 1; i < 10; i++)
	{
		if (!VEHICLE::DOES_EXTRA_EXIST(veh, i))
		{
			continue;
		}

		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_TRACKED_EXTRAS VALUES (?, ?, ?, ?)";

		sqlite3_stmt* stmt;
		const char* pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("车辆附加组件保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);
			sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(veh, i) ? 1 : 0);

			// 提交
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}
	end_transaction();

	mutex_unlock();
}

void ENTDatabase::save_tracked_mods(Vehicle veh, sqlite3_int64 rowID)
{
	mutex_lock();

	begin_transaction();

	for (int i = 0; i < 50; i++) //总共50个改装件（包括 Benny 的改装件）
	{
		std::stringstream ss;
		ss << "INSERT OR REPLACE INTO ENT_TRACKED_MODS VALUES (?, ?, ?, ?, ?)";

		sqlite3_stmt* stmt;
		const char* pzTest;
		auto ssStr = ss.str();
		int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);

		if (rc != SQLITE_OK)
		{
			write_text_to_log_file("车辆改装件保存失败");
			write_text_to_log_file(sqlite3_errmsg(db));
		}
		else
		{
			bool isToggleable = (i >= 17 && i <= 22);

			int index = 1;
			sqlite3_bind_null(stmt, index++);
			sqlite3_bind_int64(stmt, index++, rowID);
			sqlite3_bind_int(stmt, index++, i);

			if (isToggleable)
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::IS_TOGGLE_MOD_ON(veh, i) ? 1 : 0);
			}
			else
			{
				sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD(veh, i));
			}
			sqlite3_bind_int(stmt, index++, isToggleable ? 1 : 0);
			// 提交
			sqlite3_step(stmt);
			sqlite3_finalize(stmt);
		}
	}

	end_transaction();

	mutex_unlock();
}

void ENTDatabase::delete_tracked_vehicle_children(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto qStr = "DELETE FROM ENT_TRACKED_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除跟踪的车辆附加组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt* stmt2;
	const char* pzTest2;
	auto qStr2 = "DELETE FROM ENT_TRACKED_MODS WHERE id=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt2, 1, slot);

		// 提交
		sqlite3_step(stmt2);
		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能删除跟踪的车辆改装件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::delete_tracked_vehicle(sqlite3_int64 slot)
{
	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto qStr = "DELETE FROM ENT_TRACKED_VEHICLES WHERE id=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int64(stmt, 1, slot);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能删除跟踪的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();
}

void ENTDatabase::populate_tracked_vehicle(TrackedVehicleDBRow* entry)
{
	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto qStr = "select * from ENT_TRACKED_EXTRAS WHERE parentId=?";
	int rc = sqlite3_prepare_v2(db, qStr, strlen(qStr), &stmt, &pzTest);

	if (rc == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			TrackedVehicleExtraDBRow* extra = new TrackedVehicleExtraDBRow();
			//0 和 1 是 ID
			extra->extraID = sqlite3_column_int(stmt, 2);
			extra->extraState = sqlite3_column_int(stmt, 3);
			entry->extras.push_back(extra);
			r = sqlite3_step(stmt);
		}

		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取跟踪的车辆附加组件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	sqlite3_stmt* stmt2;
	const char* pzTest2;
	auto qStr2 = "select * from ENT_TRACKED_MODS WHERE parentId=?";
	int rc2 = sqlite3_prepare_v2(db, qStr2, strlen(qStr2), &stmt2, &pzTest2);

	if (rc2 == SQLITE_OK)
	{
		// 绑定值
		sqlite3_bind_int(stmt2, 1, entry->rowID);

		// 提交
		int r = sqlite3_step(stmt2);
		while (r == SQLITE_ROW)
		{
			TrackedVehicleModDBRow* mod = new TrackedVehicleModDBRow();
			//0 和 1 是 ID
			mod->modID = sqlite3_column_int(stmt2, 2);
			mod->modState = sqlite3_column_int(stmt2, 3);
			mod->isToggle = (sqlite3_column_int(stmt2, 4) == 1) ? true : false;
			entry->mods.push_back(mod);
			r = sqlite3_step(stmt2);
		}

		sqlite3_finalize(stmt2);
	}
	else
	{
		write_text_to_log_file("未能获取跟踪的车辆改装件");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	write_text_to_log_file("已完成加载跟踪的车辆");
	return;
}

std::vector<TrackedVehicleDBRow*> ENTDatabase::get_tracked_vehicles(int index)
{
	write_text_to_log_file("请求加载跟踪的车辆");

	mutex_lock();

	sqlite3_stmt* stmt;
	const char* pzTest;

	std::stringstream ss;
	ss << "select * from ENT_TRACKED_VEHICLES";
	if (index != -1)
	{
		ss << " WHERE id = ?";
	}
	auto qStr = ss.str();
	int rc = sqlite3_prepare_v2(db, qStr.c_str(), qStr.length(), &stmt, &pzTest);

	std::vector<TrackedVehicleDBRow*> results;

	if (rc == SQLITE_OK)
	{
		// 绑定值
		if (index != -1)
		{
			sqlite3_bind_int(stmt, 1, index);
		}

		int r = sqlite3_step(stmt);
		while (r == SQLITE_ROW)
		{
			write_text_to_log_file("找到车辆行");

			TrackedVehicleDBRow* veh = new TrackedVehicleDBRow();

			int index = 0;
			veh->rowID = sqlite3_column_int(stmt, index++);
			veh->saveName = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->model = sqlite3_column_int(stmt, index++);
			veh->colourPrimary = sqlite3_column_int(stmt, index++);
			veh->colourSecondary = sqlite3_column_int(stmt, index++);
			veh->colourExtraPearl = sqlite3_column_int(stmt, index++);
			veh->colourExtraWheel = sqlite3_column_int(stmt, index++);
			veh->colourMod1Type = sqlite3_column_int(stmt, index++);
			veh->colourMod1Colour = sqlite3_column_int(stmt, index++);
			veh->colourMod1P3 = sqlite3_column_int(stmt, index++);
			veh->colourMod2Type = sqlite3_column_int(stmt, index++);
			veh->colourMod2Colour = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom1RGB[2] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[0] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[1] = sqlite3_column_int(stmt, index++);
			veh->colourCustom2RGB[2] = sqlite3_column_int(stmt, index++);

			veh->livery = sqlite3_column_int(stmt, index++);
			veh->plateText = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));
			veh->plateType = sqlite3_column_int(stmt, index++);
			veh->wheelType = sqlite3_column_int(stmt, index++);
			veh->windowTint = sqlite3_column_int(stmt, index++);
			veh->burstableTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;
			veh->customTyres = sqlite3_column_int(stmt, index++) == 1 ? true : false;

			veh->dirtLevel = sqlite3_column_double(stmt, index++);
			veh->fadeLevel = sqlite3_column_double(stmt, index++);

			veh->neonRGB[0] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[1] = sqlite3_column_int(stmt, index++);
			veh->neonRGB[2] = sqlite3_column_int(stmt, index++);

			veh->neonEnablement[0] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[1] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[2] = sqlite3_column_int(stmt, index++);
			veh->neonEnablement[3] = sqlite3_column_int(stmt, index++);

			veh->tyreSmokeRGB[0] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[1] = sqlite3_column_int(stmt, index++);
			veh->tyreSmokeRGB[2] = sqlite3_column_int(stmt, index++);

			veh->convertibleRoofUp = sqlite3_column_int(stmt, index++) == 1;

			veh->dashboardColour = sqlite3_column_int(stmt, index++);
			veh->interiorColour = sqlite3_column_int(stmt, index++);

			veh->engineSound = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, index++)));

			veh->xenonColour = sqlite3_column_int(stmt, index++);

			veh->cor_x = sqlite3_column_int(stmt, index++);
			veh->cor_y = sqlite3_column_int(stmt, index++);
			veh->cor_z = sqlite3_column_int(stmt, index++);

			veh->crotation = sqlite3_column_int(stmt, index++);
			
			veh->lfuel = sqlite3_column_double(stmt, index++);

			veh->powerMultiplier = sqlite3_column_int(stmt, index++);

			results.push_back(veh);

			r = sqlite3_step(stmt);
		}
		sqlite3_finalize(stmt);
	}
	else
	{
		write_text_to_log_file("未能获取跟踪的车辆");
		write_text_to_log_file(sqlite3_errmsg(db));
	}

	mutex_unlock();

	return results;
}

bool ENTDatabase::save_tracked_vehicle(Vehicle veh, std::string saveName, sqlite3_int64 slot)
{
	mutex_lock();

	std::stringstream ss;
	ss << "INSERT OR REPLACE INTO ENT_TRACKED_VEHICLES VALUES (";
	for (int i = 0; i < 48; i++)
	{
		if (i > 0)
		{
			ss << ", ";
		}
		ss << "?";
	}
	ss << ");";

	/*
	id 整数型 主键 自增 非空, \ 1
			saveName 文本型 非空, \ 2
			model 整数型 非空, \ 3
			colourPrimary 整数型, \ 4
			colourSecondary 整数型, \ 5
			colourExtraPearl 整数型, \ 6
			colourExtraWheel 整数型, \ 7
			colourMod1Type 整数型, \ 8
			colourMod1Colour 整数型, \ 9
			colourMod1P3 整数型, \ 10
			colourMod2Type 整数型, \ 11
			colourMod2Colour 整数型, \ 12
			colourCustom1R 整数型, \ 13
			colourCustom1G 整数型, \ 14
			colourCustom1B 整数型, \ 15
			colourCustom2R 整数型, \ 16
			colourCustom2G 整数型, \ 17
			colourCustom2B 整数型, \ 18

	*/

	sqlite3_stmt* stmt;
	const char* pzTest;
	auto ssStr = ss.str();
	int rc = sqlite3_prepare_v2(db, ssStr.c_str(), ssStr.length(), &stmt, &pzTest);
	bool result = true;

	if (rc != SQLITE_OK)
	{
		write_text_to_log_file("车辆保存失败");
		write_text_to_log_file(sqlite3_errmsg(db));
		result = false;
	}
	else
	{
		int index = 1;
		if (slot == -1)
		{
			sqlite3_bind_null(stmt, index++);
		}
		else
		{
			sqlite3_bind_int64(stmt, index++, slot);
		}
		sqlite3_bind_text(stmt, index++, saveName.c_str(), saveName.length(), 0); //保存名称
		sqlite3_bind_int(stmt, index++, ENTITY::GET_ENTITY_MODEL(veh)); //模型

		int primaryCol, secondaryCol;
		VEHICLE::GET_VEHICLE_COLOURS(veh, &primaryCol, &secondaryCol);
		sqlite3_bind_int(stmt, index++, primaryCol);
		sqlite3_bind_int(stmt, index++, secondaryCol);

		int pearlCol, wheelCol;
		VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearlCol, &wheelCol);
		sqlite3_bind_int(stmt, index++, pearlCol);
		sqlite3_bind_int(stmt, index++, wheelCol);

		int mod1a, mod1b, mod1c;
		VEHICLE::GET_VEHICLE_MOD_COLOR_1(veh, &mod1a, &mod1b, &mod1c);
		sqlite3_bind_int(stmt, index++, mod1a);
		sqlite3_bind_int(stmt, index++, mod1b);
		sqlite3_bind_int(stmt, index++, mod1c);

		int mod2a, mod2b;
		VEHICLE::GET_VEHICLE_MOD_COLOR_2(veh, &mod2a, &mod2b);
		sqlite3_bind_int(stmt, index++, mod2a);
		sqlite3_bind_int(stmt, index++, mod2b);

		if (VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh))
		{
			int custR1, custG1, custB1;
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &custR1, &custG1, &custB1);
			sqlite3_bind_int(stmt, index++, custR1);
			sqlite3_bind_int(stmt, index++, custG1);
			sqlite3_bind_int(stmt, index++, custB1);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		if (VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh))
		{
			int custR2, custG2, custB2;
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &custR2, &custG2, &custB2);
			sqlite3_bind_int(stmt, index++, custR2);
			sqlite3_bind_int(stmt, index++, custG2);
			sqlite3_bind_int(stmt, index++, custB2);
		}
		else
		{
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
			sqlite3_bind_int(stmt, index++, -1);
		}

		/*
		livery 整数型, \ 19
		plateText 文本型, \ 20
		plateType 整数型, \ 21
		wheelType 整数型, \ 22
		windowTint 整数型, \ 23
		burstableTyres 整数型 \ 24
		engineSound 字符串型, \ 25
		xenonColour 整数型, \ 26
		*/

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_LIVERY(veh));

		char* plateText = VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(veh);
		sqlite3_bind_text(stmt, index++, plateText, strlen(plateText), 0);

		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WHEEL_TYPE(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_WINDOW_TINT(veh));
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_TYRES_CAN_BURST(veh) ? 1 : 0);
		sqlite3_bind_int(stmt, index++, VEHICLE::GET_VEHICLE_MOD_VARIATION(veh, 23) ? 1 : 0);

	//sqlite3_bind_text(stmt, index++, CURRSOUNDENGINE, strlen(plateText), 0);
	/*
	dirtLevel 实数型 默认值 0, \ 25
		fadeLevel 实数型 默认值 0, \ 26
		neonR 整数型 默认值 -1, \ 27
		neonG 整数型 默认值 -1, \ 28
		neonB 整数型 默认值 -1, \ 29
		neon0Enabled 整数型 默认值 0, \ 30
		neon1Enabled 整数型 默认值 0, \ 21
		neon2Enabled 整数型 默认值 0, \ 32
		neon3Enabled 整数型 默认值 0 \*/

		sqlite3_bind_double(stmt, index++, VEHICLE::GET_VEHICLE_DIRT_LEVEL(veh));
		sqlite3_bind_double(stmt, index++, VEHICLE::GET_VEHICLE_ENVEFF_SCALE(veh));

		int neonR, neonG, neonB;
		VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(veh, &neonR, &neonG, &neonB);
		sqlite3_bind_int(stmt, index++, neonR);
		sqlite3_bind_int(stmt, index++, neonG);
		sqlite3_bind_int(stmt, index++, neonB);

		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 0));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 1));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 2));
		sqlite3_bind_int(stmt, index++, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(veh, 3));

		int tyreSmokeR, tyreSmokeG, tyreSmokeB;
		VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(veh, &tyreSmokeR, &tyreSmokeG, &tyreSmokeB);
		sqlite3_bind_int(stmt, index++, tyreSmokeR);
		sqlite3_bind_int(stmt, index++, tyreSmokeG);
		sqlite3_bind_int(stmt, index++, tyreSmokeB);

		sqlite3_bind_int(stmt, index++, VEHICLE::IS_VEHICLE_A_CONVERTIBLE(veh, 0) && VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh));

		/*仪表板颜色 整数型
		内饰颜色 整数型*/
		int dashCol, interiorCol;
		VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &dashCol);
		VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &interiorCol);
		sqlite3_bind_int(stmt, index++, dashCol);
		sqlite3_bind_int(stmt, index++, interiorCol);

		current_picked_engine_sound = "";
		if (!VEHICLES_HAVE_SOUND.empty()) {
			for (int i = 0; i < VEHICLES_HAVE_SOUND.size(); i++) {
				if (veh == VEHICLES_HAVE_SOUND[i]) current_picked_engine_sound = VEHICLES_SOUND_NUMBER[i];
			}
		}
		char* tmp_i = (char*)current_picked_engine_sound.c_str();
		sqlite3_bind_text(stmt, index++, tmp_i, strlen(tmp_i), 0);
		current_picked_engine_sound = "";

		int xenonColour = -1;
		if (getGameVersion() > 45) xenonColour = VEHICLE::GET_VEHICLE_XENON_COLOUR(veh);
		sqlite3_bind_int(stmt, index++, xenonColour);

		int cor_x, cor_y, cor_z;
		cor_x = ENTITY::GET_ENTITY_COORDS(veh, true).x;
		sqlite3_bind_int(stmt, index++, cor_x);
		cor_y = ENTITY::GET_ENTITY_COORDS(veh, true).y;
		sqlite3_bind_int(stmt, index++, cor_y);
		cor_z = ENTITY::GET_ENTITY_COORDS(veh, true).z;
		sqlite3_bind_int(stmt, index++, cor_z);

		int crotation;
		crotation = ENTITY::GET_ENTITY_HEADING(veh);
		sqlite3_bind_int(stmt, index++, crotation);

		float lfuel = -1;
		if (featureFuel && !VEHICLES.empty()) {
			for (int k = 0; k < VEHICLES.size(); k++) {
				if (VEHICLES[k] == veh) {
					lfuel = FUEL[k];
				}
			}
		}
		sqlite3_bind_double(stmt, index++, lfuel);

		int powerMultiplier = -1;
		for (int kl = 0; kl < C_ENGINE_M.size(); kl++) {
			if (C_ENGINE_VEHICLE[kl] == veh) {
				powerMultiplier = C_ENGINE_M[kl];
			}
		}
		sqlite3_bind_int(stmt, index++, powerMultiplier);

		// 提交
		sqlite3_step(stmt);
		sqlite3_finalize(stmt);

		sqlite3_int64 newRowID = sqlite3_last_insert_rowid(db);

		//如果正在更新，则删除所有已存在的子项
		if (slot != -1)
		{
			delete_tracked_vehicle_children(slot);
		}

		save_tracked_extras(veh, newRowID);
		save_tracked_mods(veh, newRowID);
	}

	mutex_unlock();

	return result;
}