// A simple "caller ID" program

#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

// add a number, name pair to the phonebook
void add(vector<string> &phonebook, int number, string const& name) {
  phonebook[number] = name;
}

void add(map<int,string> &mapbook, int number, string const& name){
  map<int,string>::iterator itr = mapbook.find(number);
  if( itr == mapbook.end() ){
    mapbook.insert( make_pair(number, name) );
  } else{
    cout << "Entry for #" << number << "updated. " << itr->second
          << " replaced with " << name << endl;
    itr->second = name;
  }
}

// given a phone number, determine who is calling
void identify(const vector<string> & phonebook, int number) {
  if (phonebook[number] == "UNASSIGNED") 
    cout << "unknown caller!" << endl;
  else 
    cout << phonebook[number] << " is calling!" << endl;
}

void identify(const map<int,string> & mapbook, int number) {
  map<int,string>::const_iterator itr = mapbook.find(number);
  if ( itr == mapbook.end() ){
    cout << "unknown caller!" << endl;
  } else{ 
    cout << itr->second << " is calling!" << endl;
  }
}

int main() {
  // create the phonebook; initially all numbers are unassigned
  vector<string> phonebook(10000, "UNASSIGNED");
  map<int, string> mapbook;

  // add several names to the phonebook
  add(phonebook, 1111, "fred");
  add(phonebook, 2222, "sally");
  add(phonebook, 3333, "george");
  //add(phonebook, 50001, "jeff");
  add(mapbook, 1111, "fred");
  add(mapbook, 2222, "sally");
  add(mapbook, 3333, "george");
  add(mapbook, 1234567, "bob");
  add(mapbook, 7735551234, "larry");

  // test the phonebook
  cout << "Vector: " << endl;
  identify(phonebook, 2222);
  identify(phonebook, 4444);
  //identify(phonebook, 50001);

  cout << endl << "Map: " << endl;
  identify(mapbook, 2222);
  identify(mapbook, 4444);
  identify(mapbook, 1234567);
  identify(mapbook, 7735551234);


}




