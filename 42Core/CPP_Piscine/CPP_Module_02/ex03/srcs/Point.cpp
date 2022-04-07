//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Point.hpp"

Point::Point(): x(0), y(0) {}


Point::Point(const Point& param): x(param.x), y(param.y) {}

Point::Point(const float f1, const float f2): x(f1), y(f2) {}

Point::~Point() {}

Point Point::operator= (Point param) {
	Point tmp(param);
	return (tmp);
}

Fixed Point::get_x() const {
	return (x);
}

Fixed Point::get_y() const {
	return (y);
}

std::ostream& operator<<(std::ostream& s, const Point& p) {
	Fixed x(p.get_x());
	Fixed y(p.get_y());
	s << "(" << x << ", " << y << ")" << std::endl;
	return (s);
}
