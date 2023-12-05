//
// Created by Nitsa Kanungo on 12/1/23.
//

#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class Cell {
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & value;
        ar & editable;
    }

private:
    int value;
    bool editable;

public:
    Cell(); // constructor

    int getValue(); // returns the value of the cell

    void setValue(int num); // sets the value of the cell

    bool getEditable(); // is this cell editable?

    void setEditable(bool value);   // sets the editable field of the cell
};

#endif //SUDOKU_CELL_H
