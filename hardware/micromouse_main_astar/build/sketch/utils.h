#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\utils.h"
#ifndef UTILS_H
#define UTILS_H

#include "my_macros.h"

class Utils
{
public:
    Utils();
    std::map<std::string, bool> obstacleDecoder(uint8_t obstacle);
    uint16_t encode_coordinates(uint8_t current_x, uint8_t current_y);
    void decode_coordinates(uint16_t coordinates, uint8_t& current_x, uint8_t& current_y);
    bool is_diagonal(uint16_t coordinates_1, uint16_t coordinates_2);
    void printCoordinates(uint16_t coordinates);

private:
    uint8_t m_factor = FACTOR;
};

#endif