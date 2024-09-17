#ifndef SIMULATION_PANEL_ITEM_HPP
#define SIMULATION_PANEL_ITEM_HPP

#include "SimulationsManager.hpp"
#include "raylib.h"

class SimulationsEngine;

class SimulationPanelItem {
public:
  SimulationPanelItem(SimulationsEngine& engine, const SimulationsManager * simulations_manager, Rectangle bounds);
  SimulationPanelItem(const SimulationPanelItem & item) = default;
  Rectangle GetRectangle() const;
  float GetHeight() const;
  float GetWidth() const;
  int GetX() const;
  int GetY() const;
  void SetXY(Vector2 new_pos);
  void Draw(Vector2 scroll) const;
private:
  const SimulationsManager * simulations_manager_;
  SimulationsEngine& engine_;
  Rectangle bounds_;
};

#endif // SIMULATION_PANEL_ITEM_HPP