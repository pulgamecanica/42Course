#ifndef PARSER_HPP
#define PARSER_HPP

#include "RailwaySystem.hpp"
#include "ErrorHandler.hpp"
#include <filesystem>
#include <unordered_map>

class Parser {
 public:
  Parser() = delete;
  static void ParseScheduleFiles(const std::string& directory, RailwaySystem& system);
  static void ParseElementsFile(const std::string& filename, RailwaySystem& system);
  static void ParseNodePositions(RailwaySystem& system);
  static void WriteDataToFile(const RailwaySystem& system);
  static std::unordered_map<std::string, std::string> ParseProgramOptions(int argc, char *argv[]);
  static std::string ParseCurrentTimeString();
  static void ParseTrainFile(const std::string& filename, const std::string& name, RailwaySystem& system);
  static unsigned int ConvertToSeconds(const std::string& timeStr);
  static std::string ConvertToTimeString(unsigned int totalSeconds);
  static std::string ConvertToTimeStringHHMMSS(unsigned int seconds);
};

#endif  // PARSER_HPP