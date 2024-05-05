//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ClassRoom.hpp"
#include "Course.hpp"
#include "Staff.hpp"
#include "Student.hpp"

ClassRoom::ClassRoom(): current_course_(nullptr) {
	(void)current_course_;
	;
}

ClassRoom::~ClassRoom() {
    // std::cout << "ClassRoom" << " destroyed" << std::endl;
}

bool ClassRoom::canEnter(Person* p_person) {
	return (dynamic_cast<Staff *>(p_person) != nullptr || dynamic_cast<Student *>(p_person) != nullptr);
}

std::ostream& operator<<(std::ostream& s, const ClassRoom& cr) {
	s << "ClassRoom: " << *(dynamic_cast<const Room *>(&cr));
	return s;
}
