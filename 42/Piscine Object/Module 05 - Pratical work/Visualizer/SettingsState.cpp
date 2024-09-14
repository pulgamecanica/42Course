#include "SettingsState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
SettingsState::SettingsState(SimulationsEngine& engine) : engine_(engine) {
  // float screenHeight = GetScreenHeight();
  // float screenWidth = GetScreenWidth();
  // button_manager_.AddButton("Home", {25, 25, 100, 50}, [this]() { engine_.ChangeState(EngineStates::MENU); });
}

void SettingsState::Update() {
  // button_manager_.UpdateButtons();
}

void SettingsState::Draw() {
  // DrawText("Settings State", 350, 280, 20, BLACK);
  // button_manager_.DrawButtons();
}