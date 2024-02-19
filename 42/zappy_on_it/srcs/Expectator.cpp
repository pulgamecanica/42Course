//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Expectator.hpp"

Expectator::Expectator() {
	// TODO (default constructor)
}

Expectator::Expectator(const Expectator& param) {
	// TODO (copy constructor)
	(void)param;
}

Expectator::~Expectator() {
	std::cout << "Expectator" << " destroyed" << std::endl;
	// TODO (destructor)
}

Expectator& Expectator::operator= (const Expectator& param) {
	// TODO (Assignment operatior)
	// std::swap()
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Expectator& param) {
	// s << param.CONST_METHOD()
	(void)param;
	return (s);
}

