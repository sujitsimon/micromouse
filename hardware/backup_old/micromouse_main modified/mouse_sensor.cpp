#include "mouse_sensor.h"

MouseSensor *MouseSensor::m_mouse_sensor = nullptr;

MouseSensor::MouseSensor(uint8_t SENSOR_LEFT_TRIGGER, uint8_t SENSOR_LEFT_ECHO, uint8_t SENSOR_RIGHT_TRIGGER, uint8_t SENSOR_RIGHT_ECHO, uint8_t SENSOR_CENTER_TRIGGER, uint8_t SENSOR_CENTER_ECHO, uint8_t LEFT_WALL_LED, uint8_t RIGHT_WALL_LED, uint8_t CENTER_WALL_LED, uint8_t BALANCE_DISTANCE_LED) : m_SENSOR_LEFT_TRIGGER(SENSOR_LEFT_TRIGGER),
  m_SENSOR_LEFT_ECHO(SENSOR_LEFT_ECHO),
  m_SENSOR_RIGHT_TRIGGER(SENSOR_RIGHT_TRIGGER),
  m_SENSOR_RIGHT_ECHO(SENSOR_RIGHT_ECHO),
  m_SENSOR_CENTER_TRIGGER(SENSOR_CENTER_TRIGGER),
  m_SENSOR_CENTER_ECHO(SENSOR_CENTER_ECHO),
  m_LEFT_WALL_LED(LEFT_WALL_LED),
  m_CENTER_WALL_LED(CENTER_WALL_LED),
  m_RIGHT_WALL_LED(RIGHT_WALL_LED),
  m_BALANCE_WALL_LED(BALANCE_DISTANCE_LED)

{
  pinMode(m_SENSOR_LEFT_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_LEFT_ECHO, INPUT);
  pinMode(m_SENSOR_RIGHT_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_RIGHT_ECHO, INPUT);
  pinMode(m_SENSOR_CENTER_TRIGGER, OUTPUT);
  pinMode(m_SENSOR_CENTER_ECHO, INPUT);
  pinMode(m_LEFT_WALL_LED, OUTPUT);
  pinMode(m_RIGHT_WALL_LED, OUTPUT);
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
  // Serial.pruint8_tln("Triggering Left Sensor");
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
  Serial.print("Left Sensor: ");
  Serial.print(left_distance);
  Serial.println();
}

void MouseSensor::triggerRightSensor()
{
  // Serial.pruint8_tln("Triggering Right Sensor");
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
  Serial.print("Right Sensor: ");
  Serial.print(right_distance);
  Serial.println();
}

void MouseSensor::triggerCenterSensor()
{
  // Serial.pruint8_tln("Triggering Center Sensor");
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
  Serial.print("Center Sensor: ");
  Serial.print(center_distance);
  Serial.println();
}

void MouseSensor::triggerSensors()
{
  triggerLeftSensor();
  triggerRightSensor();
  triggerCenterSensor();
  this->updateSensorDistance();
}

void MouseSensor::triggerSensorsAverage() {
  double a[3];
  for(int i = 0; i<3; i++) {
    triggerLeftSensor();
    a[i] = left_distance;
  }
  left_distance = getAverageSensorValue(a);
  for(int i = 0; i<3; i++) {
    triggerRightSensor();
    a[i] = right_distance;
  }
  right_distance = getAverageSensorValue(a);
  for(int i = 0; i<3; i++) {
    triggerCenterSensor();
    a[i] = center_distance;
  }
  center_distance = getAverageSensorValue(a);
}

double MouseSensor::getAverageSensorValue(double a[]) {
  double diff_ab = (a[0] - a[1]);
  double diff_ac = (a[0] - a[2]);
  double diff_bc = (a[1] - a[2]);
  double min_diff = std::min({diff_ab, diff_ac, diff_bc});
  if (min_diff == diff_ab) {
        return (a[0]+ a[1]) / 2.0;
    } else if (min_diff == diff_ac) {
        return (a[0]+ a[2]) / 2.0;
    } else {
        return (a[1]+ a[2]) / 2.0;
    }
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
  return (left_distance <= DISTANCE_TO_CHECK) && (left_distance != 0);
}

bool MouseSensor::isRightObstrucle()
{
  // Serial.pruint8_t("Right Distance: ");
  // Serial.pruint8_t(right_distance);
  // Serial.pruint8_tln();
  //triggerRightSensor();
  return (right_distance <= DISTANCE_TO_CHECK) && (right_distance != 0);
}

bool MouseSensor::isCenterObstrucle()
{
  // Serial.pruint8_t("Center Distance: ");
  // Serial.pruint8_t(center_distance);
  // Serial.pruint8_tln();
  //triggerCenterSensor();
  return (center_distance <= DISTANCE_TO_CHECK);
}

bool MouseSensor::doStartupCheck()
{
  triggerCenterSensor();
  digitalWrite(m_CENTER_WALL_LED, isCenterObstrucle());
  Serial.print("left_distance - right_distance: ");
  Serial.print(abs(left_distance - right_distance));
  Serial.println();
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
