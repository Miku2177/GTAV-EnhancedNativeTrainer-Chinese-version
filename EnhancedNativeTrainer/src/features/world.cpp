/*
Some of this code began its life as a part of GTA V SCRIPT HOOK SDK.
http://dev-c.com
(C) Alexander Blade 2015

It is now part of the Enhanced Native Trainer project.
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham and fellow contributors 2015
*/


#include "world.h"
#include "area_effect.h"
#include "script.h"
#include "..\ui_support\menu_functions.h"
#include <Psapi.h>
#include <Windows.h>

/* Begin Snow related globals */
bool VehicleTracks;
bool PedTracks;
bool VehicleTrackDepth;
bool PedTrackDepth;
bool IsSnow;
/* End snow related globals*/

int activeLineIndexWorld = 0;
int activeLineIndexWeather = 0;
int activeLineIndexClouds = 0;

int r, g, b = -1;

int acid_counter, acid_counter_p = -1;

Camera DeathCam = NULL;

bool featureRestrictedZones = true;

bool featureWorldMoonGravity = false;
//bool featureWorldMoonGravityUpdated = false;

bool featureWorldNoPeds = false;
//bool featureWorldNoPedsUpdated = false;
bool featureWorldNoTraffic = false;
//bool featureWorldNoTrafficUpdated = false;
bool featureNoPoliceBlips = false;
//bool featureNoPoliceBlipsUpdated = false;
bool featureWorldNoAnimals = false;
//bool featureWorldNoAnimalsUpdated = false;
bool featureFullMap = false;
//bool featureFullMapUpdated = false;
bool featurePenitentiaryMap = false;
//bool featurePenitentiaryMapUpdated = false;
bool featureZancudoMap = false;
bool featureZancudoMapUpdated = false;
bool featureBusLight = false;
//bool featureBusLightUpdated = false;
bool featureAcidWater = false;
//bool featureAcidWaterUpdated = false;
bool featureAcidRain = false;
//bool featureAcidRainUpdated = false;
bool featureReducedGripVehiclesIfSnow = false;
//bool featureReducedGripVehiclesIfSnowUpdated = false;

bool police_blips_toogle = false;
bool windstrength_toggle = false;
int windstrength_changed = -1;

bool featureWorldRandomCops = true;
bool featureWorldRandomTrains = true;
bool featureWorldRandomBoats = true;
bool featureWorldGarbageTrucks = true;

bool featureWorldRandomCopsUpdated = false;
bool featureWorldRandomTrainsUpdated = false;
bool featureWorldRandomBoatsUpdated = false;
bool featureWorldGarbageTrucksUpdated = false;

bool featureBlackout = false;
bool featureBlackoutUpdated = false;

//bool featureWeatherWind = false;
bool featureWeatherFreeze = false;
bool featureCloudsFreeze = false;
bool featureCloudsNo = false;

bool featureSnow = false;
bool featureSnowUpdated = false;

bool featureMPMap = false;
bool featureMPMapUpdated = false;
bool radar_map_toogle_1, radar_map_toogle_2, radar_map_toogle_3 = false;

std::string lastWeather;
std::string lastWeatherName;

std::string lastClouds;
std::string lastCloudsName;

// Radar Map Size
const std::vector<std::string> WORLD_RADAR_MAP_CAPTIONS{ "Normal", "Big", "Full" };
const int WORLD_RADAR_MAP_VALUES[] = { 1, 2, 3 };
//const std::vector<int> WORLD_RADAR_MAP_VALUES{ 1, 2, 3 };
int RadarMapIndex = 0;
bool RadarMapChanged = true;

// Wind Strength
const std::vector<std::string> WORLD_WIND_STRENGTH_CAPTIONS{ "Calm", "Gentle Breeze", "Strong Breeze" };
const int WORLD_WIND_STRENGTH_VALUES[] = { 0, 3, 999 };
//const std::vector<int> WORLD_WIND_STRENGTH_VALUES{ 0, 3, 999 };
int WindStrengthIndex = 0;
bool WindStrengthChanged = true;

// Reduced Grip If Heavy Snow
const std::vector<std::string> WORLD_REDUCEDGRIP_SNOWING_CAPTIONS{ "OFF", "Simulation", "Arcade" };
const int WORLD_REDUCEDGRIP_SNOWING_VALUES[] = { 0, 1, 2 };
//const std::vector<int> WORLD_REDUCEDGRIP_SNOWING_VALUES{ 0, 1, 2 };
int RadarReducedGripSnowingIndex = 0;
bool RadarReducedGripSnowingChanged = true;

bool onconfirm_weather_menu(MenuItem<std::string> choice)
{
	std::stringstream ss; ss << "Weather Frozen at: " << lastWeatherName;
	switch (choice.currentMenuIndex)
	{
	case 0:
		// set weather
		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		if (featureWeatherFreeze && !lastWeather.empty())
		{
			std::stringstream ss; ss << "Weather frozen at: " << lastWeatherName;
			set_status_text(ss.str());
		}
		else if (!featureWeatherFreeze)
		{
			set_status_text("Weather unfrozen");
		}
		else
		{
			set_status_text("Set a weather value first");
			featureWeatherFreeze = false;
		}
		break;
	case 1: 
		// reset weather
		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
		GAMEPLAY::SET_WEATHER_TYPE_NOW("EXTRASUNNY");
		GRAPHICS::_CLEAR_CLOUD_HAT();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();
		break;
	default:
		lastWeather = choice.value.c_str();
		lastWeatherName = choice.caption;

		GAMEPLAY::CLEAR_OVERRIDE_WEATHER();
		GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		GAMEPLAY::SET_WEATHER_TYPE_NOW((char *)lastWeather.c_str());

		GRAPHICS::CLEAR_TIMECYCLE_MODIFIER();

		std::ostringstream ss2;
		ss2 << "Weather: " << choice.caption;
		set_status_text(ss2.str());
	}

	return false;
}

void process_weather_menu()
{
	const int lineCount = 17;

	std::string caption = "Weather Options";
	
	StringStandardOrToggleMenuDef lines[lineCount] = {
		{ "Freeze Weather", "FREEZEWEATHER", &featureWeatherFreeze, NULL },
		{ "Reset Weather", "RESETWEATHER", NULL, NULL },
		{ "Extra Sunny", "EXTRASUNNY", NULL, NULL },
		{ "Clear", "CLEAR", NULL, NULL },
		{ "Cloudy", "CLOUDS", NULL, NULL },
		{ "Smog", "SMOG", NULL, NULL },
		{ "Foggy", "FOGGY", NULL, NULL },
		{ "Overcast", "OVERCAST", NULL, NULL },
		{ "Rain", "RAIN", NULL, NULL },
		{ "Stormy", "THUNDER", NULL, NULL },
		{ "Clearing", "CLEARING", NULL, NULL },
		{ "Neutral", "NEUTRAL", NULL, NULL },
		{ "Snow", "SNOW", NULL, NULL },
		{ "Blizzard", "BLIZZARD", NULL, NULL },
		{ "Light Snow", "SNOWLIGHT", NULL, NULL },
		{ "Christmas", "XMAS", NULL, NULL },
		{ "Halloween", "HALLOWEEN", NULL, NULL }
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexWeather, caption, onconfirm_weather_menu);
}
//////////////////////////////////// CLOUDS MENU /////////////////////////////

bool onconfirm_clouds_menu(MenuItem<std::string> choice)
{
	std::stringstream ss; ss << "Clouds Frozen at: " << lastCloudsName;
	switch (choice.currentMenuIndex)
	{
	case 0:
		// Set Weather
		if (featureCloudsFreeze && !lastClouds.empty())
		{
			std::stringstream ss; ss << "Clouds frozen at: " << lastCloudsName;
			set_status_text(ss.str());
		}
		else if (!featureCloudsFreeze)
		{
			set_status_text("Clouds unfrozen");
		}
		else
		{
			set_status_text("Set a clouds value first");
			featureCloudsFreeze = false;
		}
		break;
	case 1:
		// No Clouds
		GRAPHICS::_CLEAR_CLOUD_HAT();
		break;
	default:
		lastClouds = choice.value.c_str();
		lastCloudsName = choice.caption;

		GRAPHICS::_SET_CLOUD_HAT_TRANSITION((char *)lastClouds.c_str(), 1.0);
				
		std::ostringstream ss2;
		ss2 << "Clouds: " << choice.caption;
		set_status_text(ss2.str());
	}

	return false;
}

void process_clouds_menu()
{
	const int lineCount = 22;

	std::string caption = "Clouds Options";

	StringStandardOrToggleMenuDef lines[lineCount] = {
		{ "Freeze Clouds", "FREEZECLOUDS", &featureCloudsFreeze, NULL },
		{ "No Clouds", "NOCLOUDS", NULL, NULL },
		//
		{ "Altostratus", "altostratus", NULL, NULL },
		{ "Cirrocumulus", "cirrocumulus", NULL, NULL },
		{ "Cirrus", "Cirrus", NULL, NULL },
		{ "Clear 01", "Clear 01", NULL, NULL },
		{ "Cloudy 01", "Cloudy 01", NULL, NULL },
		{ "Contrails", "Contrails", NULL, NULL },
		{ "Horizon", "Horizon", NULL, NULL },
		{ "Horizonband 1", "horizonband1", NULL, NULL },
		{ "Horizonband 2", "horizonband2", NULL, NULL },
		{ "Horizonband 3", "horizonband3", NULL, NULL },
		{ "Horsey", "horsey", NULL, NULL },
		{ "Nimbus", "Nimbus", NULL, NULL },
		{ "Puffs", "Puffs", NULL, NULL },
		{ "Rain", "RAIN", NULL, NULL },
		{ "Shower", "shower", NULL, NULL },
		{ "Snowy 01", "Snowy 01", NULL, NULL },
		{ "Stormy 01", "Stormy 01", NULL, NULL },
		{ "Stratoscumulus", "stratoscumulus", NULL, NULL },
		{ "Stripey", "Stripey", NULL, NULL },
		{ "Wispy", "Wispy", NULL, NULL },
		//
	
	};

	draw_menu_from_struct_def(lines, lineCount, &activeLineIndexClouds, caption, onconfirm_clouds_menu);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void onchange_world_radar_map_index(int value, SelectFromListMenuItem* source){
	RadarMapIndex = value;
	RadarMapChanged = true;
}

void onchange_world_wind_strength_index(int value, SelectFromListMenuItem* source){
	WindStrengthIndex = value;
	WindStrengthChanged = true;
}

void onchange_world_reducedgrip_snowing_index(int value, SelectFromListMenuItem* source) {
	RadarReducedGripSnowingIndex = value;
	RadarReducedGripSnowingChanged = true;
}

bool onconfirm_world_menu(MenuItem<int> choice)
{
	switch (choice.value)
	{
	case -1:
		process_areaeffect_menu();
		break;
	case -2:
		process_weather_menu();
		break;
	case -3:
		process_clouds_menu();
		break;
	case 2:
		// featureWorldRandomCops being set in update_features
		break;
	}
	return false;
}

void process_world_menu()
{
	const int lineCount = 8; // Amount of cases +1

	std::string caption = "World Options";

	std::vector<MenuItem<int>*> menuItems;

	SelectFromListMenuItem *listItem;

	MenuItem<int> *areaItem = new MenuItem<int>();
	areaItem->isLeaf = false;
	areaItem->caption = "Area Effects";
	areaItem->value = -1;
	menuItems.push_back(areaItem);

	MenuItem<int> *weatherItem = new MenuItem<int>();
	weatherItem->isLeaf = false;
	weatherItem->caption = "Weather";
	weatherItem->value = -2;
	menuItems.push_back(weatherItem);

	MenuItem<int> *cloudsItem = new MenuItem<int>();
	cloudsItem->isLeaf = false;
	cloudsItem->caption = "Clouds";
	cloudsItem->value = -3;
	menuItems.push_back(cloudsItem);
	
	listItem = new SelectFromListMenuItem(WORLD_WIND_STRENGTH_CAPTIONS, onchange_world_wind_strength_index);
	listItem->wrap = false;
	listItem->caption = "Wind Strength";
	listItem->value = WindStrengthIndex;
	menuItems.push_back(listItem);

	ToggleMenuItem<int> *togItem = new ToggleMenuItem<int>();
	togItem->caption = "Moon Gravity";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldMoonGravity;
	//togItem->toggleValueUpdated = &featureWorldMoonGravityUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "No Pedestrians";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldNoPeds;
	//togItem->toggleValueUpdated = &featureWorldNoPedsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "No Traffic";
	togItem->value = 1;
	togItem->toggleValue = &featureWorldNoTraffic;
	//togItem->toggleValueUpdated = &featureWorldNoTrafficUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "No Police Blips";
	togItem->value = 1;
	togItem->toggleValue = &featureNoPoliceBlips;
	//togItem->toggleValueUpdated = &featureNoPoliceBlipsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Cops";
	togItem->value = 2;
	togItem->toggleValue = &featureWorldRandomCops;
	togItem->toggleValueUpdated = &featureWorldRandomCopsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Trains";
	togItem->value = 3;
	togItem->toggleValue = &featureWorldRandomTrains;
	togItem->toggleValueUpdated = &featureWorldRandomTrainsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Random Boats";
	togItem->value = 4;
	togItem->toggleValue = &featureWorldRandomBoats;
	togItem->toggleValueUpdated = &featureWorldRandomBoatsUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Garbage Trucks";
	togItem->value = 5;
	togItem->toggleValue = &featureWorldGarbageTrucks;
	togItem->toggleValueUpdated = &featureWorldGarbageTrucksUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Restricted Zones";
	togItem->value = 6;
	togItem->toggleValue = &featureRestrictedZones;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Blackout";
	togItem->value = 6;
	togItem->toggleValue = &featureBlackout;
	togItem->toggleValueUpdated = &featureBlackoutUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Heavy Snow";
	togItem->value = 7;
	togItem->toggleValue = &featureSnow;
	togItem->toggleValueUpdated = &featureSnowUpdated;
	menuItems.push_back(togItem);

	listItem = new SelectFromListMenuItem(WORLD_REDUCEDGRIP_SNOWING_CAPTIONS, onchange_world_reducedgrip_snowing_index);
	listItem->wrap = false;
	listItem->caption = "Reduced Grip If Snowing";
	listItem->value = RadarReducedGripSnowingIndex;
	menuItems.push_back(listItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Show Full Map";
	togItem->value = 1;
	togItem->toggleValue = &featureFullMap;
	//togItem->toggleValueUpdated = &featureFullMapUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Show Bolingbroke Penitentiary On Map";
	togItem->value = 1;
	togItem->toggleValue = &featurePenitentiaryMap;
	//togItem->toggleValueUpdated = &featurePenitentiaryMapUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Show Fort Zancudo On Map";
	togItem->value = 1;
	togItem->toggleValue = &featureZancudoMap;
	togItem->toggleValueUpdated = &featureZancudoMapUpdated;
	menuItems.push_back(togItem);

	listItem = new SelectFromListMenuItem(WORLD_RADAR_MAP_CAPTIONS, onchange_world_radar_map_index);
	listItem->wrap = false;
	listItem->caption = "Radar Map Size";
	listItem->value = RadarMapIndex;
	menuItems.push_back(listItem); 

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Load Online Map";
	togItem->value = 8;
	togItem->toggleValue = &featureMPMap;
	togItem->toggleValueUpdated = &featureMPMapUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Bus Interior Light On At Night";
	togItem->value = 1;
	togItem->toggleValue = &featureBusLight;
	//togItem->toggleValueUpdated = &featureBusLightUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Deadly Water";
	togItem->value = 1;
	togItem->toggleValue = &featureAcidWater;
	//togItem->toggleValueUpdated = &featureAcidWaterUpdated;
	menuItems.push_back(togItem);

	togItem = new ToggleMenuItem<int>();
	togItem->caption = "Deadly Rain";
	togItem->value = 1;
	togItem->toggleValue = &featureAcidRain;
	//togItem->toggleValueUpdated = &featureAcidRainUpdated;
	menuItems.push_back(togItem);

	draw_generic_menu<int>(menuItems, &activeLineIndexWorld, caption, onconfirm_world_menu, NULL, NULL);
}

void reset_world_globals()
{
	activeLineIndexWorld = 0;
	activeLineIndexWeather = 0;
	activeLineIndexClouds = 0;
	RadarMapIndex = 0;
	RadarReducedGripSnowingIndex = 0;
	WindStrengthIndex = 0;
	lastWeather.clear();
	lastWeatherName.clear();
	lastClouds.clear();
	lastCloudsName.clear();

	featureWeatherFreeze =
	featureCloudsNo =
	featureCloudsFreeze =
	featureWorldMoonGravity = false;

	featureWorldNoPeds = false;
	featureWorldNoTraffic = false;
	featureNoPoliceBlips = false;
	featureWorldNoAnimals = false;
	featureFullMap = false;
	featurePenitentiaryMap = false;
	featureZancudoMap = false;
	featureBusLight = false;
	featureAcidWater = false;
	featureAcidRain = false;
	featureReducedGripVehiclesIfSnow = false;
	featureBlackout = false;
	featureSnow = false;
	featureMPMap = false;

	
	featureWorldRandomCops =
		featureWorldRandomTrains =
		featureWorldRandomBoats =
		featureWorldGarbageTrucks = true;

	//featureWorldNoPedsUpdated = 
	//featureWorldMoonGravityUpdated = 
	//featureWorldNoTrafficUpdated = 
	//featureNoPoliceBlipsUpdated =
	//featureWorldNoAnimalsUpdated =
	//featureFullMapUpdated =
	//featurePenitentiaryMapUpdated =
	featureZancudoMapUpdated =
	//featureBusLightUpdated = 
	//featureAcidWaterUpdated = 
	//featureAcidRainUpdated =
	//featureReducedGripVehiclesIfSnowUpdated =
	featureWorldGarbageTrucksUpdated =
	featureWorldRandomBoatsUpdated =
	featureWorldRandomCopsUpdated =
	featureWorldRandomTrainsUpdated =
	featureBlackoutUpdated =
	featureSnowUpdated =
	featureMPMapUpdated = true;
}

void update_world_features()
{
	if (featureWorldMoonGravity)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(1);
	}
	else //if (featureWorldMoonGravityUpdated)
	{
		GAMEPLAY::SET_GRAVITY_LEVEL(0);
	}

	if (featureBlackoutUpdated)
	{
		GRAPHICS::_SET_BLACKOUT(featureBlackout);
		featureBlackoutUpdated = false;
	}

	if (featureWorldRandomCopsUpdated)
	{
		PED::SET_CREATE_RANDOM_COPS(featureWorldRandomCops);
		featureWorldRandomCopsUpdated = false;
	}

	if (featureWorldRandomTrainsUpdated)
	{
		VEHICLE::SET_RANDOM_TRAINS(featureWorldRandomTrains);
		featureWorldRandomTrainsUpdated = false;
	}

	if (featureWorldRandomBoatsUpdated)
	{
		VEHICLE::SET_RANDOM_BOATS(featureWorldRandomBoats);
		featureWorldRandomBoatsUpdated = false;
	}

	if (featureWorldGarbageTrucksUpdated)
	{
		VEHICLE::SET_GARBAGE_TRUCKS(featureWorldGarbageTrucks);
		featureWorldGarbageTrucksUpdated = false;
	}

	/*
	if (featureBlackout)
	{
	if (STREAMING::IS_IPL_ACTIVE("DT1_22_bldg2"))
	{
	set_status_text("DT1_22_bldg2");
	STREAMING::REMOVE_IPL("DT1_22_bldg2");
	}

	GRAPHICS::_0x1600FD8CF72EBC12(0.001);
	GRAPHICS::_0x1A8E2C8B9CF4549C("lab_none_dark", "lab_none_dark_OVR");
	GRAPHICS::_0x1A8E2C8B9CF4549C("lab_none_exit", "lab_none_exit_OVR");

	for (int i = 0; i <= 35; i++)
	{
	std::ostringstream ss;
	ss << "distlodlights_medium";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}
	for (int i = 0; i <= 61; i++)
	{
	std::ostringstream ss;
	ss << "distlodlights_small";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}
	for (int i = 0; i <= 35; i++)
	{
	std::ostringstream ss;
	ss << "lodlights_medium";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}
	for (int i = 0; i <= 61; i++)
	{
	std::ostringstream ss;
	ss << "lodlights_small";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}

	for (int i = 0; i <= 0; i++)
	{
	std::ostringstream ss;
	ss << "lodlights_large";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}

	for (int i = 0; i <= 0; i++)
	{
	std::ostringstream ss;
	ss << "distlodlights_large";
	ss << std::setfill('0') << std::setw(3) << i;
	auto scenery = ss.str().c_str();
	if (STREAMING::IS_IPL_ACTIVE(scenery))
	{
	set_status_text(ss.str());
	STREAMING::REMOVE_IPL(scenery);
	}
	}
	}*/

	//if (featureWorldNoPedsUpdated)
	//{
		if (featureWorldNoPeds)
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_PEDS(v3.x, v3.y, v3.z, 1000.0, 0);
			STREAMING::SET_PED_POPULATION_BUDGET(0);
		}
		else
		{
			STREAMING::SET_PED_POPULATION_BUDGET(3);
		}

		//featureWorldNoPedsUpdated = false;
	//}
	if (featureWorldNoPeds && get_frame_number() % 100 == 50) // else 
	{
		Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		GAMEPLAY::CLEAR_AREA_OF_PEDS(v3.x, v3.y, v3.z, 1000.0, 0);
		STREAMING::SET_PED_POPULATION_BUDGET(0);
	}

	// No Police Blips
	if (featureNoPoliceBlips) PLAYER::SET_POLICE_RADAR_BLIPS(false);
	if (!featureNoPoliceBlips) PLAYER::SET_POLICE_RADAR_BLIPS(true);
	
	// Show Full Map
	if (featureFullMap) UI::_SET_MINIMAP_REVEALED(true); 
	if (!featureFullMap) UI::_SET_MINIMAP_REVEALED(false);
	
	// Radar Map Size
	if (WORLD_RADAR_MAP_VALUES[RadarMapIndex] == 1 && radar_map_toogle_1 == false) {
		UI::_SET_RADAR_BIGMAP_ENABLED(false, false);
		radar_map_toogle_1 = true;
		radar_map_toogle_2 = false;
		radar_map_toogle_3 = false;
	}

	if (WORLD_RADAR_MAP_VALUES[RadarMapIndex] == 2 && radar_map_toogle_2 == false) {
		UI::_SET_RADAR_BIGMAP_ENABLED(true, false);
		radar_map_toogle_1 = false;
		radar_map_toogle_2 = true;
		radar_map_toogle_3 = false;
	}

	if (WORLD_RADAR_MAP_VALUES[RadarMapIndex] == 3 && radar_map_toogle_3 == false) {
		UI::_SET_RADAR_BIGMAP_ENABLED(true, true);
		radar_map_toogle_1 = false;
		radar_map_toogle_2 = false;
		radar_map_toogle_3 = true;
	}
	
	// Show Bolingbroke Penitentiary On Map
	if (featurePenitentiaryMap) {
		UI::SET_RADAR_AS_INTERIOR_THIS_FRAME(GAMEPLAY::GET_HASH_KEY("V_FakePrison"), 1700, 2580, 0, 0);
		UI::SET_RADAR_AS_EXTERIOR_THIS_FRAME();
	}

	// Show Fort Zancudo On Map
	if (featureZancudoMap && featureZancudoMapUpdated == true) {
		UI::SET_MINIMAP_COMPONENT(15, true, -1);
		featureZancudoMapUpdated = false;
	}
	if (!featureZancudoMap) {
		UI::SET_MINIMAP_COMPONENT(15, false, -1);
		featureZancudoMapUpdated = true;
	}
	
	if (featureFirstPersonDeathCamera) FirstPersonDeathCamera();
	
	// Bus Interior Light On At Night && NPC No Lights && NPC Neon Lights && NPC Dirty Vehicles && NPC Damaged Vehicles && NPC No Gravity Vehicles && NPC Vehicles Reduced Grip && NPC Vehicle Speed
	if (featureBusLight || featureNPCNoLights || featureNPCNeonLights || featureDirtyVehicles || WORLD_DAMAGED_VEHICLES_VALUES[DamagedVehiclesIndex] > 0 || featureNPCNoGravityVehicles || featureNPCReducedGripVehicles ||
		WORLD_NPC_VEHICLESPEED_VALUES[NPCVehicleSpeedIndex] > 0 || WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] > 0) {
		Vehicle veh_mycurrveh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		const int BUS_ARR_SIZE = 1024;
		Vehicle bus_veh[BUS_ARR_SIZE];
		int found = worldGetAllVehicles(bus_veh, BUS_ARR_SIZE);
			
		for (int i = 0; i < found; i++) {
			if (featureBusLight) {
				Hash currVehModel = ENTITY::GET_ENTITY_MODEL(bus_veh[i]);
				Vector3 coords_vehicle = ENTITY::GET_ENTITY_COORDS(bus_veh[i], true);
				if (currVehModel == GAMEPLAY::GET_HASH_KEY("BUS") || currVehModel == GAMEPLAY::GET_HASH_KEY("RENTALBUS") || currVehModel == GAMEPLAY::GET_HASH_KEY("TOURBUS") || currVehModel == GAMEPLAY::GET_HASH_KEY("COACH") ||
					currVehModel == GAMEPLAY::GET_HASH_KEY("AIRBUS")) {
					BOOL lightsOn = -1;
					BOOL highbeamsOn = -1;
					VEHICLE::GET_VEHICLE_LIGHTS_STATE(bus_veh[i], &lightsOn, &highbeamsOn);
					if (lightsOn || highbeamsOn) {
						int bone1_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "window_lf");
						Vector3 bone1_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone1_index);
						int bone2_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "window_rf");
						Vector3 bone2_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone2_index);
						int bone3_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "window_lr");
						Vector3 bone3_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone3_index);
						int bone4_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "window_rr");
						Vector3 bone4_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone4_index);
						int bone7_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_dside_f");
						Vector3 bone7_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone7_index);
						int bone8_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_dside_r");
						Vector3 bone8_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone8_index);
						int bone9_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_dside_r1");
						Vector3 bone9_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone9_index);
						int bone10_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_dside_r2");
						Vector3 bone10_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone10_index);
						int bone15_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_dside_r7");
						Vector3 bone15_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone15_index);
						int bone16_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_f");
						Vector3 bone16_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone16_index);
						int bone17_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r");
						Vector3 bone17_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone17_index);
						int bone18_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r1");
						Vector3 bone18_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone18_index);
						int bone19_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r2");
						Vector3 bone19_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone19_index);
						int bone22_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r5");
						Vector3 bone22_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone22_index);
						int bone23_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r6");
						Vector3 bone23_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone23_index);
						int bone24_index = ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(bus_veh[i], "seat_pside_r7");
						Vector3 bone24_coord = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(bus_veh[i], bone24_index);
						float dirVector_lf_lr_x = bone3_coord.x - bone1_coord.x;
						float dirVector_lf_lr_y = bone3_coord.y - bone1_coord.y;
						float dirVector_lf_lr_z = bone3_coord.z - bone1_coord.z;
						if (currVehModel != GAMEPLAY::GET_HASH_KEY("COACH")) GRAPHICS::DRAW_SPOT_LIGHT(bone1_coord.x, bone1_coord.y, bone1_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 1.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone2_coord.x, bone2_coord.y, bone2_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 1.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone3_coord.x, bone3_coord.y, bone1_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 1.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone4_coord.x, bone4_coord.y, bone4_coord.z, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 1.0, 400, 50, 930, 900);
						if (currVehModel != GAMEPLAY::GET_HASH_KEY("COACH")) GRAPHICS::DRAW_SPOT_LIGHT(bone7_coord.x, bone7_coord.y, bone7_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone8_coord.x, bone8_coord.y, bone8_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone9_coord.x, bone9_coord.y, bone9_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone10_coord.x, bone10_coord.y, bone10_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone15_coord.x, bone15_coord.y, bone15_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone16_coord.x, bone16_coord.y, bone16_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone17_coord.x, bone17_coord.y, bone17_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone18_coord.x, bone18_coord.y, bone18_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone19_coord.x, bone19_coord.y, bone19_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone22_coord.x, bone22_coord.y, bone22_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone23_coord.x, bone23_coord.y, bone23_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						GRAPHICS::DRAW_SPOT_LIGHT(bone24_coord.x, bone24_coord.y, bone24_coord.z + 1, dirVector_lf_lr_x, dirVector_lf_lr_y, dirVector_lf_lr_z, 255, 255, 255, 2.0, 400, 50, 930, 900);
						VEHICLE::SET_VEHICLE_INTERIORLIGHT(bus_veh[i], true);
					}
				}
			}
			if (featureNPCNoLights && bus_veh[i] != veh_mycurrveh) {
				BOOL lightsOn = -1;
				BOOL highbeamsOn = -1;
				VEHICLE::GET_VEHICLE_LIGHTS_STATE(bus_veh[i], &lightsOn, &highbeamsOn);
				GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(true);
				VEHICLE::SET_VEHICLE_BRAKE_LIGHTS(bus_veh[i], false);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(bus_veh[i], 1, false);
				VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(bus_veh[i], 0, false);
				if (lightsOn || highbeamsOn) VEHICLE::SET_VEHICLE_LIGHTS(bus_veh[i], 1);
			} else GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(false);
			if (featureNPCNeonLights) {
				//if (VEHICLE::IS_THIS_MODEL_A_CAR(currVehModel_neon)) VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(bus_veh[i], &r, &g, &b);
				if (!VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 0) && !VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 1) && !VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 2) &&
					!VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 3)) {
				//if (r == 255 && g == 0 && b == 255) { 
					Hash currVehModel_neon = ENTITY::GET_ENTITY_MODEL(bus_veh[i]);
					if (bus_veh[i] != veh_mycurrveh && VEHICLE::IS_THIS_MODEL_A_CAR(currVehModel_neon)) {
						const std::vector<NeonLightsColor> NPC_NEON_COLORS = {{ "Bright White", NEON_COLOR_WHITE }, { "Dim White", NEON_COLOR_BLACK }, { "Electric Blue", NEON_COLOR_ELECTRICBLUE }, { "Mint Green", NEON_COLOR_MINTGREEN }, { "Lime Green", NEON_COLOR_LIMEGREEN },
						{ "Yellow", NEON_COLOR_YELLOW }, { "Gold", NEON_COLOR_GOLDENSHOWER }, { "Orange", NEON_COLOR_ORANGE }, { "Red", NEON_COLOR_RED }, { "Pink", NEON_COLOR_PONYPINK }, { "Hot Pink", NEON_COLOR_HOTPINK },
						{ "Purple", NEON_COLOR_PURPLE }, { "Black Light", NEON_COLOR_BLACKLIGHT }};
						int temp_colour = rand() % 11 + 2;
						NeonLightsColor npc_whichcolor = NPC_NEON_COLORS[temp_colour];
						VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(bus_veh[i], npc_whichcolor.rVal, npc_whichcolor.gVal, npc_whichcolor.bVal);
						VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 0, true);
						VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 1, true);
						//VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 2, true);
						//VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(bus_veh[i], 3, true); 
						if (getGameVersion() > 45) {
							VEHICLE::SET_VEHICLE_XENON_COLOUR(bus_veh[i], temp_colour);
							VEHICLE::TOGGLE_VEHICLE_MOD(bus_veh[i], 22, 1);
						}
					}
				}
			}
			if (WORLD_DAMAGED_VEHICLES_VALUES[DamagedVehiclesIndex] > 0 && bus_veh[i] != veh_mycurrveh) {
				Vector3 coords_ped = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
				int temp_damage = rand() % WORLD_DAMAGED_VEHICLES_VALUES[DamagedVehiclesIndex] + 1;
				if (!VEHICLE::_IS_VEHICLE_DAMAGED(bus_veh[i])) VEHICLE::SET_VEHICLE_DAMAGE(bus_veh[i], coords_ped.x, coords_ped.y, coords_ped.z, temp_damage, 1000, 1);
			}
			if (featureDirtyVehicles) {
				int temp_dirty = rand() % 15 + 0;
				if (VEHICLE::GET_VEHICLE_DIRT_LEVEL(bus_veh[i]) == 0 && bus_veh[i] != veh_mycurrveh) VEHICLE::SET_VEHICLE_DIRT_LEVEL(bus_veh[i], temp_dirty);
			} 
			if (featureNPCNoGravityVehicles) {
				if (bus_veh[i] != veh_mycurrveh) {
					VEHICLE::SET_VEHICLE_GRAVITY(bus_veh[i], false);
					if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(bus_veh[i]) < 2 && (ENTITY::GET_ENTITY_SPEED(bus_veh[i]) * 3.6) < 20) ENTITY::APPLY_FORCE_TO_ENTITY(bus_veh[i], 1, 0, 0, 5, 0, 0, 0, 1, true, true, false, true, true);
				}
			}
			if (featureNPCReducedGripVehicles) {
				if (bus_veh[i] != veh_mycurrveh) VEHICLE::SET_VEHICLE_REDUCE_GRIP(bus_veh[i], true);
			}
			 
			if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] > 0 && featureSnow) {
				Vehicle my_veh_on_snow = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()); 
				Vehicle veh_on_snow = PED::GET_VEHICLE_PED_IS_USING(bus_veh[i]);
				float my_speed_on_snow = ENTITY::GET_ENTITY_SPEED(my_veh_on_snow);
				float speed_on_snow = ENTITY::GET_ENTITY_SPEED(veh_on_snow);
				float slippery_randomize = -1;
				Vector3 coords_slip = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
				Vector3 coords_slip_ped = ENTITY::GET_ENTITY_COORDS(bus_veh[i], true);
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 1) { // simulation
					if (bus_veh[i] != my_veh_on_snow && (speed_on_snow < 0.5 || (speed_on_snow > 20.5 && speed_on_snow < 21.5) || (speed_on_snow > 30.5 && speed_on_snow < 31.5)) &&
						INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip_ped.x, coords_slip_ped.y, coords_slip_ped.z)) VEHICLE::SET_VEHICLE_REDUCE_GRIP(bus_veh[i], true);
					else VEHICLE::SET_VEHICLE_REDUCE_GRIP(bus_veh[i], false);
					if ((my_speed_on_snow < 0.5 || (my_speed_on_snow > 8 && my_speed_on_snow < 9.5) || (my_speed_on_snow > 16 && my_speed_on_snow < 17.5) ||
						(my_speed_on_snow > 25 && my_speed_on_snow < 26.5) || (my_speed_on_snow > 33 && my_speed_on_snow < 34.5)) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z))
						VEHICLE::SET_VEHICLE_REDUCE_GRIP(my_veh_on_snow, true);
					else VEHICLE::SET_VEHICLE_REDUCE_GRIP(my_veh_on_snow, false);
				}
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 2) { // arcade
					if (bus_veh[i] != my_veh_on_snow && (speed_on_snow < 0.2 || (speed_on_snow > 20.5 && speed_on_snow < 21) || (speed_on_snow > 30.5 && speed_on_snow < 31)) &&
						INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip_ped.x, coords_slip_ped.y, coords_slip_ped.z)) VEHICLE::SET_VEHICLE_REDUCE_GRIP(bus_veh[i], true);
					else VEHICLE::SET_VEHICLE_REDUCE_GRIP(bus_veh[i], false);
					if ((my_speed_on_snow < 0.2 || (my_speed_on_snow > 8.1 && my_speed_on_snow < 9) || (my_speed_on_snow > 16.1 && my_speed_on_snow < 17) ||
						(my_speed_on_snow > 25.1 && my_speed_on_snow < 26) || (my_speed_on_snow > 33.1 && my_speed_on_snow < 34)) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z))
						VEHICLE::SET_VEHICLE_REDUCE_GRIP(my_veh_on_snow, true);
					else VEHICLE::SET_VEHICLE_REDUCE_GRIP(my_veh_on_snow, false);
				}
				srand(time(0));
				int time11 = (rand() % 3000 + 0); // UP MARGIN + DOWN MARGIN
				int time12 = (rand() % 3000 + 0);
				int r_Type = (rand() % 3 + 0);
				slippery_randomize = (rand() % 1000 + 1);
				// simulation
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 1 && slippery_randomize > 960 && !AI::IS_PED_STILL(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) && 
					AI::IS_PED_RUNNING(PLAYER::PLAYER_PED_ID()) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z)) PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), time11, time12, r_Type, true, true, false);
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 1 && slippery_randomize > 860 && !AI::IS_PED_STILL(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) && 
					AI::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID()) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z)) PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), time11, time12, r_Type, true, true, false);
				// arcade
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 2 && slippery_randomize > 980 && !AI::IS_PED_STILL(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) &&
					AI::IS_PED_RUNNING(PLAYER::PLAYER_PED_ID()) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z)) PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), time11, time12, r_Type, true, true, false);
				if (WORLD_REDUCEDGRIP_SNOWING_VALUES[RadarReducedGripSnowingIndex] == 2 && slippery_randomize > 900 && !AI::IS_PED_STILL(PLAYER::PLAYER_PED_ID()) && !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) &&
					AI::IS_PED_SPRINTING(PLAYER::PLAYER_PED_ID()) && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_slip.x, coords_slip.y, coords_slip.z)) PED::SET_PED_TO_RAGDOLL(PLAYER::PLAYER_PED_ID(), time11, time12, r_Type, true, true, false);
			}
			if (WORLD_NPC_VEHICLESPEED_VALUES[NPCVehicleSpeedIndex] > 0 && bus_veh[i] != veh_mycurrveh) {
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(bus_veh[i], WORLD_NPC_VEHICLESPEED_VALUES[NPCVehicleSpeedIndex]); 
			}
		}
	}

	// NPC No Gravity Peds && Acid Water && Acid Rain
	if (featureNPCNoGravityPeds || featureAcidWater || featureAcidRain) {
		const int BUS_ARR_PED_SIZE = 1024;
		Ped bus_ped[BUS_ARR_PED_SIZE];
		int found_ped = worldGetAllPeds(bus_ped, BUS_ARR_PED_SIZE);
		for (int i = 0; i < found_ped; i++) {
			if (featureNPCNoGravityPeds && bus_ped[i] != PLAYER::PLAYER_PED_ID()) {
				PED::SET_PED_CAN_RAGDOLL(bus_ped[i], true);
				//PED::SET_PED_RAGDOLL_FORCE_FALL(PLAYER::PLAYER_PED_ID());
				PED::SET_PED_RAGDOLL_FORCE_FALL(bus_ped[i]);
				PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(bus_ped[i], true);
				PED::SET_PED_RAGDOLL_ON_COLLISION(bus_ped[i], true);
				if (!AI::IS_PED_STILL(bus_ped[i])) AI::TASK_STAND_STILL(bus_ped[i], 1000);
				//if (AI::IS_PED_WALKING(bus_ped[i]) || PED::IS_PED_FLEEING(bus_ped[i]) || AI::IS_PED_RUNNING(bus_ped[i]) || PED::IS_PED_IN_COMBAT(bus_ped[i], PLAYER::PLAYER_PED_ID()) || AI::IS_PED_STILL(bus_ped[i]))
				ENTITY::APPLY_FORCE_TO_ENTITY(bus_ped[i], 0, 300, 0, 0, 0, 0, 0, 1, true, true, false, true, true);
				if (!PED::IS_PED_RAGDOLL(bus_ped[i])) PED::SET_PED_TO_RAGDOLL(bus_ped[i], 10000, 10000, 0, true, true, false); //  && !PED::IS_PED_RAGDOLL(bus_ped[i])
				if (PED::IS_PED_SHOOTING(bus_ped[i])) ENTITY::APPLY_FORCE_TO_ENTITY(bus_ped[i], 4, 1000, 0, 0, 0, 0, 0, 1, true, true, true, true, true);
				ENTITY::SET_ENTITY_HAS_GRAVITY(bus_ped[i], false);
			}

			if (featureAcidWater && (ENTITY::IS_ENTITY_IN_WATER(bus_ped[i]) || PED::IS_PED_SWIMMING_UNDER_WATER(bus_ped[i]))) {
				if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3) acid_counter = acid_counter + 1;
				if (PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) {
					acid_counter_p = acid_counter_p + 1;
					been_damaged = true;
				}
				if (PED::GET_PED_ARMOUR(bus_ped[i]) > 0) {
					if (!AUDIO::IS_AMBIENT_SPEECH_PLAYING(bus_ped[i])) AUDIO::_PLAY_AMBIENT_SPEECH1(bus_ped[i], "BLOCKED_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
					if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 9)
						PED::SET_PED_ARMOUR(bus_ped[i], PED::GET_PED_ARMOUR(bus_ped[i]) - 1);
					if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 10) 
						PED::SET_PED_ARMOUR(bus_ped[i], PED::GET_PED_ARMOUR(bus_ped[i]) - 1);
					if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 9) acid_counter = 0;
					if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 10) acid_counter_p = 0;
				}
				if (ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) > 0 && PED::GET_PED_ARMOUR(bus_ped[i]) < 1) {
					if (!AUDIO::IS_AMBIENT_SPEECH_PLAYING(bus_ped[i])) AUDIO::_PLAY_AMBIENT_SPEECH1(bus_ped[i], "BLOCKED_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
					if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 4)
						ENTITY::SET_ENTITY_HEALTH(bus_ped[i], ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) - 1);
					if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 5)
						ENTITY::SET_ENTITY_HEALTH(bus_ped[i], ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) - 1);
					if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 4) acid_counter = 0;
					if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 5) acid_counter_p = 0;
				}
			}

			if (featureAcidRain) {
				Vector3 coords_ped = ENTITY::GET_ENTITY_COORDS(bus_ped[i], true);  
				Vehicle veh_currveh = PED::GET_VEHICLE_PED_IS_USING(bus_ped[i]);
				Hash currVehModel = ENTITY::GET_ENTITY_MODEL(veh_currveh);
				BOOL hit = false;
				Vector3 endCoords = ENTITY::GET_ENTITY_COORDS(bus_ped[i], true);
				Vector3 surfaceNormal = ENTITY::GET_ENTITY_COORDS(bus_ped[i], true);
				Entity entityHit = ENTITY::DOES_ENTITY_EXIST(bus_ped[i]);
				int temp1 = WORLDPROBE::_START_SHAPE_TEST_RAY(coords_ped.x, coords_ped.y, coords_ped.z, coords_ped.x, coords_ped.y, coords_ped.z + 2000, -1, bus_ped[i], 1);
				int result = WORLDPROBE::GET_SHAPE_TEST_RESULT(temp1, &hit, &endCoords, &surfaceNormal, &entityHit);
				
				if (GAMEPLAY::GET_RAIN_LEVEL() > 0 && INTERIOR::_ARE_COORDS_COLLIDING_WITH_EXTERIOR(coords_ped.x, coords_ped.y, coords_ped.z) && hit == 0 && (!PED::IS_PED_IN_ANY_VEHICLE(bus_ped[i], 0) || (PED::IS_PED_IN_ANY_VEHICLE(bus_ped[i], 0) &&
					(VEHICLE::GET_CONVERTIBLE_ROOF_STATE(veh_currveh) == 2 || !VEHICLE::DOES_VEHICLE_HAVE_ROOF(veh_currveh) || PED::IS_PED_ON_ANY_BIKE(bus_ped[i]) || VEHICLE::IS_THIS_MODEL_A_QUADBIKE(currVehModel) ||
						VEHICLE::IS_THIS_MODEL_A_BICYCLE(currVehModel) || currVehModel == GAMEPLAY::GET_HASH_KEY("BODHI2") || currVehModel == GAMEPLAY::GET_HASH_KEY("AIRTUG") || currVehModel == GAMEPLAY::GET_HASH_KEY("CADDY3") ||
						currVehModel == GAMEPLAY::GET_HASH_KEY("MOWER") || currVehModel == GAMEPLAY::GET_HASH_KEY("TRACTOR") || currVehModel == GAMEPLAY::GET_HASH_KEY("THRUSTER") || currVehModel == GAMEPLAY::GET_HASH_KEY("DUSTER") ||
						currVehModel == GAMEPLAY::GET_HASH_KEY("TORO") || currVehModel == GAMEPLAY::GET_HASH_KEY("DINGHY2") || currVehModel == GAMEPLAY::GET_HASH_KEY("DINGHY3") || currVehModel == GAMEPLAY::GET_HASH_KEY("DINGHY") ||
						currVehModel == GAMEPLAY::GET_HASH_KEY("SPEEDER") || currVehModel == GAMEPLAY::GET_HASH_KEY("JETMAX") || currVehModel == GAMEPLAY::GET_HASH_KEY("SQUALO") || currVehModel == GAMEPLAY::GET_HASH_KEY("SUNTRAP") ||
						currVehModel == GAMEPLAY::GET_HASH_KEY("SEASHARK") || currVehModel == GAMEPLAY::GET_HASH_KEY("SEASHARK2"))))) {
					if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3) acid_counter = acid_counter + 1;
					if (PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) {
						acid_counter_p = acid_counter_p + 1;
						been_damaged = true;
					}
					if (PED::GET_PED_ARMOUR(bus_ped[i]) > 0) {
						if (!AUDIO::IS_AMBIENT_SPEECH_PLAYING(bus_ped[i])) AUDIO::_PLAY_AMBIENT_SPEECH1(bus_ped[i], "BLOCKED_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
						if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 15)
							PED::SET_PED_ARMOUR(bus_ped[i], PED::GET_PED_ARMOUR(bus_ped[i]) - 1);
						if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 30)
							PED::SET_PED_ARMOUR(bus_ped[i], PED::GET_PED_ARMOUR(bus_ped[i]) - 1);
						if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 15) acid_counter = 0;
						if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 30) acid_counter_p = 0;
					}
					if (ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) > 0 && PED::GET_PED_ARMOUR(bus_ped[i]) < 1) {
						if (!AUDIO::IS_AMBIENT_SPEECH_PLAYING(bus_ped[i])) AUDIO::_PLAY_AMBIENT_SPEECH1(bus_ped[i], "BLOCKED_GENERIC", "SPEECH_PARAMS_FORCE_SHOUTED");
						if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 6)
							ENTITY::SET_ENTITY_HEALTH(bus_ped[i], ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) - 1);
						if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 10)
							ENTITY::SET_ENTITY_HEALTH(bus_ped[i], ENTITY::GET_ENTITY_HEALTH(bus_ped[i]) - 1);
						if (PED::GET_PED_TYPE(bus_ped[i]) != 0 && PED::GET_PED_TYPE(bus_ped[i]) != 1 && PED::GET_PED_TYPE(bus_ped[i]) != 2 && PED::GET_PED_TYPE(bus_ped[i]) != 3 && acid_counter > 6) acid_counter = 0;
						if ((PED::GET_PED_TYPE(bus_ped[i]) == 0 || PED::GET_PED_TYPE(bus_ped[i]) == 1 || PED::GET_PED_TYPE(bus_ped[i]) == 2 || PED::GET_PED_TYPE(bus_ped[i]) == 3) && acid_counter_p > 10) acid_counter_p = 0;
					}
				}
			}
		}
	}

	// Wind Strength
	if (windstrength_toggle == false) {
			GAMEPLAY::SET_WIND(WORLD_WIND_STRENGTH_VALUES[WindStrengthIndex]);
			if (WORLD_WIND_STRENGTH_VALUES[WindStrengthIndex] != 0) GAMEPLAY::SET_WIND_DIRECTION(ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
			if (WORLD_WIND_STRENGTH_VALUES[WindStrengthIndex] == 0) GAMEPLAY::SET_WIND_SPEED(0.0);
			windstrength_changed = WORLD_WIND_STRENGTH_VALUES[WindStrengthIndex];
			windstrength_toggle = true;
		}

	if (windstrength_changed != WORLD_WIND_STRENGTH_VALUES[WindStrengthIndex]) windstrength_toggle = false;

	//if (featureWorldNoTrafficUpdated)
	//{
		VEHICLE::_DISPLAY_DISTANT_VEHICLES(!featureWorldNoTraffic);
		GRAPHICS::DISABLE_VEHICLE_DISTANTLIGHTS(featureWorldNoTraffic);

		if (featureWorldNoTraffic)
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_VEHICLES(v3.x, v3.y, v3.z, 1000.0, 0, 0, 0, 0, 0);

			STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
		}
		else
		{
			STREAMING::SET_VEHICLE_POPULATION_BUDGET(3);
			VEHICLE::SET_ALL_VEHICLE_GENERATORS_ACTIVE();
			VEHICLE::SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(true);
		}

		//featureWorldNoTrafficUpdated = false;
	//}
	if (featureWorldNoTraffic)// && get_frame_number() % 100 == 0) // else 
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			Vector3 v3 = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
			GAMEPLAY::CLEAR_AREA_OF_VEHICLES(v3.x, v3.y, v3.z, 1000.0, 0, 0, 0, 0, 0);
		}
		STREAMING::SET_VEHICLE_POPULATION_BUDGET(0);
	}

	if (!featureWorldRandomTrains)
	{
		VEHICLE::DELETE_ALL_TRAINS();
	}

	if (featureWeatherFreeze && !lastWeather.empty())
	{
		GAMEPLAY::SET_WEATHER_TYPE_NOW((char *)lastWeather.c_str());
	}

	if (featureCloudsFreeze && !lastClouds.empty())
	{
		GRAPHICS::_SET_CLOUD_HAT_TRANSITION((char *)lastClouds.c_str(), 1.0);
	}

	if (!featureRestrictedZones)
	{
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	}

	if (featureSnowUpdated)
	{
		if (featureSnow)
		{
			EnableSnow(featureSnow);
			EnableTracks(true, true, true, true);
		}
		else
		{
			EnableSnow(featureSnow);
			EnableTracks(false, false, false, false);
		}
		
		featureSnowUpdated = false;
	}

	if (featureMPMapUpdated)
	{
		if (featureMPMap)
		{
			DLC2::_LOAD_MP_DLC_MAPS();
			set_status_text("MP Maps enabled");
		}
		else
		{
			DLC2::_LOAD_SP_DLC_MAPS();
			set_status_text("MP Maps disabled");
		}

		featureMPMapUpdated = false;
	}
}

void add_world_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results)
{
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldMoonGravity", &featureWorldMoonGravity }); // , &featureWorldMoonGravity
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomCops", &featureWorldRandomCops, &featureWorldRandomCopsUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomTrains", &featureWorldRandomTrains, &featureWorldRandomTrainsUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldRandomBoats", &featureWorldRandomBoats, &featureWorldRandomBoatsUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldGarbageTrucks", &featureWorldGarbageTrucks, &featureWorldGarbageTrucksUpdated });

	//results->push_back(FeatureEnabledLocalDefinition{ "featureWeatherWind", &featureWeatherWind });
	results->push_back(FeatureEnabledLocalDefinition{ "featureWeatherFreeze", &featureWeatherFreeze });
	results->push_back(FeatureEnabledLocalDefinition{ "featureCloudsNo", &featureCloudsNo });
	results->push_back(FeatureEnabledLocalDefinition{ "featureCloudsFreeze", &featureCloudsFreeze });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBlackout", &featureBlackout, &featureBlackoutUpdated });

	results->push_back(FeatureEnabledLocalDefinition{ "featureRestrictedZones", &featureRestrictedZones });

	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldNoPeds", &featureWorldNoPeds }); // , &featureWorldNoPedsUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldNoTraffic", &featureWorldNoTraffic }); // , &featureWorldNoTrafficUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureNoPoliceBlips", &featureNoPoliceBlips }); // , &featureNoPoliceBlipsUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureWorldNoAnimals", &featureWorldNoAnimals }); // , &featureWorldNoAnimalsUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureFullMap", &featureFullMap }); // , &featureFullMapUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featurePenitentiaryMap", &featurePenitentiaryMap }); // , &featurePenitentiaryMapUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureZancudoMap", &featureZancudoMap, &featureZancudoMapUpdated });
	results->push_back(FeatureEnabledLocalDefinition{ "featureBusLight", &featureBusLight }); // , &featureBusLightUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureAcidWater", &featureAcidWater }); // , &featureAcidWaterUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureAcidRain", &featureAcidRain }); // , &featureAcidRainUpdated
	results->push_back(FeatureEnabledLocalDefinition{ "featureReducedGripVehiclesIfSnow", &featureReducedGripVehiclesIfSnow }); // , &featureReducedGripVehiclesIfSnowUpdated

	results->push_back(FeatureEnabledLocalDefinition{ "featureSnow", &featureSnow, &featureSnowUpdated });

	//results->push_back(FeatureEnabledLocalDefinition{ "featureMPMap", &featureMPMap, &featureMPMapUpdated }); //Was causing issues as ENT was trying to load the MP map during the save load screen...
}

void add_world_generic_settings(std::vector<StringPairSettingDBRow>* settings)
{
	settings->push_back(StringPairSettingDBRow{ "lastWeather", lastWeather });
	settings->push_back(StringPairSettingDBRow{ "lastWeatherName", lastWeatherName });
	settings->push_back(StringPairSettingDBRow{ "lastClouds", lastClouds });
	settings->push_back(StringPairSettingDBRow{ "lastCloudsName", lastCloudsName });
}

void add_world_feature_enablements2(std::vector<StringPairSettingDBRow>* results)
{
	results->push_back(StringPairSettingDBRow{ "RadarMapIndex", std::to_string(RadarMapIndex) });
	results->push_back(StringPairSettingDBRow{ "WindStrengthIndex", std::to_string(WindStrengthIndex) });
	results->push_back(StringPairSettingDBRow{ "DamagedVehiclesIndex", std::to_string(DamagedVehiclesIndex) });
	results->push_back(StringPairSettingDBRow{ "NPCVehicleSpeedIndex", std::to_string(NPCVehicleSpeedIndex) });
	results->push_back(StringPairSettingDBRow{ "RadarReducedGripSnowingIndex", std::to_string(RadarReducedGripSnowingIndex) });
}

void handle_generic_settings_world(std::vector<StringPairSettingDBRow>* settings)
{
	for (int i = 0; i < settings->size(); i++)
	{
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("lastWeather") == 0)
		{
			lastWeather = setting.value;
		}
		else if (setting.name.compare("lastWeatherName") == 0)
		{
			lastWeatherName = setting.value;
		}
		if (setting.name.compare("lastClouds") == 0)
		{
			lastClouds = setting.value;
		}
		else if (setting.name.compare("lastCloudsName") == 0)
		{
			lastCloudsName = setting.value;
		}
		else if (setting.name.compare("RadarMapIndex") == 0){
			RadarMapIndex = stoi(setting.value);
		}
		else if (setting.name.compare("WindStrengthIndex") == 0){
			WindStrengthIndex = stoi(setting.value);
		}
		else if (setting.name.compare("DamagedVehiclesIndex") == 0) {
			DamagedVehiclesIndex = stoi(setting.value);
		}
		else if (setting.name.compare("NPCVehicleSpeedIndex") == 0) {
			NPCVehicleSpeedIndex = stoi(setting.value);
		}
		else if (setting.name.compare("RadarReducedGripSnowingIndex") == 0) {
			RadarReducedGripSnowingIndex = stoi(setting.value);
		}
	}
}

	/* Snow related code -Will be moved into utils*/
	/* Thanks to Sjaak for the help/code */

	//Copyright (C) GTA:Multiplayer Team (https://wiki.gta-mp.net/index.php/Team)

	void writeJmp(BYTE * pFrom, BYTE * pTo)
	{
		DWORD protect;
		VirtualProtect(pFrom, 16, PAGE_EXECUTE_READWRITE, &protect);
		pFrom[0] = 0x48;  // mov rax, func
		pFrom[1] = 0xB8;
		*reinterpret_cast<BYTE **>(&pFrom[2]) = pTo;
		pFrom[10] = 0x50; // push rax
		pFrom[11] = 0xC3; // ret
		VirtualProtect(pFrom, 16, protect, &protect);
	}

	
	intptr_t FindSnowPattern(const char* bMask, const char* sMask)
	{
		// Game Base & Size
		static intptr_t pGameBase = (intptr_t)GetModuleHandle(nullptr);
		static uint32_t pGameSize = 0;
		if (!pGameSize)
		{
			MODULEINFO info;
			GetModuleInformation(GetCurrentProcess(), (HMODULE)pGameBase, &info, sizeof(MODULEINFO));
			pGameSize = info.SizeOfImage;
		}

		// Scan
		for (uint32_t i = 0; i < pGameSize; i++)
			if (CompareMemory((uint8_t*)(pGameBase + i), (uint8_t*)bMask, sMask))
				return pGameBase + i;

		return 0;
	}


	void EnableTracks(bool tracksVehicle = false, bool tracksPeds = false, bool deepTracksVehicle = false, bool deepTracksPed = false)
	{
		static auto VAR_FeetSnowTracks_call = FindSnowPattern("\x80\x3D\x00\x00\x00\x00\x00\x48\x8B\xD9\x74\x37", "xx?????xxxxx");

		if (!VAR_FeetSnowTracks_call)
		{
			return;
		}
		static auto VAR_FeetSnowTracks = VAR_FeetSnowTracks_call + (*(int32_t *)(VAR_FeetSnowTracks_call + 2)) + 7;
		
		static auto VAR_VehicleSnowTracks_call = FindSnowPattern("\x40\x38\x3D\x00\x00\x00\x00\x48\x8B\x42\x20", "xxx????xxxx");

		if (!VAR_VehicleSnowTracks_call)
		{
			return;
		}
		static auto VAR_VehicleSnowTracks = VAR_VehicleSnowTracks_call + (*(int32_t *)(VAR_VehicleSnowTracks_call + 3)) + 7;
		
		VirtualProtect((void*)VAR_FeetSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);
		VirtualProtect((void*)VAR_VehicleSnowTracks, 1, PAGE_EXECUTE_READWRITE, nullptr);

		// Enable/Disable Vehicle/Foot Snow tracks
		*(uint8_t *)VAR_FeetSnowTracks = tracksVehicle;
		*(uint8_t *)VAR_VehicleSnowTracks = tracksPeds;

		// Switch for big/small tracks
		static auto vehicleTrackTypes = FindSnowPattern("\xB9\x00\x00\x00\x00\x84\xC0\x44\x0F\x44\xF1", "x????xxxxxx");
		if (!vehicleTrackTypes)
		{
			return;
		}

		VirtualProtect((void*)vehicleTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
		*(uint8_t *)(vehicleTrackTypes + 1) = deepTracksVehicle ? 0x13 : 0x14;

		static auto pedTrackTypes = FindSnowPattern("\xB9\x00\x00\x00\x00\x84\xC0\x0F\x44\xD9\x48\x8B\x4F\x30", "x????xxxxxxxxx");
		if (!pedTrackTypes)
		{
			return;
		}
		VirtualProtect((void*)pedTrackTypes, 1, PAGE_EXECUTE_READWRITE, nullptr);
		*(uint8_t *)(pedTrackTypes + 1) = deepTracksPed ? 0x13 : 0x14;
	}

	void FirstPersonDeathCamera() {
		Ped dead_player = PLAYER::PLAYER_PED_ID();

		if (ENTITY::GET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID()) < 1 && DeathCam == NULL && (PED::GET_PED_TYPE(dead_player) == 0 || PED::GET_PED_TYPE(dead_player) == 1 ||
			PED::GET_PED_TYPE(dead_player) == 2 || PED::GET_PED_TYPE(dead_player) == 3)) {
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(dead_player, true);
			Vector3 curRotation = ENTITY::GET_ENTITY_ROTATION(dead_player, 2);

			DeathCam = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_FLY_CAMERA", playerPosition.x, playerPosition.y, playerPosition.z, curRotation.x, curRotation.y, curRotation.z, 50.0, true, 2);
						
			CAM::ATTACH_CAM_TO_PED_BONE(DeathCam, dead_player, 31086, 0, -0.15, 0, 1);
			CAM::POINT_CAM_AT_PED_BONE(DeathCam, dead_player, 31086, 0, 0.0, 0, 1);

			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE_BLEND_LEVEL(DeathCam, 1.0);
			CAM::_SET_CAM_DOF_MAX_NEAR_IN_FOCUS_DISTANCE(DeathCam, 1.0);
			CAM::_SET_CAM_DOF_FOCUS_DISTANCE_BIAS(DeathCam, 1.0);

			CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, true);
			CAM::SET_CAM_ACTIVE(DeathCam, true);
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
		}

		if (ENTITY::GET_ENTITY_HEALTH(dead_player) > 0 && !PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) && DeathCam != NULL) {
			ENTITY::SET_ENTITY_COLLISION(PLAYER::PLAYER_PED_ID(), 1, 1);
			CAM::RENDER_SCRIPT_CAMS(false, false, 0, false, false);
			CAM::DETACH_CAM(DeathCam);
			CAM::SET_CAM_ACTIVE(DeathCam, false);
			CAM::DESTROY_CAM(DeathCam, true);
			DeathCam = NULL;
			ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true);
		}
	}

	// Snow
	void EnableSnow(bool featureSnow)
	{
		if (featureSnow)
			EnableTracks(VehicleTracks, PedTracks, VehicleTrackDepth, PedTrackDepth);
		else
			EnableTracks();

		// Addresses

		static auto addr1 = FindSnowPattern("\x80\x3D\x00\x00\x00\x00\x00\x74\x27\x84\xC0", "xx?????xxxx");
		static auto addr2 = FindSnowPattern("\x44\x38\x3D\x00\x00\x00\x00\x74\x0F", "xxx????xx");


		// Outdated
		// In future the patterns might change
		static bool bUseAddr4 = false;
		if (!addr1)
		{
			static auto addr3 = FindSnowPattern("\x40\x38\x35\x00\x00\x00\x00\x74\x18\x84\xdb\x74\x14", "xxx????xxxxxx");
			if (!addr3)
			{
				static auto addr4 = FindSnowPattern("\x80\x3D\x00\x00\x00\x00\x00\x74\x25\xB9\x40\x00\x00\x00", "xx????xxxxxxxx");
				if (!addr4)
				{
					set_status_text("~r~ Error (1): Cannot enable Snow on this version of GTA V");
					IsSnow = false;
					return;
				}
				else
				{
					addr1 = addr4;
					bUseAddr4 = true;
				}
			}
			else
			{
				addr1 = addr3;
			}
		}
		static bool bUseAddr5 = false;
		if (!addr2)
		{
			static auto addr5 = FindSnowPattern("\x44\x38\x3D\x00\x00\x00\x00\x74\x1D\xB9\x40\x00\x00\x00", "xxx????xxxxxxx");
			if (!addr5)
			{
				set_status_text("~r~ Error (2): Cannot enable Snow on this version of GTA V");
				IsSnow = false;
				return;
			}
			else
			{
				addr2 = addr5;
				bUseAddr5 = true;
			}
		}

		// Original Memory
		static uint8_t original1[14] = { 0 };
		static uint8_t original2[15] = { 0 };

		// Initialize
		static bool bInitialized = false;
		if (!bInitialized)
		{
			bInitialized = true;

			// Unprotect Memory
			VirtualProtect((void*)addr1, 13, PAGE_EXECUTE_READWRITE, nullptr);
			VirtualProtect((void*)addr2, 14, PAGE_EXECUTE_READWRITE, nullptr);

			// Copy original Memory
			memcpy(&original1, (void*)addr1, 13);
			memcpy(&original2, (void*)addr2, 14);
		}
		if (featureSnow)
		{
			// Weather
			GAMEPLAY::SET_WEATHER_TYPE_NOW_PERSIST("XMAS");

			// NOP checks
			if (!bUseAddr4)
			{
				memset((void*)addr1, 0x90, 13);
			}
			else
			{
				writeJmp((BYTE *)addr1, (BYTE *)addr1 + 0x1B);	// takes 12 bytes
			}

			if (!bUseAddr5)
			{
				memset((void*)addr2, 0x90, 14);
			}
			else
			{
				writeJmp((BYTE *)addr2, (BYTE *)addr2 + 0x1C);
			}

			// Notification
			set_status_text("Snow Enabled");
		}
		else
		{
			// Copy original memory
			memcpy((void*)addr1, &original1, 13);
			memcpy((void*)addr2, &original2, 14);

			// Weather
			GAMEPLAY::CLEAR_WEATHER_TYPE_PERSIST();
			GAMEPLAY::SET_WEATHER_TYPE_NOW("CLEAR");

			// Notification
			set_status_text("Snow Disabled");
		}
}
