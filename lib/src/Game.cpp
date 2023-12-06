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

// constructor
Game::Game() {
    // sdl set up
    window = SDL_CreateWindow("Sudoku", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("../../resources/ABeeZee-Regular.otf", 25);

    // random seed for generation
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed);

    // no selected cell upon launch
    selectedRow = -1;
    selectedCol = -1;
}

// serializes & saves game state
void Game::saveGame(const std::string &filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "unable to open file for saving" << std::endl;
        return;
    }

    boost::archive::text_oarchive archive(file);
    archive << *this;
    file.close();
}

// loads previously saved game state
void Game::loadGame(const std::string &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "unable to open file for loading" << std::endl;
        return;
    }

    boost::archive::text_iarchive archive(file);
    archive >> *this;
    file.close();
}

// checks if given row contains given number
bool Game::rowContains(int row, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // traverse column indices of given row and check for any equal values
    for (int col = 0; col < GRID_SIZE; col++) {
        if (checkGrid[row][col].getValue() == num) return true;
    }

    return false;   // value was not found
}

// checks if given column contains given number
bool Game::colContains(int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // traverse row indices of given column and check for any equal values
    for (int row = 0; row < GRID_SIZE; row++) {
        if (checkGrid[row][col].getValue() == num) return true;
    }

    return false;   // value was not found
}

// checks if box of the given [row][col] coordinate contains given number
bool Game::boxContains(int row, int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // find the top left of the box that grid[row][col] is in
    int topRow = (row / (int) (sqrt(GRID_SIZE))) * (int) (sqrt(GRID_SIZE));
    int leftCol = (col / (int) (sqrt(GRID_SIZE))) * (int) (sqrt(GRID_SIZE));

    // traverse box and check for any equal values
    for (int i = 0; i < static_cast<int>(sqrt(GRID_SIZE)); i++) {
        for (int j = 0; j < static_cast<int>(sqrt(GRID_SIZE)); j++) {
            if (checkGrid[topRow + i][leftCol + j].getValue() == num) {
                return true;
            }
        }
    }

    return false;   // value was not found
}

// finds an empty cell in the grid
bool Game::findEmpty(int &row, int &col, Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // traverses grid and checks for zero value
    for (row = 0; row < GRID_SIZE; row++) {
        for (col = 0; col < GRID_SIZE; col++) {
            if (checkGrid[row][col].getValue() == 0) return true;
        }
    }

    return false;   // value was not found
}

// fills any empty cells in the grid
bool Game::fillRest(Cell checkGrid[GRID_SIZE][GRID_SIZE]) {
    // uses a backtracking algorithm to fill the rest of the grid
    int row, col;

    if (!findEmpty(row, col, checkGrid)) {
        return true;    // grid is full
    }

    for (int num = 1; num <= GRID_SIZE; num++) {
        if (!rowContains(row, num, checkGrid) && !colContains(col, num, checkGrid) &&
            !boxContains(row, col, num, checkGrid)) {
            checkGrid[row][col].setValue(num);  // test a value
            if (fillRest(checkGrid)) return true;   // check if rest of grid can be validly filled if this cell is
            // given this value
            checkGrid[row][col].setValue(0);    // revert back to zero - tested value did not result in valid grid
        }
    }

    return false;   // grid could not be filled validly
}

// fills a GRID_SIZE x GRID_SIZE box randomly
void Game::fillBox(int row, int col) {
    std::vector<int> nums;

    // add numbers 1 - GRID_SIZE to the vector
    for (int i = 1; i <= GRID_SIZE; i++) {
        nums.push_back(i);
    }

    // randomly shuffle the vector
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(nums.begin(), nums.end(), std::default_random_engine(seed));

    // assign each cell in the box a random value from the vector
    for (int i = 0; i < sqrt(GRID_SIZE); i++) {
        for (int j = 0; j < sqrt(GRID_SIZE); j++) {
            grid[i + row][j + col].setValue(nums.back());
            nums.pop_back();
        }
    }
}

// checks if grid is solvable
bool Game::solvable() {
    // create a copy so the grid is not edited
    Cell copy[GRID_SIZE][GRID_SIZE];

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            copy[i][j].setValue(grid[i][j].getValue());
        }
    }

    return fillRest(copy);  // can the grid be filled with valid numbers?
}

// initializes grid
void Game::initGrid() {
    // init all cell values to zero and editable to false
    for (auto &i: grid) {
        for (auto &j: i) {
            j.setValue(0);
            j.setEditable(false);
        }
    }

    // fill diagonal boxes randomly with numbers if grid is big enough to not potentially cause problems
    if (GRID_SIZE > 4) {
        for (int i = 0; i < GRID_SIZE; i += (int) sqrt(GRID_SIZE)) {
            fillBox(i, i);
        }
    }

    // use a backtracking algorithm to fill the rest of the puzzle
    if (!fillRest(grid)) std::cerr << "could not generate the grid" << std::cout;

    // remove some numbers so game is playable
    int tryRemove = GRID_SIZE * 3;
    while (tryRemove > 0) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;

        if (grid[row][col].getValue() != 0) {
            Cell temp = grid[row][col];
            grid[row][col].setValue(0);
            grid[row][col].setEditable(true);

            // if removing this number makes the puzzle unsolvable, replace it
            if (!solvable()) {
                grid[row][col] = temp;
                grid[row][col].setEditable(false);
            }

            tryRemove--;
        }
    }
}

// updates selected row and column with user click
void Game::selectBox(int x, int y) {
    selectedRow = y / CELL_SIZE;
    selectedCol = x / CELL_SIZE;
}

// updates selected cell with user entered number if the cell is editable
void Game::typeNum(SDL_Keycode key) {
    if (selectedRow != -1 && selectedCol != -1 && key >= SDLK_1 && key <= SDLK_9 &&
        grid[selectedRow][selectedCol].getEditable()) {
        grid[selectedRow][selectedCol].setValue(key - SDLK_0);
    }
}

// draws the grid
void Game::drawGrid() {
    // screen color
    SDL_SetRenderDrawColor(renderer, 237, 246, 249, 225);
    SDL_RenderClear(renderer);

    // grid color
    SDL_SetRenderDrawColor(renderer, 0, 109, 119, 255);

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
                    textColor.r = 131;
                    textColor.g = 197;
                    textColor.b = 190;
                } else {
                    textColor.r = 0;
                    textColor.g = 109;
                    textColor.b = 119;
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

    // draw a box around the selected cell
    if (selectedRow != -1 && selectedCol != -1) {
        SDL_SetRenderDrawColor(renderer, 131, 197, 190, 255);
        SDL_Rect selected = {selectedCol * CELL_SIZE, selectedRow * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        SDL_RenderDrawRect(renderer, &selected);
    }

    SDL_RenderPresent(renderer);
}

// checks if game has been won
bool Game::checkWin() {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            if (grid[row][col].getValue() == 0) return false;   // there is still an empty box, so game not won yet
            int num = grid[row][col].getValue();
            grid[row][col].setValue(0);
            if (rowContains(row, num, grid) || colContains(col, num, grid) || boxContains(row, col, num, grid)) {
                grid[row][col].setValue(num);
                return false;   // a row, column, or box contain two of the same number
            }
            grid[row][col].setValue(num);
        }
    }
    return true;    // checks passed, so game has been won
}

// displays winning game scene and allows user to exit or play again
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

// displays options menu
int Game::showMenu() {
    const SDL_MessageBoxButtonData buttons[] = {
            {0,                                       0, "New Game"},
            {0,                                       1, "Load Game"},
            {0,                                       2, "Save Game"},
            {0,                                       3, "Solver Mode"},
            {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 4, "Exit"}
    };

    const SDL_MessageBoxData data = {
            SDL_MESSAGEBOX_INFORMATION, window, "Menu",
            "Choose an option:", SDL_arraysize(buttons), buttons, nullptr
    };

    int choice;
    SDL_ShowMessageBox(&data, &choice);
    return choice;
}

// launches and runs the game
void Game::run() {
    bool exit = false;
    bool menu = true;
    bool solveMode = false;

    while (!exit) {
        if (menu) {
            int choice = showMenu();
            switch (choice) {
                case 0:
                    initGrid();
                    break;
                case 1:
                    this->loadGame("save.txt");
                    break;
                case 2:
                    saveGame("save.txt");
                    break;
                case 3:
                    solveMode = true;
                    for (auto &i: grid) {
                        for (auto &j: i) {
                            j.setValue(0);
                            j.setEditable(true);
                        }
                    }
                    break;
                case 4:
                    exit = true;
                    continue;
                default:
                    break;
            }

            menu = false;
        }
        SDL_Event e;
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
                if (e.key.keysym.sym == SDLK_m) {
                    menu = !menu;
                } else if (solveMode && e.key.keysym.sym == SDLK_RETURN) {
                    if (!fillRest(grid)) std::cerr << "unsolvable" << std::endl;
                } else {
                    typeNum(e.key.keysym.sym);
                }
            }
        }

        drawGrid();
        if (!solveMode && checkWin()) handleWin();
    }
    SDL_Quit();
}
