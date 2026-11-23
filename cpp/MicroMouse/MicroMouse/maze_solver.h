#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <tuple>
#include <cassert>
#include "maze_obstrucle_mapper.h"
#include "maze_memory.h"
#include "micro_mouse.h"

class MazeSolver {
private:
    int x, y;
    int maxX, maxY;
    //std::vector<std::vector<bool>> visited;
    //std::vector<std::vector<int>> shortestPath;
    std::vector<std::vector<std::tuple<int, int>>> pathStack;
    std::vector<std::vector<std::tuple<int, int>>> pathStackHistory;
    std::vector<std::tuple<int, int>> currentPathStack;
    bool targetReached;
    int steps, sameSteps;
    MicroMouse* m_micromouse;
    MazeMemory* m_mazememory;
    MazeObstrucleMapper* m_obstrucle_mapper;
    void updateVisited();
    std::tuple<int, int> getNextBranchPath();
    std::tuple<int, int> backtrack(std::tuple<int, int> currentPath);

public:
    MazeSolver(int x, int y, int mazeSize, MicroMouse* micromouse, MazeMemory* mazememory, MazeObstrucleMapper* obstrucleMapper);
    bool solveMaze();
    void move(std::tuple<int, int> coordinates, bool backtracking = false);
    void solveMove(std::tuple<int, int> coordinates);
    void solveUsingLearning(int startX, int startY);
};
