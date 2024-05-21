//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ILOGGER_HPP__
# define __ILOGGER_HPP__

#include <iostream>

class IHeader;

class ILogger {
public:
    virtual ~ILogger() {}
    virtual void write(const std::string& message) = 0;
    virtual void setHeader(IHeader * header) = 0;
};
#endif
