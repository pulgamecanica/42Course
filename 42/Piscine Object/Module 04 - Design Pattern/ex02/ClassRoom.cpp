//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ClassRoom.hpp"
#include "Course.hpp"
#include "Staff.hpp"
#include "Student.hpp"

ClassRoom::ClassRoom(): current_course_(nullptr) {
	;
}

ClassRoom::~ClassRoom() {
    // std::cout << "ClassRoom" << " destroyed" << std::endl;
}

bool ClassRoom::canEnter(Person* p_person) {
	return (dynamic_cast<Staff *>(p_person) != nullptr || dynamic_cast<Student *>(p_person) != nullptr);
}

void ClassRoom::assignCourse(Course* p_course) {
	current_course_ = p_course;
}

Course * ClassRoom::getCourse() {
	return current_course_;
}

std::ostream& operator<<(std::ostream& s, const ClassRoom& cr) {
	s << "ClassRoom: " << *(dynamic_cast<const Room *>(&cr));
	return s;
}
