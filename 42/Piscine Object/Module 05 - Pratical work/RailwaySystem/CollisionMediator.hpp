#ifndef COLLISION_MEDIATOR_HPP_
#define COLLISION_MEDIATOR_HPP_

#include <string>

class TrainSimulation;
class RailSimulation;
class Simulation;

class CollisionMediator {
 public:
  CollisionMediator(Simulation & simulation);
  void CheckForCollisions();
  bool CanJoinRail(RailSimulation & rail, const std::string dest, const bool two_way) const;
 private:
  Simulation & simulation_;
};

#endif  // COLLISION_MEDIATOR_HPP_