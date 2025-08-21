/*
增强版原生训练器项目的一部分。
https://github.com/gtav-ent/GTAV-EnhancedNativeTrainer
(C) Rob Pridham 及其他贡献者 2015
*/

#include "config_io.h"
#include "keyboard.h"
#include "..\debug\debuglog.h"
#include "..\ui_support\entcolor.h"
#include <sstream>
#include "..\features\script.h"
#include "..\features\fuel.h"

// 一个全局的 Windows "基本字符串"。实际内存由 MSXML 使用的 COM 方法分配，
// 这些方法会使用 &keyconf_bstr。我们必须使用 SysFreeString() 
// 在后续使用前释放此内存，以防止内存泄漏。
BSTR keyconf_bstr;

TrainerConfig *config = NULL;

// 创建默认的 XML 配置文件
void write_default_config_xml_file() {
    write_text_to_log_file("正在创建默认的 ent-config.xml 配置文件...");
    
    // 确保目录存在
    CreateDirectory("Enhanced Native Trainer", NULL);
    
    std::ofstream xmlFile("Enhanced Native Trainer/ent-config.xml");
    if (xmlFile.is_open()) {

        // 写入配置文件 ent-config.xml 开始

		xmlFile << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
		xmlFile << "<!--" << std::endl;
		xmlFile << "    可用按键值的列表请参阅 https://blog.csdn.net/mystonelxj/article/details/88184829（改成国内网站了，不会打不开）" << std::endl;
		xmlFile << "    理论上支持大部分或所有按键值。" << std::endl;
		xmlFile << "    目前，如果你想要控制器(手柄)输入，需要首先使用一些工具将其映射到某个按键。" << std::endl;
		xmlFile << "    如果你搞砸了这个文件并输入了无效的按键，将使用默认值。" << std::endl;
		xmlFile << "    如果你设法将同一个按键分配给多个功能，谁知道会发生什么？" << std::endl;
		xmlFile << "    " << std::endl;
		xmlFile << "    使用 VK_NOTHING (键值)表示没有分配。" << std::endl;
		xmlFile << "    " << std::endl;
		xmlFile << "    -->" << std::endl;
		xmlFile << "<ent-config>" << std::endl;
		xmlFile << "	<!-- 主要按键(快捷键)设置 -->" << std::endl;
		xmlFile << "	<keys>" << std::endl;
		xmlFile << "	<key function=\"toggle_main_menu\" value=\"VK_F4\"/>" << std::endl;
		xmlFile << "	<!-- 开启关闭主菜单，默认按键为 F4 -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<key function=\"menu_up\" value=\"VK_NUMPAD8\"/>" << std::endl;
		xmlFile << "	<!-- 向上移动菜单，按键为数字键盘的 8 -->" << std::endl;
		xmlFile << "	<key function=\"menu_down\" value=\"VK_NUMPAD2\"/>" << std::endl;
		xmlFile << "	<!-- 向下移动菜单，按键为数字键盘的 2 -->" << std::endl;
		xmlFile << "	<key function=\"menu_left\" value=\"VK_NUMPAD4\"/>" << std::endl;
		xmlFile << "	<!-- 向左移动菜单，按键为数字键盘的 4 -->" << std::endl;
		xmlFile << "	<key function=\"menu_right\" value=\"VK_NUMPAD6\"/>" << std::endl;
		xmlFile << "	<!-- 向右移动菜单，按键为数字键盘的 6 -->" << std::endl;
		xmlFile << "	<key function=\"menu_select\" value=\"VK_NUMPAD5\"/>" << std::endl;
		xmlFile << "	<!-- 选择菜单选项，按键为数字键盘的 5 -->" << std::endl;
		xmlFile << "	<key function=\"menu_back\" value=\"VK_NUMPAD0\"/>" << std::endl;
		xmlFile << "	<!-- 返回上一级菜单，按键为数字键盘的 0 -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 车辆加速、需要开启加速度  -->" << std::endl;
		xmlFile << "	<key function=\"veh_boost\" value=\"VK_NUMPAD9\"/>" << std::endl;
		xmlFile << "	<!-- 立即加速车辆，按键为 小键盘数字9 -->" << std::endl;
		xmlFile << "	<key function=\"veh_stop\" value=\"VK_NUMPAD3\"/>" << std::endl;
		xmlFile << "	<!-- 立即停止车辆，按键为 小键盘数字3 -->" << std::endl;
		xmlFile << "	<key function=\"veh_rockets\" value=\"VK_ADD\"/>" << std::endl;
		xmlFile << "	<!-- 发射车辆火箭，按键为 加号键 -->" << std::endl;
		xmlFile << "	<key function=\"veh_leftblink\" value=\"VK_LEFT\"/>" << std::endl;
		xmlFile << "	<!-- 左转向灯，按键为 左箭头键 -->" << std::endl;
		xmlFile << "	<key function=\"veh_rightblink\" value=\"VK_RIGHT\"/>" << std::endl;
		xmlFile << "	<!-- 右转向灯，按键为 右箭头键 -->" << std::endl;
		xmlFile << "	<key function=\"veh_emergencyblink\" value=\"VK_DECIMAL\"/>" << std::endl;
		xmlFile << "	<!-- 打开双闪，按键为 小数点键 -->" << std::endl;
		xmlFile << "	<key function=\"veh_startrefueling\" value=\"VK_KEY_E\"/>" << std::endl;
		xmlFile << "	<!-- 开始加油，按键为 E 键 -->" << std::endl;
		xmlFile << "	<key function=\"veh_stoprefueling\" value=\"VK_KEY_S\"/>" << std::endl;
		xmlFile << "	<!-- 停止加油，按键为 S 键 -->" << std::endl;
		xmlFile << "	" << std::endl;
		xmlFile << "	<key function=\"veh_canrefueling\" value=\"VK_LBUTTON\"/>" << std::endl;
		xmlFile << "	<!-- 车辆加油功能，按键为 鼠标左键 -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<key function=\"toggle_airbrake\" value=\"VK_F6\"/>" << std::endl;
		xmlFile << "	<!-- 开启/关闭，自由移动功能，按键为 F6 -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<key function=\"airbrake_up\" value=\"VK_KEY_Q\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，上升，按键为 Q -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_down\" value=\"VK_KEY_Z\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，下降，按键为 Z -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_forward\" value=\"VK_KEY_W\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，前进，按键为 W -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_back\" value=\"VK_KEY_S\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，后退，按键为 S -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_rotate_left\" value=\"VK_KEY_A\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，左转，按键为 A -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_rotate_right\" value=\"VK_KEY_D\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，右转，按键为 D -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_move_aside\" value=\"VK_SPACE\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，侧移，按键为空格键 -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_mouse_control\" value=\"VK_KEY_M\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，鼠标控制，按键为 M -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_speed\" value=\"VK_SHIFT\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，加速，按键为 Shift -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_freeze_time\" value=\"VK_KEY_T\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，冻结时间，按键为 T -->" << std::endl;
		xmlFile << "	<key function=\"airbrake_help\" value=\"VK_KEY_H\"/>" << std::endl;
		xmlFile << "	<!-- 自由移动，隐藏帮助，按键为 H -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<key function=\"objectplacer_up\" value=\"VK_KEY_Q\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，上升，按键为 Q -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_down\" value=\"VK_KEY_Z\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，下降，按键为 Z -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_forward\" value=\"VK_KEY_W\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，前进，按键为 W -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_back\" value=\"VK_KEY_S\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，后退，按键为 S -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_rotate_left\" value=\"VK_KEY_A\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，左旋转，按键为 A -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_rotate_right\" value=\"VK_KEY_D\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，右旋转，按键为 D -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_speed_up\" value=\"VK_OEM_6\"/>" << std::endl;
		xmlFile << "	<!-- 物体放置，加速，按键为   ]   -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_speed_down\" value=\"VK_OEM_4\"/>" << std::endl;
		xmlFile << "	<!-- 物体放置，减速，按键为   [   -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_freeze_time\" value=\"VK_KEY_T\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，冻结时间，按键为 T -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_freeze_position\" value=\"VK_KEY_G\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，切换物体退出时的冻结状态 G -->" << std::endl;
		xmlFile << "	<key function=\"objectplacer_help\" value=\"VK_KEY_H\"/>" << std::endl;
		xmlFile << "	<!-- 物体摆放，隐藏按键说明，按键为 H -->" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 以下是快捷键设置，值为 VK_NOTHING 表示空值，值为[false 禁用]对应的组合键，值为[true 启用]对应的组合键  -->" << std::endl;
		xmlFile << "	<!-- [hotkey_1]  至 [hotkey_9] 对应游戏里快捷键，编号1到9。你不能改变 [function] 的值 -->" << std::endl;
		xmlFile << "	<!-- 你可以改变 'value' 和 'modCtrl'， 'modAlt'， 'modShift'的值。-->" << std::endl;
		xmlFile << "	<!-- 你可以使用任何组合，例如 [CTRL + ALT + SHIFT+F1] 更改后：[ value=\"VK_F1\" modCtrl=\"true\" modAlt=\"true\" modShift=\"true\" ] -->" << std::endl;
		xmlFile << "	<!-- 或者你也可以不使用组合，例如 F2 [ value=\"VK_F2\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\" ] -->" << std::endl;
		xmlFile << "	<key function=\"hotkey_1\" value=\"VK_F5\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_2\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_3\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_4\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_5\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_6\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_7\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_8\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	<key function=\"hotkey_9\" value=\"VK_NOTHING\" modCtrl=\"false\" modAlt=\"false\" modShift=\"false\"/>" << std::endl;
		xmlFile << "	</keys>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 控制器(手柄)按键 -->" << std::endl;
		xmlFile << "	<controller_keys>" << std::endl;
		xmlFile << "	<controller function=\"KEY_TOGGLE_MAIN_MENU\" button1=\"206\" button2=\"192\"/>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_UP\" button1=\"188\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_DOWN\" button1=\"187\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_LEFT\" button1=\"189\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_RIGHT\" button1=\"190\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_SELECT\" button1=\"201\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_MENU_BACK\" button1=\"202\" button2=\"-1\"/>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_BOOST\" button1=\"206\" button2=\"24\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_STOP\" button1=\"205\" button2=\"25\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_ROCKETS\" button1=\"205\" button2=\"206\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_LEFTBLINK\" button1=\"25\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_RIGHTBLINK\" button1=\"24\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_EMERGENCYBLINK\" button1=\"202\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_STARTREFUELING\" button1=\"201\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_STOPREFUELING\" button1=\"202\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_VEH_CANREFUELING\" button1=\"203\" button2=\"-1\"/>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<controller function=\"KEY_TOGGLE_AIRBRAKE\" button1=\"201\" button2=\"206\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_UP\" button1=\"25\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_DOWN\" button1=\"24\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_FORWARD\" button1=\"32\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_BACK\" button1=\"33\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_ROTATE_LEFT\" button1=\"205\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_ROTATE_RIGHT\" button1=\"206\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_SPEED\" button1=\"201\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_AIRBRAKE_FREEZE_TIME\" button1=\"202\" button2=\"-1\"/>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_UP\" button1=\"25\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_DOWN\" button1=\"24\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_FORWARD\" button1=\"32\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_BACK\" button1=\"33\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_ROTATE_LEFT\" button1=\"205\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_ROTATE_RIGHT\" button1=\"206\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_SPEED_CYCLE\" button1=\"201\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_FREEZE_TIME\" button1=\"202\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	<controller function=\"KEY_OBJECTPLACER_FREEZE_POSITION\" button1=\"192\" button2=\"-1\"/>" << std::endl;
		xmlFile << "	</controller_keys>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 更改天气的顺序 -->" << std::endl;
		xmlFile << "	<weather_list>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_1\" name=\"EXTRASUNNY\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_2\" name=\"CLEAR\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_3\" name=\"CLOUDS\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_4\" name=\"SMOG\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_5\" name=\"FOGGY\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_6\" name=\"OVERCAST\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_7\" name=\"RAIN\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_8\" name=\"THUNDER\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_9\" name=\"CLEARING\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_10\" name=\"NEUTRAL\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_11\" name=\"SNOW\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_12\" name=\"BLIZZARD\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_13\" name=\"SNOWLIGHT\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_14\" name=\"XMAS\"/>" << std::endl;
		xmlFile << "	<weather position=\"WEATHER_15\" name=\"HALLOWEEN\"/>" << std::endl;
		xmlFile << "	</weather_list>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 加油站 -->" << std::endl;
		xmlFile << "	<gas_stations>" << std::endl;
		xmlFile << "	<station x_coord=\"-71\" y_coord=\"-1762\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-90\" y_coord=\"6415\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"180\" y_coord=\"6603\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"819\" y_coord=\"-1027\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1039\" y_coord=\"2669\" z_coord=\"39\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-2555\" y_coord=\"2334\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"2581\" y_coord=\"362\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"2683\" y_coord=\"3264\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-853\" y_coord=\"-1374\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"265\" y_coord=\"-1261\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-1799\" y_coord=\"803\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-724\" y_coord=\"-935\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"264\" y_coord=\"2609\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"50\" y_coord=\"2776\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1702\" y_coord=\"6418\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1212\" y_coord=\"2657\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"2537\" y_coord=\"2593\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1182\" y_coord=\"-330\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-526\" y_coord=\"-1212\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1209\" y_coord=\"-1402\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"2005\" y_coord=\"3775\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"621\" y_coord=\"269\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-2097\" y_coord=\"-320\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-1434\" y_coord=\"-274\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1687\" y_coord=\"4929\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1785\" y_coord=\"3330\" z_coord=\"41\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-1603\" y_coord=\"5260\" z_coord=\"30\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-724\" y_coord=\"-1444\" z_coord=\"5\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"1770\" y_coord=\"3240\" z_coord=\"42\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-1272\" y_coord=\"-3381\" z_coord=\"14\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"174\" y_coord=\"-1561\" z_coord=\"29\"/>" << std::endl;
		xmlFile << "	<station x_coord=\"-67\" y_coord=\"-2532\" z_coord=\"6\"/>" << std::endl;
		xmlFile << "	</gas_stations>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "	<!-- 地图物品列表 -->" << std::endl;
		xmlFile << "	<map_stuff_list>" << std::endl;
		xmlFile << "	<stuff sname=\"m23_2_cargoship\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"m23_2_cargoship_bridge\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"sf_yacht_02\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"sf_yacht_02_int\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"sum_lost_yacht\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"sum_lost_yacht_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_yacht_heist\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_gta_milo_enginrm\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_gta_milo_bedrm\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_gta_milo_bar\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_gta_milo_bridge\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"apa_yacht_grp04_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp04_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_carrier\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_int_mph_carrierhang3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_int_mph_carrierhang2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_int_mph_carrierupper\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_int_mph_carriercontrol1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"hei_int_mph_carriercontrol2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp04_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp04_1_int\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"sf_yacht_01\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"sf_yacht_01_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp04_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp04_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp05_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_01\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_01_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp06_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp07_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp08_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp09_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp10_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp11_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"gr_heist_yacht2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"gr_gta_milo_enginrm\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"gr_gta_milo_bedrm\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"gr_gta_milo_bar\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"gr_gta_milo_bridge\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp12_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_03\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_03_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp02_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_3_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp01_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_02\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"h4_islandx_yacht_02_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_2\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_2_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_1\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_1_int\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_3\"/>" << std::endl;
		xmlFile << "	<disabled_stuff sname=\"apa_yacht_grp03_3_int\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int1\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int2\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int3\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int4\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int5\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"M24_1_carrier_int6\"/>" << std::endl;
		xmlFile << "	<stuff sname=\"m24_1_carrier_ladders\"/>" << std::endl;
		xmlFile << "	</map_stuff_list>" << std::endl;
		xmlFile << "" << std::endl;
		xmlFile << "</ent-config>" << std::endl;

		// 配置文件 ent-config.xml 写入完毕

        xmlFile.close();
        write_text_to_log_file("成功创建默认的 ent-config.xml 配置文件");
    } else {
        write_text_to_log_file("创建 ent-config.xml 配置文件失败！");
    }
}

/**读取 XML 配置文件。当前文件包含键盘按键选择。*/
void read_config_file(){
    // 检查配置文件是否存在
    std::ifstream configFile("Enhanced Native Trainer/ent-config.xml");
    bool configExists = configFile.good();
    configFile.close();
    
    // 如果配置文件不存在，创建默认配置
    if (!configExists) {
        write_text_to_log_file("未找到 ent-config.xml 配置文件，将创建默认配置");
        write_default_config_xml_file();
    }

	TrainerConfig *result = new TrainerConfig();

	CoInitialize(NULL);

	//读取 XML
	MSXML2::IXMLDOMDocumentPtr spXMLDoc;
	spXMLDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if(!spXMLDoc->load("Enhanced Native Trainer/ent-config.xml")){
		write_text_to_log_file("加载配置文件失败，将使用默认配置。");
		config = result; // 默认配置
		return;
	}

	// 键盘绑定
	IXMLDOMNodeListPtr nodes = spXMLDoc->selectNodes(L"//ent-config/keys/key");
	long length;
	nodes->get_length(&length);
	for(int i = 0; i < length; i++){
		IXMLDOMNode *node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap *attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char *attrib_key_func = NULL;
		char *attrib_key_value = NULL;
		bool modCtrl = false;
		bool modAlt = false;
		bool modShift = false;

		for(long j = 0; j < length_attribs; j++){
			IXMLDOMNode *attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if(wcscmp(keyconf_bstr, L"function") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_func = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if(wcscmp(keyconf_bstr, L"value") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_key_value = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if(wcscmp(keyconf_bstr, L"modCtrl") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modCtrl = true;
				}
			}
			else if(wcscmp(keyconf_bstr, L"modAlt") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modAlt = true;
				}
			}
			else if(wcscmp(keyconf_bstr, L"modShift") == 0){
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				char* value = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if(value != 0 && _strcmpi(value, "true") == 0){
					modShift = true;
				}
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		if(attrib_key_func != NULL && attrib_key_value != NULL){
			result->get_key_config()->set_key(attrib_key_func, attrib_key_value, modCtrl, modAlt, modShift);
		}

		delete attrib_key_func;
		delete attrib_key_value;

		attribs->Release();
		node->Release();
	}

	// 控制器绑定
	nodes = spXMLDoc->selectNodes(L"//ent-config/controller_keys/controller");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_controller_func = NULL;
		int attrib_button1_v = NULL;
		int attrib_button2_v = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"function") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_controller_func = _com_util::ConvertBSTRToString(V_BSTR(&var));
			}
			else if (wcscmp(keyconf_bstr, L"button1") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_button1_v = std::stoi(_com_util::ConvertBSTRToString(V_BSTR(&var)));
			}
			else if (wcscmp(keyconf_bstr, L"button2") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_button2_v = std::stoi(_com_util::ConvertBSTRToString(V_BSTR(&var)));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		// 必须有一段代码用于将按键绑定存储到某个地方。
		if (attrib_controller_func != NULL) {
 
			if (attrib_button1_v == NULL)
			{
				std::stringstream ss;
				ss << "[错误] 读取时出现问题 " << attrib_controller_func << "的函数，Button1 的值为空 (NULL) 已跳过，将使用默认值替代。";
				write_text_to_log_file(ss.str());
				continue;
			}

			if (controller_binds.find(attrib_controller_func) != controller_binds.end())
			{
				std::stringstream ss;
				ss << "控制器函数 " << attrib_controller_func << " 指定值: " << attrib_button1_v << " 以及 " << attrib_button2_v;
				write_text_to_log_file(ss.str());
				controller_binds.at(attrib_controller_func) = std::pair(attrib_button1_v, attrib_button2_v);
			}
			else 
			{
				std::stringstream ss;
				ss << "[错误] 未找到控制器函数 " << attrib_controller_func << " 在控制器绑定映射中，跳过。";
				write_text_to_log_file(ss.str());
				continue;
			}
		}
		else
		{
			std::stringstream ss;
			ss << "[错误] 控制器函数与按钮 ID " << attrib_button1_v << " 以及 " << attrib_button2_v << " 值为  NULL  跳过。";
			write_text_to_log_file(ss.str());
			continue;
		}

		delete attrib_controller_func;
	
		attribs->Release();
		node->Release();
	}

	// 额外的映射相关功能列表
	nodes = spXMLDoc->selectNodes(L"//ent-config/map_stuff_list/stuff");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* map_stuff = NULL;
		
		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"sname") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				map_stuff = _com_util::ConvertBSTRToString(V_BSTR(&var));
				MAP_STUFF = MAP_STUFF + map_stuff + " ";
			}
			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete map_stuff;
		
		attribs->Release();
		node->Release();
	}

	// 天气列表
	nodes = spXMLDoc->selectNodes(L"//ent-config/weather_list/weather");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_weather = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"name") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_weather = _com_util::ConvertBSTRToString(V_BSTR(&var));
				if (i == 0) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 1) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 2) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 3) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 4) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 5) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 6) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 7) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 8) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 9) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 10) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 11) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 12) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 13) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var)) + " ";
				if (i == 14) C_WEATHER_C = C_WEATHER_C + _com_util::ConvertBSTRToString(V_BSTR(&var));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete attrib_weather;

		attribs->Release();
		node->Release();
	}

	// 加油站
	nodes = spXMLDoc->selectNodes(L"//ent-config/gas_stations/station");
	nodes->get_length(&length);
	for (int i = 0; i < length; i++) {
		IXMLDOMNode* node;
		nodes->get_item(i, &node);
		IXMLDOMNamedNodeMap* attribs;
		node->get_attributes(&attribs);

		long length_attribs;
		attribs->get_length(&length_attribs);

		char* attrib_station_x_coord = NULL;
		char* attrib_station_y_coord = NULL;
		char* attrib_station_z_coord = NULL;

		for (long j = 0; j < length_attribs; j++) {
			IXMLDOMNode* attribNode;
			attribs->get_item(j, &attribNode);
			attribNode->get_nodeName(&keyconf_bstr);
			if (wcscmp(keyconf_bstr, L"x_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_x_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_X.push_back(std::stoi(attrib_station_x_coord));
			}
			else if (wcscmp(keyconf_bstr, L"y_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_y_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_Y.push_back(std::stoi(attrib_station_y_coord));
			}
			else if (wcscmp(keyconf_bstr, L"z_coord") == 0) {
				VARIANT var;
				VariantInit(&var);
				attribNode->get_nodeValue(&var);
				attrib_station_z_coord = _com_util::ConvertBSTRToString(V_BSTR(&var));
				GAS_Z.push_back(std::stoi(attrib_station_z_coord));
			}

			SysFreeString(keyconf_bstr);
			attribNode->Release();
		}

		delete attrib_station_x_coord;
		delete attrib_station_y_coord;
		delete attrib_station_z_coord;

		attribs->Release();
		node->Release();
	}

	// nodes->Release(); // 不要调用此方法，它会在退出时导致崩溃
	spXMLDoc.Release();
	CoUninitialize();

	config = result;
}

void read_config_ini_file(){
	int tmpv[12][4];
	const char *sectionMenuColor = "菜单颜色", *file = "Enhanced Native Trainer/ent_customization.ini";
	const std::string tmpk[] = {"r", "g", "b", "a"};
	std::ifstream tmp(file);
	bool notexist = !((bool) tmp);
	tmp.close();

	bool formatError = false;

	if(notexist){
		write_text_to_log_file("颜色配置 INI 文件不存在！正在创建默认配置文件...");
		// 不存在时直接创建新文件
		write_config_ini_file();
		return;
	}

	// 读取配置并检查格式是否正确
	for(int a = 0; a < ENTColor::colsVarsNum; a++){
		for(int b = 0; b < 4; b++){
			tmpv[a][b] = GetPrivateProfileInt(sectionMenuColor, (ENTColor::colsVarsReverse.at(a) + tmpk[b]).c_str(), -1, file);
			// 检查值是否在有效范围内
			if(tmpv[a][b] < 0 || tmpv[a][b] > 255){
				formatError = true;
				break;
			}
		}
		if(formatError) break;
		
		// 配置格式正确，更新颜色值
		if(!(tmpv[a][0] < 0 || tmpv[a][1] < 0 || tmpv[a][2] < 0 || tmpv[a][3] < 0) || !(tmpv[a][0] > 255 || tmpv[a][1] > 255 || tmpv[a][2] > 255 || tmpv[a][3] > 255)){
			for(int b = 0; b < 4; b++){
				ENTColor::colsMenu[a].rgba[b] = tmpv[a][b];
			}
		}
	}

	// 如果检测到格式错误，重新创建配置文件
	if(formatError){
		write_text_to_log_file("颜色配置 INI 文件格式错误！正在创建新的默认配置文件...");
		// 删除旧文件
		std::remove(file);
		// 创建新的配置文件
		write_config_ini_file();
	}
}

void write_config_ini_file(){
	const char *sectionMenuColor = "菜单颜色", *file = "Enhanced Native Trainer/ent_customization.ini";
	const std::string tmpk[] = {"r", "g", "b", "a"};
	std::ofstream ini;
	std::ifstream tmp(file);
	bool notexist = !((bool) tmp);
	tmp.close();

	// 确保目录存在
	CreateDirectory("Enhanced Native Trainer", NULL);

	// 写入颜色配置
	for(int a = 0; a < ENTColor::colsVarsNum; a++){
		for(int b = 0; b < 4; b++){
			WritePrivateProfileString(sectionMenuColor, (ENTColor::colsVarsReverse.at(a) + tmpk[b]).c_str(), std::to_string(ENTColor::colsMenu[a].rgba[b]).c_str(), file);
		}
	}

	// 如果文件不存在，添加注释和格式化文件内容
	if(notexist){
		tmp.open(file);
		if(tmp.is_open()){
			std::vector<std::string> lines, result;
			for(std::string line; std::getline(tmp, line); ){
				lines.push_back(line);
			}
			tmp.close();

			result.push_back(std::string(";;;; 增强型 原生修改器：配置 INI 文件（开始） ;;;;\n\n;;; （删除此文件以恢复默认颜色配置） ;;;\n"));

			result.push_back(std::string(";; 菜单颜色（开始） ;;\n;\t请遵循 RGBA 颜色系统，每个颜色组件的值范围为 0 ~ 255 ;"));
			for(auto a : lines){
				for(int b = 0; b < ENTColor::colsVarsNum; b++){
					if(a.compare(0, ENTColor::colsVarsReverse.at(b).length() + 1, (ENTColor::colsVarsReverse.at(b) + tmpk[0])) == 0){
						result.push_back(std::string("; ") + ENTColor::colsCaptions[b] + std::string(" ;"));
						break;
					}
				}
				result.push_back(a);
			}
			result.push_back(std::string(";; 菜单颜色（结束） ;;"));

			result.push_back(std::string("\n;;;; 增强型 原生修改器：配置 INI 文件（结束） ;;;;"));

			ini.open(file, std::ofstream::out | std::ofstream::trunc);
			if(ini.is_open()){
				for(auto a : result){
					ini << a << std::endl;
				}

				ini.close();
			}
		}
	}
}

void write_config_file(){
	// 简单的存根实现 - 仅写入日志
	// TODO: 实现完整的XML配置保存功能
	write_text_to_log_file("按键配置保存请求已记录 - XML保存功能待实现");
}

void KeyInputConfig::set_key(char* function, char* keyName, bool modCtrl, bool modAlt, bool modShift){
	std::ostringstream ss;
	ss << "按键功能 " << function << " 被给予 " << keyName;
	write_text_to_log_file(ss.str());

	int vkID = keyNameToVal(keyName);
	if(vkID == -1){
		ss.str(""); ss.clear();
		ss << "按键功能 " << keyName << " 没有对应到一个值！";
		write_text_to_log_file(ss.str());
		return;
	}

	auto match = keyConfigs.find(function);
	if(match != keyConfigs.end()){
		KeyConfig* oldConfig = match->second;
		match->second = new KeyConfig(vkID);
		match->second->modCtrl = modCtrl;
		match->second->modAlt = modAlt;
		match->second->modShift = modShift;
		delete oldConfig;
	}
	else{
		ss.str(""); ss.clear();
		ss << "按键功能 " << function << " 没有对应到已知函数！";
		write_text_to_log_file(ss.str());
	}
};

bool KeyInputConfig::is_hotkey_assigned(int i){

	std::string target;
	switch(i){
		case 1:
			target = KeyConfig::KEY_HOT_1;
			break;
		case 2:
			target = KeyConfig::KEY_HOT_2;
			break;
		case 3:
			target = KeyConfig::KEY_HOT_3;
			break;
		case 4:
			target = KeyConfig::KEY_HOT_4;
			break;
		case 5:
			target = KeyConfig::KEY_HOT_5;
			break;
		case 6:
			target = KeyConfig::KEY_HOT_6;
			break;
		case 7:
			target = KeyConfig::KEY_HOT_7;
			break;
		case 8:
			target = KeyConfig::KEY_HOT_8;
			break;
		case 9:
			target = KeyConfig::KEY_HOT_9;
			break;
		default:
			return false;
	}

	KeyConfig* conf = this->keyConfigs[target];
	if(conf == NULL || conf->keyCode == 0){
		return false;
	}
	return true;
}

TrainerConfig::TrainerConfig(){
	this->keyConfig = new KeyInputConfig();
}

KeyInputConfig::KeyInputConfig(){
	this->keyConfigs[KeyConfig::KEY_TOGGLE_MAIN_MENU] = new KeyConfig(VK_F4);

	this->keyConfigs[KeyConfig::KEY_MENU_UP] = new KeyConfig(VK_NUMPAD8);
	this->keyConfigs[KeyConfig::KEY_MENU_DOWN] = new KeyConfig(VK_NUMPAD2);
	this->keyConfigs[KeyConfig::KEY_MENU_LEFT] = new KeyConfig(VK_NUMPAD4);
	this->keyConfigs[KeyConfig::KEY_MENU_RIGHT] = new KeyConfig(VK_NUMPAD6);
	this->keyConfigs[KeyConfig::KEY_MENU_SELECT] = new KeyConfig(VK_NUMPAD5);
	this->keyConfigs[KeyConfig::KEY_MENU_BACK] = new KeyConfig(VK_NUMPAD0);

	this->keyConfigs[KeyConfig::KEY_VEH_BOOST] = new KeyConfig(VK_NUMPAD9);
	this->keyConfigs[KeyConfig::KEY_VEH_STOP] = new KeyConfig(VK_NUMPAD3);
	this->keyConfigs[KeyConfig::KEY_VEH_ROCKETS] = new KeyConfig(VK_ADD);
	this->keyConfigs[KeyConfig::KEY_VEH_LEFTBLINK] = new KeyConfig(VK_LEFT);
	this->keyConfigs[KeyConfig::KEY_VEH_RIGHTBLINK] = new KeyConfig(VK_RIGHT);
	this->keyConfigs[KeyConfig::KEY_VEH_EMERGENCYBLINK] = new KeyConfig(VK_DECIMAL);
	this->keyConfigs[KeyConfig::KEY_VEH_STARTREFUELING] = new KeyConfig(VK_KEY_E);
	this->keyConfigs[KeyConfig::KEY_VEH_STOPREFUELING] = new KeyConfig(VK_KEY_S);
	this->keyConfigs[KeyConfig::KEY_VEH_CANREFUELING] = new KeyConfig(VK_LBUTTON);
	
	this->keyConfigs[KeyConfig::KEY_TOGGLE_AIRBRAKE] = new KeyConfig(VK_F6);

	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_UP] = new KeyConfig(VK_KEY_Q);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_DOWN] = new KeyConfig(VK_KEY_Z);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FORWARD] = new KeyConfig(VK_KEY_W);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_BACK] = new KeyConfig(VK_KEY_S);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_SPEED] = new KeyConfig(VK_SHIFT);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_HELP] = new KeyConfig(VK_KEY_H);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_SPACE] = new KeyConfig(VK_SPACE);
	this->keyConfigs[KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL] = new KeyConfig(VK_KEY_M);

	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_UP] = new KeyConfig(VK_KEY_Q);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_DOWN] = new KeyConfig(VK_KEY_Z);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FORWARD] = new KeyConfig(VK_KEY_W);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_BACK] = new KeyConfig(VK_KEY_S);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT] = new KeyConfig(VK_KEY_A);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT] = new KeyConfig(VK_KEY_D);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_UP] = new KeyConfig(VK_OEM_6);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN] = new KeyConfig(VK_OEM_4);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME] = new KeyConfig(VK_KEY_T);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_HELP] = new KeyConfig(VK_KEY_H);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION] = new KeyConfig(VK_KEY_G);
	this->keyConfigs[KeyConfig::KEY_OBJECTPLACER_ALT_MOVE] = new KeyConfig(VK_MENU);

	this->keyConfigs[KeyConfig::KEY_HOT_1] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_2] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_3] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_4] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_5] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_6] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_7] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_8] = new KeyConfig(0);
	this->keyConfigs[KeyConfig::KEY_HOT_9] = new KeyConfig(0);

}

KeyInputConfig::~KeyInputConfig(){
	for each (std::pair<std::string, KeyConfig*> conf in this->keyConfigs){
		delete conf.second;
	}
	keyConfigs.clear();
}

KeyConfig* KeyInputConfig::get_key(std::string function){
	auto match = keyConfigs.find(function);
	if(match != keyConfigs.end()){
		return match->second;
	}
	return NULL;
}

const std::string KeyConfig::KEY_TOGGLE_MAIN_MENU = std::string("toggle_main_menu");
const std::string KeyConfig::KEY_TOGGLE_AIRBRAKE = std::string("toggle_airbrake");

const std::string KeyConfig::KEY_MENU_UP = std::string("menu_up");
const std::string KeyConfig::KEY_MENU_DOWN = std::string("menu_down");
const std::string KeyConfig::KEY_MENU_LEFT = std::string("menu_left");
const std::string KeyConfig::KEY_MENU_RIGHT = std::string("menu_right");
const std::string KeyConfig::KEY_MENU_SELECT = std::string("menu_select");
const std::string KeyConfig::KEY_MENU_BACK = std::string("menu_back");

const std::string KeyConfig::KEY_VEH_BOOST = std::string("veh_boost");
const std::string KeyConfig::KEY_VEH_STOP = std::string("veh_stop");
const std::string KeyConfig::KEY_VEH_ROCKETS = std::string("veh_rockets");
const std::string KeyConfig::KEY_VEH_LEFTBLINK = std::string("veh_leftblink");
const std::string KeyConfig::KEY_VEH_RIGHTBLINK = std::string("veh_rightblink");
const std::string KeyConfig::KEY_VEH_EMERGENCYBLINK = std::string("veh_emergencyblink");
const std::string KeyConfig::KEY_VEH_STARTREFUELING = std::string("veh_startrefueling");
const std::string KeyConfig::KEY_VEH_STOPREFUELING = std::string("veh_stoprefueling");
const std::string KeyConfig::KEY_VEH_CANREFUELING = std::string("veh_canrefueling");

const std::string KeyConfig::KEY_AIRBRAKE_UP = std::string("airbrake_up");
const std::string KeyConfig::KEY_AIRBRAKE_DOWN = std::string("airbrake_down");
const std::string KeyConfig::KEY_AIRBRAKE_FORWARD = std::string("airbrake_forward");
const std::string KeyConfig::KEY_AIRBRAKE_BACK = std::string("airbrake_back");
const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_LEFT = std::string("airbrake_rotate_left");
const std::string KeyConfig::KEY_AIRBRAKE_ROTATE_RIGHT = std::string("airbrake_rotate_right");
const std::string KeyConfig::KEY_AIRBRAKE_SPEED = std::string("airbrake_speed");
const std::string KeyConfig::KEY_AIRBRAKE_FREEZE_TIME = std::string("airbrake_freeze_time");
const std::string KeyConfig::KEY_AIRBRAKE_HELP = std::string("airbrake_help");
const std::string KeyConfig::KEY_AIRBRAKE_SPACE = std::string("airbrake_move_aside");
const std::string KeyConfig::KEY_AIRBRAKE_MOUSE_CONTROL = std::string("airbrake_mouse_control");

const std::string KeyConfig::KEY_OBJECTPLACER_UP = std::string("objectplacer_up");
const std::string KeyConfig::KEY_OBJECTPLACER_DOWN = std::string("objectplacer_down");
const std::string KeyConfig::KEY_OBJECTPLACER_FORWARD = std::string("objectplacer_forward");
const std::string KeyConfig::KEY_OBJECTPLACER_BACK = std::string("objectplacer_back");
const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_LEFT = std::string("objectplacer_rotate_left");
const std::string KeyConfig::KEY_OBJECTPLACER_ROTATE_RIGHT = std::string("objectplacer_rotate_right");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_CYCLE = std::string("objectplacer_speed_cycle");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_UP = std::string("objectplacer_speed_up");
const std::string KeyConfig::KEY_OBJECTPLACER_SPEED_DOWN = std::string("objectplacer_speed_down");
const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_TIME = std::string("objectplacer_freeze_time");
const std::string KeyConfig::KEY_OBJECTPLACER_FREEZE_POSITION = std::string("objectplacer_freeze_position");
const std::string KeyConfig::KEY_OBJECTPLACER_HELP = std::string("objectplacer_help");
const std::string KeyConfig::KEY_OBJECTPLACER_ALT_MOVE = std::string("objectplacer_alt_move");

const std::string KeyConfig::KEY_HOT_1 = std::string("hotkey_1");
const std::string KeyConfig::KEY_HOT_2 = std::string("hotkey_2");
const std::string KeyConfig::KEY_HOT_3 = std::string("hotkey_3");
const std::string KeyConfig::KEY_HOT_4 = std::string("hotkey_4");
const std::string KeyConfig::KEY_HOT_5 = std::string("hotkey_5");
const std::string KeyConfig::KEY_HOT_6 = std::string("hotkey_6");
const std::string KeyConfig::KEY_HOT_7 = std::string("hotkey_7");
const std::string KeyConfig::KEY_HOT_8 = std::string("hotkey_8");
const std::string KeyConfig::KEY_HOT_9 = std::string("hotkey_9");

// 绑定名称 -> 按钮 ID 1 和按钮 ID 2。对于只有一个按钮 ID 的键 - 使用 -1 作为“无绑定”值。
std::map<std::string, std::pair<int, int>> controller_binds =
{
	{ "KEY_TOGGLE_MAIN_MENU", {206, 192} },
	{ "KEY_TOGGLE_AIRBRAKE", {201, 206} },
	{ "KEY_MENU_UP", {188, -1} },
	{ "KEY_MENU_DOWN", {187, -1} },
	{ "KEY_MENU_LEFT", {189, -1} },
	{ "KEY_MENU_RIGHT", {190, -1} },
	{ "KEY_MENU_SELECT", {201, 1} },
	{ "KEY_MENU_BACK", {202, -1} },
	{ "KEY_VEH_BOOST", {206, 24} },
	{ "KEY_VEH_STOP", {205, 25} },
	{ "KEY_VEH_ROCKETS", {205, 206} },
	{ "KEY_VEH_LEFTBLINK", {25, -1} },
	{ "KEY_VEH_RIGHTBLINK", {24, -1} },
	{ "KEY_VEH_EMERGENCYBLINK", {202, -1} },
	{ "KEY_VEH_STARTREFUELING", {201, -1} },
	{ "KEY_VEH_STOPREFUELING", {202, -1} },
	{ "KEY_VEH_CANREFUELING", {203, -1} },
	{ "KEY_AIRBRAKE_UP", {25, -1} },
	{ "KEY_AIRBRAKE_DOWN", {24, -1} },
	{ "KEY_AIRBRAKE_FORWARD", {32, -1} },
	{ "KEY_AIRBRAKE_BACK", {33, -1} },
	{ "KEY_AIRBRAKE_ROTATE_LEFT", {205, -1} },
	{ "KEY_AIRBRAKE_ROTATE_RIGHT", {206, -1} },
	{ "KEY_AIRBRAKE_SPEED", {201, -1} },
	{ "KEY_AIRBRAKE_FREEZE_TIME", {202, -1} },
	{ "KEY_AIRBRAKE_HELP", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_AIRBRAKE_SPACE", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_AIRBRAKE_MOUSE_CONTROL", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_UP", {25, -1} },
	{ "KEY_OBJECTPLACER_DOWN", {24, -1} },
	{ "KEY_OBJECTPLACER_FORWARD", {32, -1} },
	{ "KEY_OBJECTPLACER_BACK", {33, -1} },
	{ "KEY_OBJECTPLACER_ROTATE_LEFT", {205, -1} },
	{ "KEY_OBJECTPLACER_ROTATE_RIGHT", {206, -1} },
	{ "KEY_OBJECTPLACER_SPEED_CYCLE", {201, -1} },
	{ "KEY_OBJECTPLACER_SPEED_UP", {-1, -1} },  // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_SPEED_DOWN", {-1, -1} }, // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_FREEZE_TIME", {202, -1} },
	{ "KEY_OBJECTPLACER_FREEZE_POSITION", {192, -1} },
	{ "KEY_OBJECTPLACER_HELP", {-1, -1} },  // XML 中没有绑定吗？
	{ "KEY_OBJECTPLACER_ALT_MOVE", {-1, -1} }, // XML 中没有绑定吗？
};