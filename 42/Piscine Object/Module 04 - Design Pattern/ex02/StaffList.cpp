//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "StaffList.hpp"

StaffList::StaffList() {
    if (DEBUG)
        std::cout << YELLOW << "[CourseList] " << GREEN << "CREATE\t" << ENDC << std::endl;
}

StaffList::~StaffList() {
    ; // Will never be destroyed
}

std::ostream& operator<<(std::ostream& s, const StaffList& sl) {
    s << "StaffList (" << sl.size() << ")";
    return (s);
}

