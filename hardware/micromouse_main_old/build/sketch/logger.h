#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main_old\\logger.h"
#ifndef LOGGER_H
#define LOGGER_H

#include "Arduino.h"
#include <SoftwareSerial.h>

const int RX_PIN = PA10;
const int TX_PIN = PA9;

class Logger {
  public:
  Logger(bool stub);
  ~Logger();
  static Logger* getInstance(bool stub)
  {
    if (m_logger_instance == nullptr)
    {
      m_logger_instance = new Logger(stub);
    }
    return m_logger_instance;
  }
  template <typename T>
  void print(T logstring);
  template <typename T>
  void println(T logstring);
  void println();
  void createBTInstance();
  private:
  bool m_stub;
  static Logger *m_logger_instance;
  SoftwareSerial *bluetooth = nullptr;
};

#endif