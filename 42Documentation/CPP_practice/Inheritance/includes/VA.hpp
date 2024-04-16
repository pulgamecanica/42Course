//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VA_HPP__
# define __VA_HPP__

#include <iostream>

#include "VMiddle.hpp"

class VA: public VMiddle {
	public:
		VA();
		VA(const VA&) = delete;
		~VA();
		VA&	operator= (const VA&) = delete; // const for safety... not super nesessary
		void msg();
};

std::ostream&	operator<<(std::ostream&, const VA&);

#endif

