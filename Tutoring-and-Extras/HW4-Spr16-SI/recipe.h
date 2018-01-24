#ifndef __recipe_h
#define __recipe_h

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <list>
#include "ingredients.h"
class Recipe{
public:
	Recipe();
	Recipe(std::string name);

	const std::list<Ingredients>& getlist() const;
	void sortlist();

	std::list<Ingredients>::const_iterator recipeListItr();
	void addIngredient(std::string ingname, int units);
	std::string Getname() const;


private:
	std::string name;
	std::list<Ingredients> recipeingredientslist;
};
#endif