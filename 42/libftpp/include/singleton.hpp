/**
* @file singleton.hpp
* @brief Contains the definition of the Singleton template class.
*
* This file declares the Singleton template class that ensures only one instance
* of a templated class `TType` exists. It provides access to that instance and ensures
* thread safety during its creation and access. The Singleton class also prevents
* re-instantiation after the instance has been created.
* 
* The `Singleton` class manages the instantiation of `TType`, and throws exceptions
* if there is an attempt to create another instance once it's already been set.
* 
* @note This class is intended to be used for classes that need a single, globally
*       accessible instance across the program.
*/

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <stdexcept>
#include <mutex>
#include <memory>
#include <iostream>

/**
* @class Singleton
* @brief A template class that ensures only one instance of `TType` exists.
*
* The `Singleton` class provides a thread-safe way to create and access a single
* instance of a templated class `TType`. It guarantees that `TType` will be instantiated
* only once and provides access to that instance through a static method.
*
* The `Singleton` class also prevents re-instantiation by throwing an exception
* if an attempt is made to instantiate the class after it has been created.
*
* ## Thread Safety:
* The `Singleton` class is designed to be thread-safe, using a mutex to ensure
* that the instance is only created once even in multithreaded scenarios.
* 
* @tparam TType The class for which the singleton pattern is implemented.
*/
template <typename TType>
class Singleton {
public:
  // Delete copy constructor and copy assignment to prevent copying
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

  /**
   * @brief Returns the managed instance of TType.
   * 
   * If the instance is not yet created, it throws an exception.
   * 
   * Protected, thread safe by using `std::lock_guard`.
   * 
   * @return Pointer to the unique instance of TType.
   * @throws std::runtime_error If the instance is not yet created.
   */
  static TType* instance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instance_)
      throw std::runtime_error("Instance not yet created");
    return instance_.get();
  }

  /**
   * @brief Instantiates the managed instance of TType with arguments.
   * 
   * This function will only instantiate the instance once. If an attempt
   * is made to instantiate it after it has already been created, it throws
   * an exception.
   * 
   * @tparam TArgs The types of the arguments used to instantiate the instance.
   * @param p_args The arguments for instantiating the instance.
   * @throws std::runtime_error If the instance is already created.
   */
  template <typename... TArgs>
  static void instantiate(TArgs&&... p_args) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_)
      throw std::runtime_error("Instance already created");
    instance_ = std::make_unique<TType>(std::forward<TArgs>(p_args)...);
  }

private:
  static std::unique_ptr<TType> instance_;  ///< The unique instance of TType.
  static std::mutex mutex_;                 ///< Mutex to ensure thread safety.

  // Constructor is private to prevent instantiating outside the class
  Singleton() = default;
  ~Singleton() = default;
};

template <typename TType>
std::unique_ptr<TType> Singleton<TType>::instance_ = nullptr;

template <typename TType>
std::mutex Singleton<TType>::mutex_;

#endif // SINGLETON_HPP
