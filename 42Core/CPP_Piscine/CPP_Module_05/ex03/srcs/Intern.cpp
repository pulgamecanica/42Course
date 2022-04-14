//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Intern.hpp"

const std::string Intern::forms[3] = {"shrubbery creation", "presidential pardon", "robotomy request"};

Intern::Intern() {}

Intern::Intern(const Intern& param) {
	(void)param;
}

Intern::~Intern() {
	std::cout << "Intern" << " destroyed" << std::endl;
}

Intern& Intern::operator= (const Intern& param) {
	(void)param;
	return (*this);
}

Form* Intern::makeForm(const std::string& name, const std::string& target) {
	int type;
	for (type = 0; type < 3; ++type)
	{
		if (name == forms[type])
			break;
	}
	switch (type) {
		case 0:
			std::cout << "Intern creates shrubbery creation form" << std::endl;
			return (new ShrubberyCreationForm(target));
		case 1:
			std::cout << "Intern creates presidential pardon form" << std::endl;
			return (new PresidentialPardonForm(target));
		case 2:
			std::cout << "Intern creates robotomy request form" << std::endl;
			return (new RobotomyRequestForm(target));
		default:
			std::cout << "Intern cannot create " << name << "form" << std::endl;
	}
	return (NULL);
}

std::ostream& operator<<(std::ostream& s, const Intern& param) {
	s << "Intern Object";
	(void)param;
	return (s);
}
