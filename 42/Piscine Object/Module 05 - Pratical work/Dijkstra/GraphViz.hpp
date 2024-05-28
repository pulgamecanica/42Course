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
  void DrawNode(mlx_image_t *image, std::set<int>& drew_nodes);
private:
  int x() const;
  int y() const;
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
  void SetUp();
  void SetUpNodes();
  void RunViz();
  void ExitMLX42();
  void DrawBG();
  void DrawNodes();
  // For Graph logic
  Graph *graph_;
  std::vector<NodeViz> nodes_;
  // list of Nodes
  // each node has it's neighbors in a list
  // Each node has it's path vector
  // There can be a selected node
  
  // For MLX
  mlx_t *mlx_;
  mlx_image_t *image_;

  // Configurations
  unsigned width_; // Default 512
  unsigned height_; // Default 512
  std::string title_; // Default "Graph Viz"
  bool resize_; // Default true

  // For multithreading
  std::thread *helper_;

  // For Singleton
  static std::mutex mutex_;
  static GraphViz *instance_;
};

#endif
