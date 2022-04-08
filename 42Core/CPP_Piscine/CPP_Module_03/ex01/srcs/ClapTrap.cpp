//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ClapTrap.hpp"

ClapTrap::ClapTrap() {
	std::cout << "ClapTrap created from default constructor" << std::endl;
	name = "";
	hp = 100;
	ep = 50;
	attack_dmg = 20;
}

ClapTrap::ClapTrap(std::string str)
{
	std::cout << "ClapTrap *" << str << "* created from constructor[str&]" << std::endl;
	name = str;
	hp = 100;
	ep = 50;
	attack_dmg = 20;
}

ClapTrap::~ClapTrap() {
	std::cout << "ClapTrap *" << name << "* Destroyed" << std::endl;
}

ClapTrap ClapTrap::operator= (ClapTrap& param) {
        name = param.name;
	hp = param.hp;
	ep = param.ep;
	attack_dmg = param.attack_dmg;
	return (*this);
}

void ClapTrap::attack(const std::string& target) {
	std::cout << "[attack(" << target << ")]";std::cout.width(20);
	if (hp > 0 && ep > 0)
	{
		std::cout << "ClapTrap *" << name << "* attacks! " << target << " lost " << attack_dmg << " hit points... RIP X(" << std::endl;
		ep--;
	}
	else
		std::cout << "ClapTrap *"<< name << "* has no energy left... :O" << std::endl;
}

void ClapTrap::takeDamage(unsigned int amount) {
	hp -= amount;
	if (hp < 0)
		hp = 0;
	std::cout << "ClapTrap " << name << " takes " << amount << " damage hp(" << hp << ")" << std::endl;
}

void ClapTrap::beRepaired(unsigned int amount) {
	std::cout << "ClapTrap [beRepaired(" << amount << ")]";std::cout.width(20);
	if (hp > 0 && ep > 0)
	{
		hp += amount;
		std::cout << name << " repaired " << amount << " hit points hp(" << hp << ")" << std::endl;
		ep--;
	}
	else
		std::cout << name << " has no energy left... :O" << std::endl;
}

std::string& ClapTrap::getName() {return (name);};
