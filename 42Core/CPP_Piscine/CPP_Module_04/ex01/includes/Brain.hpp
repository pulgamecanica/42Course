//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BRAIN_HPP__
# define __BRAIN_HPP__

#include <iostream>
#include <cmath>

class Brain {
	std::string ideas[100];
	public:
		Brain();
		Brain(const Brain&);
		virtual ~Brain();
		Brain&	operator= (const Brain&); // const for safety... not super nesessary
		std::string getIdea() const;
};

std::ostream&	operator<<(std::ostream&, const Brain&);

#endif

