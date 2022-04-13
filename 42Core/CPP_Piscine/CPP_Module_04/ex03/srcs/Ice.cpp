//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Ice.hpp"

Ice::Ice(): AMateria("ice") {}

Ice::Ice(const Ice& param):  AMateria(param.getType()) {}

Ice::~Ice() {
	std::cout << "Ice" << " destroyed" << std::endl;
}

Ice& Ice::operator= (const Ice& param) {
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Ice& param) {
	s << param.getType() << " Materia";
	return (s);
}

AMateria* Ice::clone() const {
	return (new Ice(*this));
}

void Ice::use(ICharacter& target) {
	std::cout << "Ice: * shoots an ice bolt at " << target.getName() <<" *" << std::endl;
}
