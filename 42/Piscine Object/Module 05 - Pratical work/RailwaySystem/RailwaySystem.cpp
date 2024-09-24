#include "RailwaySystem.hpp"
#include "Parser.hpp"
#include <iostream>
#include <sys/stat.h>

RailwaySystem::RailwaySystem(const std::string& elementsFileName, const std::string& scheduleDirectory) {
  Parser::ParseElementsFile(elementsFileName, *this);
  struct stat s;
  if( stat(scheduleDirectory.c_str(), &s) == 0 ) {
    if( s.st_mode & S_IFDIR )
      Parser::ParseScheduleFiles(scheduleDirectory, *this);
    else
      Parser::ParseTrainFile(scheduleDirectory, scheduleDirectory, *this);
  } else {
      throw std::runtime_error("Schedule file is not valid");
  }
  SetupRails();
  SetupEvents();
}

void RailwaySystem::SetupEvents() {
  for (const auto& event: events_) {
    Node * node = GetNode(event->GetLocation());
    if (!node) {
      throw std::runtime_error(("Invalid Event location: " + event->GetLocation()).c_str());
    }
    node->AddEvent(event.get());
    event->SetNode(node);
  }
}

void RailwaySystem::SetupRails() {
  // Setup Graph with rails info
  for (const auto & rail : rails_) {
    Rail * railPtr = rail.get();
    const std::string& node1 = railPtr->GetNode1();
    const std::string& node2 = railPtr->GetNode2();

    auto node1It = nodes_.find(node1);
    auto node2It = nodes_.find(node2);

    // Getting the node raw pointer for each
    if (node1It != nodes_.end() && node2It != nodes_.end()) {
      graph_.AddEdge(node1It->second.get(), node2It->second.get(), railPtr->GetDistance());
    } else {
      throw std::runtime_error((std::string("Rail has node which doesn't exist: ") + node1 + " - " + node2).c_str());
    }
  }
}

void RailwaySystem::AddNode(const std::string& name) {
  if (name.empty()) {
    std::cerr << "Node must have a name" << std::endl;
    return;
  }
  nodes_.emplace(name, std::make_unique<Node>(name));
}

void RailwaySystem::AddRail(const std::string& node1, const std::string& node2, double distance) {
  if (node1.empty() || node2.empty()) {
    std::cerr << "Rails must have valid node names" << std::endl;
    return;
  }
  if (node1 == node2) return ;
  rails_.emplace_back(std::make_unique<Rail>(node1, node2, distance));
}

void RailwaySystem::AddEvent(const std::string& type, double probability, double duration, const std::string& location) {
  events_.emplace_back(std::make_unique<Event>(type, probability, duration, location));
}

void RailwaySystem::AddSchedule(std::unique_ptr<Schedule>& schedule) {
  std::string name = schedule->GetName();
  schedules_.emplace(name, std::move(schedule));
}

Node* RailwaySystem::GetNode(const std::string& name) const {
  auto it = nodes_.find(name);
  if (it == nodes_.end())
    throw std::runtime_error("Node '" + name + "' not found");
  return it->second.get();
}

Schedule* RailwaySystem::GetSchedule(const std::string& name) const {
  auto it = schedules_.find(name);
  if (it == schedules_.end()) {
    throw std::runtime_error("Schedule not found: " + name);
  }
  return it->second.get();
}

const std::unordered_map<std::string, std::unique_ptr<Schedule>>& RailwaySystem::GetSchedules() const {
  return schedules_;
}

const Graph<std::string>& RailwaySystem::GetGraph() const {
  return graph_;
}

const std::unordered_map<std::string, std::unique_ptr<Node>>& RailwaySystem::GetNodes() const {
  return nodes_;
}

const std::vector<std::unique_ptr<Rail>>& RailwaySystem::GetRails() const {
  return rails_;
}

const std::vector<std::unique_ptr<Event>>& RailwaySystem::GetEvents() const {
  return events_;
}
