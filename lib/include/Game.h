//
// Created by Nitsa Kanungo on 11/30/23.
//

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Cell.h"

#include <array>

#define GRID_SIZE 9     // must be a perfect square - 9 for standard sudoku
#define SCREEN_SIZE (750 / GRID_SIZE * GRID_SIZE)
#define CELL_SIZE (SCREEN_SIZE / GRID_SIZE)

class Game {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    Cell grid[GRID_SIZE][GRID_SIZE];

    int selectedRow;
    int selectedCol;

public:
    Game();

    void fillBox(int row, int col);

    bool rowContains(int row, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool colContains(int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool boxContains(int row, int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool findEmpty(int &row, int &col);

    bool fillRest(Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool solvable();

    void initGrid();

    void selectBox(int x, int y);

    void typeNum(SDL_Keycode key);

    void drawGrid();

    void run();
};

#endif //SUDOKU_GAME_H
