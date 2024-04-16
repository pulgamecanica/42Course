//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "A.hpp"

A::A() {
	std::cout << "A" << " created" << std::endl;
	// TODO (default constructor)
}

A::~A() {
	std::cout << "A" << " destroyed" << std::endl;
	// TODO (destructor)
}

void A::msg() {
	std::cout << "MSG[A]" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const A& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

