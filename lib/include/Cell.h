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
    Cell();

    int getValue();

    void setValue(int num);

    bool getEditable();

    void setEditable(bool value);
};

#endif //SUDOKU_CELL_H
