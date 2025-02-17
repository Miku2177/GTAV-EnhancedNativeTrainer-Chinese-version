#pragma once

extern bool featurePrison_Hardcore;
extern bool featurePrison_Robe;
extern bool featurePedPrison_Robe;
extern bool featurePrison_Yard;
extern bool detained;
extern int alert_level;

//玩家入狱条件
const std::vector<std::string> PLAYER_PRISON_CAPTIONS{ "关", "等级, 在2星或更高时死亡", "等级, 在3星或更高时死亡", "等级, 在4星或更高时死亡", "等级, 在5星时立即死亡" };
const int PLAYER_PRISON_VALUES[] = { 0, 1, 2, 3, 4 };
extern int current_player_prison;
extern bool current_player_prison_Changed;

//玩家越狱罚款
extern int current_player_escapemoney;
extern bool current_player_escapemoney_Changed;

//玩家坐牢时间
const std::vector<std::string> PLAYER_DISCHARGE_CAPTIONS{ "无限", "1 分钟", "2 分钟", "3 分钟", "4 分钟", "5 分钟", "10 分钟", "20 分钟", "30 分钟", "40 分钟", "50 分钟", "60 分钟", "120 分钟" };
const int PLAYER_DISCHARGE_VALUES[] = { 0, 60, 120, 180, 240, 300, 600, 1200, 1800, 2400, 3000, 3600, 7200 };//单位秒
extern int current_player_discharge;
extern bool current_player_discharge_Changed;

//逃脱时的通缉星级数量
extern int current_escape_stars;
extern bool current_escape_stars_Changed;

void prison_break();