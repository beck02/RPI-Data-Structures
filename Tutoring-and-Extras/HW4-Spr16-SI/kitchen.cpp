#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include "kitchen.h"
#include "ingredients.h"

Kitchen::Kitchen(){
	std::string ingredients = "";
	int units = 0;
}

Kitchen::Kitchen(std::string aingredient, int aunits){
	units = aunits;
	ingredient = aingredient;
}

void Kitchen::addIngredient(std::string name, int units){
	Ingredients r(name, units);
	std::list<Ingredients>::iterator ingredientsit;
	bool inlist = false;
	for(ingredientsit = ingredientslist.begin(); ingredientsit != ingredientslist.end(); ingredientsit++){
		if(r.ingGetname() == (*ingredientsit).ingGetname()){
			(*ingredientsit).Changeunits(units);
			inlist = true;
		}
	}
	if(inlist == false){
		ingredientslist.push_back(r);
	}
}

bool my_compare(Ingredients a, Ingredients b){
	if(a.Getunits() < b.Getunits()){
		return true;
	}
	else if(a.Getunits() == b.Getunits()){
		return a.ingGetname() < b.ingGetname();
	}
	return false;
}

void Kitchen::printIngredients(std::ostream &output){
	output << "In the kitchen:" << std::endl;
	std::list<Ingredients>::iterator ingredientsit;
	ingredientslist.sort(my_compare);
	for(ingredientsit = ingredientslist.begin(); ingredientsit != ingredientslist.end(); ingredientsit++){
		output << "  " << (*ingredientsit).Getunits() << " units of " << (*ingredientsit).ingGetname() << std::endl;
	}
}

const std::list<Ingredients>& Kitchen::kitgetlist() const{
	return ingredientslist;
}

std::list<Ingredients>::const_iterator Kitchen::kitListItr(){
	return ingredientslist.begin();
}

void Kitchen::removeingredients(Recipe recipe){
	std::list<Ingredients> tempingredientslist;
    std::list<Ingredients>::iterator ingredientsit;
    tempingredientslist = recipe.getlist();
    for(ingredientsit = tempingredientslist.begin(); ingredientsit != tempingredientslist.end() ;ingredientsit++){ 
    	std::list<Ingredients>::iterator kitchenit;
        for(kitchenit = ingredientslist.begin(); kitchenit != ingredientslist.end(); kitchenit++){
        	if((*ingredientsit).ingGetname() == (*kitchenit).ingGetname()){
            	(*kitchenit).Subunits((*ingredientsit).Getunits());
            }
        }     
    }    
}



