#include "Grid.hpp"

Grid::Grid()
    : offset_({0, 0}) {}

void Grid::Update(Vector2 mousePosition, bool isDragging, Vector2 dragStartPos, float gridZoom) {
    if (isDragging) {
        Vector2 dragOffset = {
            (mousePosition.x - dragStartPos.x) * (1 / gridZoom),
            (mousePosition.y - dragStartPos.y) * (1 / gridZoom)
        };
        offset_.x += dragOffset.x;
        offset_.y += dragOffset.y;
    }
}

void Grid::Draw(Vector2 gridOffset, float gridZoom) {
    int spacing = 42.0f * gridZoom;
    Color gridColor = LIGHTGRAY;

    for (int x = 0; x < GetScreenWidth(); ++x) {
        Vector2 grid_pos = {(float)(x / gridZoom - gridOffset.x), 0};
        if ((int)grid_pos.x % spacing == 0) {
            for (; x < GetScreenWidth(); x += spacing) {
                DrawLine(x, 30, x, GetScreenHeight(), gridColor);
            }
            break;
        }
    }

    for (int y = 0; y < GetScreenHeight(); ++y) {
        Vector2 grid_pos = {0, (float)(y / gridZoom - gridOffset.y)};
        if ((int)grid_pos.y % spacing == 0) {
            for (; y < GetScreenHeight(); y += spacing) {
                DrawLine(0, y, GetScreenWidth(), y, gridColor);
            }
            break;
        }
    }
}

Vector2 Grid::GetOffset() const {
    return offset_;
}

void Grid::SetOffset(Vector2 offset) {
    offset_ = offset;
}
