#include "cpp_on_rails.inc"

#include "MenuState.hpp"
#include "SimulationsEngine.hpp"
#include "Settings.hpp"
#include "raylib.h"
#include "raygui.h"

#include <sstream>
#include <iomanip>

namespace {
  // const int kTotalFrames = 145;
  const int kTotalFrames = 2;
  const float kBackgroundFrameDuration = 0.4f;  // 42 seconds for each frame
  const int kTintFramesCycle = 12000;
  const float kScreenWidth = WIDTH;
  const float kScreenHeight = HEIGHT;
  const Vector2 kInfoButtonPos = {20, kScreenHeight - 60};
  const Vector2 kSimulationsButtonPos = {kScreenWidth / 2 - 50, kScreenHeight / 2 - 75};
  const Vector2 kNetworkButtonPos = {kScreenWidth / 2 - 50, kScreenHeight / 2};
  const Vector2 kSettingsButtonPos = {kScreenWidth / 2 - 50, kScreenHeight / 2 + 75};
  const Vector2 kLightThemeButtonPos = {kScreenWidth - 105, kScreenHeight - 35};
  const Vector2 kDarkThemeButtonPos = {kScreenWidth - 210, kScreenHeight - 35};
  const Vector2 kResetThemeButtonPos = {kScreenWidth - 315, kScreenHeight - 35};
}

MenuState::MenuState(SimulationsEngine& engine)
  : engine_(engine), 
    show_info_popup_(false), 
    background_animation_(nullptr) {

    InitializeLoadingScreen();
    InitializeBackgroundAnimation();
    InitializeButtons();
}

void MenuState::InitializeLoadingScreen() {
  Settings::Instance().DrawLoadingScreen(0.0f, "Loading Simulations Engine [MENU]");
}

void MenuState::InitializeBackgroundAnimation() {
  std::vector<std::string> backgrounds;
  for (int i = 1; i <= kTotalFrames; ++i) {
    std::stringstream ss;
    ss << "assets/images/train_lapse_animation/frame-" 
     << std::setw(3) << std::setfill('0') << i << ".png";
  
    Settings::Instance().DrawLoadingScreen(static_cast<float>(i) / kTotalFrames, 
                                           "Loading Simulations Engine [MENU]", 
                                           ss.str());
    backgrounds.push_back(ss.str());
  }
  
  std::map<std::string, int> options = {
    {"width", GetScreenWidth()}, 
    {"height", GetScreenHeight()}, 
    {"tint frames cycle", kTintFramesCycle}
  };
  background_animation_ = std::make_unique<Animation>(backgrounds, kBackgroundFrameDuration, options);
}

void MenuState::InitializeButtons() {
    button_manager_.AddButton("Info", {kInfoButtonPos.x, kInfoButtonPos.y, 100, 50}, 
                              [this]() { show_info_popup_ = true; });
    
    button_manager_.AddButton("Simulations", {kSimulationsButtonPos.x, kSimulationsButtonPos.y, 100, 50}, 
                              [this]() { engine_.ChangeState(EngineStates::kSchedules); });
    
    button_manager_.AddButton("Network", {kNetworkButtonPos.x, kNetworkButtonPos.y, 100, 50}, 
                              [this]() { engine_.ChangeState(EngineStates::kNetwork); });
    
    button_manager_.AddButton("Settings", {kSettingsButtonPos.x, kSettingsButtonPos.y, 100, 50}, 
                              [this]() { engine_.ChangeState(EngineStates::kSettings); });

    button_manager_.AddButton("Light Theme", {kLightThemeButtonPos.x, kLightThemeButtonPos.y, 100, 30}, 
                              []() { GuiLoadStyleDefault(); GuiLoadStyle("assets/themes/red_theme.rgs"); });
    
    button_manager_.AddButton("Dark Theme", {kDarkThemeButtonPos.x, kDarkThemeButtonPos.y, 100, 30}, 
                              []() { GuiLoadStyleDefault(); GuiLoadStyle("assets/themes/dark_theme.rgs"); });
    
    button_manager_.AddButton("Reset Theme", {kResetThemeButtonPos.x, kResetThemeButtonPos.y, 100, 30}, 
                              []() { GuiLoadStyleDefault(); });
    
    Settings::Instance().DrawLoadingScreen(1.0f, "Loading Simulations Engine [MENU]");
}

void MenuState::Update() {  
  background_animation_->Update();
  
  if (!show_info_popup_)
    button_manager_.UpdateButtons();
  else if (GuiWindowBox(Rectangle(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400), "Info"))
    show_info_popup_ = false;
}

void MenuState::Draw() {
  background_animation_->Draw(0, 0);

  DrawText("Cpp On Rails", GetScreenWidth() / 2 - MeasureText("Cpp On Rails", 40) / 2, 20, 40, BLACK);

  button_manager_.DrawButtons();

  if (show_info_popup_)
    DrawInfoPopup();
}

void MenuState::DrawInfoPopup() {
  DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color(42, 42, 42, 200));
  
  Rectangle popup_rect(GetScreenWidth() / 2 - 300, GetScreenHeight() / 2 - 200, 600, 400);
  Rectangle popup_info(GetScreenWidth() / 2 - 300 + 20, GetScreenHeight() / 2 - 200, 600 - 40, 120);

  GuiWindowBox(popup_rect, "Info");
  GuiLabel(popup_info, "Cpp On Rails\nAuthor: pulgamecanica | aka: @arosado\nYear: 2024");
}
