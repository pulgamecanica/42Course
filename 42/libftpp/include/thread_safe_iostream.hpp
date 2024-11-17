/**
* @file thread_safe_iostream.hpp
* @brief Provides a thread-safe version of I/O stream with prefixed lines.
*
* This header file contains the definition of the `ThreadSafeIOStream` class
* which allows for thread-safe output operations with customizable prefixes.
*/

#ifndef THREAD_SAFE_IOSTREAM_HPP
#define THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

/**
* @class ThreadSafeIOStream
* @brief A thread-safe version of I/O stream.
*
* The `ThreadSafeIOStream` class provides thread-safe output capabilities
* by locking a mutex during output operations. It allows users to set a prefix
* for output messages, which can help debug and monitor applications with
* multiple threads.
* 
* @note The stream will only be outputed when the `std::endl` manipulator is
*       founded, indicating that the line ended.
*/
class ThreadSafeIOStream {
public:
  /**
   * @brief Constructs a ThreadSafeIOStream
   */
  ThreadSafeIOStream();

  /**
   * @brief Sets the prefix printed in front of each output line.
   * 
   * @param prefix The prefix string to prepend to the output.
   */
  void setPrefix(const std::string& prefix);

  /**
   * @brief Handles the `std::endl` manipulator for proper line breaks.
   * 
   * This function flushes the thread-local buffer to the shared `std::cout`
   * under a mutex lock when `std::endl` is encountered.
   *
   * @param manip A pointer to a manipulator function such as `std::endl`.
   * @return A reference to `ThreadSafeIOStream` for chaining.
   */
  ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

  /**
   * @brief Output operator overload to handle various types.
   * 
   * This function writes to a thread-local buffer and flushes it only
   * when a newline is encountered.
   *
   * @tparam T The type of value to output.
   * @param value The value to output.
   * @return A reference to `ThreadSafeIOStream` for chaining.
   */
  template <typename T>
  ThreadSafeIOStream& operator<<(const T& value) {
    thread_buffer_ << value;
    return *this;
  }

  /**
   * @brief Input operator overload to handle various types.
   * 
   * This function locks a mutex to ensure thread-safe input operations.
   * The prefix will be appended before the input (not always a `line`)
   *
   * @tparam T The type of value to read.
   * @param value The variable to store the input.
   * @return A reference to `ThreadSafeIOStream` for chaining.
   */
  template <typename T>
  ThreadSafeIOStream& operator>>(T& value) {
    std::lock_guard<std::mutex> lock(global_mutex_);
    std::cin >> value;
    return *this;
  }

private:
  std::mutex global_mutex_; ///< Mutex to ensure thread-safe access to std::cout
  thread_local static std::ostringstream thread_buffer_; ///< Thread-local buffer for each thread
  thread_local static std::string thread_prefix_; ///< Thread-local prefix for each thread
};

#endif // THREAD_SAFE_IOSTREAM_HPP
