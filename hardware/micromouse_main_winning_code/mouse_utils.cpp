#include "mouse_utils.h"

Mouse_Utils::Mouse_Utils() {

}

std::unordered_map<std::string, bool> Mouse_Utils::obstacleDecoder(uint8_t obstacle) {
    std::string array[] = PROCESSING_ORDER;
    std::unordered_map<std::string, bool> temp;
    uint8_t size = 4;
    for (uint8_t index = 0; index < size; ++index) {
        temp[array[index]] = (obstacle & (1 << (size - index - 1))) > 0;
    }
    return temp;
}

uint16_t Mouse_Utils::encode_coordinates(uint8_t current_x, uint8_t current_y) {
    return (current_x << m_factor) | current_y;
}

void Mouse_Utils::decode_coordinates(uint16_t coordinates, uint8_t& current_x, uint8_t& current_y) {
    current_y = coordinates & (0xFF);
    current_x = coordinates >> 8;
}

bool Mouse_Utils::is_diagonal(uint16_t coordinates_1, uint16_t coordinates_2) {
    uint8_t x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    decode_coordinates(coordinates_1, x1, y1);
    decode_coordinates(coordinates_2, x2, y2);
    return (std::abs(x1 - x2) == std::abs(y1 - y2));
}

void Mouse_Utils::printCoordinates(uint16_t coordinates) {
    uint8_t x = 0, y = 0;
    decode_coordinates(coordinates, x, y);
}