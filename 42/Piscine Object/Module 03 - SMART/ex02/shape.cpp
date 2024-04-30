//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "shape.hpp"

Shape::Shape(const std::string& type): type_(type) {
    ;
}

Shape::~Shape() {
    std::cout << "Shape " << type_ << " destroyed" << std::endl;
}

const std::string Shape::getType() const {
    return type_;
}

std::ostream& operator<<(std::ostream& s, const Shape& fig) {
    s << fig.getType() << " area: " << fig.area() << " perimeter: " << fig.perimeter();
    return (s);
}

