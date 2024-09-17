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

class SimulationGrid: public Grid {
public:
  SimulationGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea);

  void Update() override;
  void Draw() override;
  void SetSimulationsManager(const SimulationsManager* manager);
  void SetCurrentSimulation(int sim);
private:
  void DrawTrainElements();
  void DrawRailElements();
  void DrawEventsElement();
  void DrawElementsBG(const std::string& title);

  const SimulationsManager* manager_;

  const Rectangle elements_menu_rec_;

  std::unique_ptr<Animation> train_icon_;
  std::unique_ptr<Animation> rail_icon_;
  std::unique_ptr<Animation> event_icon_;

  bool menu_opened_;
  bool element_menu_opened_;
  bool trains_element_open_;
  bool rails_element_open_;
  bool events_element_open_;

  Vector2 elements_menu_scroll_;

  int current_simulation_;
};
#endif // SIMULATIONGRID_HPP_
