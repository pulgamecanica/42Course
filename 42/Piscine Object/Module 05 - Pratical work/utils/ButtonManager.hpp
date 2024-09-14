#ifndef BUTTON_MANAGER_H_
#define BUTTON_MANAGER_H_

#include <unordered_map>
#include <string>
#include <functional>
#include "raylib.h"

class ButtonManager {
public:
  void AddButton(const std::string& name, Rectangle rect, std::function<void()> action);
  void UpdateButtons();
  void DrawButtons();

private:
  std::unordered_map<std::string, Rectangle> button_rects_;
  std::unordered_map<std::string, std::function<void()>> button_actions_;
};

#endif // BUTTON_MANAGER_H_
