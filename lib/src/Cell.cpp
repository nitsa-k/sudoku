//
// Created by Nitsa Kanungo on 12/1/23.
//

#include "../include/Cell.h"

// constructor
Cell::Cell() {
    value = 0;
    editable = false;
}

// returns the value of the cell
int Cell::getValue() {
    return value;
}

// sets the value of the cell
void Cell::setValue(int num) {
    value = num;
}

// is this cell editable?
bool Cell::getEditable() {
    return editable;
}

// sets the editable field of the cell
void Cell::setEditable(bool value) {
    editable = value;
}
