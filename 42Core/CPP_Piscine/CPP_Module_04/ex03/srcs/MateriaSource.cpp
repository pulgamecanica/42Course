//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
	for (int i = 0; i < 4; ++i)
		list[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource& param) {
	for (int i = 0; i < 4; ++i)
		delete (list[i]);
	for (int i = 0; i < 4; ++i)
		list[i] = param.list[i];
}

MateriaSource::~MateriaSource() {
	std::cout << "MateriaSource" << " destroyed" << std::endl;
	for (int i = 0; i < 4; ++i)
		delete (list[i]);
}

MateriaSource& MateriaSource::operator= (const MateriaSource& param) {
	delete[] (*list);
	for (int i  = 0; i < 4; i++)
		list[i] = param.list[i];
	return (*this);
}

void MateriaSource::learnMateria(AMateria* param) {
	for (int i  = 0; i < 4; i++)
	{
		if (list[i] == NULL)
		{
			list[i] = param;
			return ;
		}
	}
}

AMateria* MateriaSource::createMateria(std::string const & type) {
	for (int i  = 0; i < 4; i++)
	{
		if (list[i] != NULL && type == list[i]->getType())
		{
			return (list[i]->clone());
		}
	}
	return (0);
}
