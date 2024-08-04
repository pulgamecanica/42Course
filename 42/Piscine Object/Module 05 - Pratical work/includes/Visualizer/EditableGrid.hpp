#ifndef EDITABLE_GRID_H
#define EDITABLE_GRID_H

#include "Visualizer/Grid.hpp"
#include "Visualizer/MinimapGrid.hpp"
#include "Simulation/Node.hpp"
#include "raylib.h"
#include <string>

enum class Tool {
  MOVE,
  REMOVE,
  ADD,
  EDIT,
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
  MinimapGrid minimap_;

  void AddNode(Vector2 position);
  void RemoveNode(const std::string& id);
  void EditNode(const std::string& id, const Node& newNode);
  void MoveNode();
};

#endif // EDITABLE_GRID_H
