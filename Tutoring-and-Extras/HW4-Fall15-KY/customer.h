#include <list>
#include <string>
#include "request.h"

#ifndef __customer_h_
#define __customer_h_

class Customer{
public:
	// Constructor
	Customer(const std::string& id, const std::string& n) : name(n), idNum(id) {}
	
	// Accessors
	const std::string& getCustID() const { return idNum; }
	const std::string& getCustName() const { return name; }
	bool active() const { return (rentals.size() > 0) || (pending.size() > 0); }
	void print(std::ofstream& outfile) const;

	// Mutators
	void custRentItem(const Request& request);
	bool custReturnItem(const Request& request);
	void addToPending(const Request& request);
	bool removeFromPending(const Request& request);

private:
	std::string name;
	std::string idNum;
	std::list<Request> rentals;
	std::list<Request> pending;

};


#endif