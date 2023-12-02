//
// Created by Nitsa Kanungo on 11/30/23.
//

#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Cell.h"

#define GRID_SIZE 9     // must be a perfect square - 9 for standard sudoku
#define SCREEN_SIZE (750 / GRID_SIZE * GRID_SIZE)
#define CELL_SIZE (SCREEN_SIZE / GRID_SIZE)

class Game {
    friend class boost::serialization::access;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned version) {
        ar & grid;
        ar & selectedRow;
        ar & selectedCol;
    }

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;

    Cell grid[GRID_SIZE][GRID_SIZE];

    int selectedRow;
    int selectedCol;

public:
    Game();

    void saveGame(const std::string &filename);

    void loadGame(const std::string &filename);

    void fillBox(int row, int col);

    static bool rowContains(int row, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    static bool colContains(int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    static bool boxContains(int row, int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool findEmpty(int &row, int &col);

    bool fillRest(Cell checkGrid[GRID_SIZE][GRID_SIZE]);

    bool solvable();

    void initGrid();

    void selectBox(int x, int y);

    void typeNum(SDL_Keycode key);

    void drawGrid();

    bool checkWin();

    void handleWin();

    int showMenu();

    void run();
};

#endif //SUDOKU_GAME_H
