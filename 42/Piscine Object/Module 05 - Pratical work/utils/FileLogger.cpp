#include "FileLogger.hpp"
#include <filesystem>

FileLogger::FileLogger(const std::string& filename): filename_(filename) {
  FileSetup();
}

FileLogger::FileLogger(const FileLogger& logger): filename_(logger.filename_) {
  FileSetup();
}

void FileLogger::FileSetup() {
  std::filesystem::path path(filename_);
  // Ensure the directory exists
  std::filesystem::path dir_path = path.parent_path();
  if (!std::filesystem::exists(dir_path))
    if (!std::filesystem::create_directories(dir_path))
      throw std::runtime_error(std::string("Failed to create directory: ") + std::string(dir_path));  
  file_.open(filename_.c_str(), std::ios::trunc);
  if (!file_)
    throw std::runtime_error(std::string("Failed to create or open file: ") + filename_);
}

FileLogger::~FileLogger() {
  file_.close();
}

std::string FileLogger::GetFileName() const {
  return filename_;
}

void FileLogger::write(const std::string& message) {
  file_ << message << std::endl;
}