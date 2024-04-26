//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
	#include <cmath>
}

#include "Vector2.hpp"

Vector2::Vector2(float x, float y): x_(x), y_(y) {

}

Vector2::~Vector2() {
	// std::cout << "Vector2" << " destroyed" << std::endl;
}

float Vector2::x() const {
	return x_;
}
float Vector2::y() const {
	return y_;
}

bool Vector2::operator==(const Vector2& rhs) const {
	return floor(x()) == floor(rhs.x()) && floor(y()) == floor(rhs.y());
}

std::ostream& operator<<(std::ostream& s, const Vector2& v) {
	s << v.x() << ", " << v.y();
	return (s);
}

