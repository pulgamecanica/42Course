//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Vector.hpp"
#include "Math.hpp"

Vector::Vector() {
	Point p1(0, 0);
	Point p2(0, 0);

	a = p1;
	b = p2;
	angle = 0;
	magnitude = 0.0;
}

Vector::Vector(Point p1, Point p2) {
	a = p1;
	b = p2;
	angle = atan((p1.y - p2.y) / (p1.x - p2.x));
	magnitude = sqrt(((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)));
}

Vector::Vector(int deg, double mag)
{
	Point p1;
	Point p2(cos(deg * PI / 180) * mag, sin(deg * PI / 180) * mag);
	angle = deg;
	magnitude = mag;
	a = p1;
	b = p2;
}

Vector::~Vector() {
	// TODO (destructor)
}

void	Vector::print()
{
	std::cout << "a: ";
	a.print();
        std::cout << "b: ";
	b.print();
	std::cout << angle << "°, magnitude: " << magnitude << std::endl;
}
