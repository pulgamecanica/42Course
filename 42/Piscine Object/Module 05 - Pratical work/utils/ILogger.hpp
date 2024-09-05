#ifndef ILOGGER_HPP
# define ILOGGER_HPP

#include <iostream>

class ILogger {
public:
  virtual void write(const std::string& message) = 0;
};
#endif // ILOGGER_HPP