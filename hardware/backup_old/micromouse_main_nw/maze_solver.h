#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <fstream>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <cassert>
//#include <iostream>
//#include "maze_obstrucle_mapper.h"
#include "maze_memory.h"
#include "micro_mouse.h"
#include "mouse_sensor.h"
#include "logger.h"


class MazeSolver {
private:
    uint8_t x, y;
    uint8_t max_X, max_Y;
    std::vector<uint8_t> pathStack;
    std::vector<std::vector<uint8_t>> pathStackHistory;
    std::vector<uint8_t> currentPathStack;
    uint8_t currentPath;
    uint8_t nextBranchPath;
    bool targetReached = false;
    bool isBackTracking = false;
    uint8_t steps, sameSteps;
    MicroMouse* m_micromouse;
    MazeMemory* m_mazememory;
    //MazeObstrucleMapper* m_obstrucle_mapper;
    void updateVisited();
    uint8_t getNextBranchPath();
    uint8_t make_memory(uint8_t x, uint8_t y);
    uint8_t parseX(uint8_t memory);
    uint8_t parseY(uint8_t memory);
    Logger *m_logger;

public:
    MazeSolver(uint8_t x, uint8_t y, uint8_t mazeSize, MicroMouse* micromouse, MazeMemory* mazememory, Logger *loggerInstance);
    void solveMaze();
    void move(uint8_t coordinates, bool backtracking = false);
    void solveMove(uint8_t coordinates);
    void solveUsingLearning(uint8_t startX, uint8_t startY);
    bool isTargetReached();
    bool isBackTrackingStage();
    uint8_t backtrack();
    void resetMouseToInitialPosition();
};

#endif
