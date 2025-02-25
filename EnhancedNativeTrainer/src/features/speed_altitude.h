#pragma once

extern bool featureKMH;
extern bool featureAltitude;
extern bool featureSpeedOnFoot;
extern bool featureSpeedOnGround;
extern bool featureSpeedInAir;

//速度表大小
const int SPEED_SIZE_VALUES[] = { 3, 3, 5, 7, 10, 12, 15, 17, 20, 23 };
extern int SpeedSizeIndex;
extern bool SizeChanged;

//速度表位置
const std::vector<std::string> SPEED_POSITION_CAPTIONS{ "右下角", "底部中心", "右上角" };
extern int SpeedPositionIndexN;
extern bool PositionChanged;

void update_speed_text(int speed, Vector3 player_coords);
