//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"


Professor::Professor(const std::string & name):
	Staff(name), current_course(nullptr) {
		(void)current_course;
	;
}

Professor::~Professor() {
    // std::cout << "Professor" << " destroyed" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
	s << "Professor: " << *(dynamic_cast<const Staff*>(&p));
	return s;
}
