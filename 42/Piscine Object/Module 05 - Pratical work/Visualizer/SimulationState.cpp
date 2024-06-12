#include "SimulationState.hpp"
#include "Game.hpp"

#include "raylib.h"

// Constructor initializes the state with a pointer to the Game object
SimulationState::SimulationState(Game* game) : game_(game) {}

void SimulationState::Update() {
  // Simulation state logic goes here
}

void SimulationState::Draw() {
  DrawText("Simulation State", 350, 280, 20, BLACK);
}
