#include <iostream>

enum class Direction {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

class MicroMouse {
private:
    int x, y, max_x, max_y;
    Direction direction;

public:
    MicroMouse(int x, int y, int max_x = 10, int max_y = 10, Direction direction = Direction::SOUTH)
        : x(x), y(y), max_x(max_x), max_y(max_y), direction(direction) {}

    void turn_left() {
        int direction_value = static_cast<int>(direction);
        direction_value -= 1;
        if (direction_value < 1) {
            direction_value = 4;
        }
        direction = static_cast<Direction>(direction_value);
    }

    void turn_right() {
        int direction_value = static_cast<int>(direction);
        direction_value += 1;
        if (direction_value > 4) {
            direction_value %= 4;
        }
        direction = static_cast<Direction>(direction_value);
    }

    void make_u_turn() {
        for (int i = 0; i < 2; ++i) {
            turn_right();
        }
    }

    std::string get_direction() {
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

    void move_forward() {
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
};

int main() {
    MicroMouse mm_obj(2, 0);
    int i = 0;
    while (i < 10) {
        mm_obj.turn_right();
        std::cout << mm_obj.get_direction() << std::endl;
        i++;
    }
    return 0;
}


