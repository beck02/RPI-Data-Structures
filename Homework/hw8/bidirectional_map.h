// Partial implementation of binary-tree based set class similar to std::set.  

#ifndef bidirectional_map_h_
#define bidirectional_map_h_
#include <iostream>
#include <utility>

// =========================================================================================

// KEY NODE CLASS 
template <class K, class V>
class Node {
public:
  Node() : link(NULL), left(NULL), right(NULL), parent(NULL) {}
  Node(const K& init) : data(init), link(NULL), left(NULL), right(NULL), parent(NULL) {}
  K data;
  Node<V, K>* link;
  Node* left;
  Node* right;
  Node* parent; // to allow implementation of iterator increment & decrement
};

template <class K, class V> class bidirectional_map;



// =========================================================================================
// =========================================================================================

// KEY TREE ITERATOR CLASS
template <class K, class V>
class tree_iterator {
public:
  tree_iterator() : ptr_(NULL), root_(NULL), k_bdmap_(NULL), v_bdmap_(NULL) {}
  tree_iterator(Node<K, V>* ky, const bidirectional_map<K, V>* bdm)
                : ptr_(ky), root_(bdm->key_root_), k_bdmap_(bdm), kitr_(true) {}
  tree_iterator(Node<K, V>* vl, const bidirectional_map<V, K>* bdm)
                : ptr_(vl), root_(bdm->value_root_), v_bdmap_(bdm), kitr_(false) {}
  tree_iterator(const tree_iterator& old)
        : ptr_(old.ptr_), root_(old.root_), k_bdmap_(old.k_bdmap_), v_bdmap_(old.v_bdmap_), kitr_(old.kitr_) {}
  ~tree_iterator() {}
  tree_iterator& operator=(const tree_iterator& old) { 
    ptr_ = old.ptr_; 
    root_ = old.root_;
    k_bdmap_ = old.k_bdmap_;
    v_bdmap_ = old.v_bdmap_;
    kitr_ = old.kitr_;
    return *this;
  }
  // operator* gives constant access to the value at the pointer
  const std::pair<K, V>& operator*() { 
    info_ = std::make_pair(ptr_->data, ptr_->link->data);
    return info_;
  }
  // comparions operators are straightforward
  friend bool operator== (const tree_iterator& lft, const tree_iterator& rgt)
  { return (lft.root_ == rgt.root_ && lft.ptr_ == rgt.ptr_); }
  friend bool operator!= (const tree_iterator& lft, const tree_iterator& rgt)
  { return ( (lft.root_ != rgt.root_) || (lft.ptr_ != rgt.ptr_)); }

  // increment & decrement operators
  tree_iterator<K, V> & operator++() { 
    if (ptr_->right != NULL) { // find the leftmost child of the right node
      ptr_ = ptr_->right;
      while (ptr_->left != NULL) { ptr_ = ptr_->left; }
    } else { // go upwards along right branches...  stop after the first left
      while (ptr_->parent != NULL && ptr_->parent->right == ptr_) { ptr_ = ptr_->parent; }
      ptr_ = ptr_->parent;
    }
    return *this;
  }
  tree_iterator<K, V> operator++(int) {
    tree_iterator<K, V> temp(*this);
    ++(*this);
    return temp;
  }
  tree_iterator<K, V> & operator--() { 
    if (ptr_ == NULL) { // so that it works for end()
      if(kitr_){
        assert(k_bdmap_ != NULL);
        ptr_ = k_bdmap_->key_root_;
      } else{
        assert(v_bdmap_ != NULL);
        ptr_ = v_bdmap_->value_root_;
      }
      while (ptr_->right != NULL) { ptr_ = ptr_->right; }
    } else if (ptr_->left != NULL) { // find the rightmost child of the left node
      ptr_ = ptr_->left;
      while (ptr_->right != NULL) { ptr_ = ptr_->right; }
    } else { // go upwards along left brances... stop after the first right
      while (ptr_->parent != NULL && ptr_->parent->left == ptr_) { ptr_ = ptr_->parent; }
      ptr_ = ptr_->parent;
    }
    return *this;
  }
  tree_iterator<K, V> operator--(int) {
    tree_iterator<K, V> temp(*this);
    --(*this);
    return temp;
  }

  tree_iterator<V, K> follow_link(){
    if(kitr_){
      return tree_iterator<V, K>(ptr_->link, k_bdmap_);
    } else{
      return tree_iterator<V, K>(ptr_->link, v_bdmap_);
    }
  }

  friend class bidirectional_map<K, V>;
  friend class bidirectional_map<V, K>;

private:
  // representation
  Node<K, V>* ptr_;
  std::pair<K, V> info_;
  const Node<K, V>* root_;
  const bidirectional_map<K, V>* k_bdmap_;
  const bidirectional_map<V, K>* v_bdmap_;
  bool kitr_;
};




// =========================================================================================
// =========================================================================================
// bidirectional_map CLASS
template <class K, class V>
class bidirectional_map {
public:
  bidirectional_map() : key_root_(NULL), value_root_(NULL), size_(0) {}
  bidirectional_map(const bidirectional_map<K, V>& old) : size_(old.size_) { 
    this->copy_tree(old);
  }
  ~bidirectional_map() {
    this->destroy_tree(key_root_, value_root_);
    key_root_ = NULL; value_root_ = NULL;
  }
  bidirectional_map& operator=(const bidirectional_map<K, V>& old) {
    if (&old != this) {
      this->destroy_tree(key_root_, value_root_);
      this->copy_tree(old);
      size_ = old.size_;
    }
    return *this;
  }

  typedef tree_iterator<K, V> key_iterator;
  typedef tree_iterator<V, K> value_iterator;
  friend class tree_iterator<K, V>;
  friend class tree_iterator<V, K>;

  int size() const { return size_; }
  bool operator==(const bidirectional_map<K, V>& old) const {
    return (old.key_root_ == this->key_root_) && (old.value_root_ == this->value_root_) && (old.size_ == this->size_);
  }

  //-----------------------------------------------------------------------
  // FIND, INSERT & ERASE
  key_iterator find(const K& key) { return find(key, key_root_); }
  value_iterator find(const V& val) { return find(val, value_root_); }
  std::pair< key_iterator, bool > insert( std::pair<K, V> const& in_data ){
    std::pair<Node<K, V>* , bool> key_in = insert(in_data.first, key_root_, NULL);
    std::pair<Node<V, K>* , bool> val_in = insert(in_data.second, value_root_, NULL);
    if(key_in.second){ key_in.first->link = val_in.first; }
    if(val_in.second){ val_in.first->link = key_in.first; }
    return std::make_pair(key_iterator(key_in.first, this), key_in.second);
  }
  int erase(K const& key) { return erase(key, key_root_); }
  //int erase(V const& value) { return erase(value, value_root_); }

  V& operator[]( const K& key ){
    key_iterator kit = find(key, key_root_);
    return kit.ptr_->link->data;
  }
  K& operator[]( const V& val ){
    value_iterator vit = find(val, value_root_);
    return vit.ptr_->link->data;
  }

  //-----------------------------------------------------------------------
  // OUTPUT & PRINTING
  friend std::ostream& operator<< (std::ostream& ostr, const bidirectional_map<K, V>& s) {
    ostr << "KEYS:" << std::endl;
    s.print_in_order(ostr, s.key_root_);
    ostr << "VALUES:" << std::endl;
    s.print_in_order(ostr, s.value_root_);
    return ostr;
  }
  void print(std::ostream& ostr) const {
    int line_length = 50;
    ostr << std::string(line_length, '=') << std::endl << "KEYS:" << std::endl;
    print(ostr, key_root_, 0);
    ostr << std::string(line_length, '-') << std::endl << "VALUES:" << std::endl;
    print(ostr, value_root_, 0);
    ostr << std::string(line_length, '=') << std::endl;
  }

  //-----------------------------------------------------------------------
  // ITERATORS
  key_iterator key_begin() const { 
    if (!key_root_) return key_iterator(NULL, this);
    Node<K, V>* ky = key_root_;
    while (ky->left){ ky = ky->left; }
    return key_iterator(ky, this);
  }
  key_iterator key_end() const { return key_iterator(NULL, this); }

  value_iterator value_begin() const { 
    if (!value_root_) return value_iterator(NULL, this);
    Node<V, K>* vl = value_root_;
    while (vl->left){ vl = vl->left; }
    return value_iterator(vl, this);
  }
  value_iterator value_end() const { return value_iterator(NULL, this); }

  //-----------------------------------------------------------------------

  bool sanity_check() const {
    if (key_root_ == NULL && value_root_ == NULL) return true;
    if (key_root_->parent != NULL || value_root_->parent != NULL) { return false; }
    return sanity_check(key_root_, value_root_);
  }

private:
  //-----------------------------------------------------------------------
  // REPRESENTATION
  Node<K, V>* key_root_;
  Node<V, K>* value_root_;
  int size_;

  // PRIVATE HELPER FUNCTIONS
  //-----------------------------------------------------------------------
  // COPY TREE
  void copy_tree(const bidirectional_map<K, V>& old) {
    this->key_root_ = copy_tree(old.key_root_, NULL);
    this->value_root_ = copy_tree(old.value_root_, NULL);
    key_iterator kitr_old = old.key_begin();
    key_iterator kitr_new = this->key_begin();
    for(; kitr_old != old.key_end(); ++kitr_old, ++kitr_new){
      value_iterator vitr_new = find( kitr_old.ptr_->link->data, value_root_ );
      assert(vitr_new != this->value_end());
      kitr_new.ptr_->link = vitr_new.ptr_;
      vitr_new.ptr_->link = kitr_new.ptr_;
    }
  }
  Node<K, V>* copy_tree(Node<K, V>* old_root, Node<K, V>* the_parent) {
    if (old_root == NULL){ return NULL; }
    Node<K, V>* answer = new Node<K, V>();
    answer->data = old_root->data;
    answer->left = copy_tree(old_root->left,answer);
    answer->right = copy_tree(old_root->right,answer);
    answer->parent = the_parent;
    return answer;
  }
  Node<V, K>* copy_tree(Node<V, K>* old_root, Node<V, K>* the_parent) {
    if (old_root == NULL){ return NULL; }
    Node<V, K>* answer = new Node<V, K>();
    answer->data = old_root->data;
    answer->left = copy_tree(old_root->left,answer);
    answer->right = copy_tree(old_root->right,answer);
    answer->parent = the_parent;
    return answer;
  }

  //-----------------------------------------------------------------------
  // DESTROY TREE
  void destroy_tree(Node<K, V>* ky, Node<V, K>* vl){
    destroy_tree(ky);
    destroy_tree(vl);
  }
  void destroy_tree(Node<K, V>* ky) {
    if (!ky){ return; }
    if(ky){
      destroy_tree(ky->right);
      destroy_tree(ky->left);
      delete ky;
    }
  }
  void destroy_tree(Node<V, K>* vl) {
    if (!vl){ return; }
    if(vl){
      destroy_tree(vl->right);
      destroy_tree(vl->left);
      delete vl;
    }
  }

  //-----------------------------------------------------------------------
  // FIND
  key_iterator find(const K& key, Node<K, V>* ky) {
    if (!ky){ return key_end(); }
    if(ky->data > key){ return find(key, ky->left); }
    else if(ky->data < key){ return find(key, ky->right); }
    else { return key_iterator(ky, this); }
  }

  value_iterator find(const V& val, Node<V, K>* vl) {
    if (!vl) return value_end();
    if(vl->data > val){ return find(val, vl->left); }
    else if(vl->data < val){ return find(val, vl->right); }
    else { return value_iterator(vl, this);}
  }

  //-----------------------------------------------------------------------
  // INSERT
  std::pair<Node<K, V>* , bool> insert(const K& key, Node<K, V>*& ky, Node<K, V>* the_parent){
    if (!ky) {
      ky = new Node<K, V>(key);
      ky->parent = the_parent;
      this->size_++;
      return std::make_pair(ky, true);
    } else if (key < ky->data){ 
      return insert(key, ky->left, ky);
    } else if (key > ky->data){
      return insert(key, ky->right, ky);
    } else{
      return std::make_pair(ky, false);
    }
  }
  std::pair<Node<V, K>* , bool> insert(const V& val, Node<V, K>*& vl, Node<V, K>* the_parent){
    if (!vl) {
      vl = new Node<V, K>(val);
      vl->parent = the_parent;
      return std::make_pair(vl, true);
    } else if (val < vl->data){ 
      return insert(val, vl->left, vl);
    } else if (val > vl->data){
      return insert(val, vl->right, vl);
    } else{
      return std::make_pair(vl, false);
    }
  }

  //-----------------------------------------------------------------------
  // ERASE
  int erase(K const& key, Node<K, V>* &ky) {
    if (!ky){ return 0; }

    // look left & right
    if (ky->data < key){ return erase(key, ky->right); }
    else if (ky->data > key){ return erase(key, ky->left); }

    // Found the node.  Let's delete it
    assert (ky->data == key);
    //delete related value node
    int er_val = erase(ky->link->data, value_root_);
    assert(er_val == 1);
    if (!ky->left && !ky->right) { // leaf
      delete ky; 
      ky = NULL;       
      this->size_--;    
    } else if (!ky->left) { // no left child
      Node<K, V>* old = ky; 
      ky = ky->right; 
      assert (ky->parent == old);
      ky->parent = old->parent;
      delete old;
      this->size_--;    
    } else if (!ky->right) { // no right child
      Node<K, V>* old = ky; 
      ky = ky->left;
      assert (ky->parent == old);
      ky->parent = old->parent;
      delete old;
      this->size_--;
    } else { // Find rightmost node in left subtree
      Node<K, V>* lft_rtmost = ky->left;
      while ( lft_rtmost->right != NULL ){ lft_rtmost = lft_rtmost->right; }
      Node<K, V>* old = ky;
      lft_rtmost->right = old->right;
      lft_rtmost->parent->right = NULL;
      lft_rtmost->parent = old->parent;
      if(old->left != lft_rtmost){ lft_rtmost->left = old->left; }
      ky = lft_rtmost;
      old->link = NULL;
      delete old;
      this->size_--;
    }
    return 1;
  }

  int erase(V const& val, Node<V, K>* &vl) {
    if (!vl){ return 0; }

    // look left & right
    if (vl->data < val){ return erase(val, vl->right); }
    else if (vl->data > val){ return erase(val, vl->left); }

    // Found the node.  Let's delete it
    assert (vl->data == val);
    if (!vl->left && !vl->right) { // leaf
      delete vl; 
      vl = NULL;          
    } else if (!vl->left) { // no left child
      Node<V, K>* old = vl; 
      vl = vl->right; 
      assert (vl->parent == old);
      vl->parent = old->parent;
      delete old;  
    } else if (!vl->right) { // no right child
      Node<V, K>* old = vl; 
      vl = vl->left;
      assert (vl->parent == old);
      vl->parent = old->parent;
      delete old;
    } else { // Find rightmost node in left subtree
      Node<V, K>* lft_rtmost = vl->left;
      while ( lft_rtmost->right != NULL ){ lft_rtmost = lft_rtmost->right; } 
      Node<V, K>* old = vl;
      lft_rtmost->right = old->right;
      lft_rtmost->parent->right = NULL;
      lft_rtmost->parent = old->parent;
      if(old->left != lft_rtmost){ lft_rtmost->left = old->left; }
      vl = lft_rtmost;
      old->link = NULL;
      delete old;
    }
    return 1;
  }

  //-----------------------------------------------------------------------
  // PRINTING
  void print_in_order(std::ostream& ostr, const Node<K, V>* ky) const {
    if (ky) {
      print_in_order(ostr, ky->left);
      ostr << ky->data << " [" << ky->link->data << "]" << "\n";
      print_in_order(ostr, ky->right);
    }
  }
  void print_in_order(std::ostream& ostr, const Node<V, K>* vl) const {
    if (vl) {
      print_in_order(ostr, vl->left);
      ostr << vl->data << " [" << vl->link->data << "]" << "\n";
      print_in_order(ostr, vl->right);
    }
  }

  void print(std::ostream& ostr, const Node<K, V>* ky, int depth) const {
    if (ky) {
      print(ostr, ky->right, depth+1);
      for (int i=0; i<depth; ++i){ ostr << "    "; }
      ostr << ky->data << " [" << ky->link->data << "]" << "\n";
      print(ostr, ky->left, depth+1);
    }
  }
  void print(std::ostream& ostr, const Node<V, K>* vl, int depth) const {
    if (vl) {
      print(ostr, vl->right, depth+1);
      for (int i=0; i<depth; ++i){ ostr << "    "; }
      ostr << vl->data << " [" << vl->link->data << "]" << "\n";
      print(ostr, vl->left, depth+1);
    }
  }

  //-----------------------------------------------------------------------
  // SANITY CHECK
  bool sanity_check(Node<K, V>* ky, Node<K, V>* vl) const {
    if (ky == NULL && vl == NULL) return true;
    if ( (ky->left != NULL && ky->left->parent != ky) || ((vl->left != NULL && vl->left->parent != vl)) ) {
      std::cout << "Error: this node's left child's parent should be this node!" << std::endl;
      return false;
    }
    if ( (ky->right != NULL && ky->right->parent != ky) || ((vl->right != NULL && vl->right->parent != vl)) ) {
      std::cout << "Error: this node's right child's parent should be this node!" << std::endl;
      return false;
    }
    return sanity_check(ky->left, vl) && sanity_check(ky->right, vl) && sanity_check(ky, vl->left) && sanity_check(ky, vl->right);
  }

  //-----------------------------------------------------------------------

};

#endif
