//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __PRESIDENTIALPARDONFORM_HPP__
# define __PRESIDENTIALPARDONFORM_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include "Form.hpp"

class Form;

class PresidentialPardonForm: public Form {
	std::string const target;
	public:
		PresidentialPardonForm();
		PresidentialPardonForm(const PresidentialPardonForm&);
		PresidentialPardonForm(const std::string&);
		~PresidentialPardonForm();
		PresidentialPardonForm&	operator= (const PresidentialPardonForm&);
		virtual void execute(Bureaucrat const & executor) const throw (std::exception);
};

#endif
