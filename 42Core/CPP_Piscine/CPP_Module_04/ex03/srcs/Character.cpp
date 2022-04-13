//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "Character.hpp"

Character::Character(): name("Default") {
	for (int i  = 0; i < 4; i++)
		list[i] = NULL;
}

Character::Character(const Character& param): name(param.getName()) {
	for (int i  = 0; i < 4; i++)
		list[i] = param.list[i];
}

Character::Character(std::string const str): name(str) {
	for (int i  = 0; i < 4; i++)
		list[i] = NULL;
}

Character::~Character() {
	for (int i  = 0; i < 4; i++)
		if (list[i] != NULL)
			delete (list[i]);
	std::cout << "Character" << " destroyed" << std::endl;
}

Character& Character::operator= (const Character& param) {
	name = param.getName();
	delete[] (*list);
	for (int i  = 0; i < 4; i++)
		list[i] = param.list[i];
	return (*this);
}

std::ostream& operator<<(std::ostream& s, const Character& param) {
	s << param.getName();
	return (s);
}

std::string const & Character::getName() const {
	return (name);
}

void Character::equip(AMateria* m) {
	for (int i  = 0; i < 4; i++)
	{
		if (list[i] == NULL)
		{
			list[i] = m;
			break;
		}
	}
}

void Character::unequip(int idx) {
	if (idx < 0 || idx >= 4)
		return;
	AMateria *ptr = list[idx];
	if (ptr == NULL)
		return;
	ptr->drop();
	list[idx] = NULL;
}

void Character::use(int idx, ICharacter& target) {
	if (idx < 0 || idx >= 4)
		return;
	(void)target;
	if (list[idx] != NULL)
		list[idx]->use(target);
}
