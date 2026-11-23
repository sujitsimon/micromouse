#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\mouse_sensor.h"
#ifndef MOUSE_SENSOR_H
#define MOUSE_SENSOR_H

#include "Arduino.h"
#include "logger.h"
#include "mouse_pin_config.h"

const uint8_t DISTANCE_TO_CHECK = 75; //in mm
const uint8_t DISTANCE_TO_CHECK_CENTER = 115; //in mm
const uint8_t DISTANCE_TO_CHECK_STARTUP = 45; //in mm
const uint8_t MAXIMUM_DISTANCE_CELL = 180;
const uint8_t ROBOT_WIDTH = 55;
const uint8_t TOLLERENCE = 25;
const uint8_t DISTANCE_IF_ROBOT_IN_CENTER = 65;

class MouseSensor {
  public:
    MouseSensor(Logger *loggerInstance);
    double getLeftDistance();
    double getRightDistance();
    double getCenterDistance();
    void triggerLeftSensor();
    void triggerRightSensor();
    void triggerCenterSensor();
    void triggerSensors();
    void updateSensorDistance();
    void freezeDistance();
    bool isLeftForzenDistanceBalanced();
    bool isRightForzenDistanceBalanced();
    bool isLeftObstrucle();
    bool isRightObstrucle();
    bool isCenterObstrucle();
    bool doStartupCheck();
    bool isCenterObstrucleStopping();
    int getNumberOfWallsAround();
    static MouseSensor* getInstance(Logger *loggerInstance)
    {
      if (m_mouse_sensor == nullptr) {
        m_mouse_sensor = new MouseSensor(loggerInstance);
      }
      return m_mouse_sensor;
    }

  private:
    double left_distance = 990;
    double right_distance = 990;
    double center_distance = 990;
    uint8_t center_obstrucle_distance = 50;
    double calculate_distance(long timestamp);
    static MouseSensor* m_mouse_sensor;
    uint8_t frozenDistanceLeft = 50;
    uint8_t frozenDistanceRight = 50;
    Logger *m_logger;
};
#endif
