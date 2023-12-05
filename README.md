# Sudoku

*developed by **Nitsa Kanungo***

## What is this?

This is a C++ program for all things sudoku! It can generate random playable sudoku puzzles of any (valid) size as well
as solve user-inputted sudoku puzzles.

## How do I play?

Running the program should prompt you with a menu, where you can decide what you'd like to do.

- **New Game:** This will generate a new random puzzle. You can click on boxes and type numbers using your keyboard.
  When you accurately solve the puzzle, another menu will pop up prompting you to play again with a new puzzle or
  exit the program.
- **Load Game:** If you have previously saved a game, this will restore the saved game state, and you can continue
  playing from right where you left off!
- **Save Game:** *(intended to be used after loading a game or starting a new game)* This will save the current game
  state of the board, which you can access again with **Load Game**. There is no auto save, so if you continue
  playing a loaded game state, you must hit **Save Game** again for the changes you make to be saved.
- **Solver Mode:** This will take you to a blank sudoku grid. Click and type a partially solved sudoku puzzle. When
  you type `enter`/`return`, the program will fill in the rest of the puzzle with a valid solution, if one exists.
- **Exit:** This will terminate the program.

You can access this menu at any time by typing `m` on your keyboard!

**NOTE:** If you'd like to change the size of the puzzles you're playing, simply redefine the `GRID_SIZE` constant at
the top of Game.cpp to any perfect square greater than or equal to 4.

## Any dependencies?

Yes. In order for this program to work, you must have **SDL2** and **Boost.Serialization** installed. Find their source
code at the links below:

- **SDL:**                  https://github.com/libsdl-org/SDL
- **Boost.Serialization:**  https://github.com/boostorg/serialization

## Gallery

Some screenshots of the program in action!!

![Menu](images/Screenshot%202023-12-04%20at%2010.31.03%20PM.png?raw=true)

![Classic 9x9 New Puzzle](images/Screenshot%202023-12-04%20at%2010.30.19%20PM.png?raw=true)

![Playing the Game](images/Screenshot%202023-12-04%20at%2010.30.35%20PM.png?raw=true)

![Winning the Game](images/Screenshot%202023-12-04%20at%2010.31.03%20PM.png?raw=true)

![Solver Mode Input](images/Screenshot%202023-12-04%20at%2010.32.22%20PM.png?raw=true)

![Solver Mode Output](images/Screenshot%202023-12-04%20at%2010.32.39%20PM.png?raw=true)