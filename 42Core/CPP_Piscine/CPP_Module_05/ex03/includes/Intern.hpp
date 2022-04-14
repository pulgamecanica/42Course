//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __INTERN_HPP__
# define __INTERN_HPP__

#include <iostream>
#include <Form.hpp>
#include "ShrubberyCreationForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "RobotomyRequestForm.hpp"

class Form;
class ShrubberyCreationForm;
class PresidentialPardonForm;
class RobotomyRequestForm;

class Intern {
	static const std::string forms[3];
	public:
		Intern();
		Intern(const Intern&);
		~Intern();
		Form* makeForm(const std::string&, const std::string&);
		Intern&	operator= (const Intern&);
};

std::ostream&	operator<<(std::ostream&, const Intern&);

#endif

