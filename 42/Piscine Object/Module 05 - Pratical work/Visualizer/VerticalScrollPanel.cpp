#include "VerticalScrollPanel.hpp"
#include "raygui.h"
#include <iostream>

VerticalScrollPanel::VerticalScrollPanel()
  : bounds_((Rectangle){0, 0, 0, 0}), scroll_({0, 0}), padding_(0) {}

VerticalScrollPanel::VerticalScrollPanel(Rectangle bounds, float padding)
  : bounds_(bounds), scroll_((Vector2){0, 0}), padding_(padding) {}

void VerticalScrollPanel::AddItem(SimulationPanelItem & item) {
  float posY = items_.empty() ? bounds_.y + 25 + padding_ : items_.back().GetY() + items_.back().GetHeight() + padding_;
  item.SetXY((Vector2){bounds_.x + 25, posY});
  items_.push_back(item);
  // items_.push_back((Rectangle){bounds_.x + 25, posY, bounds_.width - 70, height});
}

void VerticalScrollPanel::SetXY(Vector2 new_pos) {
  bounds_.x = new_pos.x;
  bounds_.y = new_pos.y;
}

void VerticalScrollPanel::Draw() {
  float content_height = items_.empty() ? 0 : items_.back().GetY() + items_.back().GetHeight() - bounds_.y;

  Rectangle content = {bounds_.x, bounds_.y, bounds_.width - 40, content_height};

  GuiScrollPanel(bounds_, "Simulations", content, &scroll_, &view_);

  // Draw each item within the visible area
  BeginScissorMode(view_.x, view_.y, view_.width, view_.height);
  for (const auto& item : items_) {
    // Rectangle itemRect = (Rectangle){item.x + scroll_.x, item.y + scroll_.y, item.width, item.height};
    item.Draw(scroll_);
    // DrawRectangleRec(itemRect, PINK);
    // DrawRectangleLinesEx(itemRect, 1, BLACK);
  }
  EndScissorMode();
}