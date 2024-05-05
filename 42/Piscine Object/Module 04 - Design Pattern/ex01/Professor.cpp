//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"

Professor::Professor(const std::string & name):
	Staff(name), current_course(nullptr) {
    StaffList::GetInstance()->add(this); // Allow Polymorphism, This will add the Professor Pointer instead of Staff
    (void)current_course;
}

Professor::~Professor() {
	current_course = nullptr;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
	s << "Professor: " << *(dynamic_cast<const Staff*>(&p));
	return s;
}
