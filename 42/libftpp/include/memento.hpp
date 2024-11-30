/**
* @file memento.hpp
* @brief Provides the Memento pattern implementation for saving and restoring the state of objects.
*
* This file defines the `Memento` class, which provides a framework for implementing the Memento design pattern. 
* It allows derived classes to save and restore their state via `Snapshot` objects while keeping the internals of 
* the state encapsulated.
*/

#ifndef MEMENTO_HPP
#define MEMENTO_HPP

#include "data_buffer.hpp"

/**
* @class Memento
* @brief Base class for implementing the Memento design pattern.
*
* The `Memento` class enables derived classes to save and restore their state using `Snapshot` objects. 
* It achieves this by exposing `save()` and `load()` methods that internally call the private `_saveToSnapshot()` 
* and `_loadFromSnapshot()` methods of the derived class. These private methods are accessed using the `friend` 
* mechanism, allowing `Memento` to interact directly with the internals of derived classes.
* 
* ## Usage:
* - Derive your class from `Memento`.
* - Implement the private `_saveToSnapshot()` and `_loadFromSnapshot()` methods to define what state should 
*   be saved and restored.
*
* ### Example:
* ```cpp
* class TestClass : public Memento {
*     friend class Memento;
*
* public:
*     int x_ = 0;
*     std::string y_;
*
* private:
*     void _saveToSnapshot(Snapshot& snapshot) const override {
*         snapshot << x_ << y_;
*     }
*
*     void _loadFromSnapshot(Snapshot& snapshot) override {
*         snapshot >> x_ >> y_;
*     }
* };
* ```
*/
class Memento {
public:
  using Snapshot = DataBuffer;

  /**
   * @brief Saves the current state of the object into a `Snapshot`.
   * 
   * This method creates a new `Snapshot` and invokes the `_saveToSnapshot()` method
   * of the derived class to populate it with the object's state.
   *
   * @return A `Snapshot` containing the serialized state of the object.
   */
  Snapshot save();

  /**
   * @brief Restores the object's state from a given `Snapshot`.
   * 
   * This method takes a `Snapshot` containing a previously saved state and invokes the 
   * `_loadFromSnapshot()` method of the derived class to restore the object's state.
   *
   * @param snapshot The `Snapshot` containing the serialized state to restore.
   */
  void load(const Snapshot& snapshot);

protected:
  /**
   * @brief Virtual destructor for proper cleanup of derived classes.
   */
  virtual ~Memento() = default;

private:
  /**
   * @brief Saves the state of the object into a `Snapshot`.
   *
   * Derived classes must implement this method to define how their state is serialized.
   *
   * @param snapshot The `Snapshot` to populate with the object's state.
   */
  virtual void _saveToSnapshot(Memento::Snapshot& snapshot) = 0;

  /**
   * @brief Restores the state of the object from a `Snapshot`.
   *
   * Derived classes must implement this method to define how their state is deserialized.
   *
   * @param snapshot The `Snapshot` containing the state to restore.
   */
  virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};

#endif // MEMENTO_HPP
