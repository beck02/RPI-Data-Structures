#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include <sstream>
#include "recipe.h"
#include "ingredients.h"

Recipe::Recipe(){
	std::string name = "";
}
Recipe::Recipe(std::string aname){
	name  = aname;
}
bool my_compare2(Ingredients a, Ingredients b){
	if(a.Getunits() < b.Getunits()){
		return true;
	}
	else if(a.Getunits() == b.Getunits()){
		return a.ingGetname() < b.ingGetname();
	}
	return false;
}
void Recipe::sortlist(){
	recipeingredientslist.sort(my_compare2);
}	
const std::list<Ingredients>& Recipe::getlist() const{
	return recipeingredientslist;
}

std::list<Ingredients>::const_iterator Recipe::recipeListItr(){
	return recipeingredientslist.begin();
}

void Recipe::addIngredient(std::string aname, int aunits){
	Ingredients r(aname, aunits);
	recipeingredientslist.push_back(r);
}
std::string Recipe::Getname() const{
	return name;
}