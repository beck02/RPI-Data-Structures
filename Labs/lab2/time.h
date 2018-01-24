/** File: time.h
Purpose: Header file with declaration of the Time class, including 
member functions and private member variables.
**/

class Time {
public:
	//Constructors
	Time();
	Time(int h, int m, int s);
	
	//Accessors
	int getHour() const;
	int getMin() const;
	int getSec() const;

	//Mutators
	void setHour(int h);
	void setMin(int m);
	void setSec(int s);
	
	//Other helpers
	void print() const;
	void printAmPm() const;

private: //Member variables
	int hour;
	int min;
	int sec;
};

bool isEarlierThan(const Time & t1, const Time & t2);