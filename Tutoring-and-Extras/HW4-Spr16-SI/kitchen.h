#ifndef __Kitchen_h
#define __Kitchen_h


#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include "ingredients.h"
#include "recipe.h"
class Kitchen{
public:
	Kitchen();
	Kitchen(std::string aingredient, int aunits);
	void addIngredient(std::string name, int units);
	void printIngredients(std::ostream &outstream);
	const std::list<Ingredients>& kitgetlist() const;
	std::list<Ingredients>::const_iterator kitListItr();
	void removeingredients(Recipe recipe);
private:
	std::string ingredient;
	int units;
	std::list<Ingredients> ingredientslist;
};
#endif