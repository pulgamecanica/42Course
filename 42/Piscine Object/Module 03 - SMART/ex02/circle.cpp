//***************************//
//*Template by pulgamecanica*//
//***************************//

#include <math.h>

#include "circle.hpp"
#define PI 3.1416

Circle::Circle(Point xx, float rad):
    Shape("circle"), xx_(xx), rad_(rad) {
    ;
}

Circle::~Circle() {
    std::cout << "Circle destroyed" << std::endl;
}

float Circle::area() const {
    return PI * (pow(rad_, 2));
}


float Circle::perimeter() const {
    return 2 * PI * rad_;
}

Point Circle::getCenter() const {
    return xx_;
}

float Circle::getRad() const {
    return rad_;
}

std::ostream& operator<<(std::ostream& s, const Circle& c) {
    s << "Xo: "<< c.getCenter() << " Rad: " << c.getRad();
    return (s);
}

