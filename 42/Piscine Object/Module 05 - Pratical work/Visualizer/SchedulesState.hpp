#ifndef SCHEDULES_STATE_H_
#define SCHEDULES_STATE_H_

#include "IState.hpp"
#include "ButtonManager.hpp"
#include "Animation.hpp"
#include "VerticalScrollPanel.hpp"
#include "raylib.h"
#include <memory>

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

#endif // SCHEDULES_STATE_H_