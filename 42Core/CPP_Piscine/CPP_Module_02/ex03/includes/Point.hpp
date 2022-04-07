//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __POINT_HPP__
# define __POINT_HPP__

#include "Fixed.hpp"
#include "colors.h"

class Point {
	Fixed const	x;
	Fixed const	y;
	public:
		Point();
		Point(const Point&);
		Point(const float, const float);
		~Point();
		Point	operator= (Point);
		Fixed	get_x() const;
		Fixed	get_y() const;
};

std::ostream&	operator<<(std::ostream&, const Point&);

#endif

