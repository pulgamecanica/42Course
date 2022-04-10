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
		virtual ~DiamondTrap();
		DiamondTrap	operator= (DiamondTrap);
		void	whoAmI();
};

#endif

