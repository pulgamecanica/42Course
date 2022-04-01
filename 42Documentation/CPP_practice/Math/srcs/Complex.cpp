//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Complex.hpp"
#include "Math.hpp"

Complex::Complex() {
	a = 0;
	a = 0;
}

Complex::Complex(double n1, double n2) {
	a = n1;
	b = n2;
}

Complex::~Complex() {
	// TODO (destructor)
}

Complex Complex::operator+ (Complex c) {
	Complex new_c;

	new_c.a = a + c.a;
	new_c.b = b + c.b;
	return (new_c);
}

Complex Complex::operator- (Complex c) {
	Complex new_c;

	new_c.a = a + c.a;
	new_c.b = b + c.b;
	return (new_c);
}

void	Complex::print() {
	std::cout << "[" << a << ", " << b << "i]" << std::endl;
}
