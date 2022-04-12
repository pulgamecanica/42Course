//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ANIMAL_HPP__
# define __ANIMAL_HPP__

#include <iostream>

class Animal {
	protected:
		std::string type;
	public:
		Animal();
		Animal(std::string);
		Animal(const Animal&);
		virtual ~Animal();
		Animal&	operator= (const Animal&);
		std::string getType() const;
		virtual void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const Animal&);

#endif

