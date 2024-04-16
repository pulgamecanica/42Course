//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "VMiddle.hpp"

VMiddle::VMiddle() {
	std::cout << "VMiddle" << " created" << std::endl;
}

VMiddle::~VMiddle() {
	std::cout << "VMiddle" << " destroyed" << std::endl;
}

void VMiddle::msg() {
	std::cout << "MSG[VMIDDLE]" << std::endl;
}

std::ostream& operator<<(std::ostream& s, const VMiddle& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

