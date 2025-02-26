#include "vehicles.h"
#include "..\ui_support\menu_functions.h"
#include "..\io\config_io.h"

const std::vector<PaintColor> PAINTS_ALL{
	{0, "黑色"},
	{1, "石墨色"},
	{2, "黑钢色"},
	{3, "暗钢色"},
	{4, "银色"},
	{5, "蓝银色"},
	{6, "轧钢色"},
	{7, "暗影银色"},
	{8, "石银色"},
	{9, "午夜银色"},
	{10, "铸铁银色"},
	{11, "炭黑灰色"},
	{27, "红色"},
	{28, "都灵红色"},
	{29, "方程式红色"},
	{30, "烈焰红色"},
	{31, "优雅红色"},
	{32, "石榴红色"},
	{33, "日落红色"},
	{34, "赤霞红色"},
	{35, "糖果红色"},
	{36, "日出橙色"},
	{37, "金色"},
	{38, "橙色"},
	{49, "深绿色"},
	{50, "赛车绿色"},
	{51, "海绿色"},
	{52, "橄榄绿色"},
	{53, "亮绿色"},
	{54, "汽油绿色"},
	{61, "银河蓝色"},
	{62, "深蓝色"},
	{63, "撒克逊蓝色"},
	{64, "蓝色"},
	{65, "水手蓝色"},
	{66, "港口蓝色"},
	{67, "钻石蓝色"},
	{68, "浪花蓝色"},
	{69, "航海蓝色"},
	{70, "超炫蓝色"},
	{71, "莎夫特紫色"},
	{72, "帆船紫色"},
	{73, "赛车蓝色"},
	{74, "浅蓝色"},
	{88, "黄色"},
	{89, "竞速黄色"},
	{90, "青铜色"},
	{91, "露珠黄色"},
	{92, "柠檬绿色"},
	{93, "香槟色"},
	{94, "费尔泽棕色"},
	{95, "溪棕色"},
	{96, "巧克力棕色"},
	{97, "枫棕色"},
	{98, "鞍棕色"},
	{99, "稻草棕色"},
	{100, "苔藓棕色"},
	{101, "野牛棕色"},
	{102, "山毛榉木棕色"},
	{103, "榉木棕色"},
	{104, "赭石棕色"},
	{105, "沙棕色"},
	{106, "漂白棕色"},
	{107, "奶油色"},
	{111, "冰白色"},
	{112, "霜白色"},
	{125, "保安绿色"},
	{127, "警车蓝色"},
	{134, "纯白色"},
	{135, "热粉色"},
	{136, "鲑鱼粉色"},
	{137, "菲斯特粉色"},
	{138, "亮橙色"},
	{139, "绿色"},
	{140, "荧光蓝色"},
	{141, "午夜蓝色"},
	{142, "黑紫色"},
	{143, "酒红色"},
	{144, "猎人绿色"},
	{145, "亮紫色"},
	{146, "午夜紫色"},
	{147, "碳黑色"},
	{150, "熔岩红色"},
	{156, "合金色"},
	{157, "伊普西隆蓝色"},
	{12, "哑光黑色"},
	{13, "哑光灰色"},
	{14, "哑光浅灰色"},
	{39, "哑光红色"},
	{40, "哑光深红色"},
	{41, "哑光橙色"},
	{42, "哑光黄色"},
	{55, "哑光柠檬绿色"},
	{82, "哑光深蓝色"},
	{83, "哑光蓝色"},
	{84, "哑光午夜蓝色"},
	{128, "哑光绿色"},
	{129, "哑光棕色"},
	{131, "哑光冰白色"},
	{148, "哑光莎夫特紫色"},
	{149, "哑光午夜紫色"},
	{151, "哑光森林绿色"},
	{152, "哑光橄榄绿色"},
	{153, "哑光暗土色"},
	{154, "哑光沙漠棕色"},
	{155, "哑光叶绿色"},
	{117, "拉丝钢色"},
	{118, "拉丝黑钢色"},
	{119, "拉丝铝色"},
	{158, "纯金色"},
	{159, "拉丝金色"},
	{160, "抛光金色"},
	{15, "工业黑色"},
	{16, "工业黑聚酯"},
	{17, "工业深银色"},
	{18, "工业银色"},
	{19, "工业枪金属色"},
	{20, "工业暗影银色"},
	{43, "工业红色"},
	{44, "工业亮红色"},
	{45, "工业石榴红色"},
	{56, "工业深绿色"},
	{57, "工业绿色"},
	{75, "工业深蓝色"},
	{76, "工业午夜蓝色"},
	{77, "工业蓝色"},
	{78, "工业海沫蓝色"},
	{79, "工业闪电蓝色"},
	{80, "工业毛伊蓝色"},
	{81, "工业亮蓝色"},
	{108, "工业棕色"},
	{109, "工业中棕色"},
	{110, "工业浅棕色"},
	{122, "工业米白色"},
	{21, "磨损黑色"},
	{22, "磨损石墨色"},
	{23, "磨损银灰色"},
	{24, "磨损银色"},
	{25, "磨损蓝银色"},
	{26, "磨损暗影银色"},
	{46, "磨损红色"},
	{47, "磨损金红色"},
	{48, "磨损深红色"},
	{58, "磨损深绿色"},
	{59, "磨损绿色"},
	{60, "磨损海浪色"},
	{85, "磨损深蓝色"},
	{86, "磨损蓝色"},
	{87, "磨损浅蓝色"},
	{113, "磨损蜜棕色"},
	{114, "磨损棕色"},
	{115, "磨损深棕色"},
	{116, "磨损稻草棕色"},
	{121, "磨损米白色"},
	{123, "磨损橙色"},
	{124, "磨损浅橙色"},
	{126, "磨损士黄色"},
	{130, "磨损淡橙色"},
	{132, "磨损白色"},
	{133, "磨损橄榄军绿色"},
	{120, "铬合金"}
};

void apply_dash_colors(int colorIndex){
	VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), PAINTS_ALL.at(colorIndex).colorIndex);
}

void onhighlight_dash_colour_selection(MenuItem<int> choice){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(playerPed)){
		return;
	}

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家未进入车辆中！");
		return;
	}

	apply_dash_colors(choice.value);
}

bool onconfirm_dash_colour_selection(MenuItem<int> choice){
	return true;
}

bool process_dash_colour_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int colIndex = 0, currDashCol;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::_GET_VEHICLE_DASHBOARD_COLOUR(veh, &currDashCol);

	for(int a = 0; a < PAINTS_ALL.size(); a++){
		PaintColor b = PAINTS_ALL.at(a);

		if(currDashCol == b.colorIndex){
			colIndex = a;
		}

		item = new MenuItem<int>();
		item->caption = b.name;
		item->value = a;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "仪表板颜色选项", onconfirm_dash_colour_selection, onhighlight_dash_colour_selection, NULL, vehicle_menu_interrupt);
}

void apply_trim_colors(int colorIndex){
	VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), PAINTS_ALL.at(colorIndex).colorIndex);
}

void onhighlight_trim_colour_selection(MenuItem<int> choice){
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!ENTITY::DOES_ENTITY_EXIST(playerPed)){
		return;
	}

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆内！");
		return;
	}

	apply_trim_colors(choice.value);
}

bool onconfirm_trim_colour_selection(MenuItem<int> choice){
	return true;
}

bool process_trim_colour_menu(){
	std::vector<MenuItem<int> *> menuItems;
	MenuItem<int> *item;
	int colIndex = 0, currTrimCol;
	Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
	VEHICLE::_GET_VEHICLE_INTERIOR_COLOUR(veh, &currTrimCol);

	for(int a = 0; a < PAINTS_ALL.size(); a++){
		PaintColor b = PAINTS_ALL.at(a);

		if(currTrimCol == b.colorIndex){
			colIndex = a;
		}

		item = new MenuItem<int>();
		item->caption = b.name;
		item->value = a;
		menuItems.push_back(item);
	}

	return draw_generic_menu<int>(menuItems, &colIndex, "内饰颜色选项", onconfirm_trim_colour_selection, onhighlight_trim_colour_selection, NULL, vehicle_menu_interrupt);
}

bool onconfirm_trim_colour_menu(MenuItem<int> choice){

	if(choice.value == -1){
		process_dash_colour_menu();
	}
	else if(choice.value == -2){
		process_trim_colour_menu();
	}

	return false;
}

bool process_interior_colour_menu(){
	BOOL bPlayerExists = ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID());

	if(!bPlayerExists){
		return false;
	}

	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if(!PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)){
		set_status_text("玩家不在车辆中！");
		return false;
	}

	if(!is_this_a_car(PED::GET_VEHICLE_PED_IS_USING(playerPed))){
		set_status_text("不支持该车辆！");
		return false;
	}

	std::vector<MenuItem<int>*> menuItems;

	MenuItem<int> *chooseDashColor = new MenuItem<int>();
	chooseDashColor->caption = "仪表板颜色 ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseDashColor->value = -1;
	chooseDashColor->isLeaf = false;
	menuItems.push_back(chooseDashColor);

	MenuItem<int> *chooseTrimColor = new MenuItem<int>();
	chooseTrimColor->caption = "内饰颜色 ~HUD_COLOUR_GREYLIGHT~(161)";
	chooseTrimColor->value = -2;
	chooseTrimColor->isLeaf = false;
	menuItems.push_back(chooseTrimColor);

	return draw_generic_menu<int>(menuItems, 0, "内饰颜色", onconfirm_trim_colour_menu, NULL, NULL, vehicle_menu_interrupt);
}
