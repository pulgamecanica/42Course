//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ROBOTOMYREQUESTFORM_HPP__
# define __ROBOTOMYREQUESTFORM_HPP__

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <exception>
#include "Form.hpp"

class Form;

class RobotomyRequestForm: public Form {
	std::string const target;
	public:
		RobotomyRequestForm();
		RobotomyRequestForm(const RobotomyRequestForm&);
		RobotomyRequestForm(const std::string&);
		~RobotomyRequestForm();
		RobotomyRequestForm&	operator= (const RobotomyRequestForm&);
		virtual void execute(Bureaucrat const & executor) const throw (std::exception);

};

#endif
