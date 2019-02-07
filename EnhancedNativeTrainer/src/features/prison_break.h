#pragma once

extern bool featurePrison_Hardcore;
extern bool featurePrison_Robe;
extern bool featurePedPrison_Robe;
extern bool featurePrison_Yard;
extern bool detained;

//Player Goes To Prison
const std::vector<std::string> PLAYER_PRISON_CAPTIONS{ "OFF", "Died At 2 Stars Or More", "Died At 3 Stars Or More", "Died At 4 Stars Or More", "Died At 5 Stars" };
const int PLAYER_PRISON_VALUES[] = { 0, 1, 2, 3, 4 };
//const std::vector<int> PLAYER_PRISON_VALUES{ 0, 1, 2, 3, 4 };
extern int current_player_prison;
extern bool current_player_prison_Changed;

//Money For escape attempt
const std::vector<std::string> PLAYER_ESCAPEMONEY_CAPTIONS{ "OFF", "10 000$", "30 000$", "50 000$", "100 000$", "500 000$", "1 000 000$", "5 000 000$", "10 000 000$", "50 000 000$", "100 000 000$" };
const int PLAYER_ESCAPEMONEY_VALUES[] = { 0, 10000, 30000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 };
//const std::vector<int> PLAYER_ESCAPEMONEY_VALUES{ 0, 10000, 30000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000 };
extern int current_player_escapemoney;
extern bool current_player_escapemoney_Changed;

//Time before discharge
const std::vector<std::string> PLAYER_DISCHARGE_CAPTIONS{ "UNLIMITED", "10 MIN", "20 MIN", "30 MIN", "60 MIN", "90 MIN", "120 MIN" };
const int PLAYER_DISCHARGE_VALUES[] = { 0, 600, 1200, 1800, 3600, 5400, 7200 };
//const std::vector<int> PLAYER_DISCHARGE_VALUES{ 0, 600, 1200, 1800, 3600, 5400, 7200 };
extern int current_player_discharge;
extern bool current_player_discharge_Changed;

//Number of stars when escaping
const std::vector<std::string> PLAYER_ESCAPESTARS_CAPTIONS{ "3 Stars", "4 Stars", "5 Stars" };
const int PLAYER_ESCAPESTARS_VALUES[] = { 3, 4, 5 };
//const std::vector<int> PLAYER_ESCAPESTARS_VALUES{ 3, 4, 5 };
extern int current_escape_stars;
extern bool current_escape_stars_Changed;

void prison_break();