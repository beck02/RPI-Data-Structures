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
  Point() {}
  Point(const number_type& x_, const number_type& y_) : x(x_),y(y_) {}
  // REPRESENTATION
  number_type x;
  number_type y;
};

template <class number_type, class label_type> class QuadTree;
template <class number_type, class label_type> class BreadthIterator;
// -------------------------------------------------------------------
// TREE NODE CLASS 
template <class number_type, class label_type>
class Node {
public:
  Node<number_type, label_type>* children[4];
  Node(){
    for (int i = 0; i < 4; i++){
      children[i] = NULL;
    }
  }
  Node(const label_type& init, Point<number_type> p1) : pt(p1), label(init) {
    for (int i = 0; i < 4; i++){
      children[i] = NULL;
    }
  }
  Point<number_type> pt;
  label_type label;

  Node* parent_;
};

// a helper function to print Points to STL output stream
template <class number_type>
inline std::ostream& operator<<(std::ostream &ostr, const Point<number_type> &pt) {
  ostr << "(" << pt.x << "," << pt.y << ")";
  return ostr;
}
// ==============================================================

// -------------------------------------------------------------------
// DEPTH ITERATOR CLASS
template <class number_type, class label_type>
class DepthIterator {
public:
  DepthIterator() : ptr_(NULL), tree_(NULL) {}
  DepthIterator(Node<number_type, label_type>* p, const QuadTree<number_type, label_type>* t) : ptr_(p), tree_(t) {}
  DepthIterator(const DepthIterator& old) : ptr_(old.ptr_), tree_(old.tree_) {}
  ~DepthIterator() {}
  DepthIterator& operator=(const DepthIterator& old) { ptr_ = old.ptr_; tree_ = old.tree_;  return *this; }
  // operator* gives constant access to the value at the pointer
  const Point<number_type>& operator*() const { return ptr_->pt; }
  //return label
  const label_type getLabel() const{ return ptr_->label; }
  //get the depth of what the iterator is on
  int getDepth(){
    if(ptr_ != NULL){
      int levels = 0;
      Node<number_type, label_type>* temp = ptr_;
      while(temp->parent_ != NULL){
        levels++;
        temp = temp->parent_;
      }
      return levels;
    } else{
      return -1;
    }
  }
  // comparions operators are straightforward
  bool operator== (const DepthIterator& rgt) { return (ptr_ == rgt.ptr_ && tree_ == rgt.tree_); }
  bool operator!= (const DepthIterator& rgt) { return (ptr_ != rgt.ptr_ || tree_ != rgt.tree_); }

  //increment operators
  DepthIterator<number_type, label_type> & operator++() {  // pre-increment, ++itr
    if (ptr_->children[0] != NULL) { // find the top-leftmost child of the node
      ptr_ = ptr_->children[0];
    }else if(ptr_->children[1] != NULL){ // go clockwise along right branches...
      ptr_ = ptr_->children[1];
    }else if(ptr_->children[2] != NULL){
      ptr_ = ptr_->children[2];
    }else if(ptr_->children[3] != NULL){
      ptr_ = ptr_->children[3];
    }else{ //if all children are NULL
      bool foundchild = false;
      while(!foundchild){
        if(ptr_ == tree_->root_){
          //return DepthIterator(NULL, tree_);
          ptr_ = NULL;
          //foundchild = true;
          return *this;
        }
        int currIndex;
        for(currIndex = 0; currIndex < 4; currIndex++){
          if(ptr_ == ptr_->parent_->children[currIndex]){ break; }
        }
        ptr_ = ptr_->parent_;
        for(int i = currIndex + 1; i < 4; i++){
          if(ptr_->children[i] != NULL){
            ptr_ = ptr_->children[i];
            foundchild = true;
            break;
          }
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

template <class number_type, class label_type>
class BreadthIterator{
public:
  BreadthIterator() : tree_(NULL), index_(0) {}
  BreadthIterator(Node<number_type, label_type>* p, const QuadTree<number_type, label_type>* t) : tree_(t), index_(0){
    if(p != NULL){
      int depth = 0;
      Node<number_type, label_type>* temp = p;
      while(temp->parent_ != NULL){ depth++; temp = temp->parent_; }
      make_vectors(depth, temp);
      for(index_ = 0; index_ < current_.size(); index_++){
        if(current_[index_] == p){ break; }
      }
    }
  }

  BreadthIterator(const BreadthIterator& old) : tree_(old.tree_), current_(old.current_), next_(old.next_), index_(old.index_) {}
  ~BreadthIterator() {}
  BreadthIterator& operator=(const BreadthIterator& old) {
    current_ = old.current_;
    next_ = old.next_;
    index_ = old.index_;
    tree_ = old.tree_;
    return *this;
  }
  // operator* gives constant access to the value at the pointer
  const Point<number_type>& operator*() const { return current_[index_]->pt; }
  //return label
  const label_type getLabel() const{ return current_[index_]->label; }
  //get the depth of what the iterator is on
  int getDepth(){
    if(current_.size() != 0 && index_ < current_.size() && current_[index_] != NULL){
      int levels = 0;
      Node<number_type, label_type>* temp = current_[index_];
      while(temp->parent_ != NULL){
        levels++;
        temp = temp->parent_;
      }
      return levels;
    } else{
      return -1;
    }
  }
  // comparions operators are straightforward
  bool operator== (const BreadthIterator& rgt) { return (current_ == rgt.current_ && index_ == rgt.index_ && tree_ == rgt.tree_); }
  bool operator!= (const BreadthIterator& rgt) { return (current_ != rgt.current_ || index_ != rgt.index_ || tree_ != rgt.tree_); }

  //increment operators
  BreadthIterator<number_type, label_type> & operator++() {  // pre-increment, ++itr
    /*if(dumb){
      if(ptr_ == tree_->root_){
        ptr_ = ptr_->children[0];
      } else{
        int depth = getDepth();
        bool foundchild = false;
        while(!foundchild){
          if(ptr_ == tree_->root_){
            if(depth == tree_->height()){ ptr_ = NULL; return *this; }
            
            for(int i = 0; i < 4; i++){
              Node<number_type, label_type>* temp = ptr_;
              if(ptr_->children[i] != NULL){
                ptr_ = ptr_->children[i];
                int j = 0;
                while(getDepth() < depth + 1){
                  
                  for(j = 0; j < 4; j++){
                    if(ptr_->children[j] != NULL){
                      ptr_ = ptr_->children[j];
                      break;
                    }
                  }
                  if(j == 4){
                    ptr_ = temp;
                    break;
                  }
                }


              }
            }
            return *this;
          }
          int currindex_;
          for(currindex_ = 0; currindex_ < 4; currindex_++){
            if(ptr_ == ptr_->parent_->children[currindex_]){ break; }
          }
          ptr_ = ptr_->parent_;
          for(int i = currindex_ + 1; i < 4; i++){
            Node<number_type, label_type>* temp = ptr_;
            if(ptr_->children[i] != NULL){
              ptr_ = ptr_->children[i];
              while(getDepth() < depth){
                if(ptr_->children[0] != NULL){
                  ptr_ = ptr_->children[0];
                } else if(ptr_->children[1] != NULL){
                  ptr_ = ptr_->children[1];
                } else if(ptr_->children[2] != NULL){
                  ptr_ = ptr_->children[2];
                } else if(ptr_->children[3] != NULL){
                  ptr_ = ptr_->children[3];
                } else{
                  ptr_ = temp;
                  break;
                }
              }
              if(getDepth() == depth){ foundchild = true; return *this; }
            }
          }
        }

      }
    }*/

    if(index_ + 1 < current_.size()){
      index_++;
    } else{
      index_ = 0;
      current_ = next_;
      next_.clear();
      for(int i = 0; i < current_.size(); i++){
        Node<number_type, label_type>* p = current_[i];
        for(int j = 0; j < 4; j++){
          if(p->children[j] != NULL){
            next_.push_back(p->children[j]);
          }
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
  // representation
  const QuadTree<number_type, label_type>* tree_;
  std::vector<Node<number_type, label_type>* > current_;
  std::vector<Node<number_type, label_type>* > next_;
  unsigned int index_;

  void make_vectors(int depth_wanted, Node<number_type, label_type>* p){
    if(p != NULL){
      int curr_depth = 0;
      Node<number_type, label_type>* temp = p;
      while(temp->parent_ != NULL){ curr_depth++; temp = temp->parent_; }
      if(curr_depth == depth_wanted){
        current_.push_back(p);
        for(int i = 0; i < 4; i++){
          if(p->children[i] != NULL){
            next_.push_back(p->children[i]);
          }
        }
      } else if(curr_depth < depth_wanted){
        for(int i = 0; i < 4; i++){
          if(p->children[i] != NULL){
            make_vectors(depth_wanted, p->children[i]);
          }
        }
      }
    }
  }

};

// ==============================================================
// ==============================================================

template<class number_type, class label_type>
class QuadTree{

public:
  QuadTree() : root_(NULL), size_(0) {}
  QuadTree(const QuadTree<number_type, label_type>& old) : size_(old.size_) {
    root_ = this->copy_tree(old.root_, NULL);
  }
  QuadTree& operator=(const QuadTree<number_type, label_type>& old){
    if(&old != this){
      this->destroy_tree(root_);
      root_ = this->copy_tree(old.root_,NULL);
      size_ = old.size_;
    }
    return *this;
  }
  ~QuadTree(){
    this->destroy_tree(root_);
    root_ = NULL;
  }

  typedef DepthIterator<number_type, label_type> iterator;
  friend class DepthIterator<number_type, label_type>;
  typedef BreadthIterator<number_type, label_type> bf_iterator;
  friend class BreadthIterator<number_type, label_type>;



  unsigned int size() const {return size_;};
  std::pair<iterator, bool> insert(Point<number_type> p, label_type lab){return insert(p, lab, root_, NULL);}
  iterator find(int x, int y){ return find(x, y, root_);}
  //recursive function to find height of the tree
  int height() const { return height(root_); }
  //returns the root
  iterator begin() {return iterator(root_, this);}
  //returns a null pointer
  iterator end() {return iterator(NULL, this);}
  bf_iterator bf_begin() { return bf_iterator(root_, this); }
  bf_iterator bf_end() { return bf_iterator(NULL, this); }



private:
  Node<number_type, label_type>* root_;
  unsigned int size_;

  // PRIVATE HELPER FUNCTIONS
  Node<number_type, label_type>*  copy_tree(Node<number_type, label_type>* old_root, Node<number_type, label_type>* the_parent) {
    if (old_root == NULL) return NULL;
    Node<number_type, label_type> *answer = new Node<number_type, label_type>();
    answer->label = old_root->label;
    answer->pt = old_root->pt;
    for(int i = 0; i < 4; i++){
      answer->children[i] = copy_tree(old_root->children[i],answer);
    }
    answer->parent_ = the_parent;
    return answer;
  }

  int height(Node<number_type, label_type>* ptr) const{
    if(ptr == NULL) return -1;
    int zero = height(ptr->children[0]);
    int one = height(ptr->children[1]);
    int two = height(ptr->children[2]);
    int three = height(ptr->children[3]);
    return 1 + std::max(std::max(zero,one),std::max(two,three)); 
  }

  void destroy_tree(Node<number_type, label_type>* p) {
    if (!p) return;
    for(int i = 0; i < 4; i++){
      destroy_tree(p->children[i]);
    }
    delete p;
  }

  iterator find(int x_, int y_, Node<number_type, label_type>* p) {
    if (!p) return iterator(NULL, this);
    if(p->pt.x > x_){
      if(p->pt.y > y_){
        return find(x_, y_, p->children[0]);
      }else if(p->pt.y <= y_){
        return find(x_, y_, p->children[2]);
      }
    }else if(p->pt.x < x_){
      if(p->pt.y > y_){
        return find(x_, y_, p->children[1]);
      }else if(p->pt.y <= y_){
        return find(x_, y_, p->children[3]);
      }
    }else{
      if(p->pt.y > y_){
        return find(x_, y_, p->children[1]);
      }else if(p->pt.y < y_){
        return find(x_, y_, p->children[3]);
      }else{
        return iterator(p, this);
      }
    }
    return iterator(NULL, this);
  }

  std::pair<iterator,bool> insert(const Point<number_type>& pnt, const label_type& lab, Node<number_type, label_type>*& p, Node<number_type, label_type>* the_parent ) {
    if (!p) {
      p = new Node<number_type, label_type>(lab, pnt);
      p->parent_ = the_parent;
      this->size_++;
      return std::pair<iterator,bool>(iterator(p,this), true);
    }else if(p->pt.x > pnt.x){
      if(p->pt.y > pnt.y){
        return insert(pnt, lab, p->children[0], p);
      }else if(p->pt.y <= pnt.y){
        return insert(pnt, lab, p->children[2], p);
      }
    }else if(p->pt.x < pnt.x){
      if(p->pt.y > pnt.y){
        return insert(pnt, lab, p->children[1], p);
      }else if(p->pt.y <= pnt.y){
        return insert(pnt, lab, p->children[3], p);
      }
    }else{
      if(p->pt.y > pnt.y){
        return insert(pnt, lab, p->children[1], p);
      }else if(p->pt.y < pnt.y){
        return insert(pnt, lab, p->children[3], p);
      }else{
        return std::make_pair(iterator(p, this), false);
      }
    }
    return std::make_pair(iterator(NULL, this), false);
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

};

// ==============================================================
// ==============================================================


#endif
