#pragma once

extern bool featureKMH;
extern bool featureAltitude;
extern bool featureSpeedOnFoot;
extern bool featureSpeedOnGround;
extern bool featureSpeedInAir;

//Speedometer size
//const std::vector<std::string> SPEED_SIZE_CAPTIONS{ "1x", "2x", "3x", "4x", "5x", "6x", "7x", "8x", "9x", "10x" };
const int SPEED_SIZE_VALUES[] = { 3, 5, 7, 10, 12, 15, 17, 20, 23, 25 };
extern int SpeedSizeIndex;
extern bool SizeChanged;

//Speedometer position
const std::vector<std::string> SPEED_POSITION_CAPTIONS{ "Bottom Right", "Bottom Center", "Top Right" };
//const int SPEED_POSITION_VALUES[] = { 1, 2, 3 };
extern int SpeedPositionIndexN;
extern bool PositionChanged;

void update_speed_text(int speed, Vector3 player_coords);
