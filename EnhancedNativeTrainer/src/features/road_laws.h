#pragma once

extern bool featureRoadLaws;
extern bool featurePoliceVehicleBlip;
extern bool featurePoliceNoFlip;
extern bool featurePoliceNoDamage;
extern bool featureCopsUseRadio;
extern bool featureRunningRedLight;
extern bool featurePavementDriving;
extern bool featureDrivingAgainstTraffic;
extern bool featureCarCollision;
extern bool featureUsingMobilePhone;
extern bool featureVehicleHeavilyDamaged;
extern bool featureNoHelmetOnBike;
extern bool featureStolenVehicle;
extern bool featureNoLightsNightTime;
extern bool featureEscapingPolice;

extern bool featurePlayerMostWanted;
extern bool featurePlayerNoSwitch;

//在城市中(市区)超速
const std::vector<std::string> VEH_SPEEDINGCITY_CAPTIONS{ "关", "40 KM/H", "60 KM/H", "80 KM/H", "100 KM/H", "120 KM/H", "150 KM/H", "180 KM/H", "200 KM/H" };
const int VEH_SPEEDINGCITY_VALUES[] = { 0, 25, 37, 50, 62, 74, 93, 112, 124 }; //单位为 MPH，整数
extern int SpeedingCityIndex;

extern int SpeedingSpeedwayIndex; //在高速公路上超速行驶

extern int FineSizeIndex; //罚款金额

//侦测范围
const std::vector<std::string> VEH_DETECTIONRANGE_CAPTIONS{ "3 米", "5 米", "10 米", "20 米", "30 米", "50 米", "100 米", "200 米", "500 米", "1000 米" };
const int VEH_DETECTIONRANGE_VALUES[] = { 3, 5, 10, 20, 30, 50, 100, 200, 500, 1000 };
extern int DetectionRangeIndex;

//追捕范围
const std::vector<std::string> VEH_PIRSUITRANGE_CAPTIONS{ "100 米", "200 米", "300 米", "400 米", "500 米", "600 米", "700 米", "800 米", "900 米", "1000 米" };
const int VEH_PIRSUITRANGE_VALUES[] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };
extern int PirsuitRangeIndexN;

//如果试图逃脱时的星级
const std::vector<std::string> VEH_STARSPUNISH_CAPTIONS{ "1 星", "2 星", "3 星", "4 星", "5 星" };
const int VEH_STARSPUNISH_VALUES[] = { 1, 2, 3, 4, 5 };
extern int StarsPunishIndex;

//头号通缉犯
extern int current_player_mostwanted;
extern bool current_player_mostwanted_Changed;
extern int mostwanted_level_enable;
extern bool mostwanted_level_enable_Changed;

void road_laws();
void most_wanted();