#include "RailwaySystem.hpp"

#include <iostream>
#include <stdexcept>

void RailwaySystem::AddNode(const Node& node) {
  nodes_.emplace(node.GetName(), node);
}

void RailwaySystem::AddRail(const Rail& rail) {
  rails_.emplace_back(rail);
  // Adding rails generates an entry in the graph
  graph_.AddEdge(&nodes_[rail.node1], &nodes_[rail.node2], rail.distance);
}

void RailwaySystem::AddEvent(const Event& event) {
  events_.push_back(event);
}

void RailwaySystem::AddSimulation(Simulation& simulation) {
  simulation.SetGraph(&graph_);
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

void RailwaySystem::Run() {
  PrintElementsData();
  std::cout << "Simulation scheduled trips:" << std::endl;
  PrintSimulationsData();


  // PathInfo path = graph_.Dijkstra(0, 6);
  // for (const auto& [node, distance] : path.path) {
  //   std::cout << "Node: " << node << ", Distance: " << distance << std::endl;
  // }
}

