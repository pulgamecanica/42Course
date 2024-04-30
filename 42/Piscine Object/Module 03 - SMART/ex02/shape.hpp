//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FIGURE_HPP__
# define __FIGURE_HPP__

#include <iostream>

class Shape {
    public:
        Shape(const std::string & type);
        ~Shape();
        const std::string getType() const;
        virtual float perimeter() const = 0;
        virtual float area() const = 0;
    private:
        const std::string type_;
};
std::ostream& operator<<(std::ostream&, const Shape&);
#endif
