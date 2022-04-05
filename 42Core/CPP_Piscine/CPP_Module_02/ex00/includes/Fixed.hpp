//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FIXED_HPP__
# define __FIXED_HPP__

#include <iostream>

class Fixed {
	int	num;
	static const int frac_bits = 8;
	public:
		Fixed();
		Fixed(const Fixed&);
		~Fixed();
		Fixed	operator= (Fixed);
		int		getRawBits(void) const;
		void	setRawBits(int const raw);
};

#endif

