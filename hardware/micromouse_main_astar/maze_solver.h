#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <fstream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <cmath>
#include <cassert>
#include "maze_memory.h"
#include "micro_mouse.h"
#include "mouse_sensor.h"
#include "logger.h"
#include "utils.h"


class MazeSolver : public Utils {
private:
    uint8_t x, y;    
    MicroMouse* m_micromouse;
    MazeMemory* m_mazememory;
    Logger *m_logger;
    bool isMazeSolved;
    bool isBackTracking;
    std::stack<uint16_t> branch;
    int steps;

public:
    MazeSolver(uint8_t start_x, uint8_t start_y, uint8_t max_x, uint8_t max_y, MicroMouse* micromouse, MazeMemory* mazememory, Logger *loggerInstance);
    void solve();
    void moveTo(const uint16_t& next_path);
    bool isBacktrackingRequired(const uint16_t& next_path);
    void solveShortest(const uint16_t& start_location, const uint16_t& end_location);
    bool updateMazeSolved(const uint16_t& next_path);
    void backtrack();
    void getDestination(uint8_t& x_dest, uint8_t& y_dest);
    void solveToDestination(const uint16_t& start_location);
    bool isTargetReached();
};

#endif
