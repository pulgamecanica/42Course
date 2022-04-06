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
		Fixed(const int);
		Fixed(const float);
		~Fixed();
		Fixed&				operator= (const Fixed&);
		bool				operator> (const Fixed&) const;
		bool				operator< (const Fixed&) const;
		bool				operator== (const Fixed&) const;
		bool				operator!= (const Fixed&) const;
		bool				operator<= (const Fixed&) const;
		bool				operator>= (const Fixed&) const;
		Fixed				operator+ (const Fixed&) const;
		Fixed				operator- (const Fixed&) const;
		Fixed				operator* (const Fixed&) const;
		Fixed				operator/ (const Fixed&) const;
		Fixed&				operator++(void);
		Fixed				operator++(int);
		Fixed&				operator--(void);
		Fixed				operator--(int);
		static Fixed&			min(Fixed&, Fixed&);
		static Fixed&			max(Fixed&, Fixed&);
		static const Fixed&		min(const Fixed&, const Fixed&);
		static const Fixed&		max(const Fixed&, const Fixed&);
		float				toFloat(void) const;
		int				toInt(void) const;
		int				getRawBits(void) const;
		void				setRawBits(int const raw);
};

std::ostream&	operator<<(std::ostream&, const Fixed&);

#endif

