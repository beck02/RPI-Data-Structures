HOMEWORK 8: BIDIRECTIONAL MAPS


NAME:  Sam Saks-Fithian


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

TA Ridwan, TA Sean
http://en.cppreference.com/w/cpp/language/operators
www.cplusplus.com (various)

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  11



ORDER NOTATION ANALYSIS: 
Give the big O notation of each of the library operations and justify
your answer (please be concise!)  You may assume that the tree is
reasonably well balanced for all operations.  (You do not need to
implement re-balancing.)

n = the number of elements

k = maximum number of links stored in a single node 
    (for extra credit non one-to-one associations)


size: O(1) - It just returns the value of a member variable.

insert: O(log n) - Calls the secondary/helper insert functions for each the key tree and the value tree. Both secondary functions are the same but just with flipped typing. These methods search down each tree recursively the same way normal binary trees do, so at worst they would have to go down a path the entire height of the tree, which on average will be log n. Put together, the main insert would have time of 2*log n, but the constant doesn’t matter for order notation, so just O(log n).

erase: O(log n) — Recursively searches down the tree for the node to be erased. Once it finds the node, it triggers the secondary erase for the value tree, which then just recursively searches down that tree for the correct value node. Once the secondary erase finds the right node, it moves around a few pointers before deleting the node. Then back to the primary erase, which just also switches around a few pointers before deleting the node. The only operations with significant running time are the two recursive search sections, each of which at worst will have to traverse the height of the tree (log n) and each of which happen once. Thus, the overall the running time is 2*log n, which comes out to just O(log n).

find: O(log n) — Just searches down the tree going left or right based on the sorting of the key to find, just like a normal binary tree/set/map. At worst, it will have to go all the way down a path that’s the height of the tree, which on average will be log n.

operator[]: O(log n) — Just calls a single find, so same order notation as that.

key_begin: O(log n) — Moves down the left side of the tree to find the smallest element. The while loop is executed once for each element in that path, which is just the height of the tree, which on average will be log n.

key_end: O(1) — Just returns a null iterator

value_begin: O(log n) — Moves down the left side of the tree to find the smallest element. The while loop is executed once for each element in that path, which is just the height of the tree, which on average will be log n.

value_end: O(1) — Just returns a null iterator

key iterator++/--: O(log n) — At worst, it either goes all the way down a left path, or all the way back up a right path, which is just the height of the tree, which on average will be log n.

value iterator++/--: O(log n) — At worst, it either goes all the way down a left path, or all the way back up a right path, which is just the height of the tree, which on average will be log n.

follow_link: O(1) — Just creates and returns a new iterator

default constructor: O(1) — Doesn’t actually do anything, just sets member variables to NULL.

copy constructor: O(n + n*log n) — First, it calls the two secondary copy functions for the key and value trees, each of which recursively copies each element in the respective tree. To copy everything, each must visit every node in that tree, so the running time will be n for each secondary copy function. Then the main copy function iterates through each node from the old tree (so n times), each time calling a find on that value in the new tree (which is running time log n). In total, this method’s running time will be 2n + n*log n, which becomes O(n + n*log n). 

destructor: O(n) — Recursively deletes every element in both the key tree and value tree. To delete everything, it must visit each node, so the running time will be n for each tree, and so 2n for the entire function, which just becomes O(n).

assignment operator: O(n + n*log n) — Just calls the destructor and then the copy constructor, so running time is n + n + n*log n = 2n + n*log n => O(n + n*log n).



MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


