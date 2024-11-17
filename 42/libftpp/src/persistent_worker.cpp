#include "persistent_worker.hpp"
#include <iostream>

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute) {
  task_queues_[name].addJob(jobToExecute);
}

void PersistentWorker::removeTask(const std::string& name) {
  auto it = task_queues_.find(name);
  if (it != task_queues_.end()) {
    task_queues_.erase(it);
  } else {
    throw std::invalid_argument("Task not found: " + name);
  }
}
