#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/ButtonManager.hpp"
#include "Visualizer/EditableGrid.hpp"
#include "Visualizer/Grid.hpp"
#include "Visualizer/Animation.hpp"

#include "raylib.h"

class SimulationsEngine;

class NetworkState : public IState {
public:
  explicit NetworkState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  SimulationsEngine& engine_;
  ButtonManager button_manager_;
  EditableGrid editable_grid_;
  std::unique_ptr<Animation> background_animation_;
  std::unique_ptr<Animation> blue_train_;
  std::unique_ptr<Animation> blue_train_rails_;
  std::unique_ptr<Animation> blue_train_woods_;
  int blue_train_animation_y_;
  float blue_train_pos_x_factor;
  // Grid grid_;
};

#endif // NETWORK_STATE_H
