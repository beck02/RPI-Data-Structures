// CSCI 1200 Data Structures
// Homework 5: Multi-Linked Lists


// NOTE: You should not need to make any changes to this file, except
// to add your own test cases at the bottom of the file where
// indicated.


#include <iostream>
#include <cmath>
#include <string>
#include <list>
#include <vector>
#include <cassert>
#include <cstdlib>

#include "multi_linked_list.h"

void ProvidedTests();
void StudentTests();
void TypeTests();
void CopyTests();
void DecrementTests();

// ================================================================================
// A simple function to test if two STL lists contain the "same" elements
// returns true if it contains the same number of elements in the same order
// otherwise returns false
template <class T> bool same(const std::list<T> &a, const std::list<T> &b) {
	if (a.size() != b.size()) return false;
	typename std::list<T>::const_iterator a_itr = a.begin();
	typename std::list<T>::const_iterator b_itr = b.begin();
	while (a_itr != a.end()) {
		if (*a_itr != *b_itr) return false;
		a_itr++;
		b_itr++;
	}
	return true;
}

// ================================================================================
// This program stress tests the templated MultiLL container class

int main() {
	ProvidedTests();
	StudentTests();
}

// ================================================================================
// ================================================================================

void ProvidedTests() {

	std::cout << "Begin ProvidedTests..." << std::endl;

	// The test data (stored in STL lists)
	std::list<std::string> songs;
	songs.push_back("hound dog");
	songs.push_back("poker face");
	songs.push_back("brown eyed girl");
	songs.push_back("let it be");
	songs.push_back("walk like an egyptian");
	songs.push_back("man in the mirror");
	songs.push_back("stairway to heaven");
	songs.push_back("dancing in the street");
	songs.push_back("every breath you take");
	songs.push_back("hotel california");
	// the same data, sorted!
	std::list<std::string> sorted_songs(songs);
	sorted_songs.sort();

	// create an empty multi-linked list and fill it with the test data
	MultiLL<std::string> my_list;
	for (std::list<std::string>::iterator itr = songs.begin(); itr != songs.end(); itr++) {
		my_list.add(*itr);
	}
	assert (songs.size() == my_list.size());

	// -------------------
	// iterator tests

	// test the chronological iterator (forwards)
	std::cout << "chronological order" << std::endl;
	std::list<std::string> chrono_order;
	MultiLL<std::string>::iterator itr = my_list.begin_chronological();
	while (itr != my_list.end_chronological()) {
		std::cout << "  " << *itr << std::endl;
		chrono_order.push_back(*itr);
		itr++;
	}
	std::cout << std::endl;
	assert (same(songs,chrono_order));

	// test the sorted order iterator (forwards)
	std::cout << "sorted order" << std::endl;
	std::list<std::string> sorted_order;
	itr = my_list.begin_sorted();
	while (itr != my_list.end_sorted()) {
		std::cout << "  " << *itr << std::endl;
		sorted_order.push_back(*itr);
		itr++;
	}
	std::cout << std::endl;
	assert (same(sorted_songs,sorted_order));


	// test the random order iterator
	std::cout << "random order" << std::endl;
	std::list<std::string> random_order;
	itr = my_list.begin_random();
	for (unsigned int i = 0; i < my_list.size(); i++,itr++) {
		std::cout << "  " << *itr << std::endl;
		random_order.push_back(*itr);
	}
	std::cout << std::endl;
	// loop through the elements a second time (the order should be the same!)
	std::list<std::string>::iterator itr2 = random_order.begin();
	for (unsigned int i = 0; i < my_list.size(); i++,itr++,itr2++) {
		// verify that the elements repeat the order
		assert (*itr == *itr2);
	}
	std::list<std::string> random_order_check(random_order);
	random_order_check.sort();
	// verify that all of the elements appeared in the initial loop
	assert (same(sorted_songs,random_order_check));

	// test the re-randomization by creating a new random iterator
	std::cout << "random order 2" << std::endl;
	std::list<std::string> random_order2;
	itr = my_list.begin_random();
	for (unsigned int i = 0; i < my_list.size(); i++,itr++) {
		std::cout << "  " << *itr << std::endl;
		random_order2.push_back(*itr);
	}
	std::cout << std::endl;
	// with over 3 million different possible permutations of 10
	// elements, it is highly unlikely they will be the same!
	assert (!same(random_order,random_order2));
	
	// -------------------
	// erase tests

	// erase the first element inserted
	itr = my_list.begin_chronological();
	assert (*itr == "hound dog");
	itr = my_list.erase(itr);
	assert (*itr == "poker face");
	assert (my_list.size() == 9);
	std::cout << "erased: hound dog" << std::endl;

	// erase the second to last element in sorted order
	itr = my_list.begin_sorted();
	for (int i = 0; i < 7; i++) { itr++; }
	assert (*itr == "stairway to heaven");
	itr = my_list.erase(itr);
	assert (*itr == "walk like an egyptian");
	assert (my_list.size() == 8);  
	std::cout << "erased: stairway to heaven" << std::endl;

	// erase the third element in the random order
	itr = my_list.begin_random();
	itr++;
	itr++;
	std::string tmp = *itr;
	// note that the return value of erase with a random iterator is undefined
	my_list.erase(itr);
	std::cout << "erased: " << tmp << std::endl;
	assert (my_list.size() == 7);
	assert (!my_list.empty());

	my_list.clear();
	assert (my_list.empty());
	assert (my_list.size() == 0);
	std::cout << "cleared the whole list!" << std::endl << std::endl;

	std::cout << "Finished ProvidedTests." << std::endl;
}

// ================================================================================
// ================================================================================

void StudentTests() {

	std::cout << "Begin StudentTests..." << std::endl;

	// ---------------------------
	// ADD YOUR OWN TEST CASES BELOW
	
	// be sure to test:
	//   copy constructor
	//   assignment operator
	//   destructor
	//   all corner cases of erase
	//   decrement operator for the chronological & sorted iterators
	//   pre vs. post increment & decrement operations for iterators
	//   MultiLL containing types other than std::string
	//   anything else that is necessary

	std::cout << "----TypeTests----" << std::endl;
	// TypeTests();

	std::cout << "----CopyTests----" << std::endl;
	CopyTests();

	std::cout << "----DecrementTests----" << std::endl;
	DecrementTests();

	std::cout << "Finished StudentTests." << std::endl;
}

// ================================================================================

void TypeTests(){
	// The test data (stored in STL lists)
	std::list<int> lst_int;
	lst_int.push_back(12); lst_int.push_back(3); lst_int.push_back(66); lst_int.push_back(25);
	lst_int.push_back(34); lst_int.push_back(102); lst_int.push_back(567); lst_int.push_back(1);
	// the same data, sorted!
	std::list<int> sorted_lst_int(lst_int);
	sorted_lst_int.sort();

	std::list<char> lst_char;
	lst_char.push_back('&'); lst_char.push_back('4'); lst_char.push_back('a'); lst_char.push_back('G');
	lst_char.push_back(']'); lst_char.push_back('f'); lst_char.push_back('?'); lst_char.push_back('0');
	// the same data, sorted!
	std::list<char> sorted_lst_char(lst_char);
	sorted_lst_char.sort();

	// create an empty multi-linked list and fill it with the test data
	MultiLL<int> mllist_int;
	for (std::list<int>::iterator itr = lst_int.begin(); itr != lst_int.end(); itr++) {
		mllist_int.add(*itr);
	}
	assert (lst_int.size() == mllist_int.size());
	MultiLL<char> mllist_char;
	for (std::list<char>::iterator itr = lst_char.begin(); itr != lst_char.end(); itr++) {
		mllist_char.add(*itr);
	}
	assert (lst_char.size() == mllist_char.size());

	std::cout << "chronological order" << std::endl;
	std::list<int> chrono_order_int;
	MultiLL<int>::iterator int_itr = mllist_int.begin_chronological();
	std::list<char> chrono_order_char;
	MultiLL<char>::iterator char_itr = mllist_char.begin_chronological();
	std::string int_order = "  ints:";
	std::string char_order = "  chars:";
	while (int_itr != mllist_int.end_chronological()) {
		int_order += " " + std::to_string(*int_itr);
		char_order += *char_itr;
		chrono_order_int.push_back(*int_itr);
		chrono_order_char.push_back(*char_itr);
		int_itr++; char_itr++;
	}
	std::cout << int_order << std::endl;
	std::cout << char_order << std::endl;
	std::cout << std::endl;
	assert (same(lst_int,chrono_order_int));
	assert (same(lst_char,chrono_order_char));

	// test the sorted order iterator (forwards)
	std::cout << "sorted order" << std::endl;
	std::list<int> sorted_order_int;
	int_itr = mllist_int.begin_sorted();
	int_order = "  ints:";
	std::list<char> sorted_order_char;
	char_itr = mllist_char.begin_sorted();
	char_order = "  chars:";
	while (int_itr != mllist_int.end_sorted()) {
		int_order += " " + std::to_string(*int_itr);
		char_order += *char_itr;
		sorted_order_int.push_back(*int_itr);
		sorted_order_char.push_back(*char_itr);
		int_itr++; char_itr++;
	}
	std::cout << int_order << std::endl;
	assert (same(sorted_lst_int,sorted_order_int));
	std::cout << char_order << std::endl;
	assert (same(sorted_lst_char,sorted_order_char));
	std::cout << std::endl;

	std::cout << "random order" << std::endl;
	std::list<int> random_order_int;
	int_itr = mllist_int.begin_random();
	int_order = "  ints:";
	std::list<char> random_order_char;
	char_itr = mllist_char.begin_random();
	char_order = "  chars:";
	for (unsigned int i = 0; i < mllist_int.size(); i++) {
		int_order += " " + std::to_string(*int_itr);
		char_order += *char_itr;
		random_order_int.push_back(*int_itr);
		random_order_char.push_back(*char_itr);
		int_itr++; char_itr++;
	}
	std::cout << int_order << std::endl;
	std::cout << char_order << std::endl;
	std::cout << std::endl;

	// loop through the elements a second time (the order should be the same!)
	std::list<int>::iterator int_itr2 = random_order_int.begin();
	std::list<char>::iterator char_itr2 = random_order_char.begin();
	for (unsigned int i = 0; i < mllist_int.size(); i++) {
		// verify that the elements repeat the order
		assert (*int_itr == *int_itr2);
		int_itr++; int_itr2++;
		assert (*char_itr == *char_itr2);
		char_itr++; char_itr2++;
	}
	std::list<int> random_order_int_check(random_order_int);
	random_order_int_check.sort();
	assert (same(sorted_lst_int,random_order_int_check));
	std::list<char> random_order_char_check(random_order_char);
	random_order_char_check.sort();
	assert (same(sorted_lst_char,random_order_char_check));

	std::cout << "random order 2" << std::endl;
	std::list<int> random_order_int2;
	int_itr = mllist_int.begin_random();
	int_order = "  ints:";
	std::list<char> random_order_char2;
	char_itr = mllist_char.begin_random();
	char_order = "  chars:";
	for (unsigned int i = 0; i < mllist_int.size(); i++,int_itr++) {
		int_order += " " + std::to_string(*int_itr);
		char_order += *char_itr;
		random_order_int2.push_back(*int_itr);
		random_order_char2.push_back(*char_itr);
		int_itr++; char_itr++;
	}
	std::cout << int_order << std::endl;
	std::cout << char_order << std::endl;
	std::cout << std::endl;
	// with over 3 million different possible permutations of 10
	// elements, it is highly unlikely they will be the same!
	assert (!same(random_order_int,random_order_int2));
	assert (!same(random_order_char,random_order_char2));

}

// ================================================================================

void CopyTests(){
	MultiLL<std::string> mult_list1;
	mult_list1.add("first"); mult_list1.add("second"); mult_list1.add("third"); mult_list1.add("fourth");
	mult_list1.add("fifth"); mult_list1.add("sixth"); mult_list1.add("seventh"); mult_list1.add("eighth");

	MultiLL<std::string> mult_list2;
	mult_list2.add("Z"); mult_list2.add("A"); mult_list2.add("Y"); mult_list2.add("B");
	mult_list2.add("X"); mult_list2.add("C"); mult_list2.add("W"); mult_list2.add("D");

	std::cout << "original lists:" << std::endl;
	std::string lst1 = "  lst1:";
	std::string lst2 = "  lst2:";
	MultiLL<std::string>::iterator itr1 = mult_list1.begin_chronological();
	MultiLL<std::string>::iterator itr2 = mult_list2.begin_chronological();
	while(itr1 != mult_list1.end_chronological()){
		lst1 += "  " + *itr1;
		lst2 += "  " + *itr2;
		++itr1; ++itr2;
	}
	std::cout << lst1 << std::endl;
	std::cout << lst2 << std::endl;
	std::cout << std::endl;

	MultiLL<std::string> mult_list3(mult_list1);
	assert(mult_list1.size() == mult_list3.size());
	mult_list2 = mult_list1;
	assert(mult_list1.size() == mult_list2.size());

	std::cout << "chronological order" << std::endl;
	lst1 = "  lst1:";
	lst2 = "  lst2:";
	std::string lst3 = "  lst3:";
	itr1 = mult_list1.begin_chronological();
	itr2 = mult_list2.begin_chronological();
	MultiLL<std::string>::iterator itr3 = mult_list3.begin_chronological();
	while(itr1 != mult_list1.end_chronological()){
		assert(*itr1 == *itr2);
		assert(*itr1 == *itr3);
		lst1 += "  " + *itr1;
		lst2 += "  " + *itr2;
		lst3 += "  " + *itr3;
		++itr1; ++itr2; ++itr3;
	}
	std::cout << lst1 << std::endl;
	std::cout << lst2 << std::endl;
	std::cout << lst3 << std::endl;
	std::cout << std::endl;

	std::cout << "sorted order" << std::endl;
	lst1 = "  lst1:";
	lst2 = "  lst2:";
	lst3 = "  lst3:";
	itr1 = mult_list1.begin_sorted();
	itr2 = mult_list2.begin_sorted();
	itr3 = mult_list3.begin_sorted();
	while(itr1 != mult_list1.end_sorted()){
		assert(*itr1 == *itr2);
		// assert(*itr1 == *itr3);
		lst1 += "  " + *itr1;
		lst2 += "  " + *itr2;
		lst3 += "  " + *itr3;
		++itr1; ++itr2; ++itr3;
	}
	std::cout << lst1 << std::endl;
	std::cout << lst2 << std::endl;
	std::cout << lst3 << std::endl;
	std::cout << std::endl;
}

// ================================================================================

void DecrementTests(){
	std::list<std::string> lst;
	lst.push_back("first"); lst.push_back("second"); lst.push_back("third"); lst.push_back("fourth");
	lst.push_back("fifth"); lst.push_back("sixth"); lst.push_back("seventh"); lst.push_back("eighth");

	std::list<std::string> sorted_lst(lst);
	sorted_lst.sort();

	MultiLL<std::string> mllst;
	for (std::list<std::string>::iterator itr = lst.begin(); itr != lst.end(); itr++) {
		mllst.add(*itr);
	}
	assert(lst.size() == mllst.size());

	std::cout << "reverse chronological order" << std::endl;
	std::list<std::string>::iterator itr1 = lst.end(); --itr1;
	MultiLL<std::string>::iterator itr2 = mllst.begin_chronological();
	for(int i = 0; i < mllst.size()-1; ++i){ ++itr2; }
	assert(*itr2 == mllst.chrono_back());
	std::string lst_str = "    lst:";
	std::string mllst_str = "  mllst:";
	while(itr1 != lst.begin()){
		assert(*itr1 == *itr2);
		lst_str += "  " + *itr1;
		mllst_str += "  " + *itr2;
		--itr1; --itr2;
	}
	assert(*itr1 == *itr2);
	lst_str += "  " + *itr1;
	mllst_str += "  " + *itr2;
	std::cout << lst_str << std::endl;
	std::cout << mllst_str << std::endl;
	std::cout << std::endl;

	std::cout << "reverse sorted order" << std::endl;
	itr1 = sorted_lst.end(); --itr1;
	itr2 = mllst.begin_sorted();
	for(int i = 0; i < mllst.size()-1; ++i){ ++itr2; }
	assert(*itr2 == mllst.sorted_back());
	lst_str = "  s_lst:";
	mllst_str = "  mllst:";
	while(itr1 != sorted_lst.begin()){
		assert(*itr1 == *itr2);
		lst_str += "  " + *itr1;
		mllst_str += "  " + *itr2;
		--itr1; --itr2;
	}
	assert(*itr1 == *itr2);
	lst_str += "  " + *itr1;
	mllst_str += "  " + *itr2;
	std::cout << lst_str << std::endl;
	std::cout << mllst_str << std::endl;
	std::cout << std::endl;

}

// ================================================================================
// ================================================================================


















