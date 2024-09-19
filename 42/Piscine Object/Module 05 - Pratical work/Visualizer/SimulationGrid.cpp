//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SimulationGrid.hpp"
#include "Settings.hpp"
#include "SimulationsState.hpp"
#include "Parser.hpp"
#include "cpp_on_rails.inc"

#include "raygui.h"

namespace SimulationGridOptions {
  const float kMenuHeight = 42;
  const float kMenuWidth = 100;
  Rectangle kMenuBGArea = {WIDTH - kMenuWidth, 0, kMenuWidth, kMenuHeight};
  Rectangle kCloseMenuArea = {WIDTH - (kMenuWidth + 15), 0, 15, kMenuHeight};
  Rectangle kOpenMenuArea = {WIDTH - 15, 0, 15, kMenuHeight};
  const int kElementsMenuWidth = 242;
  const int kIconPadding = 5;
  const int kIconButtonSize = (kMenuWidth / 3);
  const int kIconSize = kIconButtonSize - kIconPadding;
  const std::map<std::string, int> kOptions = {
    {"width", kIconSize}, 
    {"height", kIconSize}
  };
  const std::vector<const char*> kTrainIconImages = {
    "assets/images/train_icon.png"
  };
  const std::vector<const char*> kRailIconImages = {
    "assets/images/rail_icon.png"
  };
  const std::vector<const char*> kEventIconImages = {
    "assets/images/event_icon.png"
  };
}

SimulationGrid::SimulationGrid(RailwaySystem &rail_sys, SimulationsState& sim_state, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea), 
    manager_(nullptr),
    elements_menu_rec_(displayArea.x + 5, display_area_.y + 5, SimulationGridOptions::kElementsMenuWidth, display_area_.height),
    rail_sys_(rail_sys),
    sim_state_(sim_state),
    menu_opened_(true),
    element_menu_opened_(false),
    trains_element_open_(false),
    rails_element_open_(false),
    events_element_open_(false) {

  Settings::Instance().DrawLoadingScreen(1 / 4,"Loading Simulation Grid", "Train Icon");
  train_icon_ = std::make_unique<Animation>(SimulationGridOptions::kTrainIconImages, 1.0f, SimulationGridOptions::kOptions);

  Settings::Instance().DrawLoadingScreen(2 / 4,"Loading Simulation Grid", "Rail Icon");
  rail_icon_ = std::make_unique<Animation>(SimulationGridOptions::kRailIconImages, 1.0f, SimulationGridOptions::kOptions);

  Settings::Instance().DrawLoadingScreen(3 / 4,"Loading Simulation Grid", "Event Icon");
  event_icon_ = std::make_unique<Animation>(SimulationGridOptions::kEventIconImages, 1.0f, SimulationGridOptions::kOptions);

  SimulationGridOptions::kMenuBGArea.y += displayArea.y;
  SimulationGridOptions::kOpenMenuArea.y += displayArea.y;
  SimulationGridOptions::kCloseMenuArea.y += displayArea.y;
  Settings::Instance().DrawLoadingScreen(4 / 4,"Loading Simulation Grid");
}

void SimulationGrid::Update() {
  if (!manager_) return;
  if ((!element_menu_opened_ || (element_menu_opened_ && !CheckCollisionPointRec(GetMousePosition(), elements_menu_rec_)))) {
    Grid::Update();
  }
  ; // TODO
}

void SimulationGrid::Draw() {
  if (manager_) {
    Grid::Draw();
    if (menu_opened_) {
      DrawRectangleRec(SimulationGridOptions::kMenuBGArea, GRAY);
      if (GuiButton(SimulationGridOptions::kCloseMenuArea, GuiIconText(ICON_ARROW_RIGHT_FILL, "")))
        menu_opened_ = false;
      
      float icon_y = SimulationGridOptions::kMenuBGArea.y + (SimulationGridOptions::kMenuBGArea.height / 2) - (SimulationGridOptions::kIconButtonSize / 2);
      float icon_x = SimulationGridOptions::kMenuBGArea.x;

      bool clicked_train = GuiButton((Rectangle){icon_x, icon_y, SimulationGridOptions::kIconButtonSize, SimulationGridOptions::kIconButtonSize} , "");
      icon_x += SimulationGridOptions::kIconButtonSize;
      bool clicked_rail  = GuiButton((Rectangle){icon_x, icon_y, SimulationGridOptions::kIconButtonSize, SimulationGridOptions::kIconButtonSize} , "");
      icon_x += SimulationGridOptions::kIconButtonSize;
      bool clicked_event = GuiButton((Rectangle){icon_x, icon_y, SimulationGridOptions::kIconButtonSize, SimulationGridOptions::kIconButtonSize} , "");
      
      icon_y += SimulationGridOptions::kIconPadding / 2;
      icon_x = SimulationGridOptions::kMenuBGArea.x + SimulationGridOptions::kIconPadding / 2;
      train_icon_->Draw(icon_x, icon_y);
      icon_x += SimulationGridOptions::kIconButtonSize;
      rail_icon_->Draw(icon_x, icon_y);
      icon_x += SimulationGridOptions::kIconButtonSize;
      event_icon_->Draw(icon_x, icon_y);

      if (clicked_train) {
        trains_element_open_ = true;
        rails_element_open_ = false;
        events_element_open_ = false;
        element_menu_opened_ = true;
      } else if (clicked_rail) {
        trains_element_open_ = false;
        rails_element_open_ = true;
        events_element_open_ = false;
        element_menu_opened_ = true;
      } else if (clicked_event) {
        trains_element_open_ = false;
        rails_element_open_ = false;
        events_element_open_ = true;
        element_menu_opened_ = true;
      }
    } else {
      if (GuiButton(SimulationGridOptions::kOpenMenuArea, GuiIconText(ICON_ARROW_LEFT_FILL, "")))
        menu_opened_ = true;
    }
    if (trains_element_open_) {
      DrawTrainElements();
    } else if (rails_element_open_) {
      DrawRailElements();
    } else if (events_element_open_) {
      DrawEventsElement();
    }
    ; // TODO
  } else {
    DrawText("No Simulation Available", 350, 280, 20, BLACK);
  }
}

void SimulationGrid::DrawTrainElements() {
  if (!manager_)
    throw std::runtime_error("Simulations Manager must be present to draw elements");
  DrawElementsBG("Trains");
  float x = elements_menu_rec_.x;
  float y = elements_menu_rec_.y;
  y += 30;
  x += 10;

  float width = elements_menu_rec_.width - 20.0f;
  Simulation& sim = manager_->GetSimulation(sim_state_.GetCurrentSimulation());
  for (const std::shared_ptr<TrainSimulationState> &ts : sim.GetSimulationState(sim_state_.GetProgress())) {
    GuiLine((Rectangle){x, y, width, 10.0f}, ts->GetName().c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure: " + ts->GetDeparture()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Arrival: " + ts->GetArrival()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure time: " + ts->GetHour()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Current Position: " + ts->GetCurrentPositionName()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Speed: " + std::to_string(ts->GetSpeed())).c_str());
    y += 15.0f; // Spacing
    DrawTrain(ts);
    // GuiLabel((Rectangle)(x, y, width, 10), train.GetTrain().GetName());
  }
  // for(const auto &train : sim.GetTrains()) {
  //   const Train & t = train->GetTrain();
  //   GuiLine((Rectangle){x, y, width, 10.0f}, t.GetName().c_str());
  //   y += 10.0f;
  //   GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure: " + t.GetDeparture()).c_str());
  //   y += 10.0f;
  //   GuiLabel((Rectangle){x, y, width, 10.0f}, ("Arrival: " + t.GetArrival()).c_str());
  //   y += 10.0f;
  //   GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure time: " + Parser::ConvertToTimeString(t.GetHour())).c_str());
  //   y += 10.0f;
  //   GuiLabel((Rectangle){x, y, width, 10.0f}, ("Time: " + std::to_string(sim_state_.GetProgress())).c_str());
  //   y += 15.0f; // Spacing
  //   // GuiLabel((Rectangle)(x, y, width, 10), train.GetTrain().GetName());
  // }

}

void SimulationGrid::DrawTrain(const std::shared_ptr<TrainSimulationState>& train) {
  if (train->HasArrivedToNode()) {
    // Draw Train in the node
    const Node & node = train->GetCurrentNode()->GetNode();
    Vector2 node_pos = GetAbsoluteCoordinates(node.GetPosition());
    DrawPoly(node_pos, 6, 6, 0.0f, RED);
  } else {
    const RailSimulation* rail = train->GetCurrentRail();
    const Node * node1 = rail_sys_.GetNode(train->GetPrevNodeName());
    const Node * node2 = rail_sys_.GetNode(train->GetNextNodeName());

    const Vector2 A = GetAbsoluteCoordinates(node1->GetPosition());
    const Vector2 B = GetAbsoluteCoordinates(node2->GetPosition());
    float percent = train->GetPosition() / rail->GetRail().GetDistance();
    if (percent > 1)
      percent = 1;

    Vector2 node_pos;
    node_pos.x = A.x + percent * (B.x - A.x);
    node_pos.y = A.y + percent * (B.y - A.y);
    DrawPoly(node_pos, 6, 6, 0.0f, ORANGE);

    // Draw Train in the Rail
  }
}
void SimulationGrid::DrawRailElements() {
  if (!manager_)
    throw std::runtime_error("Simulations Manager must be present to draw elements");
  DrawElementsBG("Rails");
}
void SimulationGrid::DrawEventsElement() {
  if (!manager_)
    throw std::runtime_error("Simulations Manager must be present to draw elements");
  DrawElementsBG("Events");
}

void SimulationGrid::DrawElementsBG(const std::string& title) {
  Rectangle content = {0, 0, 10, 10};
  Rectangle view;
  GuiScrollPanel(elements_menu_rec_, title.c_str(), content, &elements_menu_scroll_, &view);
  if (GuiButton((Rectangle){elements_menu_rec_.x + elements_menu_rec_.width - 22, elements_menu_rec_.y + 2, 20, 20}, GuiIconText(ICON_CROSS, ""))) {
    element_menu_opened_ = false;
    trains_element_open_ = false;
    rails_element_open_ = false;
    events_element_open_ = false;
  }
}

void SimulationGrid::SetSimulationsManager(const SimulationsManager* manager) {
  manager_ = manager;
}