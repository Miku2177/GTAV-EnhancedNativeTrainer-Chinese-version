#pragma once

extern bool featureEngineDegrade;
extern bool featureEngineHealthBar;
extern bool featureLimpMode;

// 引擎健康百分比（最小余量）
const std::vector<std::string> VEH_ENGINEHEALTH_CAPTIONS{ "关", "2", "5", "10", "30", "50", "70", "90", "100" }; // "20", "40", "60", "80", 
const int VEH_ENGINEHEALTH_VALUES[] = { 0, 2, 5, 10, 30, 50, 70, 90, 100 }; // 20, 40, 60, 80,  
extern int CarEngineHealthIndexN;
extern int BikeEngineHealthIndexN;
extern int PlaneEngineHealthIndexN;
extern int HeliEngineHealthIndexN;
extern int BoatEngineHealthIndexN;
extern int RestorationSpeedIndexN;

// 引擎损坏速度（每英里百分比）
const std::vector<std::string> VEH_ENGINEDEGRADE_CAPTIONS{ "0.1", "0.5", "0.7", "1.0", "1.5", "2.0", "3.0", "5.0", "7.0", "10.0", "20.0" };
const double VEH_ENGINEDEGRADE_VALUES[] = { 0.1, 0.5, 0.7, 1.0, 1.5, 2.0, 3.0, 5.0, 7.0, 10.0, 20.0 };
extern int CarEngineDegradeIndex;
extern int BikeEngineDegradeIndex;
extern int PlaneEngineDegradeIndex;
extern int HeliEngineDegradeIndex;
extern int BoatEngineDegradeIndex;

void engine_can_degrade();