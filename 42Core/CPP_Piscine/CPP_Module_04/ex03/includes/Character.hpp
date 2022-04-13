//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CHARACTER_HPP__
# define __CHARACTER_HPP__

#include <iostream>
#include "ICharacter.hpp"

class Character: public ICharacter {
	AMateria *list[4];
	std::string name;
	public:
		Character();
		Character(std::string const);
		Character(const Character&);
		~Character();
		Character&	operator= (const Character&);
		std::string const & getName() const;
		void equip(AMateria* m);
		void unequip(int idx);
		void use(int idx, ICharacter& target);
};

std::ostream&	operator<<(std::ostream&, const Character&);

#endif

