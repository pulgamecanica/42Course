//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __COMPLEX_HPP__
# define __COMPLEX_HPP__

class Complex {
	public:
		double	a;
		double	b;
		Complex();
		Complex(double a, double b);
		~Complex();		// Destructor
		Complex operator + (Complex);
		Complex operator - (Complex);
		void	print();
};

#endif

