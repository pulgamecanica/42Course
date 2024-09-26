#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include "Graph.hpp" // PathInfo -> From Dijkstra's Lib
#include "FileLogger.hpp"

class Train {
 public:
  Train(const std::string& name, double maxAcceleration, double maxBrakeForce, 
        const std::string& departure, const std::string& arrival, unsigned hour);

  const std::string&  GetName() const;
  const std::string&  GetDeparture() const;
  const std::string&  GetArrival() const;
  double              GetMaxAcceleration() const;
  double              GetMaxBrakeForce() const;
  unsigned            GetHour() const;

 private:
  std::string name_;
  std::string departure_;
  std::string arrival_;
  double      maxAcceleration_;
  double      maxBrakeForce_;
  unsigned    departure_hour_s_;
};

#include "Observer.hpp"

class RailSimulation;
class NodeSimulation;
class TrainSimulation;
class Simulation;

class TrainSimulationState {
public:
  TrainSimulationState(
    const TrainSimulation* train_simulation,
    const RailSimulation* current_rail,
    const NodeSimulation* current_node,
    const std::string&    next_node_name,
    const std::string& prev_node_name,
    float position_m,
    float speed,
    float acceleration,
    bool safe_distance,
    bool event_warning_stop,
    double total_distance);
  TrainSimulationState(const TrainSimulationState& tss) = default;
  const std::string GetCurrentPositionName() const;

  const RailSimulation* GetCurrentRail() const;
  const NodeSimulation* GetCurrentNode() const;
  const TrainSimulation* GetTrainSimulation() const;
  const std::string& GetNextNodeName() const;
  const std::string& GetPrevNodeName() const;
  const std::string& GetArrival() const;
  const std::string& GetDeparture() const;
  const std::string& GetName() const;
  const std::string GetHour() const;

  bool HasArrivedToNode() const;
  bool HasSafeDistance() const;
  bool HasEventWarningStop() const;

  float GetPosition() const;
  float GetSpeed() const;
  float GetAcceleration() const;
  double GetTotalDistance() const;
private:
  const TrainSimulation* train_simulation_;
  const RailSimulation* current_rail_;
  const NodeSimulation* current_node_;
  const std::string     next_node_name_;
  const std::string     prev_node_name_;
  
  const float           position_m_;
  const float           speed_;
  const float           acceleration_;

  const bool            safe_distance_;
  const bool            event_warning_stop_;

  const double          total_distance_;
};

class TrainSimulation : public Observer {
public:
  enum Status {
    kStoped,
    kAccelerating,
    kBraking,
    kMantaining,
  };

  TrainSimulation(Simulation& simulation, const Train& train);

  void  Update();
  void  Update(Subject* subject) override;
  void  ManageArrivalToNode();

  const Train & GetTrain() const;
  const std::string & GetNextNodeDestiny() const;
  const std::string   GetRailStringRep() const;
  const std::string& GetLogs() const;

  void SetMaxAcceleration(double acceleration);

  unsigned int GetCurrentTime() const;
  unsigned int GetTotalTime() const;
  double  GetOptimalTime() const;
  double  GetMaxAccelerationForce() const;
  float   GetStoppingDistance() const;
  float   GetPosition() const;
  float   GetPositionPercentage() const;
  
  bool        HasFinished() const;
  bool        InvalidPath() const;

  TrainSimulationState GetCurrentState() const;
private:
  void Accelerate();
  void Brake();
  void UpdatePosition();
  void UpdateStatus();
  void SubscribeToNode(NodeSimulation* node);
  void SubscribeToRail(RailSimulation* rail);
  void UnsubscribeCurrentNode();
  void UnsubscribeCurrentRail();
  void CalculateFastestRoute();
  void StartRoute();
  void Log();

  bool ShouldStop();
  bool CanStart() const;
  bool HasStoped() const;
  bool HasArrivedToNode() const;
  bool TraveledAllRail() const;
  bool IsTimeToStart() const;
  bool HasSafeDistance();

  double GetMaxBrakeForce() const;
  double GetOptimalTimeForDistance(double distance) const;

  Simulation&     simulation_;
  const Train&          train_;
  const NodeSimulation& node_source_;
  const NodeSimulation& node_destiny_;

  // FileLogger logger_;

  PathInfo<std::string> path_info_;

  std::string train_logs_;

  RailSimulation* current_rail_;
  NodeSimulation* current_node_;
  std::string     next_node_name_;
  std::string     prev_node_name_;
  
  float           position_m_; // Meters
  float           speed_; // m / s
  float           acceleration_;
  
  enum Status     status_;
  bool            event_warning_stop_;
  bool            train_can_start_;
  bool            has_safe_distance_;

  double          optimal_time_;
  double          max_acceleration_;
  double          total_distance_;
  unsigned        time_running_s_;
};

#endif  // TRAIN_HPP
