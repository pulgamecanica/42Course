#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "RailwaySystem.hpp"
#include "Schedule.hpp"
#include "Train.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "EventMediator.hpp"
#include "CollisionMediator.hpp"
#include "FileLogger.hpp"

#include <vector>
#include <memory>
#include <string>

class Simulation {
 public:
  enum class State {
    kStarting,
    kRunning,
    kFinished
  };

  Simulation(const RailwaySystem& railSys, const Schedule& schedule, int id);

  std::vector<std::unique_ptr<RailSimulation>>& GetRails();
  const CollisionMediator&  GetCollisionMediator() const;
  const RailwaySystem&      GetRailwaySystem() const;
  const std::string&        GetDirectory() const;
  NodeSimulation*           GetNode(const std::string& nodeName);
  RailSimulation*           GetRailRef(const std::string& node1, const std::string& node2);
  double                    GetMaxTrainSpeed() const;
  unsigned int              GetCurrentTime() const;

  void Update();
  bool IsFinished() const;
  bool IsRailTwoWay() const;
 private:
  void InitializeNodes();
  void InitializeRails();
  void InitializeTrains();
  void CollectResults();
  // void HandleEvents();
  // void HandleCollisions();
  // void LogSimulationState();
  bool HasFinished() const;


  // References
  const RailwaySystem& rail_sys_;
  const Schedule& schedule_;

  // Identifiers
  const int   id_;
  const bool  rail_two_way_;

  // Loggers
  const std::string directory_;
  FileLogger logger_;

  // Mediators
  EventMediator event_mediator_;
  CollisionMediator collision_mediator_;

  // Logic
  State state_;
  double real_travel_time_;
  double max_speed_;
  unsigned start_time_;
  unsigned total_time_;

  // Attributes Owned
  std::vector<std::unique_ptr<NodeSimulation>> nodes_;
  std::vector<std::unique_ptr<RailSimulation>> rails_;
  std::vector<std::unique_ptr<TrainSimulation>> trains_;
};

#endif  // SIMULATION_HPP