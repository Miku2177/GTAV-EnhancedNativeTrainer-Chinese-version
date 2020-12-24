#pragma once

extern bool featurePrison_Hardcore;
extern bool featurePrison_Robe;
extern bool featurePedPrison_Robe;
extern bool featurePrison_Yard;
extern bool detained;
extern int alert_level;

//Player Goes To Prison
const std::vector<std::string> PLAYER_PRISON_CAPTIONS{ "OFF", "Died At 2 Stars Or More", "Died At 3 Stars Or More", "Died At 4 Stars Or More", "Died At 5 Stars" };
const int PLAYER_PRISON_VALUES[] = { 0, 1, 2, 3, 4 };
extern int current_player_prison;
extern bool current_player_prison_Changed;

//Money For escape attempt
extern int current_player_escapemoney;
extern bool current_player_escapemoney_Changed;

//Time before discharge
const std::vector<std::string> PLAYER_DISCHARGE_CAPTIONS{ "UNLIMITED", "10 MIN", "20 MIN", "30 MIN", "60 MIN", "90 MIN", "120 MIN" };
const int PLAYER_DISCHARGE_VALUES[] = { 0, 600, 1200, 1800, 3600, 5400, 7200 };
extern int current_player_discharge;
extern bool current_player_discharge_Changed;

//Number of stars when escaping
extern int current_escape_stars;
extern bool current_escape_stars_Changed;

void prison_break();