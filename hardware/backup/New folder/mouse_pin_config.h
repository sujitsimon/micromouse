#ifndef MOUSE_PIN_CONFIG_H
#define MOUSE_PIN_CONFIG_H

#include "Arduino.h"

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

#endif //MOUSE_PIN_CONFIG_H
