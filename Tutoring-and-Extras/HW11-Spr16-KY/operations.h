#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

//*****************************************************************************
// This header file declares all the non-Triangle functions in our program
//*****************************************************************************

//*****************************************************************************
// These function definitions are located in operations.cpp
int arithmetic_operations();
bool file_operations(int argc, char** argv, char*& returned_buffer,
		     int& length);
int array_operations();
int vector_operations();
int triangle_operations();

#endif
