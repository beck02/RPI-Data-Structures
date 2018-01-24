#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "organism.h"


// =====================================================================
// =====================================================================
// HELPER FUNCTIONS

Organism* MakeOrganismByType(const std::map<std::string,std::vector<std::string> > &facts);
Organism* MakeOrganismUnknownType(const std::map<std::string,std::vector<std::string> > &facts);
void OutputCategoryMembership(const std::vector<Organism*> &organisms, std::ostream &ostr);
bool organism_sorter(const Organism* org1, const Organism* org2){
  return org1->getName() < org2->getName();
}

// =====================================================================
// =====================================================================
int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "ERROR: Wrong number of arguments." << std::endl;
    std::cerr << "  " << argv[0] << " <input_file> <output_file>" << std::endl;
    exit(0);
  }
  // parse the input file
  std::ifstream istr(argv[1]);
  if (!istr.good()) {
    std::cerr << "ERROR: Cannot open " << argv[1] << std::endl;
    exit(0);
  }
  std::string token;
  istr >> token;
  // MAKE A POLYMORPHIC VECTOR OF ORGANISMS
  std::vector<Organism*> organisms;
  while(1) {
    if (token != "organism") break;
    std::map<std::string,std::vector<std::string> > facts;
    //std::cout << "read a thing" << std::endl;
    while (1) {
      if (!(istr >> token) || token == "organism") break;
      std::string value;
      istr >> value;
      //std::cout << "get stuff about the thing" << std::endl;
      facts[token].push_back(value);
    }
    //std::cout << "make the thing" << std::endl;
    // make the organism and add it to the vector
    Organism* organism = MakeOrganismByType(facts);
    if (organism == NULL) { organism = MakeOrganismUnknownType(facts); }
    if (organism != NULL) { organisms.push_back(organism); }
  }
  // open the output file
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: Cannot open " << argv[2] << std::endl;
    exit(0);
  }
  //std::cout << "print the things" << std::endl;
  // OUTPUT INFORMATION ABOUT THE ORGANISMS
  std::sort(organisms.begin(),organisms.end(),organism_sorter);
  OutputCategoryMembership(organisms,ostr);

  // cleanup the objects
  for (unsigned int i = 0; i < organisms.size(); i++) {
    // Note: in order for this to work, you need to define the
    // Organism destructor, and it must be virtual:
    //  virtual ~Organism() {}
    delete organisms[i];
  }

  return 0;
}


// =====================================================================
// =====================================================================
// For the first part of the implementation, assume that the each
// organism has its type explicitly listed as a fact.

Organism* MakeOrganismByType(const std::map<std::string,std::vector<std::string> > &facts) { 
  std::map<std::string,std::vector<std::string> >::const_iterator itr = facts.find("type:");
  if (itr != facts.end()) {
    assert (itr->second.size() == 1);
    if      (itr->second[0] == "bat")            { return new Bat(facts);          }
    else if (itr->second[0] == "bear")           { return new Bear(facts);         }
    else if (itr->second[0] == "cardinal")       { return new Cardinal(facts);     }
    else if (itr->second[0] == "penguin")        { return new Penguin(facts);      }
    else if (itr->second[0] == "platypus")       { return new Platypus(facts);     }
    else if (itr->second[0] == "redwood")        { return new Redwood(facts);      }
    else if (itr->second[0] == "rhinoceros")     { return new Rhinoceros(facts);   }
    else if (itr->second[0] == "trout")          { return new Trout(facts);        } 
    else if (itr->second[0] == "tiger")          { return new Tiger(facts);        }
    else if (itr->second[0] == "tortoise")       { return new Tortoise(facts);     }
    else if (itr->second[0] == "venus_fly_trap") { return new VenusFlyTrap(facts); } 
    else { std::cout << "unknown type: " << itr->second[0] << std::endl; }
  }
  return NULL;
}


// =====================================================================
// =====================================================================
// For the final part of the implementation, we must deduce the type
// of each organism by examining the set of facts.  We present the
// facts about the organism to each constructor and if the constructor
// fails (throws an exception) we try another constructor.

Organism* MakeOrganismUnknownType(const std::map<std::string,std::vector<std::string> > &facts) { 
  //std::map<std::string,std::vector<std::string> >::const_iterator itr = facts.find("name:");
  //if (itr != facts.end()) { std::cout << itr->first << itr->second[0] << std::endl; }
  Organism *answer = NULL;
  try {
    //std::cout << "trout?" << std::endl;
    answer = new Bat(facts);
  }
  catch (int) {
    try {
      answer = new Bear(facts);
    }
    catch (int) {
      try {
        answer = new Cardinal(facts);
      }
      catch (int) {
        try {
          answer = new Penguin(facts);
        }
        catch (int) {
          try {
            answer = new Platypus(facts);
          }
          catch (int) {
            try {
              answer = new Redwood(facts);
            }
            catch (int) {
              try {
                answer = new Rhinoceros(facts);
              }
              catch (int) {
                try {
                  answer = new Trout(facts);
                }
                catch (int) {
                  try {
                    answer = new Tiger(facts);
                  }
                  catch (int) {
                    try {
                      answer = new Tortoise(facts);
                    }
                    catch (int) {
                      try {
                        answer = new VenusFlyTrap(facts);
                      }
                      catch (int) {
                        try {
                          answer = new Mammal(facts);
                        }
                        catch (int) {
                          try {
                            answer = new Bird(facts);
                          }
                          catch (int) {
                            try {
                              answer = new Reptile(facts);
                            }
                            catch (int) {
                              try {
                                answer = new Fish(facts);
                              }
                              catch (int) {
                                try {
                                  answer = new Ectothermic(facts);
                                }
                                catch (int) {
                                  try {
                                    answer = new LaysEggs(facts);
                                  }
                                  catch (int) {
                                    try {
                                      answer = new Endothermic(facts);
                                    }
                                    catch (int) {
                                      try {
                                        answer = new Animal(facts);
                                      }
                                      catch (int) {
                                        try {
                                          answer = new Plant(facts);
                                        }
                                        catch (int) {
                                          answer = new Organism(facts);
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  assert (answer != NULL);
  return answer;
}


// =====================================================================
// =====================================================================

// This function prints the output.  C++ macros are used to abbreviate
// some repetitive code.  The function call-like macros are actually
// replaced using substitution by the preprocessor before the code is
// given to the compiler.  (You are not required to understand the
// details of the macros. )

void OutputCategoryMembership(const std::vector<Organism*> &organisms, std::ostream &ostr) {

  // define and initialize variables that hold the members of each category
# define InitializeCount(type) std::vector<std::string> all_##type
# define InitializeCountF(func) std::vector<std::string> all_##func
  // the 21 required classes must be define
  InitializeCount(Animal);
  InitializeCount(Bat);
  InitializeCount(Bird);
  InitializeCount(Bear);
  InitializeCount(Cardinal);
  InitializeCount(Ectothermic);
  InitializeCount(Endothermic);
  InitializeCount(Fish);
  InitializeCount(LaysEggs);
  InitializeCount(Mammal);
  InitializeCount(Organism);
  InitializeCount(Penguin);
  InitializeCount(Plant);
  InitializeCount(Platypus);
  InitializeCount(Redwood);
  InitializeCount(Reptile);
  InitializeCount(Rhinoceros);
  InitializeCount(Trout);
  InitializeCount(Tiger);
  InitializeCount(Tortoise);
  InitializeCount(VenusFlyTrap);
  // These 6 functions must be defined for each class:
  // isCarnivore, isHerbivore, isOmnivore, livesInAir, livesOnLand, and livesInWater
  InitializeCountF(isCarnivore);
  InitializeCountF(isHerbivore);
  InitializeCountF(isOmnivore);
  InitializeCountF(livesInAir);
  InitializeCountF(livesOnLand);
  InitializeCountF(livesInWater);

  // count & record the names of shapes in each category
  for (std::vector<Organism*>::const_iterator i = organisms.begin(); i!=organisms.end(); ++i) {
#   define IncrementCount(type) if (dynamic_cast<type*> (*i)) all_##type.push_back((*i)->getName())
#   define IncrementCountF(func) if ((*i)->func()) all_##func.push_back((*i)->getName())
    IncrementCount(Animal);
    IncrementCount(Bat);
    IncrementCount(Bird);
    IncrementCount(Bear);
    IncrementCount(Cardinal);
    IncrementCount(Ectothermic);
    IncrementCount(Endothermic);
    IncrementCount(Fish);
    IncrementCount(LaysEggs);
    IncrementCount(Mammal);
    IncrementCount(Organism);
    IncrementCount(Penguin);
    IncrementCount(Plant);
    IncrementCount(Platypus);
    IncrementCount(Redwood);
    IncrementCount(Reptile);
    IncrementCount(Rhinoceros);
    IncrementCount(Trout);
    IncrementCount(Tiger);
    IncrementCount(Tortoise);
    IncrementCount(VenusFlyTrap);
    IncrementCountF(isCarnivore);
    IncrementCountF(isHerbivore);
    IncrementCountF(isOmnivore);
    IncrementCountF(livesInAir);
    IncrementCountF(livesOnLand);
    IncrementCountF(livesInWater);
  }

  // create a pretty output of the data for each category
# define PrintCount(type) ostr << all_##type.size() << " " #type"(s): "; \
  for (unsigned int j = 0; j < all_##type.size(); j++) { ostr << " " << all_##type[j]; } ostr << std::endl
# define PrintCountF(desc,func) ostr << all_##func.size() << " " #desc": "; \
  for (unsigned int j = 0; j < all_##func.size(); j++) { ostr << " " << all_##func[j]; } ostr << std::endl
  PrintCount(Animal);
  PrintCount(Bat);
  PrintCount(Bird);
  PrintCount(Bear);
  PrintCount(Cardinal);
  PrintCount(Ectothermic);
  PrintCount(Endothermic);
  PrintCount(Fish);
  PrintCount(LaysEggs);
  PrintCount(Mammal);
  PrintCount(Organism);
  PrintCount(Penguin);
  PrintCount(Plant);
  PrintCount(Platypus);
  PrintCount(Redwood);
  PrintCount(Reptile);
  PrintCount(Rhinoceros);
  PrintCount(Trout);
  PrintCount(Tiger);
  PrintCount(Tortoise);
  PrintCount(VenusFlyTrap);
  PrintCountF(Carnivore(s),isCarnivore);
  PrintCountF(Herbivore(s),isHerbivore);
  PrintCountF(Omnivore(s),isOmnivore);
  PrintCountF(Organism(s) live(s) in Air,livesInAir);
  PrintCountF(Organism(s) live(s) on Land,livesOnLand);
  PrintCountF(Organism(s) live(s) in Water,livesInWater);
}

// =====================================================================
// =====================================================================


