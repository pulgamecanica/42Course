//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __ANIMAL_HPP__
# define __ANIMAL_HPP__

class Animal {
	public:
		Animal();			// Constructor
		~Animal();			// Destructor
		Animal	operator= (Animal);	// Assign Operatior
		void	print();		// Class String representation
		void	set();			// Setter
};

#endif

