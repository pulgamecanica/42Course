//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CAT_HPP__
# define __CAT_HPP__

#include <iostream>
#include "Animal.hpp"
#include "Brain.hpp"

class Cat: public Animal {
	Brain* b;
	public:
		Cat();
		Cat(const Cat&);
		virtual ~Cat();
		Cat&	operator= (const Cat&);
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const Cat&);

#endif

