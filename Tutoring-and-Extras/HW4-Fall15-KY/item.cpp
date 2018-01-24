#include <fstream>
#include <iostream>
#include "item.h"
#include "customer.h"
#include "request.h"

// Mutators
bool Item::removeStock(const Request& request){
	if(enough(request.getQuantity())){
		quantity -= request.getQuantity();
		std::list<Request>::iterator itr;
		if(renters.size() == 0){
			renters.push_front(request);
		} else if( renters.back().getCustID() < request.getCustID() ){
			renters.push_back(request);
		} else{
			for(itr = renters.begin(); itr != renters.end(); ++itr){
				if(request.getCustID() == itr->getCustID()){ 
					itr->addQuantity(request.getQuantity());
					break; 
				} 
				else if(request.getCustID() < itr->getCustID()){
					renters.insert(itr, request);
					break;
				}
			}
		}
	}
	return enough(request.getQuantity());
}

bool Item::addStock(const Request& rental){
	bool returned = false;
	std::list<Request>::iterator itr;
	for(itr = renters.begin(); itr != renters.end(); ++itr){
		if(rental.getCustID() == itr->getCustID()){ 
			returned = itr->removeQuantity(rental.getQuantity());
			if(returned){
				quantity += rental.getQuantity();
			}
			if(itr->getQuantity() == 0){
				renters.erase(itr);
			}
			break; 
		} 
	}
	return returned;
}

void Item::addToWaitlist(const Request& request){
	std::list<Request>::iterator wtlstItr;
	for(wtlstItr = waitlist.begin(); wtlstItr != waitlist.end(); ++wtlstItr){
		if(request.getCustID() == wtlstItr->getCustID()){
			wtlstItr->addQuantity(request.getQuantity());
			return;
		}
	}
	if(waitlist.size() == 0){
		waitlist.push_front(request);
	} else if( waitlist.back().getTimestamp() < request.getTimestamp() ){
		waitlist.push_back(request);
	} else{
		for(wtlstItr = waitlist.begin(); wtlstItr != waitlist.end(); ++wtlstItr){
			if(request.getTimestamp() < wtlstItr->getTimestamp()){
				waitlist.insert(wtlstItr, request);
				return;
			}
		}
	}
}

void Item::checkWaitlist(std::list<Customer>& custList){
	std::list<Request>::iterator wtlstItr;
	for(wtlstItr = waitlist.begin(); wtlstItr != waitlist.end(); ++wtlstItr){
		if(quantity >= wtlstItr->getQuantity())	{
			Request request = *wtlstItr;
			removeStock(request);
			wtlstItr = waitlist.erase(wtlstItr);
			wtlstItr--;
			std::list<Customer>::iterator custItr;
			for(custItr = custList.begin(); custItr != custList.end(); custItr++){
				if(custItr->getCustID() == request.getCustID()){
					custItr->custRentItem(request);
					custItr->removeFromPending(request);
					break;
				}
			}
		}
	}
}

void Item::print(std::ofstream& outfile) const {
	outfile << idNum << " " << quantity << " available " << name << std::endl;
	std::list<Request>::const_iterator itr;
	if( renters.size() > 0 ){
		outfile << "Rental Customers: ";
		for(itr = renters.begin(); itr != renters.end(); itr++){
			outfile << itr->getCustID() << " " << itr->getCustName() << " (" << itr->getQuantity() << ") ";
		}
		outfile << std::endl;
	}
	if( waitlist.size() > 0 ){
		outfile << "Pending Customers: ";
		for(itr = waitlist.begin(); itr != waitlist.end(); itr++){
			outfile << itr->getCustID() << " " << itr->getCustName() << " (" << itr->getQuantity() << ") ";
		}
		outfile << std::endl;
	}
}
























