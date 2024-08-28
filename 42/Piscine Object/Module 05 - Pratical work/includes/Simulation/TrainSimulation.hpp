#ifndef TRAIN_SIMULATION_HPP
#define TRAIN_SIMULATION_HPP

#include "Train.hpp"
#include "Observer.hpp"

class RailSimulation;
class NodeSimulation;
class Simulation;

class TrainSimulation : public Observer {
public:
  enum Status {
    STOPED,
    ACCELERATING,
    BRAKING,
    MANTAINING,
  };

  TrainSimulation(Simulation& simulation, const Train& train);
  void    Update();
  void    Update(Subject* subject) override;
  void    SetRail(RailSimulation* rail);
  void    SetNode(NodeSimulation* node);

  std::string GetName() const;

  float   CalculateStoppingDistance() const;
  double  GetOptimalTime() const;

private:
  void Accelerate();
  void Brake();
  void UpdatePosition();
  void UpdateStatus();
  void StartMoving();
  void SubscribeNode(NodeSimulation& node);
  void SubscribeRail(RailSimulation& rail);
  void UnsubscribeCurrentNode();
  void UnsubscribeCurrentRail();
  void CalculateFastestRoute();
  void StartRoute();

  bool ShouldStop() const;
  bool CanStart() const;
  bool HasFinished() const;
  bool HasStoped() const;
  bool HasArrivedToNode() const;
  bool TraveledAllRail() const;

  double GetMaxAccelerationForce() const;
  double GetMaxBrakeForce() const;
  const std::string GetArrivalName() const;
  const std::string GetDepartureName() const;
  double TimePassedS() const;

  const Simulation&     simulation_;
  const Train&          train_;
  const NodeSimulation& node_source_;
  const NodeSimulation& node_destiny_;

  RailSimulation* current_rail_;
  NodeSimulation* current_node_;
  float           position_m_; // Meters
  float           speed_; // m / s
  float           acceleration_;
  enum Status     status_;
  bool            has_safe_distance_;
};

#endif  // TRAIN_SIMULATION_HPP
