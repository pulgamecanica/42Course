#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "RailwaySystem.hpp"
#include "Schedule.hpp"
#include "Train.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "Event.hpp"
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
  std::vector<std::unique_ptr<TrainSimulation>>& GetTrains();
  std::vector<std::unique_ptr<NodeSimulation>>& GetNodes();
  const CollisionMediator&  GetCollisionMediator() const;
  const RailwaySystem&      GetRailwaySystem() const;
  const std::string&        GetDirectory() const;
  NodeSimulation*           GetNode(const std::string& nodeName);
  RailSimulation*           GetRailRef(const std::string& node1, const std::string& node2);
  double                    GetMaxTrainSpeed() const;
  unsigned int              GetCurrentTime() const;
  unsigned int              GetTotalTime() const;
  unsigned int              GetStartTime() const;

  void Update();
  bool IsFinished() const;
  bool IsRailTwoWay() const;
  void Log() const;

  const std::vector<std::shared_ptr<TrainSimulationState>>& GetSimulationTrainsState(int index);
  const std::vector<std::shared_ptr<EventSimulationState>>& GetSimulationEventsState(int index);
 private:
  void InitializeNodes();
  void InitializeRails();
  void InitializeTrains();
  void HandleCollisions();
  void HandleEvents();
  void LogSimulationState();
  bool HasFinished() const;

  // References
  const RailwaySystem& rail_sys_;
  const Schedule& schedule_;

  // Identifiers
  const int   id_;
  const bool  rail_two_way_;

  // Loggers
  const std::string directory_;

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
  std::vector<std::vector<std::shared_ptr<TrainSimulationState>>> trains_states_; // For each second (collection of seconds) we want the collection of trains
  std::vector<std::vector<std::shared_ptr<EventSimulationState>>> events_states_;
};

#endif  // SIMULATION_HPP
