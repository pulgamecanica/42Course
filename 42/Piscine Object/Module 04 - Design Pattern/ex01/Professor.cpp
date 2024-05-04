//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"
#include "StaffList.hpp"

Professor::Professor(const std::string & name):
	Staff(name), current_room_(nullptr) {
    StaffList::GetInstance()->add(this); // Allow Polymorphism, This will add the Professor Pointer instead of Staff
}

Professor::~Professor() {
    // std::cout << "Professor" << " destroyed" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
	s << "Professor: " << *(dynamic_cast<const Staff*>(&p));
	return s;
}
