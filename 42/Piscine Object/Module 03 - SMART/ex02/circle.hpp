//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CIRCLE_HPP__
# define __CIRCLE_HPP__

#include <iostream>

#include "shape.hpp"
#include "point.hpp"

class Circle: public Shape {
    public:
        Circle(Point xx, float rad);
        ~Circle();
        float area() const;
        float perimeter() const;
        Point getCenter() const;
        float getRad() const;
    private:
        Point xx_;
        float rad_;
};
std::ostream& operator<<(std::ostream&, const Circle&);
#endif
