#include "Simulation/CollisionMediator.hpp"
#include "Simulation/TrainSimulation.hpp"
#include <algorithm>

void CollisionMediator::RegisterTrain(TrainSimulation* train) {
  trains_.push_back(train);
}

void CollisionMediator::DeregisterTrain(TrainSimulation* train) {
  trains_.erase(
      std::remove(trains_.begin(), trains_.end(), train),
      trains_.end());
}

void CollisionMediator::CheckForCollisions() {
  // Simple collision detection logic
  for (size_t i = 0; i < trains_.size(); ++i) {
    for (size_t j = i + 1; j < trains_.size(); ++j) {
      // if sharing rail
      float distance = std::abs(trains_[i]->CalculateStoppingDistance() -
                                trains_[j]->CalculateStoppingDistance());
      if (distance < trains_[i]->CalculateStoppingDistance()) {
        ;
        // choose the one behind
        // trains_[i]->SlowDown();
        // trains_[j]->SlowDown();
      }
    }
  }
}
