//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex01.hpp"

int	main(void)
{
	ClapTrap c("Awesome Clap");
	ScavTrap s1("Scav Scum");
	ScavTrap s2("Good Boy");
	ScavTrap s3;

	
	c.takeDamage(5);
	c.attack("Wild Mad Dog");
	s1.guardGate();
	s1.attack("CowMan");
	s2.attack("KIKI");
	s2.attack("Bone");
	s2.takeDamage(50);
	s2.takeDamage(50);
	s2.beRepaired(10);
	s1.beRepaired(40);
	s3 = s1;
	s3.takeDamage(50);
	s3.beRepaired(10);
	s3.takeDamage(120);
	s3.takeDamage(10);
	s1.takeDamage(10);
	s1.beRepaired(10);

	return (0);
}
