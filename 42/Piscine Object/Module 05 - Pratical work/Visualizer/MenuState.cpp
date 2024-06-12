#include "MenuState.hpp"
#include "Game.hpp"

#include "raylib.h"
#include "raygui.h"


// Constructor initializes the state with a pointer to the Game object
MenuState::MenuState(Game* game)
  : game_(game),
    show_info_popup_(false),
    background_animation_(nullptr) {
  std::vector<const char*> backgrounds = { "assets/images/background.png", "assets/images/background2.png", "assets/images/background3.png" };
  std::map<std::string, int> options = {{"width", GetScreenWidth()}, {"height", GetScreenHeight()}, {"tint frames cycle", 1200}};
  background_animation_ = std::make_unique<Animation>(backgrounds, 19.0f, options); // This means, 42 seconds for each frame, which is a lot wow
  
  // Initialize button states
  button_states_["Info"] = false;
  button_states_["Simulation"] = false;
  button_states_["Network"] = false;
  button_states_["Settings"] = false;
  button_states_["Light Theme"] = false;
  button_states_["Dark Theme"] = false;
  button_states_["Reset Theme"] = false;
  // Add more buttons here, if you want to see the button on the screen you need to set it's rectangle

  // Define button rectangles
  button_rects_["Info"] = {20, GetScreenHeight() - 60, 100, 50};
  button_rects_["Simulation"] = {GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 - 75, 100, 50};
  button_rects_["Network"] = {GetScreenWidth() / 2 - 50, GetScreenHeight() / 2, 100, 50};
  button_rects_["Settings"] = {GetScreenWidth() / 2 - 50, GetScreenHeight() / 2 + 75, 100, 50};
  button_rects_["Light Theme"] = {GetScreenWidth() - 105, GetScreenHeight() - 35, 100, 30};
  button_rects_["Dark Theme"] = {GetScreenWidth() - 210, GetScreenHeight() - 35, 100, 30};
  button_rects_["Reset Theme"] = {GetScreenWidth() - 315, GetScreenHeight() - 35, 100, 30};
  // Add more rects for the buttons
}

void MenuState::Update() {  
  // Update the background animation
  background_animation_->UpdateAnimation(GetFrameTime());

  // Set all the button states
  for (std::pair<std::string, bool> button : button_states_)
    button_states_[button.first] = GuiButton(button_rects_[button.first], button.first.c_str());

  if (button_states_["Info"])
    show_info_popup_ = true;
  if (show_info_popup_) {
    if(GuiWindowBox(Rectangle(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400), "Info"))
      show_info_popup_ = false;
  } else { // Buttons work only when info is not present in the screen
    if (button_states_["Simulation"])
      game_->ChangeState(&game_->simulation_state_);
    else if (button_states_["Network"])
      game_->ChangeState(&game_->network_state_);
    else if (button_states_["Settings"])
      game_->ChangeState(&game_->settings_state_);
    else if (button_states_["Light Theme"]) {
      GuiLoadStyleDefault();
      GuiLoadStyle("assets/themes/red_theme.rgs");
    } else if (button_states_["Dark Theme"]) {
      GuiLoadStyleDefault();
      GuiLoadStyle("assets/themes/dark_theme.rgs");
    } else if (button_states_["Reset Theme"]) {
      GuiLoadStyleDefault();
    }
    // Add here more buttons logic if you want
  }
}

void MenuState::Draw() {
  // Draw the background animation
  background_animation_->DrawAnimation(0, 0);

 // Draw text
  DrawText("Cpp On Rails", GetScreenWidth() / 2 - MeasureText("Cpp On Rails", 40) / 2, 20, 40, BLACK);

  // Draw buttons
  for (std::pair<std::string, bool> button : button_states_)
    GuiButton(button_rects_[button.first], button.first.c_str());

  // Draw info popup if needed
  // Draw info popup if needed
  if (show_info_popup_) {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color(42, 42, 42, 200));
    Rectangle popup_rect(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400);
    Rectangle pupup_info(GetScreenWidth() / 2 - 300 + 20, GetScreenHeight() / 2 - 200, 600 - 40, 120);
    GuiWindowBox(popup_rect, "Info");
    GuiLabel(pupup_info, "Cpp On Rails\nAuthor: Your Name\nYear: 2024");
  }
}
