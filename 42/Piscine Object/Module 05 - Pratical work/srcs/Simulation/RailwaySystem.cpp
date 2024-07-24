#include "Simulation/RailwaySystem.hpp"
#include "Simulation/Parser.hpp"
#include <iostream>
#include <stdexcept>

RailwaySystem::RailwaySystem(const std::string & elements_file_name, const std::string & schedule_directory) {
  Parser::ParseElementsFile(elements_file_name, *this);
  Parser::ParseScheduleFiles(schedule_directory, *this);
}

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

void RailwaySystem::AddSchedule(Schedule& schedule) {
  schedules_.emplace(schedule.GetName(), schedule);
}

const Schedule& RailwaySystem::GetSchedule(const std::string& name) const {
  if (schedules_.find(name) == schedules_.end())
    throw std::runtime_error("Schedule not found: " + name);
  return schedules_.at(name);
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

void RailwaySystem::PrintSchedulesData() const {
  for (const auto& [name, schedule] : schedules_) {
    schedule.PrintTrainData();
  }
}

const std::unordered_map<std::string, Schedule> RailwaySystem::GetSchedules() const {
  return schedules_;
}

const Graph<std::string>& RailwaySystem::GetGraph() const {
  return graph_;
}

const std::unordered_map<std::string, Node>& RailwaySystem::GetNodes() const {
  return nodes_;
}

const std::vector<Rail>& RailwaySystem::GetRails() const {
  return rails_;
}

Node& RailwaySystem::GetNode(const std::string & name) {
  if (!nodes_.contains(name))
    throw std::runtime_error(std::string("Node '") + name + std::string("' not founded"));
  return nodes_[name];
}

void RailwaySystem::Print() {
  PrintElementsData();
  std::cout << "Scheduled train trips:" << std::endl;
  PrintSchedulesData();


  // PathInfo path = graph_.Dijkstra(0, 6);
  // for (const auto& [node, distance] : path.path) {
  //   std::cout << "Node: " << node << ", Distance: " << distance << std::endl;
  // }
}

