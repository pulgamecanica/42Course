#include "ScheduleParser.hpp"

#include "Simulation.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

void ScheduleParser::ParseScheduleFiles(const std::string& directory, RailwaySystem& system) {
  for (const auto& entry : std::filesystem::directory_iterator(directory)) {
    if (entry.path().extension() == ".schedule") {
      ParseTrainFile(entry.path().string(), system);
    }
  }
}

void ScheduleParser::ParseTrainFile(const std::string& filename, RailwaySystem& system) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    ErrorHandler::ReportError(filename, 0, 0, "Failed to open file", "");
    return;
  }

  Simulation simulation(filename);

  std::string line;
  int line_number = 0;
  while (std::getline(file, line)) {
    if (line.empty())
        continue;
    line_number++;
    std::istringstream iss(line);
    Train train;
    int column_number = 0;

    if (!(iss >> train.name)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read train name", line);
      continue;
    }
    column_number += train.name.length() + 1;

    if (!(iss >> train.max_acceleration)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read max acceleration", line);
      continue;
    }
    column_number += std::to_string(train.max_acceleration).length() + 1;

    if (!(iss >> train.max_brake_force)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read max brake force", line);
      continue;
    }
    column_number += std::to_string(train.max_brake_force).length() + 1;

    if (!(iss >> train.departure)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read departure", line);
      continue;
    }
    column_number += train.departure.length() + 1;

    if (!(iss >> train.arrival)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read arrival", line);
      continue;
    }
    column_number += train.arrival.length() + 1;

    if (!(iss >> train.hour)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read hour", line);
      continue;
    }
    column_number += train.hour.length() + 1;

    if (train.max_acceleration < 0) {
      ErrorHandler::ReportError(filename, line_number, column_number, "MaxAcceleration must be non-negative", line);
      continue;
    }

    if (train.max_brake_force < 0) {
      ErrorHandler::ReportError(filename, line_number, column_number, "MaxBrakeForce must be non-negative", line);
      continue;
    }

    std::regex time_pattern(R"(^\d{2}h\d{2}$)");
    if (!std::regex_match(train.hour, time_pattern)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Invalid time format", line);
      continue;
    }

    simulation.AddTrain(train);
  }

  system.AddSimulation(simulation);
}
