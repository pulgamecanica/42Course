/**
* @file observer.hpp
* @brief Provides the Observer pattern implementation.
*
* This file defines the template `Observer` class.
* It allows defining an Observer for a generic <template TEvent>
* which can later subscribe events for instances of the same type.
*/

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stdexcept>

/**
 * @class Observer
 * @brief A templated Observer class for subscribing to and notifying events.
 *
 * This class implements the Observer design pattern. It allows users to subscribe to specific events
 * with lambda functions, and it notifies all subscribers when an event is triggered.
 * 
 * @tparam TEvent The type of event that will be observed.
 */
template <typename TEvent>
class Observer {
public:
  /**
   * @brief Subscribe a lambda function to an event.
   *
   * This method allows a user to subscribe a lambda function to a specific event.
   * The lambda will be called when the event is triggered via the `notify` method.
   *
   * @param event The event to subscribe to.
   * @param lambda The lambda function to be called when the event is triggered.
   */
  void subscribe(const TEvent& event, const std::function<void()>& lambda) {
    event_map_[event].push_back(lambda);
  }

  /**
   * @brief Notify all subscribers of a specific event.
   *
   * This method notifies all subscribers that are subscribed to the provided event.
   * It executes all lambdas that were subscribed to the event.
   *
   * @param event The event to notify subscribers about.
   */
  void notify(const TEvent& event) {
    for (auto& lambda : event_map_[event]) {
      lambda();  // Call all subscribed lambdas
    }
  }

private:
  
  std::unordered_map<TEvent, std::vector<std::function<void()>>> event_map_; ////< A map that associates an event with a list of lambdas subscribed to that event.
};

#endif // OBSERVER_HPP