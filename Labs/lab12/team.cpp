#include <cassert>
#include "superhero.h"
#include "team.h"


void Team::operator+=(const Superhero &a){
  std::list<Superhero>::iterator itr;
  for (itr = superheroes.begin(); itr != superheroes.end(); itr++) {
    if( a.getTrueIdentity() == itr->getTrueIdentity() ) { return; }
  }
  superheroes.push_back(a);
}
void Team::operator-=(const Superhero &a){
  std::list<Superhero>::iterator itr;
  for (itr = superheroes.begin(); itr != superheroes.end(); itr++) {
    if( a.getTrueIdentity() == itr->getTrueIdentity() ) { 
      superheroes.erase(itr);
      return;
    }
  }
}

Team operator+(const Team &a, const Team &b){
  Team nt(a);
  std::list<Superhero>::const_iterator itr;
  std::list<Superhero>::const_iterator itr2;
  for(itr = b.superheroes.begin(); itr != b.superheroes.end(); itr++){
    nt.superheroes.push_back(*itr);
    for (itr2 = a.superheroes.begin(); itr2 != a.superheroes.end(); itr2++) {
      if( itr->getName() == itr2->getName() ) { 
        nt.superheroes.pop_back();
        break;
      }
    }
  }
  return nt;
}

Team operator+(const Team &a, const Superhero &b){
  Team nt(a);
  std::list<Superhero>::iterator itr;
  for (itr = nt.superheroes.begin(); itr != nt.superheroes.end(); itr++) {
    if( b.getName() == itr->getName() ) { return nt; }
  }
  nt.superheroes.push_back(b);
  return nt;
}

Team operator+(const Superhero &a, const Superhero &b){
  return Team(a, b);
}


bool isvowel(char c) {
  if (!isalpha(c)) return false;
  char c2 = tolower(c);
  return (c2 == 'a' || c2 == 'e' || c2 == 'i' || c2 == 'o' || c2 == 'u');
}

bool isconsonant(char c) {
  return (isalpha(c) && !isvowel(c));
}

std::string Team::getName() const {
  if (superheroes.size() == 0) 
    return "";
  std::string answer;
  std::list<Superhero>::const_iterator itr;
  for (itr = superheroes.begin(); itr != superheroes.end(); itr++) {
    char first_consonant = ' ';
    char first_vowel = ' ';
    std::string true_identity = itr->getTrueIdentity();
    for (int j = 0; j < true_identity.size(); j++) {
      if (first_consonant == ' ' && isconsonant(true_identity[j]))
        first_consonant = tolower(true_identity[j]);
      if (first_vowel == ' ' && isvowel(true_identity[j]))
        first_vowel = tolower(true_identity[j]);
    }
    answer.push_back(first_consonant);
    answer.push_back(first_vowel);
  }

  answer[0] = toupper(answer[0]);
  return answer;
}

