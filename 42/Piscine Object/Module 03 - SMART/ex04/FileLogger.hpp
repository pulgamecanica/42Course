//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FILELOGGER_HPP__
# define __FILELOGGER_HPP__

#include <iostream>
#include <fstream>

#include "ILogger.hpp"
#include "IHeader.hpp"

class FileLogger: public ILogger {
public:
    FileLogger(const std::string& filename, IHeader *header = NULL);
    ~FileLogger();
    void write(const std::string& message);
private:
    IHeader *header;
    std::ofstream file;
};
#endif
