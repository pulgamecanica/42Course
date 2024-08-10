#ifndef SIMULATION_PANEL_ITEM_HPP
#define SIMULATION_PANEL_ITEM_HPP

#include "raylib.h"

class SimulationPanelItem {
public:
  SimulationPanelItem(Rectangle bounds);
  SimulationPanelItem(const SimulationPanelItem & item) = default;
  Rectangle GetRectangle() const;
  int GetHeight() const;
  int GetWidth() const;
  int GetX() const;
  int GetY() const;
  void SetXY(Vector2 new_pos);
  void Draw(Vector2 scroll) const;
private:
  Rectangle bounds_;
};

#endif // SIMULATION_PANEL_ITEM_HPP
