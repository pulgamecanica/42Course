/**
 * Why Use the k Prefix?
 * 
 * Clarity: It makes it clear that the value is a constant or an enumerator, which can help avoid confusion with variables or other identifiers that are not constants.
 * 
 * Consistency: It follows a common convention used in many codebases, especially in projects with a strict coding style.
 **/

#ifndef SIMULATIONS_MANAGER_HPP
#define SIMULATIONS_MANAGER_HPP

#include "Simulation.hpp"
#include "TrainsTimeMediator.hpp"
#include <vector>
#include <memory>
#include <string>

class SimulationsManager {
 public:
  // Enum for simulation states
  enum class State {
    kStarting,
    kRunning,
    kFinished
  };

  SimulationsManager(const RailwaySystem& railSys, const Schedule& schedule, int numSimulations);

  void UpdateSimulations();
  bool AreSimulationsFinished() const;

  unsigned GetTotalAverageTime() const;
  unsigned GetTotalTime() const;
  double GetAverageTravelTime() const;
  const std::vector<std::unique_ptr<Simulation>>& GetSimulations() const;
  const Schedule& GetSchedule() const;
  std::vector<std::string> GetEventList() const;
  Simulation& GetSimulation(int index) const;
  unsigned    GetCurrentTime() const;
  unsigned    GetTrainAverageTime(const Train* train) const;
  void LogSimulation(unsigned id) const;
  void LogSimulations() const;
  
 private:
  void InitializeSimulations();
  void CollectResults();

  const Schedule& schedule_;
  State state_;
  TrainsTimeMediator ttm_;

  std::vector<std::unique_ptr<Simulation>>  simulations_;
  std::vector<double>                       travel_times_;
  std::vector<std::string>                  event_list_;
  unsigned                                  start_time_;
  unsigned                                  time_running_;
  unsigned                                  average_time_running;
};

#endif  // SIMULATIONS_MANAGER_HPP
