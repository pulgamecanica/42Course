#ifndef GRID_H
#define GRID_H

#include "Simulation/RailwaySystem.hpp"
#include "raylib.h"
#include <utility>
#include <cmath>

class Grid {
public:
    Grid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea);
    virtual void Update();
    virtual void Draw();
    void ToggleShowGrid();
    void SetGridSize(float gridSize);
    void SetDisplayArea(Rectangle displayArea);
    void SetCanDragGrid(bool can_drag);
    Vector2 GetAbsoluteCoordinates(Vector2 pos) const;
    Vector2 GetRelativeCoordinates(Vector2 pos) const;
    Vector2 GetOffset() const;
protected:
    RailwaySystem &rail_sys_;
    float grid_size_;
    float scale_;
    Vector2 offset_;
    Rectangle display_area_;
    bool show_grid_;
    Vector2 drag_start_pos_;
    bool is_dragging_;
    bool can_drag_grid_;
    void ClipLine(Vector2& p0, Vector2& p1);
    void DrawGridLines();
};

#endif // GRID_H
