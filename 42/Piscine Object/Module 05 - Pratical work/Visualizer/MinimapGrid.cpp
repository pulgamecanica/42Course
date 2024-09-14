#include "Visualizer/MinimapGrid.hpp"
#include <raygui.h>

MinimapGrid::MinimapGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea) {
  show_grid_ = false;
  SetCanDragGrid(false);
  scale_ = 0.5f;
  std::vector<const char*> backgrounds = { "assets/images/background_map.png" };
  std::map<std::string, int> options = {{"width", displayArea.width}, {"height", displayArea.height}};
  background_animation_ = std::make_unique<Animation>(backgrounds, 1.0f, options);
}

void MinimapGrid::Update() {
  int x_min = std::numeric_limits<int>::max();
  int y_min = std::numeric_limits<int>::max();
  std::string node_a, node_b;
  for (const auto& [name, node] : rail_sys_.GetNodes()) {
    const Vector2 & node_pos = node->GetPosition();
    x_min = std::min<int>(x_min, node_pos.x);
    y_min = std::min<int>(y_min, node_pos.y);
  }
  offset_ = (Vector2){(x_min - display_area_.x - 50) / scale_, (y_min - display_area_.y - 50) / scale_};
}

void MinimapGrid::Draw() {
  background_animation_->Draw(display_area_.x, display_area_.y);
  DrawRectangleRounded(display_area_, 0.05f, 9, (Color){ 42, 42, 42, 100 });
  Grid::Draw();
  GuiSliderBar((Rectangle){(display_area_.x + display_area_.width / 2) - 20, display_area_.y + display_area_.height - 10, 40, 10}, "-", "+", &scale_, 0.2f, 0.8f);
}