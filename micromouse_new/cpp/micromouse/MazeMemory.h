#ifndef MAZEMEMORY_H
#define MAZEMEMORY_H

#include <map>
#include <vector>
#include <string>
#include <queue>
#include <cstdint>
#include <json/json.h>
#include <algorithm>
#include "MazeObstacleMapper.h"
#include "Utils.h"

class MazeMemory : public Utils {
public:
    MazeMemory(uint8_t max_x, uint8_t max_y, const std::string& maze_file = "");
    void addVisited(uint8_t x, uint8_t y);
    bool isVisited(uint8_t x, uint8_t y);
    uint8_t getNumberOfPathsAvailable() const;
    bool isFourInterconnected();
    void checkIsValidDestination();
    uint16_t getDestination();
    void updateDestination(uint8_t current_x, uint8_t current_y);
    std::vector<uint16_t> getPossibleNodes(uint8_t current_x, uint8_t current_y);
    uint16_t getNextPathLearning(uint8_t current_x, uint8_t current_y);
    std::vector<uint16_t> astarGetShortest(const uint16_t& current_path, const uint16_t& next_path);

private:
    uint8_t m_max_x, m_max_y;
    uint8_t target_x = -1, target_y = -1;
    uint8_t path_available;
    bool may_be_destination;
    uint8_t factor = FACTOR;
    std::map<uint16_t, bool> visited;
    std::map<uint16_t, uint8_t> memory;
    std::vector<uint16_t> destination_mapper;
    uint16_t destination_point = -1;
    std::map<std::string, uint8_t> destination;
    MazeObstacleMapper maze_loader;
    uint16_t calculateCostFunction(std::map<uint16_t, uint16_t> came_from, uint16_t current_path);
};

#endif // MAZEMEMORY_H
