//***************************
//*Template by pulgamecanica*//
//***************************//

#include "Fixed.hpp"

Fixed::Fixed() {num = 0;}

Fixed::Fixed(const Fixed& param) {num = param.num;}

Fixed::Fixed(const int a) {num = a<<8;}

Fixed::Fixed(const float a) {num = roundf(a * 256);}

Fixed::~Fixed() {}

Fixed& Fixed::operator= (Fixed param) {
	std::swap(num, param.num);
	return (*this);
}

void Fixed::setRawBits(int const raw) {num = raw;}

int Fixed::getRawBits(void) const {return (num);}

float Fixed::toFloat(void) const {return ((float)num / 256);}

int	Fixed::toInt(void) const {return (num>>8);}

std::ostream& operator<<(std::ostream& s, const Fixed& fn) {
	s << fn.toFloat();
	return (s);
}

bool Fixed::operator> (const Fixed& fp) const {return (num > fp.num);}

bool Fixed::operator< (const Fixed& fp) const {return (num < fp.num);}

bool Fixed::operator== (const Fixed& fp) const {return (fp.num == num);}

bool Fixed::operator!= (const Fixed& fp) const {return (fp.num != num);}

bool Fixed::operator<= (const Fixed& fp) const {return (*this == fp || *this < fp);}

bool Fixed::operator>= (const Fixed& fp) const {return (*this == fp || *this > fp);}

Fixed Fixed::operator+ (const Fixed& fp) const
{
	Fixed new_fp;

	new_fp.setRawBits(num + fp.num);
	return(new_fp);
}

Fixed Fixed::operator- (const Fixed& fp) const {
	Fixed new_fp;

	new_fp.setRawBits(num - fp.num);
	return (new_fp);
}

Fixed Fixed::operator* (const Fixed& fp) const {return (Fixed(this->toFloat() * fp.toFloat()));}

Fixed Fixed:: operator/ (const Fixed& fp) const {return (Fixed(this->toFloat() / fp.toFloat()));}

Fixed& Fixed::operator++(void) {
	++num;
	return (*this);
}

Fixed Fixed::operator++(int) {
	Fixed tmp(*this);
	num++;
	return (tmp);
}

Fixed& Fixed::operator--(void) {
	--num;
	return (*this);
}

Fixed Fixed::operator--(int) {
	Fixed tmp(*this);
	num--;
	return (tmp);
}

Fixed& Fixed::min(Fixed& fp1, Fixed& fp2) {
	if (fp2 < fp1)
		return (fp2);
	return (fp1);
}

Fixed& Fixed::max(Fixed& fp1, Fixed& fp2) {
	if (fp2 > fp1)
		return (fp2);
	return (fp1);
}

const Fixed& Fixed::min(const Fixed& fp1, const Fixed& fp2) {
	if (fp2 < fp1)
		return (fp2);
	return (fp1);
}

const Fixed& Fixed::max(const Fixed& fp1, const Fixed& fp2) {
	if (fp2 > fp1)
		return (fp2);
	return (fp1);
}
