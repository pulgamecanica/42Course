//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ICE_HPP__
# define __ICE_HPP__

#include <iostream>
#include "AMateria.hpp"
#include "ICharacter.hpp"

class Ice: public AMateria {
	public:
		Ice();
		Ice(const Ice&);
		~Ice();
		Ice&	operator= (const Ice&); // const for safety... not super nesessary
		virtual AMateria* clone() const;
		virtual void use(ICharacter& target);
};

std::ostream&	operator<<(std::ostream&, const Ice&);

#endif
