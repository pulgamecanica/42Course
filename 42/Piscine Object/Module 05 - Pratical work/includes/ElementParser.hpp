#ifndef ELEMENT_PARSER_HPP
#define ELEMENT_PARSER_HPP

#include "RailwaySystem.hpp"
#include "ErrorHandler.hpp"

class ElementParser {
 public:
  void ParseElementFile(const std::string& filename, RailwaySystem& system);
};

#endif  // ELEMENT_PARSER_HPP
