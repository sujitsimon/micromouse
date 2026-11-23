#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <iostream>
#include <vector>
#include <stack>
#include "MazeMemory.h"
#include "Utils.h"

class MazeSolver : public Utils {
private:
    uint8_t x, y;
    MazeMemory mazeMemory;
    bool isMazeSolved;
    std::stack<uint16_t> branch;
    int steps;

public:
    MazeSolver(uint8_t start_x, uint8_t start_y, uint8_t max_x, uint8_t max_y, const std::string& maze_file = "");
    void solve();
    void moveTo(const uint16_t& next_path);
    bool isBacktrackingRequired(const uint16_t& next_path);
    void solveShortest(const uint16_t& start_location, const uint16_t& end_location);
    bool updateMazeSolved(const uint16_t& next_path);
    void backtrack();
    void getDestination(uint8_t& x_dest, uint8_t& y_dest);
    void solveToDestination(const uint16_t& start_location);
};

#endif // MAZE_SOLVER_H