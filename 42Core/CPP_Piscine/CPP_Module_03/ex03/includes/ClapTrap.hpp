//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLAPTRAP_HPP__
# define __CLAPTRAP_HPP__

#include <iostream>

class ClapTrap {
	std::string	name;
	int		hp;
	int		ep;
	int		attack_dmg;
	public:
		ClapTrap();
		ClapTrap(std::string);
		ClapTrap(std::string, int, int, int);
		virtual ~ClapTrap();
		virtual ClapTrap&	operator= (ClapTrap&);
		virtual void		attack(const std::string&);
		void		takeDamage(unsigned int);
		void		beRepaired(unsigned int);
		void		useEnergy();
		bool		canPerform();
		virtual const std::string&	getName();
		int	getAtackDmg();
};

#endif

