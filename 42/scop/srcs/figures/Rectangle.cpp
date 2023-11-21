//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Rectangle.hpp"

using namespace scop;

Rectangle::Rectangle(point p, int width, int height): Figure(p) {
	this->width = width;
	this->height = height;
}

Rectangle::~Rectangle() {
	std::cout << *this << " destroyed" << std::endl;
}

int Rectangle::getWidth() const { return (width); }

int Rectangle::getHeight() const { return (height); }

bool	Rectangle::isHovered (point & p) const {
	return (p.x >= pos.x && p.x < pos.x + width && p.y >= pos.y && p.y < pos.y + height);
}

std::ostream& scop::operator<<(std::ostream& s, const Rectangle& param) {
	s << "Rectangle [" << param.getX() << "," << param.getY() << "] " << param.getWidth() << "x" <<  param.getHeight() << "px";
	return (s);
}

