#define PRINT_LOGS 1
#define BT_LOGS 0
#include "angle_monitor.h"
#include "logger.h"
#include "maze_memory.h"
#include "maze_solver.h"
#include "mouse_motor.h"
#include "mouse_sensor.h"
#include <queue>
#include <string>

//#define DEBUG
//#define RELEASE

const uint8_t START_X = 0;
const uint8_t START_Y = 0;
const uint8_t MAZE_DIMENSION_X = 16;
const uint8_t MAZE_DIMENSION_Y = 16;
const int LONG_PRESS_TIME = 3000;

enum SOLVING_RUN { 
   LEARNING = 1,
   SOLVING = 2
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
  SOLVE_MAZE = 5,
  MOVEMENT_DECISION = 6,
  MOVEMENT_PID = 7,
  RETRACING = 8
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

SOLVING_RUN CURRENT_RUN = SOLVING_RUN::LEARNING;
MOUSE_STATE CURRENT_MOUSE_STATE = MOUSE_STATE::HALT;
MOUSE_MOVEMENT CURRENT_MOVEMENT = MOUSE_MOVEMENT::UNDEFINED;
TURNS CURRENT_TURN = TURNS::NO_TURN;
MOUSE_SEARCH_LEVELS CURRENT_MOUSE_LEVEL = MOUSE_SEARCH_LEVELS::SEARCHING;
MOUSE_POSITION CURRENT_DIRECTION = MOUSE_POSITION::POSITION_UNDEFINED;

bool STUB = false;

const int LEFT_TRIGGER = PB3;
const int CENTER_TRIGGER = PB4;
const int RIGHT_TRIGGER = PB5;
const int LEFT_ECHO = PA2;
const int RIGHT_ECHO = PA0;
const int CENTER_ECHO = PA1;

// Motor Setup

const int LEFT_MOTOR_P = PB13;
const int LEFT_MOTOR_N = PB12;
const int RIGHT_MOTOR_P = PB14;
const int RIGHT_MOTOR_N = PB15;
const int LEFT_MOTOR_PWM = PA6;
const int RIGHT_MOTOR_PWM = PA7;
const int LEFT_MOTOR_ENCODER_YELLOW = PA5;
const int LEFT_MOTOR_ENCODER_GREEN = PA4;
const int RIGHT_MOTOR_ENCODER_YELLOW = PB1; // PA7
const int RIGHT_MOTOR_ENCODER_GREEN = PB0;  // PA6

const int STAND_BY_MOTOR_DRIVER = PB10;

// LED Wall
const int LEFT_WALL_LED = PC14;
const int CENTER_WALL_LED = PC15;
const int RIGHT_WALL_LED = PA11;
const int BALANCE_LEFT_RIGHT_WALL_LED = PB8;

const int ALGORITHM_CHANGE = PA8;
const int ALGORITHM_CHANGE_LED = PC13;

bool stopInterrupt = false;
bool error_to_fix = false;
bool solution_mode_change = false;
uint8_t solution_change_blink_count = 5;

int DESTINATION_WAIT = 500;
int DELAY_START_UP = 2000;
int SENSOR_CALIBRATION_DELAY = 3000;
int LEARNING_POWER_MAX = 70;
int SOLVING_POWER = 255;
int CENTER_STOP_CHECKING_DISTANCE = 250;
int STOP_BEFORE_DISTANCE = 30;
float SAVE_SAMPLE_PERCENT = 75;
bool CURRENT_UPDATE_FLAG = false;
// int DELAY_SENSOR_TRIGGER = 10;
// unsigned long LAST_SENSOR_TRIGGER = 0;

// Gyroscope Sensor
const int GYROSCOPE_INTERRUPT_PIN = PB9;

volatile unsigned long pressTime = 0;
volatile bool isPressed = false;
// std::queue<int> turn_stack;
// int turn_value;

Logger *loggerInstance = Logger::getInstance(PRINT_LOGS, BT_LOGS);
MouseSensor *mouse_sensor_obj = MouseSensor::getInstance(
    LEFT_TRIGGER, LEFT_ECHO, RIGHT_TRIGGER, RIGHT_ECHO, CENTER_TRIGGER,
    CENTER_ECHO, LEFT_WALL_LED, RIGHT_WALL_LED, CENTER_WALL_LED,
    BALANCE_LEFT_RIGHT_WALL_LED, loggerInstance);

AngleMonitor *angle_monitor_obj = AngleMonitor::getInstance(loggerInstance);
// //loggerInstance->println("mouse_sensor_obj init COMPLETE....");
MazeMemory *maze_memory_obj = MazeMemory::getInstance(
    MAZE_DIMENSION_X, MAZE_DIMENSION_Y, STUB, loggerInstance);
// //loggerInstance->println("maze_memory_obj init COMPLETE....");
MouseMotor *mouse_motor_obj = MouseMotor::getInstance(
    LEFT_MOTOR_P, LEFT_MOTOR_N, RIGHT_MOTOR_P, RIGHT_MOTOR_N, LEFT_MOTOR_PWM,
    RIGHT_MOTOR_PWM, angle_monitor_obj, mouse_sensor_obj, loggerInstance);
// //loggerInstance->println("mouse_motor_obj init COMPLETE....");
MicroMouse *micro_mouse_obj = MicroMouse::getInstance(
    START_X, START_Y, mouse_sensor_obj, maze_memory_obj, mouse_motor_obj,
    loggerInstance, MAZE_DIMENSION_X, MAZE_DIMENSION_Y);
// //loggerInstance->println("micro_mouse_obj init COMPLETE....");
MazeSolver *maze_solver_obj =
    new MazeSolver(START_X, START_Y, MAZE_DIMENSION_X, MAZE_DIMENSION_Y,
                   micro_mouse_obj, maze_memory_obj, loggerInstance);
// //loggerInstance->println("maze_solver_obj init COMPLETE....");

void setup() {

  pinMode(LEFT_MOTOR_ENCODER_YELLOW, INPUT);
  pinMode(LEFT_MOTOR_ENCODER_GREEN, INPUT);
  pinMode(RIGHT_MOTOR_ENCODER_YELLOW, INPUT);
  pinMode(RIGHT_MOTOR_ENCODER_GREEN, INPUT);
  pinMode(ALGORITHM_CHANGE, INPUT_PULLDOWN);
  pinMode(GYROSCOPE_INTERRUPT_PIN, INPUT);
  pinMode(ALGORITHM_CHANGE_LED, OUTPUT);
  pinMode(STAND_BY_MOTOR_DRIVER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ALGORITHM_CHANGE), detectButtonPress, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENCODER_YELLOW), readEncoderLeftMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENCODER_YELLOW), readEncoderRightMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(GYROSCOPE_INTERRUPT_PIN), dmpDataReadySet, RISING);
  digitalWrite(STAND_BY_MOTOR_DRIVER, HIGH);
  digitalWrite(BALANCE_LEFT_RIGHT_WALL_LED, LOW);
  digitalWrite(ALGORITHM_CHANGE_LED, HIGH);
  mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
  Serial.begin(115200);
  loggerInstance->createBTInstance();
  if (STUB) {
    while (!Serial);
    loggerInstance->println("SETUP COMPLETE....");
    delay(DELAY_START_UP);
  }
  CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
}

void loop() {
  mouse_sensor_obj->triggerSensors();
  // loggerInstance->print("CURRENT_MOUSE_STATE: ");
  // loggerInstance->println(int(CURRENT_MOUSE_STATE));
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::CALIBRATION_CHECK) {
    // loggerInstance->println("MOUSE_STATE::CALIBRATION_CHECK");
    if (mouse_sensor_obj->doStartupCheck()) {
      digitalWrite(BALANCE_LEFT_RIGHT_WALL_LED, HIGH);
      angle_monitor_obj->calibrateOffset();
      digitalWrite(BALANCE_LEFT_RIGHT_WALL_LED, LOW);
      CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
    }
    delay(10);
    return;
  }
  if (solution_mode_change && (CURRENT_RUN == SOLVING_RUN::SOLVING)) {
    while (solution_change_blink_count) {
      digitalWrite(ALGORITHM_CHANGE_LED, HIGH);
      delay(100);
      digitalWrite(ALGORITHM_CHANGE_LED, LOW);
      delay(100);
      solution_change_blink_count--;
    }
    solution_mode_change = false;
  } else {
    digitalWrite(ALGORITHM_CHANGE_LED, LOW);
  }
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::START_CHECK) {
    if (mouse_sensor_obj->doStartupCheck()) {
      stopInterrupt = false;
      angle_monitor_obj->updateOffset(0);
      angle_monitor_obj->initialize();
      delay(SENSOR_CALIBRATION_DELAY);
      mouse_motor_obj->resetMotor();
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_PID;
      mouse_motor_obj->calibrate_minimum_speed();
      // while (true) {
      //   angle_monitor_obj->loop();
      //   mouse_motor_obj->turnToAngle(0);
      //   if (mouse_motor_obj->getTurnError()) {
      //     break;
      //   }
      // }
      if ((CURRENT_MOUSE_LEVEL == MOUSE_SEARCH_LEVELS::DESTINATION_IDENTIFIED) ||
          (CURRENT_MOUSE_LEVEL == MOUSE_SEARCH_LEVELS::DESTINATION_OPTIMIZED) &&
          (CURRENT_RUN == SOLVING_RUN::SOLVING)) {
        loggerInstance->println("Solve Run Starting!!!!");
        micro_mouse_obj->resetToEast();
        maze_solver_obj->solveToDestination(0);
        CURRENT_DIRECTION = MOUSE_POSITION::TO_DESTINATION;
        CURRENT_TURN = TURNS::NO_TURN;
      } else if (CURRENT_RUN == SOLVING_RUN::SOLVING) {
        loggerInstance->println("Failed Because of Other Two!!!!");
      }
      CURRENT_MOUSE_STATE = MOUSE_STATE::MEMORY_UPDATE;
    } else {
      if (CURRENT_MOUSE_STATE == MOUSE_STATE::CALIBRATION_CHECK) {
        return;
      }
      delay(10);
      CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
      return;
    }
  }

  // Sensor Block
  if (!STUB) {
    mouse_sensor_obj->triggerSensors();
    angle_monitor_obj->loop();
    // loggerInstance->print("CURRENT_UPDATE_FLAG:");
    // loggerInstance->println(CURRENT_UPDATE_FLAG);
    if ((!maze_solver_obj->isBackTrackingNow()) && (CURRENT_UPDATE_FLAG) && (mouse_motor_obj->getSolvingPercent() > SAVE_SAMPLE_PERCENT) && (CURRENT_RUN == SOLVING_RUN::LEARNING)) {
      micro_mouse_obj->updateMemoryData();
      loggerInstance->print("Updating Memory. Percent: ");
      loggerInstance->println(mouse_motor_obj->getSolvingPercent());
      CURRENT_UPDATE_FLAG = false;
    }
  }

  if (CURRENT_MOUSE_STATE == MOUSE_STATE::MEMORY_UPDATE) {
    if (CURRENT_RUN == SOLVING_RUN::LEARNING) {
      if (!STUB) {
        micro_mouse_obj->updateMemoryData();
      }
    } 
    CURRENT_MOUSE_STATE = MOUSE_STATE::SOLVE_MAZE;
  }

  if (CURRENT_MOUSE_STATE == MOUSE_STATE::SOLVE_MAZE) {    
    loggerInstance->println("Solve Maze Called...");
    maze_solver_obj->solve();
    CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
  }

  if ((CURRENT_MOUSE_STATE == MOUSE_STATE::MOVEMENT_DECISION) && !micro_mouse_obj->isPathEmpty()) {
    stopInterrupt = false;
    float StraightCount = 0.0;
    MovementDirection current_path = micro_mouse_obj->getNextPathToTravel();
    loggerInstance->println("Poping current path");
    CURRENT_MOVEMENT = MOUSE_MOVEMENT::MOVING_STRAIGHT;
    switch (current_path) {
    case MovementDirection::STRAIGHT:
      CURRENT_UPDATE_FLAG = true;
      mouse_motor_obj->resetMotor();
      if (mouse_sensor_obj->getCenterDistance() <
          CENTER_STOP_CHECKING_DISTANCE) {
        // Double check the center sensor Issue making sure that it does not
        // take a false move before it
        //mouse_sensor_obj->triggerCenterSensor();
        //mouse_sensor_obj->updateSensorDistance();
        if (mouse_sensor_obj->getCenterDistance() <
            CENTER_STOP_CHECKING_DISTANCE) {
            loggerInstance->println("Move Straight to Stop");
            CURRENT_MOVEMENT = MOUSE_MOVEMENT::MOVING_STRAIGHT_TO_STOP;
            mouse_motor_obj->resetSolvingPercent(1);
            angle_monitor_obj->updateOffset(0);
        }
      } else {
        if (CURRENT_TURN == TURNS::U_TURN) {
          StraightCount = 0.96;
        } else if (CURRENT_TURN == TURNS::LEFT_TURN) {
          StraightCount = 0.925;
        } else if (CURRENT_TURN == TURNS::RIGHT_TURN) {
          StraightCount = 0.925;
        } else if (CURRENT_TURN == TURNS::D_ROTATION) {
          StraightCount = 1;
        } else {
          StraightCount = 1;
        }
        StraightCount += micro_mouse_obj->getNextNStraight();
        loggerInstance->println("Move Straight");
        micro_mouse_obj->move_forward(StraightCount);
        mouse_motor_obj->resetSolvingPercent(0);
      }
      CURRENT_TURN = TURNS::NO_TURN;
      break;
    case MovementDirection::LEFT:
      loggerInstance->println("Move Left");
      micro_mouse_obj->turn_left();
      CURRENT_MOVEMENT = MOUSE_MOVEMENT::TURN;
      CURRENT_TURN = TURNS::LEFT_TURN;
      angle_monitor_obj->updateOffset(0);
      break;
    case MovementDirection::RIGHT:
      loggerInstance->println("Move Right");
      micro_mouse_obj->turn_right();
      CURRENT_MOVEMENT = MOUSE_MOVEMENT::TURN;
      CURRENT_TURN = TURNS::RIGHT_TURN;
      angle_monitor_obj->updateOffset(0);
      break;
    case MovementDirection::REVERSE:
      loggerInstance->println("Move Reverse");
      micro_mouse_obj->make_u_turn();
      CURRENT_MOVEMENT = MOUSE_MOVEMENT::TURN;
      CURRENT_TURN = TURNS::U_TURN;
      angle_monitor_obj->updateOffset(0);
      break;
    case MovementDirection::ROTATION:
      loggerInstance->println("Move Rotate");
      CURRENT_MOVEMENT = MOUSE_MOVEMENT::TURN;
      CURRENT_TURN = TURNS::D_ROTATION;
      mouse_motor_obj->setFullRotationFlag();
      angle_monitor_obj->updateOffset(0);
      break;
    case MovementDirection::DO_NOTHING:
      break;
    }
    CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_PID;
  }

  if (CURRENT_MOUSE_STATE == MOUSE_STATE::MOVEMENT_PID) {
    if (!STUB) {
        if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
          mouse_motor_obj->loopStraight();
        } else if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT_TO_STOP) {
          mouse_motor_obj->stopBeforeDistance(STOP_BEFORE_DISTANCE);
        } else if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::TURN) {
          if (CURRENT_TURN == TURNS::D_ROTATION) {
            mouse_motor_obj->do360Degree();
          } else {
            mouse_motor_obj->turnToAngle(mouse_motor_obj->getDesiredAngle());
          }
        }
      }
    error_to_fix = false;
    if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
      //loggerInstance->println("Moving Straight: ");
      error_to_fix = mouse_motor_obj->getError();
    } else if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT_TO_STOP) {
      //loggerInstance->println(" Moving Straight To Stop: ");
      error_to_fix = (mouse_motor_obj->getStopBeforeDistanceError() == 0);
    } else if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::TURN) {
      //loggerInstance->println(" Turing: ");
      error_to_fix = (mouse_motor_obj->getTurnError());
    }
    if (error_to_fix) {
      loggerInstance->println("Error Cleared: ");
      //delay(100);
      stopInterrupt = true;
      mouse_motor_obj->stopBothMotor();
      mouse_motor_obj->resetMotor();
      //CURRENT_TURN = TURNS::NO_TURN;
      if (micro_mouse_obj->isPathEmpty()) {
        if (CURRENT_RUN == SOLVING_RUN::LEARNING) {
          CURRENT_MOUSE_STATE = MOUSE_STATE::SOLVE_MAZE;
          if (maze_memory_obj->isDestinationFound()) {
            CURRENT_MOUSE_LEVEL = MOUSE_SEARCH_LEVELS::DESTINATION_IDENTIFIED;
            if (maze_solver_obj->isMazeSolvedComplete()) {
              CURRENT_MOUSE_LEVEL = MOUSE_SEARCH_LEVELS::DESTINATION_OPTIMIZED;
              CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
            }
          }  
        } else if (CURRENT_RUN == SOLVING_RUN::SOLVING) {
          loggerInstance->println("Setting to Retracing....");
          CURRENT_MOUSE_STATE = MOUSE_STATE::RETRACING;
          CURRENT_MOVEMENT = MOUSE_MOVEMENT::UNDEFINED;
        }
      } else {
        CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
      }
    }
  }
  
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::RETRACING) {
    delay(DESTINATION_WAIT);
    if (CURRENT_DIRECTION == MOUSE_POSITION::TO_DESTINATION) {
      maze_solver_obj->goBackToStartPoint();
      micro_mouse_obj->addFinishMove();
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
      CURRENT_DIRECTION = MOUSE_POSITION::TO_ORGIN;
    } else {
      CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
    }
  }

  if (STUB) {
    delay(50);
  }
}

void readEncoderLeftMotor() {
  if ((CURRENT_MOUSE_STATE != MOUSE_STATE::MOVEMENT_PID)) {
    //loggerInstance->print("Read Left Encoder Skipped:");
    return;
  }
  if (stopInterrupt) {
    return;
  }
  int b = digitalRead(LEFT_MOTOR_ENCODER_GREEN);
  if (b > 0) {
    mouse_motor_obj->updatePositionLeftMotor(true);
    if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
      mouse_motor_obj->updatePositionRightMotor(true);
    }
  } else {
    mouse_motor_obj->updatePositionLeftMotor(false);
    if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
      mouse_motor_obj->updatePositionRightMotor(false);
    }
  }
}

void readEncoderRightMotor() {
  return;
  if (CURRENT_MOUSE_STATE != MOUSE_STATE::MOVEMENT_PID) {
    return;
  }
  if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
    return;
  }
  int b = digitalRead(RIGHT_MOTOR_ENCODER_GREEN);
  if (b > 0) {
    mouse_motor_obj->updatePositionRightMotor(true);
  } else {
    mouse_motor_obj->updatePositionRightMotor(false);
  }
}

void detectButtonPress() {
  if (digitalRead(ALGORITHM_CHANGE) == HIGH) {
    pressTime = millis();
    isPressed = true;
    return;
  }
  if (isPressed && (millis() - pressTime) < LONG_PRESS_TIME) {
    loggerInstance->println("Short Press -> Calculated");
    changeSolvingMode();
  } else {
    loggerInstance->println("Long Press -> Calculated");
    CURRENT_MOUSE_STATE = MOUSE_STATE::CALIBRATION_CHECK;
    loggerInstance->print("detectButtonPress CURRENT_MOUSE_STATE: ");
    loggerInstance->println(int(CURRENT_MOUSE_STATE));
  }
  isPressed = false;
}

void changeSolvingMode() {
  if (CURRENT_RUN == SOLVING_RUN::SOLVING) {
    CURRENT_RUN = SOLVING_RUN::LEARNING;
    digitalWrite(ALGORITHM_CHANGE_LED, LOW);
    loggerInstance->println("Solution Path Clearing if exists");
    micro_mouse_obj->clearSolutionRunQueue();
    mouse_motor_obj->loop_stop();
    micro_mouse_obj->resetToEast();
    error_to_fix = false;
    CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
    CURRENT_MOVEMENT = MOUSE_MOVEMENT::UNDEFINED;
    CURRENT_TURN = TURNS::NO_TURN;
    solution_mode_change = false;
  } else {
    CURRENT_RUN = SOLVING_RUN::SOLVING;
    digitalWrite(ALGORITHM_CHANGE_LED, HIGH);
    solution_mode_change = true;
    solution_change_blink_count = 5;
    loggerInstance->println("Switching to Solve Run");
    CURRENT_MOUSE_STATE = MOUSE_STATE::START_CHECK;
    CURRENT_DIRECTION = MOUSE_POSITION::POSITION_UNDEFINED;
    CURRENT_TURN = TURNS::NO_TURN;
  }
  loggerInstance->print("Interrupt HIT....");
  mouse_motor_obj->stopBothMotor();
  // loggerInstance->print(CURRENT_RUN);
  loggerInstance->println();
}

void dmpDataReadySet() { angle_monitor_obj->dmpDataReady(); }
