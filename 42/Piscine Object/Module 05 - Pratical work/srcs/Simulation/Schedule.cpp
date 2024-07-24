#include "Simulation/Schedule.hpp"

#include <iostream>

Schedule::Schedule(const std::string& name, const Graph<std::string> &graph) : name_(name), graph_(graph) {}


void Schedule::AddTrain(const Train& train) {
  trains_.push_back(train);
}

void Schedule::PrintTrainData() const {
  std::cout << "Schedule: " << name_ << std::endl;
  for (const auto& train : trains_) {
    std::cout << "  Train: " << train.name << ", MaxAcceleration: " << train.max_acceleration
              << ", MaxBrakeForce: " << train.max_brake_force << ", Departure: " << train.departure
              << ", Arrival: " << train.arrival << ", Hour: " << train.hour << std::endl;
  }
}
