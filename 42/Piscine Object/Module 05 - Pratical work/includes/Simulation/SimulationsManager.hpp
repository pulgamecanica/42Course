// #ifndef SIMULATIONS_MANAGER_HPP
// #define SIMULATIONS_MANAGER_HPP

// #include "Simulation/Simulation.hpp"
// #include <vector>
// #include <memory>

// class SimulationsManager {
// public:
//   SimulationsManager(const RailwaySystem &rail_sys, const Schedule &schedule, int num_simulations);
  
//   // Run all simulations
//   void RunSimulations();

//   // Get results
//   double GetAverageTravelTime() const;
//   std::vector<std::string> GetEventList() const;

// private:
//   std::vector<std::unique_ptr<Simulation>> simulations_;
//   std::vector<double> travel_times_;
//   std::vector<std::string> event_list_;

//   void CollectResults();
// };

// #endif  // SIMULATIONS_MANAGER_HPP
