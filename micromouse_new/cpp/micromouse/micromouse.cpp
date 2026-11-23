#include <iostream>
#include <string>
#include "MazeSolver.h"



int main()
{
    uint8_t start_x = 0;
    uint8_t start_y = 0;
    uint8_t max_x = 16;
    uint8_t max_y = 16;
    std::string file_to_load = "C:\\D_Drive\\Events\\MicroMouse\\MazeSimulator\\dist\\Generated\\20241018120922.json";
    MazeSolver* maze_solver_obj = new MazeSolver(start_x, start_y, max_x, max_y, file_to_load);
    maze_solver_obj->solve();
    maze_solver_obj->solveToDestination(start_x);

}