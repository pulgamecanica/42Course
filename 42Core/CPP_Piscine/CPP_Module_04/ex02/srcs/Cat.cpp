//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Cat.hpp"

Cat::Cat(): Animal("Cat") {
	std::cout << "Cat" << " created [default]" << std::endl;
	b = new Brain;
}

Cat::Cat(const Cat& param): Animal(param.getType()) {
	std::cout << "Cat" << " created [copy]" << std::endl;
	*this = param;
}

Cat::~Cat() {
	delete(b);
	std::cout << "Cat" << " destroyed" << std::endl;
}

Cat& Cat::operator= (const Cat& param) {
	std::cout << "Cat" << " [operator=]" << std::endl;
	Animal::operator=(param);
	if (this != &param)
		*b = *param.b;
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Cat& param) {
	return (s << param.getType());
}

void Cat::makeSound() const {
	std::cout << "Miaaauuu miaaaauuuu grrrrr grrrrr grrrr brrrgbbbrrr" << std::endl;
}
