//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm(): Form("PresidentialPardonForm", 25, 5), target("") {}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& param): Form("PresidentialPardonForm", 25, 5), target(param.target) {}

PresidentialPardonForm::PresidentialPardonForm(const std::string& str): Form("PresidentialPardonForm", 25, 5), target(str) {}

PresidentialPardonForm::~PresidentialPardonForm() {std::cout << "PresidentialPardonForm" << " destroyed" << std::endl;}

PresidentialPardonForm& PresidentialPardonForm::operator= (const PresidentialPardonForm& param) {
	Form::operator=(param);
	return (*this);
}

void PresidentialPardonForm::execute(Bureaucrat const & executor) const throw (std::exception) {
	checkPermissions(executor);
	std::cout << target << " has been pardoned by Zaphod Beeblebrox" << std::endl;
}
