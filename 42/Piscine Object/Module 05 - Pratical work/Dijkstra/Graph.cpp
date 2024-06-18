//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef DEBUG
# define DEBUG 0
#endif

#include "Graph.hpp"

Graph::~Graph() {
  for (const auto& pair : nodes_)
    delete pair.second;
}

void Graph::AddNode(uint id) {
  if (nodes_.find(id) == nodes_.end())
    nodes_[id] = new Node(id);
}

void Graph::AddEdge(uint from, uint to, uint weight) {
  // if (from == to && weight != 0)
  //   throw std::runtime_error("Cannot add an edge with the same node")
  AddNode(from);
  AddNode(to);
  nodes_[from]->neighbors[to] = weight;
  nodes_[to]->neighbors[from] = weight;  // bidirectional
}

  std::unordered_map<uint, Node*>& Graph::GetNodes() {
    return nodes_;
  }

PathInfo Graph::Dijkstra(uint src, uint dest) {
  std::unordered_map<uint, uint> dist;    // Edges
  std::unordered_map<uint, uint> parent;  // Node source, to next
  std::set<std::pair<uint, uint>> pq;     // {distance, node_id}

  // Set the distance to infinite at first for each eadge
  for (const auto& pair : nodes_)
    dist[pair.first] = std::numeric_limits<uint>::max();

  if (dist.contains(src)) {
    dist[src] = 0;
    pq.insert({0, src});

    while (!pq.empty()) {
      uint current = pq.begin()->second;
      pq.erase(pq.begin());

      if (current == dest) {
        PathInfo path_info;
        for (uint at = dest; at != src; at = parent[at])
          path_info.path.push_front({at, dist[at]});
        path_info.path.push_front({src, dist[src]});
        return path_info;
      }

      for (const auto& neighbor : nodes_[current]->neighbors) {
        uint neighbor_id = neighbor.first;
        uint weight = neighbor.second;
        uint new_dist = dist[current] + weight;

        if (new_dist < dist[neighbor_id]) {
          pq.erase({dist[neighbor_id], neighbor_id});
          dist[neighbor_id] = new_dist;
          parent[neighbor_id] = current;
          pq.insert({new_dist, neighbor_id});
        }
      }
    }
  }
  return PathInfo();  // return empty PathInfo if no path found
}
