//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Span.hpp"

int RandomNumber () { return (std::rand()%100000); }

int	main(void)
{
	{
		Span s(5);
		Span s2;
		int myints[] = {32,71,12,45,26,80,53,33, 42, 0};
		std::vector<int> myvector (myints, myints+10); 

		s2.addNumbers(myvector);
		s.addNumber(42);
		s.addNumber(420);
		s.addNumber(402);
		s.addNumber(4);
		s.addNumber(2);
		std::cout << s;
		std::cout << s2;
	}
	{
		Span s(1);
		s.addNumber(42);
		try {
			std::cout << s.longestSpan() << std::endl;
		} catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}
		
		try {
			std::cout << s.shortestSpan() << std::endl;
		} catch (std::exception &e) {
			std::cout << e.what() << std::endl;
		}

		try {
			s.addNumber(24);
		} catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		Span sp = Span(5);
		sp.addNumber(6);
		sp.addNumber(3);
		sp.addNumber(17);
		sp.addNumber(9);
		sp.addNumber(11);
		std::cout << sp.shortestSpan() << std::endl;
		std::cout << sp.longestSpan() << std::endl;
	}
	{
		std::srand ( unsigned ( std::time(0) ) );
		std::vector<int> myvector (10000);
		Span s(10000);
		
		std::generate (myvector.begin(), myvector.end(), RandomNumber);
		s.addNumbers(myvector);
		std::cout << s;

		

	}
	return (0);
}
