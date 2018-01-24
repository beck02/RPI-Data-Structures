#ifndef _SUPERHERO_H_
#define _SUPERHERO_H_

#include <string>
#include <iostream>
using namespace std;

class Superhero {

friend class Team;

public:
  Superhero(string n, string i, string p)
        : hero_name_(n), identity_(i), power_(p), good_(true) {}

  // accessor functions
  const string& getName() const { return hero_name_; }
  const string& getPower() const { return power_; }

  bool isGood() const { return good_; }

  //operators
  bool operator==(const string& guess_id){ return guess_id == identity_; }
  bool operator!=(const string& guess_id){ return guess_id != identity_; }
  void operator-(){ good_ = !good_; } 
  
private:
  // REPRESENATION
  string hero_name_;
  string identity_;
  string power_;
  bool good_;

  const string& getTrueIdentity() const { return identity_; }
};

bool operator>(const Superhero& a, const Superhero& b);

ostream& operator<<(ostream& os, const Superhero& hero);

#endif



