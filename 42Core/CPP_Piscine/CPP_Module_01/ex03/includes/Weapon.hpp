//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WEAPON_HPP__
# define __WEAPON_HPP__

#include <string>
#include <iostream>

class Weapon {
	std::string	type;
	public:
		Weapon();
		Weapon(std::string);
		~Weapon();
		void	print();
		void	setType(std::string);
		const std::string& getType();
};

#endif
