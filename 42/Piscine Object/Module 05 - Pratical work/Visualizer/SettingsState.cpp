#include "SettingsState.hpp"
#include "Settings.hpp"
#include "SimulationsEngine.hpp"
#include "cpp_on_rails.inc"
#include "raygui.h"

#include <cstring>

namespace SettingsOptions {
  const Rectangle kMenuRec = {25, 25, 50, 25};
  const float kMidWidth = WIDTH / 2;
  const float kMidHeight = HEIGHT / 2;
  const int   kWindowWidth = (WIDTH / 3) * 2;
  const int   kWindowHeight = (HEIGHT / 5) * 4;
  const int   kMinimapWidth = 100;
  const int   kMinimapHeight = 60;
  const std::map<std::string, int> kOptionsMini = {
    {"width", kMinimapWidth}, 
    {"height", kMinimapHeight}
  };
  const Rectangle kWindowArea = {kMidWidth - (kWindowWidth / 2), kMidHeight - (kWindowHeight / 2), kWindowWidth, kWindowHeight};
  const Rectangle kSelectImageLabel = {kWindowArea.x + 42, kWindowArea.y + 42, 100, 30};
  const Rectangle kSelectImageButton = {kSelectImageLabel.x + kSelectImageLabel.width + 10, kSelectImageLabel.y, 140, 30};
  const Rectangle kSelectImageSizeLabel = {kSelectImageLabel.x, kSelectImageLabel.y + 50, 100, 30};
  const Rectangle kSelectImageSizeWidth = {kSelectImageSizeLabel.x + kSelectImageSizeLabel.width + 10, kSelectImageSizeLabel.y, 42, 30};
  const Rectangle kSelectImageSizeHeight = {kSelectImageSizeWidth.x + kSelectImageSizeWidth.width + 10, kSelectImageSizeWidth.y, 42, 30};
  const Vector2   kSelectImagePos = {kSelectImageButton.x + kSelectImageButton.width + 10, kSelectImageLabel.y - 10};
  const Rectangle kOutputDirLabel = {kSelectImageLabel.x, kSelectImageLabel.y + 100, 100, 30};
  const Rectangle kOutputDirBox = {kOutputDirLabel.x + kOutputDirLabel.width + 10, kOutputDirLabel.y, 242, 30};
  const Rectangle kToggleBidirectionalLabel = {kOutputDirLabel.x, kOutputDirLabel.y + 60, 100, 30};
  const Rectangle kToggleBidirectional = {kToggleBidirectionalLabel.x + 110, kToggleBidirectionalLabel.y, 200, 30};
  const Rectangle kDistanceLabel = {kToggleBidirectionalLabel.x, kToggleBidirectionalLabel.y + 60, 100, 30};
  const Rectangle kDistanceBox = {kDistanceLabel.x + kDistanceLabel.width + 10 , kDistanceLabel.y, 100, 30};
  const Rectangle kMaxSpeedLabel = {kDistanceLabel.x, kDistanceLabel.y + 60, 100, 30};
  const Rectangle kMaxSpeedBox = {kMaxSpeedLabel.x + kMaxSpeedLabel.width + 10, kMaxSpeedLabel.y, 100, 30};
  const Rectangle kMaxSpeedConvertLabel = {kMaxSpeedBox.x + kMaxSpeedBox.width + 30, kMaxSpeedLabel.y, 242, 30};
  const Rectangle kNodeSizeLabel = {kMaxSpeedLabel.x, kMaxSpeedLabel.y + 60, 142, 30};
  const Rectangle kNodeSizeBox = {kNodeSizeLabel.x + kNodeSizeLabel.width + 10, kNodeSizeLabel.y, 142, 30};
  const Rectangle kSaveRec = {kWindowArea.x + 10, kWindowArea.y + kWindowArea.height - 60, kWindowArea.width - 20, 50};

};

// Constructor initializes the state with a pointer to the Game object
SettingsState::SettingsState(SimulationsEngine& engine)
 :  engine_(engine),
    map_background_img_mini_(nullptr),
    output_direcctory_enabled_(false),
    bidirectional_toggle_(false),
    distance_preference_(0),
    map_width_(WIDTH),
    map_width_enabled_(false),
    map_height_(HEIGHT),
    map_height_enabled_(false),
    max_speed_(Settings::Instance().MaxTrainSpeed()),
    node_size_(Settings::Instance().GetNodeSize()) {
  button_manager_.AddButton("Home", SettingsOptions::kMenuRec, [this]() { engine_.ChangeState(EngineStates::kMenu); });
  button_manager_.AddButton("Save", SettingsOptions::kSaveRec, [this]() { Save(); });
  strcpy(output_direcctory_, Settings::Instance().GetOutputDirectory().c_str());
  file_dialog_state_ = InitGuiWindowFileDialog(GetWorkingDirectory());
  // std::vector<std::string> imgs = {"assets/images/background_map.png"};
  // std::map<std::string, int> options;
  //   options["width"] = 142;
  //   options["height"] = 142;
  // map_background_img_mini_ = std::make_unique<Animation>(imgs, 1, SettingsOptions::kOptionsMini);
}

void SettingsState::Update() {
  button_manager_.UpdateButtons();
}

void SettingsState::Save() {
  Settings::Instance().SetOutputDirectory(output_direcctory_);
  Settings::Instance().SetRailTwoWay(bidirectional_toggle_);
  Settings::Instance().SetDistancePreference(distance_preference_ == 0);
  Settings::Instance().SetBackground(map_background_img_path_, map_width_, map_height_);
  Settings::Instance().SetMapPosition({0, 0});
  Settings::Instance().SetMaxSpeed(max_speed_);
  Settings::Instance().SetNodeSize(node_size_);
}

void SettingsState::Draw() {
  GuiWindowBox(SettingsOptions::kWindowArea, "General Settings");
  button_manager_.DrawButtons();
  DrawMapSize();
  DrawOutputDirectory();
  DrawBidirectonal();
  DrawDistance();
  DrawMaxSpeed();
  DrawBackgroundSelector();
  DrawNodeSize();
}

void SettingsState::DrawNodeSize() {
  GuiLabel(SettingsOptions::kNodeSizeLabel, (std::string("Node Radius (") + std::to_string(node_size_) + "):").c_str());
  GuiSliderBar(SettingsOptions::kNodeSizeBox, "-", "+", &node_size_, 0.1f, 12.0f);
}

void SettingsState::DrawMapSize() {
  GuiLabel(SettingsOptions::kSelectImageSizeLabel, "Map Size");
  if (GuiValueBox(SettingsOptions::kSelectImageSizeWidth, "", &map_width_, 42, 4200, map_width_enabled_))
    map_width_enabled_ = !map_width_enabled_;
  if (GuiValueBox(SettingsOptions::kSelectImageSizeHeight, "x", &map_height_, 42, 4200, map_height_enabled_))
    map_height_enabled_ = !map_height_enabled_;
}

void SettingsState::DrawMaxSpeed() {
  GuiLabel(SettingsOptions::kMaxSpeedLabel, "Max Train Speed:");
  GuiSliderBar(SettingsOptions::kMaxSpeedBox, "-", "+", &max_speed_, 1.0f, 420.0f);
  GuiButton(SettingsOptions::kMaxSpeedConvertLabel, (std::to_string(max_speed_) + " m/s | " + std::to_string(max_speed_ * 3.6) + " km/h").c_str());
}

void SettingsState::DrawDistance() {
  GuiLabel(SettingsOptions::kDistanceLabel, "Distance Format:");
  GuiToggleSlider(SettingsOptions::kDistanceBox, "m;km", &distance_preference_);
}

void SettingsState::DrawBidirectonal() {
  GuiLabel(SettingsOptions::kToggleBidirectionalLabel, "Rails:");
  GuiToggle(SettingsOptions::kToggleBidirectional, bidirectional_toggle_ ? GuiIconText(ICON_OK_TICK, "Bidirectional") : GuiIconText(ICON_CROSS, "Bidirectional"), &bidirectional_toggle_);
}

void SettingsState::DrawOutputDirectory() {
  GuiLabel(SettingsOptions::kOutputDirLabel, "Output Directory:");
  GuiTextBox(SettingsOptions::kOutputDirBox, output_direcctory_, 111, output_direcctory_enabled_);
  if (GuiLabelButton(SettingsOptions::kOutputDirBox, "")) {
    output_direcctory_enabled_ = !output_direcctory_enabled_;
  }
}

void SettingsState::DrawBackgroundSelector() {
  if (map_background_img_mini_) {
    Rectangle rec_mini = {SettingsOptions::kSelectImagePos.x, SettingsOptions::kSelectImagePos.y, SettingsOptions::kMinimapWidth, SettingsOptions::kMinimapHeight};
    map_background_img_mini_->Draw(SettingsOptions::kSelectImagePos.x, SettingsOptions::kSelectImagePos.y);
    if (CheckCollisionPointRec(GetMousePosition(), rec_mini)) {
      DrawRectangleRec(rec_mini, Color(122, 42, 42, 122));
      GuiLabel(rec_mini, "-> Remove");
    }
    if (GuiLabelButton(rec_mini, "")) {
      map_background_img_path_ = "";
      map_background_img_mini_ = nullptr;
    }
  }
  if (file_dialog_state_.SelectFilePressed) {
  // Load image file (if supported extension)
  if (IsFileExtension(file_dialog_state_.fileNameText, ".png")) {
    std::vector<std::string> imgs;
    map_background_img_path_ = std::string(TextFormat("%s/%s", file_dialog_state_.dirPathText, file_dialog_state_.fileNameText));
    imgs.push_back(map_background_img_path_);
    map_background_img_mini_ = std::make_unique<Animation>(imgs, 1, SettingsOptions::kOptionsMini);
  }
  file_dialog_state_.SelectFilePressed = false;
  }

  // raygui: controls drawing
  if (file_dialog_state_.windowActive)
    GuiLock();

  GuiLabel(SettingsOptions::kSelectImageLabel, "Map Background:");
  if (GuiButton(SettingsOptions::kSelectImageButton, GuiIconText(ICON_FILE_OPEN, "Open Image")))
    file_dialog_state_.windowActive = true;
  GuiUnlock();

  GuiWindowFileDialog(&file_dialog_state_);
}