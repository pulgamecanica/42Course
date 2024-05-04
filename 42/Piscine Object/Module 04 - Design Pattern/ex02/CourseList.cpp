//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <algorithm>

#include "School.inc"

#include "CourseList.hpp"

CourseList::CourseList() {
    if (DEBUG)
        std::cout << YELLOW << "[CourseList] " << GREEN << "CREATE\t" << ENDC << std::endl;
}

CourseList::~CourseList() {
    ; // Will never be destroyed
}

std::ostream& operator<<(std::ostream& s, const CourseList& cl) {
    s << "CourseList (" << cl.size() << ")";
    return (s);
}

