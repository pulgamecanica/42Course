#ifndef NETWORK_STATE_HPP
#define NETWORK_STATE_HPP

#include "IGameState.hpp"
#include "VisualNode.hpp"
#include "Graph.hpp"
#include <unordered_map>
#include <memory>

class Game;

class NetworkState : public IGameState {
public:
  explicit NetworkState(Game* game);
  void SetGraph(Graph* graph); // New method to set the graph
  void Update() override;
  void Draw() override;

private:
  Game* game_;
  Graph* graph_;
  std::unordered_map<uint, VisualNode> visual_nodes_;
  VisualNode* selected_node_; // Pointer to the currently selected node

  // Dragging Status
  bool is_dragging_;          // Flag to check if a node is being dragged
  bool is_dragging_node_;     // If it's dragging a node
  
  // Grid 
  bool show_grid_;            // Flag to show/hide grid
  Vector2 grid_offset_;       // Offset for the grid

  Vector2 drag_start_pos_;    // Position where the drag started
  float node_size_;           // Slider to control node size
  float grid_zoom_;           // Slider to control grid zoom level

  // Info pannel
  Rectangle info_panel_; // Info panel rectangle
  Vector2 info_panel_scroll_; // Scroll offset for the info panel

  void SyncWithGraph();
  void UpdateVisualNodes();
  void DrawEdges();
  void DrawGrid();
  void DrawNodeInfo();
  Vector2 GetRelativeGridPosition(Vector2 position);
};

#endif // NETWORK_STATE_HPP
