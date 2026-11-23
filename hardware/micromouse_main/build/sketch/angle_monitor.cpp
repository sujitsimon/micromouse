#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\angle_monitor.cpp"
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
  // m_logger->println("Testing device connections...");
  m_logger->println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  delay(100);
  devStatus = accelgyro.dmpInitialize();
  calib = new MPU6050Calibration(accelgyro, m_logger);
  calib->readOffsetsFromFlash();
  calib->updateOffset(ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset);
  accelgyro.setXAccelOffset(ax_offset);
  accelgyro.setYAccelOffset(ay_offset);
  accelgyro.setZAccelOffset(az_offset);
  accelgyro.setXGyroOffset(gx_offset);
  accelgyro.setYGyroOffset(gy_offset);
  accelgyro.setZGyroOffset(gz_offset);
  if (devStatus == 0)
  {
    accelgyro.CalibrateAccel(10);
    accelgyro.CalibrateGyro(10);
    accelgyro.PrintActiveOffsets();
    accelgyro.setDMPEnabled(true);
    mpuIntStatus = accelgyro.getIntStatus();
    dmpReady = true;
    packetSize = accelgyro.dmpGetFIFOPacketSize();
    m_logger->println("DMP Initialization Successful...");
  }
  else
  {
    m_logger->print(F("DMP Initialization failed (code "));
    m_logger->print(devStatus);
    m_logger->println(F(")"));
  }
  isInit = true;
  lastTime = millis();
}

void AngleMonitor::updateOffset(float offset)
{
  this->offset = offset * 2;
  m_logger->print("offset:\t", true);
  m_logger->println(offset, true);
}

int AngleMonitor::getMouseAngle()
{
  return yaw - this->offset;
}

void AngleMonitor::clearAngleError(int angle)
{
  yaw = angle;
}

void AngleMonitor::loop()
{
  if (!dmpReady)
    return;
  if (accelgyro.dmpGetCurrentFIFOPacket(fifoBuffer))
  {

    // Angle Monitor
    accelgyro.dmpGetQuaternion(&q, fifoBuffer);
    accelgyro.dmpGetGravity(&gravity, &q);
    accelgyro.dmpGetYawPitchRoll(ypr, &q, &gravity);
    yaw = ypr[0] * 180 / M_PI;
    yaw = map(yaw, -180, 180, 0, 360) + 180;
    yaw = fmod(yaw, 360);
    m_logger->print("ypr:\t", true);
    m_logger->println(yaw, true);
    m_logger->print("offset:\t", true);
    m_logger->println(offset, true);

  }

  // accelgyro.getAcceleration(&accelX, &accelY, &accelZ);
  // unsigned long currentTime = millis();
  // float deltaTime = (currentTime - lastTime) / 1000.0;
  // lastTime = currentTime;

  // if (!lastTimeFlag) {
  //   lastTimeFlag = true;
  //   return;
  // }

  // float accelX_mps2 = accelX / accelSensitivity * 9.81;
  // float accelY_mps2 = accelY / accelSensitivity * 9.81;
  // float accelZ_mps2 = accelZ / accelSensitivity * 9.81;

  // if (abs(accelX_mps2) <= 0.03) accelX_mps2 = 0;
  // if (abs(accelY_mps2) <= 0.03) accelY_mps2 = 0;
  // if (abs(accelZ_mps2) <= 0.03) accelZ_mps2 = 0;

  // velocityX += accelX_mps2 * deltaTime;
  // velocityY += accelY_mps2 * deltaTime;
  // velocityZ += accelZ_mps2 * deltaTime;

  // distanceX += velocityX * deltaTime * 100;
  // distanceY += velocityY * deltaTime * 100;
  // distanceZ += velocityZ * deltaTime * 100;

  // m_logger->print("deltaTime: ");
  // m_logger->println(deltaTime);

  // m_logger->print("Velocity (X, Y, Z)");
  // m_logger->print(velocityX); m_logger->print(", ");
  // m_logger->print(velocityY); m_logger->print(", ");
  // m_logger->println(velocityZ);

  // m_logger->print("Distance (X, Y, Z) in cm: ");
  // m_logger->print(distanceX); m_logger->print(", ");
  // m_logger->print(distanceY); m_logger->print(", ");
  // m_logger->println(distanceZ);

  // m_logger->print("Received Accel (X, Y, Z) in m/s²: ");
  // m_logger->print(accelX); m_logger->print(", ");
  // m_logger->print(accelY); m_logger->print(", ");
  // m_logger->println(accelZ);

  // m_logger->print("Accel (X, Y, Z) in m/s²: ");
  // m_logger->print(accelX_mps2); m_logger->print(", ");
  // m_logger->print(accelY_mps2); m_logger->print(", ");
  // m_logger->println(accelZ_mps2);
}

void AngleMonitor::printOffset()
{
  accelgyro.PrintActiveOffsets();
}

void AngleMonitor::calibrateOffset()
{
  m_logger->println("Calibrating MPU6050");
  if (!isInit)
  {
    m_logger->println("MPU6050 not initialized");
    initialize();
  }
  calib->Calibrate();
  m_logger->println("Updating Offsets Complete");
  calib->updateOffset(ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset);
  accelgyro.setXAccelOffset(ax_offset);
  accelgyro.setYAccelOffset(ay_offset);
  accelgyro.setZAccelOffset(az_offset);
  accelgyro.setXGyroOffset(gx_offset);
  accelgyro.setYGyroOffset(gy_offset);
  accelgyro.setZGyroOffset(gz_offset);
  m_logger->println("Storing Offsets");
  calib->storeOffsets();
  m_logger->println("Calibration Complete");
}

int16_t AngleMonitor::getXDisplacement()
{
  return aaReal.x;
}

int16_t AngleMonitor::getYDisplacement()
{
  return aaReal.y;
}

void AngleMonitor::printDetails() {
    m_logger->print("ypr:\t");
    m_logger->println(yaw);
    m_logger->print("offset:\t");
    m_logger->println(offset);
    m_logger->print("Corrected Angle:\t");
    m_logger->println(getMouseAngle());
}