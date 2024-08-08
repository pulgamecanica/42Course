#include "Visualizer/SchedulesState.hpp"
#include "SimulationsEngine.hpp"

#include "raygui.h"

// Constructor initializes the state with a pointer to the Game object
SchedulesState::SchedulesState(SimulationsEngine& engine) : engine_(engine) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  enable_simulation_selection_ = false;
  selected_simulation_ = 0;
  number_of_simulations_ = 0;
  button_manager_.AddButton("Home", {25, 25, 50, 25}, [this]() { engine_.ChangeState(EngineStates::MENU); });
  // button_manager_.AddButton("Simulation For Schedule 1", {50, 125, 100, 50}, [this]() { engine_.ChangeState(EngineStates::SIMULATIONS); });
}

void SchedulesState::Update() {
  button_manager_.UpdateButtons();
}

void SchedulesState::Draw() {
  GuiGroupBox((Rectangle){50, 50, GetScreenWidth() - 100, GetScreenHeight() / 2 - 50}, "New Simulation");
  GuiGroupBox((Rectangle){50, GetScreenHeight() / 2 + 25, GetScreenWidth() - 100, GetScreenHeight() / 2 - 50}, "Simulations");
  if (GuiButton((Rectangle){200, 150, 100, 30}, ""))
    enable_simulation_selection_ = !enable_simulation_selection_;
  GuiDropdownBox((Rectangle){200, 150, 100, 30}, "Some;Other;Thing", &selected_simulation_, enable_simulation_selection_);
  float tmp = number_of_simulations_;
  GuiSliderBar((Rectangle){400, 150, 100, 30}, "-", "+", &tmp, 1.0f, 100.0f);
  number_of_simulations_ = tmp;
  GuiLabel((Rectangle){430, 180, 100, 30}, std::to_string(number_of_simulations_).c_str());
  if (GuiButton((Rectangle){600, 150, 100, 30}, "Generate Simulation(s)"))
    std::cout << "Creating Simulation" << std::endl;

  button_manager_.DrawButtons();
}
