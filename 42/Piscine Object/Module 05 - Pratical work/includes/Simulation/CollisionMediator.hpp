#ifndef COLLISION_MEDIATOR_HPP
#define COLLISION_MEDIATOR_HPP

#include <vector>

class TrainSimulation;

class CollisionMediator {
 public:
  void RegisterTrain(TrainSimulation* train);
  void DeregisterTrain(TrainSimulation* train);
  void CheckForCollisions();

 private:
  std::vector<TrainSimulation*> trains_;
};

#endif  // COLLISION_MEDIATOR_HPP
