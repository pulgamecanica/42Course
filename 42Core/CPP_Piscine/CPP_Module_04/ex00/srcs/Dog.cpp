//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Dog.hpp"

Dog::Dog(): Animal("Dog") {
	std::cout << "Dog" << " created [default]" << std::endl;
}

Dog::Dog(const Dog& param): Animal(param.getType()) {
	std::cout << "Dog" << " created [copy]" << std::endl;
}

Dog::~Dog() {
	std::cout << "Dog" << " destroyed" << std::endl;
}

Dog& Dog::operator= (const Dog& param) {
	std::cout << "Dog" << " [operator=]" << std::endl;
	Animal::operator=(param);
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Dog& param) {
	return (s << (Animal)param);
}

void Dog::makeSound() const {
	std::cout << "Bark Wauuuuuuuuu Wauuuuffffff GRRRRRRR" << std::endl;
}
