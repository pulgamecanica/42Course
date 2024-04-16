//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "VA.hpp"

VA::VA() {
	std::cout << "VA" << " created" << std::endl;
}

VA::~VA() {
	std::cout << "VA" << " destroyed" << std::endl;
}

void VA::msg() {
	std::cout << "MSG[VA]" << std::endl;
}


std::ostream& operator<<(std::ostream& s, const VA& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

