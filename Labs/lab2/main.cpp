#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <vector>
#include "time.h"

int main(){
	std::vector<Time> times;
	Time t1;
	std::cout << "Please enter a time (h, m, s): ";
	int hr, mn, sc;
	std::cin >> hr >> mn >> sc;
	Time t2(hr, mn, sc);
	/*
	std::cout << "Time1 = " << t1.getHour() << ":" << t1.getMin() 
			<< ":" << t1.getSec() << std::endl;
	std::cout << "Time2 = " << t2.getHour() << ":" << t2.getMin() 
			<< ":" << t2.getSec() << std::endl;
	*/
	std::cout << "You entered ";
	t2.printAmPm();
	std::cout << " for the time." << std::endl;
	times.push_back(t2);
	srand (time(NULL));
	for(int i = 0; i < 5; i++){
		int th = rand() % 24;
		int tm = rand() % 60;
		int ts = rand() % 60;
		Time nt(th, tm, ts);
		times.push_back(nt);
	}
	sort(times.begin(), times.end(), isEarlierThan);
	for(int i = 0; i < times.size(); i++){
		times[i].printAmPm();
		std::cout << std::endl;
	}
	return 0;
}