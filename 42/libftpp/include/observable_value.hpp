/**
 * @file observable_value.hpp
 * @brief Header file for the ObservableValue class.
 */

#ifndef OBSERVABLE_VALUE_HPP
#define OBSERVABLE_VALUE_HPP

#include <functional>
#include <vector>

/**
 * @class ObservableValue
 * @brief Encapsulates a value and notifies subscribers when it changes.
 * @tparam T The type of the value being observed.
 */
template <typename T>
class ObservableValue {
public:
  /**
   * @brief Constructs an ObservableValue with an initial value.
   * @param initial_value The initial value.
   */
  ObservableValue(const T& initial_value): value_(initial_value) {}

  /**
   * @brief Gets the current value.
   * @return The current value.
   */
  const T& get() const {
    return value_;
  }

  /**
   * @brief Sets a new value and notifies subscribers if the value has changed.
   * @param new_value The new value.
   */
  void set(const T& new_value) {
    if (value_ != new_value) {
      value_ = new_value;
      for (const auto& subscriber : subscribers_) {
        subscriber(value_);
      }
    }
  }

  /**
   * @brief Adds a subscriber to be notified of value changes.
   * @param callback The callback function to be invoked when the value changes.
   */
  void subscribe(const std::function<void(const T&)>& callback) {
    subscribers_.push_back(callback);
  }

private:
  T value_; ///< The encapsulated value.
  std::vector<std::function<void(const T&)>> subscribers_; ///< List of subscribers.
};

#endif // OBSERVABLE_VALUE_HPP
