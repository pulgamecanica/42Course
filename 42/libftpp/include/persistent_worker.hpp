/**
 * @file persistent_worker.hpp
 * @brief Header file for the persistent worker.
 */

#ifndef PERSISTENT_WORKER_HPP
#define PERSISTENT_WORKER_HPP

#include <functional>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include "worker_pool.hpp"

/**
 * @class PersistentWorker
 * @brief Manages task names and assigns them to respective worker pools.
 *
 * The `PersistentWorker` class maps each task name to its own worker pool, allowing tasks to be added
 * or removed dynamically.
 */
class PersistentWorker {
 public:
  /**
   * @brief Constructor for PersistentWorker.
   */
  PersistentWorker() = default;

  /**
   * @brief Destructor for PersistentWorker.
   *
   * Ensures that all tasks are cleaned up (although tasks are managed by WorkerPool).
   */
  ~PersistentWorker() = default;

  /**
   * @brief Adds a new task to the worker's task pool.
   *
   * @param name The unique name of the task.
   * @param jobToExecute The function representing the task to be executed.
   */
  void addTask(const std::string& name, const std::function<void()>& jobToExecute);

  /**
   * @brief Removes a task from the worker's task pool.
   *
   * @param name The unique name of the task to remove.
   * @throws std::invalid_argument if the task is not founded.
   */
  void removeTask(const std::string& name);

 private:
  std::unordered_map<std::string, WorkerPool> task_queues_;  ///< Mapping of task names to worker pools.
};

#endif // PERSISTENT_WORKER_HPP
