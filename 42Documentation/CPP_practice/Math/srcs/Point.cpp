//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Point.hpp"
#include "Math.hpp"

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int n1, int n2) {
	x = n1;
	y = n2;
}

Point::~Point() {
	// TODO (destructor)
}

Point Point::operator + (Point p) {
	Point new_p;

	new_p.x = x + p.x;
	new_p.y = y + p.y;
	return (new_p);
}

Point Point::operator + (Vector vec) {
	Point	new_p;

	new_p.x = x + cos(vec.angle * PI / 180) * vec.magnitude;
        new_p.y = y + sin(vec.angle * PI / 180) * vec.magnitude;
	return (new_p);
}

Point Point::operator - (Point p) {
	Point new_p;

	new_p.x = x - p.x;
	new_p.y = y - p.y;
	return (new_p);
}

void	Point::print() {
	std::cout << "(" << x << ", " << y << ")" << std::endl;
}
