//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Cat.hpp"

Cat::Cat(): Animal("Cat") {
	std::cout << "Cat" << " created [default]" << std::endl;
}

Cat::Cat(const Cat& param): Animal(param.getType()) {
	std::cout << "Cat" << " created [copy]" << std::endl;
}

Cat::~Cat() {
	std::cout << "Cat" << " destroyed" << std::endl;
}

Cat& Cat::operator= (const Cat& param) {
	std::cout << "Cat" << " [operator=]" << std::endl;
	Animal::operator=(param);
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Cat& param) {
	return (s << (Animal)param);
}

void Cat::makeSound() const {
	std::cout << "Miaaauuu miaaaauuuu grrrrr grrrrr grrrr brrrgbbbrrr" << std::endl;
}
