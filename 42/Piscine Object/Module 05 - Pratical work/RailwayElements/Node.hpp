#ifndef NODE_HPP_
#define NODE_HPP_

#include "INode.hpp" // From Dijkstra's Lib
#include "raylib.h"

#include <unordered_map>
#include <string>
#include <vector>

class Event;

// Node Class
class Node: public INode<std::string> {
 public:
  Node(const std::string& name);
  ~Node();

  const std::unordered_map<std::string, uint>& GetNeighbors() const override;
  std::string GetName() const;
  std::string GetKey() const override;
  Vector2 GetPosition() const;
  const std::vector<Event*> GetEvents() const;
  Color GetColor() const;

  void SetPosition(Vector2 new_pos);
  void SetNeighbor(std::string, int weight) override;
  void AddEvent(Event* event);
 private:
  const std::string name_;
  Vector2 pos_;
  Color color_;
  std::unordered_map<std::string, uint> neighbors_;
  std::vector<Event*> events_;
};

#include "Subject.hpp"
#include "Event.hpp"

#include <memory>
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
  std::vector<std::unique_ptr<EventOccurrence>>& GetEventsOccurrences();
  bool IsNodeBlocked() const;

 private:
  const Node& node_;
  EventMediator* mediator_;
  std::vector<std::unique_ptr<EventOccurrence>> events_occurrences_;
};

#endif  // NODE_HPP_
