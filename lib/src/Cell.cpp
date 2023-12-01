//
// Created by Nitsa Kanungo on 12/1/23.
//

#include "../include/Cell.h"

Cell::Cell() {
    value = 0;
    editable = false;
}

int Cell::getValue() {
    return value;
}

void Cell::setValue(int num) {
    value = num;
}

bool Cell::getEditable() {
    return editable;
}

void Cell::setEditable(bool value) {
    editable = value;
}
