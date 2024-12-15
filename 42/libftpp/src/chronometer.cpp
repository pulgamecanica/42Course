#include "chronometer.hpp"

void Chronometer::start() {
  running_ = true;
  start_time_ = std::chrono::steady_clock::now();
}

int64_t Chronometer::stop() {
  if (running_) {
    end_time_ = std::chrono::steady_clock::now();
    running_ = false;
  }
  return elapsed();
}

int64_t Chronometer::elapsed() const {
  auto end = running_ ? std::chrono::steady_clock::now() : end_time_;
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time_).count();
}
