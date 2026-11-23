#ifndef MAZE_MEMORY_H
#define MAZE_MEMORY_H

#include "Arduino.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <cstdint>
//#include <cassert>

const uint8_t TOP_SIDE = 0;
const uint8_t RIGHT_SIDE = 1;
const uint8_t BOTTOM_SIDE = 2;
const uint8_t LEFT_SIDE = 3;
const uint8_t X_MASK = 0xF0;
const uint8_t Y_MASK = 0x0F;

class MazeMemory {
private:
    bool stub;
    uint8_t row;
    uint8_t column;
    uint8_t min_row_dest;
    uint8_t min_col_dest;
    uint8_t max_row_dest;
    uint8_t max_col_dest;
    std::map<uint8_t, uint16_t> memory;
    void create_initial_memory();
    std::set<uint8_t> visited;
    std::map<uint8_t, uint8_t> obstructle_memory;
    uint8_t make_memory(uint8_t x, uint8_t y);
    uint8_t parseX(uint8_t memory);
    uint8_t parseY(uint8_t memory);
    static MazeMemory* m_mazememory;
public:
    MazeMemory(uint8_t row, uint8_t column, uint8_t min_row_dest, uint8_t min_col_dest, uint8_t max_row_dest, uint8_t max_col_dest, bool stub);
    std::vector<uint8_t> get_next_shortest(uint8_t x, uint8_t y);
    bool is_destination(uint8_t x, uint8_t y);
    void add_visited(uint8_t coordinates);
    bool check_if_visited(uint8_t coordinates);
    std::vector<uint8_t> get_next_shortest_from_learning(uint8_t coordinates);
    void update_shortest_coordinates_calculation(std::vector<uint8_t> path, int start_value = -1);
    static MazeMemory* getInstance(uint8_t row, uint8_t column, uint8_t min_row_dest, uint8_t min_col_dest, uint8_t max_row_dest, uint8_t max_col_dest, bool stub) {
        if (m_mazememory == nullptr) {
            m_mazememory = new MazeMemory(row, column, min_row_dest, min_col_dest, max_row_dest, max_col_dest, stub);
        }
        return m_mazememory;
    }
    void updateObstrucleMemory(uint8_t x, uint8_t y, uint8_t current_grid);
    void updateSavedObstrucleMemory();
};
#endif
