#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\my_macros.h"
#ifndef MY_MACROS_H
#define MY_MACROS_H

//#include <string>
#include "Arduino.h"
#include <unordered_map>

//LOGS_FLAGS
#define PRINT_LOGS 1
#define BT_LOGS 0

//MAZE DIMENSIONS
const uint8_t START_X = 0;
const uint8_t START_Y = 0;
const uint8_t MAZE_DIMENSION_X = 16;
const uint8_t MAZE_DIMENSION_Y = 16;

//INTERACTIONS
const int LONG_PRESS_TIME = 3000;

//MOUSE_STATE_ENUMS
enum SOLVING_RUN { 
   LEARNING = 1,
   SOLVING = 2
};

enum PID_CHECK {
  DONT_CHECK = 1,
  CHECK = 2
};

enum MOUSE_POSITION { 
  POSITION_UNDEFINED = 1,
  TO_DESTINATION = 2,
  TO_ORGIN = 3
};

enum MOUSE_STATE {
  HALT = 1,
  START_CHECK = 2,
  CALIBRATION_CHECK = 3,
  MEMORY_UPDATE = 4,
  MOVEMENT_DECISION = 5,
  MOVEMENT_PID = 6,
  RETRACING = 7
};

enum MOUSE_MOVEMENT {
  UNDEFINED = 1,
  MOVING_STRAIGHT = 2,
  MOVING_STRAIGHT_TO_STOP = 3,
  TURN = 4
};

enum TURNS {
  NO_TURN = 1,
  LEFT_TURN = 2,
  RIGHT_TURN = 3,
  U_TURN = 4,
  D_ROTATION = 5
};

enum MOUSE_SEARCH_LEVELS {
  SEARCHING = 1,
  DESTINATION_IDENTIFIED = 2,
  DESTINATION_OPTIMIZED = 3
};

// Mouse Directions

enum Direction {
    NORTH = 1,
    NORTH_EAST = 2,
    EAST = 3,
    SOUTH_EAST = 4,
    SOUTH = 5,
    SOUTH_WEST = 6,
    WEST = 7,
    NORTH_WEST = 8
};

enum MovementDirection {
    STRAIGHT = 1,
    LEFT = 2,
    RIGHT = 3,
    REVERSE = 4,
    ROTATION = 5,
    DO_NOTHING = 6
};


struct PID_Control {
    float kp;
    float ki;
    float kd;
};


// MEMORY PROCESSING
#define PROCESSING_ORDER {"left", "bottom", "right", "top"};
#define FACTOR 8
#define NO_PATH 65535

//int PWM_LIMIT = 27;
const int ERROR_LIMIT = 0;
// const int ACCELERATION__INIT = 10;
// const int ACCELERATION_INCREMENTER = 2;
// const int ACCELERATION_DIFF = 50;


const float WHEEL_DIAMETER = 33; //34mm
const float LENGTH_OF_MAZE_BOX = 185; //180mm
const float HEIGHT_OF_MAZE_BOX = 185; //180mm
const float ONE_ROTATION = 202;
const float WHEEL_DISTANCE = WHEEL_DIAMETER * PI;
const float ONE_CELL = (LENGTH_OF_MAZE_BOX / WHEEL_DISTANCE) * ONE_ROTATION;
const float FIRST_CELL = ONE_CELL * 1;


const float UPDATE_POSITION_FACTOR_LEFT = 0.85;
const float UPDATE_POSITION_FACTOR_RIGHT = 0.85;
const float UPDATE_POSITION_FACTOR_UTURN = 0.96;


//Learning Run Stop Before Distance
const int STOP_DISTANCE_LEARNING = 40; //40mm

//CONSTANTS
const int TOLLERENCE_MIN_360 = 5;
const int ANGLE_OFFSET = 45;
const int THREE_SIXTY_DEGREE = 360;
const int WALL_DISTANCE = 65;
const int DISTANCE_BETWEEN_WHEELS = 84;
const int TURN_SPEED = 60;

const int MAX_MOTOR_POWER = 255;
const int ROTATION_TURN = 135;
const int MINIMUM_SPEED_BUFFER = 10;

const int ACCLERATION_STEP_SIZE = 20;

#endif
