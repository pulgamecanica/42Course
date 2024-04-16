//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __MIDDLE_HPP__
# define __MIDDLE_HPP__

#include <iostream>

#include "Base.hpp"

class Middle: public Base {
	protected:
		Middle();
	public:
		Middle(const Middle&) = delete;
		virtual ~Middle();
		Middle&	operator= (const Middle&) = delete; // const for safety... not super nesessary
		virtual void msg();
};

std::ostream&	operator<<(std::ostream&, const Middle&);

#endif

