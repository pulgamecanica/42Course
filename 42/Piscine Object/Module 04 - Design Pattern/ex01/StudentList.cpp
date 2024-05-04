//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "StudentList.hpp"

StudentList::StudentList() {
    if (DEBUG)
        std::cout << YELLOW << "[CourseList] " << GREEN << "CREATE\t" << ENDC << std::endl;
}

StudentList::~StudentList() {
    ; // Will never be destroyed
}

std::ostream& operator<<(std::ostream& s, const StudentList& sl) {
    s << "StudentList (" << sl.size() << ")";
    return (s);
}

