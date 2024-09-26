#ifndef EDITABLE_GRID_H_
#define EDITABLE_GRID_H_

#include "Grid.hpp"
#include "MinimapGrid.hpp"
#include "ButtonManager.hpp"
#include "Node.hpp"
#include "raylib.h"
#include <string>

enum class Tool {
  MOVE,
  MOVEMAP,
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
  void HandleNodeSelection(const Vector2& mousePos, const float rad);
  void AddNode(Vector2 position);
  void RemoveNode(const std::string& id);
  void AddRail(const std::string& id);
  void MoveNode();
  void MoveMap();
  void DrawNewNodeDialog();
  
  Tool current_tool_;
  std::string selected_node_;
  std::string rail_source_node_selected_;
  Vector2 drag_start_pos_;
  bool is_dragging_;
  bool show_minimap_;
  bool adding_node_;
  MinimapGrid minimap_;
  ButtonManager button_manager_;
  std::string new_node_name_;
  Vector2 new_node_position_;
};

#endif // EDITABLE_GRID_H_