//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Cure.hpp"

Cure::Cure(): AMateria("cure") {}

Cure::Cure(const Cure& param):  AMateria(param.getType()) {}

Cure::~Cure() {
	std::cout << "Cure" << " destroyed" << std::endl;
}

Cure& Cure::operator= (const Cure& param) {
	(void)param;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Cure& param) {
	s << param.getType() << " Materia";
	return (s);
}

AMateria* Cure::clone() const {
	return (new Cure(*this));
}

void Cure::use(ICharacter& target) {
	std::cout << "Cure: * heals " << target.getName() <<"’s wounds *" << std::endl;
}
