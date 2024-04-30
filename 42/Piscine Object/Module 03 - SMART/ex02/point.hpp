//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __POINT_HPP__
# define __POINT_HPP__

#include <iostream>

struct Point {
    Point(float x, float y);
    ~Point();
    float x;
    float y;
};
std::ostream& operator<<(std::ostream&, const Point&);

#endif
