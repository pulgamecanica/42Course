//***************************//
//*Template by pulgamecanica*//
//***************************//

#ifndef __VMIDDLE_HPP__
# define __VMIDDLE_HPP__

#include <iostream>

#include "Base.hpp"

class VMiddle: public virtual Base {
	protected:
		VMiddle();
	public:
		VMiddle(const VMiddle&) = delete;
		virtual ~VMiddle();
		VMiddle&	operator= (const VMiddle&) = delete; // const for safety... not super nesessary
		void msg();
};

std::ostream&	operator<<(std::ostream&, const VMiddle&);

#endif

