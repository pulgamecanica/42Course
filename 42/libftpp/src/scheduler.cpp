#include "scheduler.hpp"
#include <chrono>

Scheduler::Scheduler()
  : persistent_worker_(std::make_unique<PersistentWorker>()) {}

void Scheduler::schedule_once(int64_t delay_ms, const std::function<void()>& task) {
  auto thread = std::make_unique<Thread>("OneTimeTask", [delay_ms, task]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    task();
  });
  thread->start();
  one_time_tasks_.push_back(std::move(thread));
}

void Scheduler::schedule_repeating(const std::string& name, int64_t interval_ms, const std::function<void()>& task) {
  persistent_worker_->addTask(name, [interval_ms, task]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    task();
  });
}

void Scheduler::cancel(const std::string& name) {
  persistent_worker_->removeTask(name);
}

Scheduler::~Scheduler() {
  for (auto& thread : one_time_tasks_) {
    if (thread) {
      thread->stop();
    }
  }
}
