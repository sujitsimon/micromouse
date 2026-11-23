#ifndef ACCLERATION_H
#define ACCLERATION_H

#include "mouse_pin_config.h"
#include "my_macros.h"
#include "encoder.h"
#include "logger.h"

class Accleration {
public:
    Accleration(PID_Control left_motor_pid, PID_Control right_motor_pid, int step_size, Logger *loggerInstance);

    // Gradually increase the speed of the left motor
    void accelerateLeftMotor(int target_speed);

    // Gradually increase the speed of the right motor
    void accelerateRightMotor(int target_speed);

    // Gradually decrease the speed of the left motor
    void decelerateLeftMotor(int target_speed);

    // Gradually decrease the speed of the right motor
    void decelerateRightMotor(int target_speed);

    // Gradually increase the speed of both motor
    void acclerateMotors(int left_motor_target_speed, int right_motor_target_speed);
    
    // Gradually decrease the speed of both motor
    void decelerateMotors(int left_motor_target_speed, int right_motor_target_speed);

    // Get the current speed of the left motor
    int getLeftMotorSpeed() const;

    // Get the current speed of the right motor
    int getRightMotorSpeed() const;

    // Get the instance of LeftMotor
    Encoder* getLeftMotor() const;

    // Get the instance of RightMotor
    Encoder* getRightMotor() const;

private:
    Encoder *LeftMotor;
    Encoder *RightMotor;
    int left_motor_speed;  // Current speed of the left motor
    int right_motor_speed; // Current speed of the right motor
    int step_size;         // Step size for acceleration and deceleration
};

#endif // ACCLERATION_H