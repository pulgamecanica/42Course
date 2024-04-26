//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Line.hpp"

namespace ex01_bonus {
            
    Line::Line(const Vector2& p1, const Vector2& p2, uint32_t color): p1_(p1), p2_(p2), color_(color) {
    }

    Line::~Line() {
    }

    const Vector2& Line::p1() const {
        return p1_;
    }

    const Vector2& Line::p2() const {
        return p2_;
    }

    uint32_t Line::get_color() const {
        return color_;
    }


    std::ostream& operator<<(std::ostream& s, const Line& v) {
        s << v.p1() << " - " << v.p2();
        return s;
    }


}
