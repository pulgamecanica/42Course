#include "Visualizer/Grid.hpp"
#include "Simulation/Node.hpp"

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
    float prev_scale = scale_;
    scale_ += wheel * 0.1f;
    if (scale_ < 0.1f) scale_ = 0.1f;
    if (scale_ > 10.0f) scale_ = 10.0f;

    // Vector2 mouse_pos = GetMousePosition();
    // offset_.x = mouse_pos.x / scale_ - mouse_pos.x / prev_scale + offset_.x;
    // offset_.y = mouse_pos.y / scale_ - mouse_pos.y / prev_scale + offset_.y;
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
  if (show_grid_) {
    DrawGridLines();
  }
  // Draw edges
  const std::vector<Rail>& rails = rail_sys_.GetRails();
  for (std::vector<Rail>::const_iterator i = rails.begin(); i != rails.end(); ++i) {
    const Node & node1 = rail_sys_.GetNode(i->node1);
    const Node & node2 = rail_sys_.GetNode(i->node2);
    Vector2 grid_pos1 = GetAbsoluteCoordinates(node1.GetPosition());
    Vector2 grid_pos2 = GetAbsoluteCoordinates(node2.GetPosition());
    ClipLine(grid_pos1, grid_pos2);
    DrawLineEx(grid_pos1, grid_pos2, 2.0f, BLUE);
  }
  // Draw nodes
  const std::unordered_map<std::string, Node>& nodes_ = rail_sys_.GetNodes();
  const float rad = scale_ * 5;
  for (std::unordered_map<std::string, Node>::const_iterator i = nodes_.begin(); i != nodes_.end(); ++i) {
    const Node & node = i->second;
    Vector2 grid_pos = GetAbsoluteCoordinates(node.GetPosition());
    if (CheckCollisionPointRec(grid_pos, display_area_)) {
      DrawPoly(grid_pos, 6, rad, 0.0f, node.GetColor());
      Vector2 node_name_pos = (Vector2){grid_pos.x - rad, grid_pos.y - rad - 10};
      DrawText(node.GetName().c_str(), node_name_pos.x, node_name_pos.y, rad, BLACK);
    }
  }
  // Draw events
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

void Grid::ToggleShowGrid() {
  show_grid_ = !show_grid_;
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
