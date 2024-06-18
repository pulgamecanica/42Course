#ifndef GRID_HPP
#define GRID_HPP

#include "raylib.h"

class Grid {
public:
    Grid();
    void Update(Vector2 mousePosition, bool isDragging, Vector2 dragStartPos, float gridZoom);
    void Draw(Vector2 gridOffset, float gridZoom);
    Vector2 GetOffset() const;
    void SetOffset(Vector2 offset);

private:
    Vector2 offset_;
};

#endif // GRID_HPP
