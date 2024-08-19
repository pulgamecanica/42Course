#ifndef PARSER_HPP
#define PARSER_HPP

#include "Simulation/RailwaySystem.hpp"
#include "Simulation/ErrorHandler.hpp"
#include <filesystem>
#include <unordered_map>

class Parser {
 public:
  Parser() = delete;
  static void ParseScheduleFiles(const std::string& directory, RailwaySystem& system);
  static void ParseElementsFile(const std::string& filename, RailwaySystem& system);
  static void WriteDataToFile(const RailwaySystem& system);
  static std::unordered_map<std::string, std::string> ParseProgramOptions(int argc, char *argv[]);
  static std::string ParseCurrentTimeString();
 private:
  static void ParseTrainFile(const std::string& filename, const std::string& name, RailwaySystem& system);
};

#endif  // PARSER_HPP
