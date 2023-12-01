//
// Created by Nitsa Kanungo on 11/30/23.
//

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <SDL.h>
#include <SDL_ttf.h>

#define GRID_SIZE 4
#define SCREEN_SIZE (750 / GRID_SIZE * GRID_SIZE)
#define CELL_SIZE (SCREEN_SIZE / GRID_SIZE)

class Game {
private:
    SDL_Window *window;
    SDL_Renderer *renderer;

public:
    Game();

    void drawGrid();

    void run();
};

#endif //SUDOKU_GAME_H
