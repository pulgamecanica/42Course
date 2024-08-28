#ifndef SIMULATION_PANEL_ITEM_HPP
#define SIMULATION_PANEL_ITEM_HPP

#include "Simulation/SimulationsManager.hpp"
#include "raylib.h"

class SimulationPanelItem {
public:
  SimulationPanelItem(const SimulationsManager * simulations_manager, Rectangle bounds);
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
  const SimulationsManager * simulations_manager_;
};

#endif // SIMULATION_PANEL_ITEM_HPP
