// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//                 This file is #include'd from traincar.h and main.cpp
//
// =======================================================================
//
// There are a number of additional functions used in main.cpp that
// you need to declare and implement.  Study main.cpp and the provided
// output to determine the arguments, return type, and overall
// behavior.
//
// Add all required additional function prototypes here
// (you may also add your own helper function prototypes here too)
//
// Implement these functions in "traincar.cpp"
//
#ifndef traincar_p_
#define traincar_p_
#include <vector>

void DeleteAllCars(TrainCar* train);
void TotalWeightAndCountCars(TrainCar* train, int& total_weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleeping_cars);
float CalculateSpeed(TrainCar* train);
float AverageDistanceToDiningCar(TrainCar* train);
int ClosestEngineToSleeperCar(TrainCar* train);
void PushBack(TrainCar*& train, TrainCar* newcar);

std::vector<TrainCar*> ShipFreight(TrainCar* & engines, TrainCar* & cars, int min_speed, int max_cars);

void Separate(TrainCar* & fulltrain, TrainCar* & small1, TrainCar* & small2);

void MoveCarBetweenTrains(TrainCar* & toTrain, TrainCar* & fromTrain);

#endif