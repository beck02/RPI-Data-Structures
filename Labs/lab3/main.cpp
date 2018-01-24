#include <iostream>
using namespace std;

void compute_squares( uintptr_t a[], uintptr_t b[], int n ){
	uintptr_t *p = a;
	uintptr_t *q = b;
	while( p < (a + n) ){
		*q = (*p) * (*p);
		++p;
		++q;
	}
}

int main(){
	int n = 5;
	uintptr_t a[n];
	uintptr_t b[n];
	cout << "Input: " << endl;
	for(int i = 0; i < n; i++){
		a[i] = i + 1;
		cout << a[i] << " , ";
	}
	print_stack();
	compute_squares(a, b, n);
	cout << endl << "Output: " << endl;
	for(int i = 0; i < n; i++){
		cout << b[i] << " , ";
	}
	cout << endl;
	print_stack();
	return 0;
}