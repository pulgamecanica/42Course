#include "Simulation/Parser.hpp"

#include "Simulation/Schedule.hpp"
#include "Simulation/Event.hpp"
#include "Simulation/Node.hpp"
#include "Simulation/Rail.hpp"
#include "Settings.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <sstream>

void Parser::ParseElementsFile(const std::string& filename, RailwaySystem& system) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    ErrorHandler::ReportError(filename, 0, 0, "Failed to open file", "");
    return;
  }

  std::string line;
  int line_number = 0;
  while (std::getline(file, line)) {
    if (line.empty())
      continue ;
    line_number++;
    std::istringstream iss(line);
    std::string type;
    int column_number = 0;

    if (!(iss >> type)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read element type", line);
      continue;
    }
    column_number += type.length() + 1;

    if (type == "Node") {
      Node node;
      std::string node_name;
      if (!(iss >> node_name)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read node name", line);
        continue;
      }
      // Node(node_name);
      node.SetName(node_name);
      system.AddNode(node);
    } else if (type == "Rail") {
      Rail rail;
      if (!(iss >> rail.node1 >> rail.node2 >> rail.distance)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read rail data", line);
        continue;
      }
      if (rail.distance < 0) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Distance cannot be negative", line);
        continue;
      }
      rail.distance *= 1000; // KM -> m
      system.AddRail(rail);
    } else if (type == "Event") {
      Event event;
      std::string event_name;
      // Read the quoted event name
      iss >> std::ws;  // Consume any leading whitespace
      if (iss.peek() == '"') {
        iss.get();  // Consume the opening quote
        std::getline(iss, event_name, '"');
      } else {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read event name", line);
        continue;
      }
      event.type = event_name;
      column_number = line.find(event_name) + event_name.length() + 3;
      std::string time_factor;
      if (!(iss >> event.probability >> event.duration >> time_factor >> event.location)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read event data", line);
        continue;
      }
      // Probability error
      if (event.probability < 0 || event.probability > 1) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Probability must be between 0 and 1", line);
        continue;
      }
      column_number += std::to_string(event.probability).length();
      // Normalize the duration in minutes
      if (time_factor == "m") {
        event.duration = event.duration;
      } else if (time_factor == "h") {
        event.duration = event.duration * 60;
      } else if (time_factor == "d") {
        event.duration = event.duration * 60 * 24;
      } else if (time_factor == "y") {
        event.duration = event.duration * 60 * 24 * 365;
      } else {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read time format", line);
      }
      system.AddEvent(event);
    } else {
      ErrorHandler::ReportError(filename, line_number, column_number, "Unknown element type: " + type, line);
    }
  }
}

void Parser::ParseScheduleFiles(const std::string& directory, RailwaySystem& system) {
  try {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.path().extension() == ".schedule") {
        ParseTrainFile(entry.path().string(), entry.path().stem().string(), system);
      }
    }
  } catch (std::exception & e) {
    std::cerr << C_RED << e.what() << ENDC << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Parser::ParseTrainFile(const std::string& filename, const std::string& name, RailwaySystem& system) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    ErrorHandler::ReportError(filename, 0, 0, "Failed to open file", "");
    return;
  }

  Schedule schedule(name, system.GetGraph());

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

    schedule.AddTrain(train);
  }

  system.AddSchedule(schedule);
}

void Parser::WriteDataToFile(const RailwaySystem& system) {
    const std::string& filename = Settings::Instance().GetDataFileName();
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Write Nodes
    const auto& nodes = system.GetNodes();
    for (const auto& [node_name, node] : nodes) {
        file << "Node " << node_name << std::endl;
    }

    // Write Rails
    const auto& rails = system.GetRails();
    for (const auto& rail : rails) {
        file << "Rail " << rail.node1 << " " << rail.node2 << " " << rail.distance << std::endl;
    }

    // Write Events
    const auto& events = system.GetEvents();
    for (const auto& event : events) {
        file << "Event \"" << event.type << "\" " << event.probability << " " << event.duration;
        if (event.duration >= 60 * 24 * 365) {
            file << "y";
        } else if (event.duration >= 60 * 24) {
            file << "d";
        } else if (event.duration >= 60) {
            file << "h";
        } else {
            file << "m";
        }
        file << " " << event.location << std::endl;
    }

    file.close();
}

std::unordered_map<std::string, std::string> Parser::ParseProgramOptions(int argc, char *argv[]) {
  std::unordered_map<std::string, std::string> options;

  // Defaults
  options["elements_file"] = "data.txt";
  options["schedule_directory"] = "Schedules";

  // Handle Options
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      ErrorHandler::PrintUsage();
      exit(EXIT_FAILURE);
    } else if (arg == "-e" || arg == "--elements") {
      if (i + 1 < argc) {
        options["elements_file"] = argv[++i];
      } else {
        ErrorHandler::ReportError("", 0, 0, "No elements file specified", "");
        ErrorHandler::PrintUsage();
        exit(EXIT_FAILURE);
      }
    } else if (arg == "-d" || arg == "--directory") {
      if (i + 1 < argc) {
        options["schedule_directory"] = argv[++i];
      } else {
        ErrorHandler::ReportError("", 0, 0, "No schedule directory specified", "");
        ErrorHandler::PrintUsage();
        exit(EXIT_FAILURE);
      }
    } else {
      ErrorHandler::ReportError("", 0, 0, "Unknown option: " + arg, "");
      ErrorHandler::PrintUsage();
      exit(EXIT_FAILURE);
    }
  }
  // Error handler
  if (options["elements_file"].empty()) {
    ErrorHandler::ReportError("", 0, 0, "Elements file is required", "");
    ErrorHandler::PrintUsage();
    exit(EXIT_FAILURE);
  }
  if (options["schedule_directory"].empty()) {
    ErrorHandler::ReportError("", 0, 0, "Schedule directory is required", "");
    ErrorHandler::PrintUsage();
    exit(EXIT_FAILURE);
  }
  {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    // Format date and time
    std::ostringstream oss;
    oss << "SimulationsLog_" << std::put_time(&tm, "%d%m%y");

    Settings::Instance().SetScheduleDirectory(options["schedule_directory"]);
    Settings::Instance().SetDataFileName(options["elements_file"]);
    Settings::Instance().SetOutputDirectory(oss.str());
  }
  return options;
}

// Function to get the current time as a string in the format <hour><minute><second>_<day-month-year>
std::string Parser::ParseCurrentTimeString() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_time = *std::localtime(&now_time);

  std::stringstream time_string;
  time_string << std::setw(2) << std::setfill('0') << local_time.tm_hour
              << std::setw(2) << std::setfill('0') << local_time.tm_min
              << std::setw(2) << std::setfill('0') << local_time.tm_sec << "_"
              << std::setw(2) << std::setfill('0') << local_time.tm_mday << "-"
              << std::setw(2) << std::setfill('0') << (local_time.tm_mon + 1) << "-"
              << local_time.tm_year + 1900;

  return time_string.str();
}