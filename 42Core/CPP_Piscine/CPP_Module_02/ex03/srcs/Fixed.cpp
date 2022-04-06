//***************************
//*Template by pulgamecanica*//
//***************************//

#include "Fixed.hpp"

Fixed::Fixed() {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Default Constructor]" << std::endl;
	num = 0;
}

Fixed::Fixed(const Fixed& param) {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Copy Constructor]" << std::endl;
	num = param.num;
}

Fixed::Fixed(const int a) {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Constructor (const int&)]" << std::endl;
	num = a<<8;
}

Fixed::Fixed(const float a) {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Constructor (const float&)]" << std::endl;
	num = roundf(a * 256);
}

Fixed::~Fixed() {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Object Destroyed]" << std::endl;
}

Fixed& Fixed::operator= (const Fixed& param) {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "[Assignment operator]" << std::endl;
	std::swap(num, param.num);
	return (*this);
}

void Fixed::setRawBits(int const raw) {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "setRawBits CALLED" << std::endl;
	num = raw;
}

int Fixed::getRawBits(void) const {
	std::cout << "Fixed Point"; std::cout.width(30); std::cout << "getRawBits CALLED" << std::endl;
	return (num);
}

float Fixed::toFloat(void) const {
	return ((float)num / 256);
}

int	Fixed::toInt(void) const {
	return (num>>8);
}

std::ostream& operator<<(std::ostream& s, const Fixed& fn) {
	s << fn.toFloat();
	return (s);
}

bool Fixed::operator> (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15);std::cout << num << " > " << fp.num << "[operator>]" << std::endl;
	return (num > fp.num);
}

bool Fixed::operator< (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " < " << fp.num << "[operator<]" << std::endl;
	return (num < fp.num);
}

bool Fixed::operator== (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " == " << fp.num << "[operator==]" << std::endl;
	return (fp.num == num);
}

bool Fixed::operator!= (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " != " << fp.num << "[operator!=]" << std::endl;
	return (fp.num != num);
}

bool Fixed::operator<= (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " <= " << fp.num << "[operator<=]" << std::endl;
	return (*this == fp || *this < fp);
}

bool Fixed::operator>= (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " >= " << fp.num << "[operator>=]" << std::endl;
	return (*this == fp || *this > fp);
}

Fixed Fixed::operator+ (const Fixed& fp) const
{
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " + " << fp.num << "[operator+]" << std::endl;
	Fixed new_fp;

	new_fp.setRawBits(num + fp.num);
	return(new_fp);
}

Fixed Fixed::operator- (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " - " << fp.num << "[operator-]" << std::endl;
	Fixed new_fp;

	new_fp.setRawBits(num - fp.num);
	return (new_fp);
}

Fixed Fixed::operator* (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " * " << fp.num << "[operator*]" << std::endl;
	return (Fixed(this->toFloat() * fp.toFloat()));
}

Fixed Fixed:: operator/ (const Fixed& fp) const {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << num << " / " << fp.num << "[operator/]" << std::endl;
	return (Fixed(this->toFloat() / fp.toFloat()));
}

Fixed& Fixed::operator++(void) {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << "[++operator]" << std::endl;
	++num;
	return (*this);
}

Fixed Fixed::operator++(int) {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << "[operator++]" << std::endl;
	num++;
	return (*this);
}

Fixed& Fixed::operator--(void) {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << "[--operator]" << std::endl;
	--num;
	return (*this);
}

Fixed Fixed::operator--(int) {
	std::cout << "Fixed Point"; std::cout.width(15); std::cout << "[operator--]" << std::endl;
	num--;
	return (*this);
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
