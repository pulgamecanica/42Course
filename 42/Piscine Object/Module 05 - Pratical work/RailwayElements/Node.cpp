#include "Node.hpp"
#include "Event.hpp"
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

void Node::AddEvent(Event* event) {
  events_.push_back(event);
}

const std::vector<Event*> Node::GetEvents() const {
  return events_;
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

void Node::SetPosition(Vector2 new_pos) {
  pos_ = new_pos;
}

// NodeSimulation Implementation
NodeSimulation::NodeSimulation(const Node& node, EventMediator* mediator)
    : node_(node), mediator_(mediator) {    }

NodeSimulation::~NodeSimulation() {}

const Node& NodeSimulation::GetNode() const {
  return node_;
}

EventMediator* NodeSimulation::GetMediator() const {
  return mediator_;
}

bool NodeSimulation::IsNodeBlocked() const {
  for (const auto& event : events_occurrences_)
    if (!event->IsFinished())
      return true;
  return false;
}

#include <random>

// https://en.cppreference.com/w/cpp/numeric/random

std::knuth_b rand_engine;
std::uniform_real_distribution<> uniform_zero_to_one(0.0, 1.0);
void NodeSimulation::AddTrain(TrainSimulation* train) {
  AddObserver(train);
  // Possibly throw an event, train should subscribe to events
  for (const auto & event : node_.GetEvents()) {
    bool prob = uniform_zero_to_one(rand_engine) >= event->GetProbability();
    if (!prob) {
      events_occurrences_.emplace_back(std::make_unique<EventOccurrence>(*event, train->GetCurrentTime()));
    }
  }
  // EventMediator will update the train when the event is done
}

bool random_bool_with_prob( double prob )  // probability between 0.0 and 1.0
{
    return uniform_zero_to_one(rand_engine) >= prob;
}

void NodeSimulation::RemoveTrain(TrainSimulation* train) {
  RemoveObserver(train);
}

std::vector<std::unique_ptr<EventOccurrence>>& NodeSimulation::GetEventsOccurrences() {
  return events_occurrences_;
}