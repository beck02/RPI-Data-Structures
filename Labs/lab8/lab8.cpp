#include <iostream>

using namespace std;

int numPaths(int x, int y);

int main(){
	int p = numPaths(2, 1);
	cout << "Num paths: " << p << endl;
	int p2 = numPaths(2, 2);
	cout << "Num paths: " << p2 << endl;

	return 0;
}


int numPaths(int x, int y){
	int paths = 0;
	if( (x == 1 && y == 0) || (x == 0 && y == 1) ){
		return 1;
	}
	if (x > 0){
		paths += numPaths(x-1, y);
	}
	if (y > 0){
		paths += numPaths(x, y-1);
	}

	return paths;
}