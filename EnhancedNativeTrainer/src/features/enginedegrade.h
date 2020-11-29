#pragma once

extern bool featureEngineDegrade;
extern bool featureEngineHealthBar;
extern bool featureLimpMode;

// Engine Health Percentage (Min Margin)
const std::vector<std::string> VEH_ENGINEHEALTH_CAPTIONS{ "OFF", "2", "5", "10", "30", "50", "70", "90", "100" }; // "20", "40", "60", "80", 
const int VEH_ENGINEHEALTH_VALUES[] = { 0, 2, 5, 10, 30, 50, 70, 90, 100 }; // 20, 40, 60, 80,  
extern int CarEngineHealthIndexN;
extern bool CarEngineHealthChanged;
extern int BikeEngineHealthIndexN;
extern bool BikeEngineHealthChanged;
extern int PlaneEngineHealthIndexN;
extern bool PlaneEngineHealthChanged;
extern int HeliEngineHealthIndexN;
extern bool HeliEngineHealthChanged;
extern int BoatEngineHealthIndexN;
extern bool BoatEngineHealthChanged;
extern int RestorationSpeedIndexN;
extern bool RestorationSpeedChanged;

// Engine Damage Speed (% Per Mile)
const std::vector<std::string> VEH_ENGINEDEGRADE_CAPTIONS{ "0.1", "0.5", "0.7", "1.0", "1.5", "2.0", "3.0", "5.0", "7.0", "10.0", "20.0" };
const double VEH_ENGINEDEGRADE_VALUES[] = { 0.1, 0.5, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 20.0 };
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