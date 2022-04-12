//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WRONG_CAT_HPP__
# define __WRONG_CAT_HPP__

#include <iostream>
#include "WrongAnimal.hpp"

class WrongCat: public WrongAnimal {
	public:
		WrongCat();
		WrongCat(const WrongCat&);
		~WrongCat();
		WrongCat&	operator= (const WrongCat&);
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const WrongCat&);

#endif

