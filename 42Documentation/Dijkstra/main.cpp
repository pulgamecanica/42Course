/**
 * To test:
 * make all
 *  g++ -Wall -Werror -Wextra -I. -o test main.cpp
 * ./test <src node> <dest node>
 * ./test 1 4
 * ./test 4 6
 **/

#include "Graph.hpp"
#include "INode.hpp"
#include <iostream>

#include <iostream>
#include "Graph.hpp"

// A concrete implementation of the INode interface for integer keys
class IntNode : public INode<int> {
public:
  IntNode(int id) : id_(id) {}
  int GetKey() const override { return id_; }
  void SetNeighbor(int id, int weight) override { neighbors_[id] = weight; }
  const std::unordered_map<int, unsigned>& GetNeighbors() const override { return neighbors_; }

private:
  int id_;
  std::unordered_map<int, unsigned> neighbors_;
};

class StrNode : public INode<std::string> {
public:
  StrNode(std::string name) : name_(name) {}
  std::string GetKey() const override { return name_; }
  void SetNeighbor(std::string name, int weight) override { neighbors_[name] = weight; }
  const std::unordered_map<std::string, unsigned>& GetNeighbors() const override { return neighbors_; }

private:
  std::string name_;
  std::unordered_map<std::string, unsigned> neighbors_;
};

int main() {
  {
  Graph<std::string> graph;
  
  INode<std::string>* jun = new StrNode("Jungapeo");
  INode<std::string>* zit = new StrNode("Zitacuaro");
  INode<std::string>* mor = new StrNode("Morelia");
  INode<std::string>* cdmx = new StrNode("CDMX");
  INode<std::string>* jal = new StrNode("Jalisco");
  INode<std::string>* p_e = new StrNode("Puerto Escondido");
  INode<std::string>* oax = new StrNode("Oaxaca");
  INode<std::string>* zic = new StrNode("Zicatela");
  INode<std::string>* tux = new StrNode("Tuxpan");
  INode<std::string>* tol = new StrNode("Toluca");
  INode<std::string>* lerma = new StrNode("Lerma");
  INode<std::string>* cuau = new StrNode("Cuautla");

  graph.AddEdge(jun, zit, 26.8);
  graph.AddEdge(jun, mor, 150);
  graph.AddEdge(zit, mor, 158);
  graph.AddEdge(zit, cdmx, 142);
  graph.AddEdge(cdmx, mor, 294);
  graph.AddEdge(cdmx, jal, 540);
  graph.AddEdge(jal, p_e, 880);
  graph.AddEdge(p_e, oax, 257);
  graph.AddEdge(oax, zic, 11);
  graph.AddEdge(oax, tux, 245);
  graph.AddEdge(oax, tol, 376);
  graph.AddEdge(tol, lerma, 20);
  graph.AddEdge(tol, cuau, 145);
  graph.AddEdge(tol, cdmx, 65);
  graph.AddEdge(oax, cdmx, 462);

  for (auto& elem: graph.GetNodes()) {
    INode<std::string>& node = *elem.second;
    std::cout << "Node: " << node.GetKey() << std::endl;
  }
  std::cout << std::endl;
  PathInfo<std::string> path_cdmx_p_e = graph.Dijkstra("CDMX", "Puerto Escondido");
  PathInfo<std::string> path_jun_cuau = graph.Dijkstra("Jungapeo", "Cuautla");
  std::cout << "Trip: CDMX <-> Puerto Escondido" << std::endl;
  for (const auto& node : path_cdmx_p_e.path) {
    std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
  }
  std::cout << std::endl << "Total Distance: " << path_cdmx_p_e.TotalDistance() << std::endl;    
  std::cout << std::endl;
  std::cout << "Trip: Jungapeo <-> Cuautla" << std::endl;
  for (const auto& node : path_jun_cuau.path) {
    std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
  }
  std::cout << std::endl << "Total Distance: " << path_jun_cuau.TotalDistance() << std::endl << std::endl;
  }
  {
    Graph<int> graph;
    INode<int>* node1 = new IntNode(1);
    INode<int>* node2 = new IntNode(2);
    INode<int>* node3 = new IntNode(3);
    INode<int>* node4 = new IntNode(4);
    INode<int>* node5 = new IntNode(5);
    
    graph.AddEdge(node1, node2, 2);
    graph.AddEdge(node1, node4, 10);
    graph.AddEdge(node2, node3, 3);
    graph.AddEdge(node2, node4, 8);
    graph.AddEdge(node3, node5, 6);
    graph.AddEdge(node3, node4, 2);

    PathInfo<int> path = graph.Dijkstra(1, 4);
    
    for (const auto& node : path.path) {
      std::cout << node.first.GetKey() << " (Distance: " << node.second << ") ";
    }
    std::cout << std::endl << "Total Distance: " << path.TotalDistance() << std::endl;
    
    // Clean up dynamically allocated nodes
    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete node5;

    return 0;
  }
}
