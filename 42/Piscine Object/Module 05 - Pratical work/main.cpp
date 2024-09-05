//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "cpp_on_rails.inc"
#include "SimulationsEngine.hpp"
#include "Parser.hpp"

int main(int argc, char *argv[]) {
  srand(time(NULL)); // Seed randomnes entropy

  std::unordered_map<std::string, std::string> options = Parser::ParseProgramOptions(argc, argv);

  std::vector<SimulationsManager *> sims;
  SimulationsEngine engine = SimulationsEngine(options["elements_file"], options["schedule_directory"]);

  // Example of manual request for simulation
  SimulationsManager * sm = engine.GenerateSimulations(*engine.GetRailwaySystem().GetSchedules().begin()->second, 100);
  sims.push_back(sm);
  engine.Run();
  return (0);
}
