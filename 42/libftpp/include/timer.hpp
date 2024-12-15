/**
 * @file timer.hpp
 * @brief Header file for the Timer class.
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

/**
 * @class Timer
 * @brief Allows setting a duration and checking if it has timed out.
 */
class Timer {
public:
  /**
   * @brief Constructs a Timer with the specified duration.
   * @param duration_ms Duration in milliseconds.
   */
  explicit Timer(int64_t duration_ms);

  /**
   * @brief Resets the timer, starting from the current time.
   */
  void reset();

  /**
   * @brief Checks if the timer has timed out.
   * @return True if the timer has timed out, false otherwise.
   */
  bool hasTimedOut() const;

private:
  int64_t duration_ms_; ///< The duration in milliseconds.
  std::chrono::time_point<std::chrono::steady_clock> start_time_; ///< The start time of the timer.
};

#endif // TIMER_HPP
