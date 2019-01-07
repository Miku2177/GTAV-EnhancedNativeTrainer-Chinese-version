#pragma once

extern bool featureEngineDegrade;
extern bool featureEngineHealthBar;
extern bool featureLimpMode;

// Engine Health Percentage (Min Margin)
const std::vector<std::string> VEH_ENGINEHEALTH_CAPTIONS{ "OFF", "2", "5", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100" };
const int VEH_ENGINEHEALTH_VALUES[] = { 0, 2, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
//const std::vector<int> VEH_ENGINEHEALTH_VALUES{ 0, 2, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };
extern int CarEngineHealthIndex;
extern bool CarEngineHealthChanged;
extern int BikeEngineHealthIndex;
extern bool BikeEngineHealthChanged;
extern int PlaneEngineHealthIndex;
extern bool PlaneEngineHealthChanged;
extern int HeliEngineHealthIndex;
extern bool HeliEngineHealthChanged;
extern int BoatEngineHealthIndex;
extern bool BoatEngineHealthChanged;
extern int RestorationSpeedIndex;
extern bool RestorationSpeedChanged;

// Engine Damage Speed (% Per Mile)
const std::vector<std::string> VEH_ENGINEDEGRADE_CAPTIONS{ "0.1", "0.5", "0.7", "1.0", "1.5", "2.0", "3.0", "5.0", "7.0", "10.0", "20.0" };
const double VEH_ENGINEDEGRADE_VALUES[] = { 0.1, 0.5, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 20.0 };
//const std::vector<double> VEH_ENGINEDEGRADE_VALUES{ 0.1, 0.5, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 20.0 };
extern int CarEngineDegradeIndex;
extern bool CarEngineDegradeChanged;
extern int BikeEngineDegradeIndex;
extern bool BikeEngineDegradeChanged;
extern int PlaneEngineDegradeIndex;
extern bool PlaneEngineDegradeChanged;
extern int HeliEngineDegradeIndex;
extern bool HeliEngineDegradeChanged;
extern int BoatEngineDegradeIndex;
extern bool BoatEngineDegradeChanged;

void engine_can_degrade();