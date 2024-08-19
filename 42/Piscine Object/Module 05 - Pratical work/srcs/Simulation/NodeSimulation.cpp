#include "Simulation/NodeSimulation.hpp"
#include "Simulation/TrainSimulation.hpp"

NodeSimulation::NodeSimulation(const Node & node, EventMediator* mediator)
    : node_(node), mediator_(mediator) {}

void NodeSimulation::AddTrain(TrainSimulation* train) {
  // trains_.push_back(train);
  train->SetNode(this);
  AddObserver(train);
  // mediator_->RegisterTrain(train);
}

void NodeSimulation::RemoveTrain(TrainSimulation* train) {
  RemoveObserver(train);
  // mediator_->DeregisterTrain(train);
  // trains_.erase(
  //     std::remove(trains_.begin(), trains_.end(), train),
  //     trains_.end());
}
