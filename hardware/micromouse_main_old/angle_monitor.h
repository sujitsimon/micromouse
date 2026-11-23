#ifndef ANGLE_MONITOR_H
#define ANGLE_MONITOR_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "Arduino.h"
#include "logger.h"
#include "calibration.h"

class AngleMonitor
{
public:
  void initialize();
  void loop();
  float getMouseAngle();
  AngleMonitor(Logger *loggerInstance);
  ~AngleMonitor();
  static AngleMonitor *getInstance(Logger *loggerInstance)
  {
    if (m_angle_monitor == nullptr)
    {
      m_angle_monitor = new AngleMonitor(loggerInstance);
    }
    return m_angle_monitor;
  }
  void dmpDataReady() {
    mpuInterrupt = true;
  }
  void clearAngleError(int angle);
  void printOffset();
  void updateOffset(float offset);
  void calibrateOffset();

private:
  bool dmpReady = false;
  uint8_t mpuIntStatus;
  uint8_t devStatus;
  uint16_t packetSize;
  uint16_t fifoCount;
  uint8_t fifoBuffer[64];
  MPU6050 accelgyro;
  MPU6050Calibration* calib = nullptr;
  Quaternion q;
  VectorFloat gravity;
  float ypr[3];
  static AngleMonitor *m_angle_monitor;
  int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
  volatile bool mpuInterrupt = false;
  float yaw;
  float offset = 0.0;
  Logger *m_logger;
};

#endif