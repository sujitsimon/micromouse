#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\mouse_motor.h"
#ifndef MOUSE_MOTOR_H
#define MOUSE_MOTOR_H

#include "Arduino.h"
#include "encoder.h"
#include "angle_monitor.h"
#include "mouse_sensor.h"
#include "accleration.h"
#include "logger.h"
#include <cmath>

// const int WHEEL_DIAMETER = 34; //34mm
// const int LENGTH_OF_MAZE_BOX = 1600; //160mm
// const int HEIGHT_OF_MAZE_BOX = 1600; //160mm
// const int ONE_ROTATION = 305;
// const long WHEEL_DISTANCE = WHEEL_DIAMETER * PI;
// const long ONE_CELL = LENGTH_OF_MAZE_BOX / WHEEL_DISTANCE * ONE_ROTATION;

class MouseMotor
{
public:
  MouseMotor(AngleMonitor *m_anglemonitor_obj, MouseSensor *m_mouse_sensor_object, Logger *loggerInstance);
  void turnLeft();
  void turnRight();
  void turnReverse();
  void moveStraight(float block = 1);
  void loop();
  static MouseMotor *getInstance(AngleMonitor *m_anglemonitor_obj, MouseSensor *m_mouse_sensor_object, Logger *loggerInstance)
  {
    if (m_mouse_motor == nullptr)
    {
      m_mouse_motor = new MouseMotor(m_anglemonitor_obj,
                                     m_mouse_sensor_object,
                                     loggerInstance);
    }
    return m_mouse_motor;
  }
  bool getError();
  bool updatePositionLeftMotor(bool direction);
  bool updatePositionRightMotor(bool direction);
  int getPositionLeftMotor();
  int getPositionRightMotor();
  void setLearningPower(int power);
  void setSolvingMaxSpeed(int speed);
  int getLeftError();
  int getRightError();
  void resetMotor();
  bool loopStraight();
  void setDesiredAngle(int angle);
  int  getDesiredAngle();
  void turnToAngle(int angle);
  bool getTurnError();
  void stopBeforeDistance(int distance_to_stop);
  int getStopBeforeDistanceError();
  void calibrate_minimum_speed();
  void TurnRightOnTheGo();
  void TurnLeftOnTheGo();
  void stopBothMotor();
  void learningLoopRun();
  void prepareRun();
  int getOnTheGoTurnError();
  void setNextMemoryUpdatePosition(float position);
  int getNextMemoryUpdatePosition();
  bool isFirstBlockCrossed();

private:

  Encoder *LeftMotor;
  Encoder *RightMotor;
  Accleration *m_accleration;
  AngleMonitor *m_angle_monitor;
  MouseSensor *m_mouse_sensor;
  static MouseMotor *m_mouse_motor;
  int onTheGoTurnError = 0;
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
  struct PID_Control left_motor_pid = {0.7, 0.0, 0.0};
  struct PID_Control right_motor_pid = {0.7, 0.0, 0.0};
  bool firstBlock = true;
  TURNS getTurnDirection(int currentAngle, int destAngle);
  int memoryUpdatePosition;
};

#endif
