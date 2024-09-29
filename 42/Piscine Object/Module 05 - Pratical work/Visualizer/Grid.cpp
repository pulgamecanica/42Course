#include "Grid.hpp"
#include "Node.hpp"
#include "Settings.hpp"

#include <cmath>

Grid::Grid(RailwaySystem & rail_sys, float gridSize, Rectangle displayArea)
  : rail_sys_(rail_sys), grid_size_(gridSize), scale_(1.0f), offset_({0, 0}), display_area_(displayArea), show_grid_(true), is_dragging_(false), can_drag_grid_(true) {}

void Grid::Update() {
  if (!CheckCollisionPointRec(GetMousePosition(), display_area_)) return;
  // Handle dragging
  if (can_drag_grid_) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      drag_start_pos_ = GetMousePosition();
      is_dragging_ = true;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      is_dragging_ = false;
    }
    if (is_dragging_) {
      Vector2 current_mouse_pos = GetMousePosition();
      offset_.x -= (current_mouse_pos.x - drag_start_pos_.x) / scale_;
      offset_.y -= (current_mouse_pos.y - drag_start_pos_.y) / scale_;
      drag_start_pos_ = current_mouse_pos;
    }
  }

  // Handle scrolling
  float wheel = GetMouseWheelMove();
  if (wheel != 0) {
    scale_ += wheel * 0.1f * scale_;
    if (scale_ < 0.1f) scale_ = 0.1f;
    if (scale_ > 42.0f) scale_ = 42.0f;
  }
}

void Grid::ClipLine(Vector2& p0, Vector2& p1) {
    float edgeLeft = display_area_.x;
    float edgeRight = display_area_.x + display_area_.width;
    float edgeTop = display_area_.y;
    float edgeBottom = display_area_.y + display_area_.height;

    // Left edge
    if (p0.x < edgeLeft) {
        p0.y += (edgeLeft - p0.x) * (p1.y - p0.y) / (p1.x - p0.x);
        p0.x = edgeLeft;
    }
    if (p1.x < edgeLeft) {
        p1.y += (edgeLeft - p1.x) * (p0.y - p1.y) / (p0.x - p1.x);
        p1.x = edgeLeft;
    }
    
    // Right edge
    if (p0.x > edgeRight) {
        p0.y += (edgeRight - p0.x) * (p1.y - p0.y) / (p1.x - p0.x);
        p0.x = edgeRight;
    }
    if (p1.x > edgeRight) {
        p1.y += (edgeRight - p1.x) * (p0.y - p1.y) / (p0.x - p1.x);
        p1.x = edgeRight;
    }
    
    // Top edge
    if (p0.y < edgeTop) {
        p0.x += (edgeTop - p0.y) * (p1.x - p0.x) / (p1.y - p0.y);
        p0.y = edgeTop;
    }
    if (p1.y < edgeTop) {
        p1.x += (edgeTop - p1.y) * (p0.x - p1.x) / (p0.y - p1.y);
        p1.y = edgeTop;
    }
    
    // Bottom edge
    if (p0.y > edgeBottom) {
        p0.x += (edgeBottom - p0.y) * (p1.x - p0.x) / (p1.y - p0.y);
        p0.y = edgeBottom;
    }
    if (p1.y > edgeBottom) {
        p1.x += (edgeBottom - p1.y) * (p0.x - p1.x) / (p0.y - p1.y);
        p1.y = edgeBottom;
    }
}

void Grid::Draw() {
  BeginScissorMode(display_area_.x, display_area_.y, display_area_.width, display_area_.height);
  Settings::Instance().DrawMapBackground();
  EndScissorMode();
  if (show_grid_) {
    DrawGridLines();
  }
  // Draw edges
  for (const auto & rail : rail_sys_.GetRails()) {
    Node* node1 = rail_sys_.GetNode(rail->GetNode1());
    Node* node2 = rail_sys_.GetNode(rail->GetNode2());
    Vector2 grid_pos1 = GetAbsoluteCoordinates(node1->GetPosition());
    Vector2 grid_pos2 = GetAbsoluteCoordinates(node2->GetPosition());
    ClipLine(grid_pos1, grid_pos2);
    DrawLineEx(grid_pos1, grid_pos2, (Settings::Instance().GetNodeSize() / 2) * scale_, BLUE);
    // DrawLineEx(grid_pos1, grid_pos2, 4.2f * scale_, BLUE);
  }
  // Draw nodes
  std::vector<Rectangle> names_recs;
  const float rad = scale_ * (Settings::Instance().GetNodeSize() / 2);
  for (auto & [name, node] : rail_sys_.GetNodes()) {
    Vector2 grid_pos = GetAbsoluteCoordinates(node->GetPosition());
    if (CheckCollisionPointRec(grid_pos, display_area_)) {
      DrawNode(&(*node), node->GetColor());

      int limit = -rad - 10;
      int kill_limit = 4;
      float width = MeasureText(name.c_str(), rad);
      Vector2 node_name_pos = (Vector2){grid_pos.x - rad, grid_pos.y + limit};
      Rectangle name_rec = {node_name_pos.x, node_name_pos.y, width, rad};

      while (!RectangleIsIndependentFromOtherRectangles(name_rec, names_recs) && kill_limit >= 0) {
        limit *= -1;
        node_name_pos = (Vector2){grid_pos.x - rad, grid_pos.y + limit};
        name_rec = {node_name_pos.x, node_name_pos.y, width, rad};
        limit += 10 * (limit > 0 ? 1 : -1);
        kill_limit--;
      }
      if (Settings::Instance().ShowNodeNames())
        DrawText(name.c_str(), node_name_pos.x, node_name_pos.y, rad, BLACK);
      names_recs.push_back(name_rec);
    }
  }
}

void Grid::DrawNode(const Node* node, Color color) {
  const float rad = scale_ * Settings::Instance().GetNodeSize();
  Vector2 grid_pos = GetAbsoluteCoordinates(node->GetPosition());
  if (!CheckCollisionPointRec(grid_pos, display_area_)) return;
  DrawPoly(grid_pos, 6, rad, 0.0f, color);
}

bool Grid::RectangleIsIndependentFromOtherRectangles(const Rectangle & rec, const std::vector<Rectangle>& recs) const {
  for (const auto & r : recs)
    if (CheckCollisionRecs(r, rec))
      return false;
  return true;
}

/**
 * Get The Real Coordinates in space of a point in the grid
 * For example giving the grid coordinates of the point [10, 10]
 * would return the real position of [10, 10] in the screen
 * 
 * pos: should be a relative coordinate to the grid
**/
Vector2 Grid::GetAbsoluteCoordinates(Vector2 pos) const {
  //10 10 must correspond to: display_area.x+10, display_area.y+10 
  return (Vector2){((pos.x + display_area_.x) - offset_.x) * scale_, ((pos.y + display_area_.y) - offset_.y) * scale_};
}

/**
 * Get The Relative Coordinates in the grid
 * 
 * pos: should be an absolute coordinate of the screen
 *      which will return the coorespondent coodinate on the grid
**/
Vector2 Grid::GetRelativeCoordinates(Vector2 abs_pos) const {
  return (Vector2){((abs_pos.x / scale_) + offset_.x - display_area_.x), ((abs_pos.y / scale_) + offset_.y - display_area_.y)};
}

void Grid::SetGridSize(float gridSize) {
  grid_size_ = gridSize;
}

void Grid::SetDisplayArea(Rectangle displayArea) {
  display_area_ = displayArea;
}

void Grid::SetCanDragGrid(bool can_drag) {
  can_drag_grid_ = can_drag;
}

Vector2 Grid::GetOffset() const {
  return offset_;
}

void Grid::DrawGridLines() {
  float left = display_area_.x;
  float top = display_area_.y;
  float right = left + display_area_.width;;
  float bottom = top + display_area_.height;

  float startX = left - fmod(offset_.x * scale_, grid_size_ * scale_);
  float startY = top - fmod(offset_.y * scale_, grid_size_ * scale_);

  for (float x = startX; x < right; x += grid_size_ * scale_) {
    if (x < left || x > right)
      continue ;
    DrawLineEx({x, top}, {x, bottom}, 1, LIGHTGRAY);
  }
  for (float y = startY; y < bottom; y += grid_size_ * scale_) {
    if (y < top || y > bottom)
      continue ;
    DrawLineEx({left, y}, {right, y}, 1, LIGHTGRAY);
  }
}