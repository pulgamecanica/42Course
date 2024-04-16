//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __A_HPP__
# define __A_HPP__

#include <iostream>
#include "Middle.hpp"

class A: public Middle {
	public:
		A();
		A(const A&) = delete;
		~A();
		A&	operator= (const A&) = delete; // const for safety... not super nesessary
		void msg();
};

std::ostream&	operator<<(std::ostream&, const A&);

#endif

