#pragma once

#include <map>
#include <string>

class ENTColor{
	public:
	int rgba[4];
	static ENTColor colHeaderText, colHeaderRect, colItemText, colItemRect, colItemActiveText, colItemActiveRect, 
		colListItemLocked, colListItemUnlocked, colWanted, colUnwanted, colChildExists, colSpriteBorder, colsMenu[12];
	static const std::map<std::string, int> colsVars;
	static const std::map<int, std::string> colsVarsReverse;
	static const std::string colsCaptions[12], colsCompCaptions[4];
	static const int colsVarsNum;

	ENTColor(int r, int g, int b, int a){
		rgba[0] = r, rgba[1] = g, rgba[2] = b, rgba[3] = a;
	}

	virtual ~ENTColor(){
		// leave empty
	}

	static void reset_colors();
};
