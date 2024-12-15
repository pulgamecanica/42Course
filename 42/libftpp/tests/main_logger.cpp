#include <iostream>
#include <fstream>
#include "logger.hpp"
#include "thread_safe_iostream.hpp"

ThreadSafeIOStream threadSafeCout;

int main() {
  Logger logger(std::cout);

  logger.setLogLevel(LogLevel::INFO);

  logger.log(LogLevel::DEBUG, "This debug message will not be logged.");
  logger.log(LogLevel::INFO, "Application started.");
  logger.log(LogLevel::WARNING, "Low disk space.");
  logger.log(LogLevel::ERROR, "Critical error occurred.");

  std::ofstream file("log.txt", std::ios::app);
  Logger fileLogger(file);

  fileLogger.log(LogLevel::INFO, "This message is written to the log file.");

  file.close();
  return 0;
}
