#ifndef SCHEDULES_STATE_H
#define SCHEDULES_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/ButtonManager.hpp"

#include "raylib.h"

class SimulationsEngine;

class SchedulesState : public IState {
public:
  explicit SchedulesState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  SimulationsEngine& engine_;
  ButtonManager button_manager_;
};

#endif // SCHEDULES_STATE_H