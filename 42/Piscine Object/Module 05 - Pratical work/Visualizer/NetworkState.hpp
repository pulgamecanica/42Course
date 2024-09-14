#ifndef NETWORK_STATE_H_
#define NETWORK_STATE_H_

#include "IState.hpp"
#include "ButtonManager.hpp"
#include "EditableGrid.hpp"
#include "Animation.hpp"
#include "raylib.h"

#include <memory>

class SimulationsEngine;

class NetworkState : public IState {
public:
  explicit NetworkState(SimulationsEngine& engine);
  void Update() override;
  void Draw() override;

private:
  void InitializeBackgroundAnimation();
  void InitializeBlueTrainAnimation();
  void InitializeBlueTrainRailsAnimation();
  void InitializeBlueTrainWoodsAnimation();
  void UpdateBlueTrainPosition();
  
  std::string GetMousePositionText() const;

  SimulationsEngine& engine_;
  EditableGrid editable_grid_;
  ButtonManager button_manager_;
  std::unique_ptr<Animation> background_animation_;
  std::unique_ptr<Animation> blue_train_;
  std::unique_ptr<Animation> blue_train_rails_;
  std::unique_ptr<Animation> blue_train_woods_;
  int blue_train_animation_y_;
  float blue_train_pos_x_factor_;

};

#endif // NETWORK_STATE_H_