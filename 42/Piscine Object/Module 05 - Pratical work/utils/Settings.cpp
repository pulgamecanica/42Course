#include "Settings.hpp"
#include <vector>
#include <map>

std::mutex Settings::mutex_;
Settings* Settings::instance_ = nullptr;

Settings::Settings()
  : rail_two_way_(false) {
  // std::vector<const char*> backgrounds = {
  //   "assets/images/hourglass_animation00.png", "assets/images/hourglass_animation01.png",
  //   "assets/images/hourglass_animation02.png", "assets/images/hourglass_animation03.png",
  //   "assets/images/hourglass_animation04.png", "assets/images/hourglass_animation05.png",
  //   "assets/images/hourglass_animation06.png", "assets/images/hourglass_animation07.png",
  //   "assets/images/hourglass_animation08.png", "assets/images/hourglass_animation09.png",
  //   "assets/images/hourglass_animation10.png", "assets/images/hourglass_animation11.png",
  //   "assets/images/hourglass_animation12.png", "assets/images/hourglass_animation13.png",
  //   "assets/images/hourglass_animation14.png", "assets/images/hourglass_animation15.png" };
  // std::map<std::string, int> options = {{"width", 42}, {"height", 42}};
  // hourglass_animation_ = std::make_unique<Animation>(backgrounds, 0.05f, options);
  max_speed_ = 100;
}

Settings& Settings::Instance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance_ == nullptr) {
    instance_ = new Settings();
  }
  return *instance_;
}

// void Settings::UpdateAnimations() {
//   hourglass_animation_->Update();
// }

// void Settings::DrawHourglass(int x, int y) const {
//   hourglass_animation_->Draw(x, y);
// }

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

const std::string& Settings::GetDataFileName() const {
  return data_file_name_;
}

const std::string& Settings::GetScheduleDirectory() const {
  return schedule_directory_;
}

const std::string& Settings::GetOutputDirectory() const {
  return output_directory_;
}

bool Settings::IsRailTwoWay() const {
  return rail_two_way_;
}

double  Settings::MaxTrainSpeed() const {
  return max_speed_;
}