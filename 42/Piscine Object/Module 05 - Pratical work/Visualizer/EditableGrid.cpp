#include "EditableGrid.hpp"
#include "Parser.hpp"
#include "Settings.hpp"
#include "cpp_on_rails.inc"

#include <raygui.h>
#include <cstring>
#include <algorithm>

namespace EditableGridOptions {
  constexpr int kButtonPadding = 5;
  constexpr int kButtonWidth = 100;
  constexpr int kButtonHeight = 25;
  const Rectangle kShowGrid = {WIDTH - 125, 42, kButtonWidth, kButtonHeight};
  const Rectangle kShowNodeNames = {kShowGrid.x - kButtonWidth - 10, 42, kButtonWidth, kButtonHeight};
  const Color kNewNodeColor = {250, 128, 114, 255};
  const Rectangle kNewNodeBoxRec = {WIDTH / 2 - 70, HEIGHT / 2 - 80, 140, 160};
}

EditableGrid::EditableGrid(RailwaySystem &rail_sys, float gridSize, Rectangle displayArea)
  : Grid(rail_sys, gridSize, displayArea),
    current_tool_(Tool::MOVE),
    rail_source_node_selected_(""),
    is_dragging_(false),
    show_minimap_(false),
    adding_node_(false),
    minimap_(rail_sys, gridSize / 2, displayArea) {

  button_manager_.AddButton("Show Grid", 
    EditableGridOptions::kShowGrid, 
    [this]() { show_grid_ = !show_grid_; });
  
  button_manager_.AddButton("Show Names", 
    EditableGridOptions::kShowNodeNames, 
    [this]() { Settings::Instance().ToggleShowNodeNames(); });
  

  for (int i = 0; i < 5; ++i) {
    std::string button_text;
    Tool tool_type;
    
    switch(i) {
      case 0: 
        button_text = GuiIconText(ICON_GEAR, "Move");
        tool_type = Tool::MOVE;
        break;
      case 1:
        button_text = GuiIconText(ICON_CROSS, "Remove");
        tool_type = Tool::REMOVE;
        break;
      case 2:
        button_text = GuiIconText(ICON_WAVE, "Add Node");
        tool_type = Tool::ADDNODE;
        break;
      case 3:
        button_text = GuiIconText(ICON_PENCIL, "Add Rail");
        tool_type = Tool::ADDRAIL;
        break;
      case 4:
        button_text = GuiIconText(ICON_GEAR, "Move Map");
        tool_type = Tool::MOVEMAP;
        break;
    }

    button_manager_.AddButton(button_text, 
      {display_area_.x + (EditableGridOptions::kButtonWidth + EditableGridOptions::kButtonPadding) * i, 
      display_area_.y + display_area_.height - EditableGridOptions::kButtonHeight, 
      EditableGridOptions::kButtonWidth, 
      EditableGridOptions::kButtonHeight}, 
      [this, tool_type]() { SetTool(tool_type); });
  }
}


void EditableGrid::Update() {
  if (show_minimap_) {
    ;
    minimap_.Update();
  } else {
    Vector2 mousePos = GetMousePosition();
    const float rad = scale_ * 5;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
      HandleNodeSelection(mousePos, rad);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
      is_dragging_ = false;
    if (is_dragging_ && current_tool_ == Tool::MOVE)
      MoveNode();
    can_drag_grid_ = !is_dragging_;
    if (current_tool_ == Tool::MOVEMAP)
      MoveMap();
    Grid::Update();
    button_manager_.UpdateButtons();
  }
}

void EditableGrid::Draw() {
  if (show_minimap_) {
    minimap_.Draw();
    if (GuiButton((Rectangle){display_area_.x + display_area_.width - 50, display_area_.y, 50, 25}, GuiIconText(ICON_EYE_OFF, "Map")))
      show_minimap_ = false;
  } else {
    Grid::Draw();
    button_manager_.DrawButtons();
    if (GuiButton((Rectangle){display_area_.x + display_area_.width - 50, display_area_.y, 50, 25}, GuiIconText(ICON_EYE_ON, "Map")))
      show_minimap_ = true;
    if (!rail_source_node_selected_.empty()) {
      Node* node = rail_sys_.GetNode(rail_source_node_selected_);
      DrawNode(node, RED);
      Vector2 grid_pos = GetAbsoluteCoordinates(node->GetPosition());
      Vector2 mouse_pos = GetMousePosition();
      ClipLine(grid_pos, mouse_pos);
      DrawLineEx(mouse_pos, grid_pos, (Settings::Instance().GetNodeSize() / 2) * scale_, ORANGE);
    }
    if (adding_node_)
      DrawNewNodeDialog();
  }
}

void EditableGrid::SetTool(Tool tool) {
  current_tool_ = tool;
  MouseCursor cursor = MOUSE_CURSOR_DEFAULT;
  adding_node_ = false;
  rail_source_node_selected_ = "";
  
  switch (current_tool_) {
    case Tool::MOVE:
      cursor = MOUSE_CURSOR_POINTING_HAND;
      break;
    case Tool::REMOVE:
      cursor = MOUSE_CURSOR_CROSSHAIR;
      break;
    case Tool::ADDNODE:
      cursor = MOUSE_CURSOR_IBEAM;
      break;
    case Tool::ADDRAIL:
      cursor = MOUSE_CURSOR_IBEAM;
      break;
    case Tool::MOVEMAP:
      cursor = MOUSE_CURSOR_POINTING_HAND;
      break;
    default:
      break;
  }
  SetMouseCursor(cursor);
}

void EditableGrid::HandleNodeSelection(const Vector2& mousePos, const float rad) {
  for (const auto& [id, node] : rail_sys_.GetNodes()) {
    if (CheckCollisionPointCircle(mousePos, GetAbsoluteCoordinates(node->GetPosition()), rad)) {
      switch (current_tool_) {
        case Tool::MOVE:
          selected_node_ = id;
          drag_start_pos_ = mousePos;
          is_dragging_ = true;
          break;
        case Tool::REMOVE:
          RemoveNode(id);
          break;
        case Tool::ADDRAIL:
          if (rail_source_node_selected_.empty()) {
            rail_source_node_selected_ = id;
          } else {
            AddRail(id);
            rail_source_node_selected_ = "";
          }
          break;
        default:
          break;
      }
      return;
    }
  }
  if (current_tool_ == Tool::ADDNODE) {
    AddNode(mousePos);
  }
}

void EditableGrid::AddNode(Vector2 position) {
  if (adding_node_) return ;
  if (GetMousePosition().y > display_area_.y + display_area_.height - 25) return;
  adding_node_ = true;
  new_node_position_ = GetRelativeCoordinates(position);
}

void EditableGrid::DrawNewNodeDialog() {
  const int max_node_name = 20;
  char text[max_node_name + 1];
  strcpy(text, new_node_name_.c_str());
  DrawCircleV(GetAbsoluteCoordinates(new_node_position_), 5 * scale_, EditableGridOptions::kNewNodeColor);
  int result = !GuiTextInputBox(EditableGridOptions::kNewNodeBoxRec, "New Node", "Node Name", "Press Enter To Save", text, max_node_name, NULL);
  if (result != 0) {
    adding_node_ = false;
  } else if (IsKeyPressed(KEY_ENTER) && !std::string(text).empty()) {
    adding_node_ = false;
    Node node(text);
    rail_sys_.AddNode(text);
    rail_sys_.GetNode(text)->SetPosition(new_node_position_);
    Parser::WriteDataToFile(rail_sys_);
    new_node_name_ = "";
  } else {
    new_node_name_ = text;
    new_node_name_.erase(std::remove(new_node_name_.begin(), new_node_name_.end(), ' '), new_node_name_.end());
  }
}

void EditableGrid::RemoveNode(const std::string& id) {
  std::cout << "Removing node: " << id  << std::endl;
}

void EditableGrid::AddRail(const std::string& id) {
  rail_sys_.AddRail(rail_source_node_selected_, id, 42);
  Parser::WriteDataToFile(rail_sys_);
}

void EditableGrid::MoveNode() {
  if (!selected_node_.empty()) {
    Vector2 current_mouse_pos = GetMousePosition();
    Node* node = rail_sys_.GetNode(selected_node_);
    node->SetPosition(GetRelativeCoordinates(current_mouse_pos));
  }
}


void EditableGrid::MoveMap() {
  can_drag_grid_ = false;
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    drag_start_pos_ = GetMousePosition();
    is_dragging_ = true;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    is_dragging_ = false;
  }
  if (is_dragging_) {
    Vector2 current_mouse_pos = GetMousePosition();
    Vector2 offset = Settings::Instance().GetMapPosition();
    offset.x += (current_mouse_pos.x - drag_start_pos_.x) * 0.2f;
    offset.y += (current_mouse_pos.y - drag_start_pos_.y) * 0.2f;
    Settings::Instance().SetMapPosition(offset);
  }
}