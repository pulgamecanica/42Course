#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <iostream>
#include <string>

#include "CppOnRails.inc"

class ErrorHandler {
 public:
  static void ReportError(const std::string& file, int line, int column, const std::string& message, const std::string& line_content);
};

#endif  // ERROR_HANDLER_HPP
