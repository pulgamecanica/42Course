#include "ErrorHandler.hpp"

void ErrorHandler::ReportError(const std::string& file, int line, int column, const std::string& message, const std::string& line_content) {
  if (line != 0 && column != 0) {
    std::cerr << C_RED << "Error in file " << file << " at line " << line << ", column " << column << ": " << message << ENDC << std::endl;
    std::cerr << C_YELLOW << line_content << ENDC << std::endl;
  }
  if (column > 0)
    std::cerr << std::string(column - 1, ' ') << C_GREEN << "^" << ENDC << std::endl;
  throw std::runtime_error("Parsing Error");
}

void ErrorHandler::PrintUsage() {
  std::cout << "Usage: ./program_name [options] | default `-e data.txt -d Schedules`\n"
            << "Options:\n"
            << "  -h, --help        Show this help message\n"
            << "  -e, --elements    Specify the elements file\n"
            << "  -d, --directory   Specify the directory containing schedule files\n";
}