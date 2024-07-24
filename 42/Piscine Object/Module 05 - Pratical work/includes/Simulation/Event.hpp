#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

struct Event {
  std::string type;
  double probability;
  double duration;
  std::string location;
};

#endif  // EVENT_HPP
