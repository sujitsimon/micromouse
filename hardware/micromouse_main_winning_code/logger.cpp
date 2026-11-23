#include "logger.h"

Logger *Logger::m_logger_instance = nullptr;

Logger::Logger(bool stub, bool bt_logs) : m_stub(stub), m_bt_stub(bt_logs) {

}

Logger::~Logger() {
  //do nothing
}

void Logger::createBTInstance() {
  bluetooth = new SoftwareSerial(RX_PIN, TX_PIN);
  bluetooth->begin(9600);
}

template <typename T>
void Logger::print(T logstring, bool skip_bt) {
  if (m_stub) {
  Serial.print(logstring);
    if ((bluetooth != nullptr) && m_bt_stub && !skip_bt)
      bluetooth->print(logstring);
  }
}
template void Logger::print<char const*>(char const* logstring, bool skip_bt);
template void Logger::print<int>(int logstring, bool skip_bt);
template void Logger::print<float>(float logstring, bool skip_bt);
template void Logger::print<double>(double logstring, bool skip_bt);
template void Logger::print<bool>(bool logstring, bool skip_bt);
template void Logger::print<unsigned char>(unsigned char logstring, bool skip_bt);
template void Logger::print<unsigned int>(unsigned int logstring, bool skip_bt);
template void Logger::print<__FlashStringHelper const*>(__FlashStringHelper const* logstring, bool skip_bt);
template void Logger::print<unsigned short>(unsigned short logstring, bool skip_bt);
template void Logger::print<long>(long logstring, bool skip_bt);
template void Logger::print<short>(short logstring, bool skip_bt);
//template void Logger::print<SOLVING_RUN>(SOLVING_RUN);

template <typename T>
void Logger::println(T logstring, bool skip_bt) {
  if (m_stub) {
    Serial.println(logstring);
    if ((bluetooth != nullptr) && m_bt_stub && !skip_bt)
      bluetooth->println(logstring);
    }
}
template void Logger::println<char const*>(char const* logstring, bool skip_bt);
template void Logger::println<int>(int logstring, bool skip_bt);
template void Logger::println<float>(float logstring, bool skip_bt);
template void Logger::println<double>(double logstring, bool skip_bt);
template void Logger::println<bool>(bool logstring, bool skip_bt);
template void Logger::println<unsigned char>(unsigned char logstring, bool skip_bt);
template void Logger::println<unsigned int>(unsigned int logstring, bool skip_bt);
template void Logger::println<__FlashStringHelper const*>(__FlashStringHelper const* logstring, bool skip_bt);
template void Logger::println<unsigned short>(unsigned short logstring, bool skip_bt);
template void Logger::println<long>(long logstring, bool skip_bt);
template void Logger::println<short>(short logstring, bool skip_bt);
//template void Logger::println<std::string>(std::string logstring, bool);


void Logger::println() {
  if (m_stub) {
    Serial.println();
    if ((bluetooth != nullptr) && m_bt_stub)
      bluetooth->println();
  }
}