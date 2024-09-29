#ifndef SETTINGS_STATE_H_
#define SETTINGS_STATE_H_

#include "IState.hpp"
#include "ButtonManager.hpp"
#include "Animation.hpp"

#include "gui_window_file_dialog.h"

#include <memory>

class SimulationsEngine;

class SettingsState : public IState {
public:
  explicit SettingsState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  void DrawBackgroundSelector();
  void DrawOutputDirectory();
  void DrawMapSize();
  void DrawBidirectonal();
  void DrawDistance();
  void DrawMaxSpeed();
  void DrawNodeSize();
  void Save();

  SimulationsEngine& engine_;
  ButtonManager button_manager_;
  GuiWindowFileDialogState file_dialog_state_;
  std::unique_ptr<Animation> map_background_img_mini_;
  std::string map_background_img_path_;
  bool output_direcctory_enabled_;
  char output_direcctory_[112] = { 0 };
  bool bidirectional_toggle_;
  int distance_preference_;
  int map_width_;
  bool map_width_enabled_;
  int map_height_;
  bool map_height_enabled_;
  float max_speed_;
  float node_size_;
};

#endif // SETTINGS_STATE_H_