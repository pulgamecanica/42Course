//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CURE_HPP__
# define __CURE_HPP__

#include <iostream>
#include "AMateria.hpp"
#include "ICharacter.hpp"

class Cure: public AMateria {
	public:
		Cure();
		Cure(const Cure&);
		~Cure();
		Cure&	operator= (const Cure&); // const for safety... not super nesessary
		virtual AMateria* clone() const;
		virtual void use(ICharacter& target);
};

std::ostream&	operator<<(std::ostream&, const Cure&);

#endif
