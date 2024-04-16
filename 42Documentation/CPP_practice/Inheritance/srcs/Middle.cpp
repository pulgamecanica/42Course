//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Middle.hpp"

Middle::Middle() {
	std::cout << "Middle" << " created" << std::endl;
}

Middle::~Middle() {
	std::cout << "Middle" << " destroyed" << std::endl;
}

void Middle::msg() {
	std::cout << "MSG[MIDDLE]" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const Middle& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

