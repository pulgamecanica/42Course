//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "CollisionMediator.hpp"
#include "Train.hpp"
#include "Rail.hpp"
#include "Train.hpp"
#include "Simulation.hpp"
#include <vector>
#include <set>

CollisionMediator::CollisionMediator(Simulation & simulation)
    : simulation_(simulation) {
}

void CollisionMediator::CheckForCollisions() {
  // Simple collision detection logic
  for (auto & rail : simulation_.GetRails()) {
    std::vector<TrainSimulation*> rail_trains;
    for (const auto observer : rail->GetObservers()) {
      TrainSimulation * train = (TrainSimulation*)observer;
      if (train) {
        rail_trains.push_back(train);
      } else {
        throw std::runtime_error("Observer couldn't be casted to TrainSimulation");
      }
    }
    for (const auto& train: rail_trains) {
      for (const auto& train_behind: rail_trains) {
        if (train == train_behind) continue;
        if (train->GetNextNodeDestiny() == train->GetNextNodeDestiny()) {
          if (train->GetPosition() > train_behind->GetPosition()) { // It's not behind then!
            float distance_between_trains = train->GetPosition() - train_behind->GetPosition();
            if (train_behind->GetStoppingDistance() <= distance_between_trains - 100) { // + 100 meters safe distance
              train_behind->SetMaxAcceleration(train->GetMaxAccelerationForce());
            }
            if (distance_between_trains <= 10) { // Collision risk! Warn all observers of the rail!
              rail->NotifyObservers();
            }
          }
        }
      }
    }
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

