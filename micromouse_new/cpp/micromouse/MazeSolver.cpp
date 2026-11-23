#include <iostream>
#include <vector>
#include <stack>
#include <utility>
#include "MazeMemory.h"
#include "MazeSolver.h"

MazeSolver::MazeSolver(uint8_t start_x, uint8_t start_y, uint8_t max_x, uint8_t max_y, const std::string& maze_file)
    : x(start_x), y(start_y), mazeMemory(max_x, max_y, maze_file), isMazeSolved(false), steps(0) {
    branch.push(0);
}

void MazeSolver::solve() {
    moveTo(Utils::encode_coordinates(x, y));
    while (!isMazeSolved) {
        mazeMemory.addVisited(x, y);
        uint16_t next_path = mazeMemory.getNextPathLearning(x, y);
        if (mazeMemory.getNumberOfPathsAvailable() > 1) {
            std::cout << "Adding x: " << int(x) << ", y: " << int(y) << " to branch......\n";
            branch.push(Utils::encode_coordinates(x, y));
        }
        if (isBacktrackingRequired(next_path)) {
            std::cout << "Backtracking Start!!!!!" << std::endl;
            backtrack();
            std::cout << "Backtracking End!!!!!" << std::endl;
            continue;
        } else {
            moveTo(next_path);
        }
        if (updateMazeSolved(next_path)) {
            break;
        }
    }
    std::cout << "Overall Steps Taken for maze is " << steps << "\n";
}

void MazeSolver::moveTo(const uint16_t& next_path) {
    Utils::decode_coordinates(next_path, x, y);
    if (!isMazeSolved) {
        steps += 1;
    }
    std::cout << "Moving to x: " << int(x) << " y: " << int(y) << "\n";
}

bool MazeSolver::isBacktrackingRequired(const uint16_t& next_path) {
    return next_path == NO_PATH && !branch.empty();
}

void MazeSolver::backtrack() {
    auto last_branch = branch.top();
    branch.pop();
    auto current_path = Utils::encode_coordinates(x, y);
    auto path = mazeMemory.astarGetShortest(current_path, last_branch);
    while (!path.empty()) {
        auto current_backtrack = path.front();
        path.erase(path.begin());
        moveTo(current_backtrack);
    }
}

void MazeSolver::solveShortest(const uint16_t& start_location, const uint16_t& end_location) {
    std::cout << "Shortest Solving: Path\n";
    auto path = mazeMemory.astarGetShortest(start_location, end_location);
    size_t steps_taken = path.size();
    std::cout << "Steps taken: " << steps_taken << "\n";
}

bool MazeSolver::updateMazeSolved(const uint16_t& next_path) {
    if (next_path == NO_PATH && branch.empty()) {
        isMazeSolved = true;
    }
    return isMazeSolved;
}

void MazeSolver::getDestination(uint8_t& x_dest, uint8_t& y_dest) {
    Utils::decode_coordinates(mazeMemory.getDestination(), x_dest, y_dest);
}


void MazeSolver::solveToDestination(const uint16_t& start_location) {
    uint16_t destination = mazeMemory.getDestination();
    std::cout << "Shortest Solving: Path to destination\n";
    auto path = mazeMemory.astarGetShortest(start_location, destination);
    size_t steps_taken = path.size();
    while (!path.empty()) {
        auto current_path = path.front();
        path.erase(path.begin());
        moveTo(current_path);
    }
    std::cout << "Steps taken: " << steps_taken << "\n";

}