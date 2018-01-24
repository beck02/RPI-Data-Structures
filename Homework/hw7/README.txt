HOMEWORK 7: LIBRARY MAPS


NAME:  Sam Saks-Fithian


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

TA Jack

Remember: Your implementation for this assignment must be done on your
own, as described in the “Collaboration Policy & Academic Integrity”
handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  4



ORDER NOTATION ANALYSIS: 
Give the big O notation of each of the library operations and justify
your answer.  Let n be the number of different titles in the library,
m be the maximum number of copies of a given item, p be the number of
patrons using the library, and c be the maximum number of items any
patron has checked out at one time.  (Please be concise!)


add (a):  O(log n)
library uses operator[] once — log n
Everything else is just printing

checkout (c):  O(log p + c + log n)
people uses operator[] - log p
then a for loop to check each book the person has - c
then library uses find - log n
then people uses operator[] again - log p
Total is then: log p + c + log n + log p == 2log p + c + log n
But for order notation constants aren’t relevant so can drop the 2 to become:
O(log p + c + log n)

return (r): O(log p + c*m*log n + m)
people uses operator[] - log p
then a for loop to check each book the person has - c {
* library uses operator[] twice - 2*log n
* a for loop checking each person that has the book checked out - m{
** if statement and list erase (once per loop) - 1
*}
} overall for loop: c * 2*log n * m * 1
then people uses operator[] - log p
and does a vector clear for the books the person has - m
Total is then: log p + c*2*log n*m + log p + m == 2*log p + 2*c*m*log n + m
Can again drop constants to become: O(log p + c*m*log n + m)

lookup (l): O(log n + m)
library uses find - log n
then a for loop checking each person that has the book checked out - m{
* printing - 1
}
Total is then: O(log n + m)

print (p): O(p*c)
a for loop checking each person that has used the library - p{
* a for loop checking each book the person has - c{
** printing - 1
*}
}
Total is then: O(p*c)


YOUR OWN TEST CASES:
describe each test case you created and your motivation for designing
each (why is the test case helpful for debugging and/or interesting)

*see tests_in.txt for actual tests
I added tests to check:
1. An attempt to look up a book before it has been added to the library
-To check handling and to see if the book might get added and be able to be checked out/printed later (possible use of operator[] instead of find)

2. An attempt to add zero copies of a book to the library
-To check handling and to see if the book might get added and be able to be checked out/printed later (possible use of operator[] instead of find)

3. An attempt to add a negative number of copies of a book to the library
-Obvious reasons. We currently should not be able to remove books from the library.

4. An attempt to return books by someone who has not checked anything out (or even been mentioned) previously
-To check handling of unknown patron and make sure no books or copies got readded to the library from the nonexistent person.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


