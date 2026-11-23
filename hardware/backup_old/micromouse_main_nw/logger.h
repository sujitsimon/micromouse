#ifndef LOGGER_H
#define LOGGER_H

#include "Arduino.h"


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
  private:
  bool m_stub;
  static Logger *m_logger_instance;
};

#endif