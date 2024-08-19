#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <vector>
#include "Observer.hpp"

class Subject {
 public:
  virtual ~Subject() = default;
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void NotifyObservers();

 private:
  std::vector<Observer*> observers_;
};

#endif  // SUBJECT_HPP
