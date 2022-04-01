//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Math.hpp"

int	main(void)
{
	Complex	c1(3, 4);
	Complex	c2(-1, 3);
	Complex c3;
	Point	p1(-1, 2);
	Point 	p2(-2, -5);
	Vector	v(30, 5.0);

	std::cout << "Print a complex number ";
	c1.print();
	std::cout << "Print the sum of two complex ";
	c3 = c1 + c2;
	c3.print();
	std::cout << "Print more points... \n";
	(p1 + p2).print();
	(p1 - p2).print();
	std::cout << "Print the sum of a vector\n";
	v.print();
	p1.print();
	(p1 + v).print();
	return (0);
}
