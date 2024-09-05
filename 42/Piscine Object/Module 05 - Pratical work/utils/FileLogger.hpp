#ifndef FILELOGGER_HPP
# define FILELOGGER_HPP

#include "ILogger.hpp"

#include <iostream>
#include <fstream>

class FileLogger: public ILogger {
public:
  explicit FileLogger(const std::string& filename);
  FileLogger(const FileLogger& logger);
  ~FileLogger();
  
  void write(const std::string& message) override;
  std::string GetFileName() const;
private:
  void FileSetup();
  const std::string filename_;
  std::ofstream file_;
};
#endif // FILELOGGER_HPP