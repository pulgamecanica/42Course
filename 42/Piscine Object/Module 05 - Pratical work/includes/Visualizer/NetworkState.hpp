#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/ButtonManager.hpp"
#include "Visualizer/EditableGrid.hpp"
#include "Visualizer/Grid.hpp"

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
  // Grid grid_;
};

#endif // NETWORK_STATE_H
