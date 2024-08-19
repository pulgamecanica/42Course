#ifndef RAIL_SIMULATION_HPP
#define RAIL_SIMULATION_HPP

#include <vector>
#include "Subject.hpp"
#include "Rail.hpp"
#include "CollisionMediator.hpp"
#include "TrainSimulation.hpp"

class RailSimulation : public Subject {
 public:
  explicit RailSimulation(const Rail & rail, CollisionMediator* mediator);
  
  void CheckForCollisions();
  void AddTrain(TrainSimulation* train);
  void RemoveTrain(TrainSimulation* train);

 private:
  const Rail & rail_;
  CollisionMediator* mediator_;
};

#endif  // RAIL_SIMULATION_HPP
