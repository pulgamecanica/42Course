/**
* @file thread_safe_queue.hpp
* @brief Provides a thread-safe templated queue implementation.
*
* This file contains the definition of the `ThreadSafeQueue` class,
* which ensures safe access and modification of a queue in a multithreaded environment.
* Specialization for `std::unique_ptr<T>` is also provided for managing dynamically allocated objects.
*/

#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>
#include <stdexcept>
#include <memory>

/**
* @class ThreadSafeQueue
* @brief A thread-safe implementation of a templated queue.
*
* The `ThreadSafeQueue` class ensures safe access and modification
* of a queue shared across multiple threads using mutex locks.
*
* @tparam TType The type of elements stored in the queue.
*/
template <typename TType>
class ThreadSafeQueue {
public:
  /**
   * @brief Adds an element to the end of the queue.
   * @param new_element The element to be added.
   */
  void push_back(const TType& new_element) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(new_element);
  }

  /**
   * @brief Adds an element to the front of the queue.
   * @param new_element The element to be added.
   */
  void push_front(const TType& new_element) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_front(new_element);
  }

  /**
   * @brief Removes and returns the last element of the queue.
   * @return The last element of the queue.
   * @throws std::runtime_error if the queue is empty.
   */
  TType pop_back() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
      throw std::runtime_error("Cannot pop from an empty queue.");

    TType element = queue_.back();
    queue_.pop_back();
    return element;
  }

  /**
   * @brief Removes and returns the first element of the queue.
   * @return The first element of the queue.
   * @throws std::runtime_error if the queue is empty.
   */
  TType pop_front() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
      throw std::runtime_error("Cannot pop from an empty queue.");

    TType element = queue_.front();
    queue_.pop_front();
    return element;
  }

private:
  std::deque<TType> queue_; ///< Underlying deque to store elements.
  std::mutex mutex_; ///< Mutex for thread-safe access to the queue.
};

/**
 * @class ThreadSafeQueue<std::unique_ptr<T>>
 * @brief Specialization of ThreadSafeQueue for `std::unique_ptr<T>`.
 *
 * This specialization of the `ThreadSafeQueue` is optimized for managing
 * `std::unique_ptr` elements. It ensures thread-safety while managing
 * dynamically allocated objects, providing efficient memory management.
 *
 * @tparam T The type of object managed by the unique pointer.
 */
template<typename T>
class ThreadSafeQueue<std::unique_ptr<T>> {
public:
  /**
   * @brief Adds a new unique_ptr element at the end of the queue.
   * 
   * This method is thread-safe and ensures that the `std::unique_ptr` element
   * is added to the queue in a synchronized manner.
   * 
   * @param new_element The unique_ptr element to add to the queue.
   */
  void push_back(std::unique_ptr<T> new_element) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(std::move(new_element));
  }

  /**
   * @brief Adds a new unique_ptr element at the front of the queue.
   * 
   * This method is thread-safe and ensures that the `std::unique_ptr` element
   * is added to the front of the queue in a synchronized manner.
   * 
   * @param new_element The unique_ptr element to add to the front of the queue.
   */
  void push_front(std::unique_ptr<T> new_element) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_front(std::move(new_element));
  }

  /**
   * @brief Removes and returns the first unique_ptr element from the queue.
   * 
   * This method is thread-safe and ensures that the `std::unique_ptr` element
   * is removed from the front of the queue safely.
   * 
   * @return The unique_ptr element removed from the front of the queue.
   * @throws std::out_of_range if the queue is empty.
   */
  std::unique_ptr<T> pop_front() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
      throw std::out_of_range("Queue is empty");

    auto front = std::move(queue_.front());
    queue_.pop_front();
    return front;
  }

  /**
   * @brief Removes and returns the last unique_ptr element from the queue.
   * 
   * This method is thread-safe and ensures that the `std::unique_ptr` element
   * is removed from the back of the queue safely.
   * 
   * @return The unique_ptr element removed from the back of the queue.
   * @throws std::out_of_range if the queue is empty.
   */
  std::unique_ptr<T> pop_back() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (queue_.empty())
      throw std::out_of_range("Queue is empty");

    auto back = std::move(queue_.back());
    queue_.pop_back();
    return back;
  }

private:
  std::deque<std::unique_ptr<T>> queue_;  ///< The underlying container for unique_ptr elements.
  std::mutex mutex_;                      ///< Mutex to ensure thread safety.
};

#endif // THREAD_SAFE_QUEUE_HPP
