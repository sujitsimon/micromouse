#ifndef ANGLE_MONITOR_H
#define ANGLE_MONITOR_H

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "Arduino.h"

class AngleMonitor
{
public:
  void initialize();
  void loop();
  float getMouseAngle();
  AngleMonitor();
  ~AngleMonitor();
  static AngleMonitor *getInstance()
  {
    if (m_angle_monitor == nullptr)
    {
      m_angle_monitor = new AngleMonitor();
    }
    return m_angle_monitor;
  }
  void dmpDataReady() {
    mpuInterrupt = true;
  }
  void clearAngleError(int angle);

private:
  bool dmpReady = false;
  uint8_t mpuIntStatus;
  uint8_t devStatus;
  uint16_t packetSize;
  uint16_t fifoCount;
  uint8_t fifoBuffer[64];
  MPU6050 accelgyro;
  Quaternion q;
  VectorInt16 aa;
  VectorInt16 aaReal;
  VectorInt16 aaWorld;
  VectorFloat gravity;
  float euler[3];
  float ypr[3];
  static AngleMonitor *m_angle_monitor;
  uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
  volatile bool mpuInterrupt = false;
  float yaw;
};

#endif