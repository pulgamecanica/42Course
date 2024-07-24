#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/Animation.hpp"
#include "Visualizer/ButtonManager.hpp"

#include <memory>

#include "raylib.h"

class SimulationsEngine;

class MenuState : public IState {
public:
  explicit MenuState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  SimulationsEngine& engine_;
  bool show_info_popup_;
  std::unique_ptr<Animation> background_animation_;
  ButtonManager button_manager_;
  // std::unordered_map<std::string, bool> button_states_;
  // std::unordered_map<std::string, Rectangle> button_rects_;
};

#endif // MENU_STATE_H
