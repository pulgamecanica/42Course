//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __FRAGTRAP_HPP__
# define __FRAGTRAP_HPP__

#include "ClapTrap.hpp"

class FragTrap: virtual public ClapTrap {
	public:
		FragTrap();
		FragTrap(std::string);
		FragTrap(FragTrap&);
		virtual ~FragTrap();
		virtual FragTrap&	operator= (FragTrap&);
		void	highFivesGuys();
};

#endif
