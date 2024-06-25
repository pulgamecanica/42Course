#include "Graph.hpp"

Graph::~Graph() {
  ;
}

void Graph::AddNode(INode* node) {
  unsigned id = node->GetID();
  if (nodes_.find(id) == nodes_.end())
    nodes_[id] = node;
}

void Graph::AddEdge(INode* from_node, INode* to_node, unsigned weight) {
  AddNode(from_node);
  AddNode(to_node);
  nodes_[from_node->GetID()]->SetNeighbor(to_node->GetID(), weight);
  nodes_[to_node->GetID()]->SetNeighbor(from_node->GetID(), weight);  // bidirectional
  // nodes_[from_node->GetID()]->GetNeighbors()[to_node->GetID()] = weight;
  // nodes_[to_node->GetID()]->GetNeighbors()[from_node->GetID()] = weight;  // bidirectional
}

std::unordered_map<unsigned, INode*>& Graph::GetNodes() {
  return nodes_;
}

PathInfo Graph::Dijkstra(unsigned src, unsigned dest) {
  std::unordered_map<unsigned, unsigned> dist;    // Edges
  std::unordered_map<unsigned, unsigned> parent;  // Node source, to next
  std::set<std::pair<unsigned, unsigned>> pq;     // {distance, node_id}

  // Set the distance to infinite at first for each edge
  for (const auto& pair : nodes_)
    dist[pair.first] = std::numeric_limits<unsigned>::max();

  if (dist.contains(src)) {
    dist[src] = 0;
    pq.insert({0, src});

    while (!pq.empty()) {
      unsigned current = pq.begin()->second;
      pq.erase(pq.begin());

      if (current == dest) {
        PathInfo path_info;
        for (unsigned at = dest; at != src; at = parent[at])
          path_info.path.push_front({at, dist[at]});
        path_info.path.push_front({src, dist[src]});
        return path_info;
      }

      for (const auto& neighbor : nodes_[current]->GetNeighbors()) {
        unsigned neighbor_id = neighbor.first;
        unsigned weight = neighbor.second;
        unsigned new_dist = dist[current] + weight;

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
