#ifndef GAME_HPP
#define GAME_HPP

#include "Simulation/RailwaySystem.hpp"
#include "Visualizer/IState.hpp"
#include "Visualizer/MenuState.hpp"
#include "Visualizer/SettingsState.hpp"
#include "Visualizer/SchedulesState.hpp"
#include "Visualizer/NetworkState.hpp"
#include "Visualizer/SimulationsState.hpp"
#include "Graph.hpp" // From Dijkstra's Library

enum EngineStates {
  MENU,
  SETTINGS,
  NETWORK,
  SCHEDULES,
  SIMULATIONS
};

class SimulationsEngine {
public:
  SimulationsEngine(RailwaySystem& system);
  void Run();
  void ChangeState(enum EngineStates e_state);
  RailwaySystem & GetRailwaySystem() const;
  // const Graph<std::string>& GetGraph() const;
private:
  void Update();
  void Draw();

  RailwaySystem &rail_sys_;
  IState* current_state_; // Desgin Pattern "Current State w/ interface"
  // Persistent state instances
  MenuState           menu_state_;
  SettingsState       settings_state_;
  SchedulesState      schedules_state_;
  NetworkState        network_state_;
  SimulationsState    simulations_state_;
};

#endif // GAME_HPP