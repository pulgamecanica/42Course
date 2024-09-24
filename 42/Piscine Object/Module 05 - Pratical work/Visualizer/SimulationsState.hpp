#ifndef SIMULATIONS_STATE_H_
#define SIMULATIONS_STATE_H_

#include "IState.hpp"
#include "ButtonManager.hpp"
#include "SimulationsManager.hpp"
#include "SimulationGrid.hpp"

#include "raylib.h"

class SimulationsEngine;

enum Controls {
  kForward,
  kBackward,
  kStop
};

class SimulationsState : public IState {
public:
  explicit SimulationsState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;
  
  void SetupNewSimulation();
  
  void SetSimulationsManager(const SimulationsManager* manager);
  int GetCurrentSimulation() const;
  int GetProgress() const;
private:
  void DrawBackground();
  void DrawSettings();
  void DrawInfo();

  bool IsTimeToUpdate();

  SimulationsEngine&  engine_;
  const SimulationsManager* manager_;

  ButtonManager   button_manager_;
  SimulationGrid  grid_;

  bool show_log_;
  bool simulation_running_;
  bool settings_open_;
  bool info_open_;

  float simulation_progress_;
  int current_simulation_;
  float last_update_s_;

  enum Controls control_;
  std::string simulation_options_;
  bool simulation_options_enabled_;

};

#endif // SIMULATIONS_STATE_H_
