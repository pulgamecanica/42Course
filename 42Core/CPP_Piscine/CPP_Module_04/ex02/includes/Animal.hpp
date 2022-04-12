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
		Animal(const std::string);
		Animal(const Animal&);
		virtual ~Animal();
		virtual Animal&	operator= (const Animal&);
		virtual void makeSound() const = 0;
		std::string getType() const;
};

#endif

