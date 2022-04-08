//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex00.hpp"

int	main(void)
{
	ClapTrap c1("Awesome Clap");
	ClapTrap c2("Zombie Trap");

	c1.attack("Villan");
	c2.takeDamage(6);
	c1.takeDamage(9);
	c2.beRepaired(20);
	c1.attack("Comunity");
	c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);
	c1.attack("Villagers");
	c1.takeDamage(5);
	c1.beRepaired(1);c1.beRepaired(1);c1.beRepaired(1);
	c1.attack("Castle");
	return (0);
}
