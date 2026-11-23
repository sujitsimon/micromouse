#include "logger.h"

Logger *Logger::m_logger_instance = nullptr;

Logger::Logger(bool stub) : m_stub(stub) {
}

Logger::~Logger() {
  //do nothing
}

void Logger::createBTInstance() {
  bluetooth = new SoftwareSerial(RX_PIN, TX_PIN);
  bluetooth->begin(9600);
}

template <typename T>
void Logger::print(T logstring) {
  if (m_stub) {
    Serial.print(logstring);
  //   if (bluetooth != nullptr)
  //     bluetooth->print(logstring);
  }
}
template void Logger::print<char const*>(char const* logstring);
template void Logger::print<int>(int logstring);
template void Logger::print<float>(float logstring);
template void Logger::print<double>(double logstring);
template void Logger::print<bool>(bool logstring);
template void Logger::print<unsigned char>(unsigned char logstring);
template void Logger::print<unsigned int>(unsigned int logstring);
template void Logger::print<__FlashStringHelper const*>(__FlashStringHelper const* logstring);
template void Logger::print<unsigned short>(unsigned short logstring);
template void Logger::print<long>(long logstring);
template void Logger::print<short>(short logstring);

template <typename T>
void Logger::println(T logstring) {
  if (m_stub) {
    Serial.println(logstring);
    // if (bluetooth != nullptr)
    //   bluetooth->println(logstring);
    }
}
template void Logger::println<char const*>(char const* logstring);
template void Logger::println<int>(int logstring);
template void Logger::println<float>(float logstring);
template void Logger::println<double>(double logstring);
template void Logger::println<bool>(bool logstring);
template void Logger::println<unsigned char>(unsigned char logstring);
template void Logger::println<unsigned int>(unsigned int logstring);
template void Logger::println<__FlashStringHelper const*>(__FlashStringHelper const* logstring);
template void Logger::println<unsigned short>(unsigned short logstring);
template void Logger::println<long>(long logstring);
template void Logger::println<short>(short logstring);


void Logger::println() {
  if (m_stub) {
    Serial.println();
  //   if (bluetooth != nullptr)
  //     bluetooth->println();
  }
}