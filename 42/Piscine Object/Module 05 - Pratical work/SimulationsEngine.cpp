#include "SimulationsEngine.hpp"
#include "Settings.hpp"
#include "raylib.h"

SimulationsEngine::SimulationsEngine(const std::string& elementsFileName, const std::string& scheduleDirectory)
  : rail_sys_(elementsFileName, scheduleDirectory),
    current_state_(&menu_state_), // Start with the menu state
    menu_state_(*this),
    settings_state_(*this),
    network_state_(*this),
    schedules_state_(*this),
    simulation_state_(*this)
{
}

void SimulationsEngine::Run() {
  while (!WindowShouldClose()) {
    Update();
    Draw();
    // Update Settings animations
    Settings::Instance().UpdateAnimations();
  }
}

void SimulationsEngine::ChangeState(enum EngineStates e_state) {
  if (e_state == EngineStates::kMenu)
    current_state_ = &menu_state_;
  else if (e_state == EngineStates::kSettings)
    current_state_ = &settings_state_;
  else if (e_state == EngineStates::kNetwork)
    current_state_ = &network_state_;
  else if (e_state == EngineStates::kSchedules)
      current_state_ = &schedules_state_;
  else if (e_state == EngineStates::kSimulation)
      current_state_ = &simulation_state_;
  SetMouseCursor(MOUSE_CURSOR_DEFAULT);
}

RailwaySystem & SimulationsEngine::GetRailwaySystem() {
  return rail_sys_;
}

void SimulationsEngine::Update() {
  if (current_state_)
    current_state_->Update();
  for (auto & manager : simulations_managers_)
    manager->UpdateSimulations();
}

// // Liskov's Substitution SO(L)ID
// // Game State Design pattern (current state with interface implementation)
void SimulationsEngine::Draw() {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  if (current_state_)
    current_state_->Draw();
  EndDrawing();
}

SimulationsManager * SimulationsEngine::GenerateSimulations(const Schedule &schedule, int amount) {
  simulations_managers_.emplace_back(std::make_unique<SimulationsManager>(rail_sys_, schedule, amount));
  return simulations_managers_.back().get();
}

void SimulationsEngine::SetSimulationsManager(const SimulationsManager* manager) {
  simulation_state_.SetSimulationsManager(manager);
  simulation_state_.SetupNewSimulation();
}

