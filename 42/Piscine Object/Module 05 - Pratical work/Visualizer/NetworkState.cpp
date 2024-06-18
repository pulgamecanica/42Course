#include "NetworkState.hpp"
#include "Game.hpp"
#include "raylib.h"
#include "raygui.h"
#include <cmath>

NetworkState::NetworkState(Game* game)
  : game_(game), graph_(nullptr), selected_node_(nullptr), is_dragging_(false),
    show_grid_(false), grid_offset_({0, 0}), node_size_(20.0f), grid_zoom_(1.0f),
    info_panel_({GetScreenWidth() - 251, 31, 250, GetScreenHeight() - 31}),
    info_panel_content({0, 0, 0, 0}),
    info_panel_scroll_({0, 0}) {}
    
void NetworkState::SetGraph(Graph* graph) {
  graph_ = graph;
  SyncWithGraph();
}

// To get the absolute position in the window (grid_pos + offset * zoom = window_pos)
// To get the position in the grid (grid_pos = window_pos / zoom - offset)
Vector2 NetworkState::GetRelativeGridPosition(Vector2 position) {
  return (Vector2){(int)(position.x / grid_zoom_ - grid_offset_.x), (int)(position.y / grid_zoom_ - grid_offset_.y)};
}

void NetworkState::Update() {
  if (!graph_)
    throw std::runtime_error("Graph is not availabe for Network");
  SyncWithGraph();
  UpdateVisualNodes();

  Vector2 mousePosition = GetMousePosition();
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    bool node_selected = false;
    for (auto& [id, visual_node] : visual_nodes_) {
      if (visual_node.IsMouseOver(mousePosition, grid_offset_, grid_zoom_)) {
        selected_node_ = &visual_node;
        is_dragging_node_ = true;
        node_selected = true;
        break;
      }
    }
    if (!node_selected) {
      drag_start_pos_ = mousePosition;
      is_dragging_ = true;
      is_dragging_node_ = false;
    }
  }

  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
    is_dragging_ = false;
    is_dragging_node_ = false;
  }
  if (is_dragging_node_ && selected_node_) {
      Vector2 new_position = GetRelativeGridPosition(mousePosition);
      selected_node_->UpdatePosition(new_position);
  } else if (CheckCollisionPointRec(mousePosition, (Rectangle){0, 0, GetScreenWidth(), 30}) ||
            (selected_node_ != nullptr && CheckCollisionPointRec(mousePosition, info_panel_))) {
    return ; // If mouse is dragging in navbar, do not move grid or if pannel is on and dragging is inside panel
  } else if (is_dragging_) {
    Vector2 drag_offset = {(GetMousePosition().x - drag_start_pos_.x) * (1 / grid_zoom_), (GetMousePosition().y - drag_start_pos_.y) * (1 / grid_zoom_)};
    grid_offset_ = {grid_offset_.x + drag_offset.x, grid_offset_.y + drag_offset.y};
    drag_start_pos_ = GetMousePosition();
  }
}

void NetworkState::Draw() {
  if (!graph_)
    throw std::runtime_error("Graph is not availabe for Network");
  if (show_grid_) {
    DrawGrid();
  }
  DrawEdges();
  for (auto& [id, visual_node] : visual_nodes_) {
    visual_node.Draw(grid_offset_, grid_zoom_);
  }
  if (selected_node_) {
    DrawNodeInfo();
  }
  // Navbar
  DrawNavbar();
}

void NetworkState::DrawNavbar() {
  GuiDummyRec((Rectangle){0, 0, GetScreenWidth(), 30}, "");
  if (GuiButton((Rectangle){10, 5, 80, 20}, "Menu")) {
    game_->ChangeState(&game_->menu_state_);
  }
  GuiCheckBox((Rectangle){100, 5, 20, 20}, "Show Grid", &show_grid_);
  GuiSlider((Rectangle){225, 5, 100, 20}, "Zoom -", "Zoom +", &grid_zoom_, 0.15f, 4.0f);
  GuiSlider((Rectangle){450, 5, 100, 20}, "Node Size -", "Node Size +", &node_size_, 1.0f, 25.0f);
  GuiCheckBox((Rectangle){650, 5, 20, 20}, "Reload", &show_grid_);
}

void NetworkState::SyncWithGraph() {
  auto& nodes = graph_->GetNodes();

  for (auto& [id, node] : nodes) {
    if (visual_nodes_.find(id) == visual_nodes_.end()) {
      Vector2 position = { static_cast<float>(GetRandomValue(0, GetScreenWidth())), static_cast<float>(GetRandomValue(0, GetScreenHeight())) };
      float size = 20.0f;
      Color color = BLUE;
      visual_nodes_.emplace(std::piecewise_construct,
                            std::forward_as_tuple(id),
                            std::forward_as_tuple(node, position, size, color));
    }
  }

  std::vector<uint> nodes_to_remove;
  for (auto& [id, visual_node] : visual_nodes_) {
    if (nodes.find(id) == nodes.end()) {
      nodes_to_remove.push_back(id);
    }
  }

  for (uint id : nodes_to_remove) {
    visual_nodes_.erase(id);
  }
}

void NetworkState::UpdateVisualNodes() {
  Vector2 mousePosition = GetMousePosition();
  for (auto& [id, visual_node] : visual_nodes_) {
    visual_node.SetSize(node_size_ * grid_zoom_); // General Node Size
    if (&visual_node == selected_node_) {
      visual_node.SetState(NodeState::SELECTED);
    } else if (visual_node.IsMouseOver(mousePosition, grid_offset_, grid_zoom_)) {
      visual_node.SetState(NodeState::HOVERED);
      visual_node.SetSize((node_size_ * grid_zoom_) + 1.0f);
    } else {
      visual_node.SetState(NodeState::NORMAL);
    }
  }
}

void NetworkState::DrawEdges() {
  auto& nodes = graph_->GetNodes();
  for (auto& [id, visual_node] : visual_nodes_) {
    Node* node = visual_node.GetNode();
    for (auto& [neighbor_id, weight] : node->neighbors) {
      if (id < neighbor_id) {
        if (visual_nodes_.find(neighbor_id) != visual_nodes_.end()) {
          Vector2 start = visual_node.GetAbsolutePosition(grid_offset_, grid_zoom_);
          Vector2 end = visual_nodes_.at(neighbor_id).GetAbsolutePosition(grid_offset_, grid_zoom_);
          DrawLineV(start, end, BLACK);
          Vector2 midpoint = {(start.x + end.x) / 2, (start.y + end.y) / 2};
          DrawText(TextFormat("%u", weight), midpoint.x, midpoint.y, 10, DARKGRAY);
        }
      }
    }
  }
}

void NetworkState::DrawGrid() {
  // If in a 1:1 grid I can put 50 grids how many can I put in a 1:1
  // Given the number of grids I can put this factor is the one I will increment
  int spacing = 42.0f * grid_zoom_;
  Color gridColor = LIGHTGRAY;
  for (int x = 0; x < GetScreenWidth(); ++x) {
    Vector2 grid_pos = GetRelativeGridPosition((Vector2){x, 0});
    if ((int)grid_pos.x % spacing == 0) {
      for (; x < GetScreenWidth(); x += spacing) {
        DrawLine(x, 30, x, GetScreenHeight(), gridColor);   
      }
      break;
    }
  }
  for (int y = 0; y < GetScreenHeight(); ++y) {
    Vector2 grid_pos = GetRelativeGridPosition((Vector2){0, y});
    if ((int)grid_pos.y % spacing == 0) {
      for (; y < GetScreenHeight(); y += spacing) {
        DrawLine(0, y, GetScreenWidth(), y, gridColor);   
      }
      break;
    }
  }
}

void NetworkState::DrawNodeInfo() {
  // Calculate the info_panel size
  if (GuiWindowBox(info_panel_, "Node Info")) {
    selected_node_ = nullptr;
    return;
  }
  int padding = 5;
  Node* node = selected_node_->GetNode();
  Rectangle rect = (Rectangle){info_panel_.x + padding, info_panel_.y + 30, info_panel_.width, 10};
  // Info
  GuiLabel(rect, TextFormat("Node ID: %u", node->id));
  rect.y += 60;
  GuiLabel(rect, TextFormat("\
    Position: (%.2f, %.2f)\n\
    Abs Pos: (%.2f, %.2f)\n\
    Mouse Pos: (%.2f, %.2f)\n\
    ", selected_node_->GetPosition().x, selected_node_->GetPosition().y,
    selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).x, selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).y,
    GetMousePosition().x, GetMousePosition().y));
  rect.y += 60;
  for (const auto& [neighbor_id, weight] : node->neighbors) {
    GuiLabel(rect, TextFormat("To %u (weight %u)", neighbor_id, weight));
    rect.y += 20;
  }
  info_panel_content = (Rectangle){0, 30 + 10 + 60, 0, rect.y + rect.height - 60};
  GuiScrollPanel(info_panel_, "Settings", info_panel_content, &info_panel_scroll_, NULL);
  {
    if (GuiWindowBox(info_panel_, "Node Info")) {
      selected_node_ = nullptr;
      return;
    }
    rect = (Rectangle){info_panel_.x + padding, info_panel_.y + 30 + (info_panel_scroll_.y), info_panel_.width, 10};
    if (rect.y >= info_panel_.y + 30)
      GuiLabel(rect, TextFormat("Node ID: %u", node->id));
    rect.y += 60;
    if (info_panel_.y + 30 + 10 < rect.y)
      GuiLabel(rect, TextFormat("\
        Position: (%.2f, %.2f)\n\
        Abs Pos: (%.2f, %.2f)\n\
        Mouse Pos: (%.2f, %.2f)\n\
        ", selected_node_->GetPosition().x, selected_node_->GetPosition().y,
        selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).x, selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).y,
        GetMousePosition().x, GetMousePosition().y));
    rect.y += 60;
    for (const auto& [neighbor_id, weight] : node->neighbors) {
      if (info_panel_.y + 30 + 10 < rect.y)
        GuiLabel(rect, TextFormat("To %u (weight %u)", neighbor_id, weight));
      rect.y += 20;
    }
  }
}
