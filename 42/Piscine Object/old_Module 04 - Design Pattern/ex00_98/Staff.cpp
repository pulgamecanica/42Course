//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.inc"
#include "Staff.hpp"

namespace SpringfieldElementary {
    
Staff::~Staff() {
    ;
}

void Staff::sign() {
    ;
}

std::ostream & operator<<(std::ostream & s, const Staff & st) {
    s << BLUE << "[Staff] " << ENDC << *(dynamic_cast<const Person*>(&st));
    return s;
}

}
