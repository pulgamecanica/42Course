//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GRAPHVIZ_HPP__
# define __GRAPHVIZ_HPP__

#include "Graph.hpp"

#include <iostream>
#include <mutex>
#include <thread>
#include <set>

extern "C" {
  #include <stdio.h>
  #include <stdlib.h>
  #include <stdbool.h>
  #include <MLX42/MLX42.h>
}

class NodeViz;
class ConnectionViz;
class GraphViz;

class ConnectionViz {
public:
  ConnectionViz(Connection & conn, NodeViz * node);
  ConnectionViz& operator=(const ConnectionViz& other);
  NodeViz &GetNode();
  Connection &GetConnection();
private:
  Connection & conn_; // conn_.Dest() would corresponds to node_.GetNodeNum()
  NodeViz * node_;
};

class NodeViz {
public:
  NodeViz();
  void SetNode(int node_num, std::vector<ConnectionViz>& connections,
                std::vector<int> paths, int x, int y);
  int GetNodeNum() const;
  void DrawNode(mlx_t *mlx, mlx_image_t *image, std::set<int>& drew_nodes);
  void SetX(int x);
  void SetY(int y);
  void SetColor(int color);
  std::vector<ConnectionViz>& GetConnections();
  std::vector<int> GetPaths();
  int x() const;
  int y() const;
private:
  int GetColor() const;
  int node_num_;
  int x_;
  int y_;
  int color_;
  std::vector<ConnectionViz> connections_;
  std::vector<int> paths_;
};
bool operator==(const NodeViz& lhs, int rhs);

class GraphViz {
public:
  void Open();
  void JoinThread();
  static GraphViz *GetInstance();
  void SetWidth(unsigned width);
  void SetHeight(unsigned height);
  void SetGraph(Graph *graph);
  void Update();
private:
  GraphViz();
  void UpdateKeys();
  void UpdateMouse();
  void UpdateSelectedNode();
  void UpdateHoveredNode();
  void ComputeHover();
  void SetUp();
  void SetUpNodes();
  void RunViz();
  void ExitMLX42();
  void DrawBG(mlx_image_t *img, int color);
  void DrawNodes();
  void DrawSideBar();
  void SelectNode();

  // For Graph logic
  Graph *graph_;
  std::vector<NodeViz> nodes_;
  NodeViz *selected_node_;
  NodeViz *hovered_node_;
  
  // For MLX
  mlx_t *mlx_;
  mlx_image_t *image_;
  mlx_image_t *side_bar_image_;
  mlx_image_t *side_bar_node_bg_;
  mlx_image_t *side_bar_stand_by_bg_;

  // Configurations
  unsigned width_; // Default 512
  unsigned height_; // Default 512
  unsigned side_bar_width_; // Default 192
  std::string title_; // Default "Graph Viz"
  bool resize_; // Default true
  int padding_; // Default 5px
  int background_color_;        // Light blue 0xc0d9ebff
  int node_number_color_;       // Black 0x000000ff
  int edge_color_;              // Gray Blue 0x323040ff
  int node_color_;              // Dark Blue 0x043354ff
  int node_selected_color_;     // Light Blue 0x3323e8ff
  int node_hover_color_;        // Lighter Blue 0x9590d4ff
  int node_hover_border_color_; // Dark Gray 0x26262bff
  int weight_color_;            // Red 0xb30000ff


  // For multithreading
  std::thread *helper_;

  // For Singleton
  static std::mutex mutex_;
  static GraphViz *instance_;

  // GUI
  bool dragging_;
  bool select_;
  int32_t x_cur_; // cursor x position
  int32_t y_cur_; // cursor y position
};

#endif
