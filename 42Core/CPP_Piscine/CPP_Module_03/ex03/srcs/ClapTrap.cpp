//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "ClapTrap.hpp"

ClapTrap::ClapTrap() {
	std::cout << "ClapTrap created from default constructor" << std::endl;
	name = "";
	hp = 10;
	ep = 10;
	attack_dmg = 0;
}

ClapTrap::ClapTrap(std::string str)
{
	std::cout << "ClapTrap *" << str << "* created from constructor[str&]" << std::endl;
	name = str;
	hp = 10;
	ep = 10;
	attack_dmg = 0;
}

ClapTrap::ClapTrap(std::string str, int a, int b, int c)
{
	std::cout << "ClapTrap *" << str << "* created from constructor[str&, int, int, int]" << std::endl;
	name = str;
	hp = a;
	ep = b;
	attack_dmg = c;
}

ClapTrap::~ClapTrap() {
	std::cout << "ClapTrap *" << name << "* Destroyed" << std::endl;
}

ClapTrap& ClapTrap::operator= (ClapTrap& param) {
	std::cout << "ClapTrap *" << param.name << "* copy assignment operator" << std::endl;
        name = param.name;
	hp = param.hp;
	ep = param.ep;
	attack_dmg = param.attack_dmg;
	return (*this);
}

void ClapTrap::attack(const std::string& target) {
	std::cout << "[attack(" << target << ")]";std::cout.width(20);
	if (canPerform())
	{
		std::cout << "ClapTrap *" << name << "* attacks! " << target << " lost " << attack_dmg << " hit points... RIP X(" << std::endl;
		ep--;
	}
	else
		std::cout << "ClapTrap *"<< name << "* has no energy left... :O" << std::endl;
}

void ClapTrap::takeDamage(unsigned int amount) {
	std::cout << "[takeDamage(" << amount << ")]";std::cout.width(20);
	if (canPerform())
	{
		hp -= amount;
		if (hp < 0)
			hp = 0;
		std::cout << "ClapTrap *" << name << "* takes " << amount << " damage hp(" << hp << ")" << std::endl;
	}
	else
		std::cout << "ClapTrap *" << name << "* has no energy left... :O" << std::endl;
}

void ClapTrap::beRepaired(unsigned int amount) {
	std::cout << "[beRepaired(" << amount << ")]";std::cout.width(20);
	if (canPerform())
	{
		hp += amount;
		std::cout << "ClapTrap *" << name << "* repaired " << amount << " hit points hp(" << hp << ")" << std::endl;
		ep--;
	}
	else
		std::cout << "ClapTrap *" << name << "* has no energy left... :O" << std::endl;
}

void ClapTrap::useEnergy() {
	if (ep < 0)
		ep--;
}

bool ClapTrap::canPerform() {
	return (hp > 0 && ep > 0);
}
int ClapTrap::getAtackDmg() {return (attack_dmg);};

const std::string& ClapTrap::getName() {return (name);};
