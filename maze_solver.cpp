
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>
#include <tuple>

class MazeObstrucleMapper {
private:
    std::string obstrucleFile;
    std::unordered_map<std::tuple<int, int>, std::unordered_map<std::string, bool>> obstrucle;

    void loadFile() {
        std::ifstream file(obstrucleFile);
        std::string line;
        while (std::getline(file, line)) {
            size_t commaPos = line.find(',');
            int x = std::stoi(line.substr(0, commaPos));
            int y = std::stoi(line.substr(commaPos + 1, line.find(':')));
            std::string obstruction = line.substr(line.find(':') + 1);
            obstrucle[std::make_tuple(x, y)][obstruction] = true;
        }
        file.close();
    }

public:
    MazeObstrucleMapper(const std::string& obstrucleFile) : obstrucleFile(obstrucleFile) {
        loadFile();
    }

    std::unordered_map<std::string, bool> getObstrucle(int x, int y) {
        return obstrucle[std::make_tuple(x, y)];
    }

    int getSize() {
        return static_cast<int>(std::sqrt(obstrucle.size()));
    }
};

class MazeSolver {
private:
    int x, y;
    int maxX, maxY;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<int>> shortestPath;
    std::vector<std::tuple<int, int>> pathStack;
    std::vector<std::vector<std::tuple<int, int>>> pathStackHistory;
    std::vector<std::tuple<int, int>> currentPathStack;
    bool targetReached;
    int steps, sameSteps;

    void updateVisited() {
        visited[x][y] = true;
    }

    std::tuple<int, int> getNextBranchPath() {
        std::vector<std::tuple<int, int>> newBranch = pathStack.back();
        pathStack.pop_back();
        std::tuple<int, int> currentPath = newBranch.back();
        newBranch.pop_back();
        if (!newBranch.empty()) {
            pathStack.push_back(newBranch);
        }
        return currentPath;
    }

    std::tuple<int, int> backtrack(std::tuple<int, int> currentPath) {
        while (!currentPathStack.empty()) {
            std::tuple<int, int> backtrackPath = currentPathStack.back();
            currentPathStack.pop_back();
            std::cout << "Backtracking......" << std::endl;
            move(backtrackPath, true);
            if (currentPathStack.empty() && (std::abs(std::get<0>(currentPath) - x) + std::abs(std::get<1>(currentPath) - y) > 1)) {
                currentPathStack = pathStackHistory.back();
                pathStackHistory.pop_back();
            }
        }
        std::cout << "Backtracking Complete." << std::endl;
        return currentPath;
    }

public:
    MazeSolver(int x, int y, int mazeSize, MazeObstrucleMapper& obstrucleMapper) :
        x(x), y(y), maxX(mazeSize), maxY(mazeSize),
        visited(mazeSize, std::vector<bool>(mazeSize, false)),
        shortestPath(mazeSize, std::vector<int>(mazeSize, INT_MAX)),
        targetReached(false), steps(0), sameSteps(0) {
        move(std::make_tuple(x, y));
        while (!targetReached) {
            std::vector<std::tuple<int, int>> paths = getNextShortestPaths(x, y, obstrucleMapper.getObstrucle(x, y));
            std::tuple<int, int> currentPath;
            if (!paths.empty()) {
                currentPath = paths.back();
                paths.pop_back();
            } else {
                std::cout << "Seems Like Dead End x:" << x << ", y:" << y << std::endl;
                shortestPath[x][y] = maxX * maxY + 1;
                std::tuple<int, int> nextBranchPath = getNextBranchPath();
                currentPath = backtrack(nextBranchPath);
                assert(std::abs(std::get<0>(currentPath) - x) + std::abs(std::get<1>(currentPath) - y) == 1);
                currentPathStack = { std::make_tuple(x, y) };
            }
            if (!paths.empty()) {
                pathStack.push_back(paths);
                pathStackHistory.push_back(currentPathStack);
                currentPathStack = { std::make_tuple(x, y) };
            }
            move(currentPath);
            if (isDestination(x, y)) {
                while (!pathStackHistory.empty()) {
                    currentPathStack = pathStackHistory.back();
                    pathStackHistory.pop_back();
                }
                std::cout << "Destination Reached" << std::endl;
                std::cout << "Overall steps taken = " << steps << std::endl;
                std::cout << "Overall same steps taken = " << sameSteps << std::endl;
                targetReached = true;
            }
        }
    }

    void move(std::tuple<int, int> coordinates, bool backtracking = false) {
        if (x != std::get<0>(coordinates) || y != std::get<1>(coordinates)) {
            steps++;
        } else {
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

    void solveMove(std::tuple<int, int> coordinates) {
        x = std::get<0>(coordinates);
        y = std::get<1>(coordinates);
        std::cout << "Moving x: " << x << ", y: " << y << std::endl;
    }

    void solveUsingLearning(int startX, int startY) {
        std::cout << "************************" << std::endl;
        std::cout << "** Solving from Learning" << std::endl;
        std::cout << "************************" << std::endl;
        std::vector<std::tuple<int, int>> path = getNextShortestFromLearning(std::make_tuple(startX, startY));
        for (const auto& coord : path) {
            solveMove(coord);
        }
        std::cout << "Memory Solve Steps: " << path.size() << std::endl;
    }

private:
    std::vector<std::tuple<int, int>> getNextShortestPaths(int x, int y, const std::unordered_map<std::string, bool>& obstrucles) {
        // Implement your logic to get the next shortest paths based on obstrucles
        return {};
    }

    bool isDestination(int x, int y) {
        // Implement your logic to check if the current position is the destination
        return false;
    }

    std::vector<std::tuple<int, int>> getNextShortestFromLearning(std::tuple<int, int> start) {
        // Implement your logic to get the next shortest path from learning
        return {};
    }
};

int main() {
    // Implement your main function here
    return 0;
}

