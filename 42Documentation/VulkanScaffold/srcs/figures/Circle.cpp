//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Circle.hpp"

using namespace scop;

Circle::Circle(point p, int r): Figure(p) {
	this->r = r;
}

Circle::~Circle() {
	std::cout << *this << " destroyed" << std::endl;
}

int		Circle::getRad() const { return (r); }

bool	Circle::isHovered(point & p) const {
	return (((p.x - pos.x) * (p.x - pos.x)) +
			((p.y - pos.y) * (p.y - pos.y)) <= r*r);
}

std::ostream& scop::operator<<(std::ostream& s, const Circle& param) {
	s << "Circle [" << param.getX() << "," << param.getY() << "] rad: " << param.getRad() << "px";
	return (s);
}
