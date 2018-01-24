#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include "item.h"
#include "customer.h"
#include "request.h"

//=====================================================================

std::list<Item>::iterator findItem(std::list<Item>& lst, std::string& id){
	std::list<Item>::iterator itr = lst.begin();
	for(; itr != lst.end(); ++itr){
		if(id == itr->getItemID()){ break; }
	}
	return itr;
}

//=====================================================================

void addItemtoInventory(std::list<Item>& lst, Item itm){
	if(lst.size() == 0){
		lst.push_front(itm);
	} else if( lst.back().getItemID() < itm.getItemID() ){
		lst.push_back(itm);
	} else{
		std::list<Item>::iterator itr = lst.begin();
		for(; itr != lst.end(); itr++){
			if( itr->getItemID() > itm.getItemID() ){
				lst.insert(itr, itm);
				break;
			}
		}
	}
}

//=====================================================================

std::list<Customer>::iterator addCustomer(std::list<Customer>& lst, const std::string& id, const std::string& name){
	Customer cust(id, name);
	std::list<Customer>::iterator itr;
	if(lst.size() == 0){
		lst.push_front(cust);
		itr = lst.begin();
	} else if( lst.back().getCustID() < cust.getCustID() ){
		lst.push_back(cust);
		itr = lst.end();
		--itr;
	} else{
		for(itr = lst.begin(); itr != lst.end(); ++itr){
			if(cust.getCustID() == itr->getCustID()){ break; } 
			else if(cust.getCustID() < itr->getCustID()){
				itr = lst.insert(itr, cust);
				break;
			}
		}
	}
	return itr;
}

//=====================================================================
//=====================================================================

int main(int argc, char* argv[]) {
	if(argc != 5){
		std::cerr << "Enter 4 arguments after command name:\n" 
				<< "inventory_in_filename customer_in_filename inventory_out_filename customer_out_filename\n";
		exit(1);
	}
	std::ifstream inv_infile;
	inv_infile.open(argv[1]);
	if (!inv_infile.good()){
		std::cerr << "Can't open" << argv[1] << "to read.\n";
		exit(1);
	}
	std::ifstream cus_infile;
	cus_infile.open(argv[2]);
	if (!cus_infile.good()){
		std::cerr << "Can't open" << argv[2] << "to read.\n";
		exit(1);
	}

	std::ofstream inv_outfile;
	inv_outfile.open(argv[3]);
	if (!inv_outfile.good()){
		std::cerr << "Can't open" << argv[3] << "to write.\n";
		exit(1);
	}
	std::ofstream cus_outfile;
	cus_outfile.open(argv[4]);
	if (!cus_outfile.good()){
		std::cerr << "Can't open" << argv[4] << "to write.\n";
		exit(1);
	}

	std::list<Item> inventoryList;
	std::list<Customer> customerList;
	std::string s;
	
	// goes through each line of inventory input
	while(inv_infile >> s){ 
		std::string newid = s;
		inv_infile >> s;
		std::string newquantStr = s;
		int newquant = std::stoi(newquantStr);
		inv_infile >> s;
		std::string newname = s;

		if(newid.at(0) != 'T' || newquant <= 0){
			std::cerr << "Invalid inventory ID " << newid << " found in the inventory file." << std::endl;
			continue;
		}
		Item newItem(newid, newquant, newname);
		addItemtoInventory(inventoryList, newItem);
		
	} //end of inventory input

	std::list<Customer>::iterator custItr;
	std::list<Item>::iterator itemItr;

	// goes through each line of customer input
	while(cus_infile >> s){ 
		std::string custid = s;
		
		cus_infile >> s;
		std::string action = s;
		
		cus_infile >> s;
		std::string timestamp = s;
		
		cus_infile >> s; 
		std::string quantStr = s;
		int quant = std::stoi(quantStr);
		
		cus_infile >> s;
		std::string itemID = s;
		
		cus_infile >> s;
		std::string custname = s;

		if(custid.at(0) != 'C'){
			std::cerr << "Invalid customer information found for ID " << custid << " in the customer file." << std::endl;
			continue;
		}

		Request newRequest(itemID, quant, custid, custname, timestamp);

		custItr = addCustomer(customerList, custid, custname);
		itemItr = findItem(inventoryList, itemID);

		if(action == "rent"){
			if(itemItr == inventoryList.end()){
				std::cerr << "Customer " << custid << " requested item " << itemID << " which is not in the inventory." << std::endl;
				continue;
			} else if( itemItr->enough(quant) ){
				custItr->custRentItem(newRequest);
				itemItr->removeStock(newRequest);
			} else{
				itemItr->addToWaitlist(newRequest);
				custItr->addToPending(newRequest);
			}
		} //end rent 
		else if(action == "return"){
			if(itemItr == inventoryList.end()){
				std::cerr << "Customer " << custid << " attempted to return item " << itemID << " which is not in the inventory." << std::endl;
				continue;
			}
			bool returned = custItr->custReturnItem(newRequest);
			if(!returned){
				std::cerr << "Customer " << custid << " attempted to return item " << itemID << " which she/he did not rent." << std::endl;
				continue;
			}
			itemItr->addStock(newRequest);
			itemItr->checkWaitlist(customerList);
			if( !(custItr->active()) ){
				customerList.erase(custItr);
			}
		} //end return
	} //end of customer input
	
	for(itemItr = inventoryList.begin(); itemItr != inventoryList.end(); itemItr++){
		itemItr->print(inv_outfile);
		inv_outfile << std::endl;
	}

	for(custItr = customerList.begin(); custItr != customerList.end(); custItr++){
		custItr->print(cus_outfile);
		cus_outfile << std::endl;
	}


	return 0;
}
























