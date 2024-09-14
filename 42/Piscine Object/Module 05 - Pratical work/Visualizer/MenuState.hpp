#ifndef MENU_STATE_H_
#define MENU_STATE_H_

#include "IState.hpp"
#include "Animation.hpp"
#include "ButtonManager.hpp"

#include <memory>

#include "raylib.h"

class SimulationsEngine;

class MenuState : public IState {
public:
  explicit MenuState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  void  InitializeLoadingScreen();
  void  InitializeBackgroundAnimation();
  void  InitializeButtons();
  void  DrawInfoPopup();

  SimulationsEngine& engine_;
  bool show_info_popup_;
  std::unique_ptr<Animation> background_animation_;
  ButtonManager button_manager_;

  std::unordered_map<std::string, bool> button_states_;
  std::unordered_map<std::string, Rectangle> button_rects_;
};

#endif // MENU_STATE_H_