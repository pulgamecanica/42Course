#ifndef NODE_SIMULATION_HPP
#define NODE_SIMULATION_HPP

#include <vector>
#include "Subject.hpp"
#include "Node.hpp"
#include "EventMediator.hpp"
#include "TrainSimulation.hpp"

class NodeSimulation : public Subject {
 public:
  explicit NodeSimulation(const Node & node, EventMediator* mediator);
  
  void AddTrain(TrainSimulation* train);
  void RemoveTrain(TrainSimulation* train);
  const std::string& GetName() const;

 private:
  const Node & node_;
  EventMediator* mediator_;
};

bool operator==(const NodeSimulation &lhs, const NodeSimulation &rhs);

#endif  // NODE_SIMULATION_HPP
