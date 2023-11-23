//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "GUIContainer.hpp"

GUIContainer::GUIContainer() {
	// TODO (default constructor)
}

GUIContainer::~GUIContainer() {
	std::cout << "GUIContainer" << " destroyed" << std::endl;
	// TODO (destructor)
}

std::ostream& operator<<(std::ostream& s, const GUIContainer& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

