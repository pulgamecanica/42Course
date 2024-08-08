#ifndef EDITABLE_GRID_H
#define EDITABLE_GRID_H

#include "Visualizer/Grid.hpp"
#include "Visualizer/MinimapGrid.hpp"
#include "Visualizer/ButtonManager.hpp"
#include "Simulation/Node.hpp"
#include "raylib.h"
#include <string>

enum class Tool {
  MOVE,
  REMOVE,
  ADDNODE,
  ADDRAIL,
};

class EditableGrid : public Grid {
public:
  EditableGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea);

  void Update() override;
  void Draw() override;
  void SetTool(Tool tool);
private:
  Tool current_tool_;
  std::string selected_node_;
  Vector2 drag_start_pos_;
  bool is_dragging_;
  bool show_minimap_;
  bool adding_node_;
  MinimapGrid minimap_;
  ButtonManager button_manager_;
  std::string new_node_name_;
  Vector2 new_node_position_;

  void HandleNodeSelection(const Vector2& mousePos, const float rad);
  void AddNode(Vector2 position);
  void RemoveNode(const std::string& id);
  void AddRail(const std::string& id);
  void MoveNode();
  void DrawNewNodeDialog();
};

#endif // EDITABLE_GRID_H
