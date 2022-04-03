//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __HUMANA_HPP__
# define __HUMANA_HPP__

class HumanA {
	public:
		HumanA();			// Constructor
		~HumanA();			// Destructor
		HumanA	operator= (HumanA);	// Assign Operatior
		void	print();		// Class String representation
		void	set();			// Setter
};

#endif

