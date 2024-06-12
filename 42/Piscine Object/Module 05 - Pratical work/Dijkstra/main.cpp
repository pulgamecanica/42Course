//**************************//
//*Template by pulgamecanica*//
//***************************//

/**
 * To test:
 * make all
 * g++ -Wall -Werror -Wextra -I. -o test main.cpp  -L. -lDijkstraGraph
 * ./test <src node> <dest node>
 * ./test 1 4
 * ./test 4 6
 **/

#include "Graph.hpp"

int main(int ac, char *av[])
{
  if (ac != 3)
    return -1;
  Graph g;
  
  g.AddEdge(0, 1, 5);
  g.AddEdge(0, 2, 10);
  g.AddEdge(0, 3, 7);
  g.AddEdge(1, 2, 5);
  g.AddEdge(1, 3, 10);
  g.AddEdge(1, 5, 3);
  g.AddEdge(1, 5, 3);
  g.AddEdge(1, 6, 4);
  g.AddEdge(2, 5, 1);
  g.AddEdge(2, 7, 6);
  g.AddEdge(3, 4, 4);
  g.AddEdge(3, 6, 6);
  g.AddEdge(4, 6, 3);
  g.AddEdge(5, 6, 3);
  g.AddEdge(6, 7, 2);

  for (auto& elem: g.GetNodes()) {
    uint node_id = elem.first;
    Node node = *elem.second;
    std::cout << "Node: " << node_id << " & " << node.id << std::endl;
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
