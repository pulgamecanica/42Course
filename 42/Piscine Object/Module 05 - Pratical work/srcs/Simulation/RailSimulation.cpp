#include "Simulation/RailSimulation.hpp"
#include "Simulation/TrainSimulation.hpp"

RailSimulation::RailSimulation(const Rail & rail, CollisionMediator* mediator)
    : rail_(rail), mediator_(mediator) {}

void RailSimulation::CheckForCollisions() {
  mediator_->CheckForCollisions();
}

void RailSimulation::AddTrain(TrainSimulation* train) {
  // trains_.push_back(train);
  train->SetRail(this);
  AddObserver(train);
  mediator_->RegisterTrain(train);
}

void RailSimulation::RemoveTrain(TrainSimulation* train) {
  RemoveObserver(train);
  mediator_->DeregisterTrain(train);
  // trains_.erase(
  //     std::remove(trains_.begin(), trains_.end(), train),
  //     trains_.end());
}

double RailSimulation::Distance() const {
  return rail_.distance;
}

double RailSimulation::DistanceM() const {
  return rail_.distance * 1000;
}
