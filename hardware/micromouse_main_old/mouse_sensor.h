#ifndef MOUSE_SENSOR_H
#define MOUSE_SENSOR_H

#include "Arduino.h"
#include "logger.h"

const uint8_t DISTANCE_TO_CHECK = 75; //in mm
const uint8_t DISTANCE_TO_CHECK_STARTUP = 40; //in mm
const uint8_t MAXIMUM_DISTANCE_CELL = 180;
const uint8_t ROBOT_WIDTH = 55;
const uint8_t TOLLERENCE = 25;
const uint8_t DISTANCE_IF_ROBOT_IN_CENTER = 65;

class MouseSensor {
  public:
    MouseSensor(uint8_t SENSOR_LEFT_TRIGGER,
                uint8_t SENSOR_LEFT_ECHO,
                uint8_t SENSOR_RIGHT_TRIGGER,
                uint8_t SENSOR_RIGHT_ECHO,
                uint8_t SENSOR_CENTER_TRIGGER,
                uint8_t SENSOR_CENTER_ECHO,
                uint8_t LEFT_WALL_LED,
                uint8_t RIGHT_WALL_LED,
                uint8_t CENTER_WALL_LED,
                uint8_t BALANCE_DISTANCE_LED,
                Logger *loggerInstance
                );
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
    static MouseSensor* getInstance(uint8_t SENSOR_LEFT_TRIGGER,
                                    uint8_t SENSOR_LEFT_ECHO,
                                    uint8_t SENSOR_RIGHT_TRIGGER,
                                    uint8_t SENSOR_RIGHT_ECHO,
                                    uint8_t SENSOR_CENTER_TRIGGER,
                                    uint8_t SENSOR_CENTER_ECHO,
                                    uint8_t LEFT_WALL_LED,
                                    uint8_t RIGHT_WALL_LED,
                                    uint8_t CENTER_WALL_LED,
                                    uint8_t BALANCE_DISTANCE_LED,
                                    Logger *loggerInstance
                                    )
    {
      if (m_mouse_sensor == nullptr) {
        m_mouse_sensor = new MouseSensor(SENSOR_LEFT_TRIGGER,
                                         SENSOR_LEFT_ECHO,
                                         SENSOR_RIGHT_TRIGGER,
                                         SENSOR_RIGHT_ECHO,
                                         SENSOR_CENTER_TRIGGER,
                                         SENSOR_CENTER_ECHO,
                                         LEFT_WALL_LED,
                                         RIGHT_WALL_LED,
                                         CENTER_WALL_LED,
                                         BALANCE_DISTANCE_LED,
                                         loggerInstance
                                         );
      }
      return m_mouse_sensor;
    }

  private:
    uint8_t m_SENSOR_LEFT_TRIGGER;
    uint8_t m_SENSOR_LEFT_ECHO;
    uint8_t m_SENSOR_RIGHT_TRIGGER;
    uint8_t m_SENSOR_RIGHT_ECHO;
    uint8_t m_SENSOR_CENTER_TRIGGER;
    uint8_t m_SENSOR_CENTER_ECHO;
    uint8_t m_LEFT_WALL_LED;
    uint8_t m_CENTER_WALL_LED;
    uint8_t m_RIGHT_WALL_LED;
    uint8_t m_BALANCE_WALL_LED;
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
