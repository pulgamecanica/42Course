//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VECTOR_HPP__
# define __VECTOR_HPP__

#include "Point.hpp"

class Vector {
	public:
		int	angle;
		double 	magnitude;
		Point	a;
		Point	b;
		Vector();
		Vector(Point, Point);
		Vector(int, double);
		~Vector();		// Destructor
		void	print();
};

#endif

