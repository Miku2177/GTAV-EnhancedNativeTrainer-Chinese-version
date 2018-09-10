#pragma once

extern Blip myChauffeurBlip;
extern bool marker_been_set, blipDriveFound;
extern float planecurrspeed;
extern bool featureStickToGround, landing, landing_gear_off;
extern bool blipFound, reverse_check, altitude_reached;
extern Ped driver_to_marker_pilot;
extern Vehicle curr_veh;
extern bool featureLandAtDestination;

extern int TelChauffeurIndex;
extern bool TelChauffeur_Changed;

//Chauffeur Speed
const std::vector<std::string> TEL_CHAUFFEUR_SPEED_CAPTIONS{ "20", "30", "40", "50", "70", "100", "120", "150", "200" };
const std::vector<int> TEL_CHAUFFEUR_SPEED_VALUES{ 20, 30, 40, 50, 70, 100, 120, 150, 200 };
extern int TelChauffeur_speed_Index;
extern bool TelChauffeur_speed_Changed;

//Chauffeur Altitude
const std::vector<std::string> TEL_CHAUFFEUR_ALTITUDE_CAPTIONS{ "10", "30", "50", "100", "200", "300", "500", "1000", "1500", "2000", "2500" };
const std::vector<int> TEL_CHAUFFEUR_ALTITUDE_VALUES{ 10, 30, 50, 100, 200, 300, 500, 1000, 1500, 2000, 2500 };
extern int TelChauffeur_altitude_Index;
extern bool TelChauffeur_altitude_Changed;

//Driving Styles
const std::vector<std::string> TEL_CHAUFFEUR_DRIVINGSTYLES_CAPTIONS{ "Careless Driver", "Careful Driver", "Prioritise Shortcuts", "Straight To Target" };
const std::vector<int> TEL_CHAUFFEUR_DRIVINGSTYLES_VALUES{ 786468, 1074528293, 262144, 16777216 };
extern int TelChauffeur_drivingstyles_Index;
extern bool TelChauffeur_drivingstyles_Changed;

Vector3 get_blip_marker();

void drive_to_marker();