//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "AMateria.hpp"

AMateria** AMateria::dropped = NULL;

AMateria::AMateria(std::string const &str): type(str) {
}

AMateria::~AMateria() {
	std::cout << "AMateria" << " destroyed" << std::endl;
}

// THIS METHOD IS NOT NECESSARY ALTHOUGH IT COULD BE USED IN CASE THE OBJECT DOESNT OVERWRITES IT
void AMateria::use(ICharacter& target)
{
	std::cout << "<AMateria> " << getType() << " * used against <name> *" << std::endl;
	(void)target;
}

std::string const & AMateria::getType() const {
	return (type);
}

std::ostream& operator<<(std::ostream& s, const AMateria& param) {
	s << "AMateria type: " << param.getType();
	return (s);
}

void AMateria::drop()
{
	AMateria **list;
	int i = 0;
	while (dropped != NULL && dropped[i] != NULL)
		if (dropped[i++] == this)
			return ;
	list = new AMateria*[i + 2];
	list[i + 1] = NULL;
	list[i] = this;
	for (int j = 0; j < i; j++)
		list[j] = dropped[j];
	delete (dropped);
	dropped = list;
}

void AMateria::clean() {
	std::cout << "Cleaning dropped items..." << std::endl;
	int i = 0;
	while (dropped != NULL && dropped[i] != NULL)
	{
		delete (dropped[i++]);
	}
	std::cout << "Finished Cleanning Dropped items.." << std::endl;
}
