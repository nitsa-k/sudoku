//
// Created by Nitsa Kanungo on 11/30/23.
//

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <SDL.h>
#include <SDL_ttf.h>

#define GRID_SIZE 4     // must be a perfect square - 9 for standard sudoku
#define SCREEN_SIZE (750 / GRID_SIZE * GRID_SIZE)
#define CELL_SIZE (SCREEN_SIZE / GRID_SIZE)

class Game {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    int grid[GRID_SIZE][GRID_SIZE];

public:
    Game();

    void fillBox(int row, int col);

    bool rowContains(int row, int num);

    bool colContains(int col, int num);

    void fillRest();

    void initGrid();

    void drawGrid();

    void run();
};

#endif //SUDOKU_GAME_H
