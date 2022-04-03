//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "HumanB.hpp"

HumanB::HumanB(std::string str): name(str) {
	;
}

HumanB::~HumanB() {

}

void	HumanB::setWeapon(Weapon& wp) {
	w = &wp;
}

void	HumanB::attack() {
	std::cout << name << " attacks with their " << w->getType() << std::endl;
}
