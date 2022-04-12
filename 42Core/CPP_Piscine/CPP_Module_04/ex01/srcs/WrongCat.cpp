//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "WrongCat.hpp"

WrongCat::WrongCat(): WrongAnimal("Cat :(") {
	std::cout << "WrongCat" << " created [default]" << std::endl;
}

WrongCat::WrongCat(const WrongCat& param): WrongAnimal(param.getType()) {
	std::cout << "WrongCat" << " created [copy]" << std::endl;
}

WrongCat::~WrongCat() {
	std::cout << "WrongCat" << " destroyed" << std::endl;
}

WrongCat& WrongCat::operator= (const WrongCat& param) {
	std::cout << "WrongCat" << " [operator=]" << std::endl;
	WrongAnimal::operator=(param);
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const WrongCat& param) {
	return (s << (WrongAnimal)param);
}

void WrongCat::makeSound() const {
	std::cout << "Miaaauuu miaaaauuuu grrrrr grrrrr grrrr brrrgbbbrrr" << std::endl;
}
