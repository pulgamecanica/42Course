//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef SIMULATIONGRID_HPP_
# define SIMULATIONGRID_HPP_

#include "Grid.hpp"
#include "SimulationsManager.hpp"
#include "Animation.hpp"

#include <memory>
#include <iostream>

class SimulationsState;

class SimulationGrid: public Grid {
public:
  SimulationGrid(RailwaySystem &rail_sys, SimulationsState& sim_state, float gridSize, Rectangle displayArea);

  void Update() override;
  void Draw() override;
  void SetSimulationsManager(const SimulationsManager* manager);
private:
  void DrawTrains();
  void DrawEvents();
  void DrawTrainsElements();
  void DrawEventsElements();
  void DrawMenu();
  void DrawElementsBG(const std::string& title);
  void DrawTrain(const std::shared_ptr<TrainSimulationState>& train);

  const SimulationsManager* manager_;
  const Rectangle elements_menu_rec_;
  RailwaySystem& rail_sys_;
  SimulationsState& sim_state_;

  std::unique_ptr<Animation> train_icon_;
  std::unique_ptr<Animation> rail_icon_;
  std::unique_ptr<Animation> event_icon_;
  std::unique_ptr<Animation> event_icon_small_;

  bool menu_opened_;
  bool element_menu_opened_;
  bool trains_element_open_;
  bool rails_element_open_;
  bool events_element_open_;

  Vector2 elements_menu_scroll_;
  Rectangle elements_rec_;
  Rectangle view_;

  TrainSimulation* selected_train_;
};
#endif // SIMULATIONGRID_HPP_
