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
    static MicroMouse* m_micromouse;

public:
    MicroMouse(int x, int y, int max_x = 10, int max_y = 10, Direction direction = Direction::NORTH);
    void turn_left();
    void turn_right();
    void make_u_turn();
    std::string get_direction();
    void move_forward();
    static MicroMouse* getInstance(int x, int y, int max_x = 10, int max_y = 10, Direction direction = Direction::SOUTH) {
        if (m_micromouse == nullptr) {
            m_micromouse = new MicroMouse(x, y, max_x, max_y, direction);
        }
        return m_micromouse;
    }
};