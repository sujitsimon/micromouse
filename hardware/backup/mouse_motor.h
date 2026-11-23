#ifndef MOUSE_MOTOR_H
#define MOUSE_MOTOR_H

#include "Arduino.h"
#include "encoder.h"
#include "angle_monitor.h"
#include "mouse_sensor.h"
#include "logger.h"


const int TOLLERENCE_MIN_360 = 5;
const int ANGLE_OFFSET = 45;
const int THREE_SIXTY_DEGREE = 360;
const int WALL_DISTANCE = 65;
const int DISTANCE_BETWEEN_WHEELS = 84;
const int TURN_SPEED = 100;

const float KP_LEFT = 0.70;
const float KI_LEFT = 0.0;
const float KD_LEFT = 0.0;
const int MAX_MOTOR_POWER_LEFT = 255;

const float KP_RIGHT = 0.70;
const float KI_RIGHT = 0.0;
const float KD_RIGHT = 0.0;
const int MAX_MOTOR_POWER_RIGHT = 255;
const int ROTATION_TURN = 135;

const int MINIMUM_SPEED_BUFFER = 10;

// const int WHEEL_DIAMETER = 34; //34mm
// const int LENGTH_OF_MAZE_BOX = 1600; //160mm
// const int HEIGHT_OF_MAZE_BOX = 1600; //160mm
// const int ONE_ROTATION = 305;
// const long WHEEL_DISTANCE = WHEEL_DIAMETER * PI;
// const long ONE_CELL = LENGTH_OF_MAZE_BOX / WHEEL_DISTANCE * ONE_ROTATION;

class MouseMotor
{
public:
  MouseMotor(int LEFT_MOTOR_P, int LEFT_MOTOR_N, int RIGHT_MOTOR_P, int RIGHT_MOTOR_N, int LEFT_MOTOR_PWM_PIN, int RIGHT_MOTOR_PWM_PIN, AngleMonitor *m_anglemonitor_obj, MouseSensor *m_mouse_sensor_object, Logger *loggerInstance);
  void turnLeft();
  void turnRight();
  void turnReverse();
  void moveStraight(float block = 1);
  void moveStraightGyro(int block = 1);
  void loop(bool runLeftMotorReduced, bool runRightMotorReduced);
  void loop_stop();
  void updateLeftMotorPosition(int position);
  void updateRightMotorPosition(int position);
  static MouseMotor *getInstance(int LEFT_MOTOR_P, int LEFT_MOTOR_N, int RIGHT_MOTOR_P, int RIGHT_MOTOR_N, int LEFT_MOTOR_PWM_PIN, int RIGHT_MOTOR_PWM_PIN, AngleMonitor *m_anglemonitor_obj, MouseSensor *m_mouse_sensor_object, Logger *loggerInstance)
  {
    if (m_mouse_motor == nullptr)
    {
      m_mouse_motor = new MouseMotor(LEFT_MOTOR_P,
                                     LEFT_MOTOR_N,
                                     RIGHT_MOTOR_P,
                                     RIGHT_MOTOR_N,
                                     LEFT_MOTOR_PWM_PIN,
                                     RIGHT_MOTOR_PWM_PIN,
                                     m_anglemonitor_obj,
                                     m_mouse_sensor_object,
                                     loggerInstance);
    }
    return m_mouse_motor;
  }
  float getDistanceTravelled();
  bool getError();
  void updatePositionLeftMotor(bool direction);
  void updatePositionRightMotor(bool direction);
  void setLearningPower(int power);
  int getLeftError();
  int getRightError();
  void resetMotor();
  void clearError(int leftMotorError, int rightMotorError);
  bool loopStraight();
  void setDesiredAngle(int angle);
  void turnToAngle(int angle);
  bool getTurnError();
  void stopBeforeDistance(int distance_to_stop);
  int getStopBeforeDistanceError();
  void calibrate_minimum_speed();
  int getDesiredAngle();
  void updateSolvingMaxSpeed(int speed);
  void TurnRightOnTheGo();
  void TurnLefttOnTheGo();
  void do360Degree();
  void setFullRotationFlag();
  void resetSolvingPercent(int type);
  float getSolvingPercent();
  void stopBothMotor();

private:
  Encoder *LeftMotor;
  Encoder *RightMotor;
  AngleMonitor *m_angle_monitor;
  MouseSensor *m_mouse_sensor;
  static MouseMotor *m_mouse_motor;
  int desired_angle = 0;
  int left_motor_speed = 60;
  int right_motor_speed = 60;
  int const_speed = 80;
  int previous_error_angle = 0;
  int integral_angle = 0;
  float Kp_angle = 0.75;
  float Ki_angle = 0.0;
  float Kd_angle = 0.0;
  int motor_minimum_speed_calibration_pulse = 0;
  int stopBeforeDistanceError = 0;
  uint32_t previous_time_stamp_angle = millis();
  uint8_t minimum_speed_required = 0;
  int fullRotationStartAngle = 0;
  int lastRotationAngle = 0;
  bool fullRotationFlag = false;
  int last_power_left_motor = 0;
  int last_power_right_motor = 0;
  int TURNING_SPEED = 45;
  int TURNING_MIN_SPEED = 38;
  int TURNING_MAX_SPEED = 50;
  int tollerence_right_motor = 0;

  int SOLVING_DISTANCE_FROM_WALL_MIN = 25;
  int SOLVING_DISTANCE_FROM_WALL_MAX = 100;

  int SOLVING_STRAIGHT_LOOP_MIN = 33;
  int SOLVING_STRAIGHT_LOOP_MAX = 120;
  float current_solving_percent = 0.0;
  float solving_percent_final_value = 0.0;
  Logger *m_logger;
  void normalizeMotorPowers(int leftMotorPower, int rightMotorPower);

  // int error = 0;
};

#endif
