#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main_old\\encoder.h"
#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"
#include "logger.h"

//int PWM_LIMIT = 27;
const int ERROR_LIMIT = 0;
const int INCREMENTER = 100;

const float WHEEL_DIAMETER = 34; //34mm
const float LENGTH_OF_MAZE_BOX = 185; //180mm
const float HEIGHT_OF_MAZE_BOX = 185; //180mm
const float ONE_ROTATION = 202;
const float WHEEL_DISTANCE = WHEEL_DIAMETER * PI;
const float ONE_CELL = (LENGTH_OF_MAZE_BOX / WHEEL_DISTANCE) * ONE_ROTATION;

enum MotorDirection {
    MOTOR_FORWARD = 1,
    MOTOR_REVERSE = 2,
    MOTOR_STOP = 3
};

class Encoder{
  public:
    Encoder(int pwm_pin, int motor_p, int motor_n, float kp, float ki, float kd, int max_power, Logger *loggerInstance);
    void update_target(float Target, bool use_cell = true);
    void setMotor(MotorDirection direction, int speed);
    int calculateError(bool runMotorReduced = false);
    int get_target();
    void setPosition(int Position);
    int getPosition();
    int getError();
    void resetLastKnownPos();
    float getDistanceTravelled();
    void stopMotor();
    void setMaxPower(int power);
    void resetAll();
    int getSpeed();
    MotorDirection getDirection();
    void updateMinimumSpeed(int speed);

  private:
    long previous_timestamp = 0;
    float previous_err = 0;
    int target = 0;
    int position = 0;
    int lastKnownPos = 0;
    int pwmVal;
    int PWM_PIN;
    int MOTOR_P;
    int MOTOR_N;
    MotorDirection direction;
    float KP = 1.0;
    float KI = 0.0;
    float KD = 0.0;
    int max_power = 0;
    int PWM_LIMIT = 27;
    Logger *m_logger;
};

#endif
