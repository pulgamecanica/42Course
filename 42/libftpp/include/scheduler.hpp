/**
 * @file scheduler.hpp
 * @brief Header file for the Scheduler class using PersistentWorker for repeating tasks.
 */

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <functional>
#include <string>
#include <memory>
#include "thread.hpp"
#include "persistent_worker.hpp"

/**
 * @class Scheduler
 * @brief Allows scheduling tasks to run after a delay or at regular intervals using PersistentWorker.
 */
class Scheduler {
 public:
  /**
   * @brief Constructs a Scheduler.
   */
  Scheduler();

  /**
   * @brief Schedules a task to run once after a delay.
   * @param delay_ms Delay in milliseconds before the task is executed.
   * @param task The task to execute.
   */
  void schedule_once(int64_t delay_ms, const std::function<void()>& task);

  /**
   * @brief Schedules a task to run repeatedly at fixed intervals.
   * @param name A unique name for the task.
   * @param interval_ms The interval in milliseconds between executions.
   * @param task The task to execute.
   */
  void schedule_repeating(const std::string& name, int64_t interval_ms, const std::function<void()>& task);

  /**
   * @brief Cancels a scheduled repeating task.
   * @param name The unique name of the task to cancel.
   */
  void cancel(const std::string& name);

  /**
   * @brief Destructor ensures all repeating tasks are cleaned up.
   */
  ~Scheduler();

 private:
  std::unique_ptr<PersistentWorker> persistent_worker_; ///< Worker for managing repeating tasks.
  std::vector<std::unique_ptr<Thread>> one_time_tasks_; ///< Threads for one-time tasks.
};

#endif // SCHEDULER_HPP
