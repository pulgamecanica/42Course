//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MATERIASOURCE_HPP__
# define __MATERIASOURCE_HPP__

#include <iostream>
#include "IMateriaSource.hpp"

class MateriaSource: public IMateriaSource {
	AMateria *list[4];
	public:
		MateriaSource();
		MateriaSource(const MateriaSource&);
		~MateriaSource();
		MateriaSource&	operator= (const MateriaSource&);
		virtual void learnMateria(AMateria*);
		virtual AMateria* createMateria(std::string const & type);
};

#endif

