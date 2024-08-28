#ifndef FILELOGGER_HPP
# define FILELOGGER_HPP

#include <iostream>
#include <fstream>

#include "ILogger.hpp"

class FileLogger: public ILogger {
public:
  FileLogger(const std::string& filename);
  FileLogger(const FileLogger& logger);
  ~FileLogger();
  std::string GetFileName() const;
  void write(const std::string& message);
private:
  void FileSetup();
  const std::string filename_;
  std::ofstream file_;
};
#endif // FILELOGGER_HPP