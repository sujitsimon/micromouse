#include "mouse_sensor.h"

MouseSensor *MouseSensor::m_mouse_sensor = nullptr;

MouseSensor::MouseSensor(uint8_t SENSOR_LEFT_TRIGGER, uint8_t SENSOR_LEFT_ECHO, uint8_t SENSOR_RIGHT_TRIGGER, uint8_t SENSOR_RIGHT_ECHO, uint8_t SENSOR_CENTER_TRIGGER, uint8_t SENSOR_CENTER_ECHO, uint8_t LEFT_WALL_LED, uint8_t RIGHT_WALL_LED, uint8_t CENTER_WALL_LED, uint8_t BALANCE_DISTANCE_LED, Logger *loggerInstance) : m_SENSOR_LEFT_TRIGGER(SENSOR_LEFT_TRIGGER),
  m_SENSOR_LEFT_ECHO(SENSOR_LEFT_ECHO),
  m_SENSOR_RIGHT_TRIGGER(SENSOR_RIGHT_TRIGGER),
  m_SENSOR_RIGHT_ECHO(SENSOR_RIGHT_ECHO),
  m_SENSOR_CENTER_TRIGGER(SENSOR_CENTER_TRIGGER),
  m_SENSOR_CENTER_ECHO(SENSOR_CENTER_ECHO),
  m_LEFT_WALL_LED(LEFT_WALL_LED),
  m_CENTER_WALL_LED(CENTER_WALL_LED),
  m_RIGHT_WALL_LED(RIGHT_WALL_LED),
  m_BALANCE_WALL_LED(BALANCE_DISTANCE_LED),
  m_logger(loggerInstance)

{
  pinMode(m_SENSOR_LEFT_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_LEFT_ECHO, INPUT);
  pinMode(m_SENSOR_RIGHT_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_RIGHT_ECHO, INPUT);
  pinMode(m_SENSOR_CENTER_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_CENTER_ECHO, INPUT);
  pinMode(m_LEFT_WALL_LED, OUTPUT);
  //#ifdef RELEASE
  //pinMode(m_RIGHT_WALL_LED, OUTPUT);
  //#endif
  pinMode(m_CENTER_WALL_LED, OUTPUT);
  pinMode(m_BALANCE_WALL_LED, OUTPUT);
  //digitalWrite(m_BALANCE_WALL_LED, HIGH);
}

double MouseSensor::calculate_distance(long timestamp)
{
  return ((timestamp / 2) * 10 / 29.1);
}

double MouseSensor::getLeftDistance()
{
  return left_distance;
}

double MouseSensor::getRightDistance()
{
  return right_distance;
}

double MouseSensor::getCenterDistance()
{
  return center_distance;
}

void MouseSensor::triggerLeftSensor()
{
  // loggerInstance->pruint8_tln("Triggering Left Sensor");
  digitalWrite(m_SENSOR_LEFT_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(m_SENSOR_LEFT_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_SENSOR_LEFT_TRIGGER, LOW);
  long timestamp = pulseIn(m_SENSOR_LEFT_ECHO, HIGH);
  left_distance = calculate_distance(timestamp);
  // if (temp_left_distance < DISTANCE_CUTOFF_RANGE) {
  //   left_distance = temp_left_distance;
  // }
  // m_logger->print("Left Sensor: ");
  // m_logger->print(left_distance);
  // m_logger->println();
}

void MouseSensor::triggerRightSensor()
{
  // loggerInstance->pruint8_tln("Triggering Right Sensor");
  digitalWrite(m_SENSOR_RIGHT_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(m_SENSOR_RIGHT_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_SENSOR_RIGHT_TRIGGER, LOW);
  long timestamp = pulseIn(m_SENSOR_RIGHT_ECHO, HIGH);
  right_distance = calculate_distance(timestamp);
  // if (temp_right_distance < DISTANCE_CUTOFF_RANGE) {
  //   right_distance = temp_right_distance;
  // }
  // m_logger->print("Right Sensor: ");
  // m_logger->print(right_distance);
  // m_logger->println();
}

void MouseSensor::triggerCenterSensor()
{
  // loggerInstance->pruint8_tln("Triggering Center Sensor");
  digitalWrite(m_SENSOR_CENTER_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(m_SENSOR_CENTER_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_SENSOR_CENTER_TRIGGER, LOW);
  long timestamp = pulseIn(m_SENSOR_CENTER_ECHO, HIGH);
  center_distance = calculate_distance(timestamp);
  // if (temp_center_distance < DISTANCE_CUTOFF_RANGE) {
  //   center_distance = temp_center_distance;
  // }
  // m_logger->print("Center Sensor: ");
  // m_logger->print(center_distance);
  // m_logger->println();
}

void MouseSensor::triggerSensors()
{
  triggerLeftSensor();
  triggerRightSensor();
  triggerCenterSensor();
  this->updateSensorDistance();
}

void MouseSensor::updateSensorDistance()
{
  // updateLeftSensorDistance();
  // updateRightSensorDistance();
  // updateCenterSensorDistance();
  digitalWrite(m_LEFT_WALL_LED, isLeftObstrucle());
  digitalWrite(m_CENTER_WALL_LED, isCenterObstrucle());
  digitalWrite(m_RIGHT_WALL_LED, isRightObstrucle());
}

bool MouseSensor::isLeftObstrucle()
{
  //triggerLeftSensor();
  m_logger->println("isLeftObstrucle()");
  bool obstrucle;
  if ((left_distance <= DISTANCE_TO_CHECK) && (left_distance != 0)) {
    m_logger->println("True based on initial Condition");
    return true;
  } else if (right_distance >= 180) {
    if (left_distance <= 100) {
      m_logger->println("Logging True based on right_distance <= 100 condition");
      return true;
    } else {
      m_logger->println("Logging False based on right_distance <= 100 condition");
      return false;
    }
  } else {
    int offsetValue = (MAXIMUM_DISTANCE_CELL - right_distance - ROBOT_WIDTH) + TOLLERENCE;
    if (left_distance < offsetValue) {
      m_logger->println("Logging True based on Offset condition");
      return true;
    } else {
      m_logger->println("Logging False based on Offset condition");
      return false;
    }
  }
}

bool MouseSensor::isRightObstrucle()
{

  //m_logger->println("isRightObstrucle()");
  bool obstrucle;
  if ((right_distance <= DISTANCE_TO_CHECK) && (right_distance != 0)) {
    //m_logger->println("True based on initial Condition");
    return true;
  } else if (left_distance >= 180) {
    if (right_distance <= 100) {
      //m_logger->println("Logging True based on right_distance <= 100 condition");
      return true;
    } else {
      //m_logger->println("Logging False based on right_distance <= 100 condition");
      return false;
    }
  } else {
    int offsetValue = (MAXIMUM_DISTANCE_CELL - left_distance - ROBOT_WIDTH) + TOLLERENCE;
    if (right_distance < offsetValue) {
      //m_logger->println("Logging True based on Offset condition");
      return true;
    } else {
      //m_logger->println("Logging False based on Offset condition");
      return false;
    }
  }
}

bool MouseSensor::isCenterObstrucle()
{
  // loggerInstance->pruint8_t("Center Distance: ");
  // loggerInstance->pruint8_t(center_distance);
  // loggerInstance->pruint8_tln();
  //triggerCenterSensor();
  return (center_distance <= DISTANCE_TO_CHECK_CENTER);
}

bool MouseSensor::doStartupCheck()
{
  triggerCenterSensor();
  triggerLeftSensor();
  triggerRightSensor();
  digitalWrite(m_CENTER_WALL_LED, isCenterObstrucle());
  // m_logger->print("left_distance - right_distance: ");
  // m_logger->print(abs(left_distance - right_distance));
  // m_logger->println();
  if (abs(left_distance - right_distance) <= 1) {
    digitalWrite(m_BALANCE_WALL_LED, HIGH);
  } else {
    digitalWrite(m_BALANCE_WALL_LED, LOW);
  }
  bool return_value = ((center_distance <= DISTANCE_TO_CHECK_STARTUP) && (center_distance != 0));
  if (return_value) {
    center_distance = 2000;
    digitalWrite(m_BALANCE_WALL_LED, LOW);
  }
  return return_value;
}

void MouseSensor::freezeDistance()
{
  triggerLeftSensor();
  frozenDistanceLeft = left_distance;
  triggerRightSensor();
  frozenDistanceRight = right_distance;
}

bool MouseSensor::isRightForzenDistanceBalanced() {
  return (left_distance < (frozenDistanceLeft));
}

bool MouseSensor::isLeftForzenDistanceBalanced() {
  return (right_distance < (frozenDistanceRight));
}

bool MouseSensor::isCenterObstrucleStopping() {
  return (center_distance <= 3) && (center_distance != 0);
}
