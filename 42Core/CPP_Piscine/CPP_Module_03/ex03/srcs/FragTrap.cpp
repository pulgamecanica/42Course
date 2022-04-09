//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "FragTrap.hpp"

FragTrap::FragTrap(): ClapTrap("", 100, 100, 30)  {
	std::cout << "FragTrap created from default constructor" << std::endl;
}

FragTrap::FragTrap(FragTrap& param): ClapTrap(param.getName(), 100, 100, 30) {
	std::cout << "FragTrap created from copy constructor" << std::endl;
}
FragTrap::FragTrap(std::string str): ClapTrap(str, 100, 100, 30) {
	std::cout << "FragTrap *" << str << "* created from constructor[str]" << std::endl;
}

FragTrap::~FragTrap() {
	std::cout << "FragTrap *" << getName() << "* Destroyed" << std::endl;
}

FragTrap& FragTrap::operator= (FragTrap& param) {
	std::cout << "FragTrap *" << param.getName() << "* copy assignment operator" << std::endl;
	ClapTrap::operator=(param);
	return (*this);
}

void FragTrap::highFivesGuys() {
	if (canPerform())
		std::cout << "FragTrap *" << getName() << "* is requesting for a HIGH_FIVE!!!" << std::endl;
	else
		std::cout << "FragTrap *" << getName() << "* has no energy left... No highFivesGuys :/" << std::endl;
}