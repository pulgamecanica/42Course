#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <string>

class Node;

// Event Class
class Event {
 public:
  Event(const std::string& type, double probability, double duration, const std::string& location);

  void SetNode(Node * node);
  // Getters
  const std::string& GetType() const;
  double GetProbability() const;
  double GetDuration() const;
  const std::string& GetLocation() const;
  Node* GetNode() const;

 private:
  const std::string type_;
  const double probability_;
  const double duration_;
  const std::string location_;
  Node * node_;
};

class EventOccurrence;

class EventSimulationState {
 public:
  EventSimulationState(const EventOccurrence& eo, const bool is_finished);

  bool IsFinished() const;
  const EventOccurrence& GetEventOccurrence() const;
  // Usefull functions
  const std::string& GetType() const;
  const std::string& GetLocationName() const;
  const std::string GetDurationString() const;
  const std::string GetStartString() const;
  double GetDuration() const;
 private:
  const EventOccurrence& event_occurrence_;
  const bool is_finished_;
};

class EventOccurrence {
 public:
  EventOccurrence(const Event& event, unsigned start);

  const Event& GetEvent() const;

  unsigned GetStart() const;
  unsigned GetFinishTime() const;
  bool IsFinished() const;

  EventSimulationState GetCurrentState() const;
  void SetFinished();
 private:
  const Event& event_;
  const unsigned start_;
  bool is_finished_;
};

#endif  // EVENT_HPP_
