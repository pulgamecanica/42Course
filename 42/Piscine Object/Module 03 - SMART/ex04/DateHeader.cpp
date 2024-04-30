//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <ctime>

#include "DateHeader.hpp"

DateHeader::DateHeader() {
    ;
}

DateHeader::~DateHeader() {
    ;
}

const std::string DateHeader::header() {
    std::time_t now = std::time(NULL);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));
    return std::string(buffer);
}