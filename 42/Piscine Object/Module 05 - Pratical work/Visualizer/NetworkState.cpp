#include "NetworkState.hpp"
#include "SimulationsEngine.hpp"
#include "cpp_on_rails.inc"

#include "raygui.h"

namespace Paths {
  const std::vector<const char*> kBackgroundImages = {"assets/images/background_network.png"};
  const std::vector<const char*> kBlueTrainFrames = {
      "assets/images/blue_train_frame_1.png",
      "assets/images/blue_train_frame_2.png",
      "assets/images/blue_train_frame_3.png",
      "assets/images/blue_train_frame_4.png",
      "assets/images/blue_train_frame_5.png",
      "assets/images/blue_train_frame_6.png",
      "assets/images/blue_train_frame_7.png",
      "assets/images/blue_train_frame_8.png",
  };
  const std::vector<const char*> kBlueTrainRailsImages = {"assets/images/blue_train_rails.png"};
  const std::vector<const char*> kBlueTrainWoodsFrames = {
      "assets/images/blue_train_woods_frame_1.png",
      "assets/images/blue_train_woods_frame_2.png",
      "assets/images/blue_train_woods_frame_3.png",
      "assets/images/blue_train_woods_frame_4.png",
  };
}  // namespace Paths

namespace NetworkOptions {
  const std::map<std::string, int> kBackgroundOptions = {{"width", WIDTH}, {"height", HEIGHT}};
  const std::map<std::string, int> kBlueTrainOptions = {{"width", 120}, {"height", 24}};
  const std::map<std::string, int> kBlueTrainRailsOptions = {{"width", WIDTH / 2}, {"height", 40}};
  const std::map<std::string, int> kBlueTrainWoodsOptions = {{"width", WIDTH / 2}, {"height", 40}};
  const float kGridSize = 20.0f;
  const Rectangle kGridArea = {85, 100, WIDTH - 85 * 2, HEIGHT - 100 * 2};
}  // namespace NetworkOptions


// Constructor initializes the state with a pointer to the Game object
NetworkState::NetworkState(SimulationsEngine& engine)
  : engine_(engine),
    editable_grid_(engine.GetRailwaySystem(), NetworkOptions::kGridSize, NetworkOptions::kGridArea),
    blue_train_animation_y_((float)GetScreenHeight() - 100), 
    blue_train_pos_x_factor_(1.0f) {
  InitializeBackgroundAnimation();
  InitializeBlueTrainAnimation();
  InitializeBlueTrainRailsAnimation();
  InitializeBlueTrainWoodsAnimation();
  button_manager_.AddButton("Home", {42, 42, 50, 25}, [this]() { engine_.ChangeState(EngineStates::kMenu); });
}

void NetworkState::Update() {
  UpdateBlueTrainPosition();
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
  blue_train_->Draw((int)(blue_train_pos_x_factor_ * (float)GetScreenWidth()), blue_train_animation_y_);
  blue_train_woods_->Draw(142, blue_train_animation_y_);
  blue_train_woods_->Draw(GetScreenWidth() / 2 + 142, blue_train_animation_y_ - 5);
  button_manager_.DrawButtons();
  editable_grid_.Draw();
  DrawText(GetMousePositionText().c_str(), 50, GetScreenHeight() - 52, 10, BLACK);
  DrawText((std::string("Grid Pos: ") + VecToString(editable_grid_.GetRelativeCoordinates(GetMousePosition()))).c_str(),
    250, GetScreenHeight() - 52, 10, BLACK);
  DrawText((std::string("Offset: ") + VecToString(editable_grid_.GetOffset())).c_str(),
    450, GetScreenHeight() - 52, 10, BLACK);
}

void NetworkState::InitializeBackgroundAnimation() {
  background_animation_ = std::make_unique<Animation>(Paths::kBackgroundImages, 1.0f, NetworkOptions::kBackgroundOptions);
}

void NetworkState::InitializeBlueTrainAnimation() {
  blue_train_ = std::make_unique<Animation>(Paths::kBlueTrainFrames, 0.15f, NetworkOptions::kBlueTrainOptions);
}

void NetworkState::InitializeBlueTrainRailsAnimation() {
  blue_train_rails_ = std::make_unique<Animation>(Paths::kBlueTrainRailsImages, 1.0f, NetworkOptions::kBlueTrainRailsOptions);
}

void NetworkState::InitializeBlueTrainWoodsAnimation() {
  blue_train_woods_ = std::make_unique<Animation>(Paths::kBlueTrainWoodsFrames, 0.5f, NetworkOptions::kBlueTrainWoodsOptions);
}

void NetworkState::UpdateBlueTrainPosition() {
  blue_train_pos_x_factor_ -= 0.00008f;
  if (static_cast<int>(blue_train_pos_x_factor_ * GetScreenWidth()) <= -480) {
    blue_train_pos_x_factor_ = 1.0f;
  }
}

std::string NetworkState::GetMousePositionText() const {
  Vector2 mouse_pos = GetMousePosition();
  return "Mouse Pos: [" + std::to_string(mouse_pos.x) + "," + std::to_string(mouse_pos.y) + "]";
}