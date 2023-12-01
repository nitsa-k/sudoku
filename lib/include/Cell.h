//
// Created by Nitsa Kanungo on 12/1/23.
//

#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

class Cell {
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
