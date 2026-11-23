// MicroMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "maze_solver.h"
#undef abs
#include <vector>
#include <stack>
#include <utility>
#include "maze_memory.h"

MazeSolver::MazeSolver(uint8_t start_x, uint8_t start_y, uint8_t max_x, uint8_t max_y, MicroMouse* micromouse, MazeMemory* mazememory, Logger *loggerInstance)
    : x(start_x), y(start_y), m_micromouse(micromouse), m_logger(loggerInstance), m_mazememory(mazememory), isMazeSolved(false), isBackTracking(false), steps(0) {
    branch.push(0);
}

void MazeSolver::solve() {
    moveTo(Utils::encode_coordinates(x, y));
    if (!isMazeSolved) {
        m_mazememory->addVisited(x, y);
        uint16_t next_path = m_mazememory->getNextPathLearning(x, y);
        if (m_mazememory->getNumberOfPathsAvailable() > 1) {
            //std::cout << "Adding x: " << int(x) << ", y: " << int(y) << " to branch......\n";
            branch.push(Utils::encode_coordinates(x, y));
        }
        if (isBacktrackingRequired(next_path)) {
            isBackTracking = true;
            //std::cout << "Backtracking Start!!!!!" << std::endl;
            backtrack();
            //std::cout << "Backtracking End!!!!!" << std::endl;
            isBackTracking = false;
            return;
        } else {
            moveTo(next_path);
        }
        if (updateMazeSolved(next_path)) {
            return;
        }
    }
    //std::cout << "Overall Steps Taken for maze is " << steps << "\n";
}

void MazeSolver::moveTo(const uint16_t& next_path) {
    Utils::decode_coordinates(next_path, x, y);
    if (!isMazeSolved) {
        steps += 1;
    }
    m_micromouse->updatePathToTravel(x, y, isBackTracking);
    m_logger->print("Moving to x: ");
    m_logger->print(int(x));
    m_logger->print(" y: ");
    m_logger->print(int(y));
    m_logger->println();
}

bool MazeSolver::isBacktrackingRequired(const uint16_t& next_path) {
    return next_path == NO_PATH && !branch.empty();
}

void MazeSolver::backtrack() {
    auto last_branch = branch.top();
    branch.pop();
    auto current_path = Utils::encode_coordinates(x, y);
    auto path = m_mazememory->astarGetShortest(current_path, last_branch);
    while (!path.empty()) {
        auto current_backtrack = path.front();
        path.erase(path.begin());
        moveTo(current_backtrack);
    }
}

void MazeSolver::solveShortest(const uint16_t& start_location, const uint16_t& end_location) {
    //std::cout << "Shortest Solving: Path\n";
    auto path = m_mazememory->astarGetShortest(start_location, end_location);
    size_t steps_taken = path.size();
    //std::cout << "Steps taken: " << steps_taken << "\n";
}

bool MazeSolver::updateMazeSolved(const uint16_t& next_path) {
    if (next_path == NO_PATH && branch.empty()) {
        isMazeSolved = true;
    }
    return isMazeSolved;
}

void MazeSolver::getDestination(uint8_t& x_dest, uint8_t& y_dest) {
    Utils::decode_coordinates(m_mazememory->getDestination(), x_dest, y_dest);
}


void MazeSolver::solveToDestination(const uint16_t& start_location) {
    uint16_t destination = m_mazememory->getDestination();
    //std::cout << "Shortest Solving: Path to destination\n";
    auto path = m_mazememory->astarGetShortest(start_location, destination);
    size_t steps_taken = path.size();
    while (!path.empty()) {
        auto current_path = path.front();
        path.erase(path.begin());
        moveTo(current_path);
    }
    //std::cout << "Steps taken: " << steps_taken << "\n";

}

bool MazeSolver::isTargetReached() {
    return isMazeSolved;
}