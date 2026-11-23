#include "MazeMemory.h"
#include <iostream>
#include <cmath>

MazeMemory::MazeMemory(uint8_t max_x, uint8_t max_y, const std::string& maze_file)
    : m_max_x(max_x), m_max_y(max_y), path_available(0), may_be_destination(false),
    maze_loader(maze_file) {}

void MazeMemory::addVisited(uint8_t x, uint8_t y) {
    visited[Utils::encode_coordinates(x, y)] = true;
}

bool MazeMemory::isVisited(uint8_t x, uint8_t y) {
    return visited.find(Utils::encode_coordinates(x, y)) != visited.end();
}

uint8_t MazeMemory::getNumberOfPathsAvailable() const {
    return path_available;
}

bool MazeMemory::isFourInterconnected() {
    int missed_path = 0;
    for (const auto& each_node : destination_mapper) {
        uint8_t x, y;
        Utils::decode_coordinates(each_node, x, y);
        for (const auto& each_path : getPossibleNodes(x, y)) {
            if (std::find(destination_mapper.begin(), destination_mapper.end(), each_path) == destination_mapper.end()) {
                missed_path++;
            }
        }
    }
    return missed_path == 1;
}

void MazeMemory::checkIsValidDestination() {
    uint8_t min_x = m_max_x, min_y = m_max_y;
    uint8_t max_x = 0, max_y = 0;

    for (const auto& each : destination_mapper) {
        uint8_t x = 0, y = 0;
        Utils::decode_coordinates(each, x, y);
        min_x = std::min(min_x, x);
        min_y = std::min(min_y, y);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    if ((max_x - min_x == 1) && (max_y - min_y == 1) && isFourInterconnected()) {
        std::cout << "Destination Found: (" << int(min_x) << ", " << int(min_y) << ") to (" << int(max_x) << ", " << int(max_y) << ")\n";
        destination["min_x"] = min_x;
        destination["min_y"] = min_y;
        destination["max_x"] = max_x;
        destination["max_y"] = max_y;
        destination_point = destination_mapper[1];
    }
    destination_mapper.clear();
    may_be_destination = false;
}

uint16_t MazeMemory::getDestination() {
    return destination_point;
}

void MazeMemory::updateDestination(uint8_t current_x, uint8_t current_y) {
    if (may_be_destination) {
        if (path_available == 1 || (destination_mapper.size() == 3 && path_available == 0)) {
            destination_mapper.push_back(Utils::encode_coordinates(current_x, current_y));
        }
        else {
            destination_mapper.clear();
            may_be_destination = false;
        }
    }
    if (destination_mapper.size() == 4) {
        checkIsValidDestination();
    }
    if (path_available == 2) {
        may_be_destination = true;
        destination_mapper.push_back(Utils::encode_coordinates(current_x, current_y));
    }
}

std::vector<uint16_t> MazeMemory::getPossibleNodes(uint8_t current_x, uint8_t current_y) {
    if (!isVisited(current_x, current_y)) return {};

    uint8_t current_grid = memory[Utils::encode_coordinates(current_x, current_y)];
    auto grid_info = Utils::obstacleDecoder(current_grid);

    std::vector<uint16_t> possible_nodes;
    if (current_x - 1 >= 0 && !grid_info["left"]) {
        possible_nodes.push_back(Utils::encode_coordinates(current_x - 1, current_y));
    }
    if (current_y - 1 >= 0 && !grid_info["top"]) {
        possible_nodes.push_back(Utils::encode_coordinates(current_x, current_y - 1));
    }
    if (current_y + 1 <= m_max_y && !grid_info["bottom"]) {
        possible_nodes.push_back(Utils::encode_coordinates(current_x, current_y + 1));
    }
    if (current_x + 1 <= m_max_x && !grid_info["right"]) {
        possible_nodes.push_back(Utils::encode_coordinates(current_x + 1, current_y));
    }

    return possible_nodes;
}

uint16_t MazeMemory::getNextPathLearning(uint8_t current_x, uint8_t current_y) {
    memory[Utils::encode_coordinates(current_x, current_y)] = maze_loader.getObstacle(Utils::encode_coordinates(current_x, current_y));

    path_available = 0;
    uint16_t next_path = NO_PATH;

    std::vector<uint16_t> possible_nodes = getPossibleNodes(current_x, current_y);
    for (const auto& node : possible_nodes) {
        uint8_t x = 0, y = 0;
        Utils::decode_coordinates(node, x, y);
        if (!isVisited(x, y)) {
            path_available++;
            next_path = node;
        }
    }

    if (!(destination_point == -1)) {
        updateDestination(current_x, current_y);
    }

    return next_path;
}

std::vector<uint16_t> MazeMemory::astarGetShortest(const uint16_t& current_path, const uint16_t& next_path) {
    auto heuristic = [this](const uint16_t& a, const uint16_t& b) {
        uint8_t x1 = 0, y1 = 0;
        uint8_t x2 = 0, y2 = 0;
        Utils::decode_coordinates(a, x1, y1);
        Utils::decode_coordinates(b, x2, y2);
        return std::abs(x1 - x2) + std::abs(y1 - y2);
        };

    std::priority_queue<std::pair<uint16_t, uint16_t>, std::vector<std::pair<uint16_t, uint16_t>>, std::greater<>> open_set;
    open_set.push({ 0, current_path });

    std::map<uint16_t, uint16_t> came_from;
    std::map<uint16_t, uint16_t> g_score;
    g_score[current_path] = 0;

    std::map<uint16_t, uint16_t> f_score;
    f_score[current_path] = heuristic(current_path, next_path);

    while (!open_set.empty()) {
        auto top = open_set.top();
        auto current = top.second;
        open_set.pop();

        if (current == next_path) {
            std::vector<uint16_t> path;
            while (came_from.find(current) != came_from.end()) {
                path.push_back(current);
                current = came_from[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        uint8_t x = 0, y = 0;
        Utils::decode_coordinates(current, x, y);
        for (const auto& neighbor : getPossibleNodes(x, y)) {
            int tentative_g_score = g_score[current] + calculateCostFunction(came_from, current);
            if (!g_score.count(neighbor) || tentative_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = tentative_g_score + heuristic(neighbor, next_path);
                open_set.push({ f_score[neighbor], neighbor });
            }
        }
    }

    return {};
}

uint16_t MazeMemory::calculateCostFunction(std::map<uint16_t, uint16_t> came_from, uint16_t current_path) {
    int cost = 10;
    bool is_turn_required = false;
    auto previous_first = came_from[current_path];
    auto previous_second = came_from[previous_first];
    if (Utils::is_diagonal(current_path, previous_second)) {
        std::cout << "is diagonal true start" << std::endl;
        Utils::printCoordinates(current_path);
        Utils::printCoordinates(previous_second);
        std::cout << "is diagonal end" << std::endl;
        cost = 1;
    }
    ////do something
    return cost;
}