//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Form_up_maggots.hpp"

int	main(void)
{
	{
		try
		{
			Form f1("Test", 10, -1);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Form f1("Test", -10, 1);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Form f1("Test", 155, 1);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	{
		try
		{
			Form f1("Test", 10, 155);
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
			Form f1("War Declaration", 10, 1);
			Form f2("Freelance Form", 50, 50);
			Form f3("Website Form", 150, 10);

			std::cout << b1 << std::endl;
			std::cout << b2 << std::endl;
			std::cout << b3 << std::endl;
			std::cout << f1 << std::endl;
			std::cout << f2 << std::endl;
			std::cout << f3 << std::endl;
			f1.beSigned(b1);
			std::cout << f1 << std::endl;
			f3.beSigned(b3);
			f2.beSigned(b2);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
	return (0);
}
