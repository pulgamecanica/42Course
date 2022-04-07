//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FIXED_HPP__
# define __FIXED_HPP__

#include <iostream>
#include <cmath>

class Fixed {
	int	num;
	static const int frac_bits = 8;
	public:
		Fixed();
		Fixed(const Fixed&);
		Fixed(const int&);
		Fixed(const float&);
		~Fixed();
		Fixed&				operator= (Fixed);
		float				toFloat(void) const;
		int					toInt(void) const;
		int					getRawBits(void) const;
		void				setRawBits(int const raw);
};

std::ostream&	operator<<(std::ostream&, const Fixed&);

#endif
