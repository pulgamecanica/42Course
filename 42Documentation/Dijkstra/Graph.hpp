#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include "INode.hpp"
#include <unordered_map>
#include <list>
#include <set>
#include <limits>

template<typename KeyType>
class PathInfo {
public:
  std::list<std::pair<INode<KeyType>&, unsigned>> path;  // {node, distance from source}

  int TotalDistance() const {
    if (path.empty())
      return -1;
    return path.back().second;
  }
};

template<typename KeyType>
class Graph {
public:
  ~Graph();
  void AddNode(INode<KeyType>* node);
  void AddEdge(INode<KeyType>* from_node, INode<KeyType>* to_node, unsigned weight);
  std::unordered_map<KeyType, INode<KeyType>*>& GetNodes();
  PathInfo<KeyType> Dijkstra(KeyType src, KeyType dest);

private:
  std::unordered_map<KeyType, INode<KeyType>*> nodes_;
};

template<typename KeyType>
Graph<KeyType>::~Graph() {
  // Clean up dynamically allocated nodes if necessary
}

template<typename KeyType>
void Graph<KeyType>::AddNode(INode<KeyType>* node) {
  KeyType key = node->GetKey();
  if (nodes_.find(key) == nodes_.end())
    nodes_[key] = node;
}

template<typename KeyType>
void Graph<KeyType>::AddEdge(INode<KeyType>* from_node, INode<KeyType>* to_node, unsigned weight) {
  AddNode(from_node);
  AddNode(to_node);
  nodes_[from_node->GetKey()]->SetNeighbor(to_node->GetKey(), weight);
  nodes_[to_node->GetKey()]->SetNeighbor(from_node->GetKey(), weight);  // bidirectional
}

template<typename KeyType>
std::unordered_map<KeyType, INode<KeyType>*>& Graph<KeyType>::GetNodes() {
  return nodes_;
}

template<typename KeyType>
PathInfo<KeyType> Graph<KeyType>::Dijkstra(KeyType src, KeyType dest) {
  std::unordered_map<KeyType, unsigned> dist;    // Edges
  std::unordered_map<KeyType, KeyType> parent;  // Node source, to next
  std::set<std::pair<unsigned, KeyType>> pq;     // {distance, node_key}

  // Set the distance to infinite at first for each edge
  for (const auto& pair : nodes_)
    dist[pair.first] = std::numeric_limits<unsigned>::max();

  if (dist.count(src) > 0) {
    dist[src] = 0;
    pq.insert({0, src});

    while (!pq.empty()) {
      KeyType current = pq.begin()->second;
      pq.erase(pq.begin());

      if (current == dest) {
        PathInfo<KeyType> path_info;
        for (KeyType at = dest; at != src; at = parent[at])
          path_info.path.push_front({*nodes_[at], dist[at]});
        path_info.path.push_front({*nodes_[src], dist[src]});

        return path_info;
      }

      for (const auto& neighbor : nodes_[current]->GetNeighbors()) {
        KeyType neighbor_key = neighbor.first;
        unsigned weight = neighbor.second;
        unsigned new_dist = dist[current] + weight;

        if (new_dist < dist[neighbor_key]) {
          pq.erase({dist[neighbor_key], neighbor_key});
          dist[neighbor_key] = new_dist;
          parent[neighbor_key] = current;
          pq.insert({new_dist, neighbor_key});
        }
      }
    }
  }
  return PathInfo<KeyType>();  // return empty PathInfo if no path found
}

#endif // __GRAPH_HPP__
