#pragma once

extern Blip myChauffeurBlip;
extern bool marker_been_set; 
extern bool blipDriveFound;
extern float planecurrspeed;
extern bool landing;
extern bool altitude_reached;
extern Ped driver_to_marker_pilot;
extern Vehicle curr_veh;
extern bool featureLandAtDestination;

extern int TelChauffeurIndex;
extern bool TelChauffeur_Changed;

// 代驾驾驶速度
const std::vector<std::string> TEL_CHAUFFEUR_SPEED_CAPTIONS{ "20", "40", "70", "100", "120", "150", "200" }; // "30", "50", 
const int TEL_CHAUFFEUR_SPEED_VALUES[] = { 20, 40, 70, 100, 120, 150, 200 }; // 30, 50, 
extern int TelChauffeur_speed_IndexN;
extern bool TelChauffeur_speed_Changed;

// 代驾驾驶高度
const std::vector<std::string> TEL_CHAUFFEUR_ALTITUDE_CAPTIONS{ "10", "30", "50", "100", "200", "300", "500", "1000", "1500", "2000", "2500" };
const int TEL_CHAUFFEUR_ALTITUDE_VALUES[] = { 10, 30, 50, 100, 200, 300, 500, 1000, 1500, 2000, 2500 };
extern int TelChauffeur_altitude_Index;
extern bool TelChauffeur_altitude_Changed;

// 驾驶风格
const std::vector<std::string> TEL_CHAUFFEUR_DRIVINGSTYLES_CAPTIONS{ "鲁莽驾驶", "谨慎驾驶", "优先捷径", "直奔目标" };;
const int TEL_CHAUFFEUR_DRIVINGSTYLES_VALUES[] = { 786468, 1074528293, 262144, 16777216 };
extern int TelChauffeur_drivingstyles_Index;
extern bool TelChauffeur_drivingstyles_Changed;

Vector3 get_blip_marker();

void drive_to_marker();