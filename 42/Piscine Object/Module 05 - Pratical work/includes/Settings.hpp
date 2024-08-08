#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <mutex>

class Settings {
public:
    static Settings& Instance();

    // Prevent copy construction and assignment
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    // Setters
    void SetDataFileName(const std::string& filename);
    void SetScheduleDirectory(const std::string& directory);
    void SetOutputDirectory(const std::string& directory);

    // Getters
    const std::string& GetDataFileName() const;
    const std::string& GetScheduleDirectory() const;
    const std::string& GetOutputDirectory() const;

private:
    Settings() = default;  // Private constructor
    ~Settings() = default;

    static std::mutex mutex_;
    static Settings* instance_;

    std::string data_file_name_;
    std::string schedule_directory_;
    std::string output_directory_;
};

#endif // SETTINGS_HPP
