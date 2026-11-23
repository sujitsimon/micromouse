#include "micro_mouse.h"

MicroMouse* MicroMouse::m_micromouse = nullptr;

MicroMouse::MicroMouse(int x, int y,
                       int max_x, int max_y,
                       Direction direction)
    : x(x), y(y), max_x(max_x), max_y(max_y), direction(direction) 
{
}


void MicroMouse::turn_left() {
    int direction_value = static_cast<int>(direction);
    direction_value -= 1;
    if (direction_value < 1) {
        direction_value = 4;
    }
    direction = static_cast<Direction>(direction_value);
}


void MicroMouse::turn_right() {
    int direction_value = static_cast<int>(direction);
    direction_value += 1;
    if (direction_value > 4) {
        direction_value %= 4;
    }
    direction = static_cast<Direction>(direction_value);
}

void MicroMouse::make_u_turn() {
    for (int i = 0; i < 2; ++i) {
        turn_right();
    }
}

std::string MicroMouse::get_direction() {
    switch (direction) {
    case Direction::NORTH:
        return "NORTH";
    case Direction::EAST:
        return "EAST";
    case Direction::SOUTH:
        return "SOUTH";
    case Direction::WEST:
        return "WEST";
    default:
        return "UNKNOWN";
    }
}

void MicroMouse::move_forward() {
    switch (direction) {
    case Direction::SOUTH:
        if ((y + 1) < max_y) y += 1;
        break;
    case Direction::WEST:
        if ((x - 1) > 0) x -= 1;
        break;
    case Direction::NORTH:
        if ((y - 1) > 0) y -= 1;
        break;
    case Direction::EAST:
        if ((x + 1) < max_x) x += 1;
        break;
    }
}