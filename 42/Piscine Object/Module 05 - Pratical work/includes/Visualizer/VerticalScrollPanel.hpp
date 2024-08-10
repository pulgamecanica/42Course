#ifndef VERTICAL_SCROLLPANEL_HPP
#define VERTICAL_SCROLLPANEL_HPP

#include "Visualizer/SimulationPanelItem.hpp"
#include "raylib.h"
#include <vector>

class VerticalScrollPanel {
public:
  VerticalScrollPanel();
  VerticalScrollPanel(Rectangle bounds, float padding);
  void AddItem(SimulationPanelItem & item);
  void Draw();
private:
  Rectangle bounds_;
  Vector2 scroll_;
  Rectangle view_;
  std::vector<SimulationPanelItem> items_;
  int padding_;
};

#endif // VERTICAL_SCROLLPANEL_HPP
