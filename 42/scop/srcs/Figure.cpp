//***************************//
//*Template by pulgamecanica*//
//***************************//
#include "Figure.hpp"

using namespace scop;

Figure::Figure(point position): pos(position) {
	// TODO (default constructor)
}

Figure::~Figure() {
	std::cout << *this << " destroyed" << std::endl;
}

bool	Figure::isHovered(point & pos) const {
	(void)pos;
	return false;
};

float	Figure::getX() const { return (pos.x); };

float	Figure::getY() const { return (pos.y); };

std::ostream& scop::operator<<(std::ostream& s, const Figure& param) {
	s << "Figure [" << param.getX() << ", " << param.getY() << "]";
	return (s);
}

