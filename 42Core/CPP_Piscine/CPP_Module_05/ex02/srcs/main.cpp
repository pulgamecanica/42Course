//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "No_you_need_form_28B_not_28C.hpp"

int	main(void)
{
	srand(time(0));
	{
		try
		{
			Bureaucrat b1("Pulga", 2);
			Bureaucrat b2("Artemiza", 150);
			Bureaucrat b3("Apolo", 42);
			ShrubberyCreationForm f1("Office");
			PresidentialPardonForm f2("Jimmy Page");
			RobotomyRequestForm f3("LOL");

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
	{
		Bureaucrat b("Thor", 1);
		PresidentialPardonForm fpp("Evil Zoo");
		std::cout << b << std::endl;
		std::cout << fpp << std::endl;
		b.executeForm(fpp);
		try {
			fpp.beSigned(b);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		b.executeForm(fpp);
	}
	{
		Bureaucrat b("Yalena", 30);
		Bureaucrat b2("Noob", 150);
		ShrubberyCreationForm scf("SmallGarden");
		std::cout << b << std::endl;
		std::cout << b2 << std::endl;
		std::cout << scf << std::endl;
		try {
			scf.beSigned(b);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		b2.executeForm(scf);
		b.executeForm(scf);
	}
	{
		Bureaucrat b("Witcher", 30);
		Bureaucrat b2("Roach", 150);
		RobotomyRequestForm rrf("Witch");
		std::cout << b << std::endl;
		std::cout << b2 << std::endl;
		std::cout << rrf << std::endl;
		b2.executeForm(rrf);
		try {
			rrf.beSigned(b);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		b.executeForm(rrf);
		b.executeForm(rrf);
		b.executeForm(rrf);
		b.executeForm(rrf);
	}
	return (0);
}
