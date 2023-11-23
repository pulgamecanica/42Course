//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Settings.hpp"

Settings::Settings() {
	// TODO (default constructor)
}

Settings::~Settings() {
	std::cout << "Settings" << " destroyed" << std::endl;
	// TODO (destructor)
}

std::ostream& operator<<(std::ostream& s, const Settings& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

