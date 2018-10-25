#ifndef _train_prototypes_h_
#define _train_prototypes_h_
#include <vector>
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


//FROM SimpleTrainTest
void PushBack(TrainCar*& first_car, TrainCar* car);
void DeleteAllCars(TrainCar*& pointer);

//HELPER FUNCTIONS
TrainCar* PopFront(TrainCar*& first_car);
TrainCar* PopBack(TrainCar*& last_car);
void PushFront(TrainCar*& last_car, TrainCar* car);

//FROM ShipFreightHelper
void TotalWeightAndCountCars(TrainCar* car, int& weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleeping_cars);
float CalculateSpeed(TrainCar* car);
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights, int min_speed, int max_cars);

float AverageDistanceToDiningCar(TrainCar* train);
int ClosestEngineToSleeperCar(TrainCar* train);
void Separate(TrainCar*& original, TrainCar*& separated1, TrainCar*& separated2);

#endif
