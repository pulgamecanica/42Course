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

  InitWindow(screenWidth, screenHeight, "Cpp On Rails");
  // SetTargetFPS(60);

  Game game(&g);
  game.Run();

  CloseWindow();
  return 0;
}
