//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __POINT_HPP__
# define __POINT_HPP__

#include "Fixed.hpp"

class Point {
	Fixed const	x;
	Fixed const	y;
	public:
		Point();
		Point(const Point&);
		~Point();
		Point	operator= (const Point);
		void	print();
};

#endif

