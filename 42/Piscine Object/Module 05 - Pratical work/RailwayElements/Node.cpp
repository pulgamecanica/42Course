#include "Node.hpp"
#include "Train.hpp"
#include <cmath>

// Node Implementation
Node::Node(const std::string& name)
    : name_(name), pos_(rand() % 600, rand() % 600), color_(BLUE) {}

Node::~Node() {}

// Interface Implementation
std::string Node::GetKey() const {
  return name_;
}

const std::unordered_map<std::string, uint>& Node::GetNeighbors() const {
  return neighbors_;
}

void Node::SetNeighbor(std::string name, int weight) {
  neighbors_[name] = weight;
}

// Node Implementation
std::string Node::GetName() const {
  return name_;
}

Vector2 Node::GetPosition() const {
  return pos_;
}

Color Node::GetColor() const {
  return color_;
}

// NodeSimulation Implementation
NodeSimulation::NodeSimulation(const Node& node, EventMediator* mediator)
    : node_(node), mediator_(mediator) {}

NodeSimulation::~NodeSimulation() {}

const Node& NodeSimulation::GetNode() const {
  return node_;
}

EventMediator* NodeSimulation::GetMediator() const {
  return mediator_;
}

void NodeSimulation::AddTrain(TrainSimulation* train) {
  AddObserver(train);
}

void NodeSimulation::RemoveTrain(TrainSimulation* train) {
  RemoveObserver(train);
}
