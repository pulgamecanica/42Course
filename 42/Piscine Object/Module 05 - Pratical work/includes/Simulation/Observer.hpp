#ifndef OBSERVER_HPP_
#define OBSERVER_HPP_

class Subject;

class Observer {
 public:
  virtual ~Observer() = default;
  virtual void Update(Subject* subject) = 0;
};

#endif  // OBSERVER_HPP_