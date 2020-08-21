#pragma once

//static float get_vehicle_fuel_level(Vehicle vehicle);
static void set_vehicle_fuel_level(Vehicle vehicle, int fuelOffset, float fuelValue);
static BYTE* GetAddress(Vehicle handle);

extern bool featureFuel;
extern bool featureFuelGauge;
extern bool featureHideFuelBar;

//Car Fuel
const std::vector<std::string> VEH_CARFUEL_CAPTIONS{ "NO", "0.1", "0.5", "1.0", "1.4", "1.8", "2.2", "2.6", "2.8", "3.0", "3.3", "3.6", "5.0", "6.0", "8.0", "10.0" }; // , "13.0", "15.0", "18.0", "20.0", "23.0", "25.0"
const int VEH_CARFUEL_VALUES[] = { 0, 20000000, 10000000, 5000000, 3500000, 3000000, 2600000, 2200000, 1800000, 1700000, 1600000, 1400000, 1300000, 1000000, 400000, 100000 }; // , 50000, 10000, 5000, 1000, 500, 100
extern int CarConsumptionIndex;
extern bool CarConsumptionChanged;
extern int BikeConsumptionIndex;
extern bool BikeConsumptionChanged;
extern int PlaneConsumptionIndex;
extern bool PlaneConsumptionChanged;
extern int BoatConsumptionIndex;
extern bool BoatConsumptionChanged;
extern int HeliConsumptionIndex;
extern bool HeliConsumptionChanged;
extern int IdleConsumptionIndex;
extern bool IdleConsumptionChanged;

//Refueling Speed
const std::vector<std::string> VEH_REFUELSPEED_CAPTIONS{ "0.1", "0.5", "0.9", "1.0", "1.5", "1.9", "2.1", "2.2", "2.3", "2.5", "3.0", "5.0", "10.0" };
const double VEH_REFUELSPEED_VALUES[] = { 0.000001, 0.000005, 0.000009, 0.00001, 0.00005, 0.00009, 0.0001, 0.0002, 0.0003, 0.0005, 0.001, 0.005, 0.01 };
extern int RefuelingSpeedIndex;
extern bool RefuelingSpeedChanged;

//Fuel Price
const std::vector<std::string> VEH_FUELPRICE_CAPTIONS{ "NO", "0.1", "0.5", "1.0", "2.0", "3.0", "4.0", "5.0", "10.0", "20.0", "30.0", "50.0", "100.0", "200.0", "300.0", "500.0", "1000.0", "5000.0", "10000.0" };
const double VEH_FUELPRICE_VALUES[] = { 0, 0.1, 0.5, 1, 2, 3, 4, 5, 10, 20, 30, 50, 100, 200, 300, 500, 1000, 5000, 10000 };
extern int FuelPriceIndex;
extern bool FuelPriceChanged;
extern int JerrycanPriceIndex;
extern bool JerrycanChanged;

//Min Fuel Random Number
const std::vector<std::string> VEH_FUELRANDOM1_CAPTIONS{ "10", "20", "30", "40", "50", "60", "70" };
const int VEH_FUELRANDOM1_VALUES[] = { 1, 2, 3, 4, 5, 6, 7 };
extern int Random1Index;
extern bool Random1Changed;

//Min Fuel Random Number 2
const std::vector<std::string> VEH_FUELRANDOM2_CAPTIONS{ "20", "30", "40", "50", "60", "70", "80" };
const int VEH_FUELRANDOM2_VALUES[] = { 1, 2, 3, 4, 5, 6, 7 };
extern int Random2Index;
extern bool Random2Changed;

//Fuel Bar Position
const std::vector<std::string> VEH_FUELBARPOSITION_CAPTIONS{ "Below Radar", "Above Radar", "On The Left Of Radar", "On The Right Of Radar" };
const int VEH_FUELBARPOSITION_VALUES[] = { 1, 2, 3, 4 };
extern int BarPositionIndex;
extern bool BarPositionChanged;

//Fuel colours_R
const std::vector<std::string> FUEL_COLOURS_R_CAPTIONS{ "OFF", "10", "20", "30", "40", "50", "60", "70", "80", "90", "100", "110", "120", "130", "140", "150", "160", "170", "180", "190", "200", "210", "220", "230", "240", "250", "255" };
const int FUEL_COLOURS_R_VALUES[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 255 };
extern int FuelColours_R_Index;
extern bool FuelColours_R_Changed;
extern int FuelColours_G_Index;
extern bool FuelColours_G_Changed;
extern int FuelColours_B_Index;
extern bool FuelColours_B_Changed;

//Show Blips
const std::vector<std::string> VEH_FUELBLIPS_CAPTIONS{ "OFF", "Everywhere", "On Radar Only", "If Phone In Hand Only" };
const int VEH_FUELBLIPS_VALUES[] = { 0, 1, 2, 3 };
extern int FuelBlipsIndex;
extern bool FuelBlipsChanged;
extern int FuelBackground_Opacity_Index;
extern bool FuelBackgound_Opacity_Changed;

void fuel();

extern std::vector<Vehicle> VEHICLES;
extern std::vector<float> FUEL;

extern std::vector<int> GAS_X;
extern std::vector<int> GAS_Y;
extern std::vector<int> GAS_Z;