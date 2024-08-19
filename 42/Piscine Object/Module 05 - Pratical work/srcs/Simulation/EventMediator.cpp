#include "Simulation/EventMediator.hpp"
#include "Simulation/NodeSimulation.hpp"
#include <algorithm>

void EventMediator::RegisterNode(NodeSimulation* train) {
  nodes_.push_back(train);
}

void EventMediator::DeregisterNode(NodeSimulation* train) {
  nodes_.erase(
      std::remove(nodes_.begin(), nodes_.end(), train),
      nodes_.end());
}

void UpdateEvents() {
  ;
}

void EventMediator::CheckForEvents() {
  ;
}
