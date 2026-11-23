#line 1 "C:\\D_Drive\\MicroMouse\\hardware\\micromouse_main\\mouse_motor.cpp"
#include "mouse_motor.h"

MouseMotor* MouseMotor::m_mouse_motor = nullptr;


MouseMotor::MouseMotor(int LEFT_MOTOR_P, int LEFT_MOTOR_N, int RIGHT_MOTOR_P, int RIGHT_MOTOR_N, int LEFT_MOTOR_PWM_PIN, int RIGHT_MOTOR_PWM_PIN, AngleMonitor* m_anglemonitor_obj, MouseSensor* m_mouse_sensor_object) :
  m_angle_monitor(m_anglemonitor_obj),
  m_mouse_sensor(m_mouse_sensor_object)
{
  LeftMotor = new Encoder(LEFT_MOTOR_PWM_PIN, LEFT_MOTOR_P, LEFT_MOTOR_N, KP_LEFT, KI_LEFT, KD_LEFT, MAX_MOTOR_POWER_LEFT);
  RightMotor = new Encoder(RIGHT_MOTOR_PWM_PIN, RIGHT_MOTOR_P, RIGHT_MOTOR_N, KP_RIGHT, KI_RIGHT, KD_RIGHT, MAX_MOTOR_POWER_RIGHT);
}

void MouseMotor::loop(bool runLeftMotorReduced, bool runRightMotorReduced) {
  Serial.println("Right Motor Driving");
  RightMotor->calculateError(runRightMotorReduced);
  Serial.println("Left Motor Driving");
  LeftMotor->calculateError(runLeftMotorReduced);
}

void MouseMotor::loop_stop() {
  LeftMotor->stopMotor();
  RightMotor->stopMotor();
}

void MouseMotor::turnLeft() {
  int block = 1;
  //Serial.println("Left Motor");
  LeftMotor->update_target(ROTATION_TURN, false);
  //Serial.println("Right Motor");
  RightMotor->update_target(-ROTATION_TURN, false);
}

void MouseMotor::turnRight() {
  int block = 1;
  //Serial.println("Left Motor");
  LeftMotor->update_target(-ROTATION_TURN, false);
  //Serial.println("Right Motor");
  RightMotor->update_target(ROTATION_TURN, false);
}

void MouseMotor::turnReverse() {
  int block = 1;
  //Serial.println("Left Motor");
  LeftMotor->update_target((ROTATION_TURN * 2), false);
  //Serial.println("Right Motor");
  RightMotor->update_target(-(ROTATION_TURN * 2), false);
}

void MouseMotor::moveStraight(float block) {
  Serial.print("Moving Straight Block: ");
  Serial.println(block);
  //Serial.println("Left Motor");
  LeftMotor->update_target(block);
  //Serial.println("Right Motor");
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
  //Serial.print("Left Motor: ");
  return LeftMotor->getError();
}


int MouseMotor::getRightError() {
  //Serial.print("Right Motor: ");
  return RightMotor->getError();
}

bool MouseMotor::getError() {
  //return true;
  return ((LeftMotor->getError() <= 1) && (LeftMotor->getError() >= -1) && (RightMotor->getError() <= 1) && (RightMotor->getError() >= -1));
}

void MouseMotor::setLearningPower(int power) {
  LeftMotor->setMaxPower(power);
  RightMotor->setMaxPower(power);
}

void MouseMotor::updatePositionLeftMotor(bool direction) {
  //Serial.print("Left Motor: ");
  if (direction) {
    LeftMotor->setPosition(LeftMotor->getPosition() + 1);
  } else {
    LeftMotor->setPosition(LeftMotor->getPosition() - 1);
  }
}

void MouseMotor::updatePositionRightMotor(bool direction) {
  //Serial.print("Right Motor: ");
  if (direction) {
    RightMotor->setPosition(RightMotor->getPosition() + 1);
  } else {
    RightMotor->setPosition(RightMotor->getPosition() - 1);
  }
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
  Serial.print("Left Motor Speed: ");
  Serial.print(left_motor_speed);
  Serial.print(" Right Motor Speed: ");
  Serial.println(right_motor_speed);
  LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, left_motor_speed);
  RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, right_motor_speed);
}

bool MouseMotor::loopStraight() {
  Serial.println("Right Motor Driving");
  int right_motor_error = RightMotor->calculateError();
  Serial.println("Left Motor Driving");
  int left_motor_error = LeftMotor->calculateError();
  Serial.print("right_motor_error: ");
  Serial.println(right_motor_error);
  Serial.print("left_motor_error: ");
  Serial.println(left_motor_error);
  int current_angle = m_angle_monitor->getMouseAngle();
  if (current_angle > 345) {
    current_angle -= 360;
  }
  int angle_difference = desired_angle - current_angle;
  Serial.print("angle_difference: ");
  Serial.println(angle_difference);
  left_motor_speed = LeftMotor->getSpeed();
  right_motor_speed = RightMotor->getSpeed();
  Serial.print("left_motor_speed: ");
  Serial.println(left_motor_speed);
  Serial.print("right_motor_speed: ");
  Serial.println(right_motor_speed);
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
  Serial.print("Left Motor Speed Final: ");
  Serial.print(left_motor_speed);
  Serial.print(" Right Motor Speed Final: ");
  Serial.println(right_motor_speed);
  Serial.print(" left_motor_error: ");
  Serial.println(left_motor_error);
  Serial.print(" right_motor_error: ");
  Serial.println(right_motor_error);
  if (!(left_motor_error == 0)) {
    left_motor_speed += angle_difference;
  }
  if (!(right_motor_error == 0)) {
    right_motor_speed -= angle_difference;
  }
  Serial.print("Left Motor Speed Final with Angle Difference: ");
  Serial.print(left_motor_speed);
  Serial.print(" Right Motor Speed Final with Angle Difference: ");
  Serial.println(right_motor_speed);
  LeftMotor->setMotor(LeftMotor->getDirection(), left_motor_speed);
  RightMotor->setMotor(RightMotor->getDirection(), right_motor_speed);
  return !((right_motor_speed ==0) && (left_motor_speed == 0));
}


void MouseMotor::setDesiredAngle(int angle) {
  desired_angle = angle;
}

int MouseMotor::getDesiredAngle() {
  Serial.print("Getting Desired Angle: ");
  Serial.println(desired_angle);
  return desired_angle;
}

void MouseMotor::turnToAngle(int angle) {
  Serial.print("turnToAngle: ");
  Serial.println(angle);
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
  Serial.print("current_angle: ");
  Serial.println(current_angle);
  int error = (angle - current_angle);
  integral_angle += error;
  int derivative_error = (error - previous_error_angle) / ((millis() - previous_time_stamp_angle) /  1.0e6 );
  previous_time_stamp_angle = millis();
  if (error > 0) {
    //Should Spin Right Motor Reverse and Left Motor forward
    left_motor_direction = MotorDirection::MOTOR_FORWARD;
    Serial.println("Left Motor Spinning MOTOR_FORWARD");
    right_motor_direction = MotorDirection::MOTOR_REVERSE;
    Serial.println("Right Motor Spinning MOTOR_REVERSE");
  } else if (error < 0){
    left_motor_direction = MotorDirection::MOTOR_REVERSE;
    Serial.println("Left Motor Spinning MOTOR_REVERSE");
    right_motor_direction = MotorDirection::MOTOR_FORWARD;
    Serial.println("Right Motor Spinning MOTOR_FORWARD");
  }
  if (error == 0) {
    Serial.println("Stopping Motor: Angle");
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
  Serial.print("Left Motor Speed: ");
  Serial.print(left_motor_speed);
  Serial.print(" Right Motor Speed: ");
  Serial.println(right_motor_speed);
  LeftMotor->setMotor(left_motor_direction, left_motor_speed);
  RightMotor->setMotor(right_motor_direction, right_motor_speed);
  previous_error_angle = error;
}

int MouseMotor::getTurnError() {
  return previous_error_angle;
}


void MouseMotor::stopBeforeDistance(int distance_to_stop) {
  long current_distance = m_mouse_sensor->getCenterDistance();
  int distance_error = ((int)current_distance) - distance_to_stop;
  // Serial.print("distance_error: ");
  // Serial.println(distance_error);
  // Serial.print("current_distance: ");
  // Serial.println(current_distance);
  // Serial.print("current_distance (int): ");
  // Serial.println((int)current_distance);
  // Serial.print("distance_to_stop: ");
  // Serial.println(distance_to_stop);
  int current_angle = m_angle_monitor->getMouseAngle();
  // Serial.print("current_angle: ");
  // Serial.println(current_angle);
  if (current_angle > 345) {
    current_angle -= 360;
  }
  int angle_difference = desired_angle - current_angle;
  left_motor_speed = distance_error;
  right_motor_speed = distance_error;
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
  // Serial.print("Left Motor Speed: ");
  // Serial.print(left_motor_speed);
  // Serial.print(" Right Motor Speed: ");
  // Serial.println(right_motor_speed);
  if (distance_error > 0) {
    // Serial.println("Moving Forward: ");
    LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, left_motor_speed);
    RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, right_motor_speed);
  } else if (distance_error < 0) {
    // Serial.println("Moving Reverse: ");
    LeftMotor->setMotor(MotorDirection::MOTOR_REVERSE, left_motor_speed);
    RightMotor->setMotor(MotorDirection::MOTOR_REVERSE, right_motor_speed);
  }
  if (distance_error == 0) {
    // Serial.println("Moving Stop: ");
    LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
    RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  }
  stopBeforeDistanceError = distance_error;
}


int MouseMotor::getStopBeforeDistanceError() {
  return stopBeforeDistanceError;
}

void MouseMotor::calibrate_minimum_speed() {
  motor_minimum_speed_calibration_pulse = LeftMotor->getPosition();
  Serial.print("motor_minimum_speed_calibration_pulse start: ");
  Serial.println(motor_minimum_speed_calibration_pulse);
  while (abs(LeftMotor->getPosition()) <= motor_minimum_speed_calibration_pulse + 5) {
    Serial.print("LeftMotor->getPosition() loop: ");
    Serial.println(abs(LeftMotor->getPosition()));
    minimum_speed_required ++;
    LeftMotor->setMotor(MotorDirection::MOTOR_FORWARD, minimum_speed_required);
    RightMotor->setMotor(MotorDirection::MOTOR_FORWARD, minimum_speed_required);
    delay(50);
  }
  minimum_speed_required += MINIMUM_SPEED_BUFFER;
  LeftMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  RightMotor->setMotor(MotorDirection::MOTOR_STOP, 0);
  Serial.print("Left Motor Minimum Speed: ");
  Serial.print(minimum_speed_required);
  Serial.print(" Right Motor Minimum Speed: ");
  Serial.println(minimum_speed_required);
  TURNING_MIN_SPEED = minimum_speed_required;
  SOLVING_DISTANCE_FROM_WALL_MIN = minimum_speed_required;
  SOLVING_STRAIGHT_LOOP_MIN = minimum_speed_required;
  LeftMotor->updateMinimumSpeed(minimum_speed_required);
  RightMotor->updateMinimumSpeed(minimum_speed_required);
}