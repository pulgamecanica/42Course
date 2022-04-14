//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "At_least_this_beats_coffee-making.hpp"

int	main(void)
{
	srand(time(0));
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
	}
	{
		Bureaucrat b("GOD", 1);
		Intern someRandomIntern;
		Form* rrf;
		rrf = someRandomIntern.makeForm("robotomy request", "Bender");
		if (rrf)
		{
			rrf->beSigned(b);
			b.executeForm(*rrf);
			delete (rrf);
		}
	}
	{
		Bureaucrat b("EU", 1);
		Intern someRandomIntern;
		Form* rrf;
		rrf = someRandomIntern.makeForm("presidential pardon", "ALIENS");
		if (rrf)
		{
			rrf->beSigned(b);
			b.executeForm(*rrf);
			delete (rrf);
		}
	}
	{
		Bureaucrat b("Andre", 1);
		Intern someRandomIntern;
		Form* rrf;
		rrf = someRandomIntern.makeForm("shrubbery creation", "Lizard the tree wizard's garden");
		if (rrf)
		{
			rrf->beSigned(b);
			b.executeForm(*rrf);
			delete (rrf);
		}
	}
	{
		Bureaucrat b("FLY", 1);
		Intern someRandomIntern;
		Form* rrf;
		rrf = someRandomIntern.makeForm("WRONG", "LOL");
		if (rrf)
		{
			rrf->beSigned(b);
			b.executeForm(*rrf);
			delete (rrf);
		}
	}
	return (0);
}
