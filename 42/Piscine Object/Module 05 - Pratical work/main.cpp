#include <iostream>
#include "RailwaySystem.hpp"
#include "ElementParser.hpp"
#include "ScheduleParser.hpp"

void PrintUsage() {
  std::cout << "Usage: ./program_name [options]\n"
            << "Options:\n"
            << "  -h, --help        Show this help message\n"
            << "  -e, --elements    Specify the elements file\n"
            << "  -d, --directory   Specify the directory containing schedule files\n";
}

int main(int argc, char* argv[]) {
  std::string elements_file;
  std::string schedule_directory;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      PrintUsage();
      return 0;
    } else if (arg == "-e" || arg == "--elements") {
      if (i + 1 < argc) {
        elements_file = argv[++i];
      } else {
        ErrorHandler::ReportError("", 0, 0, "No elements file specified", "");
        PrintUsage();
        return 1;
      }
    } else if (arg == "-d" || arg == "--directory") {
      if (i + 1 < argc) {
        schedule_directory = argv[++i];
      } else {
        ErrorHandler::ReportError("", 0, 0, "No schedule directory specified", "");
        PrintUsage();
        return 1;
      }
    } else {
      ErrorHandler::ReportError("", 0, 0, "Unknown option: " + arg, "");
      PrintUsage();
      return 1;
    }
  }

  if (elements_file.empty()) {
    ErrorHandler::ReportError("", 0, 0, "Elements file is required", "");
    PrintUsage();
    return 1;
  }

  if (schedule_directory.empty()) {
    ErrorHandler::ReportError("", 0, 0, "Schedule directory is required", "");
    PrintUsage();
    return 1;
  }

  RailwaySystem system;
  ElementParser element_parser;
  ScheduleParser schedule_parser;

  element_parser.ParseElementFile(elements_file, system);
  schedule_parser.ParseScheduleFiles(schedule_directory, system);

  system.PrintElementsData();
  system.PrintSimulationsData();

  return 0;
}
