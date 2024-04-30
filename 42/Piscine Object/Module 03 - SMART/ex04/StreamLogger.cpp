//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StreamLogger.hpp"

StreamLogger::StreamLogger(std::ostream& stream, IHeader *header):
    header(header), stream(stream) {
}

void StreamLogger::write(const std::string& message) {
    if (header)
        stream << header->header() << " " << message << std::endl;
    else
        stream << message << std::endl;
}