#include "worker_pool.hpp"

WorkerPool::WorkerPool(size_t num_threads) : stop_pool_(false) {
  for (size_t i = 0; i < num_threads; ++i)
    workers_.emplace_back([this] { this->_workerLoop(); });
}

WorkerPool::~WorkerPool() {
  stop_pool_ = true;
  for (auto& worker : workers_)
    worker.join();
}

void WorkerPool::addJob(const std::function<void()>& job_to_execute) {
  job_queue_.push_back(std::make_unique<Job>(job_to_execute));
}

void WorkerPool::_workerLoop() {
  std::unique_ptr<WorkerPool::IJob> job;
  while (!stop_pool_) {
    try {
      job = job_queue_.pop_front();
    } catch (...) {
      continue ; // Continue if no jobs are founded
    }
    job->execute();
    job_queue_.push_front(std::move(job));
  }
}

WorkerPool::Job::Job(std::function<void()> func) : func_(std::move(func)) {}

void WorkerPool::Job::execute() const {
  func_();
}
