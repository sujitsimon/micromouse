#include "maze_memory.h"
#include "mouse_sensor.h"
#include "mouse_motor.h"
#include "maze_solver.h"
#include "angle_monitor.h"
#include <string>
#include <queue>

const uint8_t START_X = 0;
const uint8_t START_Y = 0;
const uint8_t MAZE_DIMENSION_X = 16;
const uint8_t MAZE_DIMENSION_Y = 16;

// const uint8_t START_X = 0;
// const uint8_t START_Y = 0;
// const uint8_t MAZE_DIMENSION_X = 6;
// const uint8_t MAZE_DIMENSION_Y = 6;

// const uint8_t START_X = 0;
// const uint8_t START_Y = 0;
// const uint8_t MAZE_DIMENSION_X = 4;
// const uint8_t MAZE_DIMENSION_Y = 4;

// Destination
const uint8_t MIN_ROW_DEST = 7;
const uint8_t MIN_COL_DEST = 7;
const uint8_t MAX_ROW_DEST = 8;
const uint8_t MAX_COL_DEST = 8;

// Destination
// const uint8_t MIN_ROW_DEST = 2;
// const uint8_t MIN_COL_DEST = 2;
// const uint8_t MAX_ROW_DEST = 3;
// const uint8_t MAX_COL_DEST = 3;

// Destination
// const uint8_t MIN_ROW_DEST = 1;
// const uint8_t MIN_COL_DEST = 1;
// const uint8_t MAX_ROW_DEST = 2;
// const uint8_t MAX_COL_DEST = 2;

// Sensor Setup
const int LEFT_TRIGGER = PB3;
const int CENTER_TRIGGER = PB4;
const int RIGHT_TRIGGER = PB5;
const int LEFT_ECHO = PA2;
const int RIGHT_ECHO = PA0;
const int CENTER_ECHO = PA1;

// Motor Setup
const int LEFT_MOTOR_P = PB12;
const int LEFT_MOTOR_N = PB13;
const int RIGHT_MOTOR_P = PB15;
const int RIGHT_MOTOR_N = PB14;
const int LEFT_MOTOR_PWM = PA6;
const int RIGHT_MOTOR_PWM = PA7;
const int LEFT_MOTOR_ENCODER_YELLOW = PA5;
const int LEFT_MOTOR_ENCODER_GREEN = PA4;
const int RIGHT_MOTOR_ENCODER_YELLOW = PB1; // PA7
const int RIGHT_MOTOR_ENCODER_GREEN = PB0;  // PA6

const int STAND_BY_MOTOR_DRIVER = PB10;

// LED Wall
const int LEFT_WALL_LED = PA15;
const int CENTER_WALL_LED = PA10;
const int RIGHT_WALL_LED = PA9;
const int BALANCE_LEFT_RIGHT_WALL_LED = PB8;

const int ALGORITHM_CHANGE = PA8;
const int ALGORITHM_CHANGE_LED = PC13;

bool start_run = false;
bool check_next_step = true;
bool run_complete = false;
bool is_back_tracking = false;
bool is_motor_running = false;
bool is_learning_run = true;
bool isMouseMovingStraight = false;
bool isMouseMovingStraightToStop = false;
bool stub = false;
bool stopInterrupt = false;
bool turnFlag = false;
bool isSolveRunComplete = false;
bool isLastDistanceAUtrun = false;
bool isMazeAtleastSolvedOnce = false;
bool isFinalPushSolved = false;

int DELAY_START_UP = 2500;
int LEARNING_POWER_MAX = 100;
int SOLVING_POWER = 255;
int angle_to_change = 180;
int counter = 0;
int CENTER_STOP_CHECKING_DISTANCE = 280;
int STOP_BEFORE_DISTANCE = 30;

// Gyroscope Sensor
const int GYROSCOPE_INTERRUPT_PIN = PB9;

// std::queue<int> turn_stack;
// int turn_value;

MouseSensor *mouse_sensor_obj = MouseSensor::getInstance(LEFT_TRIGGER, LEFT_ECHO, RIGHT_TRIGGER, RIGHT_ECHO, CENTER_TRIGGER, CENTER_ECHO, LEFT_WALL_LED, RIGHT_WALL_LED, CENTER_WALL_LED, BALANCE_LEFT_RIGHT_WALL_LED);

AngleMonitor *angle_monitor_obj = AngleMonitor::getInstance();
// //Serial.println("mouse_sensor_obj init COMPLETE....");
MazeMemory *maze_memory_obj = MazeMemory::getInstance(MAZE_DIMENSION_X, MAZE_DIMENSION_Y, MIN_ROW_DEST, MIN_COL_DEST, MAX_ROW_DEST, MAX_COL_DEST, stub);
// //Serial.println("maze_memory_obj init COMPLETE....");
MouseMotor *mouse_motor_obj = MouseMotor::getInstance(LEFT_MOTOR_P, LEFT_MOTOR_N, RIGHT_MOTOR_P, RIGHT_MOTOR_N, LEFT_MOTOR_PWM, RIGHT_MOTOR_PWM, angle_monitor_obj, mouse_sensor_obj);
// //Serial.println("mouse_motor_obj init COMPLETE....");
MicroMouse *micro_mouse_obj = MicroMouse::getInstance(START_X, START_Y, mouse_sensor_obj, maze_memory_obj, mouse_motor_obj, MAZE_DIMENSION_X, MAZE_DIMENSION_Y);
// //Serial.println("micro_mouse_obj init COMPLETE....");
MazeSolver *maze_solver_obj = new MazeSolver(START_X, START_Y, MAZE_DIMENSION_X, micro_mouse_obj, maze_memory_obj);
// //Serial.println("maze_solver_obj init COMPLETE....");

void setup()
{

  pinMode(LEFT_MOTOR_ENCODER_YELLOW, INPUT);
  pinMode(LEFT_MOTOR_ENCODER_GREEN, INPUT);
  pinMode(RIGHT_MOTOR_ENCODER_YELLOW, INPUT);
  pinMode(RIGHT_MOTOR_ENCODER_GREEN, INPUT);
  pinMode(ALGORITHM_CHANGE, INPUT_PULLDOWN);
  pinMode(GYROSCOPE_INTERRUPT_PIN, INPUT);
  pinMode(ALGORITHM_CHANGE_LED, OUTPUT);
  pinMode(STAND_BY_MOTOR_DRIVER, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ALGORITHM_CHANGE), changeSolvingMode, FALLING);
  attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_ENCODER_YELLOW), readEncoderLeftMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_ENCODER_YELLOW), readEncoderRightMotor, RISING);
  attachInterrupt(digitalPinToInterrupt(GYROSCOPE_INTERRUPT_PIN), dmpDataReadySet, RISING);
  digitalWrite(STAND_BY_MOTOR_DRIVER, HIGH);
  // std::stack<MovementDirection> path_to_travel;
  //   path_to_travel.push(MovementDirection::LEFT);
  //   path_to_travel.push(MovementDirection::STRAIGHT);
  //   path_to_travel.push(MovementDirection::LEFT);
  //   path_to_travel.push(MovementDirection::STRAIGHT);
  //   path_to_travel.push(MovementDirection::LEFT);
  //   path_to_travel.push(MovementDirection::STRAIGHT);
  //   path_to_travel.push(MovementDirection::RIGHT);
  //   path_to_travel.push(MovementDirection::STRAIGHT);
  // turn_stack.push(0);
  // turn_stack.push(90);
  // turn_stack.push(270);
  // turn_stack.push(90);
  // turn_stack.push(270);
  // turn_stack.push(90);
  // turn_stack.push(270);
  // turn_stack.push(90);
  // turn_stack.push(270);
  // turn_stack.push(0);
  // turn_stack.push(270);
  // turn_stack.push(0);
  // turn_stack.push(270);
  // turn_stack.push(90);
  // turn_stack.push(0);
  // turn_stack.push(180);
  // turn_stack.push(0);
  // turn_stack.push(270);
  // turn_stack.push(0);
  // turn_stack.push(90);
  // turn_stack.push(180);
  // turn_stack.push(270);
  // turn_stack.push(0);
  // turn_stack.push(270);
  // turn_stack.push(180);
  // turn_stack.push(90);
  // turn_stack.push(0);
  mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
  Serial.begin(115200);
  if (stub)
  {
    while (!Serial)
      ;
    // delay(5000);
  }
  Serial.println("SETUP COMPLETE....");
}

void loop()
{
  // Serial.print("ALGORITHM_CHANGE Pin value....");
  // Serial.println(digitalRead(ALGORITHM_CHANGE));
  // Serial.print("is_learning_run Pin value....");
  // Serial.println(is_learning_run);
  if (is_learning_run)
  {
    digitalWrite(ALGORITHM_CHANGE_LED, LOW); // HIGH will turn OFF the LED
  }
  else
  {
    digitalWrite(ALGORITHM_CHANGE_LED, HIGH); // LOW will turn ON the LED
  }
  if (stub)
  {
    start_run = true;
    // angle_monitor_obj->initialize();
    // mouse_motor_obj->calibrate_minimum_speed();
    //   while (true)
    //   {
    //     angle_monitor_obj->loop();
    //     mouse_motor_obj->turnToAngle(0);
    //     if (mouse_motor_obj->getTurnError() == 0)
    //     {
    //       break;
    //     }
    //   }
    if (maze_solver_obj->isTargetReached() && !is_learning_run && !isSolveRunComplete)
    {
      micro_mouse_obj->resetToEast();
      maze_solver_obj->solveUsingLearning(0, 0);
      maze_solver_obj->resetMouseToInitialPosition();
      run_complete = false;
      isSolveRunComplete = true;
      isFinalPushSolved = false;
    }
  }
  if (!start_run)
  {
    mouse_sensor_obj->triggerSensors();
    start_run = mouse_sensor_obj->doStartupCheck();
    if (start_run)
    {
      angle_monitor_obj->initialize();
      mouse_sensor_obj->freezeDistance();
      // delay(DELAY_START_UP);
      mouse_motor_obj->calibrate_minimum_speed();
      while (true)
      {
        angle_monitor_obj->loop();
        mouse_motor_obj->turnToAngle(0);
        if (mouse_motor_obj->getTurnError() == 0)
        {
          break;
        }
      }
      if (maze_solver_obj->isTargetReached() && !is_learning_run)
      {
        micro_mouse_obj->resetToEast();
        maze_solver_obj->resetMouseToInitialPosition();
        maze_solver_obj->solveUsingLearning(0, 0);
        maze_solver_obj->resetMouseToInitialPosition();
        run_complete = false;
        isFinalPushSolved = false;
      }
      start_run = true;
    }
    else
    {
      // Serial.println("Returning");
      return;
    }
  }
  if (!stub)
  {
    mouse_sensor_obj->triggerSensors();
    angle_monitor_obj->loop();
  }
  /*
  mouse_motor_obj->turnToAngle(90);
  if (!is_motor_running) {
    mouse_motor_obj->stopBeforeDistance(35);
  }
  if (mouse_motor_obj->getStopBeforeDistanceError() == 0) {
    is_motor_running = true;
    if (counter == 0) {
      turnFlag = true;
      counter++;
    }
  }
  if (turnFlag) {
    mouse_motor_obj->turnToAngle(270);
    if (mouse_motor_obj->getTurnError() == 0) {
      is_motor_running = false;
    }
  }*/
  if (is_learning_run)
  {
    Serial.println("is_learning_run....");
    if (check_next_step)
    {
      Serial.println("check_next_step....");
      if (!stub)
      {
        mouse_sensor_obj->triggerSensors();
        micro_mouse_obj->updateMemoryData();
        // delay(50);
      }
      maze_solver_obj->solveMaze();
      check_next_step = false;
    }
    else if (is_back_tracking)
    {
      // do backtracking logic
      Serial.println("is_back_tracking....");
      maze_solver_obj->backtrack();
      is_back_tracking = false;
    }
  }
  /*
  if (!is_motor_running)
  {
    mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
    micro_mouse_obj->move_forward(1);
    isMouseMovingStraight = true;
    is_motor_running = true;
  }
  if (is_motor_running)
  {
    is_motor_running = mouse_motor_obj->loopStraight();
    if (is_motor_running == false)
    {
      delay(100);
    }
  }
  */
  //  mouse_motor_obj->turnToAngle(0);
  //  if (!is_motor_running) {
  //    if (!turn_stack.empty()) {
  //      turn_value = turn_stack.front();
  //      turn_stack.pop();
  //    }
  //    is_motor_running = true;
  //  }
  //  if (is_motor_running) {
  //    mouse_motor_obj->turnToAngle(turn_value);
  //    if (mouse_motor_obj->getTurnError() == 0) {
  //      is_motor_running = false;
  //      delay(100);
  //    }
  //  }
  if (!is_motor_running && !micro_mouse_obj->isPathEmpty())
  {
    stopInterrupt = false;
    float StraightCount = 0;
    MovementDirection current_path = micro_mouse_obj->getNextPathToTravel();
    switch (current_path)
    {
    case MovementDirection::STRAIGHT:
      mouse_motor_obj->resetMotor();
      mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      turnFlag = false;
      if (mouse_sensor_obj->getCenterDistance() < CENTER_STOP_CHECKING_DISTANCE)
      {
        isMouseMovingStraight = false;
        isMouseMovingStraightToStop = true;
      }
      else
      {
        if (isLastDistanceAUtrun)
        {
          StraightCount = 0.92;
        }
        else
        {
          StraightCount = 1;
        }
        //StraightCount += micro_mouse_obj->getNextNStraightPath();
        micro_mouse_obj->move_forward(StraightCount);
        // if (isLastDistanceAUtrun) {
        //   micro_mouse_obj->move_forward(0.92);
        // } else {
        //   micro_mouse_obj->move_forward(1);
        // }
        isMouseMovingStraight = true;
        isMouseMovingStraightToStop = false;
      }
      isLastDistanceAUtrun = false;

      // isMouseMovingRight = false;
      // isMouseMovingLeft = false;
      break;
    case MovementDirection::LEFT:
      mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      micro_mouse_obj->turn_left();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUtrun = false;
      // isMou.........seMovingRight = false;
      // isMouseMovingLeft = true;
      break;
    case MovementDirection::RIGHT:
      mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      micro_mouse_obj->turn_right();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUtrun = false;
      // isMouseMovingRight = true;
      // isMouseMovingLeft = false;
      break;
    case MovementDirection::REVERSE:
      mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      micro_mouse_obj->make_u_turn();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUtrun = true;
      // isMouseMovingStraight = false;
      // isMouseMovingRight = false;
      // isMouseMovingLeft = true;
      break;
    case MovementDirection::DO_NOTHING:
      break;
    }
    is_motor_running = true;
  }
  // if (stub)
  // {
  //   mouse_motor_obj->loop(false, false);
  // }
  // else
  // {
  //   bool leftBalance = mouse_sensor_obj->isLeftForzenDistanceBalanced();
  //   bool rightBalance = mouse_sensor_obj->isRightForzenDistanceBalanced();
  //   bool isCenterBlock = mouse_sensor_obj->isCenterObstrucleStopping();
  //   if (isCenterBlock && isMouseMovingStraight) {
  //     mouse_motor_obj->loop_stop();
  //   }
  //   else if (isMouseMovingStraight) {
  //     mouse_motor_obj->loop(leftBalance, rightBalance);
  //   } else {
  //     mouse_motor_obj->loop(false, false);
  //   }
  if (isMouseMovingStraight)
  {
    mouse_motor_obj->loopStraight();
  }
  else if (isMouseMovingStraightToStop)
  {
    mouse_motor_obj->stopBeforeDistance(STOP_BEFORE_DISTANCE);
  }
  else if (turnFlag)
  {
    mouse_motor_obj->turnToAngle(mouse_motor_obj->getDesiredAngle());
    // delay(50);
  }

  //}

  // //stub to check code
  if (stub)
  {
    if (isMouseMovingStraight)
      mouse_motor_obj->clearError(-mouse_motor_obj->getLeftError(), -mouse_motor_obj->getRightError());
    else if (turnFlag)
      angle_monitor_obj->clearAngleError(mouse_motor_obj->getDesiredAngle());
    // check how motor behaves
    // if (mouse_motor_obj->getLeftError() < 0) {
    //   mouse_motor_obj->updatePositionLeftMotor(true);
    // } else {
    //   mouse_motor_obj->updatePositionLeftMotor(false);
    // }
    // if (mouse_motor_obj->getRightError() < 0) {
    //   mouse_motor_obj->updatePositionRightMotor(true);
    // } else {
    //   mouse_motor_obj->updatePositionRightMotor(false);
    // }
  }

  if (is_motor_running)
  {
    // do motor
    // check if its required or solve it main loop
    bool error_to_fix = false;
    if (isMouseMovingStraight)
    {
      Serial.println(" isMouseMovingStraight: ");
      error_to_fix = mouse_motor_obj->getError();
    }
    else if (turnFlag)
    {
      Serial.println(" turnFlag: ");
      error_to_fix = (mouse_motor_obj->getTurnError() == 0);
    }
    else if (isMouseMovingStraightToStop)
    {
      Serial.println(" isMouseMovingStraightToStop: ");
      error_to_fix = (mouse_motor_obj->getStopBeforeDistanceError() == 0);
    }
    if (error_to_fix)
    {
      Serial.println("Error Cleared: ");
      stopInterrupt = true;
      mouse_motor_obj->resetMotor();
      is_motor_running = false;
      if (micro_mouse_obj->isPathEmpty())
      {
        is_back_tracking = maze_solver_obj->isBackTrackingStage();
        check_next_step = !is_back_tracking;
      }
    }
  }
  if (micro_mouse_obj->isPathEmpty() && mouse_motor_obj->getError())
  {
    run_complete = maze_solver_obj->isTargetReached();
    if (run_complete && !isFinalPushSolved)
    {
      micro_mouse_obj->addFinishMove();
      isFinalPushSolved = true;
    }
  }
  if (!is_learning_run && micro_mouse_obj->isPathEmpty())
  {
    Serial.println("Solving Run::Entered inside finish...");
    if (!isFinalPushSolved)
    {
      Serial.println("Solving Run::Adding Finish Push...");
      micro_mouse_obj->addFinishMove();
      isFinalPushSolved = true;
    }
  }
  if (!is_learning_run && micro_mouse_obj->isPathEmpty() && isFinalPushSolved)
  {
    Serial.println("Solving Run::Run Complete");
    mouse_motor_obj->loop_stop();
    start_run = false;
    check_next_step = false;
    micro_mouse_obj->resetToEast();
  }
  if (run_complete && micro_mouse_obj->isPathEmpty())
  {
    Serial.println("Run Complete");
    // reset on run complete
    isMazeAtleastSolvedOnce = true;
    mouse_motor_obj->loop_stop();
    start_run = false;
    check_next_step = false;
    micro_mouse_obj->resetToEast();
  }
  if (stub)
  {
    delay(100);
  }
}

void readEncoderLeftMotor()
{
  if (!start_run)
  {
    return;
  }
  if (stopInterrupt)
  {
    return;
  }
  int b = digitalRead(LEFT_MOTOR_ENCODER_GREEN);
  if (b > 0)
  {
    mouse_motor_obj->updatePositionLeftMotor(true);
    if (isMouseMovingStraight)
    {
      mouse_motor_obj->updatePositionRightMotor(true);
    }
    //  if (isMouseMovingRight) {
    //    mouse_motor_obj->updatePositionRightMotor(false);
    //  }
    //  if (isMouseMovingLeft) {
    //    mouse_motor_obj->updatePositionRightMotor(true);
    //  }
  }
  else
  {
    mouse_motor_obj->updatePositionLeftMotor(false);
    if (isMouseMovingStraight)
    {
      mouse_motor_obj->updatePositionRightMotor(false);
    }
    //  if (isMouseMovingRight) {
    //    mouse_motor_obj->updatePositionRightMotor(true);
    //  }
    //  if (isMouseMovingLeft) {
    //    mouse_motor_obj->updatePositionRightMotor(false);
    //  }
  }
}

void readEncoderRightMotor()
{
  if (!start_run)
  {
    return;
  }
  if (isMouseMovingStraight)
  {
    return;
  }
  int b = digitalRead(RIGHT_MOTOR_ENCODER_GREEN);
  if (b > 0)
  {
    mouse_motor_obj->updatePositionRightMotor(true);
  }
  else
  {
    mouse_motor_obj->updatePositionRightMotor(false);
  }
}

void changeSolvingMode()
{
  is_learning_run = !is_learning_run;
  if (!is_learning_run)
  {
    isSolveRunComplete = false;
  }
  if (is_learning_run && isMazeAtleastSolvedOnce)
  {
    micro_mouse_obj->clearSolutionRunQueue();
    start_run = false;
  }
  Serial.print("Interrupt HIT....");
  Serial.print(is_learning_run);
  Serial.println();
}

void dmpDataReadySet()
{
  angle_monitor_obj->dmpDataReady();
}