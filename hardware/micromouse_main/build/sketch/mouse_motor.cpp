#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\mouse_motor.cpp"
#include "mouse_motor.h"

MouseMotor* MouseMotor::m_mouse_motor = nullptr;


MouseMotor::MouseMotor(int LEFT_MOTOR_P, int LEFT_MOTOR_N, int RIGHT_MOTOR_P, int RIGHT_MOTOR_N, int LEFT_MOTOR_PWM_PIN, int RIGHT_MOTOR_PWM_PIN, AngleMonitor* m_anglemonitor_obj, MouseSensor* m_mouse_sensor_object, Logger *loggerInstance) :
  m_angle_monitor(m_anglemonitor_obj),
  m_mouse_sensor(m_mouse_sensor_object),
  m_logger(loggerInstance)
{
  LeftMotor = new Encoder(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_P, LEFT_MOTOR_N, KP_LEFT, KI_LEFT, KD_LEFT, MAX_MOTOR_POWER_LEFT, m_logger);
  RightMotor = new Encoder(RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_P, RIGHT_MOTOR_N, KP_RIGHT, KI_RIGHT, KD_RIGHT, MAX_MOTOR_POWER_RIGHT, m_logger);
}

void MouseMotor::loop(bool runLeftMotorReduced, bool runRightMotorReduced) {
  //m_logger->println("Right Motor Driving");
  RightMotor->calculateError(runRightMotorReduced);
  //m_logger->println("Left Motor Driving");
  LeftMotor->calculateError(runLeftMotorReduced);
}

void MouseMotor::loop_stop() {
  LeftMotor->stopMotor();
  RightMotor->stopMotor();
}

void MouseMotor::turnLeft() {
  int block = 1;
  ////m_logger->println("Left Motor");
  LeftMotor->update_target(ROTATION_TURN, false);
  ////m_logger->println("Right Motor");
  RightMotor->update_target(-ROTATION_TURN, false);
}

void MouseMotor::turnRight() {
  int block = 1;
  ////m_logger->println("Left Motor");
  LeftMotor->update_target(-ROTATION_TURN, false);
  ////m_logger->println("Right Motor");
  RightMotor->update_target(ROTATION_TURN, false);
}

void MouseMotor::turnReverse() {
  int block = 1;
  ////m_logger->println("Left Motor");
  LeftMotor->update_target((ROTATION_TURN * 2), false);
  ////m_logger->println("Right Motor");
  RightMotor->update_target(-(ROTATION_TURN * 2), false);
}

void MouseMotor::moveStraight(float block) {
  ////m_logger->println("Left Motor");
  LeftMotor->update_target(block);
  ////m_logger->println("Right Motor");
  RightMotor->update_target(block);
}

void MouseMotor::updateLeftMotorPosition(int position) {
  LeftMotor->setPosition(position);
}

void MouseMotor::updateRightMotorPosition(int position) {
  RightMotor->setPosition(position);
}

float MouseMotor::getDistanceTravelled() {
  return LeftMotor->getDistanceTravelled();
}

int MouseMotor::getLeftError() {
  //m_logger->print("Left Motor: ");
  return LeftMotor->getError();
}


int MouseMotor::getRightError() {
  //m_logger->print("Right Motor: ");
  return RightMotor->getError();
}

bool MouseMotor::getError() {
  //return true;
  return ((LeftMotor->getError() <= 1) && (LeftMotor->getError() >= -1) && (RightMotor->getError() <= 1) && (RightMotor->getError() >= -1));
}

void MouseMotor::setLearningPower(int power) {
  LeftMotor->setMaxPower(power);
  RightMotor->setMaxPower(power);
  TURNING_MAX_SPEED = power - 20;
  SOLVING_DISTANCE_FROM_WALL_MAX = power - 20;
  SOLVING_STRAIGHT_LOOP_MAX = power;
}

void MouseMotor::updatePositionLeftMotor(bool direction) {
  // m_logger->print("Left Motor: ");
  if (direction) {
    LeftMotor->setPosition(LeftMotor->getPosition() + 1);
  } else {
    LeftMotor->setPosition(LeftMotor->getPosition() - 1);
  }
  // m_logger->println(LeftMotor->getPosition());
}

void MouseMotor::updatePositionRightMotor(bool direction) {
  // m_logger->print("Right Motor: ");
  if (direction) {
    RightMotor->setPosition(RightMotor->getPosition() + 1);
  } else {
    RightMotor->setPosition(RightMotor->getPosition() - 1);
  }
  // m_logger->println(RightMotor->getPosition());
}

void MouseMotor::resetMotor() {
  LeftMotor->resetAll();
  RightMotor->resetAll();
}

void MouseMotor::clearError(int leftMotorError, int rightMotorError) {
  LeftMotor->setPosition(leftMotorError);
  RightMotor->setPosition(rightMotorError);
}

void MouseMotor::moveStraightGyro(int block) {
  int current_angle = m_angle_monitor->getMouseAngle();
  if (current_angle > 270) {
    current_angle -= 360;
  }
  int angle_difference = desired_angle - current_angle;
  left_motor_speed = const_speed + angle_difference;
  right_motor_speed = const_speed - angle_difference;
  //m_logger->print("Left Motor Speed: ");
  //m_logger->print(left_motor_speed);
  //m_logger->print(" Right Motor Speed: ");
  //m_logger->println(right_motor_speed);
  LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, left_motor_speed);
  RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, right_motor_speed);
}

void MouseMotor::updateSolvingMaxSpeed(int speed) {
  SOLVING_STRAIGHT_LOOP_MAX = speed;
}

bool MouseMotor::loopStraight() {
  //m_logger->println("Right Motor Driving");
  int right_motor_error = RightMotor->calculateError();
  //m_logger->println("Left Motor Driving");
  int left_motor_error = LeftMotor->calculateError();
  current_solving_percent = (solving_percent_final_value - left_motor_error)/(solving_percent_final_value) * 100;
  //m_logger->print("right_motor_error: ");
  //m_logger->println(right_motor_error);
  //m_logger->print("left_motor_error: ");
  //m_logger->println(left_motor_error);
  int leftDistance = m_mouse_sensor->getLeftDistance() / 10;
  int rightDistance = m_mouse_sensor->getRightDistance() / 10;
  if ((leftDistance + rightDistance + 5.5) < (18)) {
    m_angle_monitor->updateOffset(rightDistance - leftDistance);
  } else {
    m_angle_monitor->updateOffset(0);
  }
  int current_angle = m_angle_monitor->getMouseAngle();
  if (current_angle > 345) {
    current_angle -= 360;
  }
  int angle_difference = desired_angle - current_angle;
  //m_logger->print("angle_difference: ");
  //m_logger->println(angle_difference);
  left_motor_speed = LeftMotor->getSpeed();
  right_motor_speed = RightMotor->getSpeed();
  //m_logger->print("left_motor_speed: ");
  //m_logger->println(left_motor_speed);
  //m_logger->print("right_motor_speed: ");
  //m_logger->println(right_motor_speed);
  if (left_motor_speed > SOLVING_STRAIGHT_LOOP_MAX) {
    left_motor_speed = SOLVING_STRAIGHT_LOOP_MAX;
  }
  if ((left_motor_speed < SOLVING_STRAIGHT_LOOP_MIN) && !(left_motor_error == 0)) {
    left_motor_speed = SOLVING_STRAIGHT_LOOP_MIN;
  }
  if (right_motor_speed > SOLVING_STRAIGHT_LOOP_MAX) {
    right_motor_speed = SOLVING_STRAIGHT_LOOP_MAX;
  }
  if ((right_motor_speed < SOLVING_STRAIGHT_LOOP_MIN) && !(right_motor_error == 0)) {
    right_motor_speed = SOLVING_STRAIGHT_LOOP_MIN;
  }
  // m_logger->print("Left Motor Speed Final: ");
  // m_logger->print(left_motor_speed);
  // m_logger->print(" Right Motor Speed Final: ");
  // m_logger->println(right_motor_speed);
  // m_logger->print(" left_motor_error: ");
  // m_logger->println(left_motor_error);
  // m_logger->print(" right_motor_error: ");
  // m_logger->println(right_motor_error);
  if (!(left_motor_error == 0)) {
    left_motor_speed += angle_difference;
  }
  if (!(right_motor_error == 0)) {
    right_motor_speed -= angle_difference;
  }
  //m_logger->print("Left Motor Speed Final with Angle Difference: ");
  //m_logger->print(left_motor_speed);
  //m_logger->print(" Right Motor Speed Final with Angle Difference: ");
  //m_logger->println(right_motor_speed);
  LeftMotor->setMotor(LeftMotor->getDirection(), left_motor_speed);
  RightMotor->setMotor(RightMotor->getDirection(), right_motor_speed);
  return !((right_motor_speed ==0) && (left_motor_speed == 0));
}


void MouseMotor::setDesiredAngle(int angle) {
  desired_angle = angle;
}

int MouseMotor::getDesiredAngle() {
  // m_logger->print("Getting Desired Angle: ");
  // m_logger->println(desired_angle);
  return desired_angle;
}

void MouseMotor::setFullRotationFlag() {
  lastRotationAngle = m_angle_monitor->getMouseAngle() - TOLLERENCE_MIN_360;
  fullRotationFlag = true;
}

void MouseMotor::do360Degree() {
  int current_angle = m_angle_monitor->getMouseAngle();
  MotorDirection left_motor_direction;
  MotorDirection right_motor_direction;
  if (fullRotationFlag) {
    fullRotationStartAngle = current_angle;
    fullRotationFlag = false;
  }
  int finalAngle = fullRotationStartAngle + THREE_SIXTY_DEGREE;
  if ((current_angle < lastRotationAngle)) {
    current_angle += THREE_SIXTY_DEGREE;
  }
  lastRotationAngle = current_angle;
  if (current_angle == finalAngle) {
    //stop
    left_motor_direction = MotorDirection::MOTOR_STOP;
    right_motor_direction = MotorDirection::MOTOR_STOP;
  } else if (current_angle < finalAngle) {
    //move right
    left_motor_direction = MotorDirection::MOTOR_FORWARD;
    right_motor_direction = MotorDirection::MOTOR_REVERSE;
  } else {
    //move left
    left_motor_direction = MotorDirection::MOTOR_REVERSE;
    right_motor_direction = MotorDirection::MOTOR_FORWARD;
  }
  int error = (finalAngle - current_angle);
  integral_angle += error;
  float derivative_error = (error - previous_error_angle) / ((millis() - previous_time_stamp_angle) /  1.0e6 );
  previous_time_stamp_angle = millis();
  int error_tmp = Kp_angle*error + Ki_angle * integral_angle + Kd_angle * derivative_error;
  error_tmp = abs(error_tmp);
  left_motor_speed = error_tmp;
  right_motor_speed = error_tmp;
  if (left_motor_speed > TURNING_MAX_SPEED) {
    left_motor_speed = TURNING_MAX_SPEED;
  }
  if ((left_motor_speed < TURNING_MIN_SPEED) && !(error == 0)) {
    left_motor_speed = TURNING_MIN_SPEED;
  }
  right_motor_speed = left_motor_speed;
  LeftMotor->setMotor(left_motor_direction, left_motor_speed);
  RightMotor->setMotor(right_motor_direction, right_motor_speed);
  previous_error_angle = error;
}

void MouseMotor::turnToAngle(int angle) {
  //m_logger->print("turnToAngle: ");
  //m_logger->println(angle);
  this->setDesiredAngle(angle);
  MotorDirection left_motor_direction;
  MotorDirection right_motor_direction;
  int current_angle = m_angle_monitor->getMouseAngle();
  bool enable_flag_positive = false;
  // if (current_angle > 345) {
  //   current_angle -= 360;
  // }
  if ((current_angle <= 25) && (angle == 270)) {
    current_angle = current_angle + 360;
  } else if ((current_angle >= 225) && (angle == 0)) {
    angle = 360;
    enable_flag_positive = true;
  }
  // if ((current_angle > 345) && !skip_zero_flag) {
  //   current_angle -= 360;
  // } else if ((current_angle < 45) && skip_zero_flag_positive) {
  //   current_angle += 360;
  // }
  if ((current_angle > 345) && (angle != 270) && !(enable_flag_positive)) {
    current_angle -= 360;
  }
  if ((enable_flag_positive) && ((current_angle >= 0) && (current_angle < 45))) {
    current_angle += 360;
  }
  //m_logger->print("current_angle: ");
  //m_logger->println(current_angle);
  int error = (angle - current_angle);
  int actual_error = (angle - current_angle);
  integral_angle += error;
  int derivative_error = (error - previous_error_angle) / ((millis() - previous_time_stamp_angle) /  1.0e6 );
  previous_time_stamp_angle = millis();
  if (error > 0) {
    //Should Spin Right Motor Reverse and Left Motor forward
    left_motor_direction = MotorDirection::MOTOR_FORWARD;
    ////m_logger->println("Left Motor Spinning MOTOR_FORWARD");
    right_motor_direction = MotorDirection::MOTOR_REVERSE;
    ////m_logger->println("Right Motor Spinning MOTOR_REVERSE");
  } else if (error < 0){
    left_motor_direction = MotorDirection::MOTOR_REVERSE;
    ////m_logger->println("Left Motor Spinning MOTOR_REVERSE");
    right_motor_direction = MotorDirection::MOTOR_FORWARD;
    ////m_logger->println("Right Motor Spinning MOTOR_FORWARD");
  }
  if (error == 0) {
    //m_logger->println("Stopping Motor: Angle");
    left_motor_direction = MotorDirection::MOTOR_STOP;
    right_motor_direction = MotorDirection::MOTOR_STOP;
  }
  error = Kp_angle*error + Ki_angle * integral_angle + Kd_angle * derivative_error;
  error = abs(error);
  left_motor_speed = error;
  right_motor_speed = error;
  if (left_motor_speed > TURNING_MAX_SPEED) {
    left_motor_speed = TURNING_MAX_SPEED;
  }
  if ((left_motor_speed < TURNING_MIN_SPEED) && !(error == 0)) {
    left_motor_speed = TURNING_MIN_SPEED;
  }
  right_motor_speed = left_motor_speed;
  // if (right_motor_speed > TURNING_MAX_SPEED) {
  //   right_motor_speed = TURNING_MAX_SPEED;
  // }
  // if ((right_motor_speed < TURNING_MIN_SPEED) && !(error == 0)) {
  //   right_motor_speed = TURNING_MIN_SPEED;
  // }
  //m_logger->print("TURNING_MAX_SPEED: ");
  //m_logger->print(TURNING_MAX_SPEED);
  //m_logger->print("TURNING_MIN_SPEED: ");
  //m_logger->print(TURNING_MIN_SPEED);
  //m_logger->print("Left Motor Speed: ");
  //m_logger->print(left_motor_speed);
  //m_logger->print(" Right Motor Speed: ");
  ////m_logger->println(right_motor_speed);
  LeftMotor->setMotor(left_motor_direction, left_motor_speed);
  RightMotor->setMotor(right_motor_direction, right_motor_speed);
  previous_error_angle = actual_error;
}

bool MouseMotor::getTurnError() {
  return abs(getDesiredAngle() - m_angle_monitor->getMouseAngle()) < 2;
}


void MouseMotor::stopBeforeDistance(int distance_to_stop) {
  distance_to_stop = distance_to_stop / 10;
  int current_distance = m_mouse_sensor->getCenterDistance() / 10;
  int distance_error = ((int)current_distance) - distance_to_stop;
  current_solving_percent = (solving_percent_final_value - current_distance)/(solving_percent_final_value - distance_to_stop) * 100;
  m_logger->print("distance_error: ");
  m_logger->println(distance_error);
  // m_logger->print("current_distance: ");
  // m_logger->println(current_distance);
  // m_logger->print("current_distance (int): ");
  // m_logger->println((int)current_distance);
  // m_logger->print("distance_to_stop: ");
  // m_logger->println(distance_to_stop);
  int current_angle = m_angle_monitor->getMouseAngle();
  // m_logger->print("current_angle: ");
  //m_logger->println(current_angle);
  if (current_angle > 345) {
    current_angle -= 360;
  }
  getDesiredAngle();
  int angle_difference = desired_angle - current_angle;
  left_motor_speed = distance_error * 10;
  right_motor_speed = distance_error * 10;// 10;
  if (left_motor_speed > SOLVING_DISTANCE_FROM_WALL_MAX) {
    left_motor_speed = SOLVING_DISTANCE_FROM_WALL_MAX;
  }
  if ((left_motor_speed < SOLVING_DISTANCE_FROM_WALL_MIN) && !(distance_error == 0)) {
    left_motor_speed = SOLVING_DISTANCE_FROM_WALL_MIN;
  }
  if (right_motor_speed > SOLVING_DISTANCE_FROM_WALL_MAX) {
    right_motor_speed = SOLVING_DISTANCE_FROM_WALL_MAX;
  }
  if ((right_motor_speed < SOLVING_DISTANCE_FROM_WALL_MIN) && !(distance_error == 0)) {
    right_motor_speed = SOLVING_DISTANCE_FROM_WALL_MIN;
  }
  if (!(distance_error == 0)) {
    left_motor_speed += angle_difference;
    right_motor_speed -= angle_difference;
  }
  // m_logger->print("Left Motor Speed: ");
  // m_logger->print(left_motor_speed);
  // m_logger->print(" Right Motor Speed: ");
  // m_logger->println(right_motor_speed);
  if ((distance_error > 0) && (distance_error < 950)) {
    // //m_logger->println("Moving Forward: ");
    LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, left_motor_speed);
    RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, right_motor_speed);
  } else if ((distance_error < 0) || (distance_error > 950)) {
    m_logger->println("Moving Reverse: ");

    LeftMotor->setMotor(MotorDirection::MOTOR_REVERSE, left_motor_speed);
    RightMotor->setMotor(MotorDirection::MOTOR_REVERSE, right_motor_speed);
  }
  if (distance_error == 0) {
    // //m_logger->println("Moving Stop: ");
    LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
    RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  }
  stopBeforeDistanceError = distance_error;
}

void MouseMotor::stopBothMotor() {
  LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
}


int MouseMotor::getStopBeforeDistanceError() {
  // m_logger->print("getStopBeforeDistanceError: ");
  // m_logger->println(stopBeforeDistanceError);
  return stopBeforeDistanceError;
}

void MouseMotor::calibrate_minimum_speed() {
  minimum_speed_required = 0;
  motor_minimum_speed_calibration_pulse = LeftMotor->getPosition();
  //m_logger->print("motor_minimum_speed_calibration_pulse start: ");
  //m_logger->println(motor_minimum_speed_calibration_pulse);
  while (abs(LeftMotor->getPosition()) <= motor_minimum_speed_calibration_pulse + 5) {
    // m_logger->print("LeftMotor->getPosition() loop: ");
    // m_logger->println(abs(LeftMotor->getPosition()));
    minimum_speed_required ++;
    LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, minimum_speed_required);
    RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, minimum_speed_required);
    delay(50);
  }
  minimum_speed_required += MINIMUM_SPEED_BUFFER;
  LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  m_logger->print("Motor Minimum Speed Set as: ");
  m_logger->println(minimum_speed_required);
  TURNING_MIN_SPEED = minimum_speed_required - 3;
  SOLVING_DISTANCE_FROM_WALL_MIN = minimum_speed_required - 5;
  SOLVING_STRAIGHT_LOOP_MIN = minimum_speed_required;
  LeftMotor->updateMinimumSpeed(minimum_speed_required);
  RightMotor->updateMinimumSpeed(minimum_speed_required);
}


void MouseMotor::TurnRightOnTheGo() {
  int current_angle = m_angle_monitor->getMouseAngle();
  int desired_angle = this->getDesiredAngle();
  int difference = desired_angle - current_angle;
  float factor = float(desired_angle - current_angle) / (float)desired_angle;
  float right_wheel_arc = (2 * PI * WALL_DISTANCE) / 4;
  float left_wheel_arc = (2 * PI * (WALL_DISTANCE + DISTANCE_BETWEEN_WHEELS)) / 4;
  float ratio_ws = left_wheel_arc / right_wheel_arc;
  int leftWheelSpeed = factor * TURN_SPEED;
  int rightWheelSpeed = (int)(factor * TURN_SPEED / ratio_ws);
  if (leftWheelSpeed < minimum_speed_required) {
    leftWheelSpeed = TURNING_MIN_SPEED;
  }
  if (difference == 0) {
    LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
    RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  } else if (difference > 0) {
    LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, leftWheelSpeed);
    RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, rightWheelSpeed);
  } else if (difference < 0) {
    LeftMotor->setMotor(MotorDirection::MOTOR_REVERSE, leftWheelSpeed);
    RightMotor->setMotor(MotorDirection::MOTOR_REVERSE, rightWheelSpeed);
  }

  
}

void MouseMotor::TurnLefttOnTheGo() {
  float right_wheel_arc = (2 * PI * WALL_DISTANCE) / 4;
  float left_wheel_arc = (2 * PI * (WALL_DISTANCE + DISTANCE_BETWEEN_WHEELS)) / 4;
  float ratio_ws = left_wheel_arc / right_wheel_arc;
  int rightWheelSpeed = TURN_SPEED;
  int leftWheelSpeed = (int)(TURN_SPEED / ratio_ws);
  LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, leftWheelSpeed);
  RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, rightWheelSpeed);
}

void MouseMotor::resetSolvingPercent(int type) {
  if (type == 0) {
    //m_logger->print("getSolvingPercent:  Move Straight");
    solving_percent_final_value = LeftMotor->calculateError();
  } else if (type == 1) {
    solving_percent_final_value = m_mouse_sensor->getCenterDistance() / 10;
    //m_logger->print("getSolvingPercent:  Move Straight to Stop");
  }
  current_solving_percent = 0.0;
}

float MouseMotor::getSolvingPercent() {
  
  // m_logger->print("getSolvingPercent: ");
  // m_logger->println(current_solving_percent);
  return current_solving_percent;
}