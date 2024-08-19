#ifndef EVENT_MEDIATOR_HPP
#define EVENT_MEDIATOR_HPP

#include <vector>

class NodeSimulation;

class EventMediator {
 public:
  void RegisterNode(NodeSimulation* node);
  void DeregisterNode(NodeSimulation* node);
  void UpdateEvents();
  void CheckForEvents();

 private:
  std::vector<NodeSimulation*> nodes_;
};

#endif  // EVENT_MEDIATOR_HPP
