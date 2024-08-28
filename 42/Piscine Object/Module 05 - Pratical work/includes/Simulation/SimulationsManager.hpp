#ifndef SIMULATIONS_MANAGER_HPP
#define SIMULATIONS_MANAGER_HPP

#include "Simulation/Simulation.hpp"
#include <vector>
#include <memory>
#include <string>

class SimulationsManager {
public:
  enum class State {
    Starting,
    Running,
    Finished
  };

  SimulationsManager(const RailwaySystem &rail_sys, const Schedule &schedule, int num_simulations);
  void UpdateSimulations();
  bool AreSimulationsFinished() const;

  double GetAverageTravelTime() const;
  const Schedule & GetSchedule() const;
  std::vector<std::string> GetEventList() const;
  std::vector<Simulation> GetSimulations() const;

private:
  void InitializeSimulations();
  void CollectResults();

  State state_;
  const Schedule & schedule_;
  std::vector<Simulation> simulations_;
  std::vector<double> travel_times_;
  std::vector<std::string> event_list_;
  double optimal_travel_time_;
};

#endif  // SIMULATIONS_MANAGER_HPP
