#ifndef SIMULATION_STATE_H
#define SIMULATION_STATE_H

#include "IGameState.hpp"

class Game;

class SimulationState : public IGameState {
public:
  explicit SimulationState(Game* game);
  void Update() override;
  void Draw() override;

private:
  Game* game_;
};

#endif // SIMULATION_STATE_H
