//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm(): Form("RobotomyRequestForm", 72, 45), target("") {}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& param): Form("RobotomyRequestForm", 72, 45), target(param.target) {}

RobotomyRequestForm::RobotomyRequestForm(const std::string& str): Form("RobotomyRequestForm", 72, 45), target(str) {}

RobotomyRequestForm::~RobotomyRequestForm() {std::cout << "RobotomyRequestForm" << " destroyed" << std::endl;}

RobotomyRequestForm& RobotomyRequestForm::operator= (const RobotomyRequestForm& param) {
	Form::operator=(param);
	return (*this);
}

void RobotomyRequestForm::execute(Bureaucrat const & executor) const throw (std::exception) {
	checkPermissions(executor);
	short int prob(rand() % 2);
	if (prob % 2)
		std::cout << target << " has been robotomized successfully" << std::endl;
	else
		std::cout << target << " robotomization failed :(" << std::endl;
}
