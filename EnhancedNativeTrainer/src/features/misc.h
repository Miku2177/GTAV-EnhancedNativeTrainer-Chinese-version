/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\io\io.h"
#include "..\io\config_io.h"
#include "..\ui_support\menu_functions.h"
#include "..\debug\debuglog.h"
#include "..\storage\database.h"
/* From Zorg93's "EnableMPCars" code */
#include <cstddef>

extern bool mouse_view_control;
extern bool help_showing;
extern bool frozen_time;

void process_misc_menu();

void reset_misc_globals();

void update_misc_features(BOOL playerExists, Ped playerPed);

void add_misc_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results);

void add_misc_generic_settings(std::vector<StringPairSettingDBRow>* results);

void handle_generic_settings_misc(std::vector<StringPairSettingDBRow>* settings);

void onchange_misc_phone_bill_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_default_index(int value, SelectFromListMenuItem* source);

void onchange_misc_def_menutab_index(int value, SelectFromListMenuItem* source);

void onchange_misc_phone_freeseconds_index(int value, SelectFromListMenuItem* source);

bool is_player_reset_on_death();

bool is_input_blocked_in_menu();

bool is_vehicle_preview_enabled();

bool is_controller_ignored_in_trainer();

bool is_hud_hidden();

void set_hud_hidden(bool hidden);

bool is_jellman_scenery_enabled();

struct ScriptHeader
{
	char padding1[16];					//0x0
	unsigned char** codeBlocksOffset;	//0x10
	char padding2[4];					//0x18
	int codeLength;						//0x1C
	char padding3[4];					//0x20
	int localCount;						//0x24
	char padding4[4];					//0x28
	int nativeCount;					//0x2C
	__int64* localOffset;				//0x30
	char padding5[8];					//0x38
	__int64* nativeOffset;				//0x40
	char padding6[16];					//0x48
	int nameHash;						//0x58
	char padding7[4];					//0x5C
	char* name;							//0x60
	char** stringsOffset;				//0x68
	int stringSize;						//0x70
	char padding8[12];					//0x74
										//END_OF_HEADER

	bool IsValid() const { return codeLength > 0; }
	int CodePageCount() const { return (codeLength + 0x3FFF) >> 14; } //Exception = codeLength is nullPtr
	int GetCodePageSize(int page) const
	{
		return (page < 0 || page >= CodePageCount() ? 0 : (page == CodePageCount() - 1) ? codeLength & 0x3FFF : 0x4000);
	}
	unsigned char* GetCodePageAddress(int page) const { return codeBlocksOffset[page]; }
	unsigned char* GetCodePositionAddress(int codePosition) const
	{
		return codePosition < 0 || codePosition >= codeLength ? NULL : &codeBlocksOffset[codePosition >> 14][codePosition & 0x3FFF];
	}
	char* GetString(int stringPosition)const
	{
		return stringPosition < 0 || stringPosition >= stringSize ? NULL : &stringsOffset[stringPosition >> 14][stringPosition & 0x3FFF];
	}

};

struct ScriptTableItem
{
	ScriptHeader* Header;
	char padding[4];
	int hash;

	inline bool IsLoaded() const
	{
		return Header != NULL;
	}
};

struct ScriptTable
{
	ScriptTableItem* TablePtr;
	char padding[16];
	int count;
	ScriptTableItem* FindScript(int hash)
	{
		if (TablePtr == NULL)
		{
			return NULL;//table initialisation hasnt happened yet
		}
		for (int i = 0; i<count; i++)
		{
			if (TablePtr[i].hash == hash)
			{
				return &TablePtr[i];
			}
		}
		return NULL;
	}
};

struct GlobalTable
{
	__int64** GlobalBasePtr;
	__int64* AddressOf(int index) const { return &GlobalBasePtr[index >> 18 & 0x3F][index & 0x3FFFF]; }
	bool IsInitialised()const { return *GlobalBasePtr != NULL; }
};

void FindScriptAddresses();

void FindPatterns();

uintptr_t FindPattern2(const char *pattern, const char *mask, const char* startAddress, size_t size);

uintptr_t FindPattern2(const char *pattern, const char *mask);

void EnableCarsGlobal();