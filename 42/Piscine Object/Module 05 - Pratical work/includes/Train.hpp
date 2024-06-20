#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>

struct Train {
  std::string name;
  double max_acceleration;
  double max_brake_force;
  std::string departure;
  std::string arrival;
  std::string hour;
};

#endif  // TRAIN_HPP
