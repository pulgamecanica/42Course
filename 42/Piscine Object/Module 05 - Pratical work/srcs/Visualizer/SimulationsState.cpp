#include "Visualizer/SimulationsState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
SimulationsState::SimulationsState(SimulationsEngine& engine) : engine_(engine) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  button_manager_.AddButton("Home", {25, 25, 100, 25}, [this]() { engine_.ChangeState(EngineStates::MENU); });
}

void SimulationsState::Update() {
  button_manager_.UpdateButtons();
}

void SimulationsState::Draw() {
  DrawText("Simulations State", 350, 280, 20, BLACK);
  button_manager_.DrawButtons();
}
