//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FRAGTRAP_HPP__
# define __FRAGTRAP_HPP__

#include "ClapTrap.hpp"

class FragTrap: public ClapTrap {
	public:
		FragTrap();
		FragTrap(std::string);
		FragTrap(FragTrap&);
		~FragTrap();
		FragTrap&	operator= (FragTrap&);
		void	highFivesGuys();
};

#endif