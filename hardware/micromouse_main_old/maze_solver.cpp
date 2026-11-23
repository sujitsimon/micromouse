// MicroMouse.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "maze_solver.h"
#undef abs

uint8_t MazeSolver::make_memory(uint8_t x, uint8_t y)
{
    uint8_t memory_address = 0;
    memory_address |= x;
    memory_address = memory_address << 4;
    memory_address |= y;
    return memory_address;
}

uint8_t MazeSolver::parseX(uint8_t memory)
{
    return (memory & 0xF0) >> 4;
}

uint8_t MazeSolver::parseY(uint8_t memory)
{
    return (memory & 0xF);
}

void MazeSolver::updateVisited()
{
    m_mazememory->add_visited(make_memory(x, y));
}

uint8_t MazeSolver::getNextBranchPath()
{
    uint8_t currentPath = pathStack.back();
    pathStack.pop_back();
    return currentPath;
}

// uint8_t MazeSolver::backtrack() {
//     uint8_t backtrackPath;
//     // if (!isBackTracking) {
//     //     return 0;
//     // }
//     while (true) {
//         backtrackPath = currentPathStack.back();
//         currentPathStack.pop_back();
//         //m_logger->println("Backtracking......");
//         if (!((parseX(backtrackPath) == x) && (parseY(backtrackPath) == y))) {
//             this->move(backtrackPath, true);
//             break;
//         } else {
//             //m_logger->print("Backtracking Same Coordinates Skipping: next_x: ");
//             //m_logger->print(parseX(backtrackPath));
//             //m_logger->print(" next_y: ");
//             //m_logger->print(parseY(backtrackPath));
//             //m_logger->print(" current_x: ");
//             //m_logger->print(x);
//             //m_logger->print(" current_y: ");
//             //m_logger->print(y);
//             //m_logger->println();
//         }
//     }
//     //std::cout << "Backtracking......" << std::endl;
//     if (currentPathStack.empty() && (std::abs(parseX(nextBranchPath) - x) + std::abs(parseY(nextBranchPath) - y) >= 1)) {
//         currentPathStack = pathStackHistory.back();
//         m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
//         pathStackHistory.pop_back();
//     } else if (std::abs(parseX(nextBranchPath) - x) + std::abs(parseY(nextBranchPath) - y) == 0) {
//         //m_logger->println("Backtracking Complete......");
//         currentPathStack = { make_memory(x, y) };
//         isBackTracking = false;
//     }
//     return backtrackPath;
// }

uint8_t MazeSolver::backtrack()
{
    uint8_t backtrackPath;
    // if (!isBackTracking) {
    //     return 0;
    // }
    while (true)
    {
        backtrackPath = currentPathStack.back();
        currentPathStack.pop_back();
        //m_logger->println("Backtracking......");
        if (!((parseX(backtrackPath) == x) && (parseY(backtrackPath) == y)))
        {
            this->move(backtrackPath, true);
        }
        else
        {
           /* //m_logger->print("Backtracking Same Coordinates Skipping: next_x: ");
            //m_logger->print(parseX(backtrackPath));
            //m_logger->print(" next_y: ");
            //m_logger->print(parseY(backtrackPath));
            //m_logger->print(" current_x: ");
            //m_logger->print(x);
            //m_logger->print(" current_y: ");
            //m_logger->print(y);
            //m_logger->println();*/
        }
        if (currentPathStack.empty() && (std::abs(parseX(nextBranchPath) - x) + std::abs(parseY(nextBranchPath) - y) >= 1))
        {
            currentPathStack = pathStackHistory.back();
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
            pathStackHistory.pop_back();
        }
        else if (std::abs(parseX(nextBranchPath) - x) + std::abs(parseY(nextBranchPath) - y) == 0)
        {
            //m_logger->println("Backtracking Complete......");
            // currentPathStack = {make_memory(x, y)};
            currentPathStack.push_back(make_memory(x, y));
            isBackTracking = false;
            break;
        }
    }
    return backtrackPath;
}

MazeSolver::MazeSolver(uint8_t x, uint8_t y, uint8_t mazeSize, MicroMouse *micromouse, MazeMemory *mazememory, Logger *loggerInstance) : x(x), y(y), max_X(mazeSize), max_Y(mazeSize),
                                                                                                                 targetReached(false), steps(0), sameSteps(0),
                                                                                                                 m_micromouse(micromouse), m_mazememory(mazememory), m_logger(loggerInstance)
{
}

void MazeSolver::solveMaze()
{
    if (x == 0 && y == 0)
    {
        this->move(make_memory(x, y));
    }
    if (!targetReached)
    {
        //m_logger->print("solveMaze:- x: ");
        //m_logger->print(x);
        //m_logger->print(" y: ");
        //m_logger->print(y);
        //m_logger->println();
        std::vector<uint8_t> paths = m_mazememory->get_next_shortest(x, y);
        if (!paths.empty())
        {
            currentPath = paths.back();
            paths.pop_back();
            if (!paths.empty())
            {
                m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
                pathStack.push_back(make_memory(x, y));
                pathStackHistory.push_back(currentPathStack);
                currentPathStack = {make_memory(x, y)};
            }
            this->move(currentPath);
        }
        else
        {
            // std::cout << "Seems Like Dead End x:" << x << ", y:" << y << std::endl;
            //m_logger->print("Seems Like Dead End x: ");
            //m_logger->print(x);
            //m_logger->print(" y: ");
            //m_logger->print(y);
            //m_logger->println();
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack, max_X * max_Y + 1);
            nextBranchPath = getNextBranchPath();
            //m_logger->print("nextBranchPath x: ");
            //m_logger->print(parseX(nextBranchPath));
            //m_logger->print(" y: ");
            //m_logger->print(parseY(nextBranchPath));
            //m_logger->println();
            m_micromouse->storeMultiStepDirection();
            isBackTracking = true;
            currentPath = backtrack();
            // assert(std::abs(std::get<0>(currentPath) - x) + std::abs(parseY(currentPath) - y) == 1);
            // currentPathStack = { make_memory(x, y) };
            // this->move(currentPath, true);
        }
    }
}

void MazeSolver::move(uint8_t coordinates, bool backtracking)
{
    x = parseX(coordinates);
    y = parseY(coordinates);
    //m_logger->print("move:- x: ");
    //m_logger->print(x);
    //m_logger->print(" y: ");
    //m_logger->print(y);
    //m_logger->println();
    m_micromouse->updatePathToTravel(x, y, backtracking);
    if (!backtracking)
    {
        currentPathStack.push_back(coordinates);
    }
    // std::cout << "Moving x: " << x << ", y: " << y << std::endl;
    updateVisited();
}

// void MazeSolver::solveMove(uint8_t coordinates) {
//     x = std::get<0>(coordinates);
//     y = parseY(coordinates);
//     //std::cout << "Moving x: " << x << ", y: " << y << std::endl;
// }

void MazeSolver::solveUsingLearning(uint8_t startX, uint8_t startY)
{
    //m_logger->print("solveUsingLearning Called x: ");
    //m_logger->print(startX);
    //m_logger->print(" y: ");
    //m_logger->println(startY);
    m_micromouse->storeMultiStepDirection();
    //m_logger->println("get_next_shortest_from_learning BEFORE");
    std::vector<uint8_t> path = m_mazememory->get_next_shortest_from_learning(make_memory(startX, startY));
    //m_logger->println("get_next_shortest_from_learning COMPLETE");
    for (const auto &coord : path)
    {
        move(coord, true);
        // delay(50);
    }
}

bool MazeSolver::isTargetReached()
{
    targetReached = false;
    //m_logger->print("isTargetReached x: ");
    //m_logger->print(x);
    //m_logger->print(" y: ");
    //m_logger->print(y);
    //m_logger->print(" status: ");
    //m_logger->print(m_mazememory->is_destination(x, y));
    //m_logger->println();
    if (m_mazememory->is_destination(x, y))
    {
        m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
        while (!pathStackHistory.empty())
        {
            currentPathStack = pathStackHistory.back();
            m_mazememory->update_shortest_coordinates_calculation(currentPathStack);
            pathStackHistory.pop_back();
        }
        targetReached = true;
    }
    return targetReached;
}

bool MazeSolver::isBackTrackingStage()
{
    return isBackTracking;
}

void MazeSolver::resetMouseToInitialPosition()
{
    x = 0;
    y = 0;
}

// uint8_t main()
//{
//     uint8_t start_x = 2, start_y = 0;
//     std::string file_path = "C:\\D_Drive\\MicroMouse\\maze_download\\20240411115950.json";
//     MazeObstrucleMapper* mom = new MazeObstrucleMapper(file_path);
//     uint8_t mazeSize = mom->getSize();
//     MicroMouse* micro_mouse_obj = MicroMouse::getInstance(start_x, start_y, mazeSize, mazeSize);
//     MazeMemory* maze_memory_obj = MazeMemory::getInstance(mazeSize, mazeSize);
//     MazeSolver* maze_solver_obj = new MazeSolver(start_x, start_y, mazeSize, micro_mouse_obj, maze_memory_obj, mom);
//     maze_solver_obj->solveMaze();
//     maze_solver_obj->solveUsingLearning(start_x, start_y);
// }
