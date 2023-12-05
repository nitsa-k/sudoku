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
    Game(); // constructor

    void saveGame(const std::string &filename) const;   // serializes & saves game state

    void loadGame(const std::string &filename); // loads previously saved game state

    static bool rowContains(int row, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);    // checks if given row
                                                                                        // contains given number

    static bool colContains(int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);    // checks if given column
                                                                                        // contains given number

    static bool boxContains(int row, int col, int num, Cell checkGrid[GRID_SIZE][GRID_SIZE]);   // checks if box of
                                                                                                // the given [row][col]
                                                                                                // coordinate contains
                                                                                                // given number

    static bool findEmpty(int &row, int &col, Cell checkGrid[GRID_SIZE][GRID_SIZE]);    // finds an empty cell in the
                                                                                        // grid

    bool fillRest(Cell checkGrid[GRID_SIZE][GRID_SIZE]);    // fills any empty cells in the grid

    void fillBox(int row, int col); // fills a GRID_SIZE x GRID_SIZE box randomly

    bool solvable();    // checks if grid is solvable

    void initGrid();    // initializes grid

    void selectBox(int x, int y);   // updates selected row and column with user click

    void typeNum(SDL_Keycode key);  // updates selected cell with user entered number if the cell is editable

    void drawGrid();    // draws the grid

    bool checkWin();    // checks if game has been won

    void handleWin();   // displays winning game scene and allows user to exit or play again

    int showMenu(); // displays options menu

    void run(); // launches and runs the game
};

#endif //SUDOKU_GAME_H
