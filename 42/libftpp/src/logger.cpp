#include "logger.hpp"

Logger::Logger(std::ostream& stream)
  : stream_(&stream) {}

void Logger::log(LogLevel level, const std::string& message) {
  if (level < min_log_level_) {
    return;
  }

  std::lock_guard<std::mutex> lock(mutex_);

  *stream_ << "[" << getTimestamp() << "] "
           << "[" << levelToString(level) << "] "
           << message << std::endl;
}

void Logger::setLogLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(mutex_);
  min_log_level_ = level;
}

std::string Logger::levelToString(LogLevel level) const {
  switch (level) {
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO";
    case LogLevel::WARNING: return "WARNING";
    case LogLevel::ERROR:   return "ERROR";
    default:                return "UNKNOWN";
  }
}

std::string Logger::getTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  std::ostringstream ss;
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}
