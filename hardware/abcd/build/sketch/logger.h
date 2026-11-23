#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\logger.h"
#ifndef LOGGER_H
#define LOGGER_H

#include "Arduino.h"
#include <SoftwareSerial.h>

const int RX_PIN = PA10;
const int TX_PIN = PA9;

class Logger {
  public:
  Logger(bool stub, bool bt_logs);
  ~Logger();
  static Logger* getInstance(bool stub, bool bt_logs)
  {
    if (m_logger_instance == nullptr)
    {
      m_logger_instance = new Logger(stub, bt_logs);
    }
    return m_logger_instance;
  }
  template <typename T>
  void print(T logstring, bool skip_bt = false);
  template <typename T>
  void println(T logstring, bool skip_bt = false);
  void println();
  void createBTInstance();
  private:
  bool m_stub = false;
  bool m_bt_stub = false;
  static Logger *m_logger_instance;
  SoftwareSerial *bluetooth = nullptr;
};

#endif