//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "SimulationGrid.hpp"
#include "Settings.hpp"
#include "SimulationState.hpp"
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

SimulationGrid::SimulationGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea), 
    manager_(nullptr),
    elements_menu_rec_(displayArea.x + 5, display_area_.y + 5, SimulationGridOptions::kElementsMenuWidth, display_area_.height),
    menu_opened_(true),
    element_menu_opened_(false),
    trains_element_open_(false),
    rails_element_open_(false),
    events_element_open_(false),
    current_simulation_(0) {

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
  float width = elements_menu_rec_.width - 10.0f;
  Simulation& sim = manager_->GetSimulation(current_simulation_);
  for(const auto &train : sim.GetTrains()) {
    const Train & t = train->GetTrain();
    GuiLabel((Rectangle){x, y, width, 10.0f}, t.GetName().c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure: " + t.GetDeparture()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Arrival: " + t.GetArrival()).c_str());
    y += 10.0f;
    GuiLabel((Rectangle){x, y, width, 10.0f}, ("Departure time: " + t.GetDeparture()).c_str());
    y += 10.0f;
    // GuiLabel((Rectangle)(x, y, width, 10), train.GetTrain().GetName());

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

void SimulationGrid::SetCurrentSimulation(int sim) {
  current_simulation_ = sim;
}


void SimulationGrid::SetSimulationsManager(const SimulationsManager* manager) {
  manager_ = manager;
}