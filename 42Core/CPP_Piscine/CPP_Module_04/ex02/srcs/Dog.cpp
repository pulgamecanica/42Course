//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Dog.hpp"

Dog::Dog(): Animal("Dog") {
	std::cout << "Dog" << " created [default]" << std::endl;
	b = new Brain;
}

Dog::Dog(const Dog& param): Animal(param.getType()) {
	std::cout << "Dog" << " created [copy]" << std::endl;
	*this = param;
}

Dog::~Dog() {
	delete(b);
	std::cout << "Dog" << " destroyed" << std::endl;
}

Dog& Dog::operator= (const Dog& param) {
	std::cout << "Dog" << " [operator=]" << std::endl;
	Animal::operator=(param);
	*b = *param.b;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Dog& param) {
	return (s << param.getType());
}

void Dog::makeSound() const {
	std::cout << "Bark Wauuuuuuuuu Wauuuuffffff GRRRRRRR" << std::endl;
}
