//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SHRUBBERYCREATIONFORM_HPP__
# define __SHRUBBERYCREATIONFORM_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include "Form.hpp"

class Form;

class ShrubberyCreationForm: public Form {
	std::string const target;
	public:
		ShrubberyCreationForm();
		ShrubberyCreationForm(const std::string&);
		ShrubberyCreationForm(const ShrubberyCreationForm&);
		~ShrubberyCreationForm();
		ShrubberyCreationForm&	operator= (const ShrubberyCreationForm&);
		virtual void execute(Bureaucrat const & executor) const throw (std::exception);
};

#endif
