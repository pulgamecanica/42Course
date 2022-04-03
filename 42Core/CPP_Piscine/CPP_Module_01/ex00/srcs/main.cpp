//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "braiiiiiiinnnzzzZ.hpp"

Zombie* newZombie(std::string name)
{
	Zombie *z;

	z = new Zombie(name);
	return(z);
}

void randomChump(std::string name)
{
	Zombie z(name);

	z.announce();
}

int	main(void)
{
	Zombie *z;

	randomChump("LOLILOOOL");
	z = newZombie("Crazy zombie");
	z->announce();

	//Zombie z2 = *z; // ASIGN OPERATOR WORKS :D
	//z2.announce();

	delete(z);
	return (0);
}
