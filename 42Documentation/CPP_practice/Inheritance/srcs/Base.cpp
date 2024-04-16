//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Base.hpp"

Base::Base() {
	std::cout << "Base" << " created" << std::endl;
}

Base::~Base() {
	std::cout << "Base" << " destroyed" << std::endl;
}

void Base::msg() {
	std::cout << "MSG[BASE]" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Base& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

