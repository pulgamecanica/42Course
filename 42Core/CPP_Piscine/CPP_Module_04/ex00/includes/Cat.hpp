//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CAT_HPP__
# define __CAT_HPP__

#include <iostream>
#include "Animal.hpp"

class Cat: virtual public Animal {
	public:
		Cat();
		Cat(const Cat&);
		~Cat();
		Cat&	operator= (const Cat&);
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const Cat&);

#endif

