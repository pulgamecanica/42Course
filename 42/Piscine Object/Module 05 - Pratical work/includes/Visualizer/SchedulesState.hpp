#ifndef SCHEDULES_STATE_H
#define SCHEDULES_STATE_H

#include "Visualizer/IState.hpp"
#include "Visualizer/ButtonManager.hpp"
#include "Visualizer/Animation.hpp"
#include "Visualizer/VerticalScrollPanel.hpp"
#include <memory>
#include "raylib.h"

class SimulationsEngine;

class SchedulesState : public IState {
public:
  explicit SchedulesState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  void SubmitForm();
  
  SimulationsEngine& engine_;
  ButtonManager button_manager_;
  std::unique_ptr<Animation> background_animation_;
  int selected_simulation_;
  bool enable_simulation_selection_;
  bool form_simulation_submit_;
  int number_of_simulations_;
  Rectangle simulation_form_;
  Rectangle simulation_form_selection_;
  Rectangle simulations_view_;
  Rectangle simulation_form_slider_;
  Rectangle simulation_form_submit_;
  std::string schedule_options_;
  std::vector<std::string> schedule_options_list_;
  VerticalScrollPanel pannel_;
};

#endif // SCHEDULES_STATE_H
