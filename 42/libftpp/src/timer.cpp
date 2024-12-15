#include "timer.hpp"

Timer::Timer(int64_t duration_ms)
  : duration_ms_(duration_ms),
    start_time_(std::chrono::steady_clock::now()) {}

void Timer::reset() {
  start_time_ = std::chrono::steady_clock::now();
}

bool Timer::hasTimedOut() const {
  auto now = std::chrono::steady_clock::now();
  int64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_).count();
  return elapsed >= duration_ms_;
}
