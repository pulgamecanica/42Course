//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "GUIContainer.hpp"

GUIContainer::GUIContainer() {
	// TODO (default constructor)
}

GUIContainer::GUIContainer(const GUIContainer& param) {
	// TODO (copy constructor)
	(void)param;
}

GUIContainer::~GUIContainer() {
	std::cout << "GUIContainer" << " destroyed" << std::endl;
	// TODO (destructor)
}

GUIContainer& GUIContainer::operator= (const GUIContainer& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const GUIContainer& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

