#include <iostream>
#include "Simulation/Parser.hpp"
#include "Simulation/RailwaySystem.hpp"
#include "SimulationsEngine.hpp"

#define RAYGUI_IMPLEMENTATION // Not needed when using the shared library
#include "raylib.h"
#include "raygui.h"
#include <cmath>
#include <time.h>

#define WIDTH 900
#define HEIGHT 600

void init_RayGui() {
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(WIDTH, HEIGHT, "Cpp On Rails");
}

int main(int argc, char* argv[]) {
  srand(time(NULL));
  init_RayGui();
  std::unordered_map<std::string, std::string> options = Parser::ParseProgramOptions(argc, argv);
  // Perhaps the simulations engine should own the system...?
  RailwaySystem system(options["elements_file"], options["schedule_directory"]);
  SimulationsEngine engine(system);
  engine.Run();

  return 0;
}
