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
    moveTo(Mouse_Utils::encode_coordinates(x, y));
    if (!isMazeSolved) {
        isBackTracking = false;
        m_mazememory->addVisited(x, y);
        uint16_t next_path = m_mazememory->getNextPathLearning(x, y);
        if (m_mazememory->getNumberOfPathsAvailable() > 1) {
            //std::cout << "Adding x: " << int(x) << ", y: " << int(y) << " to branch......\n";
            branch.push(Mouse_Utils::encode_coordinates(x, y));
        }
        if (isBacktrackingRequired(next_path)) {
            //m_logger->println("Backtracking Start!!!");
            if (m_mazememory->isDestinationFound() && !dance) {
                m_micromouse->destinationFoundMove();
                dance = true;
            } else if (m_mazememory->isDeadEnd(x, y)) {
                //m_micromouse->balanceToSideWall();
            }
            isBackTracking = true;
            backtrack();
            //m_logger->println("Backtracking End!!!");
            return;
        } else if (next_path != NO_PATH) {
            moveTo(next_path);
        }
        if (updateMazeSolved(next_path)) {
            return;
        }
    }
    //std::cout << "Overall Steps Taken for maze is " << steps << "\n";
}

bool MazeSolver::isBackTrackingNow() {
    return isBackTracking;
}

void MazeSolver::moveTo(const uint16_t& next_path) {
    Mouse_Utils::decode_coordinates(next_path, x, y);
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
    return (next_path == NO_PATH) && !branch.empty();
}

void MazeSolver::backtrack() {
    uint16_t last_branch;
    while (true) {
        uint8_t x = 0;
        uint8_t y = 0;
        last_branch = branch.top();
        branch.pop();
        Mouse_Utils::decode_coordinates(last_branch, x, y);
        if (m_mazememory->isBacktrackingRequired(x, y) || last_branch == 0) {
            break;
        } else {
            m_logger->print("Skipping Backtracking: ");
            m_logger->print(int(x));
            m_logger->print(" y: ");
            m_logger->print(int(y));
            m_logger->println();
        }
    }
    auto current_path = Mouse_Utils::encode_coordinates(x, y);
    auto path = m_mazememory->astarGetShortest(current_path, last_branch);
    m_micromouse->storeMultiStepDirection();
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
    Mouse_Utils::decode_coordinates(m_mazememory->getDestination(), x_dest, y_dest);
}


void MazeSolver::solveToDestination(const uint16_t& start_location) {
    uint16_t destination = m_mazememory->getDestination();
    //std::cout << "Shortest Solving: Path to destination\n";
    auto path = m_mazememory->astarGetShortest(start_location, destination);
    isBackTracking = true;
    size_t steps_taken = path.size();
    m_micromouse->storeMultiStepDirection();
    while (!path.empty()) {
        auto current_path = path.front();
        path.erase(path.begin());
        moveTo(current_path);
    }
    //std::cout << "Steps taken: " << steps_taken << "\n";

}

void MazeSolver::goBackToStartPoint() {
    auto path = m_mazememory->astarGetShortest(Mouse_Utils::encode_coordinates(x, y), 0);
    isBackTracking = true;
    size_t steps_taken = path.size();
    m_micromouse->storeMultiStepDirection();
    while (!path.empty()) {
        auto current_path = path.front();
        path.erase(path.begin());
        moveTo(current_path);
    }
}

bool MazeSolver::isMazeSolvedComplete() {
    m_logger->print("isMazeSolvedComplete: ");
    m_logger->print(isMazeSolved);
    m_logger->println();
    return isMazeSolved;
}