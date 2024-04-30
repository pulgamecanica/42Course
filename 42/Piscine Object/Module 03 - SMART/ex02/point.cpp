//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "point.hpp"

Point::Point(float x, float y): x(x), y(y) {
    ;
}

Point::~Point() {
    ;
}

std::ostream& operator<<(std::ostream& s, const Point& p) {
    s << "(" << p.x << ", " << p.y << ")";
    return s;
}
