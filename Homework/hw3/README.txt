HOMEWORK 3: JAGGED ARRAY


NAME:  Sam Saks-Fithian


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

www.cplusplus.com
www.stackoverflow.com
www.drmemory.org/docs/index.html

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  10



ORDER NOTATION:
For each function, for each version (packed vs. unpacked).  
b = the number of bins
e = the number of elements
k = the number of elements

numElements
 unpacked: 1
 packed: 1

numBins
 unpacked: 1
 packed: 1

numElementsInBin
 unpacked: 1
 packed: 1

getElement
 unpacked: 1
 packed: 1

isPacked
 unpacked: 1
 packed: 1

clear
 unpacked: b 
 packed: b

addElement
 unpacked: k
 packed: 1 (or potentially 0, since it’ll immediately have an error and exit?)

removeElement
 unpacked: k
 packed: 1 (or potentially 0, since it’ll immediately have an error and exit?)

pack
 unpacked: be
 packed: 0

unpack
 unpacked: 0
 packed: be

print 
 unpacked: kb
 packed: b + e

constructor w/ integer argument
 unpacked: b
 packed: 0 (always constructs as unpacked)

copy constructor
 unpacked: be (of the JaggedArray being copied)
 packed: b + e (of the JaggedArray being copied)

destructor
 unpacked: be
 packed: b

assignment operator
 unpacked: be
 packed: b + e




MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


