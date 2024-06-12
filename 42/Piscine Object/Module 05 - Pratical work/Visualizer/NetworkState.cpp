#include "NetworkState.hpp"
#include "Game.hpp"
#include "raylib.h"
#include "raygui.h"

NetworkState::NetworkState(Game* game)
  : game_(game), graph_(nullptr), selected_node_(nullptr), is_dragging_(false),
    show_grid_(true), grid_offset_({0, 0}), node_size_(20.0f), grid_zoom_(1.0f),
    info_panel_({GetScreenWidth() - 250, 30, 250, GetScreenHeight() - 30}),
    info_panel_scroll_({0, 0}) {}
    
void NetworkState::SetGraph(Graph* graph) {
  graph_ = graph;
  SyncWithGraph();
}

// To get the absolute position in the window (grid_pos + offset * zoom = window_pos)
// To get the position in the grid (grid_pos = window_pos / zoom - offset)
Vector2 NetworkState::GetRelativeGridPosition(Vector2 position) {
  return (Vector2){position.x / grid_zoom_ - grid_offset_.x, position.y / grid_zoom_ - grid_offset_.y};
}

void NetworkState::Update() {
  if (graph_) {
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
    } else if (CheckCollisionPointRec(mousePosition, (Rectangle){0, 0, GetScreenWidth(), 30})) {
      return ; // If mouse is dragging in navbar, do not move grid
    } else if (is_dragging_) {
      Vector2 drag_offset = {GetMousePosition().x - drag_start_pos_.x, GetMousePosition().y - drag_start_pos_.y};
      grid_offset_ = {grid_offset_.x + drag_offset.x, grid_offset_.y + drag_offset.y};
      drag_start_pos_ = GetMousePosition();
    }
  }
}

void NetworkState::Draw() {
  if (graph_) {
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
  }
  // Navbar
  GuiDummyRec((Rectangle){0, 0, GetScreenWidth(), 30}, "");
  if (GuiButton((Rectangle){10, 5, 80, 20}, "Menu")) {
    game_->ChangeState(&game_->menu_state_);
  }
  GuiCheckBox((Rectangle){100, 5, 20, 20}, "Show Grid", &show_grid_);
  GuiSlider((Rectangle){225, 5, 100, 20}, "Zoom -", "Zoom +", &grid_zoom_, 0.25f, 4.0f);
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
    if (abs((int)grid_pos.x) % spacing == 0) {
      for (; x < GetScreenWidth(); x += spacing) {
        DrawLine(x, 30, x, GetScreenHeight(), gridColor);   
      }
      break;
    }
  }
  for (int y = 0; y < GetScreenHeight(); ++y) {
    Vector2 grid_pos = GetRelativeGridPosition((Vector2){0, y});
    if (abs((int)grid_pos.y) % spacing == 0) {
      for (; y < GetScreenHeight(); y += spacing) {
        DrawLine(0, y, GetScreenWidth(), y, gridColor);   
      }
      break;
    }
  }
}

void NetworkState::DrawNodeInfo() {
  Rectangle infoPanel = {GetScreenWidth() - 250, 30, 250, GetScreenHeight() - 30};
  GuiPanel(infoPanel, "Node Info");
  if (GuiButton((Rectangle){infoPanel.x + infoPanel.width - 70, infoPanel.y + 10, 60, 20}, "Close")) {
    selected_node_ = nullptr;
    return;
  }

  Node* node = selected_node_->GetNode();
  DrawText(TextFormat("Node ID: %u", node->id), infoPanel.x + 10, infoPanel.y + 40, 20, BLACK);
  DrawText(TextFormat("\
    Position: (%.2f, %.2f)\n\
    Abs Pos: (%.2f, %.2f)\n\
    Mouse Pos: (%.2f, %.2f)\n\
    ", selected_node_->GetPosition().x, selected_node_->GetPosition().y,
    selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).x, selected_node_->GetAbsolutePosition(grid_offset_, grid_zoom_).y,
    GetMousePosition().x, GetMousePosition().y), infoPanel.x + 10, infoPanel.y + 70, 10, BLACK);

  DrawText("Edges:", infoPanel.x + 10, infoPanel.y + 120, 20, BLACK);
  int offsetY = 150;
  for (const auto& [neighbor_id, weight] : node->neighbors) {
    DrawText(TextFormat("To %u (weight %u)", neighbor_id, weight), infoPanel.x + 10, infoPanel.y + offsetY, 20, BLACK);
    offsetY += 30;
  }
}
