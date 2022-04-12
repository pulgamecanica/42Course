//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DOG_HPP__
# define __DOG_HPP__

#include <iostream>
#include "Animal.hpp"

class Dog: virtual public Animal {
	public:
		Dog();
		Dog(const Dog&);
		~Dog();
		Dog&	operator= (const Dog&);
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const Dog&);

#endif

