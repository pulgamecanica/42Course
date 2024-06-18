//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Visualizer.inc"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION // Not needed when using the shared library
#include "raygui.h"

#include <iostream>

#define WIDTH 900
#define HEIGHT 600

#include "Graph.hpp"
#include "Game.hpp"

int main() {
    const int screenWidth = WIDTH;
    const int screenHeight = HEIGHT;

  Graph g;

  std::vector<int> nodes;
  while(nodes.size() < 50) {
    uint id = rand() % 500;
    nodes.push_back(id);
  }
  std::set<std::pair<uint, uint>> edgeSet;
  while (edgeSet.size() < 75) {
    uint from = nodes[rand() % 50];
    uint to = nodes[rand() % 50];
    if (from != to) {
        edgeSet.emplace(std::min(from, to), std::max(from, to));
    }
  }

  for (const auto& edge : edgeSet) {
    uint weight = rand() % 100 + 1; // Random weight between 1 and 100
    g.AddEdge(edge.first, edge.second, weight);
  }

  InitWindow(screenWidth, screenHeight, "Cpp On Rails");
  // SetTargetFPS(60);

  Game game(&g);
  game.Run();

  CloseWindow();
  return 0;
}
