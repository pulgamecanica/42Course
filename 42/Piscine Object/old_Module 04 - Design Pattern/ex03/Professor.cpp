//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Professor.hpp"
#include "Course.hpp"

Professor::Professor(const std::string & name):
	Staff(name) {
}

Professor::~Professor() {
	;
}

std::ostream& operator<<(std::ostream& s, const Professor& p) {
	s << "Professor: " << *(dynamic_cast<const Staff*>(&p));
	return s;
}
