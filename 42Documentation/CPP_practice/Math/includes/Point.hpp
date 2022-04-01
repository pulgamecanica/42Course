//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __POINT_HPP__
# define __POINT_HPP__

class Vector;

class Point {
	public:
		short int	x;
		short int	y;
		Point();
		Point(int, int);
		~Point();		// Destructor
		void	print();
		Point	operator + (Point);
		Point   operator + (Vector);
		Point	operator - (Point);
};

#endif

