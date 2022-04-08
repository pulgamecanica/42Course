//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ScavTrap.hpp"

ScavTrap::ScavTrap() {}

ScavTrap::ScavTrap(std::string str): ClapTrap(str) {
	std::cout << "ScavTrap *" << str << "* created from constructor[str]" << std::endl;
}
ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap *" << getName() << "* Destroyed" << std::endl;
}

ScavTrap ScavTrap::operator= (ScavTrap param) {
	ScavTrap new_obj;
	// TODO (Assignment operatior)
        (void)param;
	return (new_obj);
}

void ScavTrap::guardGate() {
	std::cout << "ScavTrap *" << getName() << "* is now in Gate Keeper Mode" << std::endl; 
}
