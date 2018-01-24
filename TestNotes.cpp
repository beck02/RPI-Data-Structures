template <class key_type, class value_type>
typedef pair<key_type, value_type> data_pair;

//Sets (Trees)
set<key_type>();
iterator begin();
iterator end();
pair<iterator,bool> insert(const key_type& val); // O(log n)
void erase(iterator position); // O(1)
void erase(iterator first, iterator last);
size_type erase(const key_type& val); // O(log n), returns num elemements erased
iterator find(const key_type& val) const; // O(log n)

void print_order(ostream& ostr; const Node<key_type>* p){
	if(p){
		if(in_order){ //in-order: left->self->right
			print_order(ostr, p->left);
			ostr << p->value << "\n";
			print_order(ostr, p->right);
		} else if(post_order){ //post-order: left->right->self
			print_order(ostr, p->left);
			print_order(ostr, p->right);
			ostr << p->value << "\n";
		} else if(pre_order){ //pre-order: self->left->right
			ostr << p->value << "\n";
			print_order(ostr, p->left);
			print_order(ostr, p->right);
		}
	}
}

//							best 	avg		worst
// run time: 		O(n)	O(n)	O(n)
// add. memory:		O(1)	O(n)	O(n)
void breadth_first_print(ostream& ostr, Node<key_type>* p){
	if(p){
		list<Node<key_type>*> curr_lvl;
		curr_lvl.push_back(p);
		while(curr_lvl.size() != 0){
			list<Node<key_type>*> nxt_lvl;
			list<Node<key_type>*>::iterator itr = curr_lvl.begin();
			for(; itr != curr_lvl.end(); ++itr){
				ostr << (*itr)->value << "\n";
				if( (*itr)->left != NULL ){ nxt_lvl.push_back( (*itr)->left ); }
				if( (*itr)->right != NULL ){ nxt_lvl.push_back( (*itr)->right ); }
			}
			curr_lvl = nxt_lvl;
		}
	}
}

//Maps
map<key_type, value_type>();
pair<const key_type, value_type>;
iterator begin();
iterator end();
value_type& operator[] (const key_type& k); // O(log n)
pair<iterator,bool> insert(const data_pair& p); // O(log n)
void erase(iterator position); // O(1)
void erase(iterator first, iterator last);
size_type erase(const key_type& k); // O(log n), returns num elemements erased
iterator find(const key_type& k) const; // O(log n)

//Linked Lists
list<key_type>();
iterator begin();
iterator end();
iterator insert(iterator position, const key_type& val); // O(1)
void push_back(const key_type& val);
void pop_back();
void push_front(const key_type& val);
void pop_front();
iterator erase(iterator position); // O(1)
void clear(); // O(n)

//Vectors
vector<key_type>();
iterator begin();
iterator end();
size_type size() const;
void resize(size_type s); // O(n)
key_type& operator[] (size_type s); // O(1)
void push_back(const key_type& val);
void pop_back();
void clear(); // O(n)
iterator insert(iterator position, const key_type& val); // O(n)
iterator erase(iterator position); // O(n)
void erase(iterator first, iterator last);
