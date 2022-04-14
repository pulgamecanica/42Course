//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Mommy_when_I_grow_up_I_want_to_be_a_bureaucrat.hpp"

int	main(void)
{
	{
		try
		{
			Bureaucrat b("Test", 1);
			b++;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Bureaucrat b("Test", 150);
			--b;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Bureaucrat b("Test", -10);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Bureaucrat b("Test", 155);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Bureaucrat b1("Pulga", 2);
			Bureaucrat b2("Artemiza", 150);
			Bureaucrat b3("Apolo", 42);
			Bureaucrat b4(b1);

			std::cout << b1 << std::endl;
			std::cout << b2 << std::endl;
			std::cout << b3 << std::endl;
			std::cout << b4 << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return (0);
}
