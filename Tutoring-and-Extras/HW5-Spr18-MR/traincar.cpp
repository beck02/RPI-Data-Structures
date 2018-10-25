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
#include "traincar.h"
#include "traincar_prototypes.h"

// =======================================================================
// =======================================================================
//
//  HERE'S WHERE YOU SHOULD IMPLEMENT THE FUNCTIONS YOU PROTOTYPED IN
//  THE "traincar_prototypes.h" FILE.
//

//============================FOR SimpleTrainTest() FUNCTION===================
void PushBack(TrainCar*& first_car, TrainCar* car) {
	if (first_car==NULL) {
		first_car=car;
	}
	else{
		TrainCar* last_car = first_car;
		while (last_car->next!=NULL) {
			last_car=last_car->next;
		}
		car->prev=last_car;
		last_car->next=car;
	}
}

void DeleteAllCars(TrainCar*& pointer){
	if (pointer != NULL) {
		while (pointer->next != NULL) {
			pointer = pointer->next;
			delete pointer->prev;
		}
		delete pointer;
		pointer = NULL;
	}
}

//=============================HELPER FUNCTIONS=============================
TrainCar* PopFront(TrainCar*& first_car) {
	if (first_car==NULL) {
		return NULL;
	}
	TrainCar* popped = first_car;
	first_car = first_car->next;
	if (first_car!=NULL) {
		first_car->prev=NULL;
	}
	popped->prev=NULL;
	popped->next=NULL;
	return popped;
}

TrainCar* PopBack(TrainCar*& first_car){
	if (first_car==NULL) {
		return NULL;
	}
	TrainCar* popped = first_car;
	while (popped->next!=NULL) {
		popped=popped->next;
	}
	if (popped->prev!=NULL) {
		popped->prev->next=NULL;
	}
	popped->prev=NULL;
	popped->next=NULL;
	return popped;
}

TrainCar* Pop(TrainCar*& car){
	if (car==NULL) {
		return NULL;
	}
	TrainCar* popped = car;
	if (popped->prev!=NULL) {
		popped->prev->next = popped->next;
	}
	if (popped->next!=NULL) {
		popped->next->prev = popped->prev;
	}
	popped->prev=NULL;
	popped->next=NULL;
	return popped;
}

void PushFront(TrainCar*& first_car, TrainCar* car) {
	if (first_car==NULL) {
		first_car=car;
	}
	else if (first_car != NULL){
		first_car->prev = car;
		if (car!= NULL) {
			car->next = first_car;
			car->prev=NULL;
			first_car = car;
		}
	}
}

void InsertBefore(TrainCar* curr_car, TrainCar* new_car) {
	if (new_car == NULL){ return; }
	if (curr_car == NULL) {
		curr_car = new_car;
	}
	else{
		new_car->next = curr_car;
		new_car->prev = curr_car->prev;
		if (curr_car->prev != NULL){
			curr_car->prev->next = new_car;
		}
		curr_car->prev = new_car;
	}
}

void InsertAfter(TrainCar* curr_car, TrainCar* new_car) {
	if (new_car == NULL){ return; }
	if (curr_car == NULL) {
		curr_car = new_car;
	}
	else{
		new_car->prev = curr_car;
		new_car->next = curr_car->next;
		if (curr_car->next != NULL){
			curr_car->next->prev = new_car;
		}
		curr_car->next = new_car;
	}
}

//===============FOR ShipFreightHelper() FUNCTION=============================
void TotalWeightAndCountCars(TrainCar* car, int& weight, int& num_engines, int& num_freight_cars, int& num_passenger_cars, int& num_dining_cars, int& num_sleeping_cars) {
	//setting all passed in variables to 0
	weight = num_engines = num_freight_cars = num_passenger_cars = 0;
	num_dining_cars = num_sleeping_cars = 0;
	while (car != NULL) {
		weight += car->getWeight();
		if (car->isEngine()) {
			num_engines++;
		}
		else if (car->isFreightCar()) {
			num_freight_cars++;
		}
		else if (car->isPassengerCar()) {
			num_passenger_cars++;
		}
		else if (car->isDiningCar()) {
			num_dining_cars++;
		}
		else if (car->isSleepingCar()) {
			num_sleeping_cars++;
		}
		car = car->next;
	}
}

float CalculateSpeed(TrainCar* car) {
	int total_weight, num_engines, num_freight_cars, num_passenger_cars, num_dining_cars, num_sleeping_cars;
	TotalWeightAndCountCars(car, total_weight, num_engines, num_freight_cars, num_passenger_cars, num_dining_cars, num_sleeping_cars);
	float top = ((float)num_engines*3000)*550*3600;
	float bottom = 20*2*5280* (float)total_weight;
	float speed = top/bottom;
	return speed;
}

//makes vector with the fewest number of trains necessary to ship all freights.
//Each train pointer contains a train with the fewest number of engines.
std::vector<TrainCar*> ShipFreight(TrainCar*& engines, TrainCar*& freights, int min_speed, int max_cars) {
	//making the vector for all the train combinations
	std::vector<TrainCar*> train_options;
	//bool condition = true;

	//runs until there are no more train options
	while (engines != NULL && freights != NULL) {
		//keeps track of when we add cars and when speed changes
		//also keeps track our last car added to our current train
		int num_cars = 0;
		float current_speed = 0;
		TrainCar* current_train = NULL;

		//add an engine first no matter what
		TrainCar* engine_to_add = PopFront(engines);
		PushBack(current_train, engine_to_add);
		num_cars++;
		current_speed = CalculateSpeed(current_train);

		//keeps adding freights to train (current_train pointer) as long as
		//the requirements for maximum number of cars per train and
		//minimum speed per train are met.
		//Each loop iteration adds one car to the current train
		while (num_cars < max_cars && freights != NULL) {
			TrainCar* freight_to_add = PopFront(freights);
			PushBack(current_train, freight_to_add);
			num_cars++;
			current_speed = CalculateSpeed(current_train);
		}

		current_speed = CalculateSpeed(current_train);
		while(current_speed < min_speed && engines != NULL) {
			TrainCar* freight = PopBack(current_train);
			PushFront(freights, freight);

			current_speed = CalculateSpeed(current_train);
			if (current_speed < min_speed) {
				TrainCar* engine = PopFront(engines);
				PushFront(current_train, engine);
			}

			current_speed = CalculateSpeed(current_train);
		}

		if (engines == NULL) {
			while (current_speed < min_speed) {
				TrainCar* freight = PopBack(current_train);
				PushFront(freights, freight);
				current_speed = CalculateSpeed(current_train);
			}
		}

		//adding the train to the vector
		if (current_train->isEngine())
			train_options.push_back(current_train);
	}

	return train_options;
}

float AverageDistanceToDiningCar(TrainCar* train){
	float thing = 0.0;
	return thing;
}

int ClosestEngineToSleeperCar(TrainCar* train){
	int thing = 0;
	return thing;
}

//===============FOR SeparateHelper() FUNCTION=============================
void Separate(TrainCar*& original, TrainCar*& separated1, TrainCar*& separated2){
	int total_weight, num_engines, num_freight_cars, num_passenger_cars, num_dining_cars, num_sleeping_cars;
	TotalWeightAndCountCars(original, total_weight, num_engines, num_freight_cars, num_passenger_cars, num_dining_cars, num_sleeping_cars);
	int num_non_engines = num_freight_cars + num_passenger_cars + num_dining_cars + num_sleeping_cars;
	int length = num_engines + num_non_engines;

	if(num_engines < 2){ return; }

	if(num_engines == 2){
		int loc_engine1 = -1;
		int loc_engine2 = -1;

		TrainCar* temp = original;
		for (int i = 0; i < length; ++i){
			if(temp->isEngine()){
				if(loc_engine1 != -1){
					loc_engine2 = i;
					break;
				}
				loc_engine1 = i;
			}
			temp = temp->next;
		}

		int midpoint = length/2;
		if ((length % 2 != 0) && (midpoint == loc_engine1)){
			midpoint = loc_engine1 + 1;
		}

		bool eng1_in_front_half = (loc_engine1 < midpoint);
		bool eng2_in_back_half = (loc_engine2 >= midpoint);

		separated1 = separated2 = original;
		for (int i = 0; i < midpoint; ++i){
			separated2 = separated2->next;
		} 
		
		if(!eng1_in_front_half){
			TrainCar* engine1 = original;
			for (int i = 0; i < loc_engine1; ++i){
				engine1 = engine1->next;
			}
			if(length % 2 == 0){
				separated2 = separated2->prev;
			}
			InsertBefore( separated2, Pop(engine1) );
		}
		else if(!eng2_in_back_half){
			TrainCar* engine2 = original;
			for (int i = 0; i < loc_engine2; ++i){
				engine2 = engine2->next;
			}
			InsertAfter( separated2, Pop(engine2) );
			separated2 = separated2->next;
		}
		
		separated2->prev->next = NULL;
		separated2->prev = NULL;
		original = NULL;
	}

}
















