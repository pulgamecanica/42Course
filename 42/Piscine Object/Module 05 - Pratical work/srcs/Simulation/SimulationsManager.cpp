// #include "Simulation/SimulationsManager.hpp"

// SimulationsManager::SimulationsManager(const RailwaySystem &rail_sys, const Schedule &schedule, int num_simulations) {
//   for (int i = 0; i < num_simulations; ++i) {
//     simulations_.emplace_back(std::make_unique<Simulation>(rail_sys, schedule));
//   }
// }

// void SimulationsManager::RunSimulations() {
//   for (auto &simulation : simulations_) {
//     simulation->Update();
//   }
//   CollectResults();
// }

// void SimulationsManager::CollectResults() {
//   double total_travel_time = 0;
//   for (const auto &simulation : simulations_) {
//     // Assuming Simulation class has a method to get the optimal travel time and events
//     double travel_time = simulation->GetOptimalTravelTime();
//     travel_times_.push_back(travel_time);
//     total_travel_time += travel_time;

//     auto events = simulation->GetEventList();
//     event_list_.insert(event_list_.end(), events.begin(), events.end());
//   }
// }

// double SimulationsManager::GetAverageTravelTime() const {
//   double total_time = 0;
//   for (const auto &time : travel_times_) {
//     total_time += time;
//   }
//   return travel_times_.empty() ? 0 : total_time / travel_times_.size();
// }

// std::vector<std::string> SimulationsManager::GetEventList() const {
//   return event_list_;
// }
