#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <string>

#include "cpp_on_rails.inc"

class ErrorHandler {
 public:
  static void ReportError(const std::string& file, int line, int column, const std::string& message, const std::string& line_content);
  static void PrintUsage();
};

#endif  // ERROR_HANDLER_HPP