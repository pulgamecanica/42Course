#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "Simulation/RailwaySystem.hpp"
#include "Simulation/Schedule.hpp"
#include "Simulation/TrainSimulation.hpp"
#include "Simulation/NodeSimulation.hpp"
#include "Simulation/RailSimulation.hpp"
#include "Simulation/EventMediator.hpp"
#include "Simulation/CollisionMediator.hpp"
#include "Simulation/FileLogger.hpp"

#include <iostream>
#include <vector>
#include <string>

class Simulation {
public:
  Simulation(const RailwaySystem &rail_sys_, const Schedule & schedule);
  void Update();
private:
  void InitializeNodes();
  void InitializeRails();
  void InitializeTrains();
  void HandleEvents();
  void HandleCollisions();
  void LogSimulationState();
  const RailwaySystem           &rail_sys_;
  const Schedule                &schedule_;
  const std::string             logger_filename_;
  const FileLogger              logger_;
  EventMediator                 event_mediator_;
  CollisionMediator             collision_mediator_;
  std::vector<NodeSimulation>   nodes_;
  std::vector<RailSimulation>   rails_;
  std::vector<TrainSimulation>  trains_;
};

#endif  // SIMULATION_HPP
