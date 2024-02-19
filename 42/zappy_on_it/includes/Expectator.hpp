//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __EXPECTATOR_HPP__
# define __EXPECTATOR_HPP__

#include <iostream>

class Expectator {
	public:
		Expectator();
		Expectator(const Expectator&);
		~Expectator();
		Expectator&	operator= (const Expectator&); // const for safety... not super nesessary
};

std::ostream&	operator<<(std::ostream&, const Expectator&);

#endif

