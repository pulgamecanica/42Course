//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "math.h"

#include "triangle.hpp"

Triangle::Triangle(Point a, Point b, Point c):
    Shape("triangle"), a_(a), b_(b), c_(c) {
}

Triangle::~Triangle() {
    std::cout << "Triangle destroyed" << std::endl;
}

// https://xoax.net/sub_cpp/ref_examples/ex_tri_area/
float Triangle::area() const {
    float area = ((b_.x - a_.x)*(c_.y - a_.y) - (c_.x - a_.x)*(b_.y - a_.y)) / 2.0;
    return (area > 0.0) ? area : -area;
}

float Triangle::perimeter() const {
    float ab = sqrt((double)(b_.x-a_.x) * (b_.x-a_.x) + (b_.y-a_.y) * (b_.y-a_.y));
    float bc = sqrt((double)(b_.x-c_.x) * (b_.x-c_.x) + (b_.y-c_.y) * (b_.y-c_.y));
    float ca = sqrt((double)(a_.x-c_.x) * (a_.x-c_.x) + (a_.y-c_.y) * (a_.y-c_.y));
    return  ab + bc + ca;
}

Point Triangle::getA() const {
    return a_;
}

Point Triangle::getB() const {
    return b_;
}

Point Triangle::getC() const {
    return c_;
}

std::ostream& operator<<(std::ostream& s, const Triangle& tri) {
    s << "A: " << tri.getA() << " B: " << tri.getB() << " C: " << tri.getC();
    return (s);
}

