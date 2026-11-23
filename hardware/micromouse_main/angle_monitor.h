#ifndef ANGLE_MONITOR_H
#define ANGLE_MONITOR_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "Arduino.h"
#include "logger.h"
#include "calibration.h"

#define MPU6050_ADDR 0x68

class AngleMonitor
{
public:
  void initialize();
  void loop();
  int getMouseAngle();
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
  int16_t getXDisplacement();
  int16_t getYDisplacement();
  void printDetails();

private:
  bool dmpReady = false;
  bool isInit = false;
  uint8_t mpuIntStatus;
  uint8_t devStatus;
  uint16_t packetSize;
  uint16_t fifoCount;
  uint8_t fifoBuffer[64];
  MPU6050 accelgyro;
  MPU6050Calibration* calib = nullptr;
  Quaternion q;
  VectorFloat gravity;
  VectorInt16 aa;
  VectorInt16 aaReal;
  
  float ypr[3];
  static AngleMonitor *m_angle_monitor;
  int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
  volatile bool mpuInterrupt = false;
  int yaw = 0;
  int offset = 0;
  unsigned long lastTime = 0;
  bool lastTimeFlag = false;
  Logger *m_logger;
  int16_t accelX, accelY, accelZ;
  float velocityX = 0, velocityY = 0, velocityZ = 0;
  float distanceX = 0, distanceY = 0, distanceZ = 0;
  float accelSensitivity = 16384.0;

};

#endif