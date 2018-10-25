/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include <vector>
#include <algorithm>
#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////

//Do not implement the class here, this is a forward declaration. Implement at
//the bottom of the .h file
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only. This is bad practice to have, because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; }

	//put this in the tree class, take in a node or something
	void print() {
		std::cout << "Keys: [";
		for (int i = 0; i < keys.size(); ++i){
			std::cout << keys[i] << ", ";
		}
		std::cout << "]\nNum children = " << children.size() << std::endl;
	}

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>;
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////

//class for the B+ trees themselves
template <class T> class BPlusTree {
public:
	//CONSTRUCTORS
	BPlusTree(); //default constructor
	BPlusTree(const int degree_); //with arguments
	BPlusTree(const BPlusTree<T> & old_tree_); //copy constructor
	//ACCESSORS
	BPlusTreeNode<T>* getRoot() const {return root;}
	BPlusTreeNode<T>* find(const T& value);
	void split(BPlusTreeNode<T>*& node);
	void print_sideways(std::ofstream& outfile);
	void print_BFS(std::ofstream& outfile);
	void print_BFS_pretty(std::ofstream& outfile) {return;}
	//MODIFIERS
	void insert(const T&value);
	//CONDITIONALS
	bool isTreeEmpty() const;
	//DESTRUCTOR
    ~BPlusTree();

private:
	//HELPER FUNCTIONS
	BPlusTreeNode<T>* copy_helper(BPlusTreeNode<T>* old_root);
	void insert_helper(const T& value, BPlusTreeNode<T>*& node);
	BPlusTreeNode<T>* find_helper(const T& value, BPlusTreeNode<T>* key_node);
	void destroy_tree(BPlusTreeNode<T>* old_node);

	//MEMBER VARIABLES
	BPlusTreeNode<T>* root;
	int degree;
};

//default Constructor
template <class T>
BPlusTree<T>::BPlusTree() {
	degree = 0;
	root = NULL;
}

//constructor with argument (the degree)
template <class T>
BPlusTree<T>::BPlusTree(int degree_) {
	degree = degree_;
	root = NULL;
}

//copy constructor
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& old_tree_) {
	this->degree= old_tree_.degree;
	if (old_tree_.isTreeEmpty()) {
		this->root = NULL;
	}
	else{
		this->root = copy_helper(old_tree_.getRoot());
	}
}

template <class T>
BPlusTreeNode<T>* BPlusTree<T>::copy_helper(BPlusTreeNode<T>* old_root) {
	BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
	new_root->keys = old_root->keys;
	new_root->parent = old_root->parent;
	for (int i = 0; i < (old_root->children).size(); i++) {
		(new_root->children)[i] = copy_helper((old_root->children)[i]);
	}
	return new_root;
}

//checks if the tree is empty
template <class T>
bool BPlusTree<T>::isTreeEmpty() const{
	if (this->root == NULL) {
		return true;
	}
	return false;
}


//insert
template <class T>
void BPlusTree<T>::insert(const T&value) {
	if (this->isTreeEmpty()) {
		root = new BPlusTreeNode<T>;
		root->keys.push_back(value);
	}
	else {
		BPlusTreeNode<T>* value_node_location = find(value);
		insert_helper(value, value_node_location);
	}
}

template <class T>
void BPlusTree<T>::insert_helper(const T& value, BPlusTreeNode<T>*& node) {
	(node->keys).push_back(value);
	std::sort((node->keys).begin(), (node->keys).end());
	if ((node->keys).size() >= degree) {
		//here i would do the split;
	}
	else {
		return;
	}

}

//split function, helps the insert function
template <class T>
void BPlusTree<T>::split(BPlusTreeNode<T>*& node) {
	//Base
	if (node->keys.size() < degree){ return; }

	BPlusTreeNode<T>* new_node = new BPlusTreeNode<T>;

	//move second half of keys to new node
	int middle = degree/2;
	for (int i = middle; i < node->keys.size(); ++i){
		new_node->keys.push_back(node->keys[i]);
	}
	node->keys.erase(node->keys.begin() + middle, node->keys.end());

	//move children
	int split_child = 0;
	for (int i = 0; i < node->children.size(); ++i){
		if (node->children[i]->keys[0] == new_node->keys){
			split_child = i;
			break;
		}
	}
	for (int i = split_child; i < node->children.size(); ++i){
		new_node->children.push_back(node->children[i]);
	}
	node->children.erase(node->children.begin() + split_child, node->children.end());

	//doesn't have a parent
	if (!node->parent){
		BPlusTreeNode<T>* new_root = new BPlusTreeNode<T>;
		new_root->keys.push_back(new_node->keys[0]);
		
		if (!node->is_leaf()){
			new_node->keys.erase(new_node->keys.begin());
		}

		new_root->children.push_back(node);
		new_root->children.push_back(new_node);
		node->parent = new_root;
		new_node->parent = new_root;
		root = new_root;
	}
	//there is already a parent 
	else{
		//add split element to parent, sort so in the right spot
		node->parent->keys.push_back(new_node->keys[0]);
		std::sort(node->parent->keys.begin(), node->parent->keys.end());
		
		if (!node->is_leaf()){
			new_node->keys.erase(new_node->keys.begin());
		}
		
		// std::vector<BPlusTreeNode<T>* >::iterator itr;
		// for (itr = node->parent->children.begin(); itr != node->parent->children.end(); ++itr){
		// 	if (*itr == node){
		// 		++itr;
		// 		node->parent->children.insert(itr, new_node);
		// 		new_node->parent = node->parent;
		// 		break;
		// 	}
		// }
		for (int i = 0; i < node->parent->children.size(); ++i){
			if (node->parent->children[i] == node){
				node->parent->children.insert(node->parent->children.begin() + i + 1, new_node);
				new_node->parent = node->parent;
				break;
			}
		}
		split(node->parent);
	}

}

//returns a pointer to a BPlusTreeNode where the value should be
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& value) {
	//returns a NULL pointer if the tree is empty
	if (this->isTreeEmpty()) {
		return NULL;
	}
	else{
		//use the helper to start searching starting at the root
		return find_helper(value, root);
	}
}

template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find_helper(const T& value, BPlusTreeNode<T>* key_node) {
	//base case
	if (key_node->is_leaf()) {
		//this also takes care of the case where the value isn't found
		//because it simply returns the last node visited where the value
		//should be
		return key_node;
	}
	//if key_node is less than any key value
	if (value < key_node->keys[0]) {
		return find_helper(value, key_node->children[0]);
	}
	//if key_node is between first and last key value
	for (int i = 0; i < key_node->keys.size(); i++) {
		if (value < key_node->keys[i] && value >= key_node->keys[i-1]) {
			return find_helper(value, key_node->children[i]);
		}
	}
	//if key_node is greater than any key value
	return find_helper(value, key_node->children[key_node->children.size()-1]);
}

//print sideways
template <class T>
void BPlusTree<T>::print_sideways(std::ofstream& outfile) {
	if (this->root == NULL) {
		outfile << "Tree is empty." << std::endl;
	}
	else{
		outfile << "Not done yet" << std::endl;
	}
}

//print bfs
template <class T>
void BPlusTree<T>::print_BFS(std::ofstream& outfile) {
	if (this->root == NULL) {
		outfile << "Tree is empty." << std::endl;
	}
	else{
		outfile << "Not done yet" << std::endl;
	}
}

//destructor
template <class T>
BPlusTree<T>::~BPlusTree() {
	if (root == NULL) {
		return;
	}
	destroy_tree(root);
}

//destructor helper function
template <class T>
void BPlusTree<T>::destroy_tree(BPlusTreeNode<T>* old_node) {
	if (old_node->is_leaf()) {
		for (int i = 0; i < old_node->keys.size(); i++) {
			old_node->keys.erase(old_node->keys.begin()+i);
		}
		delete old_node;
	}
	else{
		for (int i = 0; i < old_node->children.size(); i++) {
			destroy_tree(old_node->children[i]);
		}
	}
}

#endif
