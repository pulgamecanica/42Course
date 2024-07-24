#include "Visualizer/EditableGrid.hpp"

EditableGrid::EditableGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea), current_tool_(Tool::MOVE), is_dragging_(false) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  }

void EditableGrid::Update() {
  Vector2 mousePos = GetMousePosition();
  const std::unordered_map<std::string, Node>& nodes = rail_sys_.GetNodes();
  const float rad = scale_ * 5;

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    switch (current_tool_) {
      case Tool::MOVE:
        for (const auto& [id, node] : nodes) {
          if (CheckCollisionPointCircle(mousePos, GetAbsoluteCoordinates(node.GetPosition()), rad)) {
            selected_node_ = id;
            drag_start_pos_ = mousePos;
            is_dragging_ = true;
            break;
          }
        }
        break;
      case Tool::REMOVE:
        for (const auto& [id, node] : nodes) {
          if (CheckCollisionPointCircle(mousePos, GetAbsoluteCoordinates(node.GetPosition()), rad)) {
            RemoveNode(id);
            break;
          }
        }
        break;
      case Tool::ADD:
        AddNode(mousePos);
        break;
      default:
        break;
    }
  }

  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      is_dragging_ = false;
  }

  if (is_dragging_ && current_tool_ == Tool::MOVE) {
      MoveNode();
  }
  if (is_dragging_)
    can_drag_grid_ = false;
  Grid::Update();
  can_drag_grid_ = true;
}

void EditableGrid::Draw() {
  Grid::Draw();

}

void EditableGrid::SetTool(Tool tool) {
  current_tool_ = tool;
}

void EditableGrid::AddNode(Vector2 position) {
  (void)position;
  // Vector2 gridPos = {(position.x - offset_.x) / scale_, (position.y - offset_.y) / scale_};
  // rail_sys_.AddNode("node_" + std::to_string(rail_sys_.GetNodes().size()), gridPos);
}

void EditableGrid::RemoveNode(const std::string& id) {
  (void)id;
  // rail_sys_.RemoveNode(id);
}

void EditableGrid::EditNode(const std::string& id, const Node& newNode) {
  (void)id;
  (void)newNode;
  // if (rail_sys_.GetNodes().find(id) != rail_sys_.GetNodes().end()) {
  //     rail_sys_.EditNode(id, newNode);
  // }
}

void EditableGrid::MoveNode() {
  if (!selected_node_.empty()) {
    Vector2 current_mouse_pos = GetMousePosition();
    Node & node = rail_sys_.GetNode(selected_node_);
    node.SetPosition(GetRelativeCoordinates(current_mouse_pos));
  }
}
