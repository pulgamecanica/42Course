//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __BASE_HPP__
# define __BASE_HPP__

#include <iostream>

class Base {
	protected:
		Base();
	public:
		Base(const Base&) = delete;
		virtual~ Base();
		Base&	operator= (const Base&) = delete; // const for safety... not super nesessary
		virtual void msg();
};

std::ostream&	operator<<(std::ostream&, const Base&);

#endif

