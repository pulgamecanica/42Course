//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm(): Form("ShrubberyCreation", 145, 137), target("") {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& param): Form("ShrubberyCreation", 145, 137), target(param.target) {}

ShrubberyCreationForm::ShrubberyCreationForm(const std::string& str): Form("ShrubberyCreation", 145, 137), target(str) {}

ShrubberyCreationForm::~ShrubberyCreationForm() {std::cout << "ShrubberyCreationForm" << " destroyed" << std::endl;}

ShrubberyCreationForm& ShrubberyCreationForm::operator= (const ShrubberyCreationForm& param) {
	Form::operator=(param);
	return (*this);
}

void ShrubberyCreationForm::execute(Bureaucrat const & executor) const throw (std::exception) {
	checkPermissions(executor);
	std::ofstream file;
	file.open(target + "_shrubbery");
	if (file.is_open())
	{
		file << "               ,@@@@@@@,\n       ,,,.   ,@@@@@@/@@,  .oo8888o.\n    ,&%%&%&&%,@@@@@/@@@@@@,8888\\88/8o\n   ,%&\\%&&%&&%,@@@\\@@@/@@@88\\88888/88'\n   %&&%&%&/%&&%@@\\@@/ /@@@88888\\88888'\n   %&&%/ %&%%&&@@\\ V /@@' `88\\8 `/88'\n   `&%\\ ` /%&'    |.|        \\ '|8'\n       |o|        | |         | |\n       |.|        | |         | |\npulga\\/._\\//_/__/  ,\\_//__\\/.  \\_//__/_\n" << std::endl;
		file.close();
	}
	else
		std::cout << "Unable to open file" << std::endl;
}
