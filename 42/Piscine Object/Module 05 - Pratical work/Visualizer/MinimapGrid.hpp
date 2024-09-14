#ifndef MINIMAP_GRID_H_
#define MINIMAP_GRID_H_

#include "Grid.hpp"
#include "Animation.hpp"
#include "raylib.h"
#include <string>
#include <memory>

class MinimapGrid : public Grid {
public:
  MinimapGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea);

  void Update() override;
  void Draw() override;
private:
  std::unique_ptr<Animation> background_animation_;
};

#endif // MINIMAP_GRID_H_