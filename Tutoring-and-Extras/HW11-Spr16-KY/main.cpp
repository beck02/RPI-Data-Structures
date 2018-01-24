#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cmath>
#include "operations.h"

//*****************************************************************************
// There are NO bugs in this file. However, you will probably want to comment
// and uncomment various parts of main as you work your way through this.
//*****************************************************************************
// This file contains the definitions for the following functions:
// - void main(int argc, char* argv[])
// - void corned_beef_hash(int* output, int input)
// - unsigned int hash_browns(unsigned int input)
// - unsigned int decrypt(int array[4], char* bufPtr, int bufLen)
//*****************************************************************************
// Function prototypes for these internal functions:
void corned_beef_hash(int* output, int input);
bool decrypt(int array[4], char* bufPtr, int bufLen);
unsigned int hash_browns(unsigned int input);


int main(int argc, char* argv[]) {

  int records[4];
  records[0] = records[1] = records[2] = records[3] = 0;

  // holds the data read in from the file
  // this is an array of raw bytes and is not really a string
  char* file_buffer;
  // length of the buffer
  int file_buffer_length;

  // arithmetic first
  records[0] = arithmetic_operations();

  // test if all bugs were fixed - passing the correct value to hash_browns
  // will return this number
  if(hash_browns(records[0]) == 771403603)
    std::cout << "Arithmetic bugs are FIXED" << std::endl;

  // good, now let's open and use the data file
  if(! file_operations(argc, argv, file_buffer, file_buffer_length)) {
    // it returned false due to some error
    return 1;
  }

  // all right, let's do things to arrays
  records[1] = array_operations();
  if(hash_browns(records[1]) == 0x8FDEBF5F)
    std::cout << "Array bugs are FIXED" << std::endl;


  // arrays ok, how about vectors?
  records[2] = vector_operations();
  if(hash_browns(records[2]) == 1836042889)
    std::cout << "Vector bugs are FIXED" << std::endl;

  // now let's get down to the real stuff. TRIANGLES.

  // Test for bugs in the Triangle class
  //triangle_bug_tester();

  records[3] = triangle_operations();
  if(hash_browns(records[3]) == 1688783723)
    std::cout << "Triangle bugs are FIXED" << std::endl;

  // (Hopefully) decrypt and print out the file!
  // It should work if you passed all these asserts!
  if(decrypt(records, file_buffer, file_buffer_length)) {
    std::cout << std::endl << "Decryption successful - good job!" << std::endl << std::endl;
    return 0;
  } else {
    std::cout << "Decryption failed or was incomplete" << std::endl;
    return 1;
  }
}

/* Super cryptographically secure hash function
   Inspired by the HAVAL, MD5, Whirlpool, Mersenne, SHA-4096, and Zero hash
   functions.
*/
/// Must pass in an int[4] as the first argument.
/// Obfuscate all variables in this function to one-letter variables.
/// This allows easy creation of multiple versions of the code by changing
/// the random constants.
void corned_beef_hash(int* output, int input) {
  *(output+((input+1962) & 3)) = input ^ 3735928559; ///0xDEADBEEF
  *(output+((input+1492) & 3)) = input ^ 3056505882; ///random
  *(output+((input+'G') & 3)) = input ^ 01353340336; ///0xBADC0DE (octal!)
  *(output+((input+1) & 3)) = input ^ 3405691582; ///0xCAFEBABE
  int bitCtr = 1;
  char bit = 0;
  char* cPtr = (char*) output;
  for(bitCtr = 32767; bitCtr<32783; bitCtr++) {
    for(bit=7; bit >= 0; --bit) {
      if(((*cPtr)>>bit) & 1) (*cPtr) ^= (input >> (bit/2)*8) & 255; else
  (*cPtr) = (((*cPtr)<<5) & 224) | (((*cPtr)>>3) & 31);
      if(*cPtr & 4) (*cPtr = ~(*cPtr));
    }
    ++cPtr;
  }
}

/* Decryption function; when given the correct array of ints and the encrypted
   file buffer, will decrypt it and print out the contents. */
bool decrypt(int array[4], char* bufPtr, int bufLen) {
  std::ofstream decrptfile("decrypted-goal.txt");
  std::cout << "Writing the decrypted file..." << std::endl << std::endl;
  int bufCtr = 0;
  int arrayCtr = 0;
  int hashout[4];
  bool all_printable = true;
  for(; bufCtr < bufLen; bufCtr += 16) {
    corned_beef_hash(hashout, array[arrayCtr]);
    arrayCtr++;
    if(arrayCtr > 3) arrayCtr = 0;
    unsigned char* c = (unsigned char*) &bufPtr[bufCtr];
    unsigned char* d = (unsigned char*) hashout;
    for(int i=0; i<16; ++i, ++c, ++d) {
      if(bufCtr+i >= bufLen) break;
      char dc = *c ^ *d;
      if((dc < 32 || dc > 126) && dc != '\n' && dc != '\t') {
	all_printable = false;
      }
      decrptfile << dc;
    }
  }
  return all_printable;
}

/* Another hash function for verifying the success of the tests.
   Likewise, there are NO bugs here and you should NOT care about it */
unsigned int hash_browns(unsigned int input) {
  union __sys_rcu_tdb_ {
    unsigned int p;
    float q;
  } v;
  v.p = (input * 27828) + 1456694363;
  v.q /= 1024;
  v.p ^= 1672983815;
  v.p += (input * input ^ (input + v.p));
  unsigned int x = (v.p >> 21) & 2047;
  unsigned int y = (v.p << 11) & 037777774000;
  v.q -= 10;
  v.p ^= (x | y);
  return v.p;
}
