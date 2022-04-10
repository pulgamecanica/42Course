//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __SCAVTRAP_HPP__
# define __SCAVTRAP_HPP__

#include "ClapTrap.hpp"

class ScavTrap: virtual public ClapTrap {
	public:
		ScavTrap();
		ScavTrap(std::string);
		ScavTrap(ScavTrap&);
		~ScavTrap();
		ScavTrap&	operator= (ScavTrap&); // This copy assignment I like a lot :D No need to create nothing only pass references
		void		attack(const std::string&);
		void		guardGate();
};

#endif

