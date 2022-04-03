//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HUMANB_HPP__
# define __HUMANB_HPP__

class HumanB {
	public:
		HumanB();			// Constructor
		~HumanB();			// Destructor
		HumanB	operator= (HumanB);	// Assign Operatior
		void	print();		// Class String representation
		void	set();			// Setter
};

#endif

