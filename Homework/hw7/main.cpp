#include <cstdlib>
#include <iostream>
#include <map>
#include <cassert>
#include <string>
#include <vector>
#include <list>
using namespace std;


// two data structures store all of the information for efficiency
typedef map<string, pair<int, list<string> > > LIBRARY_TYPE;
typedef map<string, vector<string> > PEOPLE_TYPE;


// prototypes for the helper functions
void addItem(LIBRARY_TYPE& library);
void checkoutItem(LIBRARY_TYPE& library, PEOPLE_TYPE& people);
void returnItems(LIBRARY_TYPE& library, PEOPLE_TYPE& people);
void lookup(LIBRARY_TYPE& library);
void printPeople(PEOPLE_TYPE& people);


int main() {
  LIBRARY_TYPE library;
  PEOPLE_TYPE people;
  char c;
  while (cin >> c) {
    if (c == 'a') {
      addItem(library);
    } else if (c == 'c') {
      checkoutItem(library,people);
    } else if (c == 'r') {
      returnItems(library,people);
    } else if (c == 'l') {
      lookup(library);
    } else if (c == 'p') {
      printPeople(people);
    } else {
      cerr << "error unknown char " << c << endl;
      exit(0);
    }
  }
  return 1;
}


void addItem(LIBRARY_TYPE& library) {
  string title;
  int num;
  cin >> num >> title;
  if (num <= 0){
    //cerr << "Can't add 0 or negative copies of a book to library." << endl;
    return;
  }
  library[title].first += num;
  cout << "added " << num;
  if(num == 1){
    cout << " copy of ";
  } else{
    cout << " copies of ";
  }
  cout << title << endl;
}


void checkoutItem(LIBRARY_TYPE& library, PEOPLE_TYPE& people) {
  string name;
  string title;
  cin >> name >> title;
  bool chdout = false;
  vector<string> books = people[name];
  for(unsigned int i = 0; i < books.size(); ++i){
    if(books[i] == title){
      chdout = true;
      cout << name << " already has " << title << endl;
      break;
    }
  }
  if(!chdout){
    LIBRARY_TYPE::iterator itr = library.find(title);
    if( itr == library.end() ){
      cout << "don't have " << title << " in library" << endl;
    } else if( itr->second.first == 0 ){
      cout << "no copies of " << title << " available" << endl;
    } else{
      itr->second.first--;
      itr->second.second.push_back(name);
      people[name].push_back(title);
      cout << name << " checked out " << title << endl;
    }
  }
}


void returnItems(LIBRARY_TYPE& library, PEOPLE_TYPE& people) {
  string name;
  cin >> name;
  vector<string> books = people[name];
  for (unsigned int i = 0; i < books.size(); ++i){
    library[books[i]].first++;
    list<string>* ppl = &(library[books[i]].second);
    for(list<string>::iterator itr = ppl->begin();
        itr != ppl->end();
        ++itr){
      if( *itr == name ){
        itr = ppl->erase(itr);
        break;
      }
    }
  }
  cout << name;
  if(books.size() == 0){
    cout << " has no items checked out" << endl;
  } else if(people[name].size() == 1){
    cout << " returned 1 item" << endl;
  } else{
    cout << " returned " << books.size() << " items" << endl;
  }
  people[name].clear();
}


void lookup(LIBRARY_TYPE& library) {
  string title;
  cin >> title;
  LIBRARY_TYPE::iterator lit = library.find(title);
  if( lit == library.end() ){
    cout << "don't have " << title << " in library" << endl;
    return;
  }
  pair<int, list<string> >* bookinfo = &(lit->second);
  cout << "library info for: " << title << endl;
  if(bookinfo->first == 1){
    cout << "  " << bookinfo->first << " copy available" << endl;
  } else if(bookinfo->first > 1){
    cout << "  " << bookinfo->first << " copies available" << endl;
  }
  if( bookinfo->second.size() > 0 ){
    cout << "  " << bookinfo->second.size();
    if(bookinfo->second.size() == 1){
      cout << " copy checked out by:" << endl;
    } else{
      cout << " copies checked out by:" << endl;
    }
    for(list<string>::const_iterator itr = bookinfo->second.begin();
        itr != bookinfo->second.end();
        ++itr ){
      cout << "    " << *itr << endl;
    }
  }
}


void printPeople(PEOPLE_TYPE& people) {
  for(PEOPLE_TYPE::const_iterator itr = people.begin(); itr != people.end(); ++itr){
    if(itr->second.size() == 0){
      continue;
    } else if(itr->second.size() == 1){
      cout << itr->first << " has this item checked out:" << endl;
      cout << "  " << itr->second[0] << endl;
    } else{
      cout << itr->first << " has these items checked out:" << endl;
      for(unsigned int i = 0; i < itr->second.size(); ++i){
        cout << "  " << itr->second[i] << endl;
      }
    }
  }
}
