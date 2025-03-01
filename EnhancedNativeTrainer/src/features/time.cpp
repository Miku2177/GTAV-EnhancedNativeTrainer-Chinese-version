/*
这段代码最初是作为 GTA V SCRIPT HOOK SDK 的一部分而创建的。
http://dev-c.com
(C) Alexander Blade 2015

现在它已成为 Enhanced Native Trainer 项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/
#include "time.h"
#include "vehicles.h"
#include "hotkeys.h"
#include "propplacement.h"
#include <iomanip>
#include "..\ui_support\menu_functions.h"
#include "script.h"

const std::vector<std::string> TIME_SPEED_CAPTIONS{ "最低", "0.1x", "0.2x", "0.3x", "0.4x", "0.5x", "0.6x", "0.7x", "0.8x", "0.9x", "1x (正常)" };
const std::vector<float> TIME_SPEED_VALUES{ 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
const int DEFAULT_TIME_SPEED = 10;

const std::vector<std::string> TIME_FLOW_RATE_CAPTIONS{ "冻结时间 (0秒/秒)", "每秒半秒 (0.5秒/秒)", "现实时间 (1秒/秒)", "每秒 2 秒", "每秒 3 秒", "每秒 4 秒", "每秒 5 秒", "每秒 6 秒", "每秒 7 秒", "每秒 8 秒", "每秒 9 秒", "每秒 10 秒", "每秒 12 秒", "每秒 15 秒", "正常时间流速 (30秒/秒)", "每秒 1 分钟", "每秒 2 分钟", "每秒 3 分钟", "每秒 4 分钟", "每秒 5 分钟", "每秒 6 分钟", "每秒 7 分钟", "每秒 8 分钟", "每秒 9 分钟", "每秒 10 分钟", "每秒 12 分钟", "每秒 15 分钟", "每秒 30 分钟", "每秒 1 小时", "每秒 2 小时", "每秒 3 小时", "每秒 4 小时", "每秒 5 小时", "每秒 6 小时", "每秒 12 小时", "每秒 1 天" };
const std::vector<float> TIME_FLOW_RATE_VALUES{ 0.0f, 0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 12.0f, 15.0f, 30.0f, 60.0f, 120.0f, 180.0f, 240.0f, 300.0f, 360.0f, 420.0f, 480.0f, 540.0f, 600.0f, 720.0f, 900.0f, 1800.0f, 3600.0f, 7200.0f, 10800.0f, 14400.0f, 18000.0f, 21600.0f, 43200.0f, 86400.0f };
const int DEFAULT_TIME_FLOW_RATE = 10;

const int DEFAULT_HOTKEY_FLOW_RATE = 10;

const int TIME_TO_SLOW_AIM = 2000;

std::vector<float> VEH_S;
std::vector<Vehicle> VEH_CURR;

int timeSpeedIndexWhileAiming = DEFAULT_TIME_SPEED;
int timeSpeedIndex = DEFAULT_TIME_SPEED;

int timeFlowRateIndex = DEFAULT_TIME_FLOW_RATE;
int HotkeyFlowRateIndex = DEFAULT_HOTKEY_FLOW_RATE;

bool featureTimeSynced = false;
bool featureShowtime = false;
bool featurehotkeytime = false;
bool featureSpeedAimInVeh = false;
bool timeFlowRateChanged = true, timeFlowRateLocked = true;
bool HotkeyFlowRateChanged = true, HotkeyFlowRateLocked = true;

bool slow_aim = false;

float frozentimestate = -1;

bool requireRefreshOfTime = false;

int activeLineIndexTime = 0;

float timeFactor = 1000.0f / TIME_FLOW_RATE_VALUES.at(timeFlowRateIndex);

int timeSinceAimingBegan = 0;

bool weHaveChangedTimeScale;

float quadratic_time_transition(float start, float end, float progress) {
	//二次方程相关内容
	float t = 1 - progress;
	t = 1 - (t * t);

	float difference = end - start;

	return (start + (difference * t));
}

bool onconfirm_time_set_menu(MenuItem<int> choice) {
	switch (choice.value) {//预设的时间点
	case 0:
		// 半夜 00:00
		movetime_set(0, 0);
		break;
	case 1:
		// 早晨 06:00
		movetime_set(6, 0);
		break;
	case 2:
		// 上午 08:00
		movetime_set(8, 0);
		break;
	case 3:
		// 上午 10:00
		movetime_set(10, 0);
		break;
	case 4:
		// 中午 12:00
		movetime_set(12, 0);
		break;
	case 5:
		// 下午 14:00
		movetime_set(14, 0);
		break;
	case 6:
		// 下午 16:00
		movetime_set(16, 0);
		break;
	case 7:
		// 傍晚 18:00
		movetime_set(18, 0);
		break;
	case 8:
		// 晚上 20:00
		movetime_set(20, 0);
		break;
	case 9:
		// 晚上 22:00
		movetime_set(22, 0);
		break;
	}

	return false;
}

void onconfirm_time_flow_rate(MenuItem<int> choice) {
	if (timeFlowRateLocked = !timeFlowRateLocked) {
		std::ostringstream ss;
		ss << "时间流速: " << TIME_FLOW_RATE_CAPTIONS.at(choice.value);
		set_status_text(ss.str());
	}
}

void onchange_game_speed_callback(int value, SelectFromListMenuItem* source) {
	timeSpeedIndex = value;
	std::ostringstream ss;
	ss << "游戏速度: " << TIME_SPEED_CAPTIONS.at(value);
	set_status_text(ss.str());
}

void onchange_aiming_speed_callback(int value, SelectFromListMenuItem* source) {
	timeSpeedIndexWhileAiming = value;
	std::ostringstream ss;
	ss << "瞄准速度: " << TIME_SPEED_CAPTIONS.at(value);
	set_status_text(ss.str());
}

void onchange_time_flow_rate_callback(int value, SelectFromListMenuItem* source) {
	timeFlowRateIndex = value, timeFlowRateChanged = true, timeFlowRateLocked = false;
}

void onchange_hotkey_flow_rate_callback(int value, SelectFromListMenuItem* source) {
	HotkeyFlowRateIndex = value, HotkeyFlowRateChanged = true, HotkeyFlowRateLocked = false;
}

void onchange_hotkey_freeze_unfreeze_time() {
	if (timeFlowRateIndex != 0) {
		frozentimestate = timeFlowRateIndex;
		timeFlowRateIndex = 0;
		timeFlowRateChanged = true;
		set_status_text("时间已冻结！");
		requireRefreshOfTime = true;
	}
	else
	{
		if (frozentimestate != -1) {
			timeFlowRateIndex = frozentimestate;
			timeFlowRateChanged = true;
		}
		else {
			timeFlowRateIndex = DEFAULT_TIME_FLOW_RATE;
			timeFlowRateChanged = true;
		}
		set_status_text("时间已解冻！");
		requireRefreshOfTime = true;
	}
}

bool onconfirm_time_flowrate_menu(MenuItem<int> choice) {
	if (choice.value == 0) {
		if (featureTimeSynced) {
			set_status_text("时间已与电脑系统同步！");
		}
	}
	else if (choice.value == 666) {
		onchange_hotkey_freeze_unfreeze_time();
	}
	return false;
}

bool flowtime_menu_interrupt() {
	if (requireRefreshOfTime) {
		return true;
	}
	return false;
}

void all_time_flow_rate() {
	do {
		requireRefreshOfTime = false;
		std::vector<MenuItem<int>*> menuItems;
		MenuItem<int>* item;
		int index = 0;

		ToggleMenuItem<int>* togItem = new ToggleMenuItem<int>();
		togItem->caption = "时间与电脑系统同步";
		togItem->value = 0;
		togItem->toggleValue = &featureTimeSynced;
		togItem->toggleValueUpdated = NULL;
		menuItems.push_back(togItem);

		SelectFromListMenuItem* listItem = new SelectFromListMenuItem(TIME_SPEED_CAPTIONS, onchange_hotkey_flow_rate_callback);
		listItem->wrap = false;
		listItem->caption = "全局游戏速度";
		listItem->value = HotkeyFlowRateIndex;
		menuItems.push_back(listItem);

		listItem = new SelectFromListMenuItem(TIME_SPEED_CAPTIONS, onchange_aiming_speed_callback);
		listItem->wrap = false;
		listItem->caption = "瞄准时的游戏速度";
		listItem->value = timeSpeedIndexWhileAiming;
		menuItems.push_back(listItem);

		togItem = new ToggleMenuItem<int>();
		togItem->caption = "仅在车辆内瞄准时的游戏速度";
		togItem->value = 0;
		togItem->toggleValue = &featureSpeedAimInVeh;
		togItem->toggleValueUpdated = NULL;
		menuItems.push_back(togItem);

		listItem = new SelectFromListMenuItem(TIME_FLOW_RATE_CAPTIONS, onchange_time_flow_rate_callback);
		listItem->caption = "时间流速";
		listItem->value = timeFlowRateIndex;
		listItem->wrap = false;
		listItem->onConfirmFunction = onconfirm_time_flow_rate;
		menuItems.push_back(listItem);

		item = new MenuItem<int>();
		item->caption = "冻结时间 [开/关]";
		item->value = 666;
		item->isLeaf = true;
		menuItems.push_back(item);

		togItem = new ToggleMenuItem<int>();
		togItem->caption = "显示当前游戏内时间";
		togItem->value = 0;
		togItem->toggleValue = &featureShowtime;
		togItem->toggleValueUpdated = NULL;
		menuItems.push_back(togItem);

		togItem = new ToggleMenuItem<int>();
		togItem->caption = "快速时间切换 [右Alt + 1-8, 右Alt + 小键盘 -/+]";
		togItem->value = 0;
		togItem->toggleValue = &featurehotkeytime;
		togItem->toggleValueUpdated = NULL;
		menuItems.push_back(togItem);

		draw_generic_menu<int>(menuItems, nullptr, "时间设置", onconfirm_time_flowrate_menu, nullptr, nullptr, flowtime_menu_interrupt);
	} while (requireRefreshOfTime);
}

void process_time_set_menu() {
	std::vector<MenuItem<int>*> menuItems;
	int index = 0;

	MenuItem<int>* item = new MenuItem<int>();
	item->caption = "半夜 12:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "早晨 6:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "上午 8:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "上午 10:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "中午 12:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "下午 2:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "下午 4:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "傍晚 6:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "晚上 8:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "晚上 10:00 点";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	draw_generic_menu<int>(menuItems, nullptr, "预设的时间点", onconfirm_time_set_menu, nullptr, nullptr, nullptr);
}

bool onconfirm_time_menu(MenuItem<int> choice) {
	switch (activeLineIndexTime) {
	case 0:
		process_time_set_menu();
		break;
	case 1:
		movetime_hour_forward();
		break;
	case 2:
		movetime_hour_backward();
		break;
	case 3:
		movetime_fivemin_forward();
		break;
	case 4:
		movetime_fivemin_backward();
		break;
	case 5:
		movetime_day_forward();
		break;
	case 6:
		movetime_day_backward();
		break;
	case 7:
		set_date();
		break;
	case 8:
		set_time();
		break;
	case 9:
		all_time_flow_rate();
		break;
	}
	return false;
}

void process_time_menu() {
	const std::string caption = "时间选项";

	std::vector<MenuItem<int>*> menuItems;

	int index = 0;

	MenuItem<int>* item = new MenuItem<int>();
	item->caption = "预设的时间点";
	item->value = -1;
	item->isLeaf = false;
	menuItems.insert(menuItems.begin(), item);

	item = new MenuItem<int>();
	item->caption = "前进 1 小时";// 前进
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "后退 1 小时";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "前进 5 分钟";// 前进
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "后退 5 分钟";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "前进 1 天";// 前进
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "后退 1 天";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "设置日期 (年/月/日)";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "设置时间 (时/分)";
	item->value = index++;
	item->isLeaf = true;
	menuItems.push_back(item);

	item = new MenuItem<int>();
	item->caption = "时间设置";
	item->value = index++;
	item->isLeaf = false;
	menuItems.insert(menuItems.end(), item);

	draw_generic_menu<int>(menuItems, &activeLineIndexTime, caption, onconfirm_time_menu, nullptr, nullptr, nullptr);
}

void reset_time_globals() {
	featureTimeSynced = false;
	timeFlowRateChanged = true;
	HotkeyFlowRateChanged = true;
	featureShowtime = false;
	featurehotkeytime = false;
	featureSpeedAimInVeh = false;

	timeSpeedIndexWhileAiming = DEFAULT_TIME_SPEED;
	timeSpeedIndex = DEFAULT_TIME_SPEED;
	timeFlowRateIndex = DEFAULT_TIME_FLOW_RATE;
	HotkeyFlowRateIndex = DEFAULT_HOTKEY_FLOW_RATE;
}

void add_time_feature_enablements(std::vector<FeatureEnabledLocalDefinition>* results) {
	results->push_back(FeatureEnabledLocalDefinition{ "featureTimeSynced", &featureTimeSynced });
	results->push_back(FeatureEnabledLocalDefinition{ "featureShowtime", &featureShowtime });
	results->push_back(FeatureEnabledLocalDefinition{ "featurehotkeytime", &featurehotkeytime });
	results->push_back(FeatureEnabledLocalDefinition{ "featureSpeedAimInVeh", &featureSpeedAimInVeh });
}

void movetime_day_forward() {
	/*
	bool timeWasPaused = featureTimePaused;
	TIME::PAUSE_CLOCK(true);
	*/

	int calDay = TIME::GET_CLOCK_DAY_OF_MONTH();
	int calMon = TIME::GET_CLOCK_MONTH();
	int calYear = TIME::GET_CLOCK_YEAR();

	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	bool leapYear = false;
	if (calYear % 4 == 0) {
		leapYear = true;
	}

	/*
	std::ostringstream ss2;
	ss2 << "Date is: ";
	ss2 << std::setfill('0') << std::setw(2) << calDay;
	ss2 << ".";
	ss2 << std::setfill('0') << std::setw(2) << calMon;
	ss2 << ".";
	ss2 << calYear;
	set_status_text(ss2.str());
	*/

	if ((calDay == 27 && calMon == 2 && !leapYear) ||
		(calDay == 28 && calMon == 2 && leapYear) ||
		(calDay == 30 && (calMon == 4 || calMon == 6 || calMon == 9 || calMon == 11)) ||
		(calDay == 31)) {
		calDay = 1;
		if (calMon == 12) {
			calMon = 1;
			calYear++;
		}
		else {
			calMon++;
		}
	}
	else {
		calDay++;
	}

	TIME::SET_CLOCK_DATE(calDay, calMon, calYear);
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 0);

	std::ostringstream ss;
	ss << "当前日期: "; // 左下角 日期提示
	ss << std::setfill('0') << std::setw(4) << TIME::GET_CLOCK_YEAR(); // 年，格式化为4位数（不足补零）
	ss << "."; // 分隔符
	ss << std::setfill('0') << std::setw(2) << TIME::GET_CLOCK_MONTH(); // 月，格式化为两位数（不足补零）
	ss << "."; // 分隔符
	ss << std::setfill('0') << std::setw(2) << TIME::GET_CLOCK_DAY_OF_MONTH(); // 日，格式化为两位数（不足补零）
	ss << "  "; // 分隔符
	ss << get_day_of_game_week(); // 星期
	set_status_text(ss.str()); // 将格式化后的字符串设置为状态文本

	//TIME::PAUSE_CLOCK(timeWasPaused);
}

void movetime_day_backward() {
	int calDay = TIME::GET_CLOCK_DAY_OF_MONTH();
	int calMon = TIME::GET_CLOCK_MONTH();
	int calYear = TIME::GET_CLOCK_YEAR();

	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	bool leapYear = false;
	if (calYear % 4 == 0) {
		leapYear = true;
	}

	if (calDay != 1) {
		calDay--;
	}
	else if (calMon == 1) {
		calDay = 31;
		calMon = 12;
		calYear--;
	}
	else {
		if (calMon == 5 || calMon == 7 || calMon == 10 || calMon == 12) {
			calDay = 30;
		}
		if (calMon == 3) {
			if (leapYear) {
				calDay = 29;
			}
			else {
				calDay = 28;
			}
		}
		else {
			calDay = 31;
		}
		calMon--;
	}

	TIME::SET_CLOCK_DATE(calDay, calMon, calYear);
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 0);

	std::ostringstream ss;
	ss << "当前日期: "; // 左下角 日期提示
	ss << std::setfill('0') << std::setw(4) << calYear; // 年，格式化为4位数（不足补零）
	ss << "."; // 分隔符
	ss << std::setfill('0') << std::setw(2) << calMon; // 月，格式化为两位数（不足补零）
	ss << "."; // 分隔符
	ss << std::setfill('0') << std::setw(2) << calDay; // 日，格式化为两位数（不足补零）
	ss << "  "; // 分隔符
	ss << get_day_of_game_week(); // 星期
	set_status_text(ss.str()); // 将格式化后的字符串设置为状态文本
}

void set_date() {
	keyboard_on_screen_already = true;
	curr_message = "输入新日期 (年/月/日):"; // 提示用户输入格式
	std::string lastDateSpawn;
	std::string tmp_Year, tmp_Mon, tmp_Day;   // 年、月、日字段

	std::string result = show_keyboard("手动输入日期", (char*)lastDateSpawn.c_str());
	if (!result.empty()) {
		result = trim(result); // 去除首尾空白
		lastDateSpawn = result;

		std::string a = result;
		int found_separator = 0; // 记录分隔符数量
		bool found_symbol = false; // 标记是否遇到数字

		// 支持的分隔符：/、空格、.
		std::string separators = "/ .";

		// 解析输入字符串
		for (int i = 0; i < a.size(); i++) {
			bool is_separator = (separators.find(a[i]) != std::string::npos);
			bool is_digit = (a[i] >= '0' && a[i] <= '9');

			// 如果遇到非数字、非分隔符的字符，返回默认日期
			if (!is_separator && !is_digit) {
				TIME::SET_CLOCK_DATE(1, 1, 2025);
				set_status_text("~r~错误: ~s~日期格式设置不正确！\n已恢复默认日期: 2025.01.01");
				return;
			}

			if (!is_separator) {
				found_symbol = true; // 标记已找到数字
			}
			if (is_separator && found_symbol) {
				found_separator++; // 遇到分隔符且之前有数字，计数加1
				found_symbol = false;
			}

			if (is_digit) {
				if (found_separator == 0) tmp_Year += a[i]; // 年
				else if (found_separator == 1) tmp_Mon += a[i]; // 月
				else if (found_separator == 2) tmp_Day += a[i]; // 日
			}
		}

		// 验证输入完整性：必须有2个分隔符且字段非空
		if (found_separator != 2 || tmp_Year.empty() || tmp_Mon.empty() || tmp_Day.empty()) {
			TIME::SET_CLOCK_DATE(1, 1, 2025);
			set_status_text("~r~错误: ~s~日期格式设置不正确！\n已恢复默认日期: 2025.01.01");
			return;
		}

		// 保存原始输入值，用于检测是否有非法输入
		std::string original_year = tmp_Year;
		std::string original_month = tmp_Mon;
		std::string original_day = tmp_Day;

		// 限制长度和范围，并处理异常
		int year = 2025, month = 1, day = 1; // 默认值
		std::string::size_type sz;
		bool date_modified = false; // 标记日期是否被修改过

		try {
			// 年份处理 - 添加更严格的限制
			if (tmp_Year.length() > 4) {
				year = 2025; // 长度大于4位，使用默认年份
				date_modified = true;
			}
			else {
				year = std::stoi(tmp_Year, &sz);
				// 年份范围限制在合理区间，例如1945-2099
				if (year < 1945 || year > 2099) {
					date_modified = true;
					year = (year < 1945) ? 1945 : 2099;
				}
			}

			// 月份处理
			if (tmp_Mon.length() > 2) {
				month = 1; // 长度大于2位，使用默认月份
				date_modified = true;
			}
			else {
				month = std::stoi(tmp_Mon, &sz);
				if (month < 1 || month > 12) {
					date_modified = true;
					month = (month < 1) ? 1 : 12;
				}
			}

			// 日期处理 - 根据月份确定最大天数
			int max_days = 31; // 默认最大天数
			if (month == 4 || month == 6 || month == 9 || month == 11) {
				max_days = 30;
			}
			else if (month == 2) {
				// 简单闰年判断
				if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
					max_days = 29;
				}
				else {
					max_days = 28;
				}
			}

			if (tmp_Day.length() > 2) {
				day = 1; // 长度大于2位，使用默认天数
				date_modified = true;
			}
			else {
				day = std::stoi(tmp_Day, &sz);
				if (day < 1 || day > max_days) {
					date_modified = true;
					day = (day < 1) ? 1 : max_days;
				}
			}
		}
		catch (const std::exception& e) {
			// 解析失败，返回默认日期
			TIME::SET_CLOCK_DATE(1, 1, 2025);
			set_status_text("~r~错误: ~s~日期解析失败！\n已恢复默认日期: 2025.01.01");
			return;
		}

		// 设置日期
		TIME::SET_CLOCK_DATE(day, month, year);

		// 根据是否修改了日期来显示不同的状态信息
		std::ostringstream ss;
		if (date_modified ||
			original_year != std::to_string(year) ||
			original_month != std::to_string(month) ||
			original_day != std::to_string(day)) {
			// 有非法值，显示警告信息
			ss << "~r~警告: ~s~输入日期 " << original_year << "." << original_month << "." << original_day;
			ss << " 超出范围, 已调整为: ";
			ss << std::setfill('0') << std::setw(4) << year; // 年
			ss << ".";
			ss << std::setfill('0') << std::setw(2) << month; // 月
			ss << ".";
			ss << std::setfill('0') << std::setw(2) << day; // 日
		}
		else {
			// 合法值，显示正常信息
			ss << "当前日期: ";
			ss << std::setfill('0') << std::setw(4) << year; // 年
			ss << ".";
			ss << std::setfill('0') << std::setw(2) << month; // 月
			ss << ".";
			ss << std::setfill('0') << std::setw(2) << day; // 日
			ss << "  " << get_day_of_game_week(); // 星期
		}
		set_status_text(ss.str());
	}
	else {
		// 输入为空，返回默认日期
		TIME::SET_CLOCK_DATE(1, 1, 2025);
		set_status_text("用户已经取消输入！\n恢复默认日期: 2025.01.01");
	}
}

void set_time() {
	keyboard_on_screen_already = true;  // 标记键盘已在屏幕上
	curr_message = "输入新时间 (时/分):";  // 设置提示信息
	std::string lastTimeSpawn;  // 用于存储上一次输入的时间
	std::string result = show_keyboard("手动输入时间", (char*)lastTimeSpawn.c_str());  // 显示键盘并获取用户输入

	if (!result.empty()) {  // 用户输入非空
		result = trim(result);  // 去除首尾空白
		lastTimeSpawn = result;  // 保存用户输入

		std::string a = result;  // 临时存储输入字符串
		int found_separator = 0;  // 记录分隔符数量
		bool found_symbol = false;  // 标记是否遇到有效字符
		std::string separators = ": .";  // 支持的分隔符：冒号、空格、点

		std::string tmp_Hour, tmp_Min;  // 存储小时和分钟的字符串

		// 解析输入字符串
		for (int i = 0; i < a.size(); i++) {
			bool is_separator = (separators.find(a[i]) != std::string::npos);  // 检查是否为分隔符
			bool is_digit = (a[i] >= '0' && a[i] <= '9');  // 检查是否为数字

			if (!is_separator && !is_digit) {  // 遇到非法字符
				movetime_set(12, 0);  // 设置默认时间
				set_status_text("~r~错误: ~s~时间格式设置不正确！\n已恢复默认时间: 12:00");
				return;
			}

			if (!is_separator) {  // 非分隔符字符
				found_symbol = true;
			}
			if (is_separator && found_symbol) {  // 遇到分隔符且之前有数字
				found_separator++;
				found_symbol = false;
			}

			if (is_digit) {  // 处理数字字符
				if (found_separator == 0) tmp_Hour += a[i];  // 小时部分
				else if (found_separator == 1) tmp_Min += a[i];  // 分钟部分
			}
		}

		// 验证输入完整性
		if (found_separator != 1 || tmp_Hour.empty() || tmp_Min.empty()) {  // 格式错误
			movetime_set(12, 0);
			set_status_text("~r~错误: ~s~时间格式设置不正确！\n已恢复默认时间: 12:00");
			return;
		}

		// 保存原始输入值，用于后续比较
		std::string original_hour = tmp_Hour;
		std::string original_min = tmp_Min;

		// 处理时间值并捕获异常
		int hour = 12, min = 0;  // 默认时间
		bool time_modified = false;  // 标记时间是否被调整

		try {
			// 处理小时
			if (tmp_Hour.length() > 2) {  // 长度超限，截断
				tmp_Hour.resize(2);
				time_modified = true;
			}
			hour = std::stoi(tmp_Hour);  // 转换为整数
			if (hour > 23) {  // 超出范围，调整
				hour = 23;
				time_modified = true;
			}
			else if (hour < 0) {
				hour = 0;
				time_modified = true;
			}

			// 处理分钟
			if (tmp_Min.length() > 2) {  // 长度超限，截断
				tmp_Min.resize(2);
				time_modified = true;
			}
			min = std::stoi(tmp_Min);  // 转换为整数
			if (min > 59) {  // 超出范围，调整
				min = 59;
				time_modified = true;
			}
			else if (min < 0) {
				min = 0;
				time_modified = true;
			}
		}
		catch (const std::exception& e) {  // 解析异常
			movetime_set(12, 0);
			set_status_text("~r~错误: ~s~时间解析失败！\n已恢复默认时间: 12:00");
			return;
		}

		// 设置游戏时间
		movetime_set(hour, min);

		// 显示状态信息
		std::ostringstream ss;  // 用于构建提示信息
		if (time_modified || original_hour != std::to_string(hour) || original_min != std::to_string(min)) {
			// 时间被调整，显示警告
			ss << "~r~警告: ~s~输入时间 " << original_hour << ":" << original_min;
			ss << "\n超出范围, 已调整为: ";
			ss << std::setfill('0') << std::setw(2) << hour << ":";
			ss << std::setfill('0') << std::setw(2) << min;
		}
		else {
			// 正常设置，显示当前时间
			ss << "当前时间: ";
			ss << std::setfill('0') << std::setw(2) << hour << ":";
			ss << std::setfill('0') << std::setw(2) << min;
		}
		set_status_text(ss.str());  // 显示提示信息
	}
	else {  // 用户取消输入
		movetime_set(12, 0);
		set_status_text("用户已经取消输入！\n恢复默认时间: 12:00");
	}
}

void movetime_hour_forward() {
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();
	gameHour++;
	if (gameHour == 24) {
		movetime_day_forward();
		gameHour = 00;
	}
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "当前时间:  %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_hour_backward() {
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();
	gameHour--;
	if (gameHour == -1) {
		movetime_day_backward();
		gameHour = 23;
	}
	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "当前时间:  %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_fivemin_forward() {
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	if (gameHour == 23 && gameMins > 54) {
		movetime_day_forward();
		gameHour = 0;
		gameMins = (gameMins + (-55));
	}
	else if (gameMins > 54) {
		gameHour++;
		gameMins = gameMins + (-55);
	}
	else {
		gameMins = gameMins + 5;
	}

	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "当前时间:  %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_fivemin_backward() {
	int gameHour = TIME::GET_CLOCK_HOURS();
	int gameMins = TIME::GET_CLOCK_MINUTES();

	if (gameHour == 0 && gameMins < 5) {
		movetime_day_backward();
		gameHour = 23;
		gameMins = gameMins + 55;
	}
	else if (gameMins < 5) {
		gameHour--;
		gameMins = gameMins + 55;
	}
	else {
		gameMins = gameMins - 5;
	}

	TIME::SET_CLOCK_TIME(gameHour, gameMins, 00);
	char text[32];
	sprintf_s(text, "当前时间:  %02d:%02d", gameHour, gameMins);
	set_status_text(text);
}

void movetime_set(int hour, int minute) {
	TIME::SET_CLOCK_TIME(hour, minute, 0);
	char text[32];
	sprintf_s(text, "当前时间:  %02d:%02d", TIME::GET_CLOCK_HOURS(), TIME::GET_CLOCK_MINUTES());
	set_status_text(text);
}

void toggle_game_speed()
{
	if (HotkeyFlowRateIndex != DEFAULT_HOTKEY_FLOW_RATE && PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) HotkeyFlowRateLocked = !HotkeyFlowRateLocked;
	WAIT(100);
}

std::string get_day_of_game_week() {
	int day = TIME::GET_CLOCK_DAY_OF_WEEK();
	switch (day) {
	case 0:
		return "周日";
	case 1:
		return "周一";
	case 2:
		return "周二";
	case 3:
		return "周三";
	case 4:
		return "周四";
	case 5:
		return "周五";
	case 6:
		return "周六";
	}
	return std::string();
}

void handle_generic_settings_time(std::vector<StringPairSettingDBRow>* settings) {
	for (int i = 0; i < settings->size(); i++) {
		StringPairSettingDBRow setting = settings->at(i);
		if (setting.name.compare("timeSpeedIndexWhileAiming") == 0) {
			timeSpeedIndexWhileAiming = stoi(setting.value);
		}
		else if (setting.name.compare("timeFlowRateIndex") == 0) {
			timeFlowRateIndex = stoi(setting.value);
		}
		else if (setting.name.compare("HotkeyFlowRateIndex") == 0) {
			HotkeyFlowRateIndex = stoi(setting.value);
		}
	}
}

void add_time_generic_settings(std::vector<StringPairSettingDBRow>* results) {
	results->push_back(StringPairSettingDBRow{ "timeSpeedIndexWhileAiming", std::to_string(timeSpeedIndexWhileAiming) });
	results->push_back(StringPairSettingDBRow{ "timeFlowRateIndex", std::to_string(timeFlowRateIndex) });
	results->push_back(StringPairSettingDBRow{ "HotkeyFlowRateIndex", std::to_string(HotkeyFlowRateIndex) });
}

void update_time_features(Player player) {
	// 时间同步
	if (featureTimeSynced) {
		if (timeFlowRateIndex != DEFAULT_TIME_FLOW_RATE) {
			timeFlowRateIndex = DEFAULT_TIME_FLOW_RATE, timeFlowRateChanged = true;

		}

		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);
		TIME::SET_CLOCK_TIME(t.tm_hour, t.tm_min, t.tm_sec);
	}

	if ((PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && featureSpeedAimInVeh) || !featureSpeedAimInVeh) slow_aim = true;
	if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0) && featureSpeedAimInVeh) slow_aim = false;

	// 时间流逝速率
	if (timeFlowRateChanged) {
		timeFlowRateChanged = false;

		if (timeFlowRateIndex == DEFAULT_TIME_FLOW_RATE) {
			TIME::PAUSE_CLOCK(false);
		}
		else {
			TIME::PAUSE_CLOCK(true);
		}
		timeFactor = timeFlowRateIndex == 0 ? -1.0f : 1000.0f / TIME_FLOW_RATE_VALUES.at(timeFlowRateIndex);
		SYSTEM::SETTIMERA(0);
	}
	if (timeFlowRateIndex != DEFAULT_TIME_FLOW_RATE) {
		TIME::PAUSE_CLOCK(true);
		if (timeFlowRateIndex > 0) {
			int hours, minutes, seconds = static_cast<int>(static_cast<float>(SYSTEM::TIMERA()) / timeFactor);
			hours = seconds / 3600, seconds %= 3600;
			minutes = seconds / 60, seconds %= 60;
			SYSTEM::SETTIMERA(SYSTEM::TIMERA() - static_cast<int>(static_cast<float>(hours * 3600 + minutes * 60 + seconds) * timeFactor));
			TIME::ADD_TO_CLOCK_TIME(hours, minutes, seconds);
		}
	}

	if ((is_in_airbrake_mode() && is_airbrake_frozen_time()) || (is_in_prop_placement_mode() && is_prop_placement_frozen_time())) {
		GAMEPLAY::SET_TIME_SCALE(0.0f);
		weHaveChangedTimeScale = true;
	}
	else if (CONTROLS::IS_CONTROL_PRESSED(0, 19) || PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) {
		// 什么也不做，让游戏为我们选择速度
	}
	else if (is_hotkey_held_normal_speed()) {
		GAMEPLAY::SET_TIME_SCALE(1.0f);
		weHaveChangedTimeScale = true;
	}
	else if (is_hotkey_held_slow_mo()) {
		GAMEPLAY::SET_TIME_SCALE(0.0f);
		weHaveChangedTimeScale = true;
	}
	else if (is_hotkey_held_half_normal_speed()) {
		GAMEPLAY::SET_TIME_SCALE(0.4f);
		weHaveChangedTimeScale = true;
	}
	else if (!HotkeyFlowRateLocked && HotkeyFlowRateIndex != DEFAULT_HOTKEY_FLOW_RATE && PLAYER::IS_PLAYER_CONTROL_ON(player) && !PLAYER::IS_PLAYER_DEAD(PLAYER::PLAYER_ID())) { // 通过快捷键切换游戏速度
		GAMEPLAY::SET_TIME_SCALE(TIME_SPEED_VALUES.at(HotkeyFlowRateIndex));
		weHaveChangedTimeScale = true;
	}
	else if (PLAYER::IS_PLAYER_FREE_AIMING(player) && PLAYER::IS_PLAYER_CONTROL_ON(player) && slow_aim == true) {
		if (timeSinceAimingBegan == 0) {
			timeSinceAimingBegan = GetTickCount();
		}
		else { // 这必须修复一个bug：即使未瞄准时游戏仍然卡顿
			GAMEPLAY::SET_TIME_SCALE(1.0f);
			weHaveChangedTimeScale = true;
		}

		if ((GetTickCount() - timeSinceAimingBegan) < TIME_TO_SLOW_AIM) {
			float fullSpeedTime = weHaveChangedTimeScale ? TIME_SPEED_VALUES.at(timeSpeedIndex) : 1.0f;
			float targetTime = TIME_SPEED_VALUES.at(timeSpeedIndexWhileAiming);

			float progress = ((float)(GetTickCount() - timeSinceAimingBegan) / TIME_TO_SLOW_AIM);

			float rate = quadratic_time_transition(fullSpeedTime, targetTime, progress);

			GAMEPLAY::SET_TIME_SCALE(rate);
		}
		else {
			GAMEPLAY::SET_TIME_SCALE(TIME_SPEED_VALUES.at(timeSpeedIndexWhileAiming));
			weHaveChangedTimeScale = true;
		}
	}
	else if (weHaveChangedTimeScale) {
		GAMEPLAY::SET_TIME_SCALE(1.0f);
		weHaveChangedTimeScale = false;
	}

	if (timeSinceAimingBegan > 0 && !(PLAYER::IS_PLAYER_FREE_AIMING(player) && PLAYER::IS_PLAYER_CONTROL_ON(player))) {
		timeSinceAimingBegan = 0;
	}

	// 显示当前时间
	if (featureShowtime && menu_showing == false) {
		int currHours = TIME::GET_CLOCK_HOURS(); // 获取当前小时（0-23）
		int currMins = TIME::GET_CLOCK_MINUTES(); // 获取当前分钟（0-59）
		int currSecs = TIME::GET_CLOCK_SECONDS(); // 获取当前秒数（0-59）
		int calDay = TIME::GET_CLOCK_DAY_OF_MONTH(); // 获取当前日期（1-31）
		int calMon = TIME::GET_CLOCK_MONTH(); // 获取当前月份（1-12）
		int calYear = TIME::GET_CLOCK_YEAR(); // 获取当前年份（例如 2023）
		int day = TIME::GET_CLOCK_DAY_OF_WEEK(); // 获取当前星期（0-6，0表示星期日）

		char hours_to_show_char_modifiable[3];
		char mins_to_show_char_modifiable[3];
		char secs_to_show_char_modifiable[3];
		sprintf(hours_to_show_char_modifiable, "%d", currHours);
		sprintf(mins_to_show_char_modifiable, "%d", currMins);
		sprintf(secs_to_show_char_modifiable, "%d", currSecs);
		char* hours_to_show_char = "60";
		char* minutes_to_show_char = "60";
		char* seconds_to_show_char = "60";
		if (currHours == 0 || currHours == 60 || calDay == 0) hours_to_show_char = "00";
		if (currHours == 1 || calDay == 1) hours_to_show_char = "01";
		if (currHours == 2 || calDay == 2) hours_to_show_char = "02";
		if (currHours == 3 || calDay == 3) hours_to_show_char = "03";
		if (currHours == 4 || calDay == 4) hours_to_show_char = "04";
		if (currHours == 5 || calDay == 5) hours_to_show_char = "05";
		if (currHours == 6 || calDay == 6) hours_to_show_char = "06";
		if (currHours == 7 || calDay == 7) hours_to_show_char = "07";
		if (currHours == 8 || calDay == 8) hours_to_show_char = "08";
		if (currHours == 9 || calDay == 9) hours_to_show_char = "09";
		if (currMins == 0 || currMins == 60) minutes_to_show_char = "00";
		if (currMins == 1) minutes_to_show_char = "01";
		if (currMins == 2) minutes_to_show_char = "02";
		if (currMins == 3) minutes_to_show_char = "03";
		if (currMins == 4) minutes_to_show_char = "04";
		if (currMins == 5) minutes_to_show_char = "05";
		if (currMins == 6) minutes_to_show_char = "06";
		if (currMins == 7) minutes_to_show_char = "07";
		if (currMins == 8) minutes_to_show_char = "08";
		if (currMins == 9) minutes_to_show_char = "09";
		if (currSecs == 0 || currSecs == 60) seconds_to_show_char = "00";
		if (currSecs == 1) seconds_to_show_char = "01";
		if (currSecs == 2) seconds_to_show_char = "02";
		if (currSecs == 3) seconds_to_show_char = "03";
		if (currSecs == 4) seconds_to_show_char = "04";
		if (currSecs == 5) seconds_to_show_char = "05";
		if (currSecs == 6) seconds_to_show_char = "06";
		if (currSecs == 7) seconds_to_show_char = "07";
		if (currSecs == 8) seconds_to_show_char = "08";
		if (currSecs == 9) seconds_to_show_char = "09";
		// 小时
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		if (currHours > 9 && currHours < 60) UI::_ADD_TEXT_COMPONENT_SCALEFORM(hours_to_show_char_modifiable);
		else UI::_ADD_TEXT_COMPONENT_SCALEFORM(hours_to_show_char);
		UI::_DRAW_TEXT(0.003, 0.185);
		// :
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(":");
		UI::_DRAW_TEXT(0.013, 0.185);
		// 分钟
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		if (currMins > 9 && currMins < 60) UI::_ADD_TEXT_COMPONENT_SCALEFORM(mins_to_show_char_modifiable);
		else UI::_ADD_TEXT_COMPONENT_SCALEFORM(minutes_to_show_char);
		UI::_DRAW_TEXT(0.017, 0.185);
		// :
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(":");
		UI::_DRAW_TEXT(0.027, 0.185);
		// 秒
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 255);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 255);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		if (currSecs > 9 && currSecs < 60) UI::_ADD_TEXT_COMPONENT_SCALEFORM(secs_to_show_char_modifiable);
		else UI::_ADD_TEXT_COMPONENT_SCALEFORM(seconds_to_show_char);
		UI::_DRAW_TEXT(0.031, 0.185);
		GRAPHICS::DRAW_RECT(0.0, 0.20, 0.10, 0.03, 10, 10, 10, 100);

		char day_to_show_char_modifiable[10];
		char year_to_show_char_modifiable[10];
		sprintf(day_to_show_char_modifiable, "%d", calDay);
		sprintf(year_to_show_char_modifiable, "%d", calYear);
		char* month_to_show_char = "0";
		if (calMon == 1) month_to_show_char = "1 月";
		if (calMon == 2) month_to_show_char = "2 月";
		if (calMon == 3) month_to_show_char = "3 月";
		if (calMon == 4) month_to_show_char = "4 月";
		if (calMon == 5) month_to_show_char = "5 月";
		if (calMon == 6) month_to_show_char = "6 月";
		if (calMon == 7) month_to_show_char = "7 月";
		if (calMon == 8) month_to_show_char = "8 月";
		if (calMon == 9) month_to_show_char = "9 月";
		if (calMon == 10) month_to_show_char = "10 月";
		if (calMon == 11) month_to_show_char = "11 月";
		if (calMon == 12) month_to_show_char = "12 月";
		char* week_to_show_char = "0";
		if (day == 0) week_to_show_char = "星期日";
		if (day == 1) week_to_show_char = "星期一";
		if (day == 2) week_to_show_char = "星期二";
		if (day == 3) week_to_show_char = "星期三";
		if (day == 4) week_to_show_char = "星期四";
		if (day == 5) week_to_show_char = "星期五";
		if (day == 6) week_to_show_char = "星期六";

		// 一周中的星期
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 200);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 100);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(week_to_show_char);
		UI::_DRAW_TEXT(0.003, 0.210);
		// 一个月中的日期
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 200);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 100);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		if (calDay > 9 && calDay < 32) UI::_ADD_TEXT_COMPONENT_SCALEFORM(day_to_show_char_modifiable);
		else UI::_ADD_TEXT_COMPONENT_SCALEFORM(hours_to_show_char);
		UI::_DRAW_TEXT(0.003, 0.230);
		// 一年中的月份
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 200);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 100);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(month_to_show_char);
		UI::_DRAW_TEXT(0.003, 0.250);
		// 当前世纪中的年份
		UI::SET_TEXT_FONT(4);
		UI::SET_TEXT_SCALE(0.0, 0.45);
		UI::SET_TEXT_PROPORTIONAL(1);
		UI::SET_TEXT_COLOUR(255, 242, 0, 200);//透明度
		UI::SET_TEXT_EDGE(3, 0, 0, 0, 255);
		UI::SET_TEXT_DROPSHADOW(10, 10, 10, 10, 100);
		UI::SET_TEXT_OUTLINE();
		UI::_SET_TEXT_ENTRY("STRING");
		UI::_ADD_TEXT_COMPONENT_SCALEFORM(year_to_show_char_modifiable);
		UI::_DRAW_TEXT(0.003, 0.270);
	} // 显示当前时间结束

	if (featurehotkeytime) {
		if (GetKeyState(VK_RMENU) & 0x8000) {
			PED::SET_PED_CAN_SWITCH_WEAPON(PLAYER::PLAYER_PED_ID(), false);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(19);
			UI::HIDE_HUD_COMPONENT_THIS_FRAME(20);
		}
		else if (veh_to_spawn == "") PED::SET_PED_CAN_SWITCH_WEAPON(PLAYER::PLAYER_PED_ID(), true);

		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('1') & 0x8000) {
			movetime_set(0, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('2') & 0x8000) {
			movetime_set(5, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('3') & 0x8000) {
			movetime_set(6, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('4') & 0x8000) {
			movetime_set(8, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('5') & 0x8000) {
			movetime_set(12, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('6') & 0x8000) {
			movetime_set(16, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('7') & 0x8000) {
			movetime_set(18, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && GetKeyState('8') & 0x8000) {
			movetime_set(21, 0);
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && (/*(GetKeyState(VK_OEM_PLUS) & 0x8000) || */(GetKeyState('0') & 0x8000) || (GetKeyState(VK_ADD) & 0x8000))) {
			movetime_fivemin_forward();
		}
		if (GetKeyState(VK_RMENU) & 0x8000 && (/*(GetKeyState(VK_OEM_MINUS) & 0x8000) || */(GetKeyState('9') & 0x8000) || (GetKeyState(VK_SUBTRACT) & 0x8000))) {
			movetime_fivemin_backward();
		}
	}

} // 更新时间功能结束
