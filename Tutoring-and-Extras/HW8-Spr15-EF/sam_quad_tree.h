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
class Node {
public:
	Node() : parent_(NULL) {}
	Node(const label_type& lbl, const Point<number_type>& poi) : pt(poi), label(lbl), parent_(NULL) {}
	Point<number_type> pt;
	label_type label;
	Node<number_type, label_type>* children[4];
	Node* parent_;
};

// ==============================================================
// ==============================================================
template <class number_type, class label_type> class QuadTree;

template <class number_type, class label_type>
class DepthIterator{
public:
	DepthIterator() : ptr_(NULL), tree_(NULL) {}
	DepthIterator(Node<number_type, label_type>* p, const QuadTree<number_type, label_type>* t) : ptr_(p), tree_(t) {}
	DepthIterator(const DepthIterator& old) : ptr_(old.ptr_), tree_(old.tree_) {}
	~DepthIterator(){}
	DepthIterator& operator=(const DepthIterator& old) { tree_ = old.tree_; ptr_ = old.ptr_;  return *this; }
	// operator* gives constant access to the value at the pointer
	const Point<number_type>& operator*() const { return ptr_->pt; }
	const label_type& getLabel() const { return ptr_->label; }
	// comparions operators are straightforward
	bool operator== (const DepthIterator& rgt) { return tree_ == rgt.tree_ && ptr_ == rgt.ptr_; }
	bool operator!= (const DepthIterator& rgt) { return tree_ != rgt.tree_ || ptr_ != rgt.ptr_; }
	
	int getDepth(){
		int depth = 0;
		Node<number_type, label_type>* temp = ptr_;
		while(temp->parent_ != NULL){
			depth++;
			temp = temp->parent_;
		}
		return depth;
	}

	DepthIterator<number_type, label_type> & operator++() {  // pre-increment, ++itr
		if (ptr_->top_left != NULL) { // find the leftmost child of the right node
			ptr_ = ptr_->top_left;
		} else if(ptr_->top_right != NULL){ // go upwards along right branches...  stop after the first left
			ptr_ = ptr_->top_right;
		} else if(ptr_->bottom_left != NULL){ // go upwards along right branches...  stop after the first left
			ptr_ = ptr_->bottom_left;
		} else if(ptr_->bottom_right != NULL){ // go upwards along right branches...  stop after the first left
			ptr_ = ptr_->bottom_right;
		} else{
			if(ptr_->parent_->top_left == ptr_){
				if(ptr_->parent_->top_right != NULL){
					ptr_ = ptr_->parent_->top_right;
				} else if(ptr_->parent_->bottom_left != NULL){
					ptr_ = ptr_->parent_->bottom_left;
				} else if(ptr_->parent_->top_right != NULL){
					ptr_ = ptr_->parent_->top_right;
				}
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
	// representation
	Node<number_type, label_type>* ptr_;
	const QuadTree<number_type, label_type>* tree_;
};

// ==============================================================
// ==============================================================

/*template <class number_type, class label_type>
class BreadthIterator{
public:
	BreadthIterator() : ptr_(NULL);
	BreadthIterator(Node<number_type, label_type>* p) : ptr_(p) {}
	BreadthIterator(const BreadthIterator& old) : ptr_(old.ptr_) {}
	~BreadthIterator();
	BreadthIterator& operator=(const BreadthIterator& old) { ptr_ = old.ptr_;  return *this; }
	// operator* gives constant access to the value at the pointer
	const label_type& operator*() const { return ptr_->label; }
	// comparions operators are straightforward
	bool operator== (const BreadthIterator& rgt) { return ptr_ == rgt.ptr_; }
	bool operator!= (const BreadthIterator& rgt) { return ptr_ != rgt.ptr_; }
	// increment & decrement will be discussed in Lecture 17 and Lab 11

private:
	// representation
	Node<number_type, label_type>* ptr_;
};*/

// ==============================================================
// ==============================================================

template <class number_type, class label_type>
class QuadTree {
public:
	QuadTree() : root_(NULL), size_(0) {}
	QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_) { 
		root_ = this->copy_tree(old.root_,NULL);
	}
	QuadTree& operator=(const QuadTree<number_type, label_type>& old) {
		if (&old != this) {
			this->destroy_tree(root_);
			root_ = this->copy_tree(old.root_,NULL);
			size_ = old.size_;
		}
		return *this;
	}
	~QuadTree() {
		this->destroy_tree(root_);
		root_ = NULL;
	}

	typedef DepthIterator<number_type, label_type> iterator;
	//typedef BreadthIterator<number_type, label_type> bf_iterator;
	friend class DepthIterator<number_type, label_type>;
	//friend class BreadthIterator<number_type, label_type>;

	unsigned int size() const { return size_; }
	int height(){ return height(root_); }
	iterator begin(){ return iterator(root_, this); }
	iterator end(){ return iterator(NULL, this); }
	//bf_iterator bf_begin(){ return bf_iterator(root_, this); }
	//bf_iterator bf_end(){ return bf_iterator(NULL, this); }
	
	iterator find(int x, int y) { return find(x, y, root_); }
	std::pair< iterator, bool > insert(const Point<number_type> poi, const label_type lbl) { return insert(poi, lbl, root_, NULL); }

private:
	Node<number_type, label_type>* root_;
	unsigned int size_;

	int height(Node<number_type, label_type>* nd){
		if(nd == NULL){ return -1; }
		int zero = height(nd->children[0]);
		int one = height(nd->children[1]);
		int two = height(nd->children[2]);
		int three = height(nd->children[3]);
		return 1 + std::max(std::max(zero, one), std::max(two, three));
	}

	Node<number_type, label_type>* copy_tree(Node<number_type, label_type>* old_root, Node<number_type, label_type>* the_parent) {
		if (old_root == NULL) { return NULL; }
		Node<number_type, label_type> *answer = new Node<number_type, label_type>();
		answer->pt = old_root->pt;
		answer->label = old_root->label;
		for(int i = 0; i < 4; i++){
			answer->children[i] = copy_tree(old_root->children[i],answer);
		}
		answer->parent = the_parent;
		return answer;
	}

	void destroy_tree(Node<number_type, label_type>* p) {
		if (!p) return;
		for(int i = 0; i < 4; i++){
			destroy_tree(p->children[i]);
		}
		delete p;
	}

	iterator find(int x, int y, Node<number_type, label_type>* p) {
		if (!p) return end();
		if (p->pt.x > x){
			if(p->pt.y > y){
				return find(x, y, p->children[0]);
			} else if(p->pt.y <= y){
				return find(x, y, p->children[2]);
			}
		} else if(p->pt.x < x){
			if(p->pt.y > y){
				return find(x, y, p->children[1]);
			} else if(p->pt.y <= y){
				return find(x, y, p->children[3]);
			}
		} else{
			if(p->pt.y > y){
				return find(x, y, p->children[1]);
			} else if(p->pt.y < y){
				return find(x, y, p->children[3]);
			} else{
				return iterator(p,this);
			}
		}
		//return end();
	}

	std::pair<iterator,bool> insert(const Point<number_type>& poi, const label_type& lbl, Node<number_type,label_type>*& p, Node<number_type,label_type>* the_parent) {
		if (!p) {
			p = new Node<number_type,label_type>(lbl, poi);
			p->parent_ = the_parent;
			this->size_++;
			return std::pair<iterator,bool>(iterator(p,this), true);
		} else if (p->pt.x > poi.x){
			if(p->pt.y > poi.y){
				return insert(poi, lbl, p->children[0], p);
			} else if(p->pt.y <= poi.y){
				return insert(poi, lbl, p->children[2], p);
			}
		} else if(p->pt.x < poi.x){
			if(p->pt.y > poi.y){
				return insert(poi, lbl, p->children[1], p);
			} else if(p->pt.y <= poi.y){
				return insert(poi, lbl, p->children[3], p);
			}
		} else{
			if(p->pt.y > poi.y){
				return insert(poi, lbl, p->children[1], p);
			} else if(p->pt.y < poi.y){
				return insert(poi, lbl, p->children[3], p);
			} else{
				return std::pair<iterator,bool>(iterator(p,this), false);
			}
		}
		//return std::pair<iterator,bool>(end(), false);
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

	// ==============================================================

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

};

// ==============================================================
// ==============================================================


#endif
