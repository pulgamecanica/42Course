#include "Visualizer/NetworkState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
NetworkState::NetworkState(SimulationsEngine& engine)
  : engine_(engine), 
    editable_grid_(
      engine.GetRailwaySystem(),
      20.0f,
      {50, 50, GetScreenWidth() - 100, GetScreenHeight() - 100}) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  button_manager_.AddButton("Home", {5, 5, 50, 25}, [this]() { engine_.ChangeState(EngineStates::MENU); });
  button_manager_.AddButton("Show Grid", {screenWidth - 105, 5, 100, 25}, [this]() { editable_grid_.ToggleShowGrid(); });
}

void NetworkState::Update() {
  editable_grid_.Update();
  button_manager_.UpdateButtons();
}

std::string VecToString(Vector2 vec) {
  return ("[" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "]");
}

void NetworkState::Draw() {
  editable_grid_.Draw();
  DrawText((std::string("Mouse Pos: ") + VecToString(GetMousePosition())).c_str(),
    20, GetScreenHeight() - 20, 10, BLACK);
  DrawText((std::string("Grid Pos: ") + VecToString(editable_grid_.GetRelativeCoordinates(GetMousePosition()))).c_str(),
    220, GetScreenHeight() - 20, 10, BLACK);
  DrawText((std::string("Offset: ") + VecToString(editable_grid_.GetOffset())).c_str(),
    420, GetScreenHeight() - 20, 10, BLACK);
  button_manager_.DrawButtons();
}
