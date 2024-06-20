#include "ErrorHandler.hpp"

void ErrorHandler::ReportError(const std::string& file, int line, int column, const std::string& message, const std::string& line_content) {
  std::cerr << RED << "Error in file " << file << " at line " << line << ", column " << column << ": " << message << ENDC << std::endl;
  std::cerr << YELLOW << line_content << ENDC << std::endl;
  if (column > 0)
    std::cerr << std::string(column - 1, ' ') << GREEN << "^" << ENDC << std::endl;  
}
