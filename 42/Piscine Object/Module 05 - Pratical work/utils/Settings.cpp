#include "Settings.hpp"
#include "RailwaySystem.hpp"
#include "FileLogger.hpp"
#include "ErrorHandler.hpp"

#include "cpp_on_rails.inc"

#include <sstream>
#include <vector>
#include <map>

std::mutex Settings::mutex_;
Settings* Settings::instance_ = nullptr;

Settings::Settings()
  : map_background_(nullptr), max_speed_(25.0f), rail_two_way_(false), show_node_names_(false), simulation_fps_(10), node_size_(4.2f), map_position_(0, 0) {
}

Settings& Settings::Instance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr) {
    instance_ = new Settings();
  }
  return *instance_;
}

void Settings::ToggleShowNodeNames() {
  show_node_names_ = !show_node_names_;
}

bool Settings::ShowNodeNames() const {
  return show_node_names_;
}

void Settings::InitializeAnimations() {
  std::vector<const char*> backgrounds = {
    "assets/images/hourglass_animation00.png", "assets/images/hourglass_animation01.png",
    "assets/images/hourglass_animation02.png", "assets/images/hourglass_animation03.png",
    "assets/images/hourglass_animation04.png", "assets/images/hourglass_animation05.png",
    "assets/images/hourglass_animation06.png", "assets/images/hourglass_animation07.png",
    "assets/images/hourglass_animation08.png", "assets/images/hourglass_animation09.png",
    "assets/images/hourglass_animation10.png", "assets/images/hourglass_animation11.png",
    "assets/images/hourglass_animation12.png", "assets/images/hourglass_animation13.png",
    "assets/images/hourglass_animation14.png", "assets/images/hourglass_animation15.png" };
  std::map<std::string, int> options = {{"width", 42}, {"height", 42}};
  hourglass_animation_ = std::make_unique<Animation>(backgrounds, 0.05f, options);
}

void Settings::UpdateAnimations() {
  if (hourglass_animation_)
    hourglass_animation_->Update();
}

void Settings::DrawHourglass(int x, int y) const {
  if (hourglass_animation_)
    hourglass_animation_->Draw(x, y);
}

void Settings::SetDataFileName(const std::string& filename) {
  std::lock_guard<std::mutex> lock(mutex_);
  data_file_name_ = filename;
}

void Settings::SetScheduleDirectory(const std::string& directory) {
  std::lock_guard<std::mutex> lock(mutex_);
  schedule_directory_ = directory;
}

void Settings::SetOutputDirectory(const std::string& directory) {
  std::lock_guard<std::mutex> lock(mutex_);
  output_directory_ = directory;
}

void Settings::SetSimulationFPS(float fps) {
  if (fps <= 0 || fps > 42) return;
  simulation_fps_ = fps;
}

void Settings::SetMaxSpeed(double max_speed) {
  if (max_speed_ > 0)
    max_speed_ = max_speed;
}

const std::string& Settings::GetDataFileName() const {
  return data_file_name_;
}

const std::string& Settings::GetScheduleDirectory() const {
  return schedule_directory_;
}

const std::string& Settings::GetOutputDirectory() const {
  return output_directory_;
}

float Settings::GetSimulationFPS() const {
  return simulation_fps_;
}

float Settings::GetNodeSize() const {
  return node_size_;
}

void Settings::SetNodeSize(float size) {
  if (size >= 0.1f && size <= 42.0f)
    node_size_ = size;
}

void Settings::SaveRailwayNodePositions(RailwaySystem& rail_sys) {
  std::map<std::string, Vector2> nodes_positions;

  // Read previous positions
  std::ifstream file(Settings::Instance().GetNodePositionsFileName());
  if (!file.is_open()) {
    ErrorHandler::ReportError(Settings::Instance().GetNodePositionsFileName(), 0, 0, "Failed to open file", "");
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string node_name;
    float x, y;

    // Parse the line for node name, x and y coordinates
    if (!(ss >> node_name >> x >> y)) {
      ErrorHandler::ReportError(Settings::Instance().GetNodePositionsFileName(), file.tellg(), 0, "Malformed line", line);
      continue;  // Skip this line and continue reading
    }
    try {
      nodes_positions[node_name] = (Vector2){x, y};
    } catch (std::exception &e) {(void)e;}
  }
  file.close();

  // Update positions and new nodes
  for (const auto& [name, node] : rail_sys.GetNodes()) {
    nodes_positions[name] = node->GetPosition();
  }

  // Write the nodes
  FileLogger node_positions_log(GetNodePositionsFileName());
  for (const auto& [name, pos] : nodes_positions) {
    node_positions_log.write((name + " " + std::to_string(pos.x) + " " + std::to_string(pos.y).c_str()));
  }
}

const std::string   Settings::GetNodePositionsFileName() const {
  return std::string(POSITIONS_FILENAME);
}

bool Settings::IsRailTwoWay() const {
  return rail_two_way_;
}

void Settings::SetRailTwoWay(bool b) {
  rail_two_way_ = b;
}

void Settings::SetDistancePreference(bool meters) {
  distance_preference_in_meters_ = meters;
}

double  Settings::MaxTrainSpeed() const {
  return max_speed_;
}

void Settings::DrawMapBackground() const {
  if (map_background_)
    map_background_->Draw(map_position_.x, map_position_.y);
}

void Settings::SetBackground(std::string background_image_path, int width, int height) {
  if (background_image_path.empty()) {
    map_background_ = nullptr;
  } else {
    std::map<std::string, int> options = {{"width", width}, {"height", height}};
    std::vector<std::string> imgs = { background_image_path };
    map_background_ = std::make_unique<Animation>(imgs, 1, options);
  }
}

void Settings::SetMapPosition(Vector2 pos) {
  if (map_background_)
    map_position_ = pos;
}

const Vector2 Settings::GetMapPosition() const {
  return map_position_;
}

bool Settings::PreferMeters() const {
  return distance_preference_in_meters_;
}


#include "raylib.h"
#include "raygui.h"

void Settings::DrawLoadingScreen(float progress, const std::string loading_info, const std::string loading_info_element) {
  float screen_width = GetScreenWidth();
  float screen_height = GetScreenHeight();

  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawText("Loading Program...", screen_width / 2 - 100, screen_height / 2 - 80, 20, DARKGRAY);
  GuiProgressBar((Rectangle){screen_width / 2 - 150, screen_height / 2 - 30, 300, 30}, NULL, NULL, &progress, 0.0f, 1.0f);
  DrawText(TextFormat("Loading... %.0f%%", progress * 100), screen_width / 2 - 60, screen_height / 2 + 20, 20, DARKGRAY);
  DrawText(loading_info.c_str(), screen_width / 2 - 140, screen_height / 2 + 50, 18, GREEN);
  if (!loading_info_element.empty())
    DrawText(loading_info_element.c_str(), screen_width / 2 - 140, screen_height / 2 + 80, 12, LIME);
  DrawHourglass(screen_width - 42, screen_height - 42);
  EndDrawing();
  UpdateAnimations();
}