#ifndef SIMULATIONS_STATE_H
#define SIMULATIONS_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/ButtonManager.hpp"

#include "raylib.h"

class SimulationsEngine;

class SimulationsState : public IState {
public:
  explicit SimulationsState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  SimulationsEngine& engine_;
  ButtonManager button_manager_;
};

#endif // SIMULATIONS_STATE_H
