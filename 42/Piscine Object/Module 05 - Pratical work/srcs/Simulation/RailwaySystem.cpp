#include "Simulation/RailwaySystem.hpp"
#include "Simulation/Parser.hpp"

RailwaySystem::RailwaySystem(const std::string & elements_file_name, const std::string & schedule_directory) {
  Parser::ParseElementsFile(elements_file_name, *this);
  Parser::ParseScheduleFiles(schedule_directory, *this);
}

void RailwaySystem::AddNode(const Node& node) {
  if (node.GetName().empty())
    std::cerr << "Node must have a name" << std::endl;
  else
    nodes_.emplace(node.GetName(), node);
}

void RailwaySystem::AddRail(const Rail& rail) {
  rails_.emplace_back(rail);
  // Adding rails generates an entry in the graph of connected nodes
  graph_.AddEdge(&nodes_[rail.node1], &nodes_[rail.node2], rail.distance);
}

void RailwaySystem::AddEvent(const Event& event) {
  events_.push_back(event);
}

void RailwaySystem::AddSchedule(Schedule& schedule) {
  schedules_.emplace(schedule.GetName(), schedule);
}

Node& RailwaySystem::GetNode(const std::string & name) {
  if (!nodes_.contains(name))
    throw std::runtime_error(std::string("Node '") + name + std::string("' not founded"));
  return nodes_[name];
}

const Schedule& RailwaySystem::GetSchedule(const std::string& name) const {
  if (schedules_.find(name) == schedules_.end())
    throw std::runtime_error("Schedule not found: " + name);
  return schedules_.at(name);
}

const std::unordered_map<std::string, Schedule> RailwaySystem::GetSchedules() const {
  return schedules_;
}

const std::vector<Event>& RailwaySystem::GetEvents() const {
  return events_;
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
