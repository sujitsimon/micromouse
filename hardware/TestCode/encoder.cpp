#include "encoder.h"

Encoder::Encoder(int pwm_pin, int motor_p, int motor_n, PID_Control motor_pid, int max_power, Logger *loggerInstance) : 
  PWM_PIN(pwm_pin),
  MOTOR_P(motor_p),
  MOTOR_N(motor_n),
  KP(motor_pid.kp),
  KI(motor_pid.ki),
  KD(motor_pid.kd),
  max_power(max_power),
  m_logger(loggerInstance)
{
  pinMode(MOTOR_P, OUTPUT);
  pinMode(MOTOR_N, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
}

void Encoder::update_target(float Target, bool use_cell)
{
  if (use_cell)
  {
    target += static_cast<int>(ceil(Target * ONE_CELL));
  }
  else
  {
    target += static_cast<int>(Target);
  }
}

int Encoder::get_target()
{
  return target;
}

void Encoder::setMotor(MotorDirection direction, int speed)
{
  analogWrite(PWM_PIN, speed);
  if (direction == MotorDirection::MOTOR_FORWARD)
  {
    digitalWrite(MOTOR_P, HIGH);
    digitalWrite(MOTOR_N, LOW);
  }
  else if (direction == MotorDirection::MOTOR_REVERSE)
  {
    digitalWrite(MOTOR_P, LOW);
    digitalWrite(MOTOR_N, HIGH);
  }
  else if (direction == MotorDirection::MOTOR_STOP)
  {
    digitalWrite(MOTOR_P, LOW);
    digitalWrite(MOTOR_N, LOW);
  }
}

int Encoder::calculateError()
{
  long currT = micros();
  float deltaT = ((float)(currT - previous_timestamp)) / (1.0e6);
  previous_timestamp = currT;

  int error = position - target;
  float dedt = (error - previous_err) / (deltaT);
  float error_integral = error_integral + error * deltaT;
  int u = KP * error + KD * dedt + KI * error_integral;
  pwmVal = fabs(u);

  if (pwmVal > max_power)
  {
    pwmVal = max_power;
  }
  if (pwmVal > ERROR_LIMIT && pwmVal < PWM_LIMIT)
  {
    pwmVal = PWM_LIMIT;
  }
  if (pwmVal < -ERROR_LIMIT && pwmVal > -PWM_LIMIT)
  {
    pwmVal = -PWM_LIMIT;
  }
  direction = MotorDirection::MOTOR_FORWARD;
  if (u < 0)
  {
    direction = MotorDirection::MOTOR_REVERSE;
  }
  previous_err = error;
  return error;
}

void Encoder::setPosition(int Position)
{
  position = Position;
  lastKnownPos = Position;
}

void Encoder::stopMotor()
{
  pwmVal = 0;
  previous_err = 0;
  target = position;
  setMotor(MotorDirection::MOTOR_STOP, pwmVal);
}

void Encoder::resetLastKnownPos()
{
  lastKnownPos = position;
}

void Encoder::resetAll()
{
  target = 0;
  position = 0;
  pwmVal = 0;
  previous_err = 0;
}

float Encoder::getDistanceTravelled()
{
  return (lastKnownPos - position) * WHEEL_DISTANCE;
}

int Encoder::getPosition()
{
  return position;
}

int Encoder::getError()
{
  return previous_err;
}

void Encoder::setMaxPower(int power)
{
  max_power = power;
}

int Encoder::getSpeed()
{
  return pwmVal;
}

MotorDirection Encoder::getDirection()
{
  return direction;
}

void Encoder::updateMinimumSpeed(int speed)
{
  PWM_LIMIT = speed;
}