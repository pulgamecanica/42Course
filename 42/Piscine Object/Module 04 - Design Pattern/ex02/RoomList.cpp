//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "School.inc"

#include "RoomList.hpp"

RoomList::RoomList() {
 if (DEBUG)
        std::cout << YELLOW << "[CourseList] " << GREEN << "CREATE\t" << ENDC << std::endl;
}

RoomList::~RoomList() {
    ; // Will never be destroyed
}

std::ostream& operator<<(std::ostream& s, const RoomList& rl) {
    s << "RoomList (" << rl.size() << ")";
    return (s);
}

