//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ex03.hpp"

int	main(void)
{
	DiamondTrap dt("Evil Cat");

	dt.attack("LULA");
	dt.guardGate();
	dt.takeDamage(50);
	dt.beRepaired(90);
	dt.highFivesGuys();
	dt.whoAmI();
	dt.takeDamage(300);
	dt.whoAmI();

	// ScavTrap s1("Scav Scum");
	// FragTrap f1("Good Boy");
	// FragTrap f2;


	// s1.guardGate();
	// s1.attack("CowMan");
	// s1.beRepaired(40);

	// f1.attack("KIKI");
	// f1.attack("Bone");
	// f1.takeDamage(50);
	// f2 = f1;
	// f2.beRepaired(10);
	// f1.takeDamage(50);
	// f2.beRepaired(10);
	// f2.beRepaired(200);
	// f2.takeDamage(120);
	// f2.takeDamage(10);
	// f2.takeDamage(10);
	// f2.beRepaired(10);
	// f2.highFivesGuys();
	// f2.takeDamage(220);
	// f2.highFivesGuys();
	return (0);
}
