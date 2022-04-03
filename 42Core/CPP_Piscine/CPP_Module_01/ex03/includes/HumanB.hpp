//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HUMANB_HPP__
# define __HUMANB_HPP__

#include "Weapon.hpp"

class HumanB {
	std::string name;
	Weapon*		w;
	public:
		HumanB(std::string);
		~HumanB();
		void	attack();
		void	setWeapon(Weapon&);
};

#endif
