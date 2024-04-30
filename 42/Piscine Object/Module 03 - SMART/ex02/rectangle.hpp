//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __RECTANGLE_HPP__
# define __RECTANGLE_HPP__

#include <iostream>

#include "shape.hpp"
#include "point.hpp"

class Rectangle: public Shape {
    public:
        Rectangle(Point trc, Point size);
        ~Rectangle();
        Point getTopRight() const;
        Point getSize() const;
        float perimeter() const;
        float area() const;
    private:
        Point trc_;
        Point size_;
};
std::ostream& operator<<(std::ostream&, const Rectangle&);
#endif
