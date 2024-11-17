/**
* @file thread.hpp
* @brief Provides a wrapper around `std::thread` with a custom name and controlled lifecycle.
*
* This header file contains the definition of the `Thread` class, 
* allowing for named threads and better control over thread execution.
*/

#ifndef THREAD_HPP
#define THREAD_HPP

#include <thread>
#include <string>
#include <functional>
#include <stdexcept>
#include "thread_safe_iostream.hpp"

/**
* @class Thread
* @brief A wrapper for `std::thread` with a name and controlled start/stop lifecycle.
*
* The `Thread` class encapsulates an `std::thread` and provides additional functionality:
* - Named threads for debugging purposes.
* - Explicit start and stop control for better thread lifecycle management.
*/
class Thread {
public:
  /**
   * @brief Constructs a named thread with a function to execute.
   * @param name The name of the thread.
   * @param func_to_execute The function to execute when the thread starts.
   */
  Thread(const std::string& name, std::function<void()> func_to_execute);

  /**
   * @brief Starts the thread, executing the provided function.
   * @throws std::runtime_error if the thread has already started.
   */
  void start();

  /**
   * @brief Stops the thread and ensures proper cleanup.
   * @throws std::runtime_error if the thread is not running.
   */
  void stop();

  /**
   * @brief Destructor to ensure the thread is joined before destruction.
   */
  ~Thread();

private:
  std::string thread_name_; ///< Name of the thread for identification.
  std::function<void()> func_; ///< The function to execute in the thread.
  std::thread thread_; ///< Internal `std::thread` instance.
  bool is_running_; ///< Tracks the running state of the thread.
};

extern ThreadSafeIOStream threadSafeCout;

#endif // THREAD_HPP
