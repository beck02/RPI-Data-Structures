#ifndef UnrolledLL_h_
#define UnrolledLL_h_
// A simplified implementation of a generic list container class,
// including the iterator, but not the const_iterators.  Three
// separate classes are defined: a Node class, an iterator class, and
// the actual list class.  The underlying list is doubly-linked, but
// there is no dummy head node and the list is not circular.

#include <cassert>
#include <iostream>
using namespace std;

const int NUM_ELEMENTS_PER_NODE = 6;
// -----------------------------------------------------------------
// NODE CLASS
template <class T>
class Node {
public:
  Node() : num_elements_(0), next_(NULL), prev_(NULL) {}
  Node(const T& v1) : num_elements_(0), next_(NULL), prev_(NULL) {
    elements_ = new T[NUM_ELEMENTS_PER_NODE];
    elements_[0] = v1;
    num_elements_++;
  }
  Node(const Node<T>& old) : num_elements_(old.num_elements_), next_(NULL), prev_(NULL){
    elements_ = new T[NUM_ELEMENTS_PER_NODE];
    for(int i = 0; i < num_elements_; ++i){
      elements_[i] = old.elements_[i];
    }
  }

  /*Node(const T& v1, const T& v2 = NULL, const T& v3 = NULL, 
            const T& v4 = NULL, const T& v5 = NULL, const T& v6 = NULL)
       : next_(NULL), prev_(NULL), num_elements_(0), NUM_ELEMENTS_PER_NODE(6) {
    
    elements_ = new T[NUM_ELEMENTS_PER_NODE];
    elements_[0] = v1;
    num_elements_++;
    if (v2 != NULL){
      elements_[1] = v2;
      num_elements_++;
    }
    if (v3 != NULL){
      elements_[2] = v3;
      num_elements_++;
    }
    if (v4 != NULL){
      elements_[3] = v4;
      num_elements_++;
    }
    if (v5 != NULL){
      elements_[4] = v5;
      num_elements_++;
    }
    if (v6 != NULL){
      elements_[5] = v6;
      num_elements_++;
    }
  }*/

  bool NodeFull(){ return num_elements_ == NUM_ELEMENTS_PER_NODE; }

  // REPRESENTATION
  unsigned int num_elements_;
  T* elements_;
  Node<T>* next_;
  Node<T>* prev_;
  //const int NUM_ELEMENTS_PER_NODE;
};

// A "forward declaration" of this class is needed
template <class T> class UnrolledLL;

// -----------------------------------------------------------------
// LIST ITERATOR
template <class T>
class list_iterator {
public:
  list_iterator() : ptr_(NULL), offset_(0) {}
  list_iterator(Node<T>* p) : ptr_(p), offset_(0) {}
  list_iterator(Node<T>* p, unsigned int o) : ptr_(p), offset_(o) {}
  list_iterator(list_iterator<T> const& old) : ptr_(old.ptr_), offset_(old.offset_) {}
  ~list_iterator() {}

  unsigned int offset() const { return offset_; }

  void setOffset(unsigned int index){
    if(index < ptr_->num_elements_){
      offset_ = index;
    }
  }

  list_iterator<T> & operator=(const list_iterator<T> & old) { 
    ptr_ = old.ptr_; offset_ = old.offset_;  return *this; }

  // dereferencing operator gives access to the value at the pointer
  T& operator*()  { return ptr_->elements_[offset_];  }

  // increment & decrement operators
  list_iterator<T> & operator++() { // pre-increment, e.g., ++iter
    if( offset_ == (ptr_->num_elements_)-1 ){ 
      nextNode();
    } else{
      offset_++;
    }
    return *this;
  }
  list_iterator<T> operator++(int) { // post-increment, e.g., iter++
    list_iterator<T> temp(*this);
    if( offset_ == (ptr_->num_elements_)-1 ){ 
      nextNode();
    } else{
      offset_++;
    }
    return temp;
  }
  list_iterator<T> & operator--() { // pre-decrement, e.g., --iter
    if( offset_ == 0 ){ 
      prevNode();
    } else{
      offset_--;
    }
    return *this;
  }
  list_iterator<T> operator--(int) { // post-decrement, e.g., iter--
    list_iterator<T> temp(*this);
    if( offset_ == 0 ){ 
      prevNode();
    } else{
      offset_--;
    }
    return temp;
  }

  void nextNode(){
    if( ptr_ != NULL ){
      ptr_ = ptr_->next_;
      offset_ = 0;
    }
  }
  void prevNode(){
    if( ptr_ != NULL ){
      ptr_ = ptr_->prev_;
      offset_ = (ptr_->num_elements_)-1;
    }
  }

  friend class UnrolledLL<T>;

  // Comparions operators are straightforward
  bool operator==(const list_iterator<T>& r) const {
    return ptr_ == r.ptr_; }
  bool operator!=(const list_iterator<T>& r) const {
    return ptr_ != r.ptr_; }

private:
  // REPRESENTATION
  Node<T>* ptr_;    // ptr to node in the list
  unsigned int offset_;

};

// -----------------------------------------------------------------
// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
template <class T>
class UnrolledLL {
public:
  UnrolledLL() : head_(NULL), tail_(NULL), size_(0), nnodes_(0) {}
  UnrolledLL(const UnrolledLL<T>& old) { this->copy_list(old); }
  ~UnrolledLL() { this->destroy_list(); }
  UnrolledLL& operator= (const UnrolledLL<T>& old);

  unsigned int size() const { return size_; }
  unsigned int numNodes() const { return nnodes_; }
  bool empty() const { return head_ == NULL; }
  void clear() { this->destroy_list(); }
  void print(ostream& out) const;
   
  void push_front(const T& v);
  void pop_front();
  void push_back(const T& v);
  void pop_back();

  const T& front() const { return head_->elements_[0];  }
  T& front() { return head_->elements_[0]; }
  const T& back() const { return tail_->elements_[tail_->num_elements_-1]; }
  T& back() { return tail_->elements_[tail_->num_elements_-1]; }

  typedef list_iterator<T> iterator;
  iterator erase(iterator itr);
  iterator erase(iterator itr, unsigned int index);
  iterator insert(iterator itr, T const& v);
  iterator begin() { return iterator(head_, 0); }
  iterator end() { return iterator(NULL); }

private:
  void copy_list(UnrolledLL<T> const & old);
  void destroy_list();

  //REPRESENTATION
  Node<T>* head_;
  Node<T>* tail_;
  unsigned int size_;
  unsigned int nnodes_;
};

// -----------------------------------------------------------------
// LIST CLASS IMPLEMENTATION
template <class T>
UnrolledLL<T>& UnrolledLL<T>::operator= (const UnrolledLL<T>& old) {
  if (&old != this) {
    this->destroy_list();
    this->copy_list(old);
  }
  return *this;
}


template <class T> 
void UnrolledLL<T>::push_back(const T& v) {
  // special case:  initially empty list
  if ( empty() ) {
    head_ = tail_ = new Node<T>( v );
    nnodes_++;
  }
  // normal case:  at least one node already
  else {
    //if end node has room
    if( !(tail_->NodeFull()) ){
      tail_->elements_[tail_->num_elements_] = v;
      tail_->num_elements_++;
    } else{
      Node<T>* newp = new Node<T>(v);
      nnodes_++;
      newp -> prev_ = tail_;
      tail_ -> next_ = newp;
      tail_ = newp;
    }
  }
  ++size_;
}


template <class T> 
void UnrolledLL<T>::push_front(const T& v) {
  // special case:  initially empty list
  if ( empty() ) {
    head_ = tail_ = new Node<T>( v );
    nnodes_++;
  }
  // normal case:  at least one node already
  else {
    //if first node has room
    if( !(head_->NodeFull()) ){
      T* temp = head_->elements_;
      head_->elements_ = new T[NUM_ELEMENTS_PER_NODE];
      head_->elements_[0] = v;
      for(int i = 0; i < head_->num_elements_; ++i){
        head_->elements_[i+1] = temp[i];
      }
      head_->num_elements_++;
      delete [] temp;
    } else{
      Node<T>* newp = new Node<T>(v);
      nnodes_++;
      newp -> next_ = head_;
      head_ -> prev_ = newp;
      head_ = newp;
    }
  }
  ++size_;
}


template <class T> 
void UnrolledLL<T>::pop_back() {
  iterator itr(tail_, tail_->num_elements_-1);
  erase(itr);
}


template <class T> 
void UnrolledLL<T>::pop_front() {
  iterator itr(head_, 0);
  erase(itr);
}

template <class T> 
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr, unsigned int index) {
  iterator result(itr);
  if(index < itr.ptr_->num_elements_){
    itr.setOffset(index);
    result = erase(itr);
  }
  return result;
}

template <class T> 
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr) {
  iterator result(itr);
  ++result;
  //only erase if the list isn't empty and the iterator is pointing at a Node
  if( (!empty()) && (itr.ptr_ != NULL) ){
    //if the Node to be erased from only has one value, just delete the whole Node
    if( itr.ptr_->num_elements_ == 1 ){
      if( (itr.ptr_ == head_) && (head_ == tail_) ){ //if there's only one node left in the list
        head_ = tail_ = NULL;
      } else if( itr.ptr_ == head_ ){ //if it's the first Node, change the list head accordingly
        itr.ptr_->next_->prev_ = NULL;
        head_ = itr.ptr_->next_;
      } else if( itr.ptr_ == tail_ ){ //if it's the last Node, change the list tail accordingly
        itr.ptr_->prev_->next_ = NULL;
        tail_ = itr.ptr_->prev_;
      } else{ //otherwise change the prev and next ptrs of the prev and next Nodes accordingly
        //iterator temp(itr.ptr_->next_);
        itr.ptr_->next_->prev_ = itr.ptr_->prev_;
        itr.ptr_->prev_->next_ = itr.ptr_->next_;
      }
      delete [] itr.ptr_->elements_;
      delete itr.ptr_;
      nnodes_--;
    } else{ //if the Node has more than one value
      T* tempE = new T[NUM_ELEMENTS_PER_NODE];
      for(int i = 0; i < itr.offset_; ++i){
        tempE[i] = itr.ptr_->elements_[i];
      }
      for (int i = itr.offset_ + 1; i < itr.ptr_->num_elements_; ++i){
        tempE[i-1] = itr.ptr_->elements_[i];
      }
      if(itr.offset_ != (itr.ptr_->num_elements_)-1){
        --result;
      }
      itr.ptr_->num_elements_--;
      delete [] itr.ptr_->elements_;
      itr.ptr_->elements_ = tempE;
    }
    --size_;
  }
  return result;
}



template <class T> 
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator itr, T const& v) {
  if( (!empty()) && (itr.ptr_ != NULL) ){
    //if the node to be added to is currently full, create a new one and
    //split the current values between the two nodes
    if ( itr.ptr_->NodeFull() ){
      //if element is to be added at the beginning, check to see if the previous node has room
      if(itr.offset_ == 0){
        //if it does have room, just add the data to the end of that node's array
        if( (itr.ptr_->prev_ != NULL) && !(itr.ptr_->prev_->NodeFull()) ){
          itr.ptr_->prev_->elements_[itr.ptr_->prev_->num_elements_] = v;
          itr.ptr_->prev_->num_elements_++;
        } else{ //otherwise create a new node before the initial one
          Node<T>* newp = new Node<T>(v);
          nnodes_++;
          newp->prev_ = itr.ptr_->prev_;
          newp->next_ = itr.ptr_;
          if(itr.ptr_->prev_ != NULL){ itr.ptr_->prev_->next_ = newp; }
          else{ this->head_ = newp; }
          itr.ptr_->prev_ = newp;
        }
      }
      //if element is to be added at the end, check to see if the next node has room
      else if(itr.offset_ == (NUM_ELEMENTS_PER_NODE-1) ){
        //if it does have room, add the data to the beginning of that node's array
        if( (itr.ptr_->next_ != NULL) && !(itr.ptr_->next_->NodeFull()) ){
          iterator itr2(itr.ptr_->next_, 0);
          this->insert(itr2, v);
        } else{ //otherwise create a new node after the initial one
          Node<T>* newp = new Node<T>(v);
          nnodes_++;
          newp->next_ = itr.ptr_->next_;
          newp->prev_ = itr.ptr_;
          if(itr.ptr_->next_ != NULL){ itr.ptr_->next_->prev_ = newp; }
          else{ this->tail_ = newp; }
          itr.ptr_->next_ = newp;
        }
      } else{ //if the new data is to be added somewhere in the middle of the full current node
        Node<T>* newp = new Node<T>();
        nnodes_++;
        newp->elements_ = new T[NUM_ELEMENTS_PER_NODE];
        //copy all of the data that is after the location at which we want to add a value
        //from the original node's array into the new node's array and delete it from the original array
        int newarr = 0;
        int oldarr = itr.offset_;
        for( ; oldarr < NUM_ELEMENTS_PER_NODE; ++newarr, ++oldarr){
          newp->elements_[newarr] = itr.ptr_->elements_[oldarr];
          newp->num_elements_++;
          itr.ptr_->elements_[oldarr] = NULL;
          itr.ptr_->num_elements_--;
        }
        //add the new data value into the original node
        itr.ptr_->elements_[itr.offset_] = v;
        itr.ptr_->num_elements_++;
        //rearrange the prev and next pointers so the new node comes after the original one in the list
        newp->prev_ = itr.ptr_;
        newp->next_ = itr.ptr_->next_;
        if(itr.ptr_->next_ != NULL){ itr.ptr_->next_->prev_ = newp; }
        else{ this->tail_ = newp; }
        itr.ptr_->next_ = newp;
      }

    } else{ //if the current node isn't full
      for(int i = itr.ptr_->num_elements_; i > itr.offset_; --i){
        itr.ptr_->elements_[i] = itr.ptr_->elements_[i-1];
      }
      itr.ptr_->elements_[itr.offset_] = v;
      itr.ptr_->num_elements_++;

    }
    ++size_;
  }
  return itr;
}

template <class T> 
void UnrolledLL<T>::copy_list(UnrolledLL<T> const & old) {
  size_ = old.size_;
  nnodes_ = old.nnodes_;
  // Handle the special case of an empty list.
  if (size_ == 0) {
    head_ = tail_ = NULL;
    return;
  } else{
    // Create a new head node. 
    head_ = new Node<T>(*old.head_);
    // tail_ will point to the last node created and therefore will move
    // down the new list as it is built
    tail_ = head_;
    // old_p will point to the next node to be copied in the old list
    Node<T>* old_p = old.head_ -> next_;
    // copy the remainder of the old list, one node at a time
    while (old_p) {
      tail_ -> next_ = new Node<T>(*old_p);
      tail_ -> next_ -> prev_ = tail_;
      tail_ = tail_ -> next_;
      old_p = old_p -> next_;
    }
  }
}

template <class T>
void UnrolledLL<T>::destroy_list() {
  iterator itr = begin();
  while( itr != end() ){
    iterator itr2(itr.ptr_->next_);
    delete [] itr.ptr_->elements_;
    delete itr.ptr_;
    itr = itr2;
  }
  head_ = NULL;
  tail_ = NULL;
  size_ = 0;
  nnodes_ = 0;
}


template <class T>
void UnrolledLL<T>::print(ostream& out) const {
  out << "UnrolledLL, size: " << size_ << ", # nodes: " << nnodes_ << endl;
  iterator itr(head_);
  for(int n = 1; n <= nnodes_; ++n){
    out << " node: [" << itr.ptr_->num_elements_ << "] ";
    for(int e = 0; e < itr.ptr_->num_elements_; ++e){
      out << " " << itr.ptr_->elements_[e];
    }
    out << endl;
    itr.nextNode();
  }
}

#endif
