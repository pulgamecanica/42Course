//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "StaffRestRoom.hpp"
#include "Staff.hpp"

StaffRestRoom::StaffRestRoom() {
    ;
}

StaffRestRoom::~StaffRestRoom() {
    ;
}

bool StaffRestRoom::canEnter(Person* p_person) {
    return (dynamic_cast<Staff *>(p_person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const StaffRestRoom& srr) {
    s << "StaffRestRoom: " << *(dynamic_cast<const Room *>(&srr));
    return (s);
}

