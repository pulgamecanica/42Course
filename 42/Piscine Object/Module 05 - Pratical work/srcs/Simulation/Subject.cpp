#include "Simulation/Subject.hpp"
#include <algorithm>

void Subject::AddObserver(Observer* observer) {
  observers_.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer) {
  observers_.erase(
      std::remove(observers_.begin(), observers_.end(), observer),
      observers_.end());
}

void Subject::NotifyObservers() {
  for (auto* observer : observers_) {
    observer->Update(this);
  }
}
