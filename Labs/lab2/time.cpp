/** File: time.cpp
Purpose: Implementation of time class
**/

#include <iostream>
#include <iomanip>
#include <string>
#include "time.h"

//Constructors
Time::Time(){
	setHour(0);
	setMin(0);
	setSec(0);
}
Time::Time(int h, int m, int s){
	setHour(h);
	setMin(m);
	setSec(s);
}
	
//Accessors
int Time::getHour() const {
	return hour;
}
int Time::getMin() const {
	return min;
}
int Time::getSec() const {
	return sec;
}

//Mutators
void Time::setHour(int h){
	if( h > 23 ){
		hour = 23;
		std::cout << "Input of " << h << " is invalid for hours. Hours set to 23." << std::endl;
	} else if( h < 0 ){
		hour = 0;
		std::cout << "Input of " << h << " is invalid for hours. Hours set to 0." << std::endl;
	} else{
		hour = h;
	}
}
void Time::setMin(int m){
	if( m > 59 ){
		min = 59;
		std::cout << "Input of " << m << " is invalid for minutes. Minutes set to 59." << std::endl;
	} else if( m < 0 ){
		min = 0;
		std::cout << "Input of " << m << " is invalid for minutes. Minutes set to 0." << std::endl;
	} else{
		min = m;
	}
}
void Time::setSec(int s){
	if( s > 59 ){
		sec = 59;
		std::cout << "Input of " << s << " is invalid for seconds. Seconds set to 59." << std::endl;
	} else if( s < 0 ){
		sec = 0;
		std::cout << "Input of " << s << " is invalid for seconds. Seconds set to 0." << std::endl;
	} else{
		sec = s;
	}
}

//Other helpers
void Time::print() const{
	if( hour < 10 ){
		std::cout << " ";
	}
	std::cout << hour << ":";
	if( min < 10 ){
		std::cout << "0";
	}
	std::cout << min << ":";
	if( sec < 10 ){
		std::cout << "0";
	}
	std::cout << sec;
}
void Time::printAmPm() const{
	int tempH = hour;
	std::string ampm = " am";
	if( hour > 12 ){
		tempH = hour - 12;
		ampm = " pm";
		if( hour < 22 ){
			std::cout << " ";
		}
	} else if( hour == 12 ){
		ampm = " pm";
	} else if( hour == 0 ){
		tempH = 12;
	} else if( hour < 10 ){
		std::cout << " ";
	}
	std::cout << tempH << ":";
	if( min < 10 ){
		std::cout << "0";
	}
	std::cout << min << ":";
	if( sec < 10 ){
		std::cout << "0";
	}
	std::cout << sec << ampm;
}

bool isEarlierThan(const Time & t1, const Time & t2){
	bool ret = false;
	int tHr1 = t1.getHour(), tHr2 = t2.getHour();
	if( tHr1 < tHr2 ){
		ret = true;
	} else if( tHr1 == tHr2 ){
		int tMn1 = t1.getMin(), tMn2 = t2.getMin();
		if( tMn1 < tMn2 ){
			ret = true;
		} else if( tMn1 == tMn2 ){
			int tSc1 = t1.getSec(), tSc2 = t2.getSec();
			if( tSc1 < tSc2 ){
				ret = true;
			}
		}
	}
	return ret;
}

