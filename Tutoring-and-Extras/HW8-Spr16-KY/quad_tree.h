// ===================================================================
//
// We provide the Point class and the implementation of several
// QuadTree member functions for printing.  
//
// IMPORTANT: You should modify this file to add all of the necessary
// functionality for the QuadTree class and its helper classes: Node,
// DepthIterator, and BreadthIterator.
//
// ===================================================================

#ifndef quad_tree_h_
#define quad_tree_h_

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>


// ==============================================================
// ==============================================================
// A tiny templated class to store 2D coordinates.  This class works
// with number_type = int, float, double, unsigned char (a 1 byte=8
// bit integer), short (a 2 byte=16 bit integer).

template <class number_type>
class Point {
public:
	Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
	// REPRESENTATION
	number_type x;
	number_type y;
};

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
	ostr << "(" << pt.x << "," << pt.y << ")";
	return ostr;
}

// ==============================================================
// ==============================================================

template <class number_type, class label_type>
class Node{
public:
	Node() : parent_(NULL) {
		for(int i = 0; i < 4; ++i){
			children[i] = NULL;
		}
	}
	Node(const label_type& lbl, const number_type& x, const number_type& y) : pt(x, y), label(lbl), parent_(NULL) {
		for(int i = 0; i < 4; ++i){
			children[i] = NULL;
		}
	}
	
	Point<number_type> pt;
	label_type label;
	Node<number_type, label_type>* parent_;
	/*
	children[0] = top left quadrant,  children[1] = top right quadrant
	children[2] = bottom left quadrant,  children[3] = bottom right quadrant
	*/
	Node<number_type, label_type>* children[4];
};

// ==============================================================
// ==============================================================

// A "forward declaration" of this class is needed
template <class number_type, class label_type> class QuadTree;

// ==============================================================
// ==============================================================

template <class number_type, class label_type>
class DepthIterator{
public:
	DepthIterator() : ptr_(NULL) {}
	DepthIterator(Node<number_type, label_type>* p) : ptr_(p) {}
	DepthIterator(const DepthIterator& old) : ptr_(old.ptr_){}
	~DepthIterator(){}
	DepthIterator& operator=(const DepthIterator& old) { ptr_ = old.ptr_;  return *this; }
	// operator* gives constant access to the value at the pointer
	const Point<number_type>& operator*() const { return ptr_->pt; }
	const label_type& getLabel() const { return ptr_->label; }
	// comparions operators are straightforward
	bool operator== (const DepthIterator& rgt) { return ptr_ == rgt.ptr_; }
	bool operator!= (const DepthIterator& rgt) { return ptr_ != rgt.ptr_; }
	
	int getDepth(){
		Node<number_type, label_type>* temp = ptr_;
		int depth = 0;
		while(temp->parent_ != NULL){
			depth++;
			temp = temp->parent_;
		}
		return depth;
	}

	DepthIterator<number_type, label_type>& operator++() {  // pre-increment, ++itr
		if(ptr_->children[0] != NULL){
			ptr_ = ptr_->children[0];
		} else if(ptr_->children[1] != NULL){
			ptr_ = ptr_->children[1];
		} else if(ptr_->children[2] != NULL){
			ptr_ = ptr_->children[2];
		} else if(ptr_->children[3] != NULL){
			ptr_ = ptr_->children[3];
		} else if(ptr_->parent_ == NULL){
			ptr_ = NULL;
		} else{
			while(ptr_->parent_ != NULL){
				Node<number_type, label_type>* temp = ptr_->parent_;
				if(temp->children[0] == ptr_){ //we were the first child
					if(temp->children[1] != NULL){
						ptr_ = temp->children[1];
						break;
					} else if(temp->children[2] != NULL){
						ptr_ = temp->children[2];
						break;
					} else if(temp->children[3] != NULL){
						ptr_ = temp->children[3];
						break;
					} else{
						ptr_ = temp;
					}
				} else if(temp->children[1] == ptr_){ //we were the second child
					if(temp->children[2] != NULL){
						ptr_ = temp->children[2];
						break;
					} else if(temp->children[3] != NULL){
						ptr_ = temp->children[3];
						break;
					} else{
						ptr_ = temp;
					}
				} else if(temp->children[2] == ptr_){ //we were the third child
					if(temp->children[3] != NULL){
						ptr_ = temp->children[3];
						break;
					} else{
						ptr_ = temp;
					}
				} else{ //we were the last (fourth) chld
					ptr_ = temp;
				}

			}
			if(ptr_->parent_ == NULL){
				ptr_ = NULL;
			}
		}
		return *this;
	}
	DepthIterator<number_type, label_type> operator++(int) {  // post-increment, itr++
		DepthIterator<number_type, label_type> temp(*this);
		++(*this);
		return temp;
	}

private:
	Node<number_type, label_type>* ptr_;
};

// ==============================================================
// ==============================================================

template <class number_type, class label_type>
class BreadthIterator{
public:
	BreadthIterator() : ptr_(NULL) {}
	// BreadthIterator(Node<number_type, label_type>* p) : ptr_(p) {}
	BreadthIterator(const BreadthIterator& old) : ptr_(old.ptr_), queue(old.queue) {}
	~BreadthIterator(){}
	BreadthIterator& operator=(const BreadthIterator& old) { ptr_ = old.ptr_;  return *this; }
	// operator* gives constant access to the value at the pointer
	const Point<number_type>& operator*() const { return ptr_->pt; }
	const label_type& getLabel() const { return ptr_->label; }
	// comparions operators are straightforward
	bool operator== (const BreadthIterator& rgt) { return ptr_ == rgt.ptr_; }
	bool operator!= (const BreadthIterator& rgt) { return ptr_ != rgt.ptr_; }
	
	int getDepth(){
		Node<number_type, label_type>* temp = ptr_;
		int depth = 0;
		while(temp->parent_ != NULL){
			depth++;
			temp = temp->parent_;
		}
		return depth;
	}

	friend class QuadTree<number_type, label_type>;

	BreadthIterator<number_type, label_type>& operator++() {  // pre-increment, ++itr
		if(queue.size() == 0){
			ptr_ = NULL;
		} else{
			ptr_ = queue.front();
			queue.pop_front();
			for(int i = 0; i < 4; ++i){
				if(ptr_->children[i] != NULL){
					queue.push_back(ptr_->children[i]);
				}
			}
		}
		return *this;
	}
	BreadthIterator<number_type, label_type> operator++(int) {  // post-increment, itr++
		BreadthIterator<number_type, label_type> temp(*this);
		++(*this);
		return temp;
	}

private:
	Node<number_type, label_type>* ptr_;
	std::list<Node<number_type, label_type>* > queue;
};

// ==============================================================
// ==============================================================

template <class number_type, class label_type>
class QuadTree{
public:
	QuadTree() : root_(NULL), size_(0) {}
	QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_) { 
		root_ = this->copy_tree(old.root_);
	}
	QuadTree& operator=(const QuadTree<number_type, label_type>& old) {
		if (&old != this) {
			this->destroy_tree(root_);
			root_ = this->copy_tree(old.root_);
			size_ = old.size_;
		}
		return *this;
	}
	~QuadTree() {
		this->destroy_tree(root_);
		root_ = NULL;
	}

	typedef DepthIterator<number_type, label_type> iterator;
	typedef BreadthIterator<number_type, label_type> bf_iterator;


	unsigned int size() const { return size_; }
	int height() const { return height(root_); }
	iterator begin(){ return iterator(root_); }
	iterator end(){ return iterator(NULL); }

	bf_iterator bf_begin(){ 
		bf_iterator itr;
		itr.ptr_ = root_;
		for(int i = 0; i < 4; i++){
			if(root_->children[i] != NULL){
				itr.queue.push_back(root_->children[i]);
			}
		}
		return itr; 
	}
	bf_iterator bf_end(){ 
		bf_iterator itr;
		itr.ptr_ = NULL;
		return itr; 
	}
	
	iterator find(const number_type& x, const number_type& y) { return find(x, y, root_); }
	std::pair< iterator, bool > insert(const Point<number_type> poi, const label_type lbl) { return insert(poi, lbl, root_, NULL); }

private:
	Node<number_type, label_type>* root_;
	unsigned int size_;

	int height(Node<number_type, label_type>* currt) const{
		if(currt == NULL){
			return -1;
		}
		int zero = height(currt->children[0]);
		int one = height(currt->children[1]);
		int two = height(currt->children[2]);
		int three = height(currt->children[3]);
		return 1 + std::max(std::max(zero, one), std::max(two, three));
	}

	Node<number_type, label_type>* copy_tree(Node<number_type, label_type>* old_root) {
		if(old_root == NULL){
			return NULL;
		}
		Node<number_type, label_type>* copy = new Node<number_type, label_type>(old_root->label, old_root->pt.x, old_root->pt.y);
		for(int i = 0; i < 4; i++){
			copy->children[i] = copy_tree(old_root->children[i]);
			if(copy->children[i] != NULL){
				copy->children[i]->parent_ = copy;
			}
		}
		return copy;
	}

	void destroy_tree(Node<number_type, label_type>*& p) { 
		if(p != NULL){
			//destroy all children
			destroy_tree(p->children[0]);
			destroy_tree(p->children[1]);
			destroy_tree(p->children[2]);
			destroy_tree(p->children[3]);
			//destroy self
			delete p;
			p = NULL;
			size_ = 0;
		}
	}

	iterator find(const number_type& x, const number_type& y, Node<number_type, label_type>* p) {
		if (!p) return iterator(NULL);
		if( p->pt.x > x ){
			if( p->pt.y >= y ) {
				//top left quadrant, or on line
				return find(x, y, p->children[0]);
			} else{
				return find(x, y, p->children[2]);
			}
		} else if(p->pt.x < x){
			if( p->pt.y >= y ) {
				return find(x, y, p->children[1]);
			} else{
				return find(x, y, p->children[3]);
			}
		} else{
			if( p->pt.y > y ) {
				return find(x, y, p->children[1]);
			} else if( p->pt.y < y ){
				return find(x, y, p->children[3]);
			} else{
				return iterator(p);
			}
		}
	}

	std::pair<iterator,bool> insert(const Point<number_type> poi, const label_type lbl, Node<number_type, label_type>*& p, Node<number_type, label_type>* parent) {
		if (!p){ 
			p = new Node<number_type, label_type>(lbl, poi.x, poi.y);
			p->parent_ = parent;
			this->size_++;
			return std::pair<iterator, bool>(iterator(p), true);
		}
		if( p->pt.x > poi.x ){
			if( p->pt.y >= poi.y ) {
				//top left quadrant, or on line
				return insert(poi, lbl, p->children[0], p);
			} else{
				return insert(poi, lbl, p->children[2], p);
			}
		} else if(p->pt.x < poi.x){
			if( p->pt.y >= poi.y ) {
				return insert(poi, lbl, p->children[1], p);
			} else{
				return insert(poi, lbl, p->children[3], p);
			}
		} else{
			if( p->pt.y > poi.y ) {
				return insert(poi, lbl, p->children[1], p);
			} else if( p->pt.y < poi.y ){
				return insert(poi, lbl, p->children[3], p);
			} else{
				return std::pair<iterator, bool>(iterator(p), false);
			}
		}
	}

public:
	// ==============================================================
	// PROVIDED CODE : QUAD TREE MEMBER FUNCTIONS FOR PRINTING
	// ==============================================================

	// NOTE: this function only works for quad trees with non negative
	// integer coordinates and char labels

	// NOTE2: this function assumes that no two points have the same x
	// coordinate or the same y coordinate.

	// plot driver function
	// takes in the maximum x and y coordinates for these data points
	// the optional argument draw_lines defaults to true
	void plot(int max_x, int max_y, bool draw_lines=true) const {
		// allocate blank space for the center of the board
		std::vector<std::string> board(max_y+1,std::string(max_x+1,' '));
		// edit the board to add the point labels and draw vertical and
		// horizontal subdivisions
		plot(root_,board,0,max_x,0,max_y,draw_lines);
		// print the top border of the plot
		std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
		for (int i = 0; i <= max_y; i++) {
			// print each row of the board between vertical border bars
			std::cout << "|" << board[i] << "|" << std::endl;
		}
		// print the top border of the plot
		std::cout << "+" << std::string(max_x+1,'-') << "+" << std::endl;
	}

	// actual recursive function for plotting
	void plot(Node<number_type,label_type> *p, std::vector<std::string> &board,
						int x_min, int x_max, int y_min, int y_max, bool draw_lines) const {
		// base case, draw nothing if this node is NULL
		if (p == NULL) return;
		// check that the dimensions range of this node make sense
		assert (x_min >= 0 && x_min <= x_max);
		assert (y_min >= 0 && y_min <= y_max);
		assert (board.size() >= y_max);
		assert (board[0].size() >= x_max);
		// verify that the point stored at this node fits on the board
		assert (p->pt.y >= 0 && p->pt.y < board.size());
		assert (p->pt.x >= 0 && p->pt.x < board[0].size());
		// draw the vertical and horizontal bars extending across the
		// range of this node
		if (draw_lines) {
			for (int x = x_min; x <= x_max; x++) {
				board[p->pt.y][x] = '-';
			}
			for (int y = y_min; y <= y_max; y++) {
				board[y][p->pt.x] = '|';
			}
		}
		// draw this label
		board[p->pt.y][p->pt.x] = p->label;
		// recurse on the 4 children
		plot(p->children[0],board,x_min ,p->pt.x-1,y_min ,p->pt.y-1,draw_lines);
		plot(p->children[1],board,p->pt.x+1,x_max ,y_min ,p->pt.y-1,draw_lines);
		plot(p->children[2],board,x_min ,p->pt.x-1,p->pt.y+1,y_max ,draw_lines);
		plot(p->children[3],board,p->pt.x+1,x_max ,p->pt.y+1,y_max ,draw_lines);
	}


	// ==============================================================

	// prints all of the tree data with a pre-order (node first, then
	// children) traversal of the tree structure

	// driver function
	void print_sideways() const { print_sideways(root_,""); }

	// actual recursive function
	void print_sideways(Node<number_type,label_type>* p, const std::string &indent) const {
		// base case
		if (p == NULL) return;
		// print out this node
		std::cout << indent << p->label << " (" << p->pt.x << "," << p->pt.y << ")" << std::endl;
		// recurse on each of the children trees
		// increasing the indentation
		print_sideways(p->children[0],indent+"  ");
		print_sideways(p->children[1],indent+"  ");
		print_sideways(p->children[2],indent+"  ");
		print_sideways(p->children[3],indent+"  ");
	}

	// ==============================================================
	// ==============================================================

};

#endif
