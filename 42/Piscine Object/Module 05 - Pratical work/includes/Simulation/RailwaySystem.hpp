#ifndef RAILWAY_SYSTEM_HPP
#define RAILWAY_SYSTEM_HPP

#include "Simulation/Event.hpp"
#include "Simulation/Node.hpp"
#include "Simulation/Rail.hpp"
#include "Simulation/Schedule.hpp"
#include "Graph.hpp" // From Dijkstra's Lib
#include <string>
#include <unordered_map>
#include <vector>

class RailwaySystem {
public:
  RailwaySystem(const std::string & elements_file_name, const std::string & schedule_directory);
  void AddNode(const Node& node);
  void AddRail(const Rail& rail);
  void AddEvent(const Event& event);
  void AddSchedule(Schedule& simulation);

  Node& GetNode(const std::string & name);
  const Schedule& GetSchedule(const std::string& name) const;
  const std::unordered_map<std::string, Schedule> GetSchedules() const;
  const Graph<std::string>& GetGraph() const;
  const std::unordered_map<std::string, Node>& GetNodes() const;
  const std::vector<Rail>& GetRails() const;
  const std::vector<Event>& GetEvents() const;
private:
  std::unordered_map<std::string, Node> nodes_;
  std::vector<Rail> rails_;
  std::vector<Event> events_;
  std::unordered_map<std::string, Schedule> schedules_;
  Graph<std::string> graph_;
};

#endif  // RAILWAY_SYSTEM_HPP
