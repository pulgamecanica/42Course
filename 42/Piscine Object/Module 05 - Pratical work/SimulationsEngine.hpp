#ifndef GAME_HPP
#define GAME_HPP

#include "RailwaySystem.hpp"
#include "IState.hpp"
#include "MenuState.hpp"
#include "SettingsState.hpp"
#include "SchedulesState.hpp"
#include "NetworkState.hpp"
#include "SimulationState.hpp"
#include "SimulationsManager.hpp"

enum EngineStates {
  kMenu,
  kSettings,
  kNetwork,
  kSchedules,
  kSimulation
};

class SimulationsEngine {
public:
  SimulationsEngine(const std::string& elementsFileName, const std::string& scheduleDirectory);

  void Run();
  void ChangeState(enum EngineStates e_state);

  RailwaySystem & GetRailwaySystem();

  SimulationsManager* GenerateSimulations(const Schedule &schedule, int amount);
private:
  void Update();
  void Draw();

  RailwaySystem rail_sys_;
  std::vector<std::unique_ptr<SimulationsManager>> simulations_managers_;
  IState* current_state_; // Desgin Pattern "Current State w/ interface"
  // // Persistent state instances
  MenuState           menu_state_;
  SettingsState       settings_state_;
  NetworkState        network_state_;
  // SimulationsState    simulations_state_;
  // SchedulesState      schedules_state_;
};

#endif // GAME_HPP