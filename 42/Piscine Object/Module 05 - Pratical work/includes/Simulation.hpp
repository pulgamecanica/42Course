#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <string>
#include <vector>
#include "Train.hpp"

#include "Graph.hpp" // From Dijkstra's Lib


class Simulation {
 public:
  explicit Simulation(const std::string& name) : name_(name) {}
  void SetGraph(Graph * graph);
  void AddTrain(const Train& train);
  void PrintTrainData() const;

  const std::string& GetName() const { return name_; }

 private:
  std::string name_;
  std::vector<Train> trains_;
  Graph * graph_; // To get the path info
};

#endif  // SIMULATION_HPP
