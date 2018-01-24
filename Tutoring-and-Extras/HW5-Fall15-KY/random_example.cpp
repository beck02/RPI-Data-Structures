#include <cassert>
#include <iostream>
#include <ctime>

#include "mtrand.h"


// This program demonstrates how to use the MersenneTwister library
// (mtrand.h & mtrand.cpp) to generate random integers in a specified
// range.
int main(int argc, char* argv[]) {

  if (argc != 3) {
    std::cerr << "ERROR: This program expects 2 arguments:" << std::endl;
    std::cerr << "the # of values to generate and the max integer value." << std::endl;
    return 1;
  }

  int number = atoi(argv[1]);
  int max = atoi(argv[2]);
  assert (number > 0);
  assert (max > 0);


  std::cout << "Generating " << number 
            << " random integer values in the range [0," << max << "]" << std::endl;


  // Using a fixed seed can be useful for testing & debugging.  This
  // sequence will be the same on future runs of the program.
  MTRand_int32 mtrand_fixed_seed(42);
  std::cout << "fixed seed:                 ";
  for (int i = 0; i < number; i++) {
    std::cout << " " << mtrand_fixed_seed() % (max+1);
  }  
  std::cout << std::endl;


  // "Seeding" the random number generator will start the generation
  // at a different point in the sequence.  Here we seed by the
  // current time (in seconds).  This sequence will be different of
  // future runs of the program.  (Assuming it is at least a second
  // later!)
  std::cout << "seeded by the current time: ";
  MTRand_int32 mtrand_autoseed(time(NULL));
  for (int i = 0; i < number; i++) {
    std::cout << " " << mtrand_autoseed() % (max+1);
  }  
  std::cout << std::endl;

}





