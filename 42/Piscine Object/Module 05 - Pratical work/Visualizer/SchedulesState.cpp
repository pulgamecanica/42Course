#include "SchedulesState.hpp"
#include "SimulationPanelItem.hpp"
#include "SimulationsManager.hpp"
#include "SimulationsEngine.hpp"
#include "cpp_on_rails.inc"


#include "raygui.h"

namespace SchedulesStateConstants {
  constexpr float kScreenPaddingFactor = 0.04f;
  constexpr float kVerticalPaddingFactor = 0.1f;
  constexpr int kHomeButtonX = 25;
  constexpr int kHomeButtonY = 25;
  constexpr int kHomeButtonWidth = 50;
  constexpr int kHomeButtonHeight = 25;
  constexpr float kSliderMin = 1.0f;
  constexpr float kSliderMax = 10.0f;
  constexpr float kFormPaddingDivisor = 4.0f;
  const std::vector<const char*> kBackgroundImages = { "assets/images/background_schedules.png" };
  const std::map<std::string, int> kBackgroundOptions = {
    {"width", WIDTH},
    {"height", HEIGHT}
  };

  const Rectangle kSimulationForm = {(WIDTH / 2) + (HEIGHT * kVerticalPaddingFactor), 
                                     WIDTH * kScreenPaddingFactor, 
                                     (WIDTH / 2) - ((HEIGHT * kVerticalPaddingFactor) * 2), 
                                     (HEIGHT / 2) - ((WIDTH * kScreenPaddingFactor) * 2)};

  const Rectangle kSimulationsView = {HEIGHT * kVerticalPaddingFactor, 
                                      HEIGHT / 2, 
                                      WIDTH - (HEIGHT * kVerticalPaddingFactor * 2), 
                                      (HEIGHT / 2) - (WIDTH * kScreenPaddingFactor)};

  const float kFormPadding = kSimulationForm.height / SchedulesStateConstants::kFormPaddingDivisor;

  const Rectangle kSimulationFormSelection = {kSimulationForm.x + (HEIGHT * kVerticalPaddingFactor), 
                                              kSimulationForm.y + kFormPadding / 2, 
                                              kSimulationForm.width - (HEIGHT * kVerticalPaddingFactor * 2), 
                                              kFormPadding - 10};

  const Rectangle kSimulationFormSlider = {kSimulationFormSelection.x, 
                                           kSimulationFormSelection.y + kFormPadding, 
                                           kSimulationFormSelection.width, 
                                           kSimulationFormSelection.height};

  const Rectangle kSimulationFormSubmit = {kSimulationFormSelection.x, 
                                           kSimulationFormSelection.y + (kFormPadding * 2) + 10, 
                                           kSimulationFormSelection.width, 
                                           kSimulationFormSelection.height};
}

SchedulesState::SchedulesState(SimulationsEngine& engine)
  : engine_(engine),
    selected_simulation_(0),
    enable_simulation_selection_(false),
    form_simulation_submit_(false),
    number_of_simulations_(0),
    schedule_options_("") {

  // Initialize Background Animation
  background_animation_ = std::make_unique<Animation>(SchedulesStateConstants::kBackgroundImages, 1.0f, SchedulesStateConstants::kBackgroundOptions);

  // Initialize schedule options and list
  int i = 0;
  for (const auto& [name, schedule] : engine_.GetRailwaySystem().GetSchedules()) {
    if (i > 0) schedule_options_ += ";";
    schedule_options_ += name;
    schedule_options_list_.push_back(name);
    i++;
  }

  // Initialize buttons
  button_manager_.AddButton("Home", 
    {SchedulesStateConstants::kHomeButtonX, 
     SchedulesStateConstants::kHomeButtonY, 
     SchedulesStateConstants::kHomeButtonWidth, 
     SchedulesStateConstants::kHomeButtonHeight}, 
    [this]() { engine_.ChangeState(EngineStates::kMenu); });

  button_manager_.AddButton("", SchedulesStateConstants::kSimulationFormSelection, 
    [this]() { enable_simulation_selection_ = !enable_simulation_selection_; });
    
  button_manager_.AddButton("Simulate", SchedulesStateConstants::kSimulationFormSubmit, 
    [this]() { form_simulation_submit_ = true; });

  pannel_ = VerticalScrollPanel(SchedulesStateConstants::kSimulationsView, 5);
}

void SchedulesState::Update() {
  button_manager_.UpdateButtons();
  if (form_simulation_submit_) {
    SubmitForm();
    form_simulation_submit_ = false;
  }
}

void SchedulesState::SubmitForm() {
  if (enable_simulation_selection_) return;  // Cannot submit if dropdown selection is open
  
  try {
    Schedule* schedule = engine_.GetRailwaySystem().GetSchedule(schedule_options_list_[selected_simulation_]);
    SimulationsManager* simulation = engine_.GenerateSimulations(*schedule, number_of_simulations_);
    SimulationPanelItem spi(engine_, simulation, {0, 0, SchedulesStateConstants::kSimulationsView.width - 100, 0});
    pannel_.AddItem(spi);
  } catch (std::exception &e) {
    std::cerr << "Simulation aborted!" << std::endl;
    std::cerr << e.what() << std::endl;
  }
}

void SchedulesState::Draw() {
  background_animation_->Draw(0, 0);

  button_manager_.DrawButtons();

  GuiGroupBox(SchedulesStateConstants::kSimulationForm, "New Simulation");

  float tmp = number_of_simulations_;
  GuiSliderBar(SchedulesStateConstants::kSimulationFormSlider, "-", "+", &tmp, SchedulesStateConstants::kSliderMin, SchedulesStateConstants::kSliderMax);
  number_of_simulations_ = tmp;
  GuiLabel({SchedulesStateConstants::kSimulationFormSlider.x + SchedulesStateConstants::kSimulationFormSlider.width / 2 - 10, 
            SchedulesStateConstants::kSimulationFormSlider.y + SchedulesStateConstants::kSimulationFormSlider.height, 
            20, 15}, 
            std::to_string(number_of_simulations_).c_str());

  GuiDropdownBox(SchedulesStateConstants::kSimulationFormSelection, schedule_options_.c_str(), &selected_simulation_, enable_simulation_selection_);

  pannel_.Draw();
}
