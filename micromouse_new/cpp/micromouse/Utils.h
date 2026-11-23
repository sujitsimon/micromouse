#pragma once
#include <string>
#include <map>
#include "MyMacros.h"

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

