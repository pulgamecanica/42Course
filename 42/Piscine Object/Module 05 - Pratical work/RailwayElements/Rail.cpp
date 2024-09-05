#include "Rail.hpp"
#include "Train.hpp"

// Rail Implementation
Rail::Rail(const std::string& node1, const std::string& node2, double distance)
    : node1_(node1), node2_(node2), distance_(distance) {}

Rail::~Rail() {}

std::string Rail::GetNode1() const {
  return node1_;
}

std::string Rail::GetNode2() const {
  return node2_;
}

double Rail::GetDistance() const {
  return distance_;
}

// RailSimulation Implementation
RailSimulation::RailSimulation(const Rail& rail, CollisionMediator* mediator)
    : rail_(rail), mediator_(mediator) {}

RailSimulation::~RailSimulation() {}

const Rail& RailSimulation::GetRail() const {
  return rail_;
}

CollisionMediator* RailSimulation::GetMediator() const {
  return mediator_;
}

void RailSimulation::AddTrain(TrainSimulation* train) {
  AddObserver(train);
}

void RailSimulation::RemoveTrain(TrainSimulation* train) {
  RemoveObserver(train);
}

bool RailSimulation::HasNodes(const std::string & node1, const std::string & node2) const {
  return (
    (rail_.GetNode1() == node1 && rail_.GetNode2() == node2) ||
    (rail_.GetNode1() == node2 && rail_.GetNode2() == node1)
    );
}
