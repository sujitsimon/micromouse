#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\maze_memory.h"
#ifndef MAZE_MEMORY_H
#define MAZE_MEMORY_H

#include "Arduino.h"
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <string>
#include <cstdint>
#include "logger.h"
#include "mouse_utils.h"

//#include <cassert>

const uint8_t TOP_SIDE = 0;
const uint8_t RIGHT_SIDE = 1;
const uint8_t BOTTOM_SIDE = 2;
const uint8_t LEFT_SIDE = 3;
const uint8_t X_MASK = 0xF0;
const uint8_t Y_MASK = 0x0F;

class MazeMemory : public Mouse_Utils {
public:
    static MazeMemory* getInstance(uint8_t max_x, uint8_t max_y, bool stub, Logger *loggerInstance) {
        if (m_mazememory == nullptr) {
            m_mazememory = new MazeMemory(max_x, max_y, stub, loggerInstance);
        }
        return m_mazememory;
    }
    MazeMemory(uint8_t max_x, uint8_t max_y, bool stub, Logger *loggerInstance);
    void addVisited(uint8_t x, uint8_t y);
    bool isVisited(uint8_t x, uint8_t y);
    uint8_t getNumberOfPathsAvailable() const;
    bool isFourInterconnected();
    void checkIsValidDestination();
    bool isDestinationFound();
    uint16_t getDestination();
    void updateDestination(uint8_t current_x, uint8_t current_y);
    std::vector<uint16_t> getPossibleNodes(uint8_t current_x, uint8_t current_y);
    uint16_t getNextPathLearning(uint8_t current_x, uint8_t current_y);
    std::vector<uint16_t> astarGetShortest(const uint16_t& current_path, const uint16_t& next_path);
    void updateObstrucleMemory(uint8_t x, uint8_t y, uint8_t value);
    bool isBacktrackingRequired(uint8_t current_x, uint8_t current_y);
    bool isDeadEnd(uint8_t current_x, uint8_t current_y);

private:
    bool stub;
    uint8_t m_max_x, m_max_y;
    uint8_t target_x = -1, target_y = -1;
    uint8_t path_available;
    bool may_be_destination;
    uint8_t factor = FACTOR;
    std::map<uint16_t, bool> visited;
    std::map<uint16_t, uint8_t> memory;
    std::vector<uint16_t> destination_mapper;
    uint16_t destination_point = NO_PATH;
    std::map<std::string, uint8_t> destination;
    uint16_t calculateCostFunction(std::map<uint16_t, uint16_t> came_from, uint16_t current_path);
    static MazeMemory* m_mazememory;
    Logger *m_logger;
    void updateSavedObstrucleMemory();
};

#endif
