#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "png.h"

int main()
{
    std::cout << "Input a size that you want. lengths must be greater than 1.\n";
    std::cout << "x:";
    int x = 0;
    std::cin >> x;
    std::cout << "y:";
    int y = 0;
    std::cin >> y;
    
    SquareMaze puzzle;
    puzzle.makeMaze(x, y);

    PNG* unsolved = puzzle.drawMaze();
    unsolved->writeToFile("unsolved.png");
    delete unsolved;

    std::vector<int> sol = m.solveMaze();

    PNG* solved = puzzle.drawMazeWithSolution();
    solved->writeToFile("solved.png");
    delete solved;

    return 0; 
}
