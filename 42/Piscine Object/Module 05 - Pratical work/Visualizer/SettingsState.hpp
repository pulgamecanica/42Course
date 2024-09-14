#ifndef SETTINGS_STATE_H_
#define SETTINGS_STATE_H_

#include "IState.hpp"
// #include "Visualizer/ButtonManager.hpp"

#include "raylib.h"

class SimulationsEngine;

class SettingsState : public IState {
public:
  explicit SettingsState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  SimulationsEngine& engine_;
  // ButtonManager button_manager_;
};

#endif // SETTINGS_STATE_H_