// MicroMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "maze_solver.h"


void MazeSolver::updateVisited() {
    m_mazememory->add_visited(std::make_tuple(x, y));
}

std::tuple<int, int> MazeSolver::getNextBranchPath() {
    std::vector<std::tuple<int, int>> newBranch = pathStack.back();
    pathStack.pop_back();
    std::tuple<int, int> currentPath = newBranch.back();
    newBranch.pop_back();
    if (!newBranch.empty()) {
        pathStack.push_back(newBranch);
    }
    return currentPath;
}

std::tuple<int, int> MazeSolver::backtrack(std::tuple<int, int> currentPath) {
    while (!currentPathStack.empty()) {
        std::tuple<int, int> backtrackPath = currentPathStack.back();
        currentPathStack.pop_back();
        std::cout << "Backtracking......" << std::endl;
        move(backtrackPath, true);
        if (currentPathStack.empty() && (std::abs(std::get<0>(currentPath) - x) + std::abs(std::get<1>(currentPath) - y) > 1)) {
            currentPathStack = pathStackHistory.back();
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
            pathStackHistory.pop_back();
        }
    }
    std::cout << "Backtracking Complete." << std::endl;
    return currentPath;
}

MazeSolver::MazeSolver(int x, int y, int mazeSize, MicroMouse* micromouse, MazeMemory* mazememory, MazeObstrucleMapper* obstrucleMapper) :
    x(x), y(y), maxX(mazeSize), maxY(mazeSize),
    targetReached(false), steps(0), sameSteps(0),
    m_micromouse(micromouse), m_mazememory(mazememory), m_obstrucle_mapper(obstrucleMapper)
{

}

bool MazeSolver::solveMaze() {
    this->move(std::make_tuple(x, y));
    while (!targetReached) {
        std::vector<std::tuple<int, int>> paths = m_mazememory->get_next_shortest(x, y, m_obstrucle_mapper->getObstrucle(x, y));
        std::tuple<int, int> currentPath;
        if (!paths.empty()) {
            currentPath = paths.back();
            paths.pop_back();
        }
        else {
            std::cout << "Seems Like Dead End x:" << x << ", y:" << y << std::endl;
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack, maxX * maxY + 1);
            std::tuple<int, int> nextBranchPath = getNextBranchPath();
            currentPath = backtrack(nextBranchPath);
            assert(std::abs(std::get<0>(currentPath) - x) + std::abs(std::get<1>(currentPath) - y) == 1);
            currentPathStack = { std::make_tuple(x, y) };
        }
        if (!paths.empty()) {
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
            pathStack.push_back(paths);
            pathStackHistory.push_back(currentPathStack);
            currentPathStack = { std::make_tuple(x, y) };
        }
        this->move(currentPath);
        if (m_mazememory->is_destination(x, y)) {
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
            while (!pathStackHistory.empty()) {
                currentPathStack = pathStackHistory.back();
                m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
                pathStackHistory.pop_back();
            }
            std::cout << "Destination Reached" << std::endl;
            std::cout << "Overall steps taken = " << steps << std::endl;
            std::cout << "Overall same steps taken = " << sameSteps << std::endl;
            targetReached = true;
        }
    }
    return true;
}

void MazeSolver::move(std::tuple<int, int> coordinates, bool backtracking) {
    if (x != std::get<0>(coordinates) || y != std::get<1>(coordinates)) {
        steps++;
    }
    else {
        sameSteps++;
    }
    x = std::get<0>(coordinates);
    y = std::get<1>(coordinates);
    if (!backtracking) {
        currentPathStack.push_back(coordinates);
    }
    std::cout << "Moving x: " << x << ", y: " << y << std::endl;
    updateVisited();
}

void MazeSolver::solveMove(std::tuple<int, int> coordinates) {
    x = std::get<0>(coordinates);
    y = std::get<1>(coordinates);
    std::cout << "Moving x: " << x << ", y: " << y << std::endl;
}

void MazeSolver::solveUsingLearning(int startX, int startY) {
    std::cout << "************************" << std::endl;
    std::cout << "** Solving from Learning" << std::endl;
    std::cout << "************************" << std::endl;
    std::vector<std::tuple<int, int>> path = m_mazememory->get_next_shortest_from_learning(std::make_tuple(startX, startY));
    for (const auto& coord : path) {
        solveMove(coord);
    }
    std::cout << "Memory Solve Steps: " << path.size() << std::endl;
}

int main()
{
    int start_x = 0, start_y = 0;
    std::string file_path = "C:\\D_Drive\\MicroMouse\\maze_download_6x6\\20240411121749.json";
    MazeObstrucleMapper* mom = new MazeObstrucleMapper(file_path);
    int mazeSize = mom->getSize();
    MicroMouse* micro_mouse_obj = MicroMouse::getInstance(start_x, start_y, mazeSize, mazeSize);
    MazeMemory* maze_memory_obj = MazeMemory::getInstance(mazeSize, mazeSize);
    MazeSolver* maze_solver_obj = new MazeSolver(start_x, start_y, mazeSize, micro_mouse_obj, maze_memory_obj, mom);
    maze_solver_obj->solveMaze();
    maze_solver_obj->solveUsingLearning(start_x, start_y);
}
