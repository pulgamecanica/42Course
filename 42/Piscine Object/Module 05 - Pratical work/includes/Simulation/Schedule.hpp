#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <string>
#include <vector>
#include "Train.hpp"

#include "Graph.hpp" // From Dijkstra's Lib


class Schedule {
 public:
  explicit Schedule(const std::string& name, const Graph<std::string> &graph);
  void AddTrain(const Train& train);
  void PrintTrainData() const;
  const std::vector<Train> GetTrains() const;
  const std::string& GetName() const { return name_; }

 private:
  std::string name_;
  std::vector<Train> trains_;
  const Graph<std::string> &graph_; // To get the path info
};

#endif  // SIMULATION_HPP
