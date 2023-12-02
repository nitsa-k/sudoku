//
// Created by Nitsa Kanungo on 11/30/23.
//

#include "../include/Game.h"

#include <string>
#include <iostream>
#include <fstream>
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

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);

    selectedRow = -1;
    selectedCol = -1;
}

void Game::saveGame(const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for saving." << std::endl;
        return;
    }

    boost::archive::text_oarchive archive(file);
    archive << *this;
    file.close();
}

void Game::loadGame(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file for loading." << std::endl;
        return;
    }

    boost::archive::text_iarchive archive(file);
    archive >> *this;
    file.close();
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
            grid[i + row][j + col].setValue(nums.back());
            nums.pop_back();
        }
    }
}

bool Game::rowContains(int row, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    for (int col = 0; col < GRID_SIZE; col++) {
        if (checkGrid[row][col].getValue() == num) return true;
    }
    return false;
}

bool Game::colContains(int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    for (int row = 0; row < GRID_SIZE; row++) {
        if (checkGrid[row][col].getValue() == num) return true;
    }
    return false;
}

bool Game::boxContains(int row, int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // find the top left of the box that grid[row][col] is in
    int topRow = (row / (int) (sqrt(GRID_SIZE))) * (int) (sqrt(GRID_SIZE));
    int leftCol = (col / (int) (sqrt(GRID_SIZE))) * (int) (sqrt(GRID_SIZE));

    for (int i = 0; i < static_cast<int>(sqrt(GRID_SIZE)); i++) {
        for (int j = 0; j < static_cast<int>(sqrt(GRID_SIZE)); j++) {
            if (checkGrid[topRow + i][leftCol + j].getValue() == num) {
                return true;
            }
        }
    }

    return false;
}


bool Game::findEmpty(int &row, int &col) {
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col].getValue() == 0) return true;
        }
    }
    return false;
}

bool Game::fillRest(Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // uses a backtracking algorithm to fill the rest of the grid
    int row, col;

    if (!findEmpty(row, col)) {
        return true;
    }

    for (int num = 1; num <= GRID_SIZE; num++) {
        if (!rowContains(row, num, checkGrid) && !colContains(col, num, checkGrid) &&
            !boxContains(row, col, num, checkGrid)) {
            checkGrid[row][col].setValue(num);
            if (fillRest(checkGrid)) return true;
            checkGrid[row][col].setValue(0);
        }
    }

    return false;
}


bool Game::solvable() {
    // create a copy so the grid is not edited
    Cell copy[GRID_SIZE][GRID_SIZE];

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            copy[i][j].setValue(grid[i][j].getValue());
        }
    }

    return !fillRest(copy);
}

void Game::initGrid() {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j].setValue(0);
            grid[i][j].setEditable(false);
        }
    }

    // fill diagonal boxes with numbers
    for (int i = 0; i < GRID_SIZE; i += (int) sqrt(GRID_SIZE)) {
        fillBox(i, i);
    }

    // use a backtracking algorithm to fill the rest of the puzzle
    if (!fillRest(grid)) std::cerr << "could not generate the grid" << std::cout;

    // remove some numbers so game is playable
    // TODO: fix the segfault that sometimes gets thrown
    int tryRemove = GRID_SIZE;
    while (tryRemove > 0) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;

        if (grid[row][col].getValue() != 0) {
            Cell temp = grid[row][col];
            grid[row][col].setValue(0);
            grid[row][col].setEditable(true);

            if (!solvable()) {
                grid[row][col] = temp;
                grid[row][col].setEditable(false);
            }

            tryRemove--;
        }
    }
}

void Game::selectBox(int x, int y) {
    selectedRow = y / CELL_SIZE;
    selectedCol = x / CELL_SIZE;
}

void Game::typeNum(SDL_Keycode key) {
    if (selectedRow != -1 && selectedCol != -1 && key >= SDLK_1 && key <= SDLK_9 &&
        grid[selectedRow][selectedCol].getEditable()) {
        grid[selectedRow][selectedCol].setValue(key - SDLK_0);
    }
}

void Game::drawGrid() {
    // make the whole screen white
    SDL_SetRenderDrawColor(renderer, 249, 222, 234, 225);
    SDL_RenderClear(renderer);

    // set color to black to draw the grid
    SDL_SetRenderDrawColor(renderer, 204, 139, 134, 255);

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
            if (grid[row][col].getValue() != 0) {
                std::string val = std::to_string(grid[row][col].getValue());
                SDL_Color textColor;
                if (grid[row][col].getEditable()) {
                    textColor.r = 70;
                    textColor.g = 130;
                    textColor.b = 180;
                } else {
                    textColor.r = 204;
                    textColor.g = 139;
                    textColor.b = 134;
                }
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

    // draw selected cell
    if (selectedRow != -1 && selectedCol != -1) {
        SDL_SetRenderDrawColor(renderer, 70, 130, 180, 255);
        SDL_Rect selected = {selectedCol * CELL_SIZE, selectedRow * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderDrawRect(renderer, &selected);
    }

    SDL_RenderPresent(renderer);
}

bool Game::checkWin() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col].getValue() == 0) return false;
            int num = grid[row][col].getValue();
            grid[row][col].setValue(0);
            if (rowContains(row, num, grid) || colContains(col, num, grid) || boxContains(row, col, num, grid)) {
                grid[row][col].setValue(num);
                return false;
            }
            grid[row][col].setValue(num);
        }
    }
    return true;
}

void Game::handleWin() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Congratulations!", "You won the game!", window);

    const SDL_MessageBoxButtonData buttons[] = {{0,                                       0, "Exit"},
                                                {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Play Again"}};

    const SDL_MessageBoxData data = {SDL_MESSAGEBOX_INFORMATION, window, "Exit or Play Again?",
                                     "Do you want to exit or play again?", SDL_arraysize(buttons), buttons,
                                     nullptr};

    int choice;
    SDL_ShowMessageBox(&data, &choice);

    if (choice == 0) {
        SDL_Quit();
        exit(0);
    } else {
        run();
    }
}

void Game::run() {
    initGrid();

    SDL_Event e;

    bool exit = false;
    while (!exit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                exit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    selectBox(x, y);
                }
            } else if (e.type == SDL_KEYDOWN) {
                typeNum(e.key.keysym.sym);
            }
        }

        drawGrid();
        if (checkWin()) {
            handleWin();
        }
    }

    SDL_Quit();
}