#include "Visualizer/SchedulesState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
SchedulesState::SchedulesState(SimulationsEngine& engine) : engine_(engine) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  button_manager_.AddButton("Home", {25, 25, 100, 50}, [this]() { engine_.ChangeState(EngineStates::MENU); });
  button_manager_.AddButton("Simulation For Schedule 1", {50, 125, 100, 50}, [this]() { engine_.ChangeState(EngineStates::SIMULATIONS); });
}

void SchedulesState::Update() {
  button_manager_.UpdateButtons();
}

void SchedulesState::Draw() {
  DrawText("Schedules State", 350, 280, 20, BLACK);
  button_manager_.DrawButtons();
}
