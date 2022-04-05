//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Fixed.hpp"

Fixed::Fixed() {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "[Default Constructor]" << std::endl;
	num = 0;
}

Fixed::Fixed(const Fixed& param) {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "[Copy Constructor]" << std::endl;
	num = param.num;
}

Fixed::~Fixed() {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "[Object Destroyed]" << std::endl;
}

Fixed Fixed::operator= (Fixed param) {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "[Assignment operator]" << std::endl;
	std::swap(num, param.num);
	return (*this);
}

void Fixed::setRawBits(int const raw) {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "setRawBits CALLED" << std::endl;
	num = raw;
}


int Fixed::getRawBits(void) const {
	std::cout << "Fixed Point"; std::cout.width(22); std::cout << "getRawBits CALLED" << std::endl;
	return (num);
}
