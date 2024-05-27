//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <unordered_set>
#include <stack>

class Vertix {
 public:
  explicit Vertix(int id);
  int GetId() const;
  bool IsVisited() const;
  void SetVisited(bool visited);
  int GetPreVisit() const;
  void SetPreVisit(int pre_visit);
  int GetPostVisit() const;
  void SetPostVisit(int post_visit);
  const std::unordered_set<int>& GetNeighbors() const;
  void AddNeighbor(int neighbor_id);

 private:
  int id_;
  bool visited_;
  int pre_visit_;
  int post_visit_;
  std::unordered_set<int> neighbors_;
};

class Graph {
 public:
  Graph(int nodes);
  bool AddEdge(int from, int to);
  // std::vector<std::vector<int>> ComputeSCCs();
  bool IsReachable(int start, int end);

 private:
  // void DFS(int current, std::vector<int>& stack);
  // void DFSUtil(int current, std::vector<int>& scc);
  bool DFSTarget(int current, int end);
  // Graph Transpose();
  std::vector<Vertix> vertices_;
  int clock_; // Clock for pre-visit and post-visit numbers
  int nodes_;
};

#endif  // GRAPH_H_
