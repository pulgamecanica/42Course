/**
 * @file logger.hpp
 * @brief Header file for the Logger class.
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <fstream>
#include <mutex>
#include <string>
#include <memory>
#include <sstream>
#include <chrono>
#include <iomanip>

/**
 * @enum LogLevel
 * @brief Represents the different logging levels.
 */
enum class LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

/**
 * @class Logger
 * @brief A flexible, thread-safe logger that supports logging to any stream.
 */
class Logger {
 public:
  /**
   * @brief Constructs a Logger with a specified output stream.
   * @param stream The initial output stream.
   */
  Logger(std::ostream& stream);

  /**
   * @brief Logs a message with the specified log level.
   * @param level The log level.
   * @param message The message to log.
   */
  void log(LogLevel level, const std::string& message);

  /**
   * @brief Sets the minimum log level. Messages below this level will not be logged.
   * @param level The minimum log level.
   */
  void setLogLevel(LogLevel level);

 private:
  std::ostream* stream_; ///< Pointer to the current output stream.
  std::mutex mutex_; ///< Mutex for thread-safe logging.
  LogLevel min_log_level_ = LogLevel::DEBUG; ///< Minimum log level.

  /**
   * @brief Converts a log level to its string representation.
   * @param level The log level.
   * @return The string representation of the log level.
   */
  std::string levelToString(LogLevel level) const;

  /**
   * @brief Gets the current timestamp as a string.
   * @return The timestamp string in the format "YYYY-MM-DD HH:MM:SS".
   */
  std::string getTimestamp() const;
};

#endif // LOGGER_HPP
