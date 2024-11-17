/**
* @file state_machine.hpp
* @brief Contains the definition of the StateMachine template class.
*
* This file declares the `StateMachine` class, which manages states and transitions 
* between those states. It allows actions to be associated with specific states and 
* transitions. The StateMachine can update and transition between states while executing 
* corresponding actions and transitions.
*
* The `StateMachine` class provides a clean and flexible way to manage state-driven behavior.
* 
* @note This template class is intended for use with any enum or class representing states.
*/

#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <iostream>

/**
* @class StateMachine
* @brief A template class for managing states and transitions.
*
* The `StateMachine` class enables handling of different states and their transitions
* through a state machine pattern. It allows users to define actions that execute when 
* the state machine enters a specific state, and it also allows defining transitions between states.
*
* @tparam TState The type of the state, typically an enum representing the states.
*/
template <typename TState>
class StateMachine {
public:
  /**
   * @brief Default constructor for `StateMachine`.
   * 
   * @note `initialized_state_` is false until any state is available.
   */
  StateMachine(): initialized_state_(false) {
    ;
  }

  /**
   * @brief Adds a state to the state machine.
   *
   * This method registers a state that the state machine can transition into.
   * 
   * @param state The state to be added to the state machine.
   */
  void addState(const TState& state) {
    states_.insert(state);
    if (!initialized_state_) {
      current_state = state;
      initialized_state_ = true;
    }
  }

  /**
   * @brief Adds a transition between two states.
   *
   * This method registers a transition between a start state and a final state.
   * It associates a lambda function to execute during the transition.
   * 
   * @param startState The starting state of the transition.
   * @param finalState The final state after the transition.
   * @param lambda The lambda function to execute during the transition.
   */
  void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda) {
    transitions_[startState][finalState] = lambda;
  }

  /**
   * @brief Adds an action to be executed when the state machine enters a specific state.
   *
   * This method registers an action to be executed when the state machine is in a specific state.
   * 
   * @param state The state for which the action is to be registered.
   * @param lambda The lambda function to execute when the state machine enters the state.
   */
  void addAction(const TState& state, const std::function<void()>& lambda) {
    actions_[state] = lambda;
  }

  /**
   * @brief Transition the state machine to a specific state.
   *
   * This method changes the state of the state machine and executes any transition action associated with it.
   * If no transition is defined, it throws an exception.
   * 
   * @param state The state to transition to.
   * 
   * @throws std::runtime_error If current state has not been initialized.
   * @throws std::invalid_argument If the transition is not defined.
   */
  void transitionTo(const TState& state) {
    if (!initialized_state_)
      throw std::runtime_error("State not initialized yet");
    if (transitions_.find(current_state) == transitions_.end() || transitions_[current_state].find(state) == transitions_[current_state].end())
      throw std::invalid_argument("No transition defined for this state");
    transitions_[current_state][state]();  // Execute the transition
    current_state = state;
  }

  /**
   * @brief Updates the state machine, executing the action for the current state.
   *
   * This method checks if an action is defined for the current state and executes it.
   * If no action is defined, throws an exception.
   * 
   * @throws std::runtime_error If current state has not been initialized.
   * @throws std::invalid_argument If the transition is not defined.
   */
  void update() {
    if (!initialized_state_)
      throw std::runtime_error("State not initialized yet");
    if (actions_.find(current_state) == actions_.end())
      throw std::invalid_argument("No action defined for this state");
    actions_[current_state]();  // Execute the action
  }

private:
  std::unordered_set<TState> states_;  ///< Registered states in the state machine.
  std::unordered_map<TState, std::function<void()>> actions_;  ///< Actions for each state (one action per state).
  std::unordered_map<TState, std::unordered_map<TState, std::function<void()>>> transitions_;  ///< Transitions between states.
  TState current_state;  ///< The current state of the state machine.
  bool initialized_state_; ///<  Member to know if the machine has been initialized (any state is known)
};

#endif  // STATE_MACHINE_HPP
