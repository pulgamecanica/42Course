//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "now_we_are_talking.hpp"

int	main(void)
{
	{
		std::cout << "REGULAR TESTS" << std::endl;
		Fixed a;
		Fixed const b( Fixed( 5.05f ) * Fixed( 2 ) );
		std::cout << a << std::endl;
		std::cout << ++a << std::endl;
		std::cout << a << std::endl;
		std::cout << a++ << std::endl;
		std::cout << a << std::endl;
		std::cout << ++a << std::endl;
		std::cout << --a << std::endl;
		std::cout << a << std::endl;
		std::cout << a-- << std::endl;
		std::cout << a << std::endl;
		std::cout << b << std::endl;
		std::cout << Fixed::max( a, b ) << std::endl;
		std::cout << Fixed::min( a, b ) << std::endl;
		std::cout << "END REGULAR TESTS" << std::endl;
	}
	{
		Fixed a;
		Fixed const b( 10 );
		Fixed c;
		Fixed const d( 5.1f );
		Fixed const e( b + d );
		a = Fixed( 1234.4321f );
		c = a * b;
		std::cout << std::endl << "a is " << a << std::endl;
		std::cout << "b is " << b << std::endl;
		std::cout << "c is " << c << std::endl;
		std::cout << "d is " << d << std::endl;
		std::cout << "e is " << e << std::endl << std::endl;
		std::cout << "a is " << a.toInt() << " as integer" << std::endl;
		std::cout << "b is " << b.toInt() << " as integer" << std::endl;
		std::cout << "c is " << c.toInt() << " as integer" << std::endl;
		std::cout << "d is " << d.toInt() << " as integer" << std::endl;
		std::cout << "e is " << e.toInt() << " as integer" << std::endl << std::endl;
		std::cout << "a is " << a.toFloat() << " as float" << std::endl;
        std::cout << "b is " << b.toFloat() << " as float" << std::endl;
    	std::cout << "c is " << c.toFloat() << " as float" << std::endl;
        std::cout << "d is " << d.toFloat() << " as float" << std::endl;
		std::cout << "e is " << e.toFloat() << " as float" << std::endl << std::endl;
		std::cout << "a < b\n" << ((a < b) ? "YES" : "NO") << std::endl;
		std::cout << "a > b\n" << ((a > b) ? "YES" : "NO") << std::endl;
		std::cout << "b >= d\n" << ((b >= d) ? "YES" : "NO") << std::endl;
	}
	return 0;
}
