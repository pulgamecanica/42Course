//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DIAMONDTRAP_HPP__
# define __DIAMONDTRAP_HPP__

#include "ClapTrap.hpp"
#include "ScavTrap.hpp"
#include "FragTrap.hpp"

class DiamondTrap: public ScavTrap, public FragTrap {
	std::string name;
	public:
		DiamondTrap();
		DiamondTrap(std::string);
		DiamondTrap(DiamondTrap&);
		~DiamondTrap();
		DiamondTrap	operator= (DiamondTrap);
		void	whoAmI();
		using ScavTrap::attack;
};

#endif

