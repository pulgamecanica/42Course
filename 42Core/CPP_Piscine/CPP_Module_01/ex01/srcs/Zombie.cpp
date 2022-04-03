//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Zombie.hpp"

Zombie::Zombie() {name = "";}

Zombie::Zombie(std::string str) {name = str;}

Zombie::~Zombie() {std::cout << name << " destroyed." << std::endl;}

Zombie Zombie::operator= (Zombie param) {
	this->name = param.name;
	return (*this);
}

void	Zombie::announce() {std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;}

void	Zombie::set(std::string str) {name = str;}

Zombie* newZombie(std::string name)
{
	Zombie *z;

	z = new Zombie(name);
	return(z);
}

void randomChump(std::string name)
{
	Zombie z(name);

	z.announce();
}

Zombie* zombieHorde(int N, std::string name)
{
	Zombie *horde;

	horde = new Zombie[N];
	for (int i = 0; i < N; i++)
	{
		horde[i].set(name);
	}
	return (horde);
}