//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __GRAPH_HPP__
# define __GRAPH_HPP__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <set>
#include <list>

typedef uint uint_32;

class Node {
 public:
  explicit Node(uint id) : id(id) {}

  uint id;
  std::unordered_map<uint, uint> neighbors;  // neighbor_id -> weight
};

class PathInfo {
 public:
  std::list<std::pair<uint, uint>> path;  // {node, distance from source}

  int TotalDistance() const {
    if (path.empty())
      return -1;
    return path.back().second;
  }
};

class Graph {
public:
  ~Graph();
  void AddNode(uint id);
  void AddEdge(uint node_a, uint node_b, uint weight);
  std::unordered_map<uint, Node*>& GetNodes();
  PathInfo Dijkstra(uint src, uint dest);
private:
  std::unordered_map<uint, Node*> nodes_; // Only reason to have the map is for fast element acces, the uint corresponds to the Node id
};

#endif
