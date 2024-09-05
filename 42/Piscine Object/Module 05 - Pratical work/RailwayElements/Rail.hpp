#ifndef RAIL_HPP_
#define RAIL_HPP_

#include <string>

// Rail Class
class Rail {
 public:
  Rail(const std::string& node1, const std::string& node2, double distance);
  ~Rail();

  std::string GetNode1() const;
  std::string GetNode2() const;
  double GetDistance() const;

 private:
  const std::string node1_;
  const std::string node2_;
  const double distance_;
};

#include "Subject.hpp"

// Forward declaration of CollisionMediator class
class CollisionMediator;
class TrainSimulation;

// RailSimulation Class
class RailSimulation : public Subject {
 public:
  RailSimulation(const Rail& rail, CollisionMediator* mediator);
  ~RailSimulation();

  const Rail& GetRail() const;
  CollisionMediator* GetMediator() const;

  void AddTrain(TrainSimulation* train);
  void RemoveTrain(TrainSimulation* train);

  bool HasNodes(const std::string & node1, const std::string & node2) const;

 private:
  const Rail& rail_;
  CollisionMediator* mediator_;
};

#endif  // RAIL_HPP_
