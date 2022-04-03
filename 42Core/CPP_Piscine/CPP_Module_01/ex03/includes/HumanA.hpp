//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HUMANA_HPP__
# define __HUMANA_HPP__

#include "Weapon.hpp"

class HumanA {
	std::string name;
	Weapon&		w;
	public:
		HumanA(std::string, Weapon&);
		~HumanA();
		void	attack();
};

#endif
