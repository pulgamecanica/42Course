//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "moar_brainz.hpp"

int	main(void)
{
	Zombie *h1;
	Zombie *h2;
	std::string name1 = "Killers!";
	std::string name2 = "Good Boys!";

	h1 = zombieHorde(10, name1);
	for (int i = 0; i < 10; i++)
	{
		std::cout << i << "- ";
		h1[i].announce();
	}
	h2 = zombieHorde(3, name2);
	for (int i = 0; i < 3; i++)
	{
		std::cout << i << "- ";
		h2[i].announce();
	}

	delete[](h1);
	delete[](h2);
	return (0);
}
