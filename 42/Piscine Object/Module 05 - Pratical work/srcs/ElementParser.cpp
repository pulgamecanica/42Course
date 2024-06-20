#include "ElementParser.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

void ElementParser::ParseElementFile(const std::string& filename, RailwaySystem& system) {
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
      if (!(iss >> node.name)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read node name", line);
        continue;
      }
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
      system.AddRail(rail);
    } else if (type == "Event") {
      Event event;
      if (!(iss >> event.type >> event.probability >> event.duration >> event.location)) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Failed to read event data", line);
        continue;
      }
      if (event.probability < 0 || event.probability > 1) {
        ErrorHandler::ReportError(filename, line_number, column_number, "Probability must be between 0 and 1", line);
        continue;
      }
      system.AddEvent(event);
    } else {
      ErrorHandler::ReportError(filename, line_number, column_number, "Unknown element type: " + type, line);
    }
  }
}
