/**
 * 
 * PROBABLY THE GRAPH SHOULD BE A SINGLETON
 * ... WHY NOT... AFTER ALL.... HMMM.....
 * 
 * */


#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "INode.hpp"
#include <unordered_map>
#include <list>
#include <set>
#include <limits>

class PathInfo {
public:
  std::list<std::pair<unsigned, unsigned>> path;  // {node, distance from source}

  int TotalDistance() const {
    if (path.empty())
      return -1;
    return path.back().second;
  }
};

class Graph {
public:
  ~Graph();
  void AddNode(INode* node);
  void AddEdge(INode* from_node, INode* to_node, unsigned weight);
  std::unordered_map<unsigned, INode*>& GetNodes();
  PathInfo Dijkstra(unsigned src, unsigned dest);

private:
  std::unordered_map<unsigned, INode*> nodes_; // uint corresponds to the Node id
};

#endif // __GRAPH_HPP__
