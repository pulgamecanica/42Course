#ifndef FILELOGGER_HPP
# define FILELOGGER_HPP

#include <iostream>
#include <fstream>

#include "ILogger.hpp"

class FileLogger: public ILogger {
public:
  FileLogger(const std::string& filename);
  ~FileLogger();
  void write(const std::string& message);
private:
  std::ofstream file_;
};
#endif // FILELOGGER_HPP