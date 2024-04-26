//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VECTOR2_HPP__
# define __VECTOR2_HPP__

extern "C" {
    #include <stdint.h> // for uint32_t
}

#include <iostream>

namespace ex01_bonus {
    class Vector2 {
        public:
            Vector2(float x, float y, uint32_t color = 0x424242ff);
            Vector2(const Vector2 &);
            ~Vector2();
            uint32_t get_color() const;
            float x() const;
            float y() const;
            bool operator==(const Vector2& lhs) const;
        private:
            float x_;
            float y_;
            uint32_t color_;
    };
    std::ostream& operator<<(std::ostream& s, const Vector2& v);
}

#endif
