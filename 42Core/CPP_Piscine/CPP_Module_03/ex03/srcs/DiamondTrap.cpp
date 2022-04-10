//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap(): ClapTrap(), ScavTrap(), FragTrap() {
	std::cout << "DiamondTrap created from default constructor" << std::endl;
	name = "";
}

DiamondTrap::DiamondTrap(std::string str): ClapTrap(str + "_clap_name", 100, 50, 30), ScavTrap(), FragTrap() {
	name = str;
	std::cout << "DiamondTrap created from constructor[str]" << std::endl;
}

DiamondTrap::DiamondTrap(DiamondTrap& param): ClapTrap(param.getName() + "clap_name", 100, 50, 30), ScavTrap(), FragTrap() {
	name = param.name;
	std::cout << "DiamondTrap created from copy constructor" << std::endl;
}

DiamondTrap::~DiamondTrap() {
	std::cout << "DiamondTrap *" << name <<  "* destroyed." << std::endl;
}

DiamondTrap DiamondTrap::operator= (DiamondTrap param) {
	name = param.name;
	ClapTrap::operator=(param);
	return (*this);
}

void	DiamondTrap::whoAmI() {
	if (canPerform())
		std::cout << "[whoAmI ???] DiamondTrap(" << name <<  ") ClapTrap(" << getName() << ")" << std::endl;
	else
		std::cout << "DiamondTrap *" << name << "* has no energy left... can't tell who is who :(" << std::endl;
}
