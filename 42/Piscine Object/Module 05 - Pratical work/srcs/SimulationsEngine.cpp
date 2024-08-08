#include "SimulationsEngine.hpp"
#include "raylib.h"

SimulationsEngine::SimulationsEngine(RailwaySystem& system)
  : rail_sys_(system),
  current_state_(&menu_state_), // Start with the menu state
  menu_state_(*this),
  settings_state_(*this),
  schedules_state_(*this),
  network_state_(*this),
  simulations_state_(*this)
{
  // network_state_.SetGraph(graph_);
}

void SimulationsEngine::Run() {
  while (!WindowShouldClose()) {
    Update();
    Draw();
    // Update the railwaysystem and all simulations
  }
}

void SimulationsEngine::ChangeState(enum EngineStates e_state) {
  if (e_state == EngineStates::MENU)
    current_state_ = &menu_state_;
  else if (e_state == EngineStates::SETTINGS)
    current_state_ = &settings_state_;
  else if (e_state == EngineStates::NETWORK)
    current_state_ = &network_state_;
  else if (e_state == EngineStates::SCHEDULES)
    current_state_ = &schedules_state_;
  else if (e_state == EngineStates::SIMULATIONS)
    current_state_ = &simulations_state_;
  SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

RailwaySystem & SimulationsEngine::GetRailwaySystem() const {
  return rail_sys_;
}

void SimulationsEngine::Update() {
  if (current_state_)
    current_state_->Update();
}

// Liskov's Substitution SO(L)ID
// Game State Design pattern (current state with interface implementation)
void SimulationsEngine::Draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  if (current_state_)
    current_state_->Draw();
  EndDrawing();
}

// const Graph<std::string>& SimulationsEngine::GetGraph() const {
//     return rail_sys_.GetGraph();
// }
