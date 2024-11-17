#include "thread_safe_iostream.hpp"

thread_local std::ostringstream ThreadSafeIOStream::thread_buffer_;
thread_local std::string ThreadSafeIOStream::thread_prefix_;

ThreadSafeIOStream::ThreadSafeIOStream() {}

void ThreadSafeIOStream::setPrefix(const std::string& prefix) {
  thread_prefix_ = prefix;
}

ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
  std::lock_guard<std::mutex> lock(global_mutex_);
  if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
    std::cout << thread_prefix_ << thread_buffer_.str() << manip;
    thread_buffer_.str("");
    thread_buffer_.clear();
  } else {
    thread_buffer_ << manip;
  }
  return *this;
}
