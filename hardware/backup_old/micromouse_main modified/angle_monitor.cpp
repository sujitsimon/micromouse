#include "angle_monitor.h"

AngleMonitor *AngleMonitor::m_angle_monitor = nullptr;

AngleMonitor::AngleMonitor()
{
  // do nothing
}

AngleMonitor::~AngleMonitor()
{
  // do nothing
}

void AngleMonitor::initialize()
{
  Wire.begin();
  Wire.setClock(400000);
  accelgyro.initialize();
  //Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  devStatus = accelgyro.dmpInitialize();
  accelgyro.setXGyroOffset(220);
  accelgyro.setYGyroOffset(76);
  accelgyro.setZGyroOffset(-85);
  accelgyro.setZAccelOffset(1788);
  if (devStatus == 0) {
    accelgyro.CalibrateAccel(10);
    accelgyro.CalibrateGyro(20);
    accelgyro.PrintActiveOffsets();
    accelgyro.setDMPEnabled(true);
    mpuIntStatus = accelgyro.getIntStatus();
    dmpReady = true;
    packetSize = accelgyro.dmpGetFIFOPacketSize();
  }
  else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

float AngleMonitor::getMouseAngle()
{
  return yaw;
}

void AngleMonitor::clearAngleError(int angle) {
  yaw = angle;
}

void AngleMonitor::loop()
{
  if (!dmpReady) return;
  if (accelgyro.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    accelgyro.dmpGetQuaternion(&q, fifoBuffer);
    accelgyro.dmpGetGravity(&gravity, &q);
    accelgyro.dmpGetYawPitchRoll(ypr, &q, &gravity);
    yaw = ypr[0] * 180/M_PI;
    yaw = map(yaw, -180, 180, 0, 360) + 180;
    yaw = fmod(yaw, 360);
    Serial.print("ypr\t");
    Serial.println(yaw);
  }
}