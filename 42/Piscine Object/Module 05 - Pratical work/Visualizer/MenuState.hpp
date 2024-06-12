#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "IGameState.hpp"
#include "Animation.hpp"

#include <memory>
#include <unordered_map>
#include <string>

class Game;

class MenuState : public IGameState {
public:
  explicit MenuState(Game* game);
  void Update() override;
  void Draw() override;

private:
  Game* game_;
  bool show_info_popup_;

  // Buttons
  std::unique_ptr<Animation> background_animation_;
  std::unordered_map<std::string, bool> button_states_;
  std::unordered_map<std::string, Rectangle> button_rects_;
};

#endif // MENU_STATE_H
