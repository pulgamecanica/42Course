#include "SimulationsEngine.hpp"
#include "SimulationPanelItem.hpp"
#include "Settings.hpp"
#include "raygui.h"
#include "Parser.hpp"
#include <string.h>

SimulationPanelItem::SimulationPanelItem(SimulationsEngine& engine, const SimulationsManager * simulations_manager, Rectangle bounds) 
  : simulations_manager_(simulations_manager),
    engine_(engine),
    bounds_(bounds) {
    // int simulations = simulations_manager_.GetSimulations().size();
    // Each Simulation is about 20 pixels:
    bounds_.height = 42;
  }

Rectangle SimulationPanelItem::GetRectangle() const {
  return bounds_;
}

int SimulationPanelItem::GetX() const {
  return bounds_.x;
}

int SimulationPanelItem::GetY() const {
  return bounds_.y;
}

void SimulationPanelItem::SetXY(Vector2 new_pos) {
  bounds_.x = new_pos.x;
  bounds_.y = new_pos.y;
}

float SimulationPanelItem::GetHeight() const {
  return bounds_.height;
}

float SimulationPanelItem::GetWidth() const {
  return bounds_.width;
}

#include <cstring>

void SimulationPanelItem::Draw(Vector2 scroll) const {
  // Calculate the total height:
  // Number of simulations: * height for indivudual train info 
  // Footer: for the total traveled time unles its not finished
  // Set the height 
  // Iterate through each train and draw train info
  // Draw Optimal tavel time
  // If finished draw the real traveled time
  // Else draw the percentage of the train simulation distance
  bool finished = simulations_manager_->AreSimulationsFinished();
  if (finished) {
    if (GuiButton((Rectangle){
      GetX() + scroll.x,
      GetY() + scroll.y,
      GetWidth(),
      GetHeight()
      }, "")) {

      //
      //
      engine_.SetSimulationsManager(simulations_manager_);
      engine_.ChangeState(EngineStates::kSimulation);
      //
      //
    }
  } else {
    DrawRectangleRec((Rectangle){
      GetX() + scroll.x,
      GetY() + scroll.y,
      GetWidth(),
      GetHeight()
    }, LIGHTGRAY);
    Settings::Instance().DrawHourglass(
      GetX() + scroll.x + (bounds_.width / 2) - 21,
      GetY() + scroll.y);
  }
  char * schedule_name = strdup(std::string(simulations_manager_->GetSchedule().GetName() + " (" + std::to_string(simulations_manager_->GetSimulations().size()) + ")").c_str());

  DrawText(
    schedule_name,
    GetX() + scroll.x + 8,
    GetY() + scroll.y + 16,
    10,
    BLACK);
  DrawText(
    Parser::ConvertToTimeStringHHMMSS(simulations_manager_->GetCurrentTime()).c_str(),
    GetWidth() + scroll.x,
    GetY() + scroll.y + 16,
    10,
    BLACK);
  free(schedule_name);
  // int y = bounds_.y;
  // for (auto & simulation : simulations_manager_.GetSimulations()) {
  //   std::string text = "Optimal Travel Time: ";
  //   text += std::to_string(simulation->GetRealTravelTime());
  //   DrawText(text.c_str(), bounds_.x + 5, y, 12, WHITE);
  //   y += 20;
  // }
}