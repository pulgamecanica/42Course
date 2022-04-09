//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ScavTrap.hpp"

ScavTrap::ScavTrap():  ClapTrap("", 100, 50, 20) {
	std::cout << "ScavTrap created from default constructor" << std::endl;
}

ScavTrap::ScavTrap(ScavTrap& st): ClapTrap(st.getName(), 100, 50, 20) {
	std::cout << "ScavTrap created from copy constructor" << std::endl;
}

ScavTrap::ScavTrap(std::string str): ClapTrap(str, 100, 50, 20) {
	std::cout << "ScavTrap *" << str << "* created from constructor[str]" << std::endl;
}

ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap *" << getName() << "* Destroyed" << std::endl;
}

ScavTrap& ScavTrap::operator= (ScavTrap& param) {
	std::cout << "ScavTrap *" << param.getName() << "* copy assignment operator" << std::endl;
	ClapTrap::operator=(param); // Calling parent copy assignment operator :D
	return (*this);
}

void ScavTrap::guardGate() {
	std::cout << "ScavTrap *" << getName() << "* is now in Gate Keeper Mode" << std::endl; 
}

void ScavTrap::attack(const std::string& target) {
	std::cout << "[attack(" << target << ")]";std::cout.width(20);
	if (canPerform())
	{
		std::cout << "ScavTrap *" << getName() << "* attacks! " << target << " lost " << getAtackDmg() << " hit points... RIP X(" << std::endl;
		useEnergy();
	}
	else
		std::cout << "ScavTrap *"<< getName() << "* has no energy left... :O" << std::endl;
}