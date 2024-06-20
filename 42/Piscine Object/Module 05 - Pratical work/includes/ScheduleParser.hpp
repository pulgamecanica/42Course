#ifndef SCHEDULE_PARSER_HPP
#define SCHEDULE_PARSER_HPP

#include "RailwaySystem.hpp"
#include "ErrorHandler.hpp"
#include <filesystem>

class ScheduleParser {
 public:
  void ParseScheduleFiles(const std::string& directory, RailwaySystem& system);

 private:
  void ParseTrainFile(const std::string& filename, RailwaySystem& system);
};

#endif  // SCHEDULE_PARSER_HPP
