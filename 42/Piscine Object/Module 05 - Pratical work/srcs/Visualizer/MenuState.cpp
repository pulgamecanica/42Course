#include "Visualizer/MenuState.hpp"
#include "SimulationsEngine.hpp"

#include "raylib.h"

#include "raygui.h"


// Constructor initializes the state with a pointer to the Game object
MenuState::MenuState(SimulationsEngine& engine)
  : engine_(engine),
    show_info_popup_(false),
    background_animation_(nullptr) {
  float screenHeight = GetScreenHeight();
  float screenWidth = GetScreenWidth();
  std::vector<const char*> backgrounds = { "assets/images/background.png", "assets/images/background2.png", "assets/images/background3.png" };
  std::map<std::string, int> options = {{"width", screenWidth}, {"height", screenHeight}, {"tint frames cycle", 1200}};
  background_animation_ = std::make_unique<Animation>(backgrounds, 42.0f, options); // This means, 42 seconds for each frame, which is a lot wow

  button_manager_.AddButton("Info", {20, screenHeight - 60, 100, 50}, [this]() { show_info_popup_ = true; });
  button_manager_.AddButton("Simulations", {screenWidth / 2 - 50, screenHeight / 2 - 75, 100, 50}, [this]() { engine_.ChangeState(EngineStates::SCHEDULES); });
  button_manager_.AddButton("Network", {screenWidth / 2 - 50, screenHeight / 2, 100, 50}, [this]() { engine_.ChangeState(EngineStates::NETWORK); });
  button_manager_.AddButton("Settings", {screenWidth / 2 - 50, screenHeight / 2 + 75, 100, 50}, [this]() { engine_.ChangeState(EngineStates::SETTINGS); });
  button_manager_.AddButton("Light Theme", {screenWidth - 105, screenHeight - 35, 100, 30}, []() { GuiLoadStyleDefault(); GuiLoadStyle("assets/themes/red_theme.rgs"); });
  button_manager_.AddButton("Dark Theme", {screenWidth - 210, screenHeight - 35, 100, 30}, []() { GuiLoadStyleDefault(); GuiLoadStyle("assets/themes/dark_theme.rgs"); });
  button_manager_.AddButton("Reset Theme", {screenWidth - 315, screenHeight - 35, 100, 30}, []() { GuiLoadStyleDefault(); });
}

void MenuState::Update() {  
  // Update the background animation
  background_animation_->UpdateAnimation(GetFrameTime());
  // Update buttons
  if (!show_info_popup_) {
    button_manager_.UpdateButtons();
  } else {
    if(GuiWindowBox(Rectangle(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400), "Info"))
      show_info_popup_ = false;
  }
}

void MenuState::Draw() {
  // Draw the background animation
  background_animation_->DrawAnimation(0, 0);
  // Draw GUI TODO
 
  // Draw text
  DrawText("Cpp On Rails", GetScreenWidth() / 2 - MeasureText("Cpp On Rails", 40) / 2, 20, 40, BLACK);

  // Draw buttons
  button_manager_.DrawButtons();
  // Draw info
  if (show_info_popup_) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color(42, 42, 42, 200));
    Rectangle popup_rect(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400);
    Rectangle pupup_info(GetScreenWidth() / 2 - 300 + 20, GetScreenHeight() / 2 - 200, 600 - 40, 120);
    GuiWindowBox(popup_rect, "Info");
    GuiLabel(pupup_info, "Cpp On Rails\nAuthor: pulgamecanica | aka: @arosado\nYear: 2024");
  }
}
