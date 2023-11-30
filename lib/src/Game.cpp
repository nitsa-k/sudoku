//
// Created by Nitsa Kanungo on 11/30/23.
//

#include "../include/Game.h"

Game::Game() {
    window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Game::drawGrid() {
    // make the whole screen white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // set color to black to draw the grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // draw the 9 x 9 sudoku grid
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_SIZE);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_SIZE, i * CELL_SIZE);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    SDL_Event e;

    bool exit = false;
    while (!exit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit = true;
            }
        }

        drawGrid();
    }
}
