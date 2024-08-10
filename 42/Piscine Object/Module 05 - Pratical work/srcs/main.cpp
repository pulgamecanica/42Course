#include <iostream>
#include "Simulation/Parser.hpp"
#include "Simulation/RailwaySystem.hpp"
#include "SimulationsEngine.hpp"

#define RAYGUI_IMPLEMENTATION // Not needed when using the shared library
#include "raylib.h"
#include "raygui.h"
#include <cmath>
#include <time.h>

#define WIDTH 1200
#define HEIGHT 700

void init_RayGui() {
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(WIDTH, HEIGHT, "Cpp On Rails");
}

int main(int argc, char* argv[]) {
  srand(time(NULL)); // Seed randomnes entropy
  init_RayGui();
  std::unordered_map<std::string, std::string> options = Parser::ParseProgramOptions(argc, argv);
  // Perhaps the simulations engine should own the system...? Yes I'll do that later but try avoiding the god's object issue
  RailwaySystem system(options["elements_file"], options["schedule_directory"]);
  SimulationsEngine engine(system);
  engine.Run();

  return 0;
}
