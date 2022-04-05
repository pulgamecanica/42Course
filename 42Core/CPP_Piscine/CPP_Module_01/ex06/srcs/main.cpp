//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "harlFilter.hpp"

int	main(int ac, char **av)
{
	Harl h;

	if (ac <= 1)
	{
		std::cout << "IMPOSSIBLE!!! Harl is not complaining???? what's this sourcery :O" << std::endl;
		return (0);
	}
	h.complain(av[1]);
	return (0);
}
