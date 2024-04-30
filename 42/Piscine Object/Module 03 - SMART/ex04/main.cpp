//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <vector>

#include "ex04.inc"

#include "ILogger.hpp"
#include "FileLogger.hpp"
#include "StreamLogger.hpp"
#include "ConstHeader.hpp"
#include "DateHeader.hpp"


int main() {
    std::cout << YELLOW << "- - - - - - Without headers - - - - -" << ENDC << std::endl;
    {
    // Create instances of loggers
    ILogger * fileLogger = new FileLogger("log.txt");
    ILogger * streamLogger = new StreamLogger(std::cout);
    ILogger * errorLogger = new StreamLogger(std::cerr);

    // Vector to hold ILogger pointers
    std::vector<ILogger *> loggers;
    loggers.push_back(fileLogger);
    loggers.push_back(streamLogger);
    loggers.push_back(errorLogger);

    // List of strings to log
    std::vector<std::string> messages;
    messages.push_back("Message 1");
    messages.push_back("Message 2");
    messages.push_back("Message 3");

    // Iterate through messages and log them using all loggers
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
        for (std::vector<ILogger *>::iterator it2 = loggers.begin(); it2 != loggers.end(); ++it2) {            
            (*it2)->write(*it);
        }
    }
    delete fileLogger;
    delete streamLogger;
    delete errorLogger;
    }
    
    std::cout << YELLOW << "- - - - - - With Const headers - - - - -" << ENDC << std::endl;
    {
    // Create instances of loggers
    ConstHeader ch1("This is a const header!");
    ConstHeader ch2("*Pulgamecanica*");
    ConstHeader ch3("|Header.com|");
    ILogger * fileLogger = new FileLogger("log_const_header.txt", &ch1);
    ILogger * streamLogger = new StreamLogger(std::cout, &ch2);
    ILogger * errorLogger = new StreamLogger(std::cerr, &ch3);

    // Vector to hold ILogger pointers
    std::vector<ILogger *> loggers;
    loggers.push_back(fileLogger);
    loggers.push_back(streamLogger);
    loggers.push_back(errorLogger);

    // List of strings to log
    std::vector<std::string> messages;
    messages.push_back("Message 1");
    messages.push_back("Message 2");
    messages.push_back("Message 3");

    // Iterate through messages and log them using all loggers
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
        for (std::vector<ILogger *>::iterator it2 = loggers.begin(); it2 != loggers.end(); ++it2) {            
            (*it2)->write(*it);
        }
    }
    delete fileLogger;
    delete streamLogger;
    delete errorLogger;
    }
    std::cout << YELLOW << "- - - - - - With Date headers - - - - -" << ENDC << std::endl;
    {
    // Create instances of loggers
    DateHeader dh1;
    DateHeader dh2;
    DateHeader dh3;
    ILogger * fileLogger = new FileLogger("log_date_header.txt", &dh1);
    ILogger * streamLogger = new StreamLogger(std::cout, &dh2);
    ILogger * errorLogger = new StreamLogger(std::cerr, &dh3);

    // Vector to hold ILogger pointers
    std::vector<ILogger *> loggers;
    loggers.push_back(fileLogger);
    loggers.push_back(streamLogger);
    loggers.push_back(errorLogger);

    // List of strings to log
    std::vector<std::string> messages;
    messages.push_back("Message 1");
    messages.push_back("Message 2");
    messages.push_back("Message 3");

    // Iterate through messages and log them using all loggers
    for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it) {
        for (std::vector<ILogger *>::iterator it2 = loggers.begin(); it2 != loggers.end(); ++it2) {            
            (*it2)->write(*it);
        }
    }
    delete fileLogger;
    delete streamLogger;
    delete errorLogger;
    }
    return 0;
}