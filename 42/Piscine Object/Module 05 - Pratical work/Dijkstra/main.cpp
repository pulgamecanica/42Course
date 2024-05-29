//**************************//
//*Template by pulgamecanica*//
//***************************//

#include "Graph.hpp"
#include "GraphViz.hpp"

#include "Dijkstra.inc"

void openViz() {
  
}

int main(void)
{
  if (DEBUG)
      std::cout << "Debug ON!" << std::endl;
  Graph g(8);
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


  //  Visualizer
  GraphViz * graph_viz = GraphViz::GetInstance();
  
  graph_viz->SetGraph(&g);

  // Opens vizualiser in another detached thread, should not block
  graph_viz->Open();

  // Since thread did not block, must join before main process finishes to avoid orphans
  graph_viz->JoinThread();
  return (0);
}
