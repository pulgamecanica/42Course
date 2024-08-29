#ifndef RAIL_SIMULATION_HPP
#define RAIL_SIMULATION_HPP

#include <vector>
#include "Subject.hpp"
#include "Rail.hpp"
#include "CollisionMediator.hpp"
#include "TrainSimulation.hpp"

class RailSimulation : public Subject {
  enum Direction
  {
    DIR1TO2,
    DIR2TO1,
    NODIR
  };
 public:
  explicit RailSimulation(const Rail & rail, CollisionMediator* mediator);
  
  void CheckForCollisions();
  void AddTrain(TrainSimulation* train);
  void RemoveTrain(TrainSimulation* train);

  double  Distance() const;

  bool  HasNodes(const std::string & node1, const std::string & node2) const;
 private:
  const Rail & rail_;
  // Direction
  CollisionMediator* mediator_;
};

#endif  // RAIL_SIMULATION_HPP
