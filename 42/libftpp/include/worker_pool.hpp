/**
* @file worker_pool.hpp
* @brief Provides a worker pool that manages worker threads to execute jobs.
*
* This header file contains the definition of the `WorkerPool` class, which
* manages multiple worker threads. The pool assigns jobs (wrapped in the `IJob` interface)
* to threads, which execute them concurrently.
*/

#ifndef WORKER_POOL_HPP
#define WORKER_POOL_HPP

#include <iostream>
#include <thread>
#include <functional>
#include <atomic>
#include <vector>
#include <memory>
#include "thread_safe_queue.hpp"

/**
* @class WorkerPool
* @brief A pool of worker threads that executes jobs.
*
* The `WorkerPool` class manages a collection of worker threads, which continuously
* pull jobs from a thread-safe queue and execute them. The jobs are submitted as
* `IJob` objects, which contain a `std::function<void()>` to be executed by the threads.
*/
class WorkerPool {
public:
  /**
   * @interface IJob
   * @brief Interface for a job that can be executed by a worker pool.
   *
   * The `IJob` interface provides a way to define jobs that can be executed by worker threads.
   */
  class IJob {
  public:
    virtual ~IJob() = default;

    /**
     * @brief Executes the job's function.
     */
    virtual void execute() const = 0;
  };
  
  /**
   * @brief Constructs a WorkerPool with a specified number of threads.
   *
   * @param num_threads The number of worker threads in the pool.
   */
  WorkerPool(size_t num_threads = std::thread::hardware_concurrency());

  /**
   * @brief Destructor that stops all threads and joins them.
   */
  ~WorkerPool();

  /**
   * @brief Adds a job to the pool for execution.
   *
   * @param job_to_execute The job (function) to execute.
   */
  void addJob(const std::function<void()>& job_to_execute);

private:

  /**
   * @class Job
   * @brief Represents a job to be executed by the worker pool.
   *
   * The `Job` class holds a `std::function<void()>` and encapsulates the job
   * that will be executed by a worker thread. It implements the `IJob` interface.
   */
  class Job : public IJob {
  public:
    /**
     * @brief Constructs a Job with a specific function.
     *
     * @param func The function to be executed.
     */
    explicit Job(std::function<void()> func);

    /**
     * @brief Executes the job's function.
     */
    void execute() const override;

  private:
    std::function<void()> func_;  ///< The function to execute
  };

  /**
   * @brief The worker thread loop that continuously processes jobs from the queue.
   */
  void _workerLoop();

  ThreadSafeQueue<std::unique_ptr<IJob>> job_queue_;  ///< Queue of jobs to execute
  std::vector<std::thread> workers_;                  ///< Worker threads
  std::atomic<bool> stop_pool_;                       ///< Flag to stop the pool and workers
};

#endif // WORKER_POOL_HPP
