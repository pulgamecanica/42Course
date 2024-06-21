/**
 * To test:
 * make all
 * g++ -Wall -Werror -Wextra -I. -o test main.cpp  -L. -lDijkstraGraph
 * ./test <src node> <dest node>
 * ./test 1 4
 * ./test 4 6
 **/

#include "Graph.hpp"
#include "INode.hpp"
#include <iostream>

class CustomNode : public INode {
public:
  CustomNode(unsigned id, std::string name) : id(id), name(std::move(name)) {}

  unsigned GetID() const override {
    return id;
  }

  std::unordered_map<unsigned, unsigned>& GetNeighbors() override {
    return neighbors;
  }

  std::string GetName() const {
    return name;
  }

private:
  unsigned id;
  std::string name;
  std::unordered_map<unsigned, unsigned> neighbors;
};


int main(int ac, char *av[])
{
  if (ac != 3)
    return -1;
  Graph g;
  

  INode* node0 = new CustomNode(0, "Node0");
  INode* node1 = new CustomNode(1, "Node1");
  INode* node2 = new CustomNode(2, "Node2");
  INode* node3 = new CustomNode(3, "Node2");
  INode* node4 = new CustomNode(4, "Node2");
  INode* node5 = new CustomNode(5, "Node2");
  INode* node6 = new CustomNode(6, "Node2");
  INode* node7 = new CustomNode(7, "Node2");
  INode* node8 = new CustomNode(8, "Node2");
  INode* node9 = new CustomNode(9, "Node2");
  INode* node10 = new CustomNode(10, "Node2");
  INode* node11 = new CustomNode(11, "Node2");
  INode* node12 = new CustomNode(12, "Node2");

  g.AddEdge(node0, node1, 5);
  g.AddEdge(node0, node2, 10);
  g.AddEdge(node0, node3, 7);
  g.AddEdge(node1, node2, 5);
  g.AddEdge(node1, node3, 10);
  g.AddEdge(node1, node5, 3);
  g.AddEdge(node1, node5, 3);
  g.AddEdge(node1, node6, 4);
  g.AddEdge(node2, node5, 1);
  g.AddEdge(node2, node7, 6);
  g.AddEdge(node3, node4, 4);
  g.AddEdge(node3, node6, 6);
  g.AddEdge(node4, node6, 3);
  g.AddEdge(node5, node6, 3);
  g.AddEdge(node6, node10, 2);
  g.AddEdge(node8, node9, 2);
  g.AddEdge(node12, node10, 9);
  g.AddEdge(node12, node11, 5);
  g.AddEdge(node7, node9, 8);

  for (auto& elem: g.GetNodes()) {
    unsigned node_id = elem.first;
    INode& node = *elem.second;
    std::cout << "Node: " << node_id << " & " << node.GetID() << std::endl;
  }

  int src, dest;
  src = atoi(av[1]);
  dest = atoi(av[2]);
  PathInfo result = g.Dijkstra(src, dest);

  std::cout << "Dijkstra path from " << src << " to " << dest << " : ";
  for (const auto& node : result.path) {
    std::cout << node.first << " (Distance: " << node.second << ") ";
  }
  std::cout << std::endl;
  std::cout << "Total distance: " << result.TotalDistance() << std::endl;

  return (0);
}
