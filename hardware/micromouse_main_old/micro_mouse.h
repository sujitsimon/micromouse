#ifndef MICRO_MOUSE_H
#define MICRO_MOUSE_H

#include "Arduino.h"
#undef min
#undef max
#include "mouse_sensor.h"
#include "maze_memory.h"
#include "maze_memory.h"
#include "mouse_motor.h"
#include <queue>
#include "logger.h"


enum Direction {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

enum MovementDirection {
    STRAIGHT = 1,
    LEFT = 2,
    RIGHT = 3,
    REVERSE = 4,
    DO_NOTHING = 5
};
const int SEARCH_OFFSET = 120;

class MicroMouse {
private:
    uint8_t x, y, max_x, max_y;
    Direction mouse_direction;
    Direction multistep_direction;
    static MicroMouse* m_micromouse;
    MouseSensor* m_mouse_sensor;
    MazeMemory* m_mazememory;
    MouseMotor* m_mouse_motor;
    std::queue<MovementDirection> path_to_travel;
    void updateAngle();
    Logger *m_logger;

public:
    MicroMouse(uint8_t x, uint8_t y, MouseSensor* m_mouse_sensor_inst, MazeMemory* m_mazememory_inst, MouseMotor* m_mouse_motor, Logger *loggerInstance, uint8_t max_x = 16, uint8_t max_y = 16, Direction mouse_direction = Direction::EAST);
    void turn_left(bool isMultiStep = false);
    void turn_right(bool isMultiStep = false);
    void make_u_turn(bool isMultiStep = false);
    std::string get_direction();
    void move_forward(float blocks);
    static MicroMouse* getInstance(uint8_t x, uint8_t y, MouseSensor* m_mouse_sensor_inst, MazeMemory* m_mazememory_inst, MouseMotor* m_mouse_motor, Logger *loggerInstance, uint8_t max_x = 16, uint8_t max_y = 16, Direction mouse_direction = Direction::EAST) {
        if (m_micromouse == nullptr) {
            m_micromouse = new MicroMouse(x, y, m_mouse_sensor_inst, m_mazememory_inst, m_mouse_motor, loggerInstance, max_x, max_y, mouse_direction);
        }
        return m_micromouse;
    }
    void updateMemoryData();
    void storeMultiStepDirection();
    void updatePathToTravel(uint8_t next_x, uint8_t next_y, bool isMultiStep = false);
    MovementDirection getNextPathToTravel();
    bool isPathEmpty();
    void resetToEast();
    void addFinishMove();
    void clearSolutionRunQueue();
    uint8_t getNextNStraight();
};

#endif
