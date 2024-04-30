//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __STERAMLOGGER_HPP__
# define __STERAMLOGGER_HPP__

#include <iostream>

#include "ILogger.hpp"
#include "IHeader.hpp"

class StreamLogger : public ILogger {
public:
    StreamLogger(std::ostream& stream, IHeader *header = NULL);
    void write(const std::string& message);
private:
    IHeader *header;
    std::ostream& stream;
};
#endif
