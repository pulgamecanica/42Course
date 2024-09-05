#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <string>

// Event Class
class Event {
 public:
  Event(const std::string& type, double probability, double duration, const std::string& location);

  // Getters
  std::string GetType() const;
  double GetProbability() const;
  double GetDuration() const;
  std::string GetLocation() const;

 private:
  const std::string type_;
  const double probability_;
  const double duration_;
  const std::string location_;
};

#endif  // EVENT_HPP_
