//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __LINE_HPP__
# define __LINE_HPP__

extern "C" {
    #include <stdint.h> // for uint32_t
}

#include <iostream>

#include "Vector2.hpp"

namespace ex01_bonus {
    class Line {
        public:
            Line(const Vector2& p1, const Vector2& p2, uint32_t color);
            ~Line();
            const Vector2& p1() const;
            const Vector2& p2() const;
            uint32_t get_color() const;
        private:
            const Vector2 p1_;
            const Vector2 p2_;
            const uint32_t color_;
    };
    std::ostream& operator<<(std::ostream& s, const Line& v);
}
#endif
