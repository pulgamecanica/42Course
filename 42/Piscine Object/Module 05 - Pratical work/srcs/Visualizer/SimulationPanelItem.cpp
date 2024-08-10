#include "Visualizer/SimulationPanelItem.hpp"

SimulationPanelItem::SimulationPanelItem(Rectangle bounds) 
  : bounds_(bounds) {}

Rectangle SimulationPanelItem::GetRectangle() const {
  return bounds_;
}

int SimulationPanelItem::GetX() const {
  return bounds_.x;
}

int SimulationPanelItem::GetY() const {
  return bounds_.y;
}

void SimulationPanelItem::SetXY(Vector2 new_pos) {
  bounds_.x = new_pos.x;
  bounds_.y = new_pos.y;
}

int SimulationPanelItem::GetHeight() const {
  return bounds_.height;
}

int SimulationPanelItem::GetWidth() const {
  return bounds_.width;
}

void SimulationPanelItem::Draw(Vector2 scroll) const {
  DrawRectangleRec((Rectangle){
    GetX() + scroll.x,
    GetY() + scroll.y,
    GetWidth(),
    GetHeight()
  }, PINK);
}
