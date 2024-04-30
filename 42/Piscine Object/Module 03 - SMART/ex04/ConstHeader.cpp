//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ConstHeader.hpp"

ConstHeader::ConstHeader(const std::string header_str): header_str_(header_str) {
    ;
}

ConstHeader::~ConstHeader() {
    ;
}

const std::string ConstHeader::header() {
    return header_str_;
}