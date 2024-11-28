#include "thread.hpp"

Thread::Thread(const std::string& name, std::function<void()> func_to_execute)
  : thread_name_(name), 
    func_(std::move(func_to_execute)),
    is_running_(false) {}

void Thread::start() {
  if (!is_running_) {
    is_running_ = true;
    thread_ = std::thread([this] {
      threadSafeCout.setPrefix("[" + thread_name_ + "] ");
      func_();
    });
  }
}

void Thread::stop() {
  if (is_running_) {
    thread_.join();
    is_running_ = false;
  }
}

Thread::~Thread() {
  if (is_running_) {
    stop();
  }
}
