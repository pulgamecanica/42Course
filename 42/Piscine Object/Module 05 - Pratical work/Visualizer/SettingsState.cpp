#include "SettingsState.hpp"
#include "Game.hpp"

#include "raylib.h"

// Constructor initializes the state with a pointer to the Game object
SettingsState::SettingsState(Game* game) : game_(game) {}

void SettingsState::Update() {
  // Settings state logic goes here
}

void SettingsState::Draw() {
  DrawText("Settings State", 350, 280, 20, BLACK);
}
