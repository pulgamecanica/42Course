#ifndef RAILWAY_SYSTEM_HPP
#define RAILWAY_SYSTEM_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "Event.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "Simulation.hpp"

#include "Graph.hpp" // From Dijkstra's Lib

class RailwaySystem {
 public:
  void AddNode(const Node& node);
  void AddRail(const Rail& rail);
  void AddEvent(const Event& event);
  void AddSimulation(Simulation& simulation);
  Simulation& GetSimulation(const std::string& name);

  void PrintElementsData() const;
  void PrintSimulationsData() const;
  void Run();
 private:
  std::unordered_map<std::string, Node> nodes_;
  std::vector<Rail> rails_;
  std::vector<Event> events_;
  std::unordered_map<std::string, Simulation> simulations_;
  Graph<std::string> graph_;
};

#endif  // RAILWAY_SYSTEM_HPP
