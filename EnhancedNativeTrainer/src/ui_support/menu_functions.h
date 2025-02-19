/*
这段代码的部分最初来源于 GTA V SCRIPT HOOK SDK。
http://dev-c.com
(C) Alexander Blade 2015

它现在已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "..\debug\debuglog.h"
#include "..\io\io.h"
#include "..\features\airbrake.h"
#include "..\utils.h"

#include "entcolor.h"

#include <string>
#include <sstream> 

#include <ctime>
#include <vector>
#include <algorithm>

#pragma warning(disable : 4244 4305 4267) // double <-> float conversions <-> size_t conversions

const int fontHeader = 4, fontItem = 0, fontWanted = 7;// 修改字体类型：分别用于标题字体、项目字体、目标字体

extern void(*periodic_feature_call)(void);

extern void(*menu_per_frame_call)(void);

extern bool airbrake_enable;
extern bool mouse_view_control;
extern bool help_showing;
extern bool frozen_time;
extern bool been_damaged;

static const char* LOCAL_TEXTURE_DICT = "LOCALTEXTURES";

template<class T>
class MenuItem{
	public:

	virtual ~MenuItem(){
	}

	std::string caption; // 菜单项的标题或显示文本
	T value; // 菜单项的值，类型为模板参数 T
	int currentMenuIndex = 0; // 当前菜单项的索引，默认值为 0
	bool isLeaf = true; // 是否为叶子节点（即没有子菜单），默认值为 true
	void(*onConfirmFunction)(const MenuItem<T> choice) = NULL; // 确认时的回调函数，默认值为 NULL
	int sortval = 0; // 排序值，用于菜单项的排序，默认值为 0

	/**
	处理菜单项确认按下事件。
	返回确认是否已被处理；如果未被处理，事件将传递给父菜单。
	*/

	virtual inline bool onConfirm(){
		//set_status_text("Parent confirm");
		if(onConfirmFunction != NULL){
			onConfirmFunction(*this);
		}
		return false;
	};

	virtual bool isAbsorbingLeftAndRightEvents(){
		return false;
	};

	virtual void handleLeftPress(){
	}

	virtual void handleRightPress(){
	}

	protected:

};

template<class T>
class ToggleMenuItem: public MenuItem < T >{
	public:

	virtual ~ToggleMenuItem(){
	}

	bool *toggleValue = NULL;
	bool *toggleValueUpdated = NULL;

	virtual bool get_toggle_value(){
		return *toggleValue;
	}

	virtual bool onConfirm();
};

template<class T>
class FunctionDrivenToggleMenuItem: public ToggleMenuItem < T >{
	public:

	virtual ~FunctionDrivenToggleMenuItem(){
	}

	bool(*getter_call)(std::vector<T> extras);
	void(*setter_call)(bool, std::vector<T> extras);

	std::vector<T> extra_arguments;

	virtual bool get_toggle_value(){
		return getter_call(extra_arguments);
	}

	virtual inline bool onConfirm(){
		setter_call(!getter_call(extra_arguments), extra_arguments);
		return true;
	}
};

class WantedSymbolItem: public MenuItem <int>{
	public:

	virtual ~WantedSymbolItem(){
	}

	int get_wanted_value();

	virtual bool onConfirm(){
		return true;
	};

	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	};

	virtual void handleLeftPress();

	virtual void handleRightPress();
};

class SelectFromListMenuItem: public MenuItem <int>{
	public:

	inline SelectFromListMenuItem(std::vector<std::string> captions, void(*onValueChangeCallback)(int, SelectFromListMenuItem*)){
		this->itemCaptions = captions;
		this->onValueChangeCallback = onValueChangeCallback;
	}

	virtual ~SelectFromListMenuItem(){
	}

	virtual bool onConfirm();

	virtual bool isAbsorbingLeftAndRightEvents();

	virtual void handleLeftPress();

	virtual void handleRightPress();

	virtual std::string getCurrentCaption();

	std::vector<std::string> itemCaptions;

	void(*onValueChangeCallback)(int index, SelectFromListMenuItem* source);

	bool wrap = true; // 控制文本是否自动换行，默认值为 true（自动换行）

	bool locked = false; // 控制项是否被锁定，默认值为 false（未锁定）

	std::vector<int> extras;
};

template<class T>
class CashItem: public MenuItem <T>{
	virtual ~CashItem(){
	}

	int cash = 100000, multiplier = 10, min = -1000000000, max = 1000000000; // 增加或减少现金的金额

	virtual bool onConfirm();
	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	};
	virtual void handleLeftPress();
	virtual void handleRightPress();

	public:
	int GetCash(){
		return cash;
	}
};

template<class T>
class ColorItem: public MenuItem<T>{
	public:
	int colorval, part, component, increment = 15, min = 0, max = 255;

	virtual ~ColorItem(){
		// 应该是空的
	}

	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	};

	virtual void handleLeftPress(){
		ENTColor::colsMenu[part].rgba[component] = colorval = colorval > min + increment ? colorval - increment : colorval > min ? min : max;
	}

	virtual void handleRightPress(){
		ENTColor::colsMenu[part].rgba[component] = colorval = colorval < max - increment ? colorval + increment : colorval < max ? max : min;
	}
};

template<class T>
class PaintColorItem : public MenuItem<T>{
public:
	int colorval, part, component, increment = 1, min = 0, max = 255;

	virtual ~PaintColorItem(){
		// 应该是空的
	}

	virtual bool onConfirm() {
		return true;
	}

	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	}

	virtual void handleLeftPress(){
		colorval -= increment;
		if (colorval < min)
		{
			colorval = max;
		}

		handlePressCommon();
	}

	virtual void handleRightPress(){
		colorval += increment;
		if (colorval > max)
		{
			colorval = min;
		}

		handlePressCommon();
	}

	int GetColor() { return colorval; }

private:

	void handlePressCommon(){
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		int components[3];
		switch (part) {
		case 0:
		case 2:
			VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &components[0], &components[1], &components[2]);
			components[component] = colorval;
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, components[0], components[1], components[2]);
			if (part == 2){
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, components[0], components[1], components[2]);
			}
			break;
		case 1:
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &components[0], &components[1], &components[2]);
			components[component] = colorval;
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, components[0], components[1], components[2]);
			break;
		}
	}
};

template<class T>
class PaintIndexItem : public MenuItem<T>{
public:
	int colorindex, part, increment = 1, min = 0, max = 160;

	virtual ~PaintIndexItem(){
		// 应该是空的
	}

	virtual bool onConfirm() {
		return true;
	}

	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	}

	virtual void handleLeftPress(){
		colorindex -= increment;
		if (colorindex < min)
		{
			colorindex = max;
		}

		handlePressCommon();
	}

	virtual void handleRightPress(){
		colorindex += increment;
		if (colorindex > max)
		{
			colorindex = min;
		}

		handlePressCommon();
	}

	int GetIndex() { return colorindex; }
	void SetIndex(int index) { colorindex = index; }

private:

	void handlePressCommon(){
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		BOOL isCustom;
		int red, green, blue;
		int primary, secondary;
		int pearl, wheel;

		switch (part) {
		case 0:
			isCustom = VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(veh);
			if (isCustom){
				VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, &red, &green, &blue);
			}
			VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
			VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh);
			VEHICLE::SET_VEHICLE_COLOURS(veh, colorindex, secondary);
			if (isCustom){
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, red, green, blue);
			}
			break;
		case 1:
			isCustom = VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(veh);
			if (isCustom){
				VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, &red, &green, &blue);
			}
			VEHICLE::GET_VEHICLE_COLOURS(veh, &primary, &secondary);
			VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh);
			VEHICLE::SET_VEHICLE_COLOURS(veh, primary, colorindex);
			if (isCustom){
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, red, green, blue);
			}
			break;
		case 2:
			VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh);
			VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh);
			VEHICLE::SET_VEHICLE_COLOURS(veh, colorindex, colorindex);
			break;
		case 3:
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);
			VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, colorindex, wheel);
			break;
		case 4:
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(veh, &pearl, &wheel);
			VEHICLE::SET_VEHICLE_EXTRA_COLOURS(veh, pearl, colorindex);
			break;
		case 5:
			VEHICLE::_SET_VEHICLE_INTERIOR_COLOUR(veh, colorindex);
			break;
		case 6:
			VEHICLE::_SET_VEHICLE_DASHBOARD_COLOUR(veh, colorindex);
			break;
		}
	}
};

enum LifeItemType{
	HEALTH,
	MAXHEALTH,
	ARMOR,
	MAXARMOR
};

template<class T>
class LifeItem: public MenuItem<T>{
	public:
	int life, minimum = 0, maximum = 34464;// 生命值的最小值，默认值为 0，生命值的最大值，默认值为 34464
	LifeItemType lifeType;

	virtual ~LifeItem(){
		// 应该是空的
	}

	virtual bool onConfirm(){
		switch(lifeType){
			case HEALTH:
				ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), life);
				set_status_text("当前生命值已修改");
				break;
			case MAXHEALTH:
				PED::SET_PED_MAX_HEALTH(PLAYER::PLAYER_PED_ID(), life);
				set_status_text("最大生命值已修改");
				break;
			case ARMOR:
				PED::SET_PED_ARMOUR(PLAYER::PLAYER_PED_ID(), life);
				set_status_text("当前护甲值已修改");
				break;
			case MAXARMOR:
				PLAYER::SET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID(), life);
				set_status_text("最大护甲值已修改");
				break;
			default:
				break;
		}

		return true;
	}

	virtual bool isAbsorbingLeftAndRightEvents(){
		return true;
	}

	virtual void handleLeftPress(){
		int tmp = static_cast<int>(std::pow(10, static_cast<int>(log10(life) - 0.00001)));
		life = life > minimum ? tmp > 0 ? max((life - tmp) / tmp * tmp, minimum) : minimum : maximum;
	}

	virtual void handleRightPress(){
		int tmp = static_cast<int>(std::pow(10, static_cast<int>(max(log10(life), 0))));
		life = life < maximum ? min((life + tmp) / tmp * tmp, maximum) : minimum;
	}
};

enum MenuItemType{
	STANDARD,
	TOGGLE,
	WANTED,
	CASH,
	COLOR,
	LIFE
};

struct StandardOrToggleMenuDef{
	std::string text;
	bool *pState;
	bool *pUpdated;
	bool isLeaf;
	MenuItemType itemType;
};

struct StringStandardOrToggleMenuDef{
	std::string text;
	std::string value;
	bool *pState;
	bool *pUpdated;
	bool isLeaf;
};

class MenuItemImage{
	public:
	inline bool is_local(){
		return strcmp(dict, LOCAL_TEXTURE_DICT) == 0;
	};

	char* dict;
	char* name;
	int localID;
};

template<class T>
class MenuParameters{
	public:
	inline MenuParameters(std::vector<MenuItem<T>*> items, std::string headerText){
		this->items = items;
		this->headerText = headerText;
	}

	std::vector<MenuItem<T>*> items;
	std::string headerText;
	bool sanitiseHeaderText = false;// 是否对标题文本进行清理（例如去除非法字符或格式化）true 清理，false 不清理
	int *menuSelectionPtr = 0;
	bool(*onConfirmation)(MenuItem<T> value) = NULL;
	void(*onHighlight)(MenuItem<T> value) = NULL;
	void(*onExit)(bool returnValue) = NULL;
	bool(*interruptCheck)(void) = NULL;
	MenuItemImage*(*lineImageProvider)(MenuItem<T> value) = NULL;

	int get_menu_selection_index(){
		return *menuSelectionPtr;
	}

	void set_menu_selection_index(int index){
		*menuSelectionPtr = index;
	}

	bool has_menu_selection_ptr(){
		return menuSelectionPtr != 0;
	}
};

// 定义了静态常量浮点数用于设置文本高度，以下是各变量的说明：
static const float TEXT_HEIGHT_NORMAL = 17.0f; // 普通文本的高度（单位：像素）

static const float TEXT_HEIGHT_TITLE = 24.0f; // 标题文本的高度（单位：像素）

static const float TEXT_HEIGHT_NONLEAF = 24.0f; // 非叶子节点（例如菜单项或子菜单）文本的高度（单位：像素）

static const float TEXT_HEIGHT_WSTARS = 24.0f; // 带星号（例如通缉等级）文本的高度（单位：像素）

/**设置用于定期更新整个UI界面，并应用重复设置的方法。脚本核心会执行一次*/
void set_periodic_feature_call(void method(void));

/**执行周期性功能调用！例如，它会更新状态文本等 - 查看现有菜单以了解此操作的实现位置*/
void make_periodic_feature_call();

void set_menu_per_frame_call(void method(void));

void clear_menu_per_frame_call();

void set_menu_showing(bool showing);

bool is_menu_showing();

/** 绘制一个实心矩形。
* 我认为参数是：
* - A_0: X 坐标
* - A_1: Y 坐标
* - A_2: W 宽度
* - A_3: H 高度
* - A_4 - 7: R,G,B,A (红色、绿色、蓝色、透明度)
* 但需要查看实际用法以确定，并理解缩放比例的处理。
*/
void draw_rect(float A_0, float A_1, float A_2, float A_3, int A_4, int A_5, int A_6, int A_7);

void draw_ingame_sprite(MenuItemImage *image, float x, float y, int w, int h);

/* 清理标题：如果字符不是字母数字 ASCII、空或无效的 UTF-8（检查字符的高位是否设置），则移除该字符，否则返回完整标题 */
inline std::string sanitise_menu_header_text(std::string input){
	std::string caption(input);
	std::replace(caption.begin(), caption.end(), '-', ' ');
	std::replace(caption.begin(), caption.end(), '_', ' ');
	caption.erase(remove_if(caption.begin(), caption.end(), [](unsigned char c){
		return !(std::isalnum(c) || c == ' ' || (c & 0x80));
	}), caption.end());
	return caption;
}

inline void draw_menu_header_line(std::string caption, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool rescaleText = true, int curPage = 1, int pageCount = 1){
	float text_scale = rescaleText ? 0.60 : 0.35;// 设置文本的缩放比例：如果 rescaleText 为 true，则使用 0.60，否则使用 0.35
	bool outline = false;// 是否绘制文本轮廓（默认为 false）
	bool dropShadow = false;// 是否绘制文本阴影（默认为 false）

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	float lineWidthScaled = lineWidth / (float) screen_w; // 行宽度
	float lineTopScaled = lineTop / (float) screen_h; // 行顶部偏移量
	float textLeftScaled = textLeft / (float) screen_w; // 文本左侧偏移量
	float lineHeightScaled = lineHeight / (float) screen_h; // 行高度

	float lineLeftScaled = lineLeft / (float) screen_w;

	float textHeightScaled = TEXT_HEIGHT_TITLE / (float) screen_h;

	// 这是原始脚本中的实现方式

	// 文本的上半部分
	UI::SET_TEXT_FONT(fontHeader);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	UI::SET_TEXT_COLOUR(ENTColor::colsMenu[0].rgba[0], ENTColor::colsMenu[0].rgba[1], ENTColor::colsMenu[0].rgba[2], ENTColor::colsMenu[0].rgba[3]);
	UI::SET_TEXT_CENTRE(0);

	if(outline){
		UI::SET_TEXT_OUTLINE();
	}

	if(dropShadow){
		UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
	}

	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR) caption.c_str());

	float textY = lineTopScaled + (0.5f * (lineHeightScaled - textHeightScaled));
	float leftMarginScaled = textLeftScaled - lineLeftScaled;

	UI::_DRAW_TEXT(textLeftScaled, textY);

	// 绘制一个矩形
	draw_rect(lineLeftScaled, lineTopScaled, lineWidthScaled, lineHeightScaled,
			  ENTColor::colsMenu[1].rgba[0], ENTColor::colsMenu[1].rgba[1], ENTColor::colsMenu[1].rgba[2], ENTColor::colsMenu[1].rgba[3]);

	// 用不同颜色绘制页码
	if(pageCount > 1){
		std::ostringstream ss;
		ss << " ~HUD_COLOUR_MENU_YELLOW~" << curPage << "~HUD_COLOUR_GREYLIGHT~ 至 ~HUD_COLOUR_MENU_YELLOW~" << pageCount;

		UI::SET_TEXT_FONT(fontHeader);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		//UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		// 以防万一这个功能将来被设置为可自定义的，我会把这段代码留在这里
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}


		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		auto ssStr = ss.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *) ssStr.c_str());
		UI::_DRAW_TEXT(0, textY);
	}
}

template<typename T>
void draw_menu_item_line(MenuItem<T> *item, float lineWidth, float lineHeight, float lineTop, float lineLeft, float textLeft, bool active, bool rescaleText){
	float text_scale = 0.35;// 设置文本的缩放比例为 0.35
	bool outline = false;// 是否绘制文本轮廓（默认为 false）
	bool dropShadow = false;// 是否绘制文本阴影（默认为 false）

	// 如果是激活行，则调整相关值
	if(active){
		if(rescaleText){
			text_scale = 0.40;// 如果启用了文本缩放，则将缩放比例设置为 0.40
		}
	}
	else{
		outline = true;// 如果不是激活行，则启用文本轮廓（默认为 true）
	}

	int screen_w, screen_h;
	GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

	textLeft += lineLeft;

	float lineWidthScaled = lineWidth / (float) screen_w; // 行宽度
	float lineTopScaled = lineTop / (float) screen_h; // 行顶部偏移量
	float textLeftScaled = textLeft / (float) screen_w; // 文本左侧偏移量
	float lineHeightScaled = lineHeight / (float) screen_h; // 行高度

	float lineLeftScaled = lineLeft / (float) screen_w; // 将行的左侧位置（lineLeft）转换为归一化坐标
	float leftMarginScaled = textLeftScaled - lineLeftScaled; // 计算左侧边距的归一化值

	float textHeightScaled = TEXT_HEIGHT_NORMAL / (float) screen_h; // 将文本高度（TEXT_HEIGHT_NORMAL）转换为归一化坐标
	float rightMarginScaled = 30.0f / (float) screen_w; // 将右侧边距（30.0f）转换为归一化坐标

	// 这是原始脚本中的实现方式

	// 文本的上半部分
	UI::SET_TEXT_FONT(fontItem);
	UI::SET_TEXT_SCALE(0.0, text_scale);
	if(active){
		UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
	}
	else{
		UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
	}
	UI::SET_TEXT_CENTRE(0);

	if(outline){
		UI::SET_TEXT_OUTLINE();
	}

	if(dropShadow){
		UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
	}

	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR) item->caption.c_str());

	float textY = lineTopScaled + (0.5f * (lineHeightScaled - textHeightScaled)); // 计算文本的垂直位置（Y 坐标）

	UI::_DRAW_TEXT(textLeftScaled, textY);

	// 绘制一个矩形
	if(active){
		draw_rect(lineLeftScaled, lineTopScaled, lineWidthScaled, lineHeightScaled,
				  ENTColor::colsMenu[5].rgba[0], ENTColor::colsMenu[5].rgba[1], ENTColor::colsMenu[5].rgba[2], ENTColor::colsMenu[5].rgba[3]);
	}
	else{
		draw_rect(lineLeftScaled, lineTopScaled, lineWidthScaled, lineHeightScaled,
				  ENTColor::colsMenu[3].rgba[0], ENTColor::colsMenu[3].rgba[1], ENTColor::colsMenu[3].rgba[2], ENTColor::colsMenu[3].rgba[3]);
	}

	if(ToggleMenuItem<T>* toggleItem = dynamic_cast<ToggleMenuItem<T>*>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if(active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_CENTRE(0);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_CENTRE(1);
		UI::SET_TEXT_WRAP(0, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		if(!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED("cellphone_badger"))// 多人排行榜
		{
			GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("cellphone_badger", true);
		}

		if(toggleItem->get_toggle_value() == true){
			// 排行榜投票图标
			// (参数：纹理字典名称, 纹理名称, 屏幕X坐标, 屏幕Y坐标, X轴缩放, Y轴缩放, 旋转角度, 颜色R, 颜色G, 颜色B, 颜色A)
			GRAPHICS::DRAW_SPRITE("cellphone_badger", "t", lineLeftScaled + lineWidthScaled - rightMarginScaled, textY + 0.01f, 0.026, 0.034, 0, 255, 255, 255, 255);
		}
		else{
			GRAPHICS::DRAW_SPRITE("cellphone_badger", "u", lineLeftScaled + lineWidthScaled - rightMarginScaled, textY + 0.01f, 0.026, 0.034, 0, 0, 0, 0, 255);
		}

		UI::_DRAW_TEXT(lineLeftScaled + lineWidthScaled - rightMarginScaled, textY);
	}
	else if(CashItem<T>* cashItem = dynamic_cast<CashItem<T> *>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if(active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		// 获取现金并格式化为字符串，加入单位（万或亿）
		int cashAmount = cashItem->GetCash();
		std::string formattedAmount;
		std::string unit;
		std::string dollarSign = " 美元";  // 美元符号，默认加在金额后

		// 如果是负数，先去掉负号来避免单位与负号冲突
		bool isNegative = cashAmount < 0;
		if (isNegative) {
			cashAmount = -cashAmount;  // 临时将现金金额转换为正数，方便格式化
		}

		// 根据现金数值添加单位（万或亿），无论正负数都加单位
		if (cashAmount >= 100000000) {  // 大于等于1亿
			formattedAmount = std::to_string(cashAmount / 100000000) + " 亿";
		} else if (cashAmount >= 10000) {  // 大于等于1万
			formattedAmount = std::to_string(cashAmount / 10000) + " 万";
		} else {
			formattedAmount = std::to_string(cashAmount);  // 小于1万直接显示
			dollarSign = " 美元";  // 小于1万时，美元符号加在金额后
		}

		// 格式化金额前加上 "+" 或 "-" 符号
		std::stringstream ss;
		ss << "<< ";
		if (isNegative) {
			ss << "减 ";  // 负数前加 "-" 符号
		} else if (cashAmount > 0) {
			ss << "加 ";  // 正数前加 "+" 符号
		}

		// 加上金额和单位
		ss << formattedAmount << dollarSign << " >>";
		std::string ssStr = ss.str();

		// 显示文本到UI
		UI::_ADD_TEXT_COMPONENT_STRING(const_cast<char*>(ssStr.c_str())); // 确保类型匹配
		UI::_DRAW_TEXT(0, textY);
	}
	else if (PaintColorItem<T>* colorItem = dynamic_cast<PaintColorItem<T> *>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if (active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if (outline){
			UI::SET_TEXT_OUTLINE();
		}

		if (dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		std::stringstream ss;
		ss << "<< " << std::to_string(colorItem->GetColor()) << " >>";
		auto ssStr = ss.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *)ssStr.c_str());
		UI::_DRAW_TEXT(0, textY);
	}
	else if (PaintIndexItem<T>* indexItem = dynamic_cast<PaintIndexItem<T> *>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if (active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if (outline){
			UI::SET_TEXT_OUTLINE();
		}

		if (dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		std::stringstream ss;
		ss << "<< " << std::to_string(indexItem->GetIndex()) << " >>";
		auto ssStr = ss.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *)ssStr.c_str());
		UI::_DRAW_TEXT(0, textY);
	}
	else if (SelectFromListMenuItem* selectFromListItem = dynamic_cast<SelectFromListMenuItem*>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);

		//禁用所有未激活的项
		if(!active && selectFromListItem->locked){
			selectFromListItem->locked = false;
		}

		if(selectFromListItem->locked){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[6].rgba[0], ENTColor::colsMenu[6].rgba[1], ENTColor::colsMenu[6].rgba[2], ENTColor::colsMenu[6].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[7].rgba[0], ENTColor::colsMenu[7].rgba[1], ENTColor::colsMenu[7].rgba[2], ENTColor::colsMenu[7].rgba[3]);
		}

		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		std::string caption = selectFromListItem->getCurrentCaption();

		std::stringstream ss;

		if(selectFromListItem->wrap || selectFromListItem->value > 0){
			ss << "<< ";
		}
		else{
			ss << "";
		}

		ss << caption;

		if(selectFromListItem->wrap || selectFromListItem->value < selectFromListItem->itemCaptions.size() - 1){
			ss << " >>";
		}
		else{
			ss << "";
		}
		auto ssStr = ss.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *) ssStr.c_str());
		UI::_DRAW_TEXT(0, textY);

		UI::SET_TEXT_EDGE(1, 255, 215, 0, 255);

		textY = lineTopScaled + (0.5f * (lineHeightScaled - (TEXT_HEIGHT_NONLEAF / (float) screen_h)));
	}
	else if(WantedSymbolItem* wantedItem = dynamic_cast<WantedSymbolItem*>(item)){ // 如果当前项是 WantedSymbolItem 类型
		rightMarginScaled = 10.0f / (float) screen_w; // 计算右侧边距的缩放值
		float starTextScale = 0.6f; // 设置星号文本的缩放比例

		UI::SET_TEXT_FONT(fontWanted);
		UI::SET_TEXT_SCALE(0.0, starTextScale);
		UI::SET_TEXT_COLOUR(ENTColor::colsMenu[8].rgba[0], ENTColor::colsMenu[8].rgba[1], ENTColor::colsMenu[8].rgba[2], ENTColor::colsMenu[8].rgba[3]);
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);

		float starWidth = 19.5f / (float) screen_w; // 计算星号符号的宽度（归一化坐标）
		textY = lineTopScaled + (0.5f * (lineHeightScaled - (TEXT_HEIGHT_WSTARS / (float) screen_h))); // 计算文本的垂直位置（Y 坐标）

		std::ostringstream wantedStars; // 创建一个字符串流对象，用于构建星号字符串
		int wantedLevel = wantedItem->get_wanted_value(); // 获取当前通缉等级
		int i = 0; // 初始化循环变量 i
		for(; i < wantedLevel; i++){ // 循环生成星号字符串
			wantedStars << "*"; //绘制此处包含的任何字符
		}

		UI::SET_TEXT_WRAP(0, lineLeftScaled + lineWidthScaled - rightMarginScaled - (starWidth*(5 - i)));
		UI::_SET_TEXT_ENTRY("STRING");

		auto wantedStarsStr = wantedStars.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *) wantedStarsStr.c_str());
		UI::_DRAW_TEXT(0, textY);

		UI::SET_TEXT_FONT(fontWanted);
		UI::SET_TEXT_SCALE(0.0, starTextScale);
		UI::SET_TEXT_COLOUR(ENTColor::colsMenu[9].rgba[0], ENTColor::colsMenu[9].rgba[1], ENTColor::colsMenu[9].rgba[2], ENTColor::colsMenu[9].rgba[3]);
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);

		std::ostringstream unwantedStars;
		for(; i < 5; i++){
			unwantedStars << "*";
		}

		UI::SET_TEXT_WRAP(0, lineLeftScaled + lineWidthScaled - rightMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		auto unwantedStarsStr = unwantedStars.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *) unwantedStarsStr.c_str());

		UI::_DRAW_TEXT(0, textY);
	}
	else if(ColorItem<T> *colorItem = dynamic_cast<ColorItem<T> *>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if(active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		std::stringstream ss;
		ss << "<< " << colorItem->colorval << " >>";
		auto ssStr = ss.str();
		UI::_ADD_TEXT_COMPONENT_STRING((char *) ssStr.c_str());
		UI::_DRAW_TEXT(0, textY);
	}
	else if(LifeItem<T>* lifeItem = dynamic_cast<LifeItem<T> *>(item)){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, text_scale);
		if(active){
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[4].rgba[0], ENTColor::colsMenu[4].rgba[1], ENTColor::colsMenu[4].rgba[2], ENTColor::colsMenu[4].rgba[3]);
		}
		else{
			UI::SET_TEXT_COLOUR(ENTColor::colsMenu[2].rgba[0], ENTColor::colsMenu[2].rgba[1], ENTColor::colsMenu[2].rgba[2], ENTColor::colsMenu[2].rgba[3]);
		}
		UI::SET_TEXT_RIGHT_JUSTIFY(1);

		if(outline){
			UI::SET_TEXT_OUTLINE();
		}

		if(dropShadow){
			UI::SET_TEXT_DROPSHADOW(5, 0, 78, 255, 255);
		}

		UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");

		// 这段代码用于格式化并显示一个数值（lifeItem->life），将其转换为带有千位分隔符的字符串，并在屏幕上绘制
		std::string commaLife = std::to_string(lifeItem->life);
		int insertPosition = commaLife.length() - 3; // 计算插入逗号的位置（从字符串末尾向前每 3 位插入一个逗号）
		while(insertPosition > 0){ // 循环插入逗号
			commaLife.insert(insertPosition, ","); // 在指定位置插入逗号
			insertPosition -= 3; // 向前移动 3 位
		}

		std::stringstream ss; // 使用字符串流构建最终显示的文本
		ss << "<< " << commaLife << " >>"; // 将格式化后的数值包裹在 << >> 中
		auto ssStr = ss.str(); // 获取字符串流的内容
		UI::_ADD_TEXT_COMPONENT_STRING((char *) ssStr.c_str()); // 将文本添加到 UI 组件
		UI::_DRAW_TEXT(0, textY); // 在指定位置绘制文本
	}
	else if(!item->isLeaf){
		UI::SET_TEXT_FONT(fontItem);
		UI::SET_TEXT_SCALE(0.0, 0.4f);
		UI::SET_TEXT_COLOUR(ENTColor::colsMenu[10].rgba[0], ENTColor::colsMenu[10].rgba[1], ENTColor::colsMenu[10].rgba[2], ENTColor::colsMenu[10].rgba[3]);
		UI::SET_TEXT_CENTRE(0);

		UI::SET_TEXT_EDGE(1, 255, 215, 0, 255);

		UI::SET_TEXT_RIGHT_JUSTIFY(1);
		UI::SET_TEXT_WRAP(0.0f, lineLeftScaled + lineWidthScaled - leftMarginScaled);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_STRING(">>");
		float textY = lineTopScaled + (0.5f * (lineHeightScaled - (TEXT_HEIGHT_NONLEAF / (float) screen_h)));
		UI::_DRAW_TEXT(0, textY);
	}
}

/** 由于使用了模板，这段代码放在头文件中，而非CPP文件。可能有更好的方式。
*
* 这个方法绘制一个通用的菜单，支持键盘导航和分页。之所以把它放在这里，是为了避免在每次使用时都复制粘贴，
* 也方便我们在一个地方修改UI。显然，在修改之前请仔细考虑它的所有用法。
*
* 参数说明：
*
* - captions: 所有项目的标题列表，将显示在UI中
* - values: 所有项目的值列表。选中的项会传递给事件方法。它应该与captions大小和顺序匹配，即captions[4]对应values[4]等。
* - currentSelectionIndex: 当前导航到的位置索引
* - headerText: 菜单顶部的标题文本。该文本可能会通过此方法添加页码。
*
* 剩余参数是你的事件回调：
*
* - onConfirmation: 当做出选择时，传入所选项的方法。如果选择后菜单应关闭，返回true，否则返回false。
* - onHighlight: 可选的方法，当菜单导航发生变化时，会传入当前高亮项。如果不需要，请传入NULL。
* - onExit: 可选的方法，允许你在菜单关闭时插入行为，比如按返回键时保存位置等。如果不需要，请传入NULL。
* - interruptCheck: 可选的方法，它将被调用，以检查菜单是否应中止。
*/
template<typename T>
bool draw_generic_menu(std::vector<MenuItem<T>*> items, int *menuSelectionPtr, std::string headerText,
					   bool(*onConfirmation)(MenuItem<T> value),
					   void(*onHighlight)(MenuItem<T> value),
					   void(*onExit)(bool returnValue),
					   bool(*interruptCheck)(void) = NULL){
	MenuParameters<T> params(items, headerText);
	params.menuSelectionPtr = menuSelectionPtr;
	params.interruptCheck = interruptCheck;
	params.onConfirmation = onConfirmation;
	params.onExit = onExit;
	params.onHighlight = onHighlight;

	return draw_generic_menu(params);
}

template<typename T>
bool draw_generic_menu(MenuParameters<T> params){
	if(params.items.size() == 0){
		set_status_text("这个选项什么都没有！");
		return false;
	}

	if(NETWORK::NETWORK_IS_GAME_IN_PROGRESS()){
		return false;
	}

	bool result = false; // 初始化结果为 false
	DWORD waitTime = 150;// 设置等待时间为 150 毫秒
	const int totalItems = (int) params.items.size();
	const int itemsPerLine = 10;// 设置菜单显示的项目数
	const int lineCount = (int) (ceil((double) totalItems / (double) itemsPerLine));

	int currentSelectionIndex;
	if(params.has_menu_selection_ptr()){
		if(params.get_menu_selection_index() >= totalItems){
			params.set_menu_selection_index(0);
		}
		else if(params.get_menu_selection_index() < 0){
			params.set_menu_selection_index(0);
		}
		currentSelectionIndex = params.get_menu_selection_index();
	}
	else{
		currentSelectionIndex = 0;
	}

	if(params.onHighlight != NULL){
		params.onHighlight(*params.items[currentSelectionIndex]);
	}

	MenuItemImage* image = NULL;
	if(params.lineImageProvider != NULL){
		image = params.lineImageProvider(*params.items[currentSelectionIndex]);
	}

	// 填充菜单项的索引
	for(int i = 0; i < totalItems; i++){
		params.items[i]->currentMenuIndex = i;
	}

	MenuItem<T> *choice = NULL;

	while(true){
		if(trainer_switch_pressed()){
			menu_beep();

			set_menu_showing(!is_menu_showing());
		}
		else if(airbrake_switch_pressed()){
			menu_beep();
			set_menu_showing(false);
			if (airbrake_enable) process_airbrake_menu();
		}
		else if(params.interruptCheck != NULL && params.interruptCheck()){
			return false;
		}

		if(!is_menu_showing()){
			if(params.interruptCheck != NULL && params.interruptCheck()){
				return false;
			}

			make_periodic_feature_call();
			WAIT(0);
			continue;
		}

		if(menu_per_frame_call != NULL){
			menu_per_frame_call();
		}

		const int currentLine = floor((double) currentSelectionIndex / (double) itemsPerLine);

		const int originalIndex = currentSelectionIndex;

		int positionOnThisLine = currentSelectionIndex % itemsPerLine;
		int lineStartPosition = currentSelectionIndex - positionOnThisLine;
		int itemsOnThisLine = (lineStartPosition + itemsPerLine > totalItems) ? (totalItems - lineStartPosition) : itemsPerLine;

		// 定时菜单绘制，用于在切换活动行后暂停
		DWORD maxTickCount = GetTickCount() + waitTime;
		do{
			std::string sanit_header = params.sanitiseHeaderText ? sanitise_menu_header_text(params.headerText) : params.headerText;

			// 更改标题和菜单等，在这里！！！
			draw_menu_header_line(sanit_header,// 菜单标题文本
								  350.0f, // 标题的宽度
								  50.0f, // 标题的高度
								  15.0f, // 标题的 顶部 偏移量（Y 坐标）
								  35.0f, // 标题的 左侧 偏移量（X 坐标）
								  45.0f, // 标题文本的 左侧 偏移量
								  false, // 是否显示背景（false 表示不显示）
								  true, // 是否显示边框（true 表示显示）
								  (currentLine + 1),// 当前行号（从 1 开始）
								  lineCount
			);

			float activeLineY = 0; // 用于存储当前选中菜单项的（ Y 坐标）

			for(int i = 0; i < itemsOnThisLine; i++){ // 初始化循环变量 i，表示当前菜单项的索引，从 0 开始。
				float lineSpacingY = 8.0f; // 菜单项之间的垂直间距

				float lineWidth = 350.0f; // 菜单项的宽度
				float lineHeight = 31.0f; // 菜单项的高度

				float lineTop = 75.0 + (i * (lineHeight + lineSpacingY)); // 计算当前菜单项的顶部位置（Y 坐标）
				float lineLeft = 35.0f; // 菜单项的左侧位置（X 坐标）
				float textOffset = 10.0f; // 菜单项文本的 左侧 偏移量

				draw_menu_item_line(params.items[lineStartPosition + i], lineWidth, lineHeight, lineTop, lineLeft, textOffset, i == positionOnThisLine, false);

				if(i == positionOnThisLine){ // 如果当前菜单项是选中的项
					activeLineY = lineTop; // 记录当前菜单项的顶部位置（Y 坐标）
				}
			}

			if(image != NULL){
				int screen_w, screen_h; // 这段代码用于计算游戏内精灵（sprite）的坐标，并调用函数绘制精灵
				GRAPHICS::GET_SCREEN_RESOLUTION(&screen_w, &screen_h);

				float lineXPx = 35.0f + 350.0f + 8.0f;
				float lineXGame = lineXPx / (float) screen_w;
				float lineYGame = activeLineY / (float) screen_h;

				draw_ingame_sprite(image, lineXGame, lineYGame, 256, 128);
			}

			if(periodic_feature_call != NULL){
				periodic_feature_call();
			}

			WAIT(0); // 等待 0 毫秒（让出 CPU 时间片，避免忙等待）
		}
		while(GetTickCount() < maxTickCount); // 检查是否达到等待结束时间
		waitTime = 0; // 重置等待时间为 0

		bool bSelect, bBack, bUp, bDown, bLeft, bRight;
		get_button_state(&bSelect, &bBack, &bUp, &bDown, &bLeft, &bRight);

		choice = params.items[currentSelectionIndex];

		if(bSelect){ // 如果用户按下确认键
			menu_beep(); // 播放菜单提示音（例如“滴”声）

			waitTime = 200; // 设置等待时间为 200 毫秒，用于防止重复触发

			bool confHandled = choice->onConfirm();

			// 触发主处理函数
			if(!confHandled && params.onConfirmation != NULL){
				result = params.onConfirmation(*choice);
			}

			if(result){
				//result = false; // 设置为 false，以避免向上级传递（防止级联触发）
				break;
			}
		}
		else{ // 如果用户没有按下确认键
			if(bBack){// 如果用户按下返回键
				menu_beep(); // 播放菜单提示音（例如“滴”声）
				waitTime = 200; // 设置等待时间为 200 毫秒，用于防止重复触发
				result = false; // 将结果设置为 false，表示返回操作
				break; // 跳出当前循环或逻辑块
			}
			else{
				if(bDown){// 如果用户按下向下键
					menu_beep();
					currentSelectionIndex++;
					if(currentSelectionIndex >= totalItems || (currentSelectionIndex >= lineStartPosition + itemsOnThisLine)){
						currentSelectionIndex = lineStartPosition;
					}
					waitTime = 150; // 设置等待时间为 150 毫秒，用于防止重复触发
				}
				else if(bUp){// 如果用户按下向上键
					menu_beep();
					currentSelectionIndex--;
					if(currentSelectionIndex < 0 || (currentSelectionIndex < lineStartPosition)){
						currentSelectionIndex = lineStartPosition + itemsOnThisLine - 1;
					}
					waitTime = 150; // 设置等待时间为 150 毫秒，用于防止重复触发
				}
				else if(bLeft){ // 如果用户按下向左键
					menu_beep();

					if(choice->isAbsorbingLeftAndRightEvents()){
						choice->handleLeftPress();
					}
					else if(lineCount > 1){
						int mod = currentSelectionIndex % itemsPerLine;
						currentSelectionIndex -= itemsPerLine;
						if(currentSelectionIndex < 0){
							currentSelectionIndex = mod + ((lineCount - 1) * itemsPerLine);
							if(currentSelectionIndex >= totalItems){
								currentSelectionIndex = totalItems - 1;
							}
						}
					}
					waitTime = 200; // 设置等待时间为 200 毫秒，用于防止重复触发
				}
				else if(bRight){
					menu_beep();

					if(choice->isAbsorbingLeftAndRightEvents()){
						choice->handleRightPress();
					}
					else if(lineCount > 1){
						// 如果已经到达末尾，则重新开始
						if(currentLine == lineCount - 1){
							currentSelectionIndex = currentSelectionIndex % itemsPerLine;
							if(currentSelectionIndex >= totalItems){
								currentSelectionIndex = totalItems - 1;
							}
						}
						else{
							currentSelectionIndex += itemsPerLine;
							if(currentSelectionIndex >= totalItems){
								currentSelectionIndex = totalItems - 1;
							}
						}
					}

					waitTime = 200; // 设置等待时间为 200 毫秒，用于防止重复触发
				}

				if(params.onHighlight != NULL && originalIndex != currentSelectionIndex){
					params.onHighlight(*params.items[currentSelectionIndex]);
				}

				if(params.lineImageProvider != NULL && originalIndex != currentSelectionIndex){
					image = params.lineImageProvider(*params.items[currentSelectionIndex]);
				}

				if(params.has_menu_selection_ptr()){
					params.set_menu_selection_index(currentSelectionIndex);
				}
			}
		}
	}

	if(params.onExit != NULL){
		params.onExit(result);
	}

	// 解锁当前选中的项
	if(choice != NULL){
		if(SelectFromListMenuItem* selectFromListItem = dynamic_cast<SelectFromListMenuItem*>(choice)){
			selectFromListItem->locked = false;
		}
	}

	// 退出前等待
	if(waitTime > 0){ // 如果等待时间大于 0
		DWORD maxTickCount = GetTickCount() + waitTime; // 计算等待结束的时间点
		do{
			make_periodic_feature_call(); // 周期性地调用某个功能
			WAIT(0); // 等待 0 毫秒（让出 CPU 时间片，避免忙等待）
		}
		while(GetTickCount() < maxTickCount); // 检查是否达到等待结束时间
		waitTime = 0; // 重置等待时间为 0
	}

	// 清理项的内存
	for(int i = 0; i < params.items.size(); i++){
		delete (params.items[i]);
	}
	params.items.clear();

	if(image != NULL){
		delete image;
	}

	return result;
}

void set_status_text(std::string str, bool isGxtEntry = false);
// 要显示的文本内容，是否为 GXT 条目（默认值为 false）

void set_status_text_centre_screen(std::string str, DWORD time = 2500, bool isGxtEntry = false);
// 要显示的文本内容，文本显示的持续时间（默认值为 2500 毫秒）是否为 GXT 条目（默认值为 false）

void update_centre_screen_status_text();

void menu_beep();

void draw_menu_from_struct_def(StandardOrToggleMenuDef defs[], int lineCount, int* selectionRef, std::string caption, bool(*onConfirmation)(MenuItem<int> value));

void draw_menu_from_struct_def(StringStandardOrToggleMenuDef defs[], int lineCount, int* selectionRef, std::string caption, bool(*onConfirmation)(MenuItem<std::string> value));

std::string show_keyboard(char* title_id, char* prepopulated_text);

extern bool menu_showing;
