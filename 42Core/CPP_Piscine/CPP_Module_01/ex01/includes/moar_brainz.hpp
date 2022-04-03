//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MOAR_BRAINZ_HPP__
# define __MOAR_BRAINZ_HPP__

#include <iostream>
#include "Zombie.hpp"

Zombie* newZombie(std::string name);
Zombie* zombieHorde(int N, std::string name);
void randomChump( std::string name );

#endif
