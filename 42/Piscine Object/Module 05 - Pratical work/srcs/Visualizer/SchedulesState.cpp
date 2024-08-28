#include "Visualizer/SchedulesState.hpp"
#include "Visualizer/SimulationPanelItem.hpp"
#include "Simulation/SimulationsManager.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
SchedulesState::SchedulesState(SimulationsEngine& engine)
  : engine_(engine),
    selected_simulation_(0),
    enable_simulation_selection_(false),
    form_simulation_submit_(false),
    number_of_simulations_(0),
    schedule_options_("") {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  int i = 0;
  for (auto const& [name, schedule] : engine_.GetRailwaySystem().GetSchedules()) {
    if (i > 0)
      schedule_options_.append(";");
    schedule_options_.append(name);
    schedule_options_list_.push_back(name);
    i++;
  }
  {
    std::vector<const char*> images = { "assets/images/background_schedules.png" };
    std::map<std::string, int> options = {{"width", screenWidth}, {"height", screenHeight} };
    background_animation_ = std::make_unique<Animation>(images, 1.0f, options);
  }

  int height_padding = screenWidth * 0.04;
  int width_padding = screenHeight * 0.1;
  simulation_form_ = (Rectangle){(screenWidth / 2) + width_padding, height_padding, (screenWidth / 2) - (width_padding * 2), (screenHeight / 2) - (height_padding * 2)};
  simulations_view_ = (Rectangle){width_padding, (screenHeight / 2), screenWidth - (width_padding * 2), (screenHeight / 2) - height_padding};
  int form_padding = simulation_form_.height / 4;
  simulation_form_selection_ = (Rectangle){simulation_form_.x + (width_padding), simulation_form_.y + form_padding / 2, simulation_form_.width - (width_padding * 2), form_padding - 10};
  simulation_form_slider_ = (Rectangle){simulation_form_selection_.x, simulation_form_selection_.y + form_padding, simulation_form_selection_.width, simulation_form_selection_.height};
  simulation_form_submit_ = (Rectangle){simulation_form_selection_.x, simulation_form_selection_.y + (form_padding * 2) + 10, simulation_form_selection_.width, simulation_form_selection_.height};

  button_manager_.AddButton("Home", {25, 25, 50, 25}, [this]() { engine_.ChangeState(EngineStates::MENU); });
  button_manager_.AddButton("", simulation_form_selection_, [this]() { enable_simulation_selection_ = !enable_simulation_selection_; });
  button_manager_.AddButton("Simulate", simulation_form_submit_, [this]() {
    form_simulation_submit_ = true;
  });
  pannel_ = VerticalScrollPanel(simulations_view_, 5);
}

void SchedulesState::Update() {
  button_manager_.UpdateButtons();
  if (form_simulation_submit_) {
    SubmitForm();
    form_simulation_submit_ = false;
  }
}

void SchedulesState::SubmitForm() {
  SimulationsManager * simulation;

  if (enable_simulation_selection_) return ; // Cannot submit if dropdown selection is open
  try {
    const Schedule & schedule = engine_.GetRailwaySystem().GetSchedule(schedule_options_list_[selected_simulation_]);
    simulation = engine_.GenerateSimulations(schedule, number_of_simulations_);
  } catch (std::exception &e) {
    std::cerr << "Simulation aborted!" << std::endl;
    std::cerr << e.what() << std::endl;
    return ;
  }
  SimulationPanelItem spi = SimulationPanelItem(simulation, (Rectangle){0, 0, simulations_view_.width - 100, 0});
  pannel_.AddItem(spi);
}

void SchedulesState::Draw() {
  background_animation_->Draw(0, 0);

  button_manager_.DrawButtons();

  GuiGroupBox(simulation_form_, "New Simulation");

  float tmp = number_of_simulations_;
  GuiSliderBar(simulation_form_slider_, "-", "+", &tmp, 1.0f, 100.0f);
  number_of_simulations_ = tmp;
  GuiLabel((Rectangle){simulation_form_slider_.x + simulation_form_slider_.width / 2 - 10, simulation_form_slider_.y + simulation_form_slider_.height, 20, 15}, std::to_string(number_of_simulations_).c_str());

  GuiDropdownBox(simulation_form_selection_, schedule_options_.c_str(), &selected_simulation_, enable_simulation_selection_);

  pannel_.Draw();
}
