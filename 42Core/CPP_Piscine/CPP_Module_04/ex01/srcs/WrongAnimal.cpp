//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal(): type("Alien...") {
	std::cout << "WrongAnimal" << " created [default]" << std::endl;
}

WrongAnimal::WrongAnimal(std::string str): type(str) {
	std::cout << "WrongAnimal " <<  str << " created [str&]" << std::endl;
}
WrongAnimal::WrongAnimal(const WrongAnimal& param): type(param.type) {
	std::cout << "WrongAnimal" << " created [copy]" << std::endl;
}

WrongAnimal::~WrongAnimal() {
	std::cout << "WrongAnimal" << " destroyed" << std::endl;
}

WrongAnimal& WrongAnimal::operator= (const WrongAnimal& param) {
	std::cout << "WrongAnimal" << " created [operator=]" << std::endl;
	type = param.type;
	return (*this);
}

std::string WrongAnimal::getType() const {
	return (type);
}

std::ostream& operator<<(std::ostream& s, const WrongAnimal& param) {
	s << param.getType();
	return (s);
}

void WrongAnimal::makeSound() const {
	std::cout << "Weird Alien noise: !_\\+!\"¿≤*$ %	å©π© å∂ " << std::endl;
}
