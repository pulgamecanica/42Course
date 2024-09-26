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
  const std::map<std::string, int> kOptionsSmall = {
    {"width", 20}, 
    {"height", 20}
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
    events_element_open_(false),
    elements_menu_scroll_({0, 0}),
    selected_train_(nullptr) {

  Settings::Instance().DrawLoadingScreen(1 / 5,"Loading Simulation Grid", "Train Icon");
  train_icon_ = std::make_unique<Animation>(SimulationGridOptions::kTrainIconImages, 1.0f, SimulationGridOptions::kOptions);

  Settings::Instance().DrawLoadingScreen(2 / 5,"Loading Simulation Grid", "Rail Icon");
  rail_icon_ = std::make_unique<Animation>(SimulationGridOptions::kRailIconImages, 1.0f, SimulationGridOptions::kOptions);

  Settings::Instance().DrawLoadingScreen(3 / 5,"Loading Simulation Grid", "Event Icon");
  event_icon_ = std::make_unique<Animation>(SimulationGridOptions::kEventIconImages, 1.0f, SimulationGridOptions::kOptions);

  Settings::Instance().DrawLoadingScreen(4 / 5,"Loading Simulation Grid", "Event Icon");
  event_icon_small_ = std::make_unique<Animation>(SimulationGridOptions::kEventIconImages, 1.0f, SimulationGridOptions::kOptionsSmall);

  SimulationGridOptions::kMenuBGArea.y += displayArea.y;
  SimulationGridOptions::kOpenMenuArea.y += displayArea.y;
  SimulationGridOptions::kCloseMenuArea.y += displayArea.y;
  Settings::Instance().DrawLoadingScreen(4 / 5,"Loading Simulation Grid");
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
    DrawMenu();
    DrawTrains();
    DrawEvents();
  } else {
    DrawText("No Simulation Available", 350, 280, 20, BLACK);
  }
}

void SimulationGrid::DrawMenu() {
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

    if (clicked_train || clicked_rail || clicked_event) {
      elements_menu_scroll_ = (Vector2){0, 0};
      element_menu_opened_ = true;
      trains_element_open_ = clicked_train;
      rails_element_open_ = clicked_rail;
      events_element_open_ = clicked_event;
    }
  } else {
    if (GuiButton(SimulationGridOptions::kOpenMenuArea, GuiIconText(ICON_ARROW_LEFT_FILL, "")))
      menu_opened_ = true;
  }
}

void SimulationGrid::DrawTrains() {
  if (!manager_)
    throw std::runtime_error("Simulations Manager must be present to draw elements");
  
  Simulation& sim = manager_->GetSimulation(sim_state_.GetCurrentSimulation());
  for (const std::shared_ptr<TrainSimulationState> &ts : sim.GetSimulationTrainsState(sim_state_.GetProgress()))
    DrawTrain(ts);

  DrawTrainsElements();
}

void SimulationGrid::DrawTrainsElements() {
  if (!trains_element_open_) return;
  
  DrawElementsBG("Trains");
  BeginScissorMode(elements_menu_rec_.x, elements_menu_rec_.y + 25, elements_menu_rec_.width, elements_menu_rec_.height);
  float x = elements_menu_rec_.x + elements_menu_scroll_.x;
  float y = elements_menu_rec_.y + elements_menu_scroll_.y;
  y += 30;
  x += 10;

  float width = elements_menu_rec_.width - 25.0f;
  Simulation& sim = manager_->GetSimulation(sim_state_.GetCurrentSimulation());
  for (const std::shared_ptr<TrainSimulationState> &ts : sim.GetSimulationTrainsState(sim_state_.GetProgress())) {
    Rectangle train_area = {x, y, width, 70};
    if (GuiLabelButton(train_area, "")) {
      if (selected_train_ == const_cast<TrainSimulation*>(ts->GetTrainSimulation()))
        selected_train_ = nullptr;
      else
        selected_train_ = const_cast<TrainSimulation*>(ts->GetTrainSimulation());
    }
    if (selected_train_ == const_cast<TrainSimulation*>(ts->GetTrainSimulation()) ){
      DrawRectangleRec(train_area, BEIGE);
    }
    if (CheckCollisionPointRec(GetMousePosition(), train_area)) {
      DrawRectangleRec(train_area, (Color){42, 42, 42, 20});
    }
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
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Train Average Time: " + 
      Parser::ConvertToTimeStringHHMMSS(
        manager_->GetTrainAverageTime(
          &(ts->GetTrainSimulation()->GetTrain())
        )
      )
    ).c_str());
    //Spacing
    y += 10.0f;
  }
  y += 1000;
  elements_rec_ = (Rectangle){0, 0, x - elements_menu_rec_.x, y};
  EndScissorMode();
}

void SimulationGrid::DrawTrain(const std::shared_ptr<TrainSimulationState>& train) {
  if (train->HasArrivedToNode()) {
    // Draw Train in the node
    const Node & node = train->GetCurrentNode()->GetNode();
    Vector2 node_pos = GetAbsoluteCoordinates(node.GetPosition());
    DrawPoly(node_pos, 6, 6, 0.0f, RED);
    if (selected_train_ == const_cast<TrainSimulation*>(train->GetTrainSimulation()))
      DrawRing(node_pos, 6, 8, 0, 360, 100, LIME);
    // DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color); // Draw ring
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
    if (selected_train_ == const_cast<TrainSimulation*>(train->GetTrainSimulation()))
      DrawRing(node_pos, 6, 8, 0, 360, 100, LIME);
  }
}

void SimulationGrid::DrawEventsElements() {
  if (!manager_)
    throw std::runtime_error("Simulations Manager must be present to draw elements");
  if (!events_element_open_) return;

  DrawElementsBG("Events");
  BeginScissorMode(elements_menu_rec_.x, elements_menu_rec_.y + 25, elements_menu_rec_.width, elements_menu_rec_.height);
  Simulation& sim = manager_->GetSimulation(sim_state_.GetCurrentSimulation());
  float x = elements_menu_rec_.x + elements_menu_scroll_.x;
  float y = elements_menu_rec_.y + elements_menu_scroll_.y;
  y += 30;
  x += 10;
  float width = elements_menu_rec_.width - 25.0f;

  for (const std::shared_ptr<EventSimulationState> &event_state : sim.GetSimulationEventsState(sim_state_.GetProgress())) {
    GuiLine((Rectangle){x, y, width, 10.0f}, (event_state->GetType() + " at " + event_state->GetLocationName()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, (event_state->IsFinished() ? "Status: Finished!" : "Status: On Progress..."));
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, (std::string("Start: ") + event_state->GetStartString()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, (std::string("Duration: ") + event_state->GetDurationString() + " (" + std::to_string(event_state->GetDuration()) + "s)").c_str());
    y += 20.0f;
  }
  y += 1000.0f;
  elements_rec_ = (Rectangle){0, 0, x - elements_menu_rec_.x, y};
  EndScissorMode();
}

void SimulationGrid::DrawEvents() {
  Simulation& sim = manager_->GetSimulation(sim_state_.GetCurrentSimulation());

  for (const std::shared_ptr<EventSimulationState> &event_state : sim.GetSimulationEventsState(sim_state_.GetProgress())) {
    if (!event_state->IsFinished()) {
      Node * node = event_state->GetEventOccurrence().GetEvent().GetNode();
      const Vector2 node_pos = GetAbsoluteCoordinates(node->GetPosition());
      event_icon_small_->Draw(node_pos.x, node_pos.y);
    }
  }
  DrawEventsElements();
}

void SimulationGrid::DrawElementsBG(const std::string& title) {
  GuiScrollPanel(elements_menu_rec_, title.c_str(), elements_rec_, &elements_menu_scroll_, &view_);
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