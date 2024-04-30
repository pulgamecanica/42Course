//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex02.inc"
#include "shape.hpp"
#include "rectangle.hpp"
#include "circle.hpp"
#include "triangle.hpp"

void printShapeByRef(const Shape & fig) {
    std::cout << fig << std::endl;
}

void printShapeByPointer(const Shape* fig_ptr) {
    std::cout << *fig_ptr << std::endl;
}


int main(void)
{
    if (DEBUG)
        std::cout << "Debug ON!" << std::endl;
    std::cout << BLUE << "- - - Normal Rectangle - - -" << ENDC << std::endl;
    Rectangle rect(Point(42, 42.42), Point(4.2, 0.42));
    std::cout << rect << std::endl;
    printShapeByRef(rect);
    std::cout << BLUE << "- - - Polymorphic Rectangle - - -" << ENDC << std::endl;
    {
        Shape * fig = &rect;
        printShapeByPointer(fig);
    }
    std::cout << BLUE << "- - - Normal Circle - - -" << ENDC << std::endl;
    Circle circ(Point(2.4, 4.2), 42);
    std::cout << circ << std::endl;
    printShapeByRef(circ);
    std::cout << BLUE << "- - - Polymorphic Circle - - -" << ENDC << std::endl;
    {
        Shape * fig = &circ;
        printShapeByPointer(fig);
    }
    std::cout << BLUE << "- - - Normal Triangle - - -" << ENDC << std::endl;
    // https://www.triangle-calculator.com/
    Triangle tri(Point(2.4, 4.2), Point(4.5, 8.2), Point(3.0, 0));
    std::cout << tri << std::endl;
    printShapeByRef(tri);
    std::cout << BLUE << "- - - Polymorphic Triangle - - -" << ENDC << std::endl;
    {
        Shape * fig = &tri;
        printShapeByPointer(fig);
    }
    std::cout << BLUE << "- - - - - - - - - - - - - - - - -" << ENDC << std::endl;


    return (0);
}
