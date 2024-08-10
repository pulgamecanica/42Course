#include "Visualizer/NetworkState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
NetworkState::NetworkState(SimulationsEngine& engine)
  : engine_(engine), 
    editable_grid_(
      engine.GetRailwaySystem(),
      20.0f,
      {85, 100, GetScreenWidth() - 85 * 2, GetScreenHeight() - 100 * 2}) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  blue_train_animation_y_ = screenHeight - 100;
  blue_train_pos_x_factor = 1.0f;
  {
    std::vector<const char*> images = { "assets/images/background_network.png" };
    std::map<std::string, int> options = {{"width", screenWidth}, {"height", screenHeight} };
    background_animation_ = std::make_unique<Animation>(images, 1.0f, options);
  }
  {
    std::vector<const char*> images = {
      "assets/images/blue_train_frame_1.png",
      "assets/images/blue_train_frame_2.png",
      "assets/images/blue_train_frame_3.png",
      "assets/images/blue_train_frame_4.png",
      "assets/images/blue_train_frame_5.png",
      "assets/images/blue_train_frame_6.png",
      "assets/images/blue_train_frame_7.png",
      "assets/images/blue_train_frame_8.png",
    };
    std::map<std::string, int> options = {{"width", 120}, {"height", 24}};
    blue_train_ = std::make_unique<Animation>(images, 0.15f, options);
  }
  {
    std::vector<const char*> images = { "assets/images/blue_train_rails.png" };
    std::map<std::string, int> options = {{"width", GetScreenWidth() / 2}, {"height", 40} };
    blue_train_rails_ = std::make_unique<Animation>(images, 1.0f, options);
  }
  {
    std::vector<const char*> images = {
      "assets/images/blue_train_woods_frame_1.png",
      "assets/images/blue_train_woods_frame_2.png",
      "assets/images/blue_train_woods_frame_3.png",
      "assets/images/blue_train_woods_frame_4.png",
    };
    std::map<std::string, int> options = {{"width", GetScreenWidth() / 2}, {"height", 40} };
    blue_train_woods_ = std::make_unique<Animation>(images, 0.5f, options);
  }
  button_manager_.AddButton("Home", {42, 42, 50, 25}, [this]() { engine_.ChangeState(EngineStates::MENU); });
}

void NetworkState::Update() {
  blue_train_pos_x_factor -= 0.00008f;
  if ((int)(blue_train_pos_x_factor * (float)GetScreenWidth()) <= -480)
    blue_train_pos_x_factor = 1.0f;

  blue_train_->Update();
  blue_train_rails_->Update();
  blue_train_woods_->Update();
  button_manager_.UpdateButtons();
  editable_grid_.Update();
}

std::string VecToString(Vector2 vec) {
  return ("[" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "]");
}

void NetworkState::Draw() {
  background_animation_->Draw(0, 0);
  blue_train_rails_->Draw(0, blue_train_animation_y_);
  blue_train_rails_->Draw(GetScreenWidth() / 2, blue_train_animation_y_);
  blue_train_->Draw((int)(blue_train_pos_x_factor * (float)GetScreenWidth()), blue_train_animation_y_);
  blue_train_woods_->Draw(142, blue_train_animation_y_);
  blue_train_woods_->Draw(GetScreenWidth() / 2 + 142, blue_train_animation_y_ - 5);
  button_manager_.DrawButtons();
  editable_grid_.Draw();
  DrawText((std::string("Mouse Pos: ") + VecToString(GetMousePosition())).c_str(),
    50, GetScreenHeight() - 52, 10, BLACK);
  DrawText((std::string("Grid Pos: ") + VecToString(editable_grid_.GetRelativeCoordinates(GetMousePosition()))).c_str(),
    250, GetScreenHeight() - 52, 10, BLACK);
  DrawText((std::string("Offset: ") + VecToString(editable_grid_.GetOffset())).c_str(),
    450, GetScreenHeight() - 52, 10, BLACK);
}
