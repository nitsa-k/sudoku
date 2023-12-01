//
// Created by Nitsa Kanungo on 11/30/23.
//

#include "../include/Game.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

Game::Game() {
    window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("../../resources/PlayfairDisplaySemibold.ttf", 25);

    initGrid();
}

void Game::fillBox(int row, int col) {
    std::vector<int> nums;

    for (int i = 1; i <= GRID_SIZE; i++) {
        nums.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(nums.begin(), nums.end(), std::default_random_engine(seed));

    for (int i = 0; i < sqrt(GRID_SIZE); i++) {
        for (int j = 0; j < sqrt(GRID_SIZE); j++) {
            grid[i + row][j + col] = nums.back();
            nums.pop_back();
        }
    }
}

bool Game::rowContains(int row, int num) {
    bool value = false;
    for (int col = 0; col < GRID_SIZE; col++) {
        if (grid[row][col] == num) value = true;
    }
    return value;
}

bool Game::colContains(int col, int num) {
    bool value = false;
    for (int row = 0; row < GRID_SIZE; row++) {
        if (grid[row][col] == num) value = true;
    }
    return value;
}

void Game::fillRest() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            for (int num = 1; num <= GRID_SIZE; num++) {
                if (!rowContains(row, num) && !colContains(col, num)) {
                    grid[row][col] = num;
                    break;
                }
            }
        }
    }
}

void Game::initGrid() {
    // for now, just randomly generates a complete (valid) sudoku grid
    // TODO: design a method to remove some numbers such that the game is playable

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = 0;
        }
    }

    // fill diagonal boxes with numbers
    for (int i = 0; i < GRID_SIZE; i += (int) sqrt(GRID_SIZE)) {
        fillBox(i, i);
    }

    fillRest();
}

void Game::drawGrid() {
    // make the whole screen white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // set color to black to draw the grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // draw the sudoku grid
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_SIZE);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_SIZE, i * CELL_SIZE);

        // draw thicker lines to distinguish boxes
        if (i % ((int) sqrt(GRID_SIZE)) == 0) {
            SDL_Rect vertLine = {i * CELL_SIZE, 0, 5, SCREEN_SIZE};
            SDL_RenderFillRect(renderer, &vertLine);

            SDL_Rect horizLine = {0, i * CELL_SIZE, SCREEN_SIZE, 5};
            SDL_RenderFillRect(renderer, &horizLine);
        }
    }

    // draw the numbers
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col] != 0) {
                std::string val = std::to_string(grid[row][col]);
                SDL_Color textColor = {0, 0, 0};
                SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, val.c_str(), textColor);
                SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
                SDL_Rect messageRect = {col * CELL_SIZE + CELL_SIZE / 4, row * CELL_SIZE + CELL_SIZE / 4, CELL_SIZE / 2,
                                        CELL_SIZE / 2};
                SDL_RenderCopy(renderer, message, nullptr, &messageRect);
                SDL_DestroyTexture(message);
                SDL_FreeSurface(surfaceMessage);
            }
        }
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
