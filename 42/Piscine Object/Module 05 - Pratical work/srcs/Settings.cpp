#include "Settings.hpp"

std::mutex Settings::mutex_;
Settings* Settings::instance_ = nullptr;

Settings& Settings::Instance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance_ == nullptr) {
        instance_ = new Settings();
    }
    return *instance_;
}

void Settings::SetDataFileName(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex_);
    data_file_name_ = filename;
}

void Settings::SetScheduleDirectory(const std::string& directory) {
    std::lock_guard<std::mutex> lock(mutex_);
    schedule_directory_ = directory;
}

void Settings::SetOutputDirectory(const std::string& directory) {
    std::lock_guard<std::mutex> lock(mutex_);
    output_directory_ = directory;
}

const std::string& Settings::GetDataFileName() const {
    return data_file_name_;
}

const std::string& Settings::GetScheduleDirectory() const {
    return schedule_directory_;
}

const std::string& Settings::GetOutputDirectory() const {
    return output_directory_;
}
