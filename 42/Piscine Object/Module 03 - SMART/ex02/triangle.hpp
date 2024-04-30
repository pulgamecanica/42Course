//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __TRIANGLE_HPP__
# define __TRIANGLE_HPP__

#include <iostream>

#include "shape.hpp"
#include "point.hpp"

class Triangle: public Shape {
    public:
        Triangle(Point a, Point b, Point c);
        ~Triangle();
        float area() const;
        float perimeter() const;
        Point getA() const;
        Point getB() const;
        Point getC() const;
    private:
        Point a_;
        Point b_;
        Point c_;
};
std::ostream& operator<<(std::ostream&, const Triangle&);
#endif
