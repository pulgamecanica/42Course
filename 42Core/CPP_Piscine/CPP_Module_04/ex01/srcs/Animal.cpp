//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Animal.hpp"

Animal::Animal(): type("Alien...") {
	std::cout << "Animal" << " created [default]" << std::endl;
}

Animal::Animal(std::string str): type(str) {
	std::cout << "Animal " <<  str << " created [str&]" << std::endl;
}
Animal::Animal(const Animal& param): type(param.type) {
	std::cout << "Animal" << " created [copy]" << std::endl;
}

Animal::~Animal() {
	std::cout << "Animal" << " destroyed" << std::endl;
}

Animal& Animal::operator= (const Animal& param) {
	std::cout << "Animal" << " created [operator=]" << std::endl;
	type = param.type;
	return (*this);
}

std::string Animal::getType() const {
	return (type);
}

std::ostream& operator<<(std::ostream& s, const Animal& param) {
	s << param.getType();
	return (s);
}

void Animal::makeSound() const {
	std::cout << "Weird Alien noise: !_\\+!\"¿≤*$ %	å©π© å∂ " << std::endl;
}
