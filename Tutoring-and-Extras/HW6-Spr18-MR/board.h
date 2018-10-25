#ifndef  board_h_
#define  board_h_
#include <iostream>

class Board {

public:
    //CONSTRUCTORS
    Board();
    Board(const unsigned int & rows_, const unsigned int & columns_, const double & fill_value_);
    Board(const Board & m);

    //ACCESSORS
    unsigned int num_rows() const;
    unsigned int num_cols() const;
    bool get(unsigned int r, unsigned int c, double & at_location) const;
    int getRowCapacity(int row_num) const;

    //MEMBER FUNCTIONS
    bool set(const unsigned int & r, const unsigned int & c, const double & to_be_set);
    void copy(const Board & m);
    void clear();

    //operator overload == and !=
    bool operator==(const Board & m1) const;
    Board& operator=(Board m);
    bool operator!=(const Board & m1) const;

    //DESTRUCTOR
    ~Board();

private:
    
    unsigned int rows;
    unsigned int columns;
    char ** a;
};
//here is where the overloaded output operator goes
std::ostream& operator<< (std::ostream& out, const Board& m);

#endif
