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

// Settings::Settings(const Settings& param) {
// 	// TODO (copy constructor)
// 	(void)param;
// }

// Settings& Settings::operator= (const Settings& param) {
// 	// TODO (Assignment operatior)
// 	// std::swap()
// 	(void)param;
// 	return (*this);
// }

std::ostream& operator<<(std::ostream& s, const Settings& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

