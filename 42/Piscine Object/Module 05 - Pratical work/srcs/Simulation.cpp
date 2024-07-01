#include "Simulation.hpp"

#include <iostream>

void Simulation::AddTrain(const Train& train) {
  trains_.push_back(train);
}

void Simulation::SetGraph(Graph<std::string> * graph) {
  graph_ = graph;
}

void Simulation::PrintTrainData() const {
  std::cout << "Simulation: " << name_ << std::endl;
  for (const auto& train : trains_) {
    std::cout << "  Train: " << train.name << ", MaxAcceleration: " << train.max_acceleration
              << ", MaxBrakeForce: " << train.max_brake_force << ", Departure: " << train.departure
              << ", Arrival: " << train.arrival << ", Hour: " << train.hour << std::endl;
  }
}
