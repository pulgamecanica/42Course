//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HumanA.hpp"

HumanA::HumanA(std::string str, Weapon& wp): name(str), w(wp)
{
	;	
}

HumanA::~HumanA() {}

void	HumanA::attack() {
	std::cout << name << " attacks with their " << w.getType() << std::endl;
}
