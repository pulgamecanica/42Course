#include "RailwaySystem.hpp"

#include <iostream>
#include <stdexcept>

void RailwaySystem::AddNode(const Node& node) {
  nodes_[node.name] = node;
}

void RailwaySystem::AddRail(const Rail& rail) {
  rails_.push_back(rail);
}

void RailwaySystem::AddEvent(const Event& event) {
  events_.push_back(event);
}

void RailwaySystem::AddSimulation(const Simulation& simulation) {
  simulations_.emplace(simulation.GetName(), simulation);
}

Simulation& RailwaySystem::GetSimulation(const std::string& name) {
  if (simulations_.find(name) == simulations_.end()) {
    throw std::runtime_error("Simulation not found: " + name);
  }
  return simulations_.at(name);
}

void RailwaySystem::PrintElementsData() const {
  std::cout << "Nodes:" << std::endl;
  for (const auto& [name, node] : nodes_) {
    std::cout << "  Node: " << name << std::endl;
  }

  std::cout << "Rails:" << std::endl;
  for (const auto& rail : rails_) {
    std::cout << "  Rail: " << rail.node1 << " to " << rail.node2 << ", Distance: " << rail.distance << std::endl;
  }

  std::cout << "Events:" << std::endl;
  for (const auto& event : events_) {
    std::cout << "  Event: " << event.type << ", Probability: " << event.probability
              << ", Duration: " << event.duration << ", Location: " << event.location << std::endl;
  }
}

void RailwaySystem::PrintSimulationsData() const {
  for (const auto& [name, simulation] : simulations_) {
    simulation.PrintTrainData();
  }
}
