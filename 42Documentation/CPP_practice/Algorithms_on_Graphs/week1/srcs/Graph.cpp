#include "Graph.hpp"

Vertix::Vertix(int id) : id_(id), visited_(false), pre_visit_(0), post_visit_(0) {}

int Vertix::GetId() const {
  return id_;
}

bool Vertix::IsVisited() const {
  return visited_;
}

void Vertix::SetVisited(bool visited) {
  visited_ = visited;
}

int Vertix::GetPreVisit() const {
  return pre_visit_;
}

void Vertix::SetPreVisit(int pre_visit) {
  pre_visit_ = pre_visit;
}

int Vertix::GetPostVisit() const {
  return post_visit_;
}

void Vertix::SetPostVisit(int post_visit) {
  post_visit_ = post_visit;
}

const std::unordered_set<int>& Vertix::GetNeighbors() const {
  return neighbors_;
}

void Vertix::AddNeighbor(int neighbor_id) {
  neighbors_.insert(neighbor_id);
}

Graph::Graph(int nodes) : clock_(1), nodes_(nodes) {
  vertices_.reserve(nodes);
  for (int i = 1; i <= nodes; ++i) {
    vertices_.emplace_back(i);
  }
}

bool Graph::AddEdge(int from, int to) {
  if (from <= 0 || from > (int)vertices_.size() || to <= 0 || to > (int)vertices_.size()) {
    return false;
  }
  vertices_[from - 1].AddNeighbor(to);
  vertices_[to - 1].AddNeighbor(from); // Adding edge for both vertices
  return true;
}

// void Graph::DFS(int current, std::vector<int>& vec) {
//   vertices_[current - 1].SetVisited(true);
//   vertices_[current - 1].SetPreVisit(clock_++);
//   vec.push_back(current);
//   for (const int neighbor : vertices_[current - 1].GetNeighbors()) {
//     if (!vertices_[neighbor - 1].IsVisited()) {
//       DFS(neighbor, vec);
//     }
//   }
//   vertices_[current - 1].SetPostVisit(clock_++);
// }

// Graph Graph::Transpose() {
//   Graph transposed(vertices_.size());
//   for (size_t i = 0; i < vertices_.size(); ++i) {
//     for (int neighbor : vertices_[i].GetNeighbors()) {
//       transposed.AddEdge(neighbor, vertices_[i].GetId());
//     }
//   }
//   return transposed;
// }

// void Graph::DFSUtil(int current, std::vector<int>& scc) {
//   vertices_[current - 1].SetVisited(true);
//   scc.push_back(current);
//   for (const int neighbor : vertices_[current - 1].GetNeighbors()) {
//     if (!vertices_[neighbor - 1].IsVisited()) {
//       DFSUtil(neighbor, scc);
//     }
//   }
// }

// std::vector<std::vector<int>> Graph::ComputeSCCs() {
//   std::stack<int> stack;
//   std::vector<int> vec;
//   for (size_t i = 0; i < vertices_.size(); ++i) {
//     if (!vertices_[i].IsVisited()) {
//       DFS(vertices_[i].GetId(), vec);
//       stack(vec);
//     }
//   }

//   Graph transposed = Transpose();

//   std::vector<std::stack<int>> sccs;
//   while (!stack.empty()) {
//     int current = stack.top();
//     stack.pop();

//     if (!vertices_[current - 1].IsVisited()) {
//       std::vector<int> scc;
//       transposed.DFS(current, scc);
//       sccs.push_back(scc);
//     }
//   }
//   return sccs;
// }

bool Graph::IsReachable(int start, int end) {
  if (start <= 0 || start > nodes_ || end <= 0 || end > nodes_) {
    return false;
  }
  // Here you can set every node to unvisited if you want to run many simulations...
  return DFSTarget(start, end);
}

bool Graph::DFSTarget(int current, int end) {
  if (current == end) {
    return true;
  }
  vertices_[current - 1].SetVisited(true);
  for (const int neighbor : vertices_[current - 1].GetNeighbors())
    if (!vertices_[neighbor - 1].IsVisited())
      if (DFSTarget(neighbor, end))
        return true;
  return false;
}
