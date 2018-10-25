#include "board.h"
#include <iostream>

//default constructor
Board::Board(){
    //making 0x0 Board here
    a = NULL;
    rows = 0;
    columns = 0;
}

//create the constructor for the 2d array (Board) which takes in the rows, columns, and fill value
Board::Board(const unsigned int & rows_, const unsigned int & columns_,const double & fill_value_) {
    rows = rows_;
    columns = columns_;
    a = new double*[rows];
    //the double for loop assigns the fill_value to the specific location
    for (unsigned int i = 0; i < rows; i++) {
        a[i] = new double[columns];
        for (unsigned int j = 0; j < columns; j++) {
            a[i][j] = fill_value_;
        }
    }
}

//this is one of the copy constructors for the test case where a Board was passed in instead of individual parameters
Board::Board(const Board & m) {
    rows = m.rows;
    columns = m.columns;
    a = new double*[rows];
    for (unsigned int i = 0; i < rows; i++) {
        a[i] = new double[columns];
        for (unsigned int j = 0; j < columns; j++) {
            a[i][j] = m.a[i][j];
        }
    }
}

//get the number of rows
unsigned int Board::num_rows() const {
    return rows;
}

//get the numebr of columns
unsigned int Board::num_cols() const {
    return columns;
}

//get the value at location row x column
bool Board::get(unsigned int r,unsigned int c, double & at_location)const{
    //checks if given locations are even within bounds
    if (r <= rows && c <= columns) {
        //then retrieves value at that location
        at_location = a[r][c];
        return true;
    }
    return false;
}

//set a value at location row x column
bool Board::set(const unsigned int & r, const unsigned int & c, const double & to_be_set){
    //checks to see if locations are within bounds
    if (r <= rows && c <= columns) {
        //sets the value at that location to "to_be_set"
        a[r][c] = to_be_set;
        return true;
    }
    return false;
}

//copies the elements from one Board to a new Board (from lecture 8 )
void Board::copy(const Board & m) {
    //this-> allows my current Board to be set to the data from the Board that was passed in
    this->rows = m.rows;
    this->columns = m.columns;
    //here i assign the current Board to a new Board and i fully create the new Board in the for loops below
    this->a = new double*[rows];
    //accessing the specific locations in my current Board
    for (unsigned int i = 0; i < m.num_rows(); i++) {
        a[i] = new double[m.num_cols()];
        for (unsigned int j = 0; j < m.num_cols(); j++) {
            //assining values of my current Board to the value that is in the location in the Board that was passed in
            this -> a[i][j] = m.a[i][j];
        }
    }
}

//returns true if both matrices are equal using operator overloading (from website on README)
bool Board::operator==(const Board & m1) const {
    double k;
    //if within bounds...
    if ((this->rows==m1.num_rows())&&(this->columns==m1.num_cols())) {
        //then start get to the specific location in Board m and...
        for (unsigned int i = 0; i < m1.num_rows(); i++) {
            for (unsigned int j = 0; j < m1.num_cols(); j++) {
                //get the value at location specified
                m1.get(i,j,k);
                //but if at least one elemtn in the Board is not equal then just return false
                if (this->a[i][j] != k) {return false;}
            }
        }
        return true;
    }
    else{return false;}
}

//this is an assignment operator. assigns one Board to another
//this is similar to copy() which is why we use copy()
Board& Board::operator=(Board m){
    this->clear();
    this->copy(m);
    //returns my current pointer with new data
    return *this;
}

//returns true if even one element in matrices are different, or if columns or rows are different
bool Board::operator!=(const Board & m1) const{
    return !(*this == m1);
}

//clears everything in the Board and called Board() functions
void Board::clear() {
    //delete the date in every location within my Board
    for (unsigned int i = 0; i < rows; i++) {
        delete [] a[i];
    }
    //takes care of memory leaks
    delete [] a;
    //creates an empty Board
    Board();
}

//overloaded output operator- takes care of the Printing
//this was in the homework handout pretty much
std::ostream& operator<< (std::ostream& out, const Board& m) {
    double k;
    out<< "\n"<<m.num_rows()<<" x "<<m.num_cols() << " Board:";
    out << "\n[ ";
    for (unsigned int i = 0; i < m.num_rows(); i++) {
        for (unsigned int j = 0; j < m.num_cols(); j++) {
            m.get(i,j,k);
            out<< k<<" ";
        }
        //this only runs if we are at the last element in the Board
        if (i == m.num_rows()-1) {
            out<< "]" <<std::endl;
        }
        out <<std::endl;
    }
    //only runs if the Board is empty
    if (m.num_rows() == 0 && m.num_cols() == 0) {
        out << "]"<<std::endl;
    }
    return out;
}

//multiplies every element in my Board by the number I passed in
void Board::multiply_by_coefficient(const double & coeff) {
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            this -> a[i][j] = (a[i][j])*coeff;
        }
    }
}

//swaps two rows whose row numbers are passed in, and returns true if it was able to
//swap them. otherwise, it returns false.
bool Board::swap_row(const unsigned int & source_row, const unsigned int & target_row) {
    //if within bounds (because we don't want this code to run unless we are even inside the Board)
    if ((source_row <= rows) && (target_row <= rows)) {
        //make a new pointer to store the information temporarily
        double *b = new double[columns];
        for (unsigned int i = 0; i < rows; i++) {
            if (i == source_row) {
                //this is where i move everything from the original rows to the temporary
                //rows and then back to the main rows
                for (unsigned int j = 0; j < columns; j++) {
                    b[j] = a[i][j];
                    a[i][j] = a[target_row][j];
                    a[target_row][j] = b[j];
                }
            }
        }
        //deleting all the extra memory that will be unused
        delete [] b;
        return true;
    }
    return false;
}

//takes a mxn Board and returns an nxm Board.
//basically switches rows and columns and the elements inside it
void Board::transpose() {
    //making a new Board
    Board* temp = new Board(columns,rows,0);
    //assining the values to this temporary Board
    for (unsigned int i = 0; i < columns; i++) {
        for (unsigned int j = 0;j < rows; j++) {
            temp->set(i,j,a[j][i]);
        }
    }
    //change my current pointer to point to this new Board
    this->operator=(*temp);
    //then delete the extra memory to prevent memory leaks
    delete temp;
}

//adds the elements at the specified locations of both my current Board and Board that is passed in
bool Board::add(const Board & m) {
    //but this code only happens if the two matrices are the same dimensions
    if ((rows == m.num_rows()) && (columns == m.num_cols())) {
        for (unsigned int i = 0; i < rows; i++) {
            for (unsigned int j = 0; j < columns; j++) {
                a[i][j] += m.a[i][j];
            }
        }
        return true;
    }
    return false;
}

//subtracts the elements from my current Board and the Board that is passed in
bool Board::subtract(const Board & m) {
    //but again, only works if the dimensions of both matrices are the same
    if ((rows == m.num_rows()) && (columns == m.num_cols())) {
        for (unsigned int i = 0; i < rows; i++) {
            for (unsigned int j = 0; j < columns; j++) {
                a[i][j] -= m.a[i][j];
            }
        }
        return true;
    }
    return false;
}

//takes in the row number and returns where it points to
double * Board::get_row(unsigned int number){
    if (number > rows) {
        return NULL;
    }
    double * r = new double[columns];
    for (unsigned int i = 0; i < columns; i++) {
        r[i] = a[number][i];
    }
    return r;
}

//takes in the column number and returns where it points to
double * Board::get_col(unsigned int number){
    if (number > columns) {
        return NULL;
    }
    double * r = new double[rows];
    for (unsigned int i = 0; i < rows; i++) {
        r[i] = a[i][number];
    }
    return r;
}

//this returns the upper left corner of the Board, the upper right corner of the Board,
//the lower left corner of the Board, and the lower right corner of the Board
Board * Board::quarter() const {
    //makes a new pointer that points to an array of matrices
    Board * q = new Board[4];
    //checks if both dimensions of the Board are even
    if ((rows%2 == 0) && (columns%2 == 0)) {

        //QUADRANT 1
        Board q1= Board(rows/2,columns/2,0);
        for (unsigned int i = 0; i < rows/2; i++) {
            for (unsigned int j = 0; j < columns/2; j++) {
                q1.set(i,j,a[i][j]);
            }
        }
        //QUADRANT 2
        Board q2= Board(rows/2,columns/2,0);
        for (unsigned int i = 0; i < rows/2; i++) {
            for (unsigned int j = columns/2; j < columns; j++) {
                q2.set(i,j-(columns/2),a[i][j]);
            }
        }
        //QUADRANT 3
        Board q3=Board(rows/2,columns/2,0);
        for (unsigned int i = rows/2; i < rows; i++) {
            for (unsigned int j = 0; j < columns/2; j++) {
                q3.set(i-(rows/2),j,a[i][j]);
            }
        }
        //QUADRANT 4
        Board q4=Board(rows/2,columns/2,0);
        for (unsigned int i = rows/2; i < rows; i++) {
            for (unsigned int j = columns/2; j < columns; j++) {
                q4.set(i-(rows/2),j-(columns/2),a[i][j]);
            }
        }
        //assigns the newly created Board to the array location in the pointer q
        q[0] = q1;
        q[1] = q2;
        q[2] = q3;
        q[3] = q4;
        return q;
    }
    //checks if rows is even and columns is odd
    else if ((rows%2 == 0) && (columns%2!= 0)) {
        //QUADRANT 1
        Board q1=Board(rows/2,((columns/2)+1),0);
        for (unsigned int i = 0; i < rows/2; i++) {
            for (unsigned int j = 0; j < ((columns/2)+1); j++) {
                q1.set(i,j,a[i][j]);
            }
        }
        //QUADRANT 2
        Board q2=Board(rows/2,(columns/2+1),0);
        for (unsigned int i = 0; i < rows/2; i++) {
            for (unsigned int j = ((columns/2)-1); j < columns; j++) {
                q2.set(i,j-(columns/2),a[i][j]);
            }
        }
        //QUADRANT 3
        Board q3=Board(rows/2,((columns/2)+1),0);
        for (unsigned int i = rows/2; i < rows; i++) {
            for (unsigned int j = 0; j < ((columns/2)+1); j++) {
                q3.set(i-(rows/2),j,a[i][j]);
            }
        }
        //QUADRANT 4
        Board q4=Board(rows/2,((columns/2)+1),0);
        for (unsigned int i = rows/2; i < rows; i++) {
            for (unsigned int j = ((columns/2)-1); j < columns; j++) {
                q4.set(i-(rows/2),j-(columns/2),a[i][j]);
            }
        }
        //assigns the newly created Board to the array location in the pointer q
        q[0] = q1;
        q[1] = q2;
        q[2] = q3;
        q[3] = q4;
        return q;
    }
    //checks if rows is odd and columns is even
    else if ((rows%2 != 0) && (columns%2== 0)) {
        //QUADRANT 1
        Board q1=Board(((rows/2)+1),(columns/2),0);
        for (unsigned int i = 0; i < ((rows/2)+1); i++) {
            for (unsigned int j = 0; j < columns/2; j++) {
                q1.set(i,j,this->a[i][j]);
            }
        }
        //QUADRANT 2
        Board q2=Board(((rows/2)+1),columns/2,0);
        for (unsigned int i = 0; i < ((rows/2)+1); i++) {
            for (unsigned int j = columns/2; j < columns; j++) {
                q2.set(i,j-(columns/2),a[i][j]);
            }
        }
        //QUADRANT 3
        Board q3=Board(((rows/2)+1),columns/2,0);
        for (unsigned int i = ((rows/2)-1); i < rows; i++) {
            for (unsigned int j = 0; j < columns/2; j++) {
                q3.set(i-(rows/2),j,a[i][j]);
            }
        }
        //QUADRANT 4
        Board q4=Board(((rows/2)+1),columns/2,0);
        for (unsigned int i = ((rows/2)-1); i < rows; i++) {
            for (unsigned int j = columns/2; j < columns; j++) {
                q4.set(i-(rows/2),j-(columns/2),a[i][j]);
            }
        }
        //assigns the newly created Board to the array location in the pointer q
        q[0] = q1;
        q[1] = q2;
        q[2] = q3;
        q[3] = q4;
        return q;
    }
    //checks if both dimensions are odd
    else{
        //QUADRANT 1
        Board q1=Board(((rows/2)+1),((columns/2)+1),0);
        for (unsigned int i = 0; i < (rows/2)+1; i++) {
            for (unsigned int j = 0; j < ((columns/2)+1); j++) {
                q1.set(i,j,a[i][j]);
            }
        }
        //QUADRANT 2
        Board q2=Board(((rows/2)+1),((columns/2)+1),0);
        for (unsigned int i = 0; i < (rows/2)+1; i++) {
            for (unsigned int j = ((columns/2)-1); j < columns; j++) {
                q2.set(i,j-(columns/2),a[i][j]);
            }
        }
        //QUADRANT 3
        Board q3=Board(((rows/2)+1),((columns/2)+1),0);
        for (unsigned int i = (rows/2)-1; i < rows; i++) {
            for (unsigned int j = 0; j < ((columns/2)+1); j++) {
                q3.set(i-(rows/2),j,a[i][j]);
            }
        }
        //QUADRANT 1
        Board q4=Board(((rows/2)+1),((columns/2)+1),0);
        for (unsigned int i = (rows/2)-1; i < rows; i++) {
            for (unsigned int j = ((columns/2)-1); j < columns; j++) {
                q4.set(i-(rows/2),j-(columns/2),a[i][j]);
            }
        }
        //assigns the newly created Board to the array location in the pointer q
        q[0] = q1;
        q[1] = q2;
        q[2] = q3;
        q[3] = q4;
        return q;
    }
}

//this is my deconstructor. takes care of deleting everything so i dont have memory leaks
Board::~Board(){
    for (unsigned int i = 0; i < rows; i++) {
        delete [] a[i];
    }
    delete [] a;
}
