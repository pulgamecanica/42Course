//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ZOMBIE_HPP__
# define __ZOMBIE_HPP__

#include <iostream>
#include <string>

class Zombie {
	std::string	name;
	public:
		Zombie();
		Zombie(std::string);
		~Zombie();
		Zombie	operator= (Zombie);
		void	announce();
		void	set(std::string);
};

#endif

