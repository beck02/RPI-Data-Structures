#ifndef _multi_ll_h_
#define _multi_ll_h_

#include <iostream>
#include <ctime>
#include "mtrand.h"

// ===================================================================================
// ===================================================================================
// NODE CLASS
// ===================================================================================
template <class T>
class Node {
public:
	Node() : chrono_next(NULL), chrono_prev(NULL), sorted_next(NULL), sorted_prev(NULL), random_next(NULL) {}
	Node(const T& v) : value(v), chrono_next(NULL), chrono_prev(NULL)
				, sorted_next(NULL), sorted_prev(NULL), random_next(NULL) {}

	// REPRESENTATION
	T value;
	Node<T>* chrono_next;
	Node<T>* chrono_prev;
	Node<T>* sorted_next;
	Node<T>* sorted_prev;
	Node<T>* random_next;
};

// A "forward declaration" of this class is needed
template <class T> class MultiLL;

// ===================================================================================
// ===================================================================================
// LIST ITERATORS
// ===================================================================================
template <class T>
class list_iterator {
public:
	enum Etype {Chrono, Sorted, Random};

	// default constructor, copy constructor, assignment operator, & destructor
	list_iterator() : ptr(NULL), type(Chrono) {}
	list_iterator(Node<T>* p, int t) : ptr(p) {
		if(t < 0 || t > 2){ std::cerr << "ERROR: Invalid iterator type" << std::endl; t = 0; }
		type = static_cast<Etype>(t);
	}
	list_iterator(Node<T>* p, Etype t) : ptr(p), type(t) {}
	list_iterator(const list_iterator<T>& old) : ptr(old.ptr), type(old.type) {}
	list_iterator<T>& operator=(const list_iterator<T>& old) {
		ptr = old.ptr; type = old.type; return *this; }
	~list_iterator() {}

	// dereferencing operator gives access to the value at the pointer
	T& operator*()  { 
		if(ptr == NULL){
			std::cerr << "ERROR: Null iterator dereference attempt" << std::endl;
		}
		return ptr->value;
	}

	Etype getType() const { return type; }
	int getIntType() const { int t = type; return t; }

	// increment & decrement operators
	list_iterator<T>& operator++() { // pre-increment, e.g., ++iter
		if(ptr == NULL){
			std::cerr << "ERROR: Null iterator increment attempt" << std::endl;
		} else if(type == Chrono){
			ptr = ptr->chrono_next;
		} else if(type == Sorted){
			ptr = ptr->sorted_next;
		} else if(type == Random){
			ptr = ptr->random_next;
		} else{
			std::cerr << "ERROR: Invalid iterator type to increment" << std::endl;
		}
		return *this;
	}
	list_iterator<T> operator++(int) { // post-increment, e.g., iter++
		list_iterator<T> temp(*this);
		++(*this);
		return temp;
	}
	list_iterator<T>& operator--() { // pre-decrement, e.g., --iter
		if(ptr == NULL){
			std::cerr << "ERROR: Null iterator decrement attempt" << std::endl;
		} else if(type == Chrono){
			ptr = ptr->chrono_prev;
		} else if(type == Sorted){
			ptr = ptr->sorted_prev;
		} else{
			std::cerr << "ERROR: Invalid iterator type to decrement" << std::endl;
		}
		return *this;
	}
	list_iterator<T> operator--(int) { // post-decrement, e.g., iter--
		list_iterator<T> temp(*this);
		--(*this);
		return temp;
	}

	// the MultiLL class needs access to the private ptr_ member variable
	friend class MultiLL<T>;

	// Comparions operators are straightforward
	bool operator==(const list_iterator<T>& r) const {
		return ptr == r.ptr; }
	bool operator!=(const list_iterator<T>& r) const {
		return ptr != r.ptr; }

	bool equals(const list_iterator<T>& r) const {
		return ptr == r.ptr && type == r.type; }

private:
	// REPRESENTATION
	Node<T>* ptr;    // ptr to node in the list
	Etype type;

};

// ===================================================================================
// ===================================================================================
// LIST CLASS DECLARATION
// ===================================================================================
// Note that it explicitly maintains the size of the list.
template <class T>
class MultiLL {
public:
	// default constructor, copy constructor, assignment operator, & destructor
	MultiLL() : seed(42), chrono_head(NULL), chrono_tail(NULL), sorted_head(NULL), sorted_tail(NULL), random_head(NULL), count(0) {}
	MultiLL(const MultiLL<T>& old) { this->copy_list(old); }
	MultiLL& operator= (const MultiLL<T>& old);
	~MultiLL() { this->destroy_list(); }

	// simple accessors & modifiers
	unsigned int size() const { return count; }
	bool empty() const { return chrono_head == NULL; }
	void clear() { this->destroy_list(); }

	// read/write access to contents
	// chrono order
	const T& chrono_front() const { return chrono_head->value;  }
	T& chrono_front() { return chrono_head->value; }
	const T& chrono_back() const { return chrono_tail->value; }
	T& chrono_back() { return chrono_tail->value; }
	//sorted order
	const T& sorted_front() const { return sorted_head->value;  }
	T& sorted_front() { return sorted_head->value; }
	const T& sorted_back() const { return sorted_tail->value; }
	T& sorted_back() { return sorted_tail->value; }
	//random order
	const T& random_front() const { return random_head->value;  }
	T& random_front() { return random_head->value; }

	typedef list_iterator<T> iterator;
	iterator begin_chronological() const { return iterator(chrono_head, iterator::Chrono); }
	iterator end_chronological() const { return iterator(NULL, iterator::Chrono); }
	iterator begin_sorted() const { return iterator(sorted_head, iterator::Sorted); }
	iterator end_sorted() const { return iterator(NULL, iterator::Sorted); }
	iterator begin_random();

	iterator add(const T& v);
	iterator erase(iterator itr);

private:
	// private helper functions
	void copy_list(const MultiLL<T>& old);
	void destroy_list();

	//REPRESENTATION
	int seed;
	Node<T>* chrono_head;
	Node<T>* chrono_tail;
	Node<T>* sorted_head;
	Node<T>* sorted_tail;
	Node<T>* random_head;
	unsigned int count;
};

// ===================================================================================
// ===================================================================================
// LIST CLASS IMPLEMENTATION
template <class T>
MultiLL<T>& MultiLL<T>::operator= (const MultiLL<T>& old) {
	// check for self-assignment
	if (&old != this) {
		this->destroy_list();
		this->copy_list(old);
	}
	return *this;
}

// ===================================================================================

// do these lists look the same (length & contents)?
template <class T>
bool operator== (MultiLL<T>& left, MultiLL<T>& right) {
	if (left.size() != right.size()) return false;
	typename MultiLL<T>::iterator left_itr = left.begin_chronological();
	typename MultiLL<T>::iterator right_itr = right.begin_chronological();
	// walk over both lists, looking for a mismatched value
	while (left_itr != left.end_chronological()) {
		if (*left_itr != *right_itr) return false;
		left_itr++; right_itr++;
	}
	return true;
}

// ===================================================================================

template <class T>
bool operator!= (MultiLL<T>& left, MultiLL<T>& right){ return !(left==right); }

// ===================================================================================

template <class T>
typename MultiLL<T>::iterator MultiLL<T>::begin_random(){
	//zero out all random pointers
	random_head = NULL;
	for(iterator itr = begin_chronological(); itr != end_chronological(); ++itr){
		itr.ptr->random_next = NULL;
	}

	bool TESTING = false;
	if(!TESTING){
		if(seed != time(NULL)){
			seed = time(NULL);
		} else{
			MTRand_int32 r(seed);
			seed = ( r()/2 );
		}
	}

	MTRand_int32 rand_num(seed); 

	int num_placed = 0;
	iterator prev_node_itr;
	/* find a location for a new link in the chain and set it to follow the most recently 
	set (and so current last) link in the chain */
	while(num_placed < count){
		int steps = ( rand_num() % count );

		
		iterator curr_node_itr, end;
		int type = ( rand_num() % 2 );
		if(type == 0){
			curr_node_itr = begin_chronological();
			end = end_chronological();
		} else{
			curr_node_itr = begin_sorted();
			end = end_sorted();
		}

		/* step through the list the random number of spaces to find our
		new potential next location */
		for(int i = 0; i < steps; ++i){ 
			if(curr_node_itr == end){ break; }
			++curr_node_itr;
		}

		// std::cout << "next attempt: " << steps << " " << *curr_node_itr << std::endl;

		/* only set the node we stepped to as the next element in the chain
		if it's not already part of the chain */
		if(curr_node_itr.ptr->random_next == NULL && curr_node_itr != prev_node_itr){
			// std::cout << "  success!  (num_placed = " << num_placed+1 << ")  ";
			if(num_placed == 0 || random_head == NULL){
				random_head = curr_node_itr.ptr;
				// std::cout << "set as new head" << std::endl;
			} else{
				prev_node_itr.ptr->random_next = curr_node_itr.ptr;
				// std::cout << "placed following " << *prev_node_itr << std::endl;
			}
			prev_node_itr = curr_node_itr;
			++num_placed;
		}
	}
	/* above loop will set random_next for every node but the last one in the chain,
	because for that one the next location is the head, so set it manually */
	prev_node_itr.ptr->random_next = random_head;
	// std::cout << "last node (" << *prev_node_itr << ") linked to head (" << random_head->value << ")" << std::endl;
	
	return iterator(random_head, iterator::Random);
}

// ===================================================================================

template <class T>
typename MultiLL<T>::iterator MultiLL<T>::add(const T& v){
	Node<T>* nnode = new Node<T>(v);
	//empty list
	if(chrono_tail == NULL || sorted_tail == NULL){
		chrono_head = chrono_tail = sorted_head = sorted_tail = nnode;
	}
	else{
		//enter at end chronologically
		nnode->chrono_prev = chrono_tail;
		chrono_tail->chrono_next = nnode;
		chrono_tail = nnode;

		// //figure out where it goes in sorted order
		if(v < sorted_head->value){
			nnode->sorted_next = sorted_head;
			sorted_head->sorted_prev = nnode;
			sorted_head = nnode;
		} else if(sorted_tail->value < v){
			nnode->sorted_prev = sorted_tail;
			sorted_tail->sorted_next = nnode;
			sorted_tail = nnode;
		} else{
			iterator s_itr;
			for(s_itr = begin_sorted(); s_itr != end_sorted(); ++s_itr){
				if(v < (*s_itr) ){
					nnode->sorted_next = s_itr.ptr;
					nnode->sorted_prev = s_itr.ptr->sorted_prev;
					nnode->sorted_prev->sorted_next = nnode;
					nnode->sorted_next->sorted_prev = nnode;
					break;
				}
			}
		}
	}
	++count;
	return iterator(nnode, iterator::Chrono);
}

// ===================================================================================

template <class T>
typename MultiLL<T>::iterator MultiLL<T>::erase(iterator itr) {
	if(count == 0 || itr.ptr == NULL){
		return itr;
	}
	--count;
	iterator result(itr);
	++result;
	//Random set is invalidated by erase until reset by begin_random()
	random_head = NULL;
	// One node left in the list.
	if ( (itr.ptr == chrono_head || itr.ptr == sorted_head) && (chrono_head == chrono_tail || sorted_head == sorted_tail) ) {
		chrono_head = chrono_tail = sorted_head = sorted_tail = NULL;
	}
	// two or more nodes left in the list
	else{
		//set chrono pointers
		if(itr.ptr == chrono_head){ //removing chrono_head
			chrono_head = chrono_head->chrono_next;
			chrono_head->chrono_prev = NULL;
		} 
		else if(itr.ptr == chrono_tail){ //removing chrono_tail
			chrono_tail = chrono_tail->chrono_prev;
			chrono_tail->chrono_next = NULL;
		} 
		else{ //normal chrono remove
			itr.ptr->chrono_prev->chrono_next = itr.ptr->chrono_next;
			itr.ptr->chrono_next->chrono_prev = itr.ptr->chrono_prev;
		}

		//set sorted pointers
		if(itr.ptr == sorted_head){ //remove sorted_head
			sorted_head = sorted_head->sorted_next;
			sorted_head->sorted_prev = NULL;
		}
		else if(itr.ptr == sorted_tail){ //remove sorted_tail
			sorted_tail = sorted_tail->sorted_prev;
			sorted_tail->sorted_next = NULL;
		} 
		else{ //normal sorted remove
			itr.ptr->sorted_prev->sorted_next = itr.ptr->sorted_next;
			itr.ptr->sorted_next->sorted_prev = itr.ptr->sorted_prev;
		}
	}
	delete itr.ptr;
	return result;
}

// ===================================================================================

template <class T>
void MultiLL<T>::copy_list(const MultiLL<T>& old) {
	chrono_head = chrono_tail = sorted_head = sorted_tail = random_head = NULL;
	count = 0;
	seed = old.seed;
	iterator itr = old.begin_chronological();
	while(itr != old.end_chronological()){
		add(*itr);
		++itr;
	}
}

// ===================================================================================

template <class T>
void MultiLL<T>::destroy_list() {
	iterator itr = begin_chronological();
	iterator itr2(sorted_head, 1);
	while(itr != end_chronological()){
		itr = erase(itr); //automatically increments when element is deleted
	}
}

// ===================================================================================
// ===================================================================================

#endif





