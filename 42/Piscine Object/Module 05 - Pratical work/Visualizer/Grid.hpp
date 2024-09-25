#ifndef GRID_H_
#define GRID_H_

#include "RailwaySystem.hpp"
#include "raylib.h"
#include <utility>

class Grid {
public:
  Grid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea);
  virtual void Update();
  virtual void Draw();
  void SetGridSize(float gridSize);
  void SetDisplayArea(Rectangle displayArea);
  void SetCanDragGrid(bool can_drag);
  Vector2 GetAbsoluteCoordinates(Vector2 pos) const;
  Vector2 GetRelativeCoordinates(Vector2 pos) const;
  Vector2 GetOffset() const;

protected:
  void ClipLine(Vector2& p0, Vector2& p1);
  void DrawGridLines();
  bool RectangleIsIndependentFromOtherRectangles(const Rectangle & rec, const std::vector<Rectangle>& recs) const;
  void DrawNode(const Node* node, Color color);

  RailwaySystem &rail_sys_;
  float grid_size_;
  float scale_;
  Vector2 offset_;
  Rectangle display_area_;
  bool show_grid_;
  // bool show_distance_;
  Vector2 drag_start_pos_;
  bool is_dragging_;
  bool can_drag_grid_;
};

#endif // GRID_H_