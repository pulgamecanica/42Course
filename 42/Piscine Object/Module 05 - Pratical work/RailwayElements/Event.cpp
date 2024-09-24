#include "Event.hpp"
#include "Node.hpp"
#include "Parser.hpp"

// Event Implementation
Event::Event(const std::string& type, double probability, double duration, const std::string& location)
    : type_(type), probability_(probability), duration_(duration), location_(location), node_(nullptr) {}

const std::string& Event::GetType() const {
  return type_;
}

double Event::GetProbability() const {
  return probability_;
}

double Event::GetDuration() const {
  return duration_;
}

const std::string& Event::GetLocation() const {
  return location_;
}

void Event::SetNode(Node * node) {
  node_ = node;
}

Node* Event::GetNode() const {
  return node_;
}

/// EventSimulationState Implementation

EventSimulationState::EventSimulationState(const EventOccurrence& eo, const bool is_finished)
  :  event_occurrence_(eo), is_finished_(is_finished) {}

bool EventSimulationState::IsFinished() const {
  return is_finished_;
}

// EventOccurrence Implementation
EventOccurrence::EventOccurrence(const Event& e, unsigned start)
  : event_(e), start_(start), is_finished_(false) {}

unsigned EventOccurrence::GetStart() const {
  return start_;
}

unsigned EventOccurrence::GetFinishTime() const {
  return GetStart() + event_.GetDuration();
}

bool EventOccurrence::IsFinished() const {
  return is_finished_;
}

void EventOccurrence::SetFinished() {
  is_finished_ = true;
}

const Event& EventOccurrence::GetEvent() const {
  return event_;
}

EventSimulationState EventOccurrence::GetCurrentState() const {
  return EventSimulationState(*this, IsFinished());
}

const EventOccurrence& EventSimulationState::GetEventOccurrence() const {
  return event_occurrence_;
}

const std::string& EventSimulationState::GetType() const {
  return event_occurrence_.GetEvent().GetType();
}

const std::string& EventSimulationState::GetLocationName() const {
  return event_occurrence_.GetEvent().GetLocation();
}

const std::string EventSimulationState::GetDurationString() const {
  return Parser::ConvertToTimeStringHHMMSS(event_occurrence_.GetEvent().GetDuration());
}

const std::string EventSimulationState::GetStartString() const {
  return Parser::ConvertToTimeStringHHMMSS(event_occurrence_.GetStart());
}

double EventSimulationState::GetDuration() const {
  return event_occurrence_.GetEvent().GetDuration();
}