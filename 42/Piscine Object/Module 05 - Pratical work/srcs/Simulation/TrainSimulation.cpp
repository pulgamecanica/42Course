#include "Simulation/TrainSimulation.hpp"
#include "Simulation/RailSimulation.hpp"

TrainSimulation::TrainSimulation(const Train & train)
    : train_(train),
      speed_(0.0f),
      rail_(nullptr),
      position_(0.0f) {}

void TrainSimulation::Update() {
  ;
}

void TrainSimulation::Update(Subject* subject) {
  if (auto* rail = dynamic_cast<RailSimulation*>(subject)) {
    // React to rail updates (e.g., potential collision)
    // SlowDown();
  }
}

void TrainSimulation::SetRail(RailSimulation* rail) {
  rail_ = rail;
}

void TrainSimulation::SetNode(NodeSimulation* node) {
  node_ = node;
}

void TrainSimulation::Move() {
  // Simulate train movement
  // speed_ += acceleration_;
  // position_ += speed_;
}

void TrainSimulation::SlowDown() {
  // Calculate how much to slow down
  // speed_ -= brake_force_;
  // if (speed_ < 0) {
  //   speed_ = 0;
  // }
}

float TrainSimulation::CalculateStoppingDistance() const {
  // Simple formula for stopping distance (assuming constant deceleration)
  // return (speed_ * speed_) / (2 * brake_force_);
  return 0.0f;
}
