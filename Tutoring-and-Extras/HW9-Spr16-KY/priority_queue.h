#ifndef priority_queue_h_
#define priority_queue_h_

#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

// A custom templated priority queue.  The priority queue is a min
// heap (smallest values on top).  The heap is stored in a vector.
// The integer location for an element within the vector can be
// quickly looked up using a map.

// find out the current index of the element within the heap.

// ASSIGNMENT: The class implementation is incomplete.  Finish the
//   implementation of this class, and add any functions you need.


// =========================================================================

template <class T>
class PriorityQueue {
 public:
	// CONSTRUCTOR
	PriorityQueue() {}

	// =========================
	// ACCESSORS
	unsigned int size() const { return m_heap.size(); }
	T top() const;

	// =========================
	// MODIFIERS
	// add an element
	void push(T element);
	// remove the top element, or an element elsewhere in the structure
	void pop();
	void remove(T element);
	// update the priority of an element somewhere in the structure
	void update_position(T element);
	bool contains(T element) const;

	// =========================
	// for debugging
	void print_heap(std::ostream & ostr) const;
	bool check_heap() const;
	
 private:
	// =========================
	// PRIVATE ACCESSOR FUNCTIONS
	int last_non_leaf() const { return ((int)size()-1) / 2; }
	int get_parent(int i) const { assert (i > 0 && i < (int)size()); return (i-1) / 2; }
	bool has_left_child(int i) const { return (2*i)+1 < (int)size(); }
	bool has_right_child(int i) const { return (2*i)+2 < (int)size(); }
	int get_left_child(int i) const { assert (i >= 0 && has_left_child(i)); return 2*i + 1; }
	int get_right_child(int i) const { assert (i >= 0 && has_right_child(i)); return 2*i + 2; }
	// PRIVATE MODIFIER FUNCTIONS
	void percolate_up(int i);
	void percolate_down(int i);

	// =========================
	// REPRESENTATION
	// the heap is stored in a vector representation 
	// (the binary tree structure "unrolled" one row at a time)
	std::vector<T> m_heap;
	// the map stores a correpondence between elements & indices in the heap
	std::map<T,int> locations;

};

// =========================================================================


// access the top element (minimum value) in the heap
template <class T>
T PriorityQueue<T>::top() const  {
	assert(!m_heap.empty());
	return m_heap[0]; 
}


// add a new element to the heap
template <class T>
void PriorityQueue<T>::push(T element) {
	// first, verify that the element isn't already in the heap
	typename std::map<T,int>::iterator itr = locations.find(element);
	if (itr != locations.end()) {
		std::cout << "ERROR!  priority queue aleady contains " << element << std::endl;
		assert (element == itr->first);
	}
	assert (itr == locations.end());
	// add the element at the edge of heap vector and percolate up
	m_heap.push_back(element);
	locations[element] = m_heap.size()-1;
	this->percolate_up(int(m_heap.size()-1));
}


// remove the top element (minimum value) from the heap
template <class T>
void PriorityQueue<T>::pop() {
	assert(!m_heap.empty());
	int success = locations.erase(m_heap[0]);
	assert (success == 1);
	// place the last element temporarily at the top of the heap, but
	// push it down to a proper position using percolate down
	m_heap[0] = m_heap.back();
	m_heap.pop_back();
	this->percolate_down(0);
}


// remove a specific element that could be anywhere in the heap
template <class T>
void PriorityQueue<T>::remove(T element) {
	typename std::map<T,int>::iterator itr = locations.find(element);
	if(itr != locations.end()){
		m_heap[itr->second] = m_heap[m_heap.size()-1];
		m_heap.pop_back();
		this->percolate_down(itr->second);
		locations.erase(itr);
	}
}


// this element may have had its value change, so we should adjust the
// position of the element within the heap (it might need to move up
// or down)
template <class T>
void PriorityQueue<T>::update_position(T element) {
	typename std::map<T,int>::iterator itr = locations.find(element);
	assert (itr != locations.end());
	this->percolate_up(itr->second);
	this->percolate_down(itr->second);
}

template <class T>
bool PriorityQueue<T>::contains(T element) const {
	typename std::map<T,int>::const_iterator itr = locations.find(element);
	if(itr != locations.end()){
		return true;
	} else{
		return false;
	}
}

// =========================================================================
// =========================================================================

// for debugging, print all of the data in the heap
template <class T>
void PriorityQueue<T>::print_heap(std::ostream & ostr) const {
	for (int i=0; i<(int)m_heap.size(); ++i)
		ostr << "[" << std::setw(4) << i << "] : " 
				 << std::setw(6) << m_heap[i]->getPriorityValue() 
				 << " " << *m_heap[i] << std::endl;
}


// for debugging, walk over the heap and check all of the elements &
// their values
template <class T>
bool PriorityQueue<T>::check_heap() const {
	if (m_heap.size() <= 1) return true;
	bool error_found = false;
	
	// first check to see if the element values are up-to-date
	for (unsigned int i=0; i < m_heap.size(); ++i) {
		if (m_heap[i]->CheckValue()) {
			error_found = true;
		}
	}
	
	// next loop over all nodes that have at least one child
	for (int i=0; has_left_child(i); ++i) {
		assert (has_left_child(i));
		int left_child = 2*i+1;
		// verify that this is a min heap
		// (parent is smaller value than both children
		if (m_heap[i]->getPriorityValue() > m_heap[left_child]->getPriorityValue()) {
			error_found = true;
			std::cout << "Error: at heap location " << i 
								<< ", the value is greater than the value at the left child.\n";
			std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue() 
								<< "value@" << left_child << "=" << m_heap[left_child]->getPriorityValue() << std::endl;
			
		}
		int right_child = 2*i+2;
		if (right_child < (int)m_heap.size() && 
				m_heap[i]->getPriorityValue() > m_heap[right_child]->getPriorityValue()) {
			error_found = true;
			std::cout << "Error: at heap location " << i 
								<< ", the value is greater than the value at the right child.\n";
			std::cout << "value@" << i<< "=" <<  m_heap[i]->getPriorityValue() 
								<< "value@" <<right_child << "=" << m_heap[right_child]->getPriorityValue() << std::endl;
		}
	}

	if (error_found) {
		print_heap(std::cout);
	}
	return !error_found;
}


// =========================================================================
// =========================================================================

// allow the element at this location to move up
template <class T>
void PriorityQueue<T>::percolate_up(int i) {
	while(i > 0){
		int parent = (i-1)/2;
		if(m_heap[i] < m_heap[parent]){
			locations[m_heap[i]] = parent;
			locations[m_heap[parent]] = i;
			std::swap(m_heap[i], m_heap[parent]);
			i = parent;
		} else{
			break;
		}
	}
}

// allow the element at this location to move down
template <class T>
void PriorityQueue<T>::percolate_down(int top) {
	while (top * 2 + 1 < int(m_heap.size())) {
		int child = top * 2 + 1;   // location of top's left child

		// find top's smaller child
		int swp = -1;
		if (child + 1 < int(m_heap.size()) && m_heap[child+1] < m_heap[child]){ // check right child
			swp = child + 1;
		} else{ 
			swp = child;
		}

		if (m_heap[swp] < m_heap[top]) {  // is child less than parent
			locations[m_heap[swp]] = top;
			locations[m_heap[top]] = swp;
			std::swap(m_heap[swp], m_heap[top]);
			top = swp;  // next time swap from here
		} else{
			break;
		}
	}

}

// =========================================================================
// =========================================================================
#endif
