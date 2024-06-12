#include "VisualNode.hpp"

VisualNode::VisualNode(Node* node, Vector2 position, float size, Color color)
  : node_(node), position_(position), size_(size), color_(color), state_(NodeState::NORMAL) {}

void VisualNode::Draw(Vector2 gridOffset, float zoom) {
  Color drawColor = color_;
  if (state_ == NodeState::HOVERED) drawColor = GREEN;
  if (state_ == NodeState::SELECTED) drawColor = RED;

  Vector2 adjusted_position = GetAbsolutePosition(gridOffset, zoom);
  DrawCircleV(adjusted_position, size_, drawColor);
  DrawID(gridOffset, zoom);
}

void VisualNode::UpdatePosition(Vector2 newPosition) {
  position_ = newPosition;
}

Vector2 VisualNode::GetPosition() const {
  return position_;
}

// To get the absolute position in the window (grid_pos + offset * zoom = window_pos)
// To get the position in the grid (grid_pos = window_pos / zoom - offset)
Vector2 VisualNode::GetAbsolutePosition(Vector2 gridOffset, float zoom) const {
  return {(position_.x + gridOffset.x) * zoom, (position_.y + gridOffset.y) * zoom};
}

Node* VisualNode::GetNode() const {
  return node_;
}

bool VisualNode::IsMouseOver(Vector2 mousePosition, Vector2 gridOffset, float zoom) const {
  Vector2 adjusted_position = GetAbsolutePosition(gridOffset, zoom);
  return CheckCollisionPointCircle(mousePosition, adjusted_position, size_);
}

void VisualNode::DrawID(Vector2 gridOffset, float zoom) {
  Vector2 adjusted_position = GetAbsolutePosition(gridOffset, zoom);
  DrawText(TextFormat("%u", node_->id), adjusted_position.x - MeasureText(TextFormat("%u", node_->id), 10) / 2, adjusted_position.y - 5, 10, BLACK);
}

void VisualNode::SetState(NodeState state) {
  state_ = state;
}

NodeState VisualNode::GetState() const {
  return state_;
}

void VisualNode::SetSize(float size) {
  size_ = size;
}
