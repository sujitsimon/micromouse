#include "angle_monitor.h"
#include "logger.h"
#include "maze_memory.h"
#include "maze_solver.h"
#include "mouse_motor.h"
#include "mouse_sensor.h"
#include "my_macros.h"
#include "mouse_pin_config.h"
#include <queue>
#include <string>

//#define DEBUG
//#define RELEASE

SOLVING_RUN CURRENT_RUN = SOLVING_RUN::LEARNING;
MOUSE_STATE CURRENT_MOUSE_STATE = MOUSE_STATE::HALT;
MOUSE_MOVEMENT CURRENT_MOVEMENT = MOUSE_MOVEMENT::UNDEFINED;
TURNS CURRENT_TURN = TURNS::NO_TURN;
MOUSE_SEARCH_LEVELS CURRENT_MOUSE_LEVEL = MOUSE_SEARCH_LEVELS::SEARCHING;
MOUSE_POSITION CURRENT_MOVEMENT_DIRECTION = MOUSE_POSITION::POSITION_UNDEFINED;
PID_CHECK CURRENT_PID_CHECK_STATE = PID_CHECK::DONT_CHECK;

volatile MovementDirection MouseMovementDirection = MovementDirection::DO_NOTHING;

bool STUB = false;
bool SKIP_LOOP = false;

bool error_to_fix = false;
bool solution_mode_change = false;
uint8_t solution_change_blink_count = 5;

int DESTINATION_WAIT = 500;
int DELAY_START_UP = 2000;
int SENSOR_CALIBRATION_DELAY = 3000;
int LEARNING_POWER_MAX = 85;
int SOLVING_POWER = 110;
int CENTER_STOP_CHECKING_DISTANCE = 250;
int STOP_BEFORE_DISTANCE = 30;
int FAKE_POSITIVE_DISTANCE = 50;
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
MouseSensor *mouse_sensor_obj = MouseSensor::getInstance(loggerInstance);

AngleMonitor *angle_monitor_obj = AngleMonitor::getInstance(loggerInstance);
// //loggerInstance->println("mouse_sensor_obj init COMPLETE....");
MazeMemory *maze_memory_obj = MazeMemory::getInstance(
    MAZE_DIMENSION_X, MAZE_DIMENSION_Y, STUB, loggerInstance);
// //loggerInstance->println("maze_memory_obj init COMPLETE....");
MouseMotor *mouse_motor_obj = MouseMotor::getInstance(angle_monitor_obj, mouse_sensor_obj, loggerInstance);
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
  // loggerInstance->print("Looping.....");
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
      CURRENT_PID_CHECK_STATE = PID_CHECK::CHECK;
      angle_monitor_obj->updateOffset(0);
      angle_monitor_obj->initialize();
      delay(SENSOR_CALIBRATION_DELAY);
      mouse_motor_obj->resetMotor();
      mouse_motor_obj->calibrate_minimum_speed();
      // while (true) {
      //   angle_monitor_obj->loop();
      //   mouse_motor_obj->turnToAngle(0);        
      //   loggerInstance->println("turnToAngle 0");
      //   if (mouse_motor_obj->getTurnError()) {
      //     break;
      //   }
      // }
      if ((CURRENT_MOUSE_LEVEL == MOUSE_SEARCH_LEVELS::DESTINATION_IDENTIFIED) ||
          (CURRENT_MOUSE_LEVEL == MOUSE_SEARCH_LEVELS::DESTINATION_OPTIMIZED) &&
          (CURRENT_RUN == SOLVING_RUN::SOLVING)) {
        mouse_motor_obj->setLearningPower(SOLVING_POWER);
        loggerInstance->println("Solve Run Starting!!!!");
        micro_mouse_obj->resetToEast();
        maze_solver_obj->solveToDestination(0);
        CURRENT_MOVEMENT_DIRECTION = MOUSE_POSITION::TO_DESTINATION;
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

  //Initial Solve
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::MEMORY_UPDATE) {
      loggerInstance->println("Updating Memory...");
      micro_mouse_obj->updateMemoryData();
      maze_solver_obj->solve();
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
      loggerInstance->println("Starting Run...");
      MouseMovementDirection = micro_mouse_obj->getCurrentMovementDirection();
      mouse_motor_obj->setNextMemoryUpdatePosition(FIRST_CELL);
  }
  angle_monitor_obj->loop();
  mouse_sensor_obj->triggerSensors();

  //Movement Decision State
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::MOVEMENT_DECISION) {
      loggerInstance->print("current_path is ");
      loggerInstance->println((int)MouseMovementDirection);
      switch (MouseMovementDirection)
      {
        case MovementDirection::STRAIGHT:
          CURRENT_PID_CHECK_STATE = PID_CHECK::CHECK;
          if (mouse_motor_obj->isFirstBlockCrossed()) {
            if (CURRENT_TURN == TURNS::RIGHT_TURN) {
              mouse_motor_obj->setNextMemoryUpdatePosition(ONE_CELL * UPDATE_POSITION_FACTOR_RIGHT);
            }else if (CURRENT_TURN == TURNS::LEFT_TURN) {
              mouse_motor_obj->setNextMemoryUpdatePosition(ONE_CELL * UPDATE_POSITION_FACTOR_LEFT);
            }else if (CURRENT_TURN == TURNS::NO_TURN) {
              mouse_motor_obj->setNextMemoryUpdatePosition(ONE_CELL);
            }
          }
          CURRENT_TURN = TURNS::NO_TURN;
          loggerInstance->println("MovementDirection::STRAIGHT.....");
          break;
        case MovementDirection::RIGHT:
          micro_mouse_obj->turn_right();
          CURRENT_PID_CHECK_STATE = PID_CHECK::DONT_CHECK;
          CURRENT_TURN = TURNS::RIGHT_TURN;
          mouse_motor_obj->resetMotor();
          loggerInstance->println("MovementDirection::RIGHT.....");
          break;
        case MovementDirection::LEFT:
          micro_mouse_obj->turn_left();
          CURRENT_PID_CHECK_STATE = PID_CHECK::DONT_CHECK;
          CURRENT_TURN = TURNS::LEFT_TURN;
          mouse_motor_obj->resetMotor();
          loggerInstance->println("MovementDirection::LEFT.....");
          break;       
        default:
          loggerInstance->println("Default Stopping Motor.....");
          mouse_motor_obj->stopBothMotor();
          break;
      }
    if (MouseMovementDirection != MovementDirection::DO_NOTHING) {
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_PID;
    }
  }

  //PID Solver State
  if (CURRENT_MOUSE_STATE == MOUSE_STATE::MOVEMENT_PID) {
    loggerInstance->println("MOVEMENT_PID.......");
    int current_error;
    switch (MouseMovementDirection)
    {
    case MovementDirection::STRAIGHT:
      mouse_motor_obj->learningLoopRun();
      current_error = mouse_motor_obj->getStopBeforeDistanceError();
      break;
    case MovementDirection::LEFT:
      mouse_motor_obj->TurnLeftOnTheGo();
      current_error = mouse_motor_obj->getOnTheGoTurnError();
      break;
    case MovementDirection::RIGHT:
      mouse_motor_obj->TurnRightOnTheGo();
      current_error = mouse_motor_obj->getOnTheGoTurnError();
      break;
    case MovementDirection::REVERSE:
      mouse_motor_obj->stopBothMotor();
      break;
    default:
      mouse_motor_obj->stopBothMotor();
      break;
    }
    loggerInstance->print("Current Motor Encoder Value is");
    loggerInstance->println(mouse_motor_obj->getPositionLeftMotor());
    loggerInstance->print("Memory Update Positon");
    loggerInstance->println(mouse_motor_obj->getNextMemoryUpdatePosition());
    loggerInstance->print("Current Error is ");
    loggerInstance->println((int)current_error);
    if (((MouseMovementDirection == MovementDirection::LEFT)
     || (MouseMovementDirection == MovementDirection::RIGHT))
     && (abs(current_error) <= 1)) {
      loggerInstance->println("Turn Complete: Setting Straight");
      MouseMovementDirection = MovementDirection::STRAIGHT;
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
    }
  }
  loggerInstance->print("Current MouseMovementDirection is ");
  loggerInstance->println((int)MouseMovementDirection);
  micro_mouse_obj->printDebugMemory();
  
}

void readEncoderLeftMotor() {
  if ((CURRENT_PID_CHECK_STATE == PID_CHECK::DONT_CHECK)) {
    //loggerInstance->print("Read Left Encoder Skipped:");
    return;
  }
  int b = digitalRead(LEFT_MOTOR_ENCODER_GREEN);
  bool updateState = true;
  bool memoryUpdateState = false;
  if (b > 0) {
    memoryUpdateState = mouse_motor_obj->updatePositionLeftMotor(updateState);
    if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
      mouse_motor_obj->updatePositionRightMotor(updateState);
    }
  } else {
    updateState = false;
    memoryUpdateState = mouse_motor_obj->updatePositionLeftMotor(updateState);
    if (CURRENT_MOVEMENT == MOUSE_MOVEMENT::MOVING_STRAIGHT) {
      mouse_motor_obj->updatePositionRightMotor(updateState);
    }
  }
  if (mouse_sensor_obj->getCenterDistance() < 40) {
    memoryUpdateState = true;
  }
  if (memoryUpdateState) {
      //SKIP_LOOP = true;
      //mouse_motor_obj->stopBothMotor();
      // loggerInstance->println("Updating Memory...");
      micro_mouse_obj->updateMemoryData();
      // loggerInstance->println("Solving...");
      maze_solver_obj->solve();
      //delay(1000);
      //SKIP_LOOP = false;
      MouseMovementDirection = micro_mouse_obj->getCurrentMovementDirection();
      CURRENT_MOUSE_STATE = MOUSE_STATE::MOVEMENT_DECISION;
      mouse_motor_obj->setNextMemoryUpdatePosition(ONE_CELL);
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
    mouse_motor_obj->stopBothMotor();
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
    CURRENT_MOVEMENT_DIRECTION = MOUSE_POSITION::POSITION_UNDEFINED;
    CURRENT_TURN = TURNS::NO_TURN;
  }
  loggerInstance->print("Interrupt HIT....");
  mouse_motor_obj->stopBothMotor();
  // loggerInstance->print(CURRENT_RUN);
  loggerInstance->println();
}

void dmpDataReadySet() { angle_monitor_obj->dmpDataReady(); }
