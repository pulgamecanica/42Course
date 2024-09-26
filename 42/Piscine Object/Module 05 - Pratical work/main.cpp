//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "cpp_on_rails.inc"
#include "SimulationsEngine.hpp"
#include "Parser.hpp"
#include "Settings.hpp"

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#undef RAYGUI_IMPLEMENTATION            // Avoid including raygui implementation again
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"


void init_RayGui() {
  SetTraceLogLevel(LOG_ERROR);
  InitWindow(WIDTH, HEIGHT, "Cpp On Rails");
}

int main(int argc, char *argv[]) {
  SimulationsEngine* engine;
  srand(time(NULL)); // Seed randomnes entropy

  init_RayGui();

  Settings::Instance().InitializeAnimations();

  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Settings");

  Settings::Instance().DrawLoadingScreen(0.0f, "Loading Programm Options");
  std::unordered_map<std::string, std::string> options = Parser::ParseProgramOptions(argc, argv);
  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Programm Options");

  Settings::Instance().DrawLoadingScreen(0.0f, "Loading Simulations Engine");
  try {
    engine = new SimulationsEngine(options["elements_file"], options["schedule_directory"]);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return (1);
  }
  Settings::Instance().DrawLoadingScreen(1.0f, "Loading Simulations Engine");

  engine->Run();
  Settings::Instance().SaveRailwayNodePositions(engine->GetRailwaySystem());
  delete engine;
  return (0);
}