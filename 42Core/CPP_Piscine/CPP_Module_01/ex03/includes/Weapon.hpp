//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __WEAPON_HPP__
# define __WEAPON_HPP__

class Weapon {
	public:
		Weapon();			// Constructor
		~Weapon();			// Destructor
		Weapon	operator= (Weapon);	// Assign Operatior
		void	print();		// Class String representation
		void	set();			// Setter
};

#endif

