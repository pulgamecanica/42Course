//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "cpp_on_rails.inc"
#include "SimulationsEngine.hpp"
#include "Parser.hpp"
#include "Settings.hpp"

#define RAYGUI_IMPLEMENTATION
#include "raylib.h"
#include "raygui.h"

void init_RayGui() {
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(WIDTH, HEIGHT, "Cpp On Rails");
}

int main(int argc, char *argv[]) {
  srand(time(NULL)); // Seed randomnes entropy

  init_RayGui();

  Settings::Instance().InitializeAnimations();

  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Settings");

  Settings::Instance().DrawLoadingScreen(0.0f, "Loading Programm Options");
  std::unordered_map<std::string, std::string> options = Parser::ParseProgramOptions(argc, argv);
  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Programm Options");

  Settings::Instance().DrawLoadingScreen(0.0f, "Loading Simulations Engine");
  SimulationsEngine engine = SimulationsEngine(options["elements_file"], options["schedule_directory"]);
  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Simulations Engine");

  {
    // Example of manual request for simulation
    std::vector<SimulationsManager *> sims;
    SimulationsManager * sm = engine.GenerateSimulations(*engine.GetRailwaySystem().GetSchedules().begin()->second, 3);
    sims.push_back(sm);
  }
  engine.Run();
  return (0);
}