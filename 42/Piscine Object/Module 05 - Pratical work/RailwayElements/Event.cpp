#include "Event.hpp"

// Event Implementation
Event::Event(const std::string& type, double probability, double duration, const std::string& location)
    : type_(type), probability_(probability), duration_(duration), location_(location) {}

std::string Event::GetType() const {
  return type_;
}

double Event::GetProbability() const {
  return probability_;
}

double Event::GetDuration() const {
  return duration_;
}

std::string Event::GetLocation() const {
  return location_;
}

// Event Occurrence Implementation

EventOccurrence::EventOccurrence(const Event& e, unsigned start)
  : event_(e), start_(start) {}

bool EventOccurrence::IsFinished(unsigned time) const {
  return start_ + event_.GetDuration() <= time;
}