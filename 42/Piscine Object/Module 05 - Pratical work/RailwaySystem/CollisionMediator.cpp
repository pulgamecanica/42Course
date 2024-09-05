//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CollisionMediator.hpp"
#include "Train.hpp"
#include "Rail.hpp"
#include "Simulation.hpp"
#include <vector>
#include <set>

CollisionMediator::CollisionMediator(Simulation & simulation)
    : simulation_(simulation) {
}

void CollisionMediator::CheckForCollisions() {
  // Simple collision detection logic
  for (auto & rail : simulation_.GetRails()) {
    std::vector<float> positions;
    for (const auto observer : rail->GetObservers()) {
      TrainSimulation * train = (TrainSimulation*)observer;
      if (train) {
        positions.push_back(train->GetPosition());
      } else {
        throw std::runtime_error("Observer couldn't be casted to TrainSimulation");
      }
    }
    std::set<float> unique_positions(positions.begin(), positions.end()); // Set should be same size if there are no duplicates
    if (unique_positions.size() != positions.size())
      rail->NotifyObservers(); // Notify all observers if there was a collision
  }
}

bool CollisionMediator::CanJoinRail(RailSimulation & rail, const std::string dest, const bool two_way) const {
  int MIN_START_DISTANCE = 50; // meters

  for (auto & observer : rail.GetObservers()) {
    TrainSimulation * train = (TrainSimulation*)observer;
    if (!train)
      throw std::runtime_error("Observer couldn't be converted to train");
    bool same_way = dest == train->GetNextNodeDestiny();
    // Check if other train comming opposite way
    if (!two_way && !same_way)
      return false;
    // Check if any train on same direction is very close to start
    if (same_way && train->GetPosition() < MIN_START_DISTANCE)
      return false;
  }
  return true;
}

