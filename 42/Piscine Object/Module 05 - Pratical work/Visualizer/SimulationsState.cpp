#include "SimulationsState.hpp"
#include "SimulationsEngine.hpp"
#include "Parser.hpp"

#include "cpp_on_rails.inc"

#include "raygui.h"

namespace SimulationsOptions {
  const float kGridSize = 20.0f;
  const int   kTextSize = 10;
  const Rectangle kHomeButtonArea = {5, 2, 50, 20};
  const Rectangle kSchedulesButtonArea = {60, 2, 70, 20};
  const Rectangle kSettingsButtonArea = {WIDTH - 40, 2, 30, 20};
  const Rectangle kInfoButtonArea = {WIDTH - 80, 2, 30, 20};
  const Rectangle kGridArea = {0, 25, WIDTH, HEIGHT - 90};
  const Rectangle kProgressBarArea = {50, HEIGHT - 55, WIDTH - 100, 10};
  const Rectangle kTopNavArea = {0, 0, WIDTH, 25};
  const Rectangle kSettingsArea = {WIDTH / 2 - 200, 100, WIDTH / 2, 400};
  const Rectangle kInfoArea = {WIDTH / 2 - 200, 100, WIDTH / 2, 400};
  const Rectangle kBottomNavArea = {0, HEIGHT - 60, WIDTH, 60};
  const Vector2   kTimeProgressVec = {10, HEIGHT - 30};
  const Rectangle kBackwardsArea = {(WIDTH / 2) - 60, HEIGHT - 30, 42, 20};
  const Rectangle kPauseResumeArea = {(WIDTH / 2) - 10, HEIGHT - 30, 42, 20};
  const Rectangle kForwardsArea = {(WIDTH / 2) + 40, HEIGHT - 30, 42, 20};
  const Rectangle kStopArea = {WIDTH - 90, HEIGHT - 40, 42, 30};
};

// Constructor initializes the state with a pointer to the Game object
SimulationsState::SimulationsState(SimulationsEngine& engine)
  : engine_(engine),
    manager_(nullptr),
    grid_(engine.GetRailwaySystem(), *this, SimulationsOptions::kGridSize, SimulationsOptions::kGridArea),
    show_log_(false), simulation_running_(false), settings_open_(false), info_open_(false),
    simulation_progress_(0), current_simulation_(0), last_update_s_(0) {
  button_manager_.AddButton("Home", SimulationsOptions::kHomeButtonArea, [this]() { engine_.ChangeState(EngineStates::kMenu); });
  button_manager_.AddButton("Schedules", SimulationsOptions::kSchedulesButtonArea, [this]() { engine_.ChangeState(EngineStates::kSchedules); });
  button_manager_.AddButton(GuiIconText(ICON_GEAR, ""), SimulationsOptions::kSettingsButtonArea, [this]() { settings_open_ = !settings_open_; });
  button_manager_.AddButton(GuiIconText(ICON_INFO, ""), SimulationsOptions::kInfoButtonArea, [this]() { info_open_ = !info_open_; });
  button_manager_.AddButton(GuiIconText(ICON_PLAYER_PREVIOUS, ""), SimulationsOptions::kBackwardsArea, [this]() { control_ = Controls::kBackward; });
  button_manager_.AddButton(GuiIconText(ICON_PLAYER_PLAY, ""), SimulationsOptions::kPauseResumeArea, [this]() {
    simulation_running_ = false; // Toggle simulation running state
    if (!simulation_running_)
      control_ = Controls::kStop;
  });
  button_manager_.AddButton(GuiIconText(ICON_PLAYER_NEXT, ""), SimulationsOptions::kForwardsArea, [this]() { control_ = Controls::kForward; });
  button_manager_.AddButton(GuiIconText(ICON_PLAYER_STOP, ""), SimulationsOptions::kStopArea, [this]() {
    simulation_running_ = false;
    control_ = Controls::kStop;
    simulation_progress_ = 0;
  });
}

void SimulationsState::Update() {
  last_update_s_ += GetFrameTime();
  button_manager_.UpdateButtons();
  if (IsTimeToUpdate()) {
    if (control_ == Controls::kForward) {
      if (simulation_progress_ < manager_->GetSimulation(current_simulation_).GetTotalTime())
        simulation_progress_ += 1;
    } else if (control_ == Controls::kBackward) {
      if (simulation_progress_ > 0)
        simulation_progress_ -= 1;
    }
    last_update_s_ = 0;
  }
  grid_.Update();
}

bool SimulationsState::IsTimeToUpdate() {
  return last_update_s_ >= 0.01f;
}

void SimulationsState::Draw() {
  grid_.Draw();
  DrawBackground();
  button_manager_.DrawButtons();
  if (manager_) {
    // Draw time etc;
  }
  if (info_open_)
    DrawInfo();
  if (settings_open_)
    DrawSettings();
}

void SimulationsState::SetSimulationsManager(const SimulationsManager* manager) {
  grid_.SetSimulationsManager(manager);
  manager_ = manager;
}

int SimulationsState::GetCurrentSimulation() const {
  return current_simulation_;
}

int SimulationsState::GetProgress() const {
  return simulation_progress_;
}


void SimulationsState::DrawInfo() {
  if (GuiWindowBox(SimulationsOptions::kInfoArea, "Info"))
    info_open_ = false;
}

void SimulationsState::DrawSettings() {
  if (GuiWindowBox(SimulationsOptions::kSettingsArea, "Settings"))
    settings_open_ = false;
}

void SimulationsState::DrawBackground() {
  // Top Navbar
  DrawRectangleRec(SimulationsOptions::kTopNavArea, LIGHTGRAY); // Navbar background
  // Bottom Navbar
  DrawRectangleRec(SimulationsOptions::kBottomNavArea, LIGHTGRAY); // Navbar background
  // Player Elements (Which are not buttons)
  unsigned start_time = manager_->GetSimulation(current_simulation_).GetStartTime();
  const std::string text = Parser::ConvertToTimeStringHHMMSS(start_time + simulation_progress_) + "/" + Parser::ConvertToTimeStringHHMMSS(manager_->GetSimulation(current_simulation_).GetTotalTime() + start_time);
  DrawText(text.c_str(), SimulationsOptions::kTimeProgressVec.x, SimulationsOptions::kTimeProgressVec.y, SimulationsOptions::kTextSize, DARKGRAY);
  GuiSliderBar(SimulationsOptions::kProgressBarArea, NULL, NULL, &simulation_progress_, 0, manager_->GetSimulation(current_simulation_).GetTotalTime());  
}