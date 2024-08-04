#include "Visualizer/EditableGrid.hpp"
#include <raygui.h>

EditableGrid::EditableGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea),
    current_tool_(Tool::MOVE),
    is_dragging_(false),
    show_minimap_(false),
    minimap_(rail_sys, gridSize / 2, displayArea) {
    SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
  }

void EditableGrid::Update() {
  if (show_minimap_) { // Minimap logic 
    minimap_.Update();
  } else { // Grid logic
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
}

void EditableGrid::Draw() {
  if (show_minimap_) {
    minimap_.Draw();
    if (GuiButton((Rectangle){display_area_.x + display_area_.width - 50, display_area_.y, 50, 25}, GuiIconText(ICON_EYE_OFF, "Map")))
      show_minimap_ = false;
  } else {
    Grid::Draw();
    int buttons_padding = 5;
    GuiButton((Rectangle){display_area_.x + ((100 + buttons_padding) * 0), display_area_.y + display_area_.height - 25, 100, 25}, GuiIconText(ICON_GEAR, "Move"));
    GuiButton((Rectangle){display_area_.x + ((100 + buttons_padding) * 1), display_area_.y + display_area_.height - 25, 100, 25}, GuiIconText(ICON_CROSS, "Remove"));
    GuiButton((Rectangle){display_area_.x + ((100 + buttons_padding) * 2), display_area_.y + display_area_.height - 25, 100, 25}, GuiIconText(ICON_WAVE, "Add"));
    GuiButton((Rectangle){display_area_.x + ((100 + buttons_padding) * 3), display_area_.y + display_area_.height - 25, 100, 25}, GuiIconText(ICON_PENCIL, "Edit"));
    if (GuiButton((Rectangle){display_area_.x + display_area_.width - 50, display_area_.y, 50, 25}, GuiIconText(ICON_EYE_ON, "Map")))
      show_minimap_ = true;
  }
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
