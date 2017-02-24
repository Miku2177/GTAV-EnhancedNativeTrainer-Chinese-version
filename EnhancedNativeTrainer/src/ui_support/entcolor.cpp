#include "entcolor.h"

ENTColor ENTColor::colHeaderText(255, 255, 255, 255),
ENTColor::colHeaderRect(0, 0, 0, 200),
ENTColor::colItemText(255, 255, 255, 255),
ENTColor::colItemRect(255, 255, 255, 80),
ENTColor::colItemActiveText(0, 0, 0, 255),
ENTColor::colItemActiveRect(255, 180, 0, 200),
ENTColor::colListItemLocked(205, 205, 205, 255),
ENTColor::colListItemUnlocked(155, 155, 155, 255),
ENTColor::colWanted(255, 255, 255, 255),
ENTColor::colUnwanted(100, 100, 100, 255),
ENTColor::colChildExists(200, 200, 200, 255),
ENTColor::colSpriteBorder(0, 0, 0, 180),
ENTColor::colsMenu[12] = {
	ENTColor::colHeaderText,
	ENTColor::colHeaderRect,
	ENTColor::colItemText,
	ENTColor::colItemRect,
	ENTColor::colItemActiveText,
	ENTColor::colItemActiveRect,
	ENTColor::colListItemLocked,
	ENTColor::colListItemUnlocked,
	ENTColor::colWanted,
	ENTColor::colUnwanted,
	ENTColor::colChildExists,
	ENTColor::colSpriteBorder
};

const std::map<std::string, int> ENTColor::colsVars({
	{"colHeaderText", 0},
	{"colHeaderRect", 1},
	{"colItemText", 2},
	{"colItemRect", 3},
	{"colItemActiveText", 4},
	{"colItemActiveRect", 5},
	{"colListItemLocked", 6},
	{"colListItemUnlocked", 7},
	{"colWanted", 8},
	{"colUnwanted", 9},
	{"colChildExists", 10},
	{"colSpriteBorder", 11}
});

const std::map<int, std::string> ENTColor::colsVarsReverse({
	{0, "colHeaderText"},
	{1, "colHeaderRect"},
	{2, "colItemText"},
	{3, "colItemRect"},
	{4, "colItemActiveText"},
	{5, "colItemActiveRect"},
	{6, "colListItemLocked"},
	{7, "colListItemUnlocked"},
	{8, "colWanted"},
	{9, "colUnwanted"},
	{10, "colChildExists"},
	{11, "colSpriteBorder"}
});

const std::string ENTColor::colsCaptions[12] = {
	"Title Text",
	"Title Box",
	"Item Text",
	"Item Box",
	"Active Item Text",
	"Active Item Box",
	"Locked List Item",
	"Unlocked List Item",
	"Filled Wanted Star",
	"Empty Wanted Star",
	"Symbol When Sub-Menu Exists",
	"Preview Image Borders"
},
ENTColor::colsCompCaptions[4] = {
	"Red",
	"Green",
	"Blue",
	"Alpha (Opacity)"
};

const int ENTColor::colsVarsNum = ENTColor::colsVars.size();

void ENTColor::reset_colors(){
	ENTColor::colsMenu[0].rgba[0] = 255, ENTColor::colsMenu[0].rgba[1] = 255, ENTColor::colsMenu[0].rgba[2] = 255, ENTColor::colsMenu[0].rgba[3] = 255;
	ENTColor::colsMenu[1].rgba[0] = 0, ENTColor::colsMenu[1].rgba[1] = 0, ENTColor::colsMenu[1].rgba[2] = 0, ENTColor::colsMenu[1].rgba[3] = 200;
	ENTColor::colsMenu[2].rgba[0] = 255, ENTColor::colsMenu[2].rgba[1] = 255, ENTColor::colsMenu[2].rgba[2] = 255, ENTColor::colsMenu[2].rgba[3] = 255;
	ENTColor::colsMenu[3].rgba[0] = 255, ENTColor::colsMenu[3].rgba[1] = 255, ENTColor::colsMenu[3].rgba[2] = 255, ENTColor::colsMenu[3].rgba[3] = 80;
	ENTColor::colsMenu[4].rgba[0] = 0, ENTColor::colsMenu[4].rgba[1] = 0, ENTColor::colsMenu[4].rgba[2] = 0, ENTColor::colsMenu[4].rgba[3] = 255;
	ENTColor::colsMenu[5].rgba[0] = 255, ENTColor::colsMenu[5].rgba[1] = 180, ENTColor::colsMenu[5].rgba[2] = 0, ENTColor::colsMenu[5].rgba[3] = 200;
	ENTColor::colsMenu[6].rgba[0] = 205, ENTColor::colsMenu[6].rgba[1] = 205, ENTColor::colsMenu[6].rgba[2] = 205, ENTColor::colsMenu[6].rgba[3] = 255;
	ENTColor::colsMenu[7].rgba[0] = 155, ENTColor::colsMenu[7].rgba[1] = 155, ENTColor::colsMenu[7].rgba[2] = 155, ENTColor::colsMenu[7].rgba[3] = 255;
	ENTColor::colsMenu[8].rgba[0] = 255, ENTColor::colsMenu[8].rgba[1] = 255, ENTColor::colsMenu[8].rgba[2] = 255, ENTColor::colsMenu[8].rgba[3] = 255;
	ENTColor::colsMenu[9].rgba[0] = 100, ENTColor::colsMenu[9].rgba[1] = 100, ENTColor::colsMenu[9].rgba[2] = 100, ENTColor::colsMenu[9].rgba[3] = 255;
	ENTColor::colsMenu[10].rgba[0] = 200, ENTColor::colsMenu[10].rgba[1] = 200, ENTColor::colsMenu[10].rgba[2] = 200, ENTColor::colsMenu[10].rgba[3] = 255;
	ENTColor::colsMenu[11].rgba[0] = 0, ENTColor::colsMenu[11].rgba[1] = 0, ENTColor::colsMenu[11].rgba[2] = 0, ENTColor::colsMenu[11].rgba[3] = 180;
}
