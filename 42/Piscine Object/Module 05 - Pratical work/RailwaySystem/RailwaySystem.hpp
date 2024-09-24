#ifndef RAILWAY_SYSTEM_HPP_
#define RAILWAY_SYSTEM_HPP_

#include "Event.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "Schedule.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include "Graph.hpp" // From Dijkstra's Lib
#include <memory>  // For smart pointers

class RailwaySystem {
 public:
  RailwaySystem(const std::string& elementsFileName, const std::string& scheduleDirectory);

  void AddNode(const std::string& name);
  void AddRail(const std::string& node1, const std::string& node2, double distance);
  void AddEvent(const std::string& type, double probability, double duration, const std::string& location);
  void AddSchedule(std::unique_ptr<Schedule>& schedule);

  Node* GetNode(const std::string& name) const;
  Schedule* GetSchedule(const std::string& name) const;
  const std::unordered_map<std::string, std::unique_ptr<Schedule>>& GetSchedules() const;
  const Graph<std::string>& GetGraph() const;
  const std::unordered_map<std::string, std::unique_ptr<Node>>& GetNodes() const;
  const std::vector<std::unique_ptr<Rail>>& GetRails() const;
  const std::vector<std::unique_ptr<Event>>& GetEvents() const;

 private:
  void SetupRails();
  void SetupEvents();
  std::unordered_map<std::string, std::unique_ptr<Node>> nodes_;
  std::vector<std::unique_ptr<Rail>> rails_;
  std::vector<std::unique_ptr<Event>> events_;
  std::unordered_map<std::string, std::unique_ptr<Schedule>> schedules_;
  Graph<std::string> graph_;
};

#endif  // RAILWAY_SYSTEM_HPP_
