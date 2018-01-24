#include <string>

#ifndef __request_h_
#define __request_h_

class Request{
public:
	// Constructor
	Request(const std::string& iid, int q, const std::string& cid, const std::string& cn, const std::string& t)
		: quantity(q), itemID(iid), custID(cid), custName(cn), timestamp(t) {}

	//Accessors
	int getQuantity() const { return quantity; }
	const std::string& getItemID() const { return itemID; }
	const std::string& getCustID() const { return custID; }
	const std::string& getCustName() const { return custName; }
	const std::string& getTimestamp() const { return timestamp; }
	
	// Mutator
	void addQuantity(int q){ quantity += q; }
	bool removeQuantity(int q){
		if(quantity >= q){
			quantity -= q;
			return true;
		}
		return false;
	}

private:
	int quantity;
	std::string itemID;
	std::string custID;
	std::string custName;
	std::string timestamp;

};


#endif