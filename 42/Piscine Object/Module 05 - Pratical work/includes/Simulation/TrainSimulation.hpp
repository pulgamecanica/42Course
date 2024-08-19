#ifndef TRAIN_SIMULATION_HPP
#define TRAIN_SIMULATION_HPP

#include "Train.hpp"
#include "Observer.hpp"

class RailSimulation;
class NodeSimulation;

enum TrainStatus
{
  STOPED,
  WAITING,
  MOVING
};

class TrainSimulation : public Observer {
 public:
  TrainSimulation(const Train & train);
  
  void Update();
  void Update(Subject* subject) override;
  void SetRail(RailSimulation* rail);
  void SetNode(NodeSimulation* rail);
  void Move();
  void SlowDown();
  float CalculateStoppingDistance() const;

  // Getters and setters as needed
 private:
  const Train & train_;
  float speed_;
  RailSimulation* rail_;
  NodeSimulation* node_;
  float position_;
  enum TrainStatus status_;
};

#endif  // TRAIN_SIMULATION_HPP
