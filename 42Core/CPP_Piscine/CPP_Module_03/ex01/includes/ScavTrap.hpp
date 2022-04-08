//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCAVTRAP_HPP__
# define __SCAVTRAP_HPP__

#include "ClapTrap.hpp"

class ScavTrap: public ClapTrap {
	public:
		ScavTrap();
		ScavTrap(std::string);
		~ScavTrap();
		ScavTrap	operator= (ScavTrap);
		void		guardGate();
};

#endif

