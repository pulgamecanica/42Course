#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Animation.hpp"
#include <string>
#include <memory>
#include <mutex>

class RailwaySystem;

class Settings {
public:
  static Settings& Instance();

  // Prevent copy construction and assignment
  Settings(const Settings&) = delete;
  Settings& operator=(const Settings&) = delete;

  // Setters
  void SetDataFileName(const std::string& filename);
  void SetScheduleDirectory(const std::string& directory);
  void SetOutputDirectory(const std::string& directory);
  void SetSimulationFPS(float fps);
  void SetRailTwoWay(bool b);
  void SetDistancePreference(bool meters);
  void SetBackground(std::string background_image_path, int width, int height);
  void SetMapPosition(Vector2 pos);
  void SetMaxSpeed(double max_speed);
  void ToggleShowNodeNames();
  void SetNodeSize(float size);

  // Getters
  double              MaxTrainSpeed() const;
  const std::string&  GetDataFileName() const;
  const std::string&  GetScheduleDirectory() const;
  const std::string&  GetOutputDirectory() const;
  const std::string   GetNodePositionsFileName() const;
  const Vector2       GetMapPosition() const;
  bool                PreferMeters() const;
  bool                ShowNodeNames() const;
  float               GetNodeSize() const;
  float               GetSimulationFPS() const;
  bool                IsRailTwoWay() const;

  // Others
  void DrawLoadingScreen(float progress, const std::string loading_info, const std::string loading_info_element = "");
  void SaveRailwayNodePositions(RailwaySystem& rail_sys);
  void InitializeAnimations();
  void DrawHourglass(int x, int y) const;
  void DrawMapBackground() const;
  void UpdateAnimations();
private:
  Settings();  // Private constructor
  ~Settings() = default;

  static std::mutex mutex_;
  static Settings* instance_;

  std::string data_file_name_;
  std::string schedule_directory_;
  std::string output_directory_;

  std::unique_ptr<Animation> hourglass_animation_;
  std::unique_ptr<Animation> map_background_;

  double max_speed_;
  bool   rail_two_way_;
  bool   show_node_names_;
  bool   distance_preference_in_meters_;
  float simulation_fps_;
  float node_size_;
  Vector2 map_position_;

};

#endif // SETTINGS_HPP