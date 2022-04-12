//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WRONG_ANIMAL_HPP__
# define __WRONG_ANIMAL_HPP__

#include <iostream>

class WrongAnimal {
	protected:
		std::string type;
	public:
		WrongAnimal();
		WrongAnimal(std::string);
		WrongAnimal(const WrongAnimal&);
		~WrongAnimal();
		WrongAnimal&	operator= (const WrongAnimal&);
		std::string getType() const;
		void makeSound() const;
};

std::ostream&	operator<<(std::ostream&, const WrongAnimal&);

#endif

