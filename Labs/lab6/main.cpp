#include <iostream>
#include <vector>
#include <list>
using namespace std;

void reverse_vector( vector<int>& v );
void print_vec( const vector<int>& v );
void reverse_list( list<int>& lst );
void print_list( const list<int>& lst );

int main(){
	/*
	cout << endl << "Vector 0 (empty): " << endl;
	vector<int> v0;
	print_vec(v0);
	reverse_vector(v0);
	print_vec(v0);

	cout << endl << "Vector 1 (length 1): " << endl;
	vector<int> v1;
	v1.push_back(1);
	print_vec(v1);
	reverse_vector(v1);
	print_vec(v1);

	cout << endl << "Vector 2 (length 2): " << endl;
	vector<int> v2;
	v2.push_back(1);
	v2.push_back(2);
	print_vec(v2);
	reverse_vector(v2);
	print_vec(v2);

	cout << endl << "Vector 3 (odd length): " << endl;
	vector<int> v3;
	for(int i = 1; i <= 7; ++i){
		v3.push_back(i);
	}
	print_vec(v3);
	reverse_vector(v3);
	print_vec(v3);

	cout << endl << "Vector 4 (even length): " << endl;
	vector<int> v4;
	for(int i = 1; i <= 6; ++i){
		v4.push_back(i);
	}
	print_vec(v4);
	reverse_vector(v4);
	print_vec(v4);
	*/
	
	cout << endl << "List 0 (empty): " << endl;
	list<int> l0;
	print_list(l0);
	reverse_list(l0);
	print_list(l0);

	cout << endl << "List 1 (length 1): " << endl;
	list<int> l1;
	l1.push_back(1);
	print_list(l1);
	reverse_list(l1);
	print_list(l1);

	cout << endl << "List 2 (length 2): " << endl;
	list<int> l2;
	l2.push_back(1);
	l2.push_back(2);
	print_list(l2);
	reverse_list(l2);
	print_list(l2);

	cout << endl << "List 3 (odd length): " << endl;
	list<int> l3;
	for(int i = 1; i <= 7; ++i){
		l3.push_back(i);
	}
	print_list(l3);
	reverse_list(l3);
	print_list(l3);

	cout << endl << "List 4 (even length): " << endl;
	list<int> l4;
	for(int i = 1; i <= 6; ++i){
		l4.push_back(i);
	}
	print_list(l4);
	reverse_list(l4);
	print_list(l4);

}

void reverse_list( list<int>& lst ){
	list<int>::iterator itr = lst.begin();
	list<int>::reverse_iterator ritr = lst.rbegin();
	for (int count = 0; count < lst.size()/2; ++count){
		int temp = *itr;
		*itr = *ritr;
		*ritr = temp;
		++itr;
		++ritr;
	}
	//lst.reverse();
}

void print_list( const list<int>& lst ){
	cout << "Size: " << lst.size() << endl;
	cout << "Contents:  ";
	list<int>::const_iterator itr = lst.begin();
	if ( lst.size() != 0 ){
		for( ; itr != lst.end(); ++itr){
			cout << *itr << ", ";
		}
		cout << endl;
	}

}

void reverse_vector( vector<int>& v ){
	for (int i = 0; i < v.size()/2; ++i){
		int temp = v[i];
		v[i] = v[v.size()-1-i];
		v[v.size()-1-i] = temp;
	}
}

void print_vec( const vector<int>& v ){
	cout << "Size: " << v.size() << endl;
	cout << "Contents:  ";
	if( v.size() != 0 ){
		for (int i = 0; i < v.size(); ++i){
			cout << v[i] << ", ";
		}
		cout << endl;
	}
}



