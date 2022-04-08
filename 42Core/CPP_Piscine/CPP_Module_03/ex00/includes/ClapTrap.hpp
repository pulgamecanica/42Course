//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __CLAPTRAP_HPP__
# define __CLAPTRAP_HPP__

class ClapTrap {
	public:
		ClapTrap();			// Constructor
		~ClapTrap();			// Destructor
		ClapTrap	operator= (ClapTrap);	// Assign Operatior
		void	print();		// Class String representation
		void	set();			// Setter
};

#endif

