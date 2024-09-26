#include "SimulationsManager.hpp"

SimulationsManager::SimulationsManager(const RailwaySystem &rail_sys, const Schedule &schedule, int num_simulations)
  : schedule_(schedule),
    state_(State::kStarting),
    ttm_(*this),
    time_running_(0),
    average_time_running(0) {
  if (num_simulations <= 0)
    throw std::runtime_error("Should execute at least 1 or more simulations");
  try {
    for (int i = 0; i < num_simulations; ++i)
      simulations_.emplace_back(std::make_unique<Simulation>(rail_sys, schedule, i));
  } catch (std::exception & e) {
    throw std::runtime_error(std::string("Couldn't initialize Simulation. ") + e.what());
  }
  start_time_ = simulations_[0]->GetStartTime();
  state_ = State::kRunning;
}

void SimulationsManager::UpdateSimulations() {
  if (state_ == State::kRunning) {
    for (auto &simulation : simulations_)
      if (!simulation->IsFinished())
        simulation->Update();
    if (AreSimulationsFinished()) {
      CollectResults();
      state_ = State::kFinished;
    }
    time_running_++;
  }
}

unsigned SimulationsManager::GetCurrentTime() const {
  return start_time_ + time_running_;
}

bool SimulationsManager::AreSimulationsFinished() const {
  for (const auto &simulation : simulations_)
    if (!simulation->IsFinished())
      return false;
  return true;
}

void SimulationsManager::CollectResults() {
  average_time_running = 0;
  for (const auto & sim : simulations_) {
    for (auto & train : sim->GetTrains())
      ttm_.AddObserver(train.get());
    average_time_running += sim->GetTotalTime();
  }
  average_time_running /= simulations_.size();
  ttm_.SetupAverages();
}

unsigned SimulationsManager::GetTrainAverageTime(const Train* train) const {
  return ttm_.GetTrainAverageTime(train);
}

unsigned SimulationsManager::GetTotalAverageTime() const {
  return average_time_running;
}

unsigned SimulationsManager::GetTotalTime() const {
  return time_running_;
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

const std::vector<std::unique_ptr<Simulation>>& SimulationsManager::GetSimulations() const {
  return simulations_;
}

Simulation& SimulationsManager::GetSimulation(int index) const {
  return *simulations_[index];
}

void SimulationsManager::LogSimulation(unsigned id) const {
  simulations_[id]->Log();
}

void SimulationsManager::LogSimulations() const {
  for (const auto & simulation: simulations_)
    simulation->Log();
}

