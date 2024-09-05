#ifndef NODE_HPP_
#define NODE_HPP_

#include <string>
#include <unordered_map>
#include "INode.hpp" // From Dijkstra's Lib
#include "raylib.h"

// Node Class
class Node: public INode<std::string> {
 public:
  Node(const std::string& name);
  ~Node();

  std::string GetKey() const override;
  void SetNeighbor(std::string, int weight) override;
  const std::unordered_map<std::string, uint>& GetNeighbors() const override;

  std::string GetName() const;
  Vector2 GetPosition() const;
  Color GetColor() const;

 private:
  const std::string name_;
  Vector2 pos_;
  Color color_;
  std::unordered_map<std::string, uint> neighbors_;
};

#include "Subject.hpp"

// Forward declaration of EventMediator class
class EventMediator;
class TrainSimulation;

// NodeSimulation Class
class NodeSimulation : public Subject {
 public:
  NodeSimulation(const Node& node, EventMediator* mediator);
  ~NodeSimulation();

  const Node& GetNode() const;
  EventMediator* GetMediator() const;

  void AddTrain(TrainSimulation* train);
  void RemoveTrain(TrainSimulation* train);

 private:
  const Node& node_;
  EventMediator* mediator_;
};

#endif  // NODE_HPP_
