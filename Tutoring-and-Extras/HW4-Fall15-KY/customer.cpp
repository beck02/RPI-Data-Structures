#include <fstream>
#include <iostream>
#include "customer.h"
#include "request.h"

void Customer::custRentItem(const Request& request){
	//removeFromPending(request);
	std::list<Request>::iterator custInvItem = rentals.begin();
	for(; custInvItem != rentals.end(); ++custInvItem){
		if(request.getItemID() == custInvItem->getItemID()){ 
			custInvItem->addQuantity(request.getQuantity());
			return;
		}
	}
	rentals.push_back(request);
}

bool Customer::custReturnItem(const Request& request){
	bool returned = false;
	std::list<Request>::iterator custInvItem = rentals.begin();
	for(; custInvItem != rentals.end(); ++custInvItem){
		if(request.getItemID() == custInvItem->getItemID()){ 
			returned = custInvItem->removeQuantity(request.getQuantity());
			if(custInvItem->getQuantity() == 0){
				custInvItem = rentals.erase(custInvItem);
				break;
			}
		}
	}
	if(returned){
		std::list<Request>::iterator pendingItem;
		for(pendingItem = pending.begin(); pendingItem != pending.end(); pendingItem++){
			if(pendingItem->getItemID() == request.getItemID()){
				pendingItem->addQuantity(request.getQuantity());
				break;
			}
		}
	}
	return returned;
}


void Customer::addToPending(const Request& request){
	std::list<Request>::iterator itr;
	if(pending.size() == 0){
		pending.push_front(request);
	} else if( pending.back().getItemID() < request.getItemID() ){
		pending.push_back(request);
	} else{
		for(itr = pending.begin(); itr != pending.end(); ++itr){
			if(request.getItemID() == itr->getItemID()){ 
				itr->addQuantity(request.getQuantity());
				break; 
			} 
			else if(request.getItemID() < itr->getItemID()){
				pending.insert(itr, request);
				break;
			}
		}
	}
}

bool Customer::removeFromPending(const Request& request){
	bool found = false;
	std::list<Request>::iterator itr = pending.begin();
	for(; itr != pending.end(); ++itr){
		if(request.getItemID() == itr->getItemID()){ 
			found = itr->removeQuantity(request.getQuantity());
			if(itr->getQuantity() == 0){
				itr = pending.erase(itr);
				break;
			}
		}
	}
	return found;
}

void Customer::print(std::ofstream& outfile) const {
	outfile << idNum << " " << name << std::endl;
	std::list<Request>::const_iterator itr;
	if( rentals.size() > 0 ){
		outfile << "Rentals: ";
		for(itr = rentals.begin(); itr != rentals.end(); itr++){
			outfile << itr->getItemID() << " (" << itr->getQuantity() << ") ";
		}
		outfile << std::endl;
	}
	if( pending.size() > 0 ){
		outfile << "Pending: ";
		for(itr = pending.begin(); itr != pending.end(); itr++){
			outfile << itr->getItemID() << " (" << itr->getQuantity() << ") ";
		}
		outfile << std::endl;
	}
}












