//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Point.hpp"

Point::Point(): x(0), y(0) {}


Point::Point(const Point& param): x(param.x), y(param.y) {}

Point::~Point() {
	std::cout << "Destroyed Point" << std::endl;
}

Point Point::operator= (const Point param) {
//	std::swap(x, param.x);
//	std::swap(y, param.y);
	(void)param;
	return (*this);
}

void Point::print(void) {
//	std::cout << "P (" << x << ", " << y << ")" << std::endl;
}

