#ifndef SUBJECT_HPP_
#define SUBJECT_HPP_

#include <vector>
#include "Observer.hpp"

class Subject {
 public:
  virtual ~Subject() = default;
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void NotifyObservers();
  const std::vector<Observer*>& GetObservers() const;

 protected:
  std::vector<Observer*> observers_;
};

#endif  // SUBJECT_HPP_