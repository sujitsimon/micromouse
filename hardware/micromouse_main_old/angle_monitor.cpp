#include "angle_monitor.h"

AngleMonitor *AngleMonitor::m_angle_monitor = nullptr;

AngleMonitor::AngleMonitor(Logger *loggerInstance) : m_logger(loggerInstance)
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
  //m_logger->println("Testing device connections...");
  m_logger->println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  devStatus = accelgyro.dmpInitialize();
  calib = new MPU6050Calibration(accelgyro, m_logger);
  calib->updateOffset(ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset);
  accelgyro.setXAccelOffset(ax_offset);
  accelgyro.setYAccelOffset(ay_offset);
  accelgyro.setZAccelOffset(az_offset);
  accelgyro.setXGyroOffset(gx_offset);
  accelgyro.setYGyroOffset(gy_offset);
  accelgyro.setZGyroOffset(gz_offset);
  if (devStatus == 0) {
    accelgyro.CalibrateAccel(10);
    accelgyro.CalibrateGyro(30);
    accelgyro.PrintActiveOffsets();
    accelgyro.setDMPEnabled(true);
    mpuIntStatus = accelgyro.getIntStatus();
    dmpReady = true;
    packetSize = accelgyro.dmpGetFIFOPacketSize();
  }
  else {
        m_logger->print(F("DMP Initialization failed (code "));
        m_logger->print(devStatus);
        m_logger->println(F(")"));
    }

}

void AngleMonitor::updateOffset(float offset) {
  this->offset = offset;
}

float AngleMonitor::getMouseAngle()
{
  return yaw - offset;
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
    m_logger->print("ypr:\t");
    m_logger->println(yaw);
  }
}

void AngleMonitor::printOffset()
{
  accelgyro.PrintActiveOffsets();
}

void AngleMonitor::calibrateOffset() {
  calib->Calibrate();
  calib->storeOffset();
}