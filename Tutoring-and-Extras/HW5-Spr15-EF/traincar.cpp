// =======================================================================
//
// IMPORTANT NOTE: You should edit this file
//
// =======================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "traincar.h"

// =======================================================================
// =======================================================================
//
//  HERE'S WHERE YOU SHOULD IMPLEMENT THE FUNCTIONS YOU PROTOTYPED IN
//  THE "traincar_prototypes.h" FILE.
//

//Gets the weight, and number of each type of car
void TotalWeightAndCountCars(TrainCar* train, int& total_weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleeping_cars){
	TrainCar* t = train;
	total_weight = 0;
	num_engines = 0;
	num_freight_cars = 0;
	num_passenger_cars = 0;
	num_dining_cars = 0;
	num_sleeping_cars = 0;
	for(; t->next != NULL; t = t->next){
		total_weight += (*t).getWeight();
		if((*t).isEngine()){
			num_engines++;
		} else if ((*t).isFreightCar()){
			num_freight_cars++;
		} else if((*t).isPassengerCar()){
			num_passenger_cars++;
		} else if((*t).isDiningCar()){
			num_dining_cars++;
		} else if((*t).isSleepingCar()){
			num_sleeping_cars++;
		}
	}
	total_weight += (*t).getWeight();
	if((*t).isEngine()){
		num_engines++;
	} else if ((*t).isFreightCar()){
		num_freight_cars++;
	} else if((*t).isPassengerCar()){
		num_passenger_cars++;
	} else if((*t).isDiningCar()){
		num_dining_cars++;
	} else if((*t).isSleepingCar()){
		num_sleeping_cars++;
	}
}

//Calculate speed based on the given formula
float CalculateSpeed(TrainCar* train){
	int total_weight,num_engines,num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars;
  	TotalWeightAndCountCars(train,total_weight,num_engines,num_freight_cars,num_passenger_cars,num_dining_cars,num_sleeping_cars);
  	float top = ((float)num_engines)*3000*550*3600;
  	float bottom = 20*2*5280*total_weight;
  	// std::cout << "num_engines: " << num_engines << ", top: " << top << std::endl;
  	// std::cout << "total_weight: " << total_weight << ", bottom: " << bottom << std::endl;
  	// std::cout << "speed: " << top/bottom << std::endl;
  	return top/bottom;
}

float AverageDistanceToDiningCar(TrainCar* train){
	float num_passenger_cars = 0;
	float total_distance = 0;
	for(TrainCar* p = train; p != NULL; p = p->next){
		if(p->isPassengerCar()){
			num_passenger_cars++;
			float left_dist = 0;
			float right_dist = 0;
			TrainCar* q = p;
			while( !(q->isDiningCar()) && (q->next != NULL) ){
				right_dist++;
				q = q->next;
			}
			if( !(q->isDiningCar()) ){
				right_dist = 9999;
			}
			TrainCar* t = p;
			while( !(t->isDiningCar()) && (t->prev != NULL) ){
				left_dist++;
				t = t->prev;
			}
			if( !(t->isDiningCar()) ){
				left_dist = 9999;
			}
			if(left_dist < right_dist){
				total_distance += left_dist;
			} else{
				total_distance += right_dist;
			}
		}
	}
	if(num_passenger_cars == 0){ return 0; }

	return total_distance/num_passenger_cars;
}

int ClosestEngineToSleeperCar(TrainCar* train){
	std::vector<int> distances;
	int num_sleeping_cars = 0;
	for(TrainCar* p = train; p != NULL; p = p->next){
		if(p->isSleepingCar()){
			num_sleeping_cars++;
			int left_dist = 0;
			int right_dist = 0;
			TrainCar* q = p;
			while( !(q->isEngine()) && (q->next != NULL) ){
				right_dist++;
				q = q->next;
			}
			if( !(q->isEngine()) ){
				right_dist = 9999;
			}
			TrainCar* t = p;
			while( !(t->isEngine()) && (t->prev != NULL) ){
				left_dist++;
				t = t->prev;
			}
			if( !(t->isEngine()) ){
				left_dist = 9999;
			}
			if(left_dist < right_dist){
				distances.push_back(left_dist);
			} else{
				distances.push_back(right_dist);
			}
		}
	}
	if(distances.size() == 0){ return 0; }
	int shortest = 99999;
	for(int i = 0; i < distances.size(); i++){
		if(distances[i] < shortest){
			shortest = distances[i];
		}
	}
	
	return shortest;
}

std::vector<TrainCar*> ShipFreight(TrainCar* & engines, TrainCar* & cars, int min_speed, int max_cars){
	std::vector<TrainCar*> trains;

	// std::cout << "vector created" << std::endl;
	while( (engines != NULL) && (cars != NULL) ){
		TrainCar* newtrain = NULL;
		MoveCarBetweenTrains(newtrain, engines);
		int num_cars_added = 1;
		// std::cout << "train made with engine added" << std::endl;
		// std::cout << "min speed: " << min_speed << ",\ttrain speed: \n" << CalculateSpeed(newtrain) << std::endl;
		//addcars:
		while( (CalculateSpeed(newtrain) > min_speed) && (num_cars_added < max_cars) ){
			if(cars == NULL){ break; }
			MoveCarBetweenTrains(newtrain, cars);
			num_cars_added++;
			//std::cout << num_cars_added << std::endl;
		}
		if(num_cars_added == max_cars){
			if(CalculateSpeed(newtrain) < min_speed){
				MoveCarBetweenTrains(cars, newtrain);
				num_cars_added--;
			}
		} else{
			if(CalculateSpeed(newtrain) < min_speed){
				MoveCarBetweenTrains(newtrain, engines);
				num_cars_added++;
				if(CalculateSpeed(newtrain) < min_speed){
					MoveCarBetweenTrains(engines, newtrain);
					MoveCarBetweenTrains(cars, newtrain);
					num_cars_added -= 2;
				} /*else{
					goto addcars;
				}*/

			}
		}
		trains.push_back(newtrain);

	}


	return trains;
}

void MoveCarBetweenTrains(TrainCar* & toTrain, TrainCar* & fromTrain){
	if(fromTrain == NULL){ return; }
	TrainCar* transferCar = fromTrain;
	
	//TrainCar* newFromTrainStart = transferCar->next;
	if(transferCar->prev != NULL){
		transferCar->prev->next = transferCar->next;
	}
	if(transferCar->next != NULL){
		transferCar->next->prev = transferCar->prev;
		fromTrain = transferCar->next;
	} else{
		fromTrain = transferCar->prev;
	}
	/*if(transferCar->prev != NULL){
		transferCar->prev->next = transferCar->next;
	}*/

	if( toTrain == NULL ){
		transferCar->prev = NULL;
		transferCar->next = NULL;
		toTrain = transferCar;
	} else{
		transferCar->next = toTrain;
		transferCar->prev = toTrain->prev;
		if(toTrain->prev != NULL){
			toTrain->prev->next = transferCar;
		}
		toTrain->prev = transferCar;
		toTrain = transferCar;
	}
}



void Separate(TrainCar* & fulltrain, TrainCar* & small1, TrainCar* & small2){

}

void DeleteAllCars(TrainCar* train){
	if(train == NULL){ return; }

	TrainCar* t = train;
	//runs through all cars, deleting the one behind it
	while(t->next != NULL){
		t = t->next;
		delete t->prev;
	}
	//deleting the last remaining car
	delete t;
	train = NULL;
}

//add a new car to the train
void PushBack(TrainCar*& train, TrainCar* newcar){
	if(train == NULL){
		train = newcar;
	} else {
		TrainCar* t = train;
		//run until there is no car in the next spot
		while(t->next != NULL){
			t = t->next;
		}
		//add the car to the doubly-linked list of cars
		t->next = newcar;
		newcar->prev = t;
	}
}