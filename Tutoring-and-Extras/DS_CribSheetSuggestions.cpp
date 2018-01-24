//=======================================================================
//=======================================================================
// Container types

//Vectors
vector<value_type>();
iterator begin();
iterator end();
size_type size() const;
bool empty() const;
void resize(size_type s, value_type val = value_type()); // O(n), O(n)
value_type& front();
value_type& back();
value_type& operator[] (size_type s); // O(1)
void push_back(const value_type& val);
void pop_back();
void clear(); // O(n)
iterator insert(iterator position, const value_type& val); // O(n)
iterator erase(iterator position); // O(n)
void erase(iterator first, iterator last);

//=======================================================================
//Linked Lists
list<value_type>();
iterator begin();
iterator end();
size_type size() const;
bool empty() const;
iterator insert(iterator position, const value_type& val); // O(1)
value_type& front();
value_type& back();
void push_back(const value_type& val);
void pop_back();
void push_front(const value_type& val);
void pop_front();
iterator erase(iterator position); // O(1)
void clear(); // O(n)

//=======================================================================

template <class key_type, class value_type>
typedef pair<key_type, value_type> data_pair;

//=======================================================================
//Maps
map<key_type, value_type>();
pair<const key_type, value_type>;
iterator begin();
iterator end();
size_type size() const;
bool empty() const;
value_type& operator[] (const key_type& k); // O(log n)
pair<iterator,bool> insert(const data_pair& p); // O(log n)
void erase(iterator position); // O(1)
void erase(iterator first, iterator last);
size_type erase(const key_type& k); // O(log n), returns num elemements erased
iterator find(const key_type& k) const; // O(log n)

//=======================================================================
//Sets (Trees)
set<key_type>();
iterator begin();
iterator end();
size_type size() const;
bool empty() const;
pair<iterator,bool> insert(const key_type& val); // O(log n)
void erase(iterator position); // O(1)
void erase(iterator first, iterator last);
size_type erase(const key_type& val); // O(log n), returns num elemements erased
iterator find(const key_type& val) const; // O(log n)

//=======================================================================
//Queues
queue<value_type>();
size_type size() const;
bool empty() const;
value_type& front();
value_type& back();
void push(const value_type& val);
void pop();

//=======================================================================
//=======================================================================
// Helpful Functions

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

//=======================================================================

//					best 	avg		worst
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
				Node<key_type>* curr_node = *itr;
				ostr << curr_node->value << "\n";
				if( curr_node->left != NULL ){ nxt_lvl.push_back( curr_node->left ); }
				if( curr_node->right != NULL ){ nxt_lvl.push_back( curr_node->right ); }
			}
			curr_lvl = nxt_lvl;
		}
	}
}

//=======================================================================
//delete array of arrays
void delete_2d_array(){
	//create 2d array
	int array_size = 10;
	int** outer = new int*[array_size];
	for(int i = 0; i < array_size; ++i){
		int* inner = new int[array_size];
		outer[i] = inner;
	}
	//to delete array, must first delete all inner arrays, so there are no leaks
	for (int i = 0; i < array_size; ++i){
		delete [] outer[i];
	}
	delete [] outer;
}

//=======================================================================
//search and do something if not found
bool search(const std::list<int>& lst, int thing_to_search_for){
	bool found;
	std::list::const_iterator itr;
	for (itr = lst.begin(); itr != lst.end(); ++itr){
		if(*itr == thing_to_search_for){
			found = true;
			break;
		}
	}
	if(itr == lst.end()){
		//got to end of loop without breaking, so didn't find thing
		found = false;
	}
	return found;
}

//=======================================================================
//input/output file reading
int main(int argc, char const *argv[]){
	// process command line arguments
	if (argc != 3) {
		cerr << "ERROR: incorrect number of command line arguments" << endl;
		cerr << "USAGE:   " << argv[0] << " <input_file> <output_file>" << endl;
		exit(1);
	}
	// open the input file for parsing
	ifstream infile(argv[1]);
	if (!infile) {
		cerr << "ERROR: could not open input file " << argv[1] << endl;
		exit(1);
	}
	// open the output file
	ofstream outfile(argv[2]);
	if (!outfile) {
		cerr << "ERROR: could not open output file " << argv[2] << endl;
		exit(1);
	}

	//read word by word, words separated by any whitespace
	string word;
	while(infile >> word) {
		//do something
	}

	//reset file back to beginning
	infile.clear();
	infile.seekg(0, infile.beg);
	
	//read line by line, lines separated by newline character
	string line;
	while( getline(infile, line) ){
		//do something
	}

	infile.close();
	outfile.close();
	return EXIT_SUCCESS;
}

//=======================================================================
//automatically created class functions
class Foo{
public:
	Foo(); //default constructor
	~Foo(); //destructor 
	Foo(const Foo &other_foo); //copy constructor
	Foo& operator=(const Foo &other_foo); //assignment operator

	int getValue() const { return value; }
	const string& getName() const { return name; }
	friend Foo operator+(const Foo& a, const Foo& b);
private:
	int value;
	string name;
};

//=======================================================================
//operators

//non-member function
bool operator<(const Foo& a, const Foo& b) { // a < b
	return a.getValue() < b.getValue(); 
}
//friend function
Foo& operator+(const Foo& a, const Foo& b){ // a + b
	a.value += b.value;
	return a;
}
//member function
bool Foo::operator==(const Foo& b){ // a == b
	return value == b.value;
}
//unary member function
void Foo::operator-(){ // -a
	value = -1 * value; 
}
//stream operator
ostream& operator<<(ostream& ostr, const Foo& a){ // cout << a
	ostr << "Foo " << a.getName() << ".value = " << a.getValue();
	return ostr;
}

//=======================================================================
//priority queue percolate
void percolate_up(int i) { // O(log n)
	T value = m_heap[i];
	while (i > 0) {
		int parent = (i-1)/2;
		if (value >= m_heap[parent]) break; // done
		m_heap[i] = m_heap[parent];
		i = parent;
	}
	m_heap[i] = value;
}
void percolate_down(int i) {
	T value = m_heap[i];
	int last_non_leaf = int(m_heap.size()-1)/2;
	while (i <= last_non_leaf) {
		int child = 2*i+1, rchild = 2*i+2;
		if (rchild < m_heap.size() && m_heap[child] > m_heap[rchild]){
			child = rchild;
		}
		if (m_heap[child] >= value) break; // found right location
		m_heap[i] = m_heap[child];
		i = child;
	}
	m_heap[i] = value;
}

//=======================================================================
//=======================================================================










