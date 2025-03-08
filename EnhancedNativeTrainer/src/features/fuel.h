#pragma once

// 获取载具燃油等级的静态函数
static void set_vehicle_fuel_level(Vehicle vehicle, int fuelOffset, float fuelValue);
static BYTE* GetAddress(Vehicle handle);

extern bool featureFuel;
extern bool featureFuelGauge;
extern bool featureHideFuelBar;

// 汽车燃油
const std::vector<std::string> VEH_CARFUEL_CAPTIONS{ "关", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15" };
const int VEH_CARFUEL_VALUES[] = { 0, 20000000, 10000000, 5000000, 3000000, 2000000, 1500000, 1000000, 800000, 700000, 650000, 600000, 400000, 350000, 220000, 100000 };
//const std::vector<std::string> VEH_CARFUEL_CAPTIONS{ "NO", "0.1", "0.5", "1.0", "1.4", "1.8", "2.2", "2.6", "2.8", "3.0", "3.3", "3.6", "5.0", "6.0", "8.0", "10.0" };
//const int VEH_CARFUEL_VALUES[] = { 0, 20000000, 10000000, 5000000, 3500000, 3000000, 2600000, 2200000, 1800000, 1700000, 1600000, 1400000, 1300000, 1000000, 400000, 100000 };
extern int CarConsumptionIndex;
extern int BikeConsumptionIndex;
extern int PlaneConsumptionIndex;
extern int BoatConsumptionIndex;
extern int HeliConsumptionIndex;
extern int IdleConsumptionIndex;

// 加油速度
const std::vector<std::string> VEH_REFUELSPEED_CAPTIONS{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14" };
const int VEH_REFUELSPEED_VALUES[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
extern int RefuelingSpeedIndex;

// 燃油价格
const std::vector<std::string> VEH_FUELPRICE_CAPTIONS{ "关", "0.1", "0.5", "1.0", "2.0", "3.0", "4.0", "5.0", "10.0", "20.0", "30.0", "50.0", "100.0", "200.0", "300.0", "500.0", "1000.0", "5000.0", "10000.0" };
const double VEH_FUELPRICE_VALUES[] = { 0, 0.1, 0.5, 1, 2, 3, 4, 5, 10, 20, 30, 50, 100, 200, 300, 500, 1000, 5000, 10000 };
extern int FuelPriceIndex;
extern int JerrycanPriceIndex;

// 燃油随机数
const float VEH_FUELRANDOM_VALUES[] = { 0.0, 1.5, 3.0, 4.5, 6.0, 7.0, 8.0, 9.5, 11.0, 12.5, 14.0 };
extern int Random1Index; // 最小燃油随机数
extern int Random2Index; // 最大燃油随机数

// 燃油条位置
const std::vector<std::string> VEH_FUELBARPOSITION_CAPTIONS{ "小地图下方", "小地图上方", "小地图左侧", "小地图右侧" };
extern int BarPositionIndexN;

// 燃油颜色_R    定义 透明度（Alpha 值）
const std::vector<std::string> FUEL_COLOURS_R_CAPTIONS{ "禁用", "完全隐形", "60", "70", "80", "90", "100", "110", "120", "130", "140", "150", "160", "170", "180", "190", "200", "205", "210", "215", "220", "225", "230", "235", "240", "245", "250", "255" }; // "20", "40", "60", "80", "100", "120",  "140", "160", "180", "200", "220", "240",
const int FUEL_COLOURS_R_VALUES[] = { 0, 10, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255 }; // 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240,
extern int FuelColours_R_IndexN;
extern bool FuelColours_R_Changed;
extern int FuelColours_G_IndexN;
extern bool FuelColours_G_Changed;
extern int FuelColours_B_IndexN;
extern bool FuelColours_B_Changed;

// 显示标记
const std::vector<std::string> VEH_FUELBLIPS_CAPTIONS{ "关", "全部显示", "仅小地图显示", "打开手机时显示" };
extern int FuelBlipsIndex;
extern int FuelBackground_Opacity_IndexN;

void fuel();

extern std::vector<Vehicle> VEHICLES;
extern std::vector<float> FUEL;

extern std::vector<int> GAS_X;
extern std::vector<int> GAS_Y;
extern std::vector<int> GAS_Z;