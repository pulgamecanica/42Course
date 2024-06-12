#ifndef SETTINGS_STATE_H
#define SETTINGS_STATE_H

#include "IGameState.hpp"

class Game;

class SettingsState : public IGameState {
public:
  explicit SettingsState(Game* game);
  void Update() override;
  void Draw() override;

private:
  Game* game_;
};

#endif // SETTINGS_STATE_H
