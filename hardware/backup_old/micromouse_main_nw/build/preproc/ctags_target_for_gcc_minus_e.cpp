# 1 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino"
# 1 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino"

# 3 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 4 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 5 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 6 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 7 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 8 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 9 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2
# 10 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino" 2


# 11 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino"
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
const uint8_t X_COORDINATE_MIN = 7; // x starts from zero
const uint8_t Y_COORDINATE_MIN = 7; // y starts form zero
const uint8_t X_COORDINATE_MAX = 8; // x ends at 15
const uint8_t Y_COORDINATE_MAX = 8; // y ends at 15

// Destination
// const uint8_t X_COORDINATE_MIN = 2; //x
// const uint8_t Y_COORDINATE_MIN = 2; //y
// const uint8_t X_COORDINATE_MAX = 3; //x
// const uint8_t Y_COORDINATE_MAX = 3; //y

// Destination
// const uint8_t X_COORDINATE_MIN = 1; //x
// const uint8_t Y_COORDINATE_MIN = 1; //y
// const uint8_t X_COORDINATE_MAX = 2; //x
// const uint8_t Y_COORDINATE_MAX = 2; //y

// Sensor Setup
const int LEFT_TRIGGER = 19;
const int CENTER_TRIGGER = 20;
const int RIGHT_TRIGGER = 21;
const int LEFT_ECHO = ((0xC0 + 1) + 1);
const int RIGHT_ECHO = 0xC0;
const int CENTER_ECHO = (0xC0 + 1);

// Motor Setup
const int LEFT_MOTOR_P = 27;
const int LEFT_MOTOR_N = 28;
const int RIGHT_MOTOR_P = 30;
const int RIGHT_MOTOR_N = 29;
const int LEFT_MOTOR_PWM = ((((((0xC0 + 1) + 1) + 1) + 1) + 1) + 1);
const int RIGHT_MOTOR_PWM = (((((((0xC0 + 1) + 1) + 1) + 1) + 1) + 1) + 1);
const int LEFT_MOTOR_ENCODER_YELLOW = (((((0xC0 + 1) + 1) + 1) + 1) + 1);
const int LEFT_MOTOR_ENCODER_GREEN = ((((0xC0 + 1) + 1) + 1) + 1);
const int RIGHT_MOTOR_ENCODER_YELLOW = (((((((((0xC0 + 1) + 1) + 1) + 1) + 1) + 1) + 1) + 1) + 1); // PA7
const int RIGHT_MOTOR_ENCODER_GREEN = ((((((((0xC0 + 1) + 1) + 1) + 1) + 1) + 1) + 1) + 1); // PA6

const int STAND_BY_MOTOR_DRIVER = 26;

// LED Wall
const int LEFT_WALL_LED = 15;
const int CENTER_WALL_LED = 10;
const int RIGHT_WALL_LED = 9;
const int BALANCE_LEFT_RIGHT_WALL_LED = 24;

const int ALGORITHM_CHANGE = 8;
const int ALGORITHM_CHANGE_LED = 31;

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
bool isLastDistanceAUturn = false;
bool isLastDistanceALefturn = false;
bool isLastDistanceARightturn = false;
bool isMazeAtleastSolvedOnce = false;
bool isFinalPushSolved = false;
bool error_to_fix = false;
bool sensorCalibrationDone = false;

int DELAY_START_UP = 3500;
int SENSOR_CALIBRATION_DELAY = 7000;
int LEARNING_POWER_MAX = 100;
int SOLVING_POWER = 255;
int CENTER_STOP_CHECKING_DISTANCE = 250;
int STOP_BEFORE_DISTANCE = 30;
int SOLVING_STOP_BEFORE_DISTANCE = 40;
// Gyroscope Sensor
const int GYROSCOPE_INTERRUPT_PIN = 25;
// int LEFT_DISTANCE_BEFORE_RIGHT_TURN = 0;
// int RIGHT_DISTANCE_BEFORE_LEFT_TURN = 0;

// std::queue<int> turn_stack;
// int turn_value;

Logger *loggerInstance = Logger::getInstance(1);

MouseSensor *mouse_sensor_obj = MouseSensor::getInstance(LEFT_TRIGGER, LEFT_ECHO, RIGHT_TRIGGER, RIGHT_ECHO, CENTER_TRIGGER, CENTER_ECHO, LEFT_WALL_LED, RIGHT_WALL_LED, CENTER_WALL_LED, BALANCE_LEFT_RIGHT_WALL_LED, loggerInstance);

AngleMonitor *angle_monitor_obj = AngleMonitor::getInstance(loggerInstance);
// //loggerInstance->println("mouse_sensor_obj init COMPLETE....");
MazeMemory *maze_memory_obj = MazeMemory::getInstance(MAZE_DIMENSION_X, MAZE_DIMENSION_Y, X_COORDINATE_MIN, Y_COORDINATE_MIN, X_COORDINATE_MAX, Y_COORDINATE_MAX, stub, loggerInstance);
// //loggerInstance->println("maze_memory_obj init COMPLETE....");
MouseMotor *mouse_motor_obj = MouseMotor::getInstance(LEFT_MOTOR_P, LEFT_MOTOR_N, RIGHT_MOTOR_P, RIGHT_MOTOR_N, LEFT_MOTOR_PWM, RIGHT_MOTOR_PWM, angle_monitor_obj, mouse_sensor_obj, loggerInstance);
// //loggerInstance->println("mouse_motor_obj init COMPLETE....");
MicroMouse *micro_mouse_obj = MicroMouse::getInstance(START_X, START_Y, mouse_sensor_obj, maze_memory_obj, mouse_motor_obj, loggerInstance, MAZE_DIMENSION_X, MAZE_DIMENSION_Y);
// //loggerInstance->println("micro_mouse_obj init COMPLETE....");
MazeSolver *maze_solver_obj = new MazeSolver(START_X, START_Y, MAZE_DIMENSION_X, micro_mouse_obj, maze_memory_obj, loggerInstance);
// //loggerInstance->println("maze_solver_obj init COMPLETE....");

void setup()
{

  pinMode(LEFT_MOTOR_ENCODER_YELLOW, 0x0);
  pinMode(LEFT_MOTOR_ENCODER_GREEN, 0x0);
  pinMode(RIGHT_MOTOR_ENCODER_YELLOW, 0x0);
  pinMode(RIGHT_MOTOR_ENCODER_GREEN, 0x0);
  pinMode(ALGORITHM_CHANGE, 0x3);
  pinMode(GYROSCOPE_INTERRUPT_PIN, 0x0);
  pinMode(ALGORITHM_CHANGE_LED, 0x1);
  pinMode(STAND_BY_MOTOR_DRIVER, 0x1);
  attachInterrupt(((((((uint32_t)(ALGORITHM_CHANGE) & 0xFF) < 36) ? (PinName)(digitalPin[(uint32_t)(ALGORITHM_CHANGE) & 0xFF] | ((ALGORITHM_CHANGE) & 0x700)) : (((uint32_t)(ALGORITHM_CHANGE) & 0xC0) == 0xC0) && (((uint32_t)(ALGORITHM_CHANGE) & 0xFF) < (0xC0 + 10)) ? (PinName)(digitalPin[analogInputPin[(ALGORITHM_CHANGE) & 0x3F]] | ((ALGORITHM_CHANGE) & 0x700)) : NC) != NC) ? ALGORITHM_CHANGE : (uint32_t)NC), changeSolvingMode, 0x3);
  attachInterrupt(((((((uint32_t)(LEFT_MOTOR_ENCODER_YELLOW) & 0xFF) < 36) ? (PinName)(digitalPin[(uint32_t)(LEFT_MOTOR_ENCODER_YELLOW) & 0xFF] | ((LEFT_MOTOR_ENCODER_YELLOW) & 0x700)) : (((uint32_t)(LEFT_MOTOR_ENCODER_YELLOW) & 0xC0) == 0xC0) && (((uint32_t)(LEFT_MOTOR_ENCODER_YELLOW) & 0xFF) < (0xC0 + 10)) ? (PinName)(digitalPin[analogInputPin[(LEFT_MOTOR_ENCODER_YELLOW) & 0x3F]] | ((LEFT_MOTOR_ENCODER_YELLOW) & 0x700)) : NC) != NC) ? LEFT_MOTOR_ENCODER_YELLOW : (uint32_t)NC), readEncoderLeftMotor, 0x4);
  attachInterrupt(((((((uint32_t)(RIGHT_MOTOR_ENCODER_YELLOW) & 0xFF) < 36) ? (PinName)(digitalPin[(uint32_t)(RIGHT_MOTOR_ENCODER_YELLOW) & 0xFF] | ((RIGHT_MOTOR_ENCODER_YELLOW) & 0x700)) : (((uint32_t)(RIGHT_MOTOR_ENCODER_YELLOW) & 0xC0) == 0xC0) && (((uint32_t)(RIGHT_MOTOR_ENCODER_YELLOW) & 0xFF) < (0xC0 + 10)) ? (PinName)(digitalPin[analogInputPin[(RIGHT_MOTOR_ENCODER_YELLOW) & 0x3F]] | ((RIGHT_MOTOR_ENCODER_YELLOW) & 0x700)) : NC) != NC) ? RIGHT_MOTOR_ENCODER_YELLOW : (uint32_t)NC), readEncoderRightMotor, 0x4);
  attachInterrupt(((((((uint32_t)(GYROSCOPE_INTERRUPT_PIN) & 0xFF) < 36) ? (PinName)(digitalPin[(uint32_t)(GYROSCOPE_INTERRUPT_PIN) & 0xFF] | ((GYROSCOPE_INTERRUPT_PIN) & 0x700)) : (((uint32_t)(GYROSCOPE_INTERRUPT_PIN) & 0xC0) == 0xC0) && (((uint32_t)(GYROSCOPE_INTERRUPT_PIN) & 0xFF) < (0xC0 + 10)) ? (PinName)(digitalPin[analogInputPin[(GYROSCOPE_INTERRUPT_PIN) & 0x3F]] | ((GYROSCOPE_INTERRUPT_PIN) & 0x700)) : NC) != NC) ? GYROSCOPE_INTERRUPT_PIN : (uint32_t)NC), dmpDataReadySet, 0x4);
  digitalWrite(STAND_BY_MOTOR_DRIVER, 0x1);
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
  SerialUSB.begin(115200);
  if (stub)
  {
    while (!SerialUSB);
    // delay(5000);
  }
  loggerInstance->println("SETUP COMPLETE....");
}

void loop()
{
  // loggerInstance->print("ALGORITHM_CHANGE Pin value....");
  // loggerInstance->println(digitalRead(ALGORITHM_CHANGE));
  // loggerInstance->print("is_learning_run Pin value....");
  // loggerInstance->println(is_learning_run);
  // maze_memory_obj->printMemory();
  if (is_learning_run)
  {
    digitalWrite(ALGORITHM_CHANGE_LED, 0x0); // HIGH will turn OFF the LED
  }
  else
  {
    digitalWrite(ALGORITHM_CHANGE_LED, 0x1); // LOW will turn ON the LED
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
    mouse_sensor_obj->isLeftObstrucle();
    mouse_sensor_obj->isRightObstrucle();
    start_run = mouse_sensor_obj->doStartupCheck();
    if (start_run)
    {
      stopInterrupt = false;
      angle_monitor_obj->initialize();
      // mouse_sensor_obj->freezeDistance();
      // if (!sensorCalibrationDone) {
      //   while(GyroCalibrationCounter < 100) {
      //         angle_monitor_obj->loop();
      //         GyroCalibrationCounter++;
      //         delay(50);
      //     }
      //   sensorCalibrationDone = true;
      // }
      // delay(SENSOR_CALIBRATION_DELAY);
      mouse_motor_obj->resetMotor();
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
      if (isMazeAtleastSolvedOnce && !is_learning_run)
      {
        micro_mouse_obj->resetToEast();
        // mouse_motor_obj->updateSolvingMaxSpeed(100);
        maze_solver_obj->resetMouseToInitialPosition();
        maze_solver_obj->solveUsingLearning(0, 0);
        maze_solver_obj->resetMouseToInitialPosition();
        run_complete = false;
        isFinalPushSolved = false;
        is_motor_running = false;
      }
      start_run = true;
    }
    else
    {
      // loggerInstance->println("Returning");
      return;
    }
  }
  mouse_sensor_obj->triggerSensors();
  angle_monitor_obj->loop();
  //}
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
# 297 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino"
  if (is_learning_run)
  {
    loggerInstance->println("is_learning_run....");
    if (check_next_step)
    {
      loggerInstance->println("check_next_step....");
      if (!stub)
      {
        // mouse_sensor_obj->triggerSensors();
        micro_mouse_obj->updateMemoryData();
        // delay(50);
      }
      maze_solver_obj->solveMaze();
      check_next_step = false;
    }
    else if (is_back_tracking)
    {
      // do backtracking logic
      loggerInstance->println("is_back_tracking....");
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
# 337 "c:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micromouse_main.ino"
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
    float StraightCount = 0.0;
    MovementDirection current_path = micro_mouse_obj->getNextPathToTravel();
    switch (current_path)
    {
    case MovementDirection::STRAIGHT:
      mouse_motor_obj->resetMotor();
      // mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      turnFlag = false;
      if (mouse_sensor_obj->getCenterDistance() < CENTER_STOP_CHECKING_DISTANCE)
      {
        // Double check the center sensor Issue making sure that it does not take a false move before it
        mouse_sensor_obj->triggerCenterSensor();
        mouse_sensor_obj->updateSensorDistance();
        if (mouse_sensor_obj->getCenterDistance() < CENTER_STOP_CHECKING_DISTANCE)
        {
          isMouseMovingStraight = false;
          isMouseMovingStraightToStop = true;
          angle_monitor_obj->updateOffset(0);
        }
        else
        {
          // micro_mouse_obj->updateMemoryData();
          isMouseMovingStraight = true;
          isMouseMovingStraightToStop = false;
        }
      }
      else
      {
        if (isLastDistanceAUturn)
        {
          StraightCount = 0.93;
          // micro_mouse_obj->move_forward(0.92);
        }
        else if (isLastDistanceALefturn)
        {
          StraightCount = 0.96;
        }
        else if (isLastDistanceARightturn)
        {
          StraightCount = 0.94;
        }
        else
        {
          StraightCount = 1;
          // micro_mouse_obj->move_forward(1);
        }
        micro_mouse_obj->move_forward(StraightCount);
        isMouseMovingStraight = true;
        isMouseMovingStraightToStop = false;
      }
      isLastDistanceAUturn = false;
      isLastDistanceALefturn = false;
      isLastDistanceARightturn = false;
      // isMouseMovingRight = false;
      // isMouseMovingLeft = false;
      break;
    case MovementDirection::LEFT:
      // mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      // RIGHT_DISTANCE_BEFORE_LEFT_TURN = mouse_sensor_obj->getRightDistance();
      micro_mouse_obj->turn_left();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUturn = false;
      isLastDistanceALefturn = true;
      isLastDistanceARightturn = false;
      angle_monitor_obj->updateOffset(0);
      // isMou.........seMovingRight = false;
      // isMouseMovingLeft = true;
      break;
    case MovementDirection::RIGHT:
      // mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      // LEFT_DISTANCE_BEFORE_RIGHT_TURN = mouse_sensor_obj->getLeftDistance();
      micro_mouse_obj->turn_right();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUturn = false;
      isLastDistanceALefturn = false;
      isLastDistanceARightturn = true;
      angle_monitor_obj->updateOffset(0);
      // isMouseMovingRight = true;
      // isMouseMovingLeft = false;
      break;
    case MovementDirection::REVERSE:
      // mouse_motor_obj->setLearningPower(LEARNING_POWER_MAX);
      micro_mouse_obj->make_u_turn();
      isMouseMovingStraight = false;
      isMouseMovingStraightToStop = false;
      turnFlag = true;
      isLastDistanceAUturn = true;
      isLastDistanceALefturn = false;
      isLastDistanceARightturn = false;
      angle_monitor_obj->updateOffset(0);
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
  // if (!stub)
  // {
  if (isMouseMovingStraight)
  {
    mouse_motor_obj->loopStraight();
  }
  else if (isMouseMovingStraightToStop)
  {
    if (is_learning_run) {
      mouse_motor_obj->stopBeforeDistance(STOP_BEFORE_DISTANCE);
    } else {
      mouse_motor_obj->stopBeforeDistance(SOLVING_STOP_BEFORE_DISTANCE);
    }
  }
  else if (turnFlag)
  {
    mouse_motor_obj->turnToAngle(mouse_motor_obj->getDesiredAngle());
    // delay(50);
  }
  // }

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
    error_to_fix = false;
    if (isMouseMovingStraight)
    {
      loggerInstance->println(" isMouseMovingStraight: ");
      error_to_fix = mouse_motor_obj->getError();
    }
    else if (turnFlag)
    {
      loggerInstance->println(" turnFlag: ");
      error_to_fix = (mouse_motor_obj->getTurnError() == 0);
    }
    else if (isMouseMovingStraightToStop)
    {
      loggerInstance->println(" isMouseMovingStraightToStop: ");
      error_to_fix = (mouse_motor_obj->getStopBeforeDistanceError() == 0);
    }
    if (error_to_fix)
    {
      loggerInstance->println("Error Cleared: ");
      stopInterrupt = true;
      mouse_motor_obj->resetMotor();
      is_motor_running = false;
      if (micro_mouse_obj->isPathEmpty() && is_learning_run)
      {
        is_back_tracking = maze_solver_obj->isBackTrackingStage();
        check_next_step = !is_back_tracking;
      }
    }
  }
  if (micro_mouse_obj->isPathEmpty() && error_to_fix)
  {
    run_complete = maze_solver_obj->isTargetReached();
    if (run_complete && !isFinalPushSolved)
    {
      check_next_step = false;
      is_back_tracking = false;
      micro_mouse_obj->addFinishMove();
      isFinalPushSolved = true;
    }
  }
  if (!is_learning_run && micro_mouse_obj->isPathEmpty() && error_to_fix)
  {
    loggerInstance->println("Solving Run::Entered inside finish...");
    if (!isFinalPushSolved)
    {
      loggerInstance->println("Solving Run::Adding Finish Push...");
      micro_mouse_obj->addFinishMove();
      isFinalPushSolved = true;
    }
  }
  if (!is_learning_run && micro_mouse_obj->isPathEmpty() && isFinalPushSolved && error_to_fix)
  {
    loggerInstance->println("Solving Run::Run Complete");
    mouse_motor_obj->loop_stop();
    start_run = false;
    check_next_step = false;
    micro_mouse_obj->resetToEast();
    is_motor_running = false;
  }
  if (run_complete && micro_mouse_obj->isPathEmpty() && error_to_fix)
  {
    loggerInstance->println("Run Complete");
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
    loggerInstance->println("Switching to Solve Run");
    isSolveRunComplete = false;
  }
  if (is_learning_run && isMazeAtleastSolvedOnce)
  {
    loggerInstance->println("Solution Path Clearing");
    micro_mouse_obj->clearSolutionRunQueue();
    mouse_motor_obj->loop_stop();
    start_run = false;
    check_next_step = false;
    micro_mouse_obj->resetToEast();
    isMouseMovingStraightToStop = false;
    isMouseMovingStraight = false;
    isLastDistanceALefturn = false;
    isLastDistanceARightturn = false;
    isLastDistanceARightturn = false;
    error_to_fix = false;
    is_back_tracking = false;
  }
  loggerInstance->print("Interrupt HIT....");
  loggerInstance->print(is_learning_run);
  loggerInstance->println();
}

void dmpDataReadySet()
{
  angle_monitor_obj->dmpDataReady();
}
