#ifndef RAILWAY_SYSTEM_HPP
#define RAILWAY_SYSTEM_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "Event.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "Simulation.hpp"

class RailwaySystem {
 public:
  void AddNode(const Node& node);
  void AddRail(const Rail& rail);
  void AddEvent(const Event& event);
  void AddSimulation(const Simulation& simulation);
  Simulation& GetSimulation(const std::string& name);

  void PrintElementsData() const;
  void PrintSimulationsData() const;

 private:
  std::unordered_map<std::string, Node> nodes_;
  std::vector<Rail> rails_;
  std::vector<Event> events_;
  std::unordered_map<std::string, Simulation> simulations_;
};

#endif  // RAILWAY_SYSTEM_HPP
