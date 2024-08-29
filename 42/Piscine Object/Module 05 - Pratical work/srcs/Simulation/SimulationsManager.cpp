#include "Simulation/SimulationsManager.hpp"

SimulationsManager::SimulationsManager(const RailwaySystem &rail_sys, const Schedule &schedule, int num_simulations)
  : state_(State::Starting), schedule_(schedule) {
  if (num_simulations <= 0)
    throw std::runtime_error("Should execute at least 1 or more simulations");
  for (int i = 0; i < num_simulations; ++i) {
    simulations_.emplace_back(rail_sys, schedule);
  }
  optimal_travel_time_ = simulations_[0].GetOptimalTravelTime();
  state_ = State::Running;
}

void SimulationsManager::UpdateSimulations() {
  if (state_ == State::Running) {
    int i = 0;
    for (auto &simulation : simulations_)
      if (!simulation.IsFinished())
        simulation.Update();
    if (AreSimulationsFinished()) {
      // CollectResults();
      state_ = State::Finished;
    }
  }
}

bool SimulationsManager::AreSimulationsFinished() const {
  for (const auto &simulation : simulations_)
    if (!simulation.IsFinished())
      return false;
  return true;
}

void SimulationsManager::CollectResults() {
  travel_times_.clear();
  event_list_.clear();

  for (const auto &simulation : simulations_) {
    double travel_time = simulation.GetRealTravelTime();
    travel_times_.push_back(travel_time);

    // auto events = simulation->GetEventList();
    // event_list_.insert(event_list_.end(), events.begin(), events.end());
  }
}

double SimulationsManager::GetAverageTravelTime() const {
  double total_time = 0;
  for (const auto &time : travel_times_) {
    total_time += time;
  }
  return travel_times_.empty() ? 0 : total_time / travel_times_.size();
}

const Schedule & SimulationsManager::GetSchedule() const {
  return schedule_;
}

std::vector<std::string> SimulationsManager::GetEventList() const {
  return event_list_;
}

std::vector<Simulation> SimulationsManager::GetSimulations() const {
  return simulations_;
}