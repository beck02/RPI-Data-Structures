//
// PROVIDED CODE FOR HOMEWORK 4: GROCERY LISTS
// 
// You may use none, a little, or all of this, as you choose, but we
// strongly urge you to examine it carefully.  You may modify this
// code as you wish to complete the assignment.
//


#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "recipe.h"
#include "kitchen.h"


// Helper functions
void readRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes);
void addIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen);
void printRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes);
void makeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen);
void recipeSuggestions(std::ostream &ostr, const std::list<Recipe> &recipes, const Kitchen &kitchen);

// The main loop parses opens the files for I/O & parses the input
int main(int argc, char* argv[]) {

  // Check the number of arguments.
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " in-file out-file\n";
    return 1;
  }
  // Open and test the input file.
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "Could not open " << argv[1] << " to read\n";
    return 1;
  }
  // Open and test the output file.
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "Could not open " << argv[2] << " to write\n";
    return 1;
  }

  // the kitchen & recipe list
  Kitchen kitchen;
  std::list<Recipe> recipes;

  // some variables to help with parsing
  char c;
  while (istr >> c) {
    if (c == 'r') {
      // READ A NEW RECIPE
      readRecipe(istr,ostr,recipes);

    } else if (c == 'a') {
      // ADD INGREDIENTS TO THE KITCHEN
      addIngredients(istr,ostr,kitchen);
      
    } else if (c == 'p') {
      // PRINT A PARTICULAR RECIPE
      printRecipe(istr,ostr,recipes);

    } else if (c == 'm') {
      // MAKE SOME FOOD
      makeRecipe(istr,ostr,recipes,kitchen);

    } else if (c == 'k') {
      // PRINT THE CONTENTS OF THE KITCHEN
      kitchen.printIngredients(ostr);

    } else if (c == 's') {
      // SUGGEST ALL RECIPES THAT CAN BE MADE INDIVIDUALLY FROM THE
      //   CURRENT CONTENTS OF THE KITCHEN
      recipeSuggestions(ostr,recipes,kitchen);

    } else if (c == 'd') {
      // EXTRA CREDIT: SUGGEST THE COLLECTION OF RECIPES THAT CAN BE
      // MADE TOGETHER THAT USE THE MAXIMUM NUMBER OF UNITS OF
      // INGREDIENTS
    } else {
      std::cerr << "unknown character: " << c << std::endl;
      exit(0);
    }
  }  
}


void readRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes) {
  int units;
  std::string name, name2;  
  istr >> name;
  // build the new recipe
  Recipe r(name);
  // checks recipe
  std::list<Recipe>::const_iterator recipeit;
  bool inlist = false;
  int recipecheck = 0;
  for(recipeit = recipes.begin(); recipeit != recipes.end(); recipeit++){
    if(name == (*recipeit).Getname()){
      recipecheck = 0;
      ostr << "Recipe for " << name << " already exists" << std::endl;
      while(1){
        istr >> units;
        if(units == 0) break;
        istr >> name2;
      }
    }
    else{
      recipecheck ++;
    }
  }  
  if (recipecheck == recipes.size()){
    while (1) {
      istr >> units;
      if (units == 0) break;
      assert (units > 0);
      istr >> name2;
      r.addIngredient(name2,units);
   }
  //sorts ingredients
  r.sortlist();
  // add it to the list
  recipes.push_back(r);
  ostr << "Recipe for " << name << " added" << std::endl;
  }
}


void addIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen) {
  int units;
  std::string name;
  int count = 0;
  while (1) {
    istr >> units;
    if (units == 0) break;
    assert (units > 0);
    istr >> name;
    // add the ingredients to the kitchen
    kitchen.addIngredient(name,units);
    count++;
  }
  ostr << count << " ingredients added to kitchen" << std::endl;
}


void printRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes) {
  std::string name;
  istr >> name;
  std::list<Recipe>::const_iterator recipeit;
  bool inlist = false;
  for(recipeit = recipes.begin(); recipeit != recipes.end(); recipeit++){
    if(name == (*recipeit).Getname()){
      inlist = true;
      std::list<Ingredients> tempingredientslist;
      std::list<Ingredients>::iterator ingredientsit;
      tempingredientslist = (*recipeit).getlist();
      ostr << "To make a " << (*recipeit).Getname() << ", mix together:" << std::endl;
      for( ingredientsit = tempingredientslist.begin(); ingredientsit != tempingredientslist.end() ;ingredientsit++){
        ostr << "  " << (*ingredientsit).Getunits() << " unit of " << (*ingredientsit).ingGetname() << std::endl;
      }
    }
  }
  if(inlist == false){
    ostr << "No recipe for " << name << std::endl;
  }
}


void makeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen) {
  std::string name;
  istr >> name;
  std::list<Recipe>::const_iterator recipeit;
  std::list<Ingredients> needtobuy;
  bool inlist = false;

  for(recipeit = recipes.begin(); recipeit != recipes.end(); recipeit++){
    if(recipeit->Getname() == name){
      inlist = true;
      break;
    }
  }
  if(!inlist){
    ostr << "Don't know how to make " << name << std::endl;
  }
  else{
    //found recipe
    std::list<Ingredients> kitcheningredientslist = kitchen.kitgetlist();
    std::list<Ingredients> recipeingredientslist = (*recipeit).getlist();
    std::list<Ingredients>::iterator kitingitr;
    std::list<Ingredients>::iterator recingitr;
    for(recingitr = recipeingredientslist.begin(); recingitr != recipeingredientslist.end(); recingitr++){
      for(kitingitr = kitcheningredientslist.begin(); kitingitr != kitcheningredientslist.end(); kitingitr++){
        if((*recingitr).ingGetname() == (*kitingitr).ingGetname()){
          
          if((*recingitr).Getunits() > (*kitingitr).Getunits()){
            int unitsneeded = (*recingitr).Getunits() - (*kitingitr).Getunits();
            Ingredients tempIngred((*recingitr).ingGetname(), unitsneeded);
            needtobuy.push_back(tempIngred);
          }
          
          break;
        }
      }
      if(kitingitr == kitcheningredientslist.end()){
        needtobuy.push_back( (*recingitr) );
      }
    }

    if(needtobuy.size() == 0){
      kitchen.removeingredients( (*recipeit) );
      ostr << "Made " << name << std::endl;
    }
    else{
      ostr << "Cannot make " << (*recipeit).Getname() << ", need to buy:" << std::endl;
      std::list<Ingredients>::iterator needtobuyit;
      for(needtobuyit = needtobuy.begin(); needtobuyit != needtobuy.end(); needtobuyit++){
        ostr << "  " << (*needtobuyit).Getunits() << " units of " << (*needtobuyit).ingGetname() << std::endl;
      }
    }

  }

}              


void recipeSuggestions(std::ostream &ostr, const std::list<Recipe> &recipes, const Kitchen &kitchen) {











}
