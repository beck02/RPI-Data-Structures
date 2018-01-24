#include <iostream>
#include <cmath>
using namespace std;

#include "vec.h"

int main() {
  // ---------------------------------------------------
  // initialize v1 with 10 values...  the multiples of 5
  Vec<int>::size_type i;
  Vec<int> v1( 10, 0 );
  for ( i = 0; i < v1.size(); i++) {
    v1[i] = 5 * i;
  }
  cout << "v1.size() = " << v1.size() << ".  Should be 10.\n";
  cout << "Contents of v1 (multiples of 5):";
  for ( i = 0; i<v1.size(); ++i ) {
    cout << " " << v1[i];
  }
  cout << endl;

  // --------------------------------------------------------------------------
  // make v2 be a copy of v1, but then overwrite the 2nd half with the 1st half
  Vec<int> v2( v1 );
  v2[ 9 ] = v2[ 0 ];
  v2[ 8 ] = v2[ 1 ];
  v2[ 7 ] = v2[ 2 ];
  v2[ 6 ] = v2[ 3 ];
  v2[ 5 ] = v2[ 4 ];
  cout << "Contents of v1 (still multiples of 5):";
  for ( i = 0; i<v1.size(); ++i )
    cout << " " << v1[i];
  cout << endl;
  cout << "Contents of v2 (now palindrome):";
  for ( i = 0; i<v2.size(); ++i )
    cout << " " << v2[i];
  cout << endl;

  // ------------------------------------------
  // make v3 be a copy of v2, but then clear it
  Vec<int> v3;
  v3 = v2;
  v3.clear();
  cout << "\nAfter copying v2 to v3 and clearing v3,  v2.size() = "
       << v2.size() << " and v3.size() = " << v3.size() << endl;
  cout << "Contents of v2 (should be unchanged):";
  for ( i = 0; i<v2.size(); ++i ) {
    cout << " " << v2[i];
  }
  cout << endl;

  // --------------
  // test push back
  cout << "\nNow testing push_back.  Adding 3, 6, 9 to v2:\n";
  v2.push_back( 3 );
  v2.push_back( 6 );
  v2.push_back( 9 );
  cout << "v2 is now: \n";
  for ( i = 0; i<v2.size(); ++i ) {
    cout << " " << v2[i];
  }
  cout << endl;

  // -----------
  // test resize
  v1.resize(20,100);
  cout << "\nNow testing resize.  Resizing v1 to have 20 elements and v2 to have 2 elements\n";
  cout << "v1 is now (should have 100s at the end): \n";
  for ( i = 0; i<v1.size(); ++i )
    cout << " " << v1[i];
  cout << endl;
  v2.resize(2,100);
  cout << "v2 is now: \n";
  for ( i = 0; i<v2.size(); ++i )
    cout << " " << v2[i];
  cout << endl;

  // ------------------------
  // test of a vec of doubles
  cout << "\nStarting from an empty vector, z,  of doubles and doing\n"
       << "5 push_backs\n";
  Vec<double> z;
  for ( i = 0; i<5; ++i )
    z.push_back( sqrt( double(10*(i+1)) ));
  cout << "Contents of vector z: ";
  for ( Vec<double>::size_type j = 0; j < z.size(); j++ )
    cout << " " << z[j];
  cout << endl << endl;

  Vec<char> v4(12, 0);
  char c = 'a';
  for (int i = 0; i < v4.size(); ++i){
    if ( (i == v4.size()/3 - 1) || (i == 2*v4.size()/3 - 1) ){
      v4[i] = 'b';
    } else{
      v4[i] = c;
    }
    c++;
  }
  cout << "v4.size() = " << v4.size() << ".  Should be 12.\n";
  cout << "Contents of v4:";
  for ( i = 0; i < v4.size(); ++i ) {
    cout << " " << v4[i];
  }
  cout << endl;
  remove_matching_elements(v4, 'b');
  cout << "Removed 'b's from v4." << endl;
  cout << "v4.size() = " << v4.size() << ".\n";
  cout << "Contents of v4:";
  for ( i = 0; i < v4.size(); ++i ) {
    cout << " " << v4[i];
  }
  cout << endl << endl;

  Vec<int> v0(6, 0);
  v0[0] = 11; v0[1] = 22;
  v0[2] = 33; v0[3] = 11;
  v0[4] = 55; v0[5] = 22;
  cout << "v0.size() = " << v0.size() << ".  Should be 6.\n";
  cout << "Contents of v0 (11, 22, 33, 11, 55, 22):";
  for ( i = 0; i < v0.size(); ++i ) {
    cout << " " << v0[i];
  }
  cout << endl;
  remove_matching_elements(v0, 11);
  cout << "Removed 11s from v0." << endl;
  cout << "v0.size() = " << v0.size() << ".  Should be 4.\n";
  cout << "Contents of v0 (22, 33, 55, 22):";
  for ( i = 0; i < v0.size(); ++i ) {
    cout << " " << v0[i];
  }
  cout << endl;

  Vec<int> v5;
  for (int i = 0; i < 500; ++i){
    v5.push_back(0);
    v5.print();
  }

  return 0; 
}
