//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "rectangle.hpp"

Rectangle::Rectangle(Point trc, Point size):
    Shape("rectangle"), trc_(trc), size_(size) {
    ;
}

Rectangle::~Rectangle() {
    std::cout << "Rectangle destroyed" << std::endl;
}

float Rectangle::area() const {
    return size_.x * size_.y;
}

float Rectangle::perimeter() const {
    return (size_.x * 2) + (size_.y * 2);
}

Point Rectangle::getTopRight() const {
    return trc_;
}

Point Rectangle::getSize() const {
    return size_;
}

std::ostream& operator<<(std::ostream& s, const Rectangle& rect) {
    s << "TRC: " << rect.getTopRight() << " Size: " << rect.getSize();
    return (s);
}

