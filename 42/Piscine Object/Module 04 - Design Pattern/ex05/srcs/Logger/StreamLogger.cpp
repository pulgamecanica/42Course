//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StreamLogger.hpp"

StreamLogger::StreamLogger(std::ostream& stream, IHeader *header)
    : header(header), stream(stream) {
}

StreamLogger::~StreamLogger() {}

void StreamLogger::setHeader(IHeader * header) {
    this->header = header;
}

void StreamLogger::write(const std::string& message) {
    if (header)
        stream << header->header() << " " << message << std::endl;
    else
        stream << message << std::endl;
}