#include <list>
#include <string>
#include "customer.h"
#include "request.h"

#ifndef __item_h_
#define __item_h_

class Item{
public:
	// Constructor
	Item(const std::string& id, int q, const std::string& n) : idNum(id), quantity(q), name(n) {}
	
	// Accessors
	const std::string& getItemID() const { return idNum; }
	int getQuantity() const { return quantity; }
	const std::string& getItemName() const { return name; }
	bool enough(int q){ return quantity >= q; }
	void print(std::ofstream& outfile) const;

	// Mutators
	bool removeStock(const Request& request);
	bool addStock(const Request& request);
	void addToWaitlist(const Request& request);
	void checkWaitlist(std::list<Customer>& custList);

private:
	std::string idNum;
	int quantity;
	std::string name;
	std::list<Request> renters;
	std::list<Request> waitlist;

};


#endif