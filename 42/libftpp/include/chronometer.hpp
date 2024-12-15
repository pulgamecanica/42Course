/**
 * @file chronometer.hpp
 * @brief Header file for the Chronometer class.
 */

#ifndef CHRONOMETER_HPP
#define CHRONOMETER_HPP

#include <chrono>

/**
 * @class Chronometer
 * @brief Allows measuring durations using system time.
 */
class Chronometer {
public:
  /**
   * @brief Starts or restarts the chronometer.
   */
  void start();

  /**
   * @brief Stops the chronometer and returns the elapsed time.
   * @return Elapsed time in milliseconds.
   */
  int64_t stop();

  /**
   * @brief Returns the elapsed time without stopping the chronometer.
   * @return Elapsed time in milliseconds.
   */
  int64_t elapsed() const;

private:
  bool running_ = false; ///< Indicates whether the chronometer is running.
  std::chrono::time_point<std::chrono::steady_clock> start_time_; ///< Start time.
  std::chrono::time_point<std::chrono::steady_clock> end_time_; ///< End time.
};

#endif // CHRONOMETER_HPP
