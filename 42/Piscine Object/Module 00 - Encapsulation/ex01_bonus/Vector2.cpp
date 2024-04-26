//***************************//
//*Template by pulgamecanica*//
//***************************//

extern "C" {
    #include <cmath>
}

#include "Vector2.hpp"

namespace ex01_bonus {

    Vector2::Vector2(float x, float y, uint32_t color): x_(x), y_(y), color_(color) {
        ;
    }

    Vector2::Vector2(const Vector2 & v): x_(v.x()), y_(v.y()), color_(v.get_color()) {
        ;
    }

    Vector2::~Vector2() {
        ;
    }

    uint32_t Vector2::get_color() const {
        return color_;
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
}
