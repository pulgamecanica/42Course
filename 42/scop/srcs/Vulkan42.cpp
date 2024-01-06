//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Vulkan42.hpp"

Vulkan42::Vulkan42() {
	// TODO (default constructor)
}

Vulkan42::Vulkan42(const Vulkan42& param) {
	// TODO (copy constructor)
	(void)param;
}

Vulkan42::~Vulkan42() {
	std::cout << "Vulkan42" << " destroyed" << std::endl;
	// TODO (destructor)
}

Vulkan42& Vulkan42::operator= (const Vulkan42& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Vulkan42& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

