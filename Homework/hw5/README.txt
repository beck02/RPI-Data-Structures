HOMEWORK 5: UNROLLED LINKED LISTS


NAME:  Sam Saks-Fithian


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

www.cplusplus.com
www.drmemory.org/docs/

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  8



TESTING & DEBUGGING STRATEGY:
Please be concise!

When I had a ridiculous number of memory errors from BasicTests such that Dr Memory stopped displaying them all, I added return statements at the end of each small/specific test in the method (and then gradually removed them in order as I finished debugging each test) so I could test, debug, and identify the errors just from that specific test.



ORDER NOTATION & EVALUATION:
What is the order notation of each of the member functions in your
class? Discuss any differences between the UnrolledLL, dslist, and STL
list classes. Evaluate the memory usage of your initial implementation
in the worst case. Give a specific example sequence of operations
where the data structure is inefficient. What is the average number of
elements per node in this case? 

n = number of nodes, e = number of elements in current node

push_back: 1
push_front: e
pop_back: e
pop_front: e
erase: e
insert: e
copy_list: ne
destroy_list: n
print: ne

It’s most inefficient when it has to add an element at the very beginning of an almost full node, such that it has to move every element in the node backward before adding the new one.

EXTRA CREDIT:
Improved memory usage implementation.  Discussion as outlined in .pdf.



MISC. COMMENTS TO GRADER:  
Optional, please be concise!


