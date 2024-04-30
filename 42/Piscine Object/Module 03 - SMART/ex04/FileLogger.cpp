//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "FileLogger.hpp"

FileLogger::FileLogger(const std::string& filename, IHeader *header) : header(header) {
    file.open(filename.c_str(), std::ios::app);
}

FileLogger::~FileLogger() {
    file.close();
}

void FileLogger::write(const std::string& message) {
    if (header)
        file << header->header() << " " << message << std::endl;
    else
        file << message << std::endl;
}