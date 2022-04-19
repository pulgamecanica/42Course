//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "templates_basics.hpp"

int	main(void)
{
	{
		int a(10);
		int b(20);
		std::string s1;
		std::string s2;

		s1 = "Helloo";
		s2 = "World";
		std::cout << BLUE << "Swap Template" << std::endl;
		std::cout << YELLOW <<"Original" << std::endl;
		std::cout << WHITE << "a [" << a << "] b [" << b << "] s1 [" << s1 << "] s2 [" << s2 << "]" << std::endl;
		swap<int>(a, b);
		swap(s1, s2);
		std::cout << YELLOW << "Swapped" << std::endl;
		std::cout << WHITE << "a [" << a << "] b [" << b << "] s1 [" << s1 << "] s2 [" << s2 << "]" << ENDC << std::endl;
	}
	{
		int a(10);
		int b(20);
		float c(0.42);
		float d(4.2);

		std::cout << BLUE << "Min Template" << std::endl;
		std::cout << YELLOW <<"Original" << std::endl;
		std::cout << WHITE << "a [" << a << "] b [" << b << "] c [" << c << "] d [" << d << "]" << std::endl;
		std::cout << YELLOW << "MIN (a .. b & c .. d)" << std::endl;
		std::cout << WHITE << "min [" << ::min(a, b) << "] min [" << ::min(c, d) << "]" << ENDC << std::endl;
	}
	{
		int a(10);
		int b(20);
		double c(42);
		double d(42.42);

		std::cout << BLUE << "Max Template" << std::endl;
		std::cout << YELLOW <<"Original" << std::endl;
		std::cout << WHITE << "a [" << a << "] b [" << b << "] c [" << c << "] d [" << d << "]" << std::endl;
		std::cout << YELLOW << "MAX (a .. b & c .. d)" << std::endl;
		std::cout << WHITE << "max [" << max<int>(a, b) << "] max [" << ::max(c, d) << "]" << ENDC << std::endl;
	}
	{
		int a = 2;
		int b = 3;
		::swap( a, b );
		std::cout << "a = " << a << ", b = " << b << std::endl;
		std::cout << "min( a, b ) = " << ::min( a, b ) << std::endl;
		std::cout << "max( a, b ) = " << ::max( a, b ) << std::endl;
		std::string c = "chaine1";
		std::string d = "chaine2";
		::swap(c, d);
		std::cout << "c = " << c << ", d = " << d << std::endl;
		std::cout << "min( c, d ) = " << ::min( c, d ) << std::endl;
		std::cout << "max( c, d ) = " << ::max( c, d ) << std::endl;
	}
	return (0);
}
