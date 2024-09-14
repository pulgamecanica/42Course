#include "ButtonManager.hpp"
#include "raygui.h"

void ButtonManager::AddButton(const std::string& name, Rectangle rect, std::function<void()> action) {
  button_rects_[name] = rect;
  button_actions_[name] = action;
}

void ButtonManager::UpdateButtons() {
  for (auto& button : button_rects_)
    if (GuiButton(button.second, button.first.c_str()))
      button_actions_[button.first]();
}

void ButtonManager::DrawButtons() {
  for (auto& button : button_rects_)
    GuiButton(button.second, button.first.c_str());
}
