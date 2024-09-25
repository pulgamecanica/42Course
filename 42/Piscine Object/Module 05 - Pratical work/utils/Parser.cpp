#include "Parser.hpp"

#include "Schedule.hpp"
#include "Event.hpp"
#include "Node.hpp"
#include "Rail.hpp"
#include "Settings.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
// #include <regex>
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
    line_number++;
    if (line.empty())
      continue ;
    std::istringstream iss(line);
    std::string type;
    int column_number = 0;

    if (!(iss >> type)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read element type", line);
      continue;
    }
    column_number += type.length() + 1;

    if (type == "Node") {
      std::string node_name;
      if (!(iss >> node_name)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read node name", line);
        continue;
      }
      system.AddNode(node_name);
    } else if (type == "Rail") {
      std::string node1;
      std::string node2;
      double distance;
      if (!(iss >> node1 >> node2 >> distance)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read rail data", line);
        continue;
      }
      if (distance < 0) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Distance cannot be negative", line);
        continue;
      }
      if (distance > 1000000) { //1000 KM is the maximum amout for a rail, let's be real
        ErrorHandler::ReportError(filename, line_number, column_number, "Distance must be < 1,000,000", line);
        continue;
      }
      system.AddRail(node1, node2, distance);
    } else if (type == "Event") {
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
      column_number = line.find(event_name) + event_name.length() + 3;
      std::string time_factor;
      std::string location;
      double probability;
      double duration;
      if (!(iss >> probability >> duration >> time_factor >> location)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read event data", line);
        continue;
      }
      // Probability error
      if (probability < 0 || probability > 1) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Probability must be between 0 and 1", line);
        continue;
      }
      column_number += std::to_string(probability).length();
      // Normalize the duration in minutes
      if (time_factor == "s") {
        duration = duration;
      } else if (time_factor == "m") {
        duration = duration * 60;
      } else if (time_factor == "h") {
        duration = duration * 60 * 60;
      } else if (time_factor == "d") {
        duration = duration * 60 * 60 * 24;
      } else if (time_factor == "y") {
        duration = duration * 60 * 60 * 24 * 365;
      } else {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read time format", line);
      }
      system.AddEvent(event_name, probability, duration, location);
    } else {
      ErrorHandler::ReportError(filename, line_number, column_number, "Unknown element type: " + type, line);
    }
  }
  ParseNodePositions(system);
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

  std::unique_ptr<Schedule> schedule = std::make_unique<Schedule>(name);

  std::string line;
  int line_number = 0;
  while (std::getline(file, line)) {
    if (line.empty())
        continue;
    line_number++;
    std::istringstream iss(line);

    std::string name, departure, arrival, hour;
    double maxAcceleration, maxBrakeForce; 

    int column_number = 0;

    if (!(iss >> name)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read train name", line);
      continue;
    }
    column_number += name.length() + 1;

    if (!(iss >> maxAcceleration)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read max acceleration", line);
      continue;
    }
    column_number += std::to_string(maxAcceleration).length() + 1;

    if (!(iss >> maxBrakeForce)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read max brake force", line);
      continue;
    }
    column_number += std::to_string(maxBrakeForce).length() + 1;

    if (!(iss >> departure)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read departure", line);
      continue;
    }
    column_number += departure.length() + 1;

    if (!(iss >> arrival)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read arrival", line);
      continue;
    }
    column_number += arrival.length() + 1;

    if (!(iss >> hour)) {
      ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read hour", line);
      continue;
    }
    column_number += hour.length() + 1;

    if (maxAcceleration < 0) {
      ErrorHandler::ReportError(filename, line_number, column_number, "MaxAcceleration must be non-negative", line);
      continue;
    }

    if (maxBrakeForce < 0) {
      ErrorHandler::ReportError(filename, line_number, column_number, "MaxBrakeForce must be non-negative", line);
      continue;
    }

    // std::regex time_pattern(R"(^\d{2}h\d{2}$)");
    // if (!std::regex_match(hour, time_pattern)) {
    //   ErrorHandler::ReportError(filename, line_number, column_number, "Invalid time format", line);
    //   continue;
    // }
    unsigned hour_s = ConvertToSeconds(hour);
    std::unique_ptr<Train> train = std::make_unique<Train>(name, maxAcceleration, maxBrakeForce, departure, arrival, hour_s);
    schedule->AddTrain(train);
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
        file << "Rail " << rail->GetNode1() << " " << rail->GetNode2() << " " << rail->GetDistance() << std::endl;
    }

    // Write Events
    const auto& events = system.GetEvents();
    for (const auto& event : events) {
        file << "Event \"" << event->GetType() << "\" " << event->GetProbability() << " " << event->GetDuration() << "s";
        file << " " << event->GetLocation() << std::endl;
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

unsigned int Parser::ConvertToSeconds(const std::string& timeStr) {
  // Find the position of 'h'
  size_t hPos = timeStr.find('h');
  
  // Validate the position of 'h'
  if (hPos == std::string::npos || hPos == 0 || hPos == timeStr.length() - 1) {
    throw std::invalid_argument("Invalid time format. Expected format: NhN where N is a number between 0 and 23.");
  }

  // Extract hour and minute substrings
  std::string hourStr = timeStr.substr(0, hPos);
  std::string minuteStr = timeStr.substr(hPos + 1);

  // Validate if the hour and minute parts are digits
  if (!std::all_of(hourStr.begin(), hourStr.end(), ::isdigit) || 
      !std::all_of(minuteStr.begin(), minuteStr.end(), ::isdigit)) {
    throw std::invalid_argument("Invalid time format. Hour and minute must be numeric.");
  }

  int hour = std::stoi(hourStr);
  int minute = std::stoi(minuteStr);

  // Validate the hour and minute ranges
  if (hour < 0 || hour > 23) {
    throw std::out_of_range("Hour value out of range. Must be between 0 and 23.");
  }
  if (minute < 0 || minute > 59) {
    throw std::out_of_range("Minute value out of range. Must be between 0 and 59.");
  }

  // Calculate the total seconds since midnight
  unsigned int totalSeconds = hour * 3600 + minute * 60;
  
  return totalSeconds;
}

void Parser::ParseNodePositions(RailwaySystem& system) {
  std::ifstream file(Settings::Instance().GetNodePositionsFileName());
  if (!file.is_open()) {
    ErrorHandler::ReportError(Settings::Instance().GetNodePositionsFileName(), 0, 0, "Failed to open file", "");
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream ss(line);
    std::string node_name;
    float x, y;

    // Parse the line for node name, x and y coordinates
    if (!(ss >> node_name >> x >> y)) {
      ErrorHandler::ReportError(Settings::Instance().GetNodePositionsFileName(), file.tellg(), 0, "Malformed line", line);
      continue;  // Skip this line and continue reading
    }
    try {
      system.GetNode(node_name)->SetPosition({x, y});
    } catch (std::exception &e) {(void)e;}
  }
  file.close();
}

std::string Parser::ConvertToTimeStringHHMMSS(unsigned int seconds) {
  std::stringstream ss;

  int s = seconds;

  int h = s / 3600;
  s -= h * 3600;
  int m = s / 60;
  s -= m * 60;

  ss << std::setfill('0') << std::setw(2) << h << ':' << std::setw(2) << m
            << ':' << std::setw(2) << s;
  return (ss.str());
}

std::string Parser::ConvertToTimeString(unsigned int totalSeconds) {
  // Calculate the hour and minute
  unsigned int hour = totalSeconds / 3600;
  unsigned int minute = (totalSeconds % 3600) / 60;

  // // Validate the ranges (should be within 0-23 hours and 0-59 minutes)
  // if (hour > 23 || minute > 59) {
  //   throw std::out_of_range("Invalid total seconds for conversion to time string.");
  // }

  // Convert the hour and minute to the "NhN" format
  std::ostringstream timeStr;
  timeStr << hour << 'h' << minute;

  return timeStr.str();
}

// Function to get the current time as a string in the format <hour><minute><second>_<day-month-year>
std::string Parser::ParseCurrentTimeString() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_time = *std::localtime(&now_time);

  std::stringstream time_string;
  time_string << std::setw(2) << std::setfill('0') << local_time.tm_hour
              << std::setw(2) << std::setfill('0') << local_time.tm_min << "_"
              << std::setw(2) << std::setfill('0') << local_time.tm_mday << "-"
              << std::setw(2) << std::setfill('0') << (local_time.tm_mon + 1) << "-"
              << local_time.tm_year + 1900;

  return time_string.str();
}