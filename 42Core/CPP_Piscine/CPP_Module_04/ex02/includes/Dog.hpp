//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __DOG_HPP__
# define __DOG_HPP__

#include <iostream>
#include "Animal.hpp"
#include "Brain.hpp"

class Dog: public Animal {
	Brain* b;
	public:
		Dog();
		Dog(const Dog&);
		virtual ~Dog();
		Dog&	operator= (const Dog&);
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const Dog&);

#endif

