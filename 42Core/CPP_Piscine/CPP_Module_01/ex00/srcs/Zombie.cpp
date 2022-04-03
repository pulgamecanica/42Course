//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Zombie.hpp"

Zombie::Zombie(std::string str) {name = str;}

Zombie::~Zombie() {std::cout << name << " destroyed." << std::endl;}

Zombie Zombie::operator= (Zombie param) {
	Zombie new_obj(param.name);
	return (new_obj);
}

void	Zombie::announce() {std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;}